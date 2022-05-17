#include "EditorRenderWidget.h"

#include <iostream>
#include <Core/Base/macro.h>
#include <Engine.h>
#include <Function/Render/Renderer.h>
#include <Function/Event/EventSystem.h>

#include <Function/Scene/Scene.h>
#include <Resource/Components/Mesh.h>
#include <Resource/Components/Transform.h>
#include <qelapsedtimer.h>
#include <qevent.h>
#include <QtImGui.h>
#include <imgui.h>
#include <ImGuizmo.h>
#include <glm/gtc/type_ptr.hpp>
#include <osg/ShapeDrawable>
#include <osg/Material>
#include <osgGA/TrackballManipulator>

#include <Function/Scene/Viewer.h>
#include <osg/Matrix>

#include <Resource/Data/Implement/OSGGdalTexture.h>
namespace Soarscape
{
    OSGGdalTexture* gdaltexture;
	EditorRendererWidget::EditorRendererWidget(QWidget* parent)
		: QOpenGLWidget(parent), m_MousePos(std::make_shared<MousePos>(0.0f, 0.0f)), m_MouseAngle(std::make_shared<MouseAngle>(0.0f, 0.0f))
    {

    }
	EditorRendererWidget::~EditorRendererWidget()
	{

    }

	void EditorRendererWidget::initializeGL()
	{
        PublicSingleton<Engine>::getInstance().renderInitialize(this->x(), this->y(), this->width(), this->height());
        PublicSingleton<Engine>::getInstance().logicalInitialize();
        QtImGui::initialize(this);
        gdaltexture = new OSGGdalTexture("D:/codes/gdal_projs/OpenGL-and-GDAL-Tutorials/data/satellite/res.tif", OSGGdalTexture::ImageType::Image);
        // 创建形状绘制
        auto quad = osg::createTexturedQuadGeometry(osg::Vec3(-0.5, -0.5, 0), osg::Vec3(1.0f, 0.0f, 0.0f), osg::Vec3(0.0f, 1.0f, 0.0f));
        gdaltexture->getOsgGeode()->addDrawable(quad);
        PublicSingleton<Viewer>::getInstance().addCustomGeode(gdaltexture->getOsgGeode());
	}

	void EditorRendererWidget::resizeGL(int w, int h)
	{
        PublicSingleton<Renderer>::getInstance().resize(this->x(), this->y(), w, h);
	}

	void EditorRendererWidget::paintGL()
	{
        QElapsedTimer timer;
        timer.start();
        PublicSingleton<Engine>::getInstance().logicalTick();
        PublicSingleton<Engine>::getInstance().renderTick(defaultFramebufferObject());
        update();
        PublicSingleton<Engine>::getInstance().DeltaTime = timer.nsecsElapsed()* 1.0e-9f;
        renderImGui();
	}


    bool EditorRendererWidget::event(QEvent* event)
    {
        bool handled = QOpenGLWidget::event(event);
        this->update();
        return handled;
    }

    void EditorRendererWidget::mousePressEvent(QMouseEvent* event)
    {
        unsigned int button = 0;
        switch (event->button()) {
        case Qt::LeftButton:
            button = 1;
            break;
        case Qt::MiddleButton:
            button = 2;
            break;
        case Qt::RightButton:
            button = 3;
            break;
        default:
            break;
        }
        this->getEventQueue()->mouseButtonPress(event->x(), event->y(), button);

        QKeyEvent* e = (QKeyEvent*)event;
        if (e->modifiers() == Qt::ShiftModifier)
        {
            m_MousePos->x = event->pos().x();
            m_MousePos->y = -event->pos().y();
            PublicSingletonInstance(EventSystem).sendEvent("EditCamera_Begin", (void*)m_MousePos.get());
        }
    }

    void EditorRendererWidget::mouseDoubleClickEvent(QMouseEvent* event)
    {
    }

    void EditorRendererWidget::mouseMoveEvent(QMouseEvent* event)
    {
        this->getEventQueue()->mouseMotion(event->x(), event->y());

        QKeyEvent* e = (QKeyEvent*)event;
        if (e->modifiers() == Qt::ShiftModifier)
        {
            m_MousePos->x = -event->pos().x();
            m_MousePos->y = event->pos().y();
            if (event->buttons() & Qt::LeftButton)
            {

                PublicSingletonInstance(EventSystem).sendEvent("EditCamera_Rotate", (void*)m_MousePos.get());
            }
            else if (event->buttons() & Qt::MiddleButton)
            {

                PublicSingletonInstance(EventSystem).sendEvent("EditCamera_Pan", (void*)m_MousePos.get());
            }
        }
        
    }

