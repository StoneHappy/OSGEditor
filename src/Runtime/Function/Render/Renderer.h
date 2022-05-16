#pragma once
#include "Core/Base/PublicSingleton.h"

#include <osgViewer/GraphicsWindow>
#include <osgViewer/Viewer>
namespace Soarscape
{
	class Renderer : public PublicSingleton<Renderer>
	{
	public:

		Renderer();

		void initialize(int x, int y, int width, int height);
		void tick(uint32_t defaultFramebuffer);
		void resize(int x, int y, int width, int height);

		osg::ref_ptr<osgViewer::GraphicsWindowEmbedded> getGraphicsWindow() { return m_GraphicsWindow; }
	private:
		osg::ref_ptr<osgViewer::GraphicsWindowEmbedded> m_GraphicsWindow;
		osg::ref_ptr<osgViewer::Viewer> m_Viewer;

		bool m_isFirstFrame = true;
	};

	
}