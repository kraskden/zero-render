#ifndef GLPAINTER_MAINWINDOW_H
#define GLPAINTER_MAINWINDOW_H

#include <QtGui/QWindow>
#include "../gl/Scene.h"

class MainWindow : public  QWindow
{
Q_OBJECT
public:
    explicit MainWindow(QWindow* parent = nullptr);

public slots:
    void renderLater();
    void renderNow();

protected:
    bool event(QEvent *event) override;

    void resizeEvent(QResizeEvent *event) override;
    void exposeEvent(QExposeEvent *event) override;

    void keyPressEvent(QKeyEvent *event) override;

private:
    QBackingStore *m_backingStore;
    Scene* scene;
    Camera* camera;
};


#endif //GLPAINTER_MAINWINDOW_H
