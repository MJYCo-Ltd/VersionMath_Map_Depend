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
//��(bool)�������Ƿ���Ƕ�����ļ�
//��(int)������Ԫ�ظ���
for (...)
{
	//��(int):�������ַ�������
	//��(char[])���������ַ���              

	//��(int)�������ַ�������
	//��(char[])�������ַ���

	//��(float): ��ɫR(0~1)
	//��(float): ��ɫG(0~1)
	//��(float): ��ɫB(0~1)
	//��(float): ��ɫA(0~1)

	//��(int): ������Դ·���ַ�������(����Ϊ0��ʾû������)
	//��(char[]) ������Դ·��(���.ubvr�ļ�·��)

	//��(int):��������Ԫ�ظ���            
	for (...)
	{
		//ѭ�����ζ�ȡ(float,float,float)��px,py,pz
	}

	//��(int):����������Ԫ�ظ���
	for (...)
	{
		//ѭ�����ζ�ȡ(float,float,float)��nx,ny,nz
	}

	//��(int): ��Ƭ����
	for (...)
	{
		//��(int):��Ƭ�������(ͨ��Ϊ3��)
		for (...)
		{
			//ѭ�����ζ�ȡ(int):��������
		}

		//��(int):��Ƭ����ʸ������(ͨ��ͬ��Ƭ�������)
		for (...)
		{
			//ѭ�����ζ�ȡ(int):����������
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

		//��(int)�� �ļ��汾��
		int version = 0;
		istream.read((char*)&version, INTSIZE);

		//��(bool)�������Ƿ���Ƕ�����ļ�
		bool propertyIsContained = false;
		istream.read((char*)&propertyIsContained, CHARSIZE);

		//��(int)������Ԫ�ظ���
		int eleNum = 0;
		istream.read((char*)&eleNum, INTSIZE);
		for (int index = 0; index < eleNum; index++)
		{
			osg::Geode* geode = new osg::Geode;
			root->addChild(geode);
			osg::Geometry* geom = new osg::Geometry;
			geode->addDrawable(geom);
			//��(int):�������ַ�������
			int nameLen = 0;
			std::string name;
			istream.read((char*)&nameLen, INTSIZE);
			//��(char[])���������ַ��� 
			name.resize(nameLen);
			istream.read((char*)name.c_str(), nameLen);
			geode->setName(name);
			//��(int)�������ַ�������
			int propertyLen = 0;
			std::string property;
			istream.read((char*)&propertyLen, INTSIZE);
			//��(char[])�������ַ���
			property.resize(propertyLen);
			istream.read((char*)property.c_str(), propertyLen);

			//��(float): ��ɫR(0~1)
			//��(float): ��ɫG(0~1)
			//��(float): ��ɫB(0~1)
			//��(float): ��ɫA(0~1)
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

			//��(bool)���Ƿ�ʹ��������
			bool useTex = false;
			istream.read((char*)&useTex, CHARSIZE);
			if (useTex)
			{
				//��(int): ������Դ·���ַ�������(����Ϊ0��ʾû������)
				int texPathLen = 0;
				istream.read((char*)&texPathLen, INTSIZE);

				//��(char[]) ������Դ·��(���.ubvr�ļ�·��)
				std::string texPath;
				texPath.resize(texPathLen);
				istream.read((char*)texPath.c_str(), texPathLen);

				osg::ref_ptr<osg::Image> image = osgDB::readImageFile(osgDB::getFilePath(file) + "/" + texPath);
				
				//������ά����
				osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D;
				//���ñ߽紦��
				texture->setWrap(osg::Texture2D::WRAP_S, osg::Texture2D::REPEAT);
				texture->setWrap(osg::Texture2D::WRAP_T, osg::Texture2D::REPEAT);
				texture->setWrap(osg::Texture2D::WRAP_R, osg::Texture2D::REPEAT);
				//�����˲�
				texture->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR);
				texture->setFilter(osg::Texture::MAG_FILTER, osg::Texture::NEAREST);
				//������ͼ
				texture->setImage(image.get());

				//�������Լ�
				osg::ref_ptr<osg::StateSet> stateset = geode->getOrCreateStateSet();

				//���ö�ά����
				stateset->setTextureAttribute(0, texture.get(), osg::StateAttribute::OVERRIDE);
				stateset->setTextureMode(0, GL_TEXTURE_2D, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
			}

			/*

			//�����Զ��������꣬��ָ����ص�ƽ��
			osg::ref_ptr<osg::TexGen> texgen = new osg::TexGen;
			texgen->setMode(osg::TexGen::OBJECT_LINEAR);
			texgen->setPlane(osg::TexGen::S, osg::Plane(0.0f, 0.0f, 1.0f, 0.0f));

			*/

			//��(int):��������Ԫ�ظ���
			int posSize = 0;
			istream.read((char*)&posSize, INTSIZE);

			osg::Vec3Array* osgPts = new osg::Vec3Array;
			std::vector<osg::Vec3> ptsVec;
			for (int posIndex = 0; posIndex < posSize; posIndex++)
			{
				//ѭ�����ζ�ȡ(float,float,float)��px,py,pz
				float px, py, pz;
				istream.read((char*)&px, FLOATSIZE);
				istream.read((char*)&py, FLOATSIZE);
				istream.read((char*)&pz, FLOATSIZE);
				ptsVec.push_back(osg::Vec3(px, py, pz));
			}


			//��(int):����������Ԫ�ظ���
			int normalSize = 0;
			istream.read((char*)&normalSize, INTSIZE);

			osg::Vec3Array* osgNrmal = new osg::Vec3Array;
			std::vector<osg::Vec3> norVec;
			for (int normalIdx = 0; normalIdx < normalSize; normalIdx++)
			{
				//ѭ�����ζ�ȡ(float,float,float)��nx,ny,nz
				float nx, ny, nz;
				istream.read((char*)&nx, FLOATSIZE);
				istream.read((char*)&ny, FLOATSIZE);
				istream.read((char*)&nz, FLOATSIZE);
				norVec.push_back(osg::Vec3(nx, ny, nz));
			}

			//ʹ��������Ŷ�
			std::vector<osg::Vec2> uvVec;
			osg::Vec2Array* osgUvs = new osg::Vec2Array;
			if (useTex)
			{
				//��(int):��������Ԫ�ظ���
				int uvSize = 0;
				istream.read((char*)&uvSize, INTSIZE);
				osg::Vec2Array* uvs = new osg::Vec2Array;
				for (int idx = 0; idx < uvSize; idx++)
				{
					//ѭ�����ζ�ȡ(float,float)��u,v
					osg::Vec2 uv;
					istream.read((char*)&uv.x(), FLOATSIZE);
					istream.read((char*)&uv.y(), FLOATSIZE);
					uvVec.push_back(uv);
				}
			}

			//��(int): ��Ƭ����
			int faceSize = 0;
			istream.read((char*)&faceSize, INTSIZE);
			osg::ref_ptr<osg::UIntArray> array = new osg::UIntArray;
			for (int faceIdx = 0; faceIdx < faceSize; faceIdx++)
			{
				//��(int):��Ƭ�������(ͨ��Ϊ3��)
				int ptSize = 0;
				istream.read((char*)&ptSize, INTSIZE);

				int triIdx1, triIdx2, triIdx3;

				//ѭ�����ζ�ȡ(int):��������
				istream.read((char*)&triIdx1, INTSIZE);
				istream.read((char*)&triIdx2, INTSIZE);
				istream.read((char*)&triIdx3, INTSIZE);


				//array->push_back(triIdx1);
				//array->push_back(triIdx2);
				//array->push_back(triIdx3);

				osgPts->push_back(ptsVec.at(triIdx1));
				osgPts->push_back(ptsVec.at(triIdx2));
				osgPts->push_back(ptsVec.at(triIdx3));

				//��(int):��Ƭ����ʸ������(ͨ��ͬ��Ƭ�������)
				int norSize = 0;
				istream.read((char*)&norSize, INTSIZE);
				for (int norIdx = 0; norIdx < norSize; norIdx++)
				{
					//ѭ�����ζ�ȡ(int):����������
					int nor;
					istream.read((char*)&nor, INTSIZE);
					osgNrmal->push_back(norVec.at(nor));
				}

				//ʹ��������Ŷ�
				if (useTex)
				{
					//��(int):��Ƭ����ʸ������(ͨ��ͬ��Ƭ�������)
					int norSize = 0;
					istream.read((char*)&norSize, INTSIZE);
					for (int norIdx = 0; norIdx < norSize; norIdx++)
					{
						//ѭ�����ζ�ȡ(int):����������
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
