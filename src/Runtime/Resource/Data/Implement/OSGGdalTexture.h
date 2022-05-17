#pragma once 
#include <string>

#include <osg/Geode>
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

		osg::ref_ptr<osg::Geode> getOsgGeode();
	private:

		void loadHeightField(const std::string& filename);
		void loadImage(const std::string& filename);
		osg::ref_ptr<osg::Geode> m_Geode;

		osg::ref_ptr<osg::Image> m_Image;
	};
}