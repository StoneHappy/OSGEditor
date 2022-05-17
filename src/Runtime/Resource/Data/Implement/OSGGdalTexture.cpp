#include "OSGGdalTexture.h"
#include <osg/Material>
#include <osg/Texture2D>

#include <osgDB/ReadFile>
#include <Core/Base/macro.h>

#include <gdal.h>
#include <gdal_priv.h>
#include <gdalwarper.h>
#include <ogrsf_frmts.h>

namespace Soarscape
{
	OSGGdalTexture::OSGGdalTexture(const std::string& filename, ImageType type)
		: m_Geode(new osg::Geode), m_Image(new osg::Image)
	{
		/*auto pointPos = filename.find_last_of(".");
		auto lPos = filename.find_last_of("/");
		std::string meshName = filename.substr(lPos + 1, pointPos - 1 - lPos);
		std::string extendName = filename.substr(pointPos + 1);

		if (extendName != "tif")
		{
			LOG_ERROR("Only support tif file");
			return;
		}*/

		if (type == ImageType::Image)
		{
			loadImage(filename);
		}
		
		// 创建材料属性
		osg::ref_ptr<osg::Material> material = new osg::Material;
		material->setAmbient(osg::Material::Face::FRONT_AND_BACK, osg::Vec4f(0.5, 1.0, 1.0, 1));	// 设置全景光
		material->setDiffuse(osg::Material::Face::FRONT_AND_BACK, osg::Vec4f(1.0, 1.0, 1.0, 1));	// 设置混合光
		material->setSpecular(osg::Material::Face::FRONT_AND_BACK, osg::Vec4f(1.0, 1.0, 1.0, 1));	// 设置反射
		material->setShininess(osg::Material::Face::FRONT_AND_BACK, 60);							// 设置反射类型

		/*osg::ref_ptr<osg::Texture2D> texture2D = new osg::Texture2D;
		osg::ref_ptr<osg::Image>	 image = osgDB::readImageFile(filename);*/
		osg::ref_ptr<osg::Texture2D> texture2D = new osg::Texture2D;
		if (m_Image && m_Image->valid()) { texture2D->setImage(m_Image); }

		// 创建叶子结点并指定属性
		m_Geode->getOrCreateStateSet()->setAttributeAndModes(material.get(), osg::StateAttribute::ON);
		m_Geode->getOrCreateStateSet()->setTextureAttributeAndModes(0, texture2D.get(), osg::StateAttribute::ON);	// 开启设置图片纹理
		m_Geode->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON);								// 开启透明度
		m_Geode->getOrCreateStateSet()->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);							// 开启深度测试(顺序渲染层级)
	}

	osg::ref_ptr<osg::Geode> OSGGdalTexture::getOsgGeode()
	{
		return m_Geode;
	}
	void OSGGdalTexture::loadHeightField(const std::string& filename)
	{

	}
	void OSGGdalTexture::loadImage(const std::string& filename)
	{
		GDALDataset* poDataset;
		GDALAllRegister();
		poDataset = (GDALDataset*)GDALOpen(filename.c_str(), GA_ReadOnly);
		if (poDataset == NULL)
		{
			LOG_ERROR("gdal can't open {0}", filename);
			return;
		}
		int width = GDALGetRasterXSize(poDataset);
		int height = GDALGetRasterYSize(poDataset);

		GDALRasterBand* poBand;
		int             nBlockXSize, nBlockYSize;
		int             bGotMin, bGotMax;
		double          adfMinMax[2];
		int numbands = poDataset->GetRasterCount();

		// yay stack allocation -- replace with dynamic in the future
		unsigned char** data;
		unsigned char* packeddata;

		data = new unsigned char* [numbands];
		packeddata = new unsigned char[4 * width * height];

		for (int i = 0; i < numbands; i++)
		{
			data[i] = new unsigned char[width * height];
			GDALRasterBand* poBand;
			poBand = poDataset->GetRasterBand(i + 1);
			// Used unsiged ints for data type
			poBand->RasterIO(GF_Read, 0, 0, width, height, data[i], width, height, GDT_Byte, 0, 0);
		}

		for (int i = 0; i < width * height; i++)
		{
			if (numbands == 4)
			{
				packeddata[i * 4] = data[0][i];
				packeddata[i * 4 + 1] = data[1][i];
				packeddata[i * 4 + 2] = data[2][i];
				packeddata[i * 4 + 3] = data[3][i];
			}
			else if (numbands == 3)
			{
				packeddata[i * 4] = data[0][i];
				packeddata[i * 4 + 1] = data[1][i];
				packeddata[i * 4 + 2] = data[2][i];
				packeddata[i * 4 + 3] = 255;
			}
		}

		osg::ref_ptr<osg::Image> image = new osg::Image();
		m_Image->setImage(width, height, 1, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, packeddata, osg::Image::USE_NEW_DELETE);

	}
}
