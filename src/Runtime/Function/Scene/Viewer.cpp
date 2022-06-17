#include "Viewer.h"
#include <osg/ShapeDrawable>
#include <osg/Material>
#include <osgGA/TrackballManipulator>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
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
#if 0
        auto node = osgDB::readNodeFile("D:/codes/OSGEditor/out/bin/test.osgb");
        m_RootGroup->addChild(node);
        m_Viewer->setSceneData(m_RootGroup);
        setupCommonGeode();
#endif
#if 1
        auto node = osgDB::readNodeFile("D:/data/osgb/Tile_0015_0047_0022_DC.osgb");
        auto geode = node->asGeode();
        auto drawable = geode->getDrawable(0);
        osg::Texture2D* texture = new osg::Texture2D;
        const char* filename = "D:/data/imgs/Tile_0015_0047_0022_DC_0000.heic";
        auto osgimage = osgDB::readImageFile(filename);
        texture->setImage(osgimage);
        drawable->getOrCreateStateSet()->setTextureAttributeAndModes(0, texture, osg::StateAttribute::ON);
        m_RootGroup->addChild(node);
		m_Viewer->setSceneData(m_RootGroup);
        setupCommonGeode();
        osg::ref_ptr<osgDB::ReaderWriter::Options> options = new osgDB::ReaderWriter::Options;
        options->setOptionString("WriteImageHint=IncludeFile");   // Export option: Hint of writing image to stream: <IncludeData> writes Image::data() directly; <IncludeFile> writes the image file itself to stream; <UseExternal> writes 
        osgDB::writeNodeFile(*node, "test.osgb", options);
        osgDB::writeNodeFile(*node, "test.osgt", options);
#endif
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