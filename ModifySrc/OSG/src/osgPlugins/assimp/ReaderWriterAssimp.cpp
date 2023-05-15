#include <osg/Notify>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Texture2D>
#include <osg/TexEnv>
#include <osg/Material>
#include <osg/PolygonMode>
#include <osg/MatrixTransform>

#include <osgDB/FileNameUtils>
#include <osgDB/FileUtils>
#include <osgDB/fstream>
#include <osgDB/ReadFile>
#include <osgDB/Registry>

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

static std::string getFullPath(const std::string& relativeTo, const std::string& relativePath)
{
    if (osgDB::isAbsolutePath(relativePath) || relativeTo.empty())
    {
        return relativePath;
    }

    std::string filename;
    if (!osgDB::containsServerAddress(relativeTo))
    {
        filename = osgDB::concatPaths(osgDB::getFilePath(osgDB::getRealPath(relativeTo)), relativePath);
    }
    else
    {
        filename = osgDB::concatPaths(osgDB::getFilePath(relativeTo), relativePath);
    }

    std::list<std::string> directories;
    int start = 0;
    for (unsigned int i = 0; i < filename.size(); ++i)
    {
        if (filename[i] == '\\' || filename[i] == '/')
        {
            /// 获取当前路径
            std::string dir = filename.substr(start, i - start);
            if (dir != "..")
            {
                if (dir != ".")
                {
                    directories.push_back(dir);
                }
            }
            else if (!directories.empty())
            {
                directories.pop_back();
            }
            start = i + 1;
        }
    }

    std::string path;
    for (std::list<std::string>::iterator itr = directories.begin();
        itr != directories.end(); ++itr)
    {
        path += *itr;
        path += "/";
    }
    path += filename.substr(start, std::string::npos);
    return path;
}

class ReaderWriterAssimp : public osgDB::ReaderWriter
{
    typedef std::map<std::string, osg::ref_ptr<osg::Texture> > TextureMap;
    typedef std::map<std::string, osg::ref_ptr<osg::Image> > ImageMap;

    public:

        ReaderWriterAssimp()
        {
            supportsExtension("ass","assimp format");
        }

        virtual const char* className() const { return "assimp format"; }

        virtual ReadResult readObject(const std::string& fileName, const osgDB::ReaderWriter::Options* options) const
        {
            return readNode(fileName, options);
        }

        virtual ReadResult readNode(const std::string& file, const osgDB::ReaderWriter::Options* options) const
        {
            std::string ext = osgDB::getLowerCaseFileExtension(file);
            if (!acceptsExtension(ext)) return ReadResult::FILE_NOT_HANDLED;

            /// 去掉后缀
            std::string tmpName = osgDB::getNameLessExtension(file);
            if (tmpName.empty())
                return ReadResult::FILE_NOT_HANDLED;

            ext = osgDB::getLowerCaseFileExtension(tmpName);
            Assimp::Importer import;

            if (!import.IsExtensionSupported(ext))
                return ReadResult::FILE_NOT_HANDLED;

            /// 读取文件
            const aiScene* scene = import.ReadFile(tmpName, aiProcessPreset_TargetRealtime_Quality);

            ///检查场景是否有效
            if (nullptr == scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE
                || !scene->mRootNode || !scene->HasMeshes())
                return ReadResult::FILE_NOT_HANDLED;
            ImageMap _embedImage;
            for (unsigned int i = 0; i < scene->mNumTextures; ++i)
            {
                std::string sFileName(scene->mTextures[i]->mFilename.C_Str());
                osg::ref_ptr<osg::Image> image;
                if (0 == scene->mTextures[i]->mHeight)
                {
                    osgDB::ReaderWriter* reader = osgDB::Registry::instance()->getReaderWriterForExtension(scene->mTextures[i]->achFormatHint);
                    if (reader)
                    {
                        std::stringstream inputStream;
                        inputStream.write((const char*)scene->mTextures[i]->pcData, scene->mTextures[i]->mWidth);

                        osgDB::ReaderWriter::ReadResult rr = reader->readImage(inputStream);
                        if (rr.validImage())
                        {
                            image = rr.takeImage();
                        }
                    }
                }
                else
                {
                    image = new osg::Image;
                    if (scene->mTextures[i]->achFormatHint == "rgba8888")
                    {

                    }
                }
                _embedImage[sFileName] = image;
            }
            ///构建osg场景
            osg::Group* pRoot = new osg::Group;
            processNode(tmpName,scene->mRootNode, scene, pRoot, options, _embedImage);

            /// 释放资源
            import.FreeScene();
            return(pRoot);
        }

