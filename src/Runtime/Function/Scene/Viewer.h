#pragma once
#include <osgViewer/Viewer>
#include <Core/Base/PublicSingleton.h>
namespace Soarscape
{
	class Viewer : public PublicSingleton<Viewer>
	{
	public:
		friend class Renderer;
		Viewer();

		void initialize(int x, int y, int width, int height);

		void addNode(osg::Node* node);

		void addCommonDrawable(osg::Drawable* drawable);

		void addCustomGeode(osg::Geode* geode);

		void attacthContext(osgViewer::GraphicsWindow* context);
		void attacthFrameBuffer(uint32_t fboid);

		void tick();

		void resize(int x, int y, int width, int height);
	private:

		void setupCommonGeode();

		osg::ref_ptr<osg::Group> m_RootGroup;
		osg::ref_ptr<osg::Geode> m_CommonGeode;
		osg::ref_ptr<osgViewer::Viewer> m_Viewer;
	};
}