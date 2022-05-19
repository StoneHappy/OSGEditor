#include "HUD.h"
#include "Function/Scene/Viewer.h"
#include "Function/Scene/Viewer.h"
namespace Soarscape
{
	HUD::HUD()
        : m_HUDCamera(new osg::Camera), selectQuad(new SelectQuad)
	{
        osg::Viewport* vp = PublicSingleton<Viewer>::getInstance().getCamera()->getViewport();
        //设置投影矩阵为正交投影
        m_HUDCamera->setProjectionMatrix(osg::Matrix::ortho2D(0, vp->width(), 0, vp->height()));

        //设置其观察矩阵为单位矩阵，且不改变，该相机永远显示，也不用参与拣选
        m_HUDCamera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
        m_HUDCamera->setViewMatrix(osg::Matrix::identity());

        //只清空深度缓存，使得其显示内容可以以主相机为背景
        m_HUDCamera->setClearMask(GL_DEPTH_BUFFER_BIT);

        //最后渲染，因为需要以主相机显示的内容为背景
        m_HUDCamera->setRenderOrder(osg::Camera::POST_RENDER);

        //不需要响应事件
        m_HUDCamera->setAllowEventFocus(false);

        m_HUDCamera->addChild(selectQuad->m_Geode);
        PublicSingleton<Viewer>::getInstance().addNode(m_HUDCamera);

	}
	void HUD::addGeode(osg::Geode* geode)
	{
        m_HUDCamera->addChild(geode);
	}
    void HUD::onResize(int width, int height)
    {
        m_HUDCamera->setProjectionMatrix(osg::Matrix::ortho2D(0, width, 0, height));
        selectQuad->onReszie(width, height);
    }
}