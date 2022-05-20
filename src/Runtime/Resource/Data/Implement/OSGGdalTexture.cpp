#include "OSGGdalTexture.h"
#include <osg/Material>

#include <osgDB/ReadFile>
#include <Core/Base/macro.h>

#include <gdal.h>
#include <gdal_priv.h>
#include <gdalwarper.h>
#include <ogrsf_frmts.h>

namespace Soarscape
{
	OSGGdalTexture::OSGGdalTexture(const std::string& filename, ImageType type)
		: m_Geode(new osg::Geode), m_ImageRGBA(new osg::Image), m_ImageRed(new osg::Image), m_ImageGreen(new osg::Image), m_ImageBlue(new osg::Image),
		m_Texture2DRGBA(new osg::Texture2D), m_Texture2DRed(new osg::Texture2D),m_Texture2DGreen(new osg::Texture2D), m_Texture2DBlue(new osg::Texture2D)
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
		m_Texture2DRGBA = new osg::Texture2D;
		if (m_ImageRGBA && m_ImageRGBA->valid()) { m_Texture2DRGBA->setImage(m_ImageRGBA); }

		// 创建叶子结点并指定属性
		m_Geode->getOrCreateStateSet()->setAttributeAndModes(material.get(), osg::StateAttribute::ON);
		m_Geode->getOrCreateStateSet()->setTextureAttributeAndModes(0, m_Texture2DRGBA.get(), osg::StateAttribute::ON);	// 开启设置图片纹理
		m_Geode->getOrCreateStateSet()->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);							// 开启深度测试(顺序渲染层级)
	}

	osg::ref_ptr<osg::Geode> OSGGdalTexture::getOSGGeode()
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
		width = GDALGetRasterXSize(poDataset);
		height = GDALGetRasterYSize(poDataset);

		GDALRasterBand* poBand;
		int             nBlockXSize, nBlockYSize;
		int             bGotMin, bGotMax;
		double          adfMinMax[2];
		int numbands = poDataset->GetRasterCount();

		m_Data = new unsigned char* [numbands];
		m_PackedDataRGBA = new unsigned char[4 * width * height];
		m_PackedDataRed = new unsigned char[4 * width * height];
		m_PackedDataGreen = new unsigned char[4 * width * height];
		m_PackedDataBlue = new unsigned char[4 * width * height];
		memset(m_PackedDataRGBA, 0, 4 * width * height * sizeof(unsigned char));
		memset(m_PackedDataRed, 0, 4 * width * height * sizeof(unsigned char));
		memset(m_PackedDataGreen, 0, 4 * width * height * sizeof(unsigned char));
		memset(m_PackedDataBlue, 0, 4 * width * height * sizeof(unsigned char));
		for (int i = 0; i < numbands; i++)
		{
			m_Data[i] = new unsigned char[width * height];
			GDALRasterBand* poBand;
			poBand = poDataset->GetRasterBand(i + 1);
			// Used unsiged ints for data type
			poBand->RasterIO(GF_Read, 0, 0, width, height, m_Data[i], width, height, GDT_Byte, 0, 0);
		}

		for (int i = 0; i < width * height; i++)
		{
			if (numbands == 4)
			{
				m_PackedDataRGBA[i * 4] = m_Data[0][i];
				m_PackedDataRGBA[i * 4 + 1] = m_Data[1][i];
				m_PackedDataRGBA[i * 4 + 2] = m_Data[2][i];
				m_PackedDataRGBA[i * 4 + 3] = m_Data[2][i];

				m_PackedDataRed[i * 4] = m_Data[0][i];
				m_PackedDataGreen[i * 4 + 1] = m_Data[1][i];
				m_PackedDataBlue[i * 4 + 2] = m_Data[2][i];
			}
			else if (numbands == 3)
			{
				m_PackedDataRGBA[i * 4] = m_Data[0][i];
				m_PackedDataRGBA[i * 4 + 1] = m_Data[1][i];
				m_PackedDataRGBA[i * 4 + 2] = m_Data[2][i];
				m_PackedDataRGBA[i * 4 + 3] = 255;

				m_PackedDataRed[i * 4] = m_Data[0][i];
				m_PackedDataGreen[i * 4 + 1] = m_Data[1][i];
				m_PackedDataBlue[i * 4 + 2] = m_Data[2][i];
			}
		}

		m_ImageRGBA->setImage(width, height, 1, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, m_PackedDataRGBA, osg::Image::USE_NEW_DELETE);
		m_ImageRed->setImage(width, height, 1, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, m_PackedDataRed, osg::Image::USE_NEW_DELETE);
		m_ImageGreen->setImage(width, height, 1, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, m_PackedDataGreen, osg::Image::USE_NEW_DELETE);
		m_ImageBlue->setImage(width, height, 1, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, m_PackedDataBlue, osg::Image::USE_NEW_DELETE);

		m_Texture2DRGBA->setImage(m_ImageRGBA);
		m_Texture2DRed->setImage(m_ImageRed);
		m_Texture2DGreen->setImage(m_ImageGreen);
		m_Texture2DBlue->setImage(m_ImageBlue);
	}

	void OSGGdalTexture::setRed()
	{
		m_Geode->getOrCreateStateSet()->setTextureAttributeAndModes(0, m_Texture2DRed.get(), osg::StateAttribute::ON);
	}
	void OSGGdalTexture::setGreen()
	{
		m_Geode->getOrCreateStateSet()->setTextureAttributeAndModes(0, m_Texture2DGreen.get(), osg::StateAttribute::ON);
	}
	void OSGGdalTexture::setBlue()
	{
		m_Geode->getOrCreateStateSet()->setTextureAttributeAndModes(0, m_Texture2DBlue.get(), osg::StateAttribute::ON);
	}
	void OSGGdalTexture::setRGBA()
	{
		m_Geode->getOrCreateStateSet()->setTextureAttributeAndModes(0, m_Texture2DRGBA.get(), osg::StateAttribute::ON);
	}
}
