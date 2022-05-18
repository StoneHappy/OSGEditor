#pragma once
#include <Core/Base/PublicSingleton.h>
#include <osg/Geometry>
#include <osg/Geode>
namespace Soarscape
{
	class SelectQuad
	{
	public:
		friend class HUD;
		SelectQuad();
		void begin(int x, int y);

		void end(int x, int y);

	private:
		osg::ref_ptr<osg::Geometry> m_QuadGeometry;
		osg::ref_ptr<osg::Geode> m_Geode;
	};
}