    void EditorRendererWidget::mouseReleaseEvent(QMouseEvent* event)
    {
        unsigned int button = 0;
        switch (event->button()) {
        case Qt::LeftButton:
            button = 1;
            break;
        case Qt::MiddleButton:
            button = 2;
            break;
        case Qt::RightButton:
            button = 3;
            break;
        default:
            break;
        }
        this->getEventQueue()->mouseButtonRelease(event->x(), event->y(), button);

        QKeyEvent* e = (QKeyEvent*)event;
        if (e->modifiers() == Qt::ShiftModifier)
        {
            PublicSingletonInstance(EventSystem).sendEvent("EditCamera_End", (void*)m_MousePos.get());
        }
    }

    void EditorRendererWidget::wheelEvent(QWheelEvent* event)
    {
        int delta = event->delta();
        osgGA::GUIEventAdapter::ScrollingMotion motion = delta > 0 ?
            osgGA::GUIEventAdapter::SCROLL_UP : osgGA::GUIEventAdapter::SCROLL_DOWN;
        this->getEventQueue()->mouseScroll(motion);

        m_MouseAngle->x = event->angleDelta().x();
        m_MouseAngle->y = event->angleDelta().y();
        PublicSingletonInstance(EventSystem).sendEvent("EditCamera_Zoom", (void*)m_MouseAngle.get());
    }

    void EditorRendererWidget::keyPressEvent(QKeyEvent* event)
    {
        
    }

    void EditorRendererWidget::keyReleaseEvent(QKeyEvent* event)
    {
        switch (event->key())
        {
        case Qt::Key_R:
            gdaltexture->setRed();
            break;
        case Qt::Key_G:
            gdaltexture->setGreen();
            break;
        case Qt::Key_B:
            gdaltexture->setBlue();
            break;
        case Qt::Key_A:
            gdaltexture->setRGBA();
            break;
        default:
            break;
        }
    }

    void EditorRendererWidget::renderImGui()
    {
        QtImGui::newFrame();
        ImGuizmo::BeginFrame();
        ImGuizmo::SetOrthographic(false);
        int x, y, width, height;
        const QRect& geom = this->geometry();
        x = geom.x();
        y = geom.y();
        width = geom.width();
        height = geom.height();
        ImGuizmo::SetRect(x, y, width, height);
        osg::Matrixf view = PublicSingletonInstance(Viewer).getCamera()->getViewMatrix();
        osg::Matrixf proj = PublicSingletonInstance(Viewer).getCamera()->getProjectionMatrix();
        ImGuizmo::SetRect(x, y, width, height);
        glm::mat4 testMatrix = glm::mat4(1);
        ImGuizmo::ViewManipulate(view.ptr(), 5.0f, ImVec2(x + width - width * 0.1, 0), ImVec2(width * 0.1, width * 0.1), 0x10101010);
        //ImGuizmo::Manipulate(view.ptr(), proj.ptr(), ImGuizmo::OPERATION::ROTATE, ImGuizmo::LOCAL, glm::value_ptr(testMatrix));
        ImGui::Text("Hello");
        ImGui::Render();
        QtImGui::render();
        PublicSingletonInstance(Viewer).getCamera()->setViewMatrix(view);
        PublicSingletonInstance(Viewer).getCamera()->setProjectionMatrix(proj);
    }
    osgGA::EventQueue* EditorRendererWidget::getEventQueue()
    {
        osgGA::EventQueue* eventQueue = PublicSingletonInstance(Renderer).getGraphicsWindow()->getEventQueue();
        return eventQueue;
    }
    void EditorRendererWidget::importMesh(const std::string filename)
    {
        auto pointPos = filename.find_last_of(".");
        auto lPos = filename.find_last_of("/");
        std::string meshName = filename.substr(lPos + 1, pointPos - 1 - lPos);
        auto testMesh = PublicSingletonInstance(Scene).CreateObject(meshName);
        testMesh.AddComponent<TransformComponent>();
    }
}