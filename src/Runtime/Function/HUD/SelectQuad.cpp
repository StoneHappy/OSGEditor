#include "SelectQuad.h"
#include <osg/Geode>

#include <Core/Base/macro.h>

#include "Function/Scene/Viewer.h"
namespace Soarscape
{
	SelectQuad::SelectQuad()
		: m_QuadGeometry(new osg::Geometry), m_Geode(new osg::Geode)
	{
		//设置顶点
		osg::Vec3Array* vertices = new osg::Vec3Array;
		float depth = -0.5;
		vertices->push_back(osg::Vec3(0, 0, depth));
		vertices->push_back(osg::Vec3(100, 0, depth));
		vertices->push_back(osg::Vec3(100, 100, depth));
		vertices->push_back(osg::Vec3(0, 100, depth));
		m_QuadGeometry->setVertexArray(vertices);

		//设置颜色
		osg::Vec4Array* color = new osg::Vec4Array;
		color->push_back(osg::Vec4(0.8f, 0.8f, 0.8f, 1.0f));
		m_QuadGeometry->setColorArray(color, osg::Array::BIND_OVERALL);

		//绘制盒子
		m_QuadGeometry->addPrimitiveSet(new osg::DrawArrays(GL_QUADS, 0, 4));

		m_QuadGeometry->setNodeMask(NODE_SHOW);

		m_Geode->addDrawable(m_QuadGeometry);

		//设置透明
		osg::StateSet* ss = m_Geode->getOrCreateStateSet();
		ss->setMode(GL_BLEND, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
		ss->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
		ss->setMode(GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::OVERRIDE);
	}
	void SelectQuad::begin(int x, int y)
	{
		xStart = x;
		yStart = m_Height - y;
	}
	void SelectQuad::end(int x, int y)
	{
		//获取顶点、更新顶点
		osg::Vec3Array* vertices = (osg::Vec3Array*)m_QuadGeometry->getVertexArray();

		xEnd = x;
		yEnd = m_Height - y;

		float depth = -0.1;
		vertices->at(0).set(xStart, yStart, depth);
		vertices->at(1).set(xEnd, yStart, depth);
		vertices->at(2).set(xEnd, yEnd, depth);
		vertices->at(3).set(xStart, yEnd, depth);

		m_QuadGeometry->dirtyDisplayList();
	}
	void SelectQuad::onReszie(float width, float height)
	{
		m_Width = width;
		m_Height = height;
	}
}