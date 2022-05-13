#pragma once

#include <QMainWindow>
#include <DockManager.h>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

namespace Soarscape
{
    class EditorRendererWidget;
    class MainWindow : public QMainWindow
    {
        Q_OBJECT

    public:
        MainWindow(QWidget* parent = nullptr);
        ~MainWindow();
    public Q_SLOTS:
        void importMesh();

    private:
        Ui::MainWindow* ui;
        ads::CDockManager* m_DockManager;
        ads::CDockWidget* m_ViewportDock;
        EditorRendererWidget* m_RendererWidget;
    };

}
