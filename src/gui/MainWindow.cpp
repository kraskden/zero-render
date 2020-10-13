#include <QtGui/QPainter>
#include <QtGui/QBackingStore>
#include <QtGui/QResizeEvent>

#include <QDebug>

#include <linux/input.h>

#include <cmath>

#include "MainWindow.h"
#include "../gl/GlPainter.h"
#include "../common/const.h"


MainWindow::MainWindow(QWindow *parent) :
        QWindow(parent),
        m_backingStore(new QBackingStore(this)),
        scene(new Scene(width(), height(), this))
{
    camera = new Camera(QVector3D(0, 0, DEF_CAMERA_Z), DEF_CAMERA_YAW, DEF_CAMERA_PITCH,
                              DEF_MOVE_SPEED, DEF_ROTATE_SPEED, this);
    scene->setCamera(camera);

    updateTimer = new QTimer(this);
    connect(updateTimer, &QTimer::timeout, this, &MainWindow::onUpdateTimer);
    updateTimer->setInterval(UPDATE_TIMER_TIMEOUT);
    updateTimer->start();

    initHandlers();
    setGeometry(0, 0, 1000, 1000);
    setFlag(Qt::WindowType::Window);
    raise();
}

void MainWindow::renderLater()
{
    requestUpdate();
}

void MainWindow::renderNow()
{
    if (!isExposed()) {
        return;
    }

    QRect drawRect(0, 0, width(), height());
    m_backingStore->beginPaint(drawRect);

    QPaintDevice *device = m_backingStore->paintDevice();
    QPainter painter(device);
    scene->repaint(&painter);
    painter.end();
    m_backingStore->endPaint();
    m_backingStore->flush(drawRect);
}

bool MainWindow::event(QEvent *event)
{
    if (event->type() == QEvent::UpdateRequest) {
        renderNow();
        return true;
    }
    return QWindow::event(event);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    m_backingStore->resize(event->size());
    scene->setWidth(event->size().width());
    scene->setHeight(event->size().height());
}

void MainWindow::exposeEvent(QExposeEvent *event)
{
    if (isExposed()) {
        renderNow();
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    int key = event->key();
    if (controlHandlers.contains(key)) {
        controlHandlers[key]();
        requestUpdate();
    } else {
        keysActive += key;
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event) {
    keysActive -= event->key();
}

void MainWindow::initHandlers() {
    moveHandlers.insert(Qt::Key_W, [this]() -> void {camera->move(CamMov::Z, CamSide::FORWARD); });
    moveHandlers.insert(Qt::Key_S, [this]() -> void {camera->move(CamMov::Z, CamSide::BACKWARD); });
    moveHandlers.insert(Qt::Key_A, [this]() -> void {camera->move(CamMov::STRAFE, CamSide::BACKWARD); });
    moveHandlers.insert(Qt::Key_D, [this]() -> void {camera->move(CamMov::STRAFE, CamSide::FORWARD); });
    moveHandlers.insert(Qt::Key_K, [this]() -> void{camera->move(CamMov::YAW, CamSide::BACKWARD);});
    moveHandlers.insert(Qt::Key_Semicolon, [this]() -> void{camera->move(CamMov::YAW, CamSide::FORWARD);});
    moveHandlers.insert(Qt::Key_O, [this]() -> void{camera->move(CamMov::PITCH, CamSide::FORWARD);});
    moveHandlers.insert(Qt::Key_L, [this]() -> void{camera->move(CamMov::PITCH, CamSide::BACKWARD);});

    controlHandlers.insert(Qt::Key_Q, [this]() -> void {camera->incMovementSpeed(-DELTA_MOVE_SPEED);});
    controlHandlers.insert(Qt::Key_E, [this]() -> void {camera->incMovementSpeed(DELTA_MOVE_SPEED);});
    controlHandlers.insert(Qt::Key_I, [this]() -> void {camera->incRotateSpeed(-DELTA_ROTATE_SPEED);});
    controlHandlers.insert(Qt::Key_P, [this]() -> void {camera->incRotateSpeed(DELTA_ROTATE_SPEED);});
    controlHandlers.insert(Qt::Key_R, [this]() -> void {camera->reset(QVector3D{0, 0, DEF_CAMERA_Z}, DEF_CAMERA_YAW, DEF_CAMERA_PITCH);});
}

void MainWindow::onUpdateTimer() {

    if (!keysActive.isEmpty()) {
        for (int key : keysActive) {
            if (moveHandlers.contains(key)) {
                moveHandlers[key]();
            }
        }
        requestUpdate();
    }
}


