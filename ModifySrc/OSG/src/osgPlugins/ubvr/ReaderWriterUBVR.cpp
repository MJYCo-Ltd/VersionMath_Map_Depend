#include <windows.h>
#include <osgDB/FileNameUtils>
#include <osgDB/FileUtils>
#include <osgDB/Registry>
#include <osgDB/ObjectWrapper>
#include <stdlib.h>
#include<osg/Texture2D>
#include <osg/TexGen>
#include <osg/Texture2D>
#include <osg/TexEnv>
#include<osgDB/ReadFile>
#include<osg/CullFace>


using namespace osgDB;

/**************************************************
//读(bool)：属性是否内嵌到此文件
//读(int)：构件元素个数
for (...)
{
	//读(int):构件名字符串长度
	//读(char[])：构件名字符串              

	//读(int)：属性字符串长度
	//读(char[])：属性字符串

	//读(float): 颜色R(0~1)
	//读(float): 颜色G(0~1)
	//读(float): 颜色B(0~1)
	//读(float): 颜色A(0~1)

	//读(int): 纹理资源路径字符串长度(长度为0表示没有纹理)
	//读(char[]) 纹理资源路径(相对.ubvr文件路径)

	//读(int):顶点数组元素个数            
	for (...)
	{
		//循环依次读取(float,float,float)：px,py,pz
	}

	//读(int):法向量数组元素个数
	for (...)
	{
		//循环依次读取(float,float,float)：nx,ny,nz
	}

	//读(int): 面片个数
	for (...)
	{
		//读(int):面片顶点个数(通常为3个)
		for (...)
		{
			//循环依次读取(int):顶点索引
		}

		//读(int):面片顶点矢量个数(通常同面片顶点个数)
		for (...)
		{
			//循环依次读取(int):法向量索引
		}
	}
}
**************************************************/

#define CHARSIZE 1
#define WCHARSIZE 2
#define SHORTSIZE 2
#define INTSIZE 4
#define FLOATSIZE 4
#define DOUBLESIZE 8

class ReaderWriterUBVR : public osgDB::ReaderWriter
{
public:
	ReaderWriterUBVR()
	{
		supportsExtension("ubvr", "ubvr format");
	}

	virtual const char* className() const { return "ubvr Format Reader/Writer"; }

