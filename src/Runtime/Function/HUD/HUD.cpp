#include "HUD.h"
#include "Function/Scene/Viewer.h"
#include "Function/Scene/Viewer.h"
namespace Soarscape
{
	HUD::HUD()
        : m_HUDCamera(new osg::Camera), selectQuad(new SelectQuad)
	{
        osg::Viewport* vp = PublicSingleton<Viewer>::getInstance().getCamera()->getViewport();
        //����ͶӰ����Ϊ����ͶӰ
        m_HUDCamera->setProjectionMatrix(osg::Matrix::ortho2D(0, vp->width(), 0, vp->height()));

        //������۲����Ϊ��λ�����Ҳ��ı䣬�������Զ��ʾ��Ҳ���ò����ѡ
        m_HUDCamera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
        m_HUDCamera->setViewMatrix(osg::Matrix::identity());

        //ֻ�����Ȼ��棬ʹ������ʾ���ݿ����������Ϊ����
        m_HUDCamera->setClearMask(GL_DEPTH_BUFFER_BIT);

        //�����Ⱦ����Ϊ��Ҫ���������ʾ������Ϊ����
        m_HUDCamera->setRenderOrder(osg::Camera::POST_RENDER);

        //����Ҫ��Ӧ�¼�
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