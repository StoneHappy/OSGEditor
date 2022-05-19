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

		void onReszie(float width, float height);
	private:
		osg::ref_ptr<osg::Geometry> m_QuadGeometry;
		osg::ref_ptr<osg::Geode> m_Geode;

		float xStart = 0, yStart = 0;
		float xEnd = 0, yEnd = 0;

		float m_Width = 0, m_Height = 0;
	};
}