	virtual ReadResult readNode(const std::string& file, const Options* options) const
	{
		osgDB::ifstream istream(file.c_str(), std::ios::in | std::ios::binary);

		if (!istream)
		{
			istream.close();
			return ReadResult::FILE_NOT_FOUND;
		}

		osg::Group* root = new osg::Group;

		//读(int)： 文件版本号
		int version = 0;
		istream.read((char*)&version, INTSIZE);

		//读(bool)：属性是否内嵌到此文件
		bool propertyIsContained = false;
		istream.read((char*)&propertyIsContained, CHARSIZE);

		//读(int)：构件元素个数
		int eleNum = 0;
		istream.read((char*)&eleNum, INTSIZE);
		for (int index = 0; index < eleNum; index++)
		{
			osg::Geode* geode = new osg::Geode;
			root->addChild(geode);
			osg::Geometry* geom = new osg::Geometry;
			geode->addDrawable(geom);
			//读(int):构件名字符串长度
			int nameLen = 0;
			std::string name;
			istream.read((char*)&nameLen, INTSIZE);
			//读(char[])：构件名字符串 
			name.resize(nameLen);
			istream.read((char*)name.c_str(), nameLen);
			geode->setName(name);
			//读(int)：属性字符串长度
			int propertyLen = 0;
			std::string property;
			istream.read((char*)&propertyLen, INTSIZE);
			//读(char[])：属性字符串
			property.resize(propertyLen);
			istream.read((char*)property.c_str(), propertyLen);

			//读(float): 颜色R(0~1)
			//读(float): 颜色G(0~1)
			//读(float): 颜色B(0~1)
			//读(float): 颜色A(0~1)
			float r = 0;
			float g = 0;
			float b = 0;
			float a = 0;

			istream.read((char*)&r, FLOATSIZE);
			istream.read((char*)&g, FLOATSIZE);
			istream.read((char*)&b, FLOATSIZE);
			istream.read((char*)&a, FLOATSIZE);

			osg::Vec4Array* colors = new osg::Vec4Array;
			colors->push_back(osg::Vec4(r, g, b, a));
			geom->setColorArray(colors);
			geom->setColorBinding(osg::Geometry::BIND_OVERALL);

			if (a > 0 && a < 1)
			{
				geom->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON);
				geom->getOrCreateStateSet()->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
				geom->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
				
				osg::ref_ptr<osg::CullFace> cullface = new osg::CullFace(osg::CullFace::BACK);
				geom->getOrCreateStateSet()->setAttribute(cullface.get());
				geom->getOrCreateStateSet()->setMode(GL_CULL_FACE, osg::StateAttribute::ON);
			}

			//读(bool)：是否使用了纹理
			bool useTex = false;
			istream.read((char*)&useTex, CHARSIZE);
			if (useTex)
			{
				//读(int): 纹理资源路径字符串长度(长度为0表示没有纹理)
				int texPathLen = 0;
				istream.read((char*)&texPathLen, INTSIZE);

				//读(char[]) 纹理资源路径(相对.ubvr文件路径)
				std::string texPath;
				texPath.resize(texPathLen);
				istream.read((char*)texPath.c_str(), texPathLen);

				osg::ref_ptr<osg::Image> image = osgDB::readImageFile(osgDB::getFilePath(file) + "/" + texPath);
				
				//创建二维纹理
				osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D;
				//设置边界处理
				texture->setWrap(osg::Texture2D::WRAP_S, osg::Texture2D::REPEAT);
				texture->setWrap(osg::Texture2D::WRAP_T, osg::Texture2D::REPEAT);
				texture->setWrap(osg::Texture2D::WRAP_R, osg::Texture2D::REPEAT);
				//设置滤波
				texture->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR);
				texture->setFilter(osg::Texture::MAG_FILTER, osg::Texture::NEAREST);
				//设置贴图
				texture->setImage(image.get());

				//创建属性集
				osg::ref_ptr<osg::StateSet> stateset = geode->getOrCreateStateSet();

				//启用二维纹理
				stateset->setTextureAttribute(0, texture.get(), osg::StateAttribute::OVERRIDE);
				stateset->setTextureMode(0, GL_TEXTURE_2D, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
			}

			/*

			//设置自动纹理坐标，并指定相关的平面
			osg::ref_ptr<osg::TexGen> texgen = new osg::TexGen;
			texgen->setMode(osg::TexGen::OBJECT_LINEAR);
			texgen->setPlane(osg::TexGen::S, osg::Plane(0.0f, 0.0f, 1.0f, 0.0f));

			*/

			//读(int):顶点数组元素个数
			int posSize = 0;
			istream.read((char*)&posSize, INTSIZE);

			osg::Vec3Array* osgPts = new osg::Vec3Array;
			std::vector<osg::Vec3> ptsVec;
			for (int posIndex = 0; posIndex < posSize; posIndex++)
			{
				//循环依次读取(float,float,float)：px,py,pz
				float px, py, pz;
				istream.read((char*)&px, FLOATSIZE);
				istream.read((char*)&py, FLOATSIZE);
				istream.read((char*)&pz, FLOATSIZE);
				ptsVec.push_back(osg::Vec3(px, py, pz));
			}


			//读(int):法向量数组元素个数
			int normalSize = 0;
			istream.read((char*)&normalSize, INTSIZE);

			osg::Vec3Array* osgNrmal = new osg::Vec3Array;
			std::vector<osg::Vec3> norVec;
			for (int normalIdx = 0; normalIdx < normalSize; normalIdx++)
			{
				//循环依次读取(float,float,float)：nx,ny,nz
				float nx, ny, nz;
				istream.read((char*)&nx, FLOATSIZE);
				istream.read((char*)&ny, FLOATSIZE);
				istream.read((char*)&nz, FLOATSIZE);
				norVec.push_back(osg::Vec3(nx, ny, nz));
			}

			//使用了纹理才读
			std::vector<osg::Vec2> uvVec;
			osg::Vec2Array* osgUvs = new osg::Vec2Array;
			if (useTex)
			{
				//读(int):纹理数组元素个数
				int uvSize = 0;
				istream.read((char*)&uvSize, INTSIZE);
				osg::Vec2Array* uvs = new osg::Vec2Array;
				for (int idx = 0; idx < uvSize; idx++)
				{
					//循环依次读取(float,float)：u,v
					osg::Vec2 uv;
					istream.read((char*)&uv.x(), FLOATSIZE);
					istream.read((char*)&uv.y(), FLOATSIZE);
					uvVec.push_back(uv);
				}
			}

			//读(int): 面片个数
			int faceSize = 0;
			istream.read((char*)&faceSize, INTSIZE);
			osg::ref_ptr<osg::UIntArray> array = new osg::UIntArray;
			for (int faceIdx = 0; faceIdx < faceSize; faceIdx++)
			{
				//读(int):面片顶点个数(通常为3个)
				int ptSize = 0;
				istream.read((char*)&ptSize, INTSIZE);

				int triIdx1, triIdx2, triIdx3;

				//循环依次读取(int):顶点索引
				istream.read((char*)&triIdx1, INTSIZE);
				istream.read((char*)&triIdx2, INTSIZE);
				istream.read((char*)&triIdx3, INTSIZE);


				//array->push_back(triIdx1);
				//array->push_back(triIdx2);
				//array->push_back(triIdx3);

				osgPts->push_back(ptsVec.at(triIdx1));
				osgPts->push_back(ptsVec.at(triIdx2));
				osgPts->push_back(ptsVec.at(triIdx3));

				//读(int):面片顶点矢量个数(通常同面片顶点个数)
				int norSize = 0;
				istream.read((char*)&norSize, INTSIZE);
				for (int norIdx = 0; norIdx < norSize; norIdx++)
				{
					//循环依次读取(int):法向量索引
					int nor;
					istream.read((char*)&nor, INTSIZE);
					osgNrmal->push_back(norVec.at(nor));
				}

				//使用了纹理才读
				if (useTex)
				{
					//读(int):面片顶点矢量个数(通常同面片顶点个数)
					int norSize = 0;
					istream.read((char*)&norSize, INTSIZE);
					for (int norIdx = 0; norIdx < norSize; norIdx++)
					{
						//循环依次读取(int):法向量索引
						int nor;
						istream.read((char*)&nor, INTSIZE);
						osgUvs->push_back(uvVec.at(nor));
					}
				}
			}

			if (useTex)
			{
				geom->setTexCoordArray(0, osgUvs);
			}

			geom->setVertexArray(osgPts);
			geom->setNormalArray(osgNrmal);
			geom->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);

			geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLES, 0, osgPts->size()));
			//geom->addPrimitiveSet(new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLES, array->size(), &array->front()));

		}

		return root;
	}
};

REGISTER_OSGPLUGIN(ubvr, ReaderWriterUBVR)