        void processNode(const std::string& sFileName,const aiNode* pNode, const aiScene* pScene,
            osg::Group* pGroup, const osgDB::ReaderWriter::Options* options, const ImageMap& imageMap) const
        {
            aiMatrix4x4 m = pNode->mTransformation;
            m.Transpose();

            /// 获取节点的矩阵
            osg::ref_ptr<osg::MatrixTransform> mt;
            mt = new osg::MatrixTransform;
            mt->setMatrix(osg::Matrixf((float*)&m));

            osg::ref_ptr<osg::Geode> pGeode = new osg::Geode;
            for (GLuint i = 0; i < pNode->mNumMeshes; i++)
            {
                aiMesh* pMesh = pScene->mMeshes[pNode->mMeshes[i]];
                if (pMesh->HasPositions())
                {
                    osg::Geometry* pGeometry = new osg::Geometry;
                    pGeode->addDrawable(pGeometry);
                    processMesh(sFileName,pMesh, pScene, pGeometry, options, imageMap);
                }
            }
            mt->addChild(pGeode);
            pGroup->addChild(mt);

            // After we've processed all of the meshes (if any) we then recursively process each of the children nodes
            for (GLuint i = 0; i < pNode->mNumChildren; i++)
            {
                this->processNode(sFileName,pNode->mChildren[i], pScene, mt, options,imageMap);
            }
        }

        void processMesh(const std::string& sFileName, const aiMesh* pMesh, const aiScene* pScene,
            osg::Geometry* pGeometry, const osgDB::ReaderWriter::Options* options,const ImageMap& imageMap) const
        {
            osg::Vec3Array* pVertexArry = new osg::Vec3Array(pMesh->mNumVertices), 
                *pNormalArry = nullptr;
            pGeometry->setVertexArray(pVertexArry);

            /// 如果有法线，则开辟法线空间
            if (pMesh->HasNormals())
            {
                pNormalArry = new osg::Vec3Array(pMesh->mNumVertices);
                pGeometry->setNormalArray(pNormalArry, osg::Array::BIND_PER_VERTEX);
            }

            bool bHadColor = nullptr != pMesh->mColors[0];
            /// 如果有颜色，则开辟颜色空间
            osg::Vec4Array* pColorArry = bHadColor ? new osg::Vec4Array(pMesh->mNumVertices) : nullptr;
            
            if (bHadColor)
            {
                pGeometry->setColorArray(pColorArry,osg::Array::BIND_PER_VERTEX);
            }

            for (GLuint i = 0; i < pMesh->mNumVertices; i++)
            {
                const aiVector3D& v = pMesh->mVertices[i];
                pVertexArry->at(i).set(v.x, v.y, v.z);
                if(nullptr != pNormalArry)
                {
                    const aiVector3D& n = pMesh->mNormals[i];
                    pNormalArry->at(i).set(n.x, n.y, n.z);
                }

                ///处理颜色
                if(bHadColor)
                {
                    const aiColor4D& c = pMesh->mColors[0][i];
                    pColorArry->at(i).set(c.r, c.g, c.b, c.a);
                }
            }

            /// 创建纹理坐标
            unsigned int unit = 0;
            const aiVector3D* aiTexCoords = pMesh->mTextureCoords[unit];
            while (aiTexCoords != NULL)
            {
                switch (pMesh->mNumUVComponents[unit])
                {
                case 1:
                {
                    osg::FloatArray* ta = new osg::FloatArray(pMesh->mNumVertices);
                    for (unsigned int i = 0; i < pMesh->mNumVertices; ++i)
                        (*ta)[i] = aiTexCoords[i].x;
                    pGeometry->setTexCoordArray(unit, ta);
                }
                break;
                case 2:
                {
                    osg::Vec2Array* ta = new osg::Vec2Array(pMesh->mNumVertices);
                    for (unsigned int i = 0; i < pMesh->mNumVertices; ++i)
                    {
                        const aiVector3D& t = aiTexCoords[i];
                        (*ta)[i].set(t.x, t.y);
                    }
                    pGeometry->setTexCoordArray(unit, ta);
                }
                break;
                case 3:
                {
                    osg::Vec3Array* ta = new osg::Vec3Array(pMesh->mNumVertices);
                    for (unsigned int i = 0; i < pMesh->mNumVertices; ++i)
                    {
                        const aiVector3D& t = aiTexCoords[i];
                        (*ta)[i].set(t.x, t.y, t.z);
                    }
                    pGeometry->setTexCoordArray(unit, ta);
                }
                break;
                }
                aiTexCoords = pMesh->mTextureCoords[++unit];
            }

            /// 构建显示图元
            osg::ref_ptr<osg::DrawElementsUInt> de[5];
            de[1] = new osg::DrawElementsUInt(GL_POINTS);
            de[2] = new osg::DrawElementsUInt(GL_LINES);
            de[3] = new osg::DrawElementsUInt(GL_TRIANGLES);
            de[4] = new osg::DrawElementsUInt(GL_QUADS);
            de[0] = new osg::DrawElementsUInt(GL_POLYGON);

            osg::DrawElementsUInt* current = NULL;
            for (unsigned int f = 0; f < pMesh->mNumFaces; ++f)
            {
                const struct aiFace& face = pMesh->mFaces[f];
                if (face.mNumIndices > 4)
                {
                    current = de[0].get();
                }
                else
                {
                    current = de[face.mNumIndices].get();
                }

                for (unsigned i = 0; i < face.mNumIndices; ++i)
                {
                    current->push_back(face.mIndices[i]);
                }
            }

            for (unsigned int i = 0; i < 5; ++i)
            {
                if (de[i]->size() > 0)
                {
                    pGeometry->addPrimitiveSet(de[i].get());
                }
            }

            /// 构造纹理
            osg::StateSet* ss = pGeometry->getOrCreateStateSet();
            const aiMaterial* aiMtl = pScene->mMaterials[pMesh->mMaterialIndex];
            aiReturn texFound = AI_SUCCESS;
            aiTextureOp envOp = aiTextureOp_Multiply;
            aiTextureMapMode wrapMode[3] = { aiTextureMapMode_Wrap };
            unsigned int texIndex = 0;
            aiString path;
            TextureMap textures;
            while (texFound == AI_SUCCESS)
            {
                texFound = aiMtl->GetTexture(
                    aiTextureType_DIFFUSE, texIndex++, &path, NULL, &unit, NULL, &envOp, &(wrapMode[0]));
                if (unit > 0) unit--;  // The output UV seems to start at 1?
                if (texFound != AI_SUCCESS) break;

                
                std::string texFile(path.data);
                osg::Image* pImage = nullptr;
                ImageMap::const_iterator itor = imageMap.find(texFile);
                if (imageMap.end() == imageMap.find(texFile))
                {
                    if (!osgDB::isAbsolutePath(texFile))
                    {
                        texFile = getFullPath(sFileName, texFile);
                    }

                    pImage = osgDB::readImageFile(texFile, options);
                }
                else
                {
                    pImage = itor->second;
                }

                if (nullptr != pImage)
                {
                    auto itr = textures.find(texFile);
                    if (itr == textures.end())
                    {
                        osg::ref_ptr<osg::Texture2D> tex2D = new osg::Texture2D;
                        tex2D->setWrap(osg::Texture::WRAP_S, getWrapMode(wrapMode[0]));
                        tex2D->setWrap(osg::Texture::WRAP_T, getWrapMode(wrapMode[1]));
                        tex2D->setWrap(osg::Texture::WRAP_R, getWrapMode(wrapMode[2]));
                        tex2D->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR_MIPMAP_LINEAR);
                        tex2D->setFilter(osg::Texture::MAG_FILTER, osg::Texture::LINEAR);
                        tex2D->setImage(pImage);
                        textures[texFile] = tex2D;
                    }

                    ss->setTextureAttributeAndModes(unit, textures[texFile].get());
                    if (unit > 0)
                    {
                        ss->setTextureAttributeAndModes(unit, new osg::TexEnv(getEnvMode(envOp)));
                    }
                }
            }
            /// 添加材质
            createMaterialData(ss, aiMtl);
        }

        void createMaterialData(osg::StateSet* ss, const aiMaterial* aiMtl) const
        {
            aiColor4D c;
            osg::Material* material = new osg::Material;
            if (aiGetMaterialColor(aiMtl, AI_MATKEY_COLOR_AMBIENT, &c) == AI_SUCCESS)
            {
                material->setAmbient(osg::Material::FRONT_AND_BACK, osg::Vec4(c.r, c.g, c.b, c.a));
            }

            if (aiGetMaterialColor(aiMtl, AI_MATKEY_COLOR_DIFFUSE, &c) == AI_SUCCESS)
            {
                material->setDiffuse(osg::Material::FRONT_AND_BACK, osg::Vec4(c.r, c.g, c.b, c.a));
            }

            if (aiGetMaterialColor(aiMtl, AI_MATKEY_COLOR_SPECULAR, &c) == AI_SUCCESS)
            {
                material->setSpecular(osg::Material::FRONT_AND_BACK, osg::Vec4(c.r, c.g, c.b, c.a));
            }

            if (aiGetMaterialColor(aiMtl, AI_MATKEY_COLOR_EMISSIVE, &c) == AI_SUCCESS)
            {
                material->setEmission(osg::Material::FRONT_AND_BACK, osg::Vec4(c.r, c.g, c.b, c.a));
            }

            unsigned int maxValue = 1;
            float shininess = 0.0f, strength = 1.0f;
            if (aiGetMaterialFloatArray(aiMtl, AI_MATKEY_SHININESS, &shininess, &maxValue) == AI_SUCCESS)
            {
                maxValue = 1;
                if (aiGetMaterialFloatArray(aiMtl, AI_MATKEY_SHININESS_STRENGTH, &strength, &maxValue) == AI_SUCCESS)
                {
                    shininess *= strength;
                }
                material->setShininess(osg::Material::FRONT_AND_BACK, shininess);
            }
            else
            {
                material->setShininess(osg::Material::FRONT_AND_BACK, 0.0f);
                material->setSpecular(osg::Material::FRONT_AND_BACK, osg::Vec4());
            }
            ss->setAttributeAndModes(material);

            int wireframe = 0; maxValue = 1;
            if (aiGetMaterialIntegerArray(aiMtl, AI_MATKEY_ENABLE_WIREFRAME, &wireframe, &maxValue) == AI_SUCCESS)
            {
                ss->setAttributeAndModes(new osg::PolygonMode(
                    osg::PolygonMode::FRONT_AND_BACK, wireframe ? osg::PolygonMode::LINE : osg::PolygonMode::FILL));
            }
        }

        osg::TexEnv::Mode getEnvMode(aiTextureOp mode) const
        {
            switch (mode)
            {
            case aiTextureOp_Multiply: return osg::TexEnv::MODULATE;
            case aiTextureOp_Add: return osg::TexEnv::ADD;
            case aiTextureOp_Subtract: return osg::TexEnv::DECAL;
            case aiTextureOp_SmoothAdd: case aiTextureOp_SignedAdd:
                return osg::TexEnv::ADD;
            }
            return osg::TexEnv::REPLACE;
        }

        osg::Texture::WrapMode getWrapMode(aiTextureMapMode mode) const
        {
            switch (mode)
            {
            case aiTextureMapMode_Wrap: return osg::Texture::REPEAT;
            case aiTextureMapMode_Clamp: return osg::Texture::CLAMP;
            case aiTextureMapMode_Decal: return osg::Texture::CLAMP_TO_BORDER;
            case aiTextureMapMode_Mirror: return osg::Texture::MIRROR;
            }
            return osg::Texture::CLAMP;
        }

        virtual WriteResult writeNode(const osg::Node& node,const std::string& fileName,const Options* /*options*/ =NULL) const
        {
            std::string ext = osgDB::getLowerCaseFileExtension(fileName);
            if( !acceptsExtension(ext)) return WriteResult::FILE_NOT_HANDLED;

            osgDB::ofstream f(fileName.c_str());

            return WriteResult::FILE_SAVED;
        }
};

// now register with Registry to instantiate the above
// reader/writer.
REGISTER_OSGPLUGIN(ass, ReaderWriterAssimp)
