#include "Viewer.h"
#include <osg/ShapeDrawable>
#include <osg/Material>
#include <osgGA/TrackballManipulator>
#include <osgDB/ReadFile>
#include <osg/Texture2D>
#include <libheif/heif.h>
#include <vector>

#define LIBHEIF_TEST_ERROR(func, ...) if ((func) != heif_error_code::heif_error_Ok) {std::cout << __VA_ARGS__ << std::endl;}
namespace Soarscape
{
	Viewer::Viewer()
		: m_Viewer(new osgViewer::Viewer), m_RootGroup(new osg::Group), m_CommonGeode (new osg::Geode)
	{
        m_Viewer->setThreadingModel(osgViewer::Viewer::SingleThreaded);
        m_RootGroup->addChild(m_CommonGeode);
        auto node = osgDB::readNodeFile("D:/data/osgb/Tile_0015_0047_0022_DC.osgb");
        auto geode = node->asGeode();
        auto drawable = geode->getDrawable(0);
        osg::Texture2D* texture = new osg::Texture2D;
        const char* filename = "D:/data/imgs/Tile_0015_0047_0022_DC_0000.heic";
        heif_context* ctx = heif_context_alloc();
        LIBHEIF_TEST_ERROR(heif_context_read_from_file(ctx, filename, nullptr).code, "File read error!")

        // get a handle to the primary image
        heif_image_handle* handle;
        LIBHEIF_TEST_ERROR(heif_context_get_primary_image_handle(ctx, &handle).code, "Get image handle error")

        // decode the image and convert colorspace to RGB, saved as 24bit interleaved
        heif_image* img;
        LIBHEIF_TEST_ERROR(heif_decode_image(handle, &img, heif_colorspace_RGB, heif_chroma_interleaved_RGB, nullptr).code, "Decode image error")

        auto width = heif_image_get_primary_width(img);
        auto height = heif_image_get_primary_height(img);

        std::cout << "Get image info: " << std::endl;
        std::cout << "width: " << width << std::endl;
        std::cout << "height: " << height << std::endl;

        int stride;
        uint8_t* data = heif_image_get_plane(img, heif_channel_interleaved, &stride);
        uint8_t* reservedata = new uint8_t[stride * height];
        //memcpy(reservedata, data, sizeof(uint8_t) * stride * height);

        
        for (size_t i = 0; i < stride; i++)
        {
            for (size_t j = 0; j < height; j++)
            {
                int index1 = i + j * stride;
                int index2 = i + (height - 1 - j) * stride;
                reservedata[index1] = data[index2];
            }
        }
        /*for (size_t i = 0; i < height; i++)
        {
            for (size_t j = 0; j < width; j++)
            {
                size_t index1 = j + i * (width - 1);
                reservedata[index1] = data[index1];
            }
        }*/
        osg::Image* osgimage = new osg::Image;

        osgimage->setImage(width, height, 1, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE, reservedata, osg::Image::USE_NEW_DELETE);
        if (stride == NULL)
        {
            std::cout << "Get image data error!" << std::endl;
        }

        std::cout << "Read image successfully!" << std::endl;
        texture->setImage(osgimage);
        drawable->getOrCreateStateSet()->setTextureAttributeAndModes(0, texture, osg::StateAttribute::ON);
        m_RootGroup->addChild(node);
		m_Viewer->setSceneData(m_RootGroup);
        setupCommonGeode();
	}
    void Viewer::initialize(int x, int y, int width, int height)
    {
        m_Viewer->getCamera()->setViewport(0, 0, width, height);
        m_Viewer->getCamera()->setClearColor(osg::Vec4(0.9f, 0.9f, 1.f, 1.f));
        float aspectRatio = static_cast<float>(width) / static_cast<float>(height);
        m_Viewer->getCamera()->setProjectionMatrixAsPerspective(30.f, aspectRatio, 1.f, 1000.f);
        m_Viewer->getCamera()->setViewMatrixAsLookAt({ 0, 0, 5 }, { 0, 0, 0 }, {0, 1, 0});
        osgGA::TrackballManipulator* manipulator = new osgGA::TrackballManipulator;
        manipulator->setAllowThrow(false);
        m_Viewer->setCameraManipulator(manipulator);
    }
	void Viewer::addNode(osg::Node* node)
	{
		m_RootGroup->addChild(node);
	}
    void Viewer::addCommonDrawable(osg::Drawable* drawable)
    {
        m_CommonGeode->addChild(drawable);
    }
    void Viewer::addCustomGeode(osg::Geode* geode)
    {
        m_RootGroup->addChild(geode);
    }

    void Viewer::attacthContext(osgViewer::GraphicsWindow* context)
    {
        m_Viewer->getCamera()->setGraphicsContext(context);
        m_Viewer->realize();
    }

    void Viewer::attacthFrameBuffer(uint32_t fboid)
    {
        m_Viewer->getCamera()->getGraphicsContext()->setDefaultFboId(fboid);
    }

    void Viewer::tick()
    {
        m_Viewer->frame();
    }

    void Viewer::resize(int x, int y, int width, int height)
    {
        m_Viewer->getCamera()->setViewport(0, 0, width, height);
        float aspectRatio = static_cast<float>(width) / static_cast<float>(height);
        m_Viewer->getCamera()->setProjectionMatrixAsPerspective(30.f, aspectRatio, 1.f, 1000.f);
    }

    osg::Camera* Viewer::getCamera()
    {
        return m_Viewer->getCamera();
    }

    void Viewer::setupCommonGeode()
    {
        if (m_CommonGeode.get() == nullptr) return;
        
        // create a debug scene - cylinder
        /*osg::Cylinder* cylinder = new osg::Cylinder(osg::Vec3(0.f, 0.f, 0.f), 0.25f, 0.5f);
        osg::ShapeDrawable* sd = new osg::ShapeDrawable(cylinder);
        sd->setColor(osg::Vec4(0.8f, 0.5f, 0.2f, 1.f));
        m_CommonGeode->addDrawable(sd);*/
        osg::StateSet* stateSet = m_CommonGeode->getOrCreateStateSet();
        osg::Material* material = new osg::Material;
        material->setColorMode(osg::Material::AMBIENT_AND_DIFFUSE);
        stateSet->setAttributeAndModes(material, osg::StateAttribute::ON);
        stateSet->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);
    }
}