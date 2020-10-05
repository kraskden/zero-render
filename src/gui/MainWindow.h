#ifndef GLPAINTER_MAINWINDOW_H
#define GLPAINTER_MAINWINDOW_H

#include <QtGui/QWindow>
#include "../gl/Scene.h"
#include <QSet>
#include <QMap>

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

    void handleInput();
    void handleKey(int key);
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    QBackingStore *m_backingStore;
    Scene* scene;
    Camera* camera;

    QSet<int> keysActive = {};
    bool firstRelease = false;
};


#endif //GLPAINTER_MAINWINDOW_H
