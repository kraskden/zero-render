#ifndef GLPAINTER_MAINWINDOW_H
#define GLPAINTER_MAINWINDOW_H

#include <QtGui/QWindow>
#include "../gl/Scene.h"
#include "../gl/LightSource.h"
#include <QSet>
#include <QMap>
#include <QTimer>

#include <X11/Xlib.h>

class MainWindow : public  QWindow
{
    Q_OBJECT

    Display* dpy;
    char keysActive[32];

    QBackingStore *m_backingStore;
    Scene* scene;
    Camera* camera;
    LightSource* lightSource;
    QTimer* updateTimer;

    QMap<KeyCode, std::function<void()>> moveHandlers;
    QMap<int, std::function<void()>> controlHandlers;

public slots:
    void renderLater();
    void renderNow();


public:
    explicit MainWindow(QWindow* parent = nullptr);

private slots:
    void onUpdateTimer();

protected:
    bool event(QEvent *event) override;

    void resizeEvent(QResizeEvent *event) override;
    void exposeEvent(QExposeEvent *event) override;

    void keyPressEvent(QKeyEvent *event) override;

private:
    void initHandlers();
    void loadModel();
    inline KeyCode key2code(KeySym keySym)  {return XKeysymToKeycode(dpy, keySym); }
};


#endif //GLPAINTER_MAINWINDOW_H
