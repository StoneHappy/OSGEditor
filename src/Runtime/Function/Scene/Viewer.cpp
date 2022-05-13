#include "Viewer.h"
#include <osg/ShapeDrawable>
#include <osg/Material>
#include <osgGA/TrackballManipulator>
namespace Soarscape
{
	Viewer::Viewer()
		: m_Viewer(new osgViewer::Viewer), m_RootGroup(new osg::Group), m_CommonGeode (new osg::Geode)
	{
        m_Viewer->setThreadingModel(osgViewer::Viewer::SingleThreaded);
        m_RootGroup->addChild(m_CommonGeode);
		m_Viewer->setSceneData(m_RootGroup);
        setupCommonGeode();
	}
    void Viewer::initialize(int x, int y, int width, int height)
    {
        m_Viewer->getCamera()->setViewport(0, 0, width, height);
        m_Viewer->getCamera()->setClearColor(osg::Vec4(0.9f, 0.9f, 1.f, 1.f));
        float aspectRatio = static_cast<float>(width) / static_cast<float>(height);
        m_Viewer->getCamera()->setProjectionMatrixAsPerspective(30.f, aspectRatio, 1.f, 1000.f);
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

    void Viewer::setupCommonGeode()
    {
        if (m_CommonGeode.get() == nullptr) return;
        
        // create a debug scene - cylinder
        osg::Cylinder* cylinder = new osg::Cylinder(osg::Vec3(0.f, 0.f, 0.f), 0.25f, 0.5f);
        osg::ShapeDrawable* sd = new osg::ShapeDrawable(cylinder);
        sd->setColor(osg::Vec4(0.8f, 0.5f, 0.2f, 1.f));
        m_CommonGeode->addDrawable(sd);
        osg::StateSet* stateSet = m_CommonGeode->getOrCreateStateSet();
        osg::Material* material = new osg::Material;
        material->setColorMode(osg::Material::AMBIENT_AND_DIFFUSE);
        stateSet->setAttributeAndModes(material, osg::StateAttribute::ON);
        stateSet->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);
    }
}