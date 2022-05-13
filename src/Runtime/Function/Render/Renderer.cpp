#include "Renderer.h"
#include <osg/ShapeDrawable>
#include <osg/Material>
#include <osgGA/TrackballManipulator>

namespace Soarscape
{
	Renderer::Renderer()
	{
		
	}
	
	void Renderer::initialize(int x, int y, int width, int height)
	{
        m_GraphicsWindow = new osgViewer::GraphicsWindowEmbedded(x, y, width, height);
        m_Viewer = new osgViewer::Viewer;
        // create a debug scene - cylinder
        osg::Cylinder* cylinder = new osg::Cylinder(osg::Vec3(0.f, 0.f, 0.f), 0.25f, 0.5f);
        osg::ShapeDrawable* sd = new osg::ShapeDrawable(cylinder);
        sd->setColor(osg::Vec4(0.8f, 0.5f, 0.2f, 1.f));
        osg::Geode* geode = new osg::Geode;
        geode->addDrawable(sd);

        // create a main camera and attach a graphics context to it
        osg::Camera* camera = new osg::Camera;
        camera->setViewport(0, 0, width, height);
        camera->setClearColor(osg::Vec4(0.9f, 0.9f, 1.f, 1.f));
        float aspectRatio = static_cast<float>(width) / static_cast<float>(height);
        camera->setProjectionMatrixAsPerspective(30.f, aspectRatio, 1.f, 1000.f);
        camera->setGraphicsContext(m_GraphicsWindow);

        // attach a camera to the viewer
        m_Viewer->setCamera(camera);
        m_Viewer->setSceneData(geode);
        osgGA::TrackballManipulator* manipulator = new osgGA::TrackballManipulator;
        manipulator->setAllowThrow(false);
        m_Viewer->setCameraManipulator(manipulator);
        m_Viewer->setThreadingModel(osgViewer::Viewer::SingleThreaded);
        m_Viewer->realize();
        osg::StateSet* stateSet = geode->getOrCreateStateSet();
        osg::Material* material = new osg::Material;
        material->setColorMode(osg::Material::AMBIENT_AND_DIFFUSE);
        stateSet->setAttributeAndModes(material, osg::StateAttribute::ON);
        stateSet->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);
	}
	void Renderer::tick(uint32_t defaultFramebuffer)
	{
        if (m_isFirstFrame)
        {
            m_isFirstFrame = false;
            m_Viewer->getCamera()->getGraphicsContext()->setDefaultFboId(defaultFramebuffer);
        }
        m_Viewer->frame();
	}

    void Renderer::resize(int x, int y, int width, int height)
    {
        m_GraphicsWindow->resized(x, y, width, height);
        osg::Camera* camera = m_Viewer->getCamera();
        camera->setViewport(0, 0, width, height);
    }

}