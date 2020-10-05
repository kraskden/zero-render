#ifndef GLPAINTER_MAINWINDOW_H
#define GLPAINTER_MAINWINDOW_H

#include <QtGui/QWindow>
#include "../gl/Scene.h"
#include <QSet>
#include <QMap>
#include <QTimer>

class MainWindow : public  QWindow
{
    Q_OBJECT

    QBackingStore *m_backingStore;
    Scene* scene;
    Camera* camera;
    QTimer* updateTimer;

    QMap<int, std::function<void()>> moveHandlers;
    QMap<int, std::function<void()>> controlHandlers;
    QSet<int> keysActive = {};
    bool firstRelease = false;

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
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    void initHandlers();
};


#endif //GLPAINTER_MAINWINDOW_H
