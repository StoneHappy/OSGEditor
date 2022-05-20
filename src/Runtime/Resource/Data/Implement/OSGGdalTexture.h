#pragma once 
#include <string>

#include <osg/Geode>
#include <osg/Texture2D>

#include "Resource/Data/Implement/VCGMesh.h"
namespace Soarscape
{
	class OSGGdalTexture
	{
	public:

		enum class ImageType
		{
			HeightField = 0,
			Image
		};

		OSGGdalTexture(const std::string& filename, ImageType type);

		osg::ref_ptr<osg::Geode> getOSGGeode();

		int width, height;

		void setRed();
		void setGreen();
		void setBlue();
		void setRGBA();
	private:

		void loadHeightField(const std::string& filename);
		void loadImage(const std::string& filename);

		
		osg::ref_ptr<osg::Geode> m_Geode;

		osg::ref_ptr<osg::Image> m_ImageRGBA;
		osg::ref_ptr<osg::Image> m_ImageRed;
		osg::ref_ptr<osg::Image> m_ImageGreen;
		osg::ref_ptr<osg::Image> m_ImageBlue;
		osg::ref_ptr<osg::Texture2D> m_Texture2DRGBA;
		osg::ref_ptr<osg::Texture2D> m_Texture2DRed;
		osg::ref_ptr<osg::Texture2D> m_Texture2DGreen;
		osg::ref_ptr<osg::Texture2D> m_Texture2DBlue;

		unsigned char** m_Data;
		unsigned char* m_PackedDataRGBA;
		unsigned char* m_PackedDataRed;
		unsigned char* m_PackedDataGreen;
		unsigned char* m_PackedDataBlue;

		VCGMesh m_GridMesh;
	};
}