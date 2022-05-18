#pragma once
#include <Core/Base/PublicSingleton.h>

#include <osg/Camera>
#include <osg/Geode>
#include "SelectQuad.h"
#include <memory>
namespace Soarscape
{
	class HUD
	{
	public:
		HUD();

		void addGeode(osg::Geode* geode);

		void onResize(int width, int height);

		std::shared_ptr<SelectQuad> selectQuad;
	private:
		osg::ref_ptr<osg::Camera> m_HUDCamera;
	};
}