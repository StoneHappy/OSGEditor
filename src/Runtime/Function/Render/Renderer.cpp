#include "Renderer.h"
#include <osg/ShapeDrawable>
#include <osg/Material>
#include <osgGA/TrackballManipulator>
#include "Function/Scene/Viewer.h"
#include "Core/Base/macro.h"
namespace Soarscape
{
	Renderer::Renderer()
	{
		
	}
	
	void Renderer::initialize(int x, int y, int width, int height)
	{
        m_GraphicsWindow = new osgViewer::GraphicsWindowEmbedded(x, y, width, height);
        PublicSingletonInstance(Viewer).initialize(x, y, width, height);
        PublicSingletonInstance(Viewer).attacthContext(m_GraphicsWindow);
        m_isFirstFrame = true;
	}
	void Renderer::tick(uint32_t defaultFramebuffer)
	{
        if (m_isFirstFrame)
        {
            m_isFirstFrame = false;
            PublicSingletonInstance(Viewer).attacthFrameBuffer(defaultFramebuffer);
        }
        PublicSingletonInstance(Viewer).tick();
	}

    void Renderer::resize(int x, int y, int width, int height)
    {
        m_GraphicsWindow->resized(x, y, width, height);
        PublicSingletonInstance(Viewer).resize(x, y, width, height);
    }

}