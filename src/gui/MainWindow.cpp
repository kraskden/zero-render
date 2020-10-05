#include <QtGui/QPainter>
#include <QtGui/QBackingStore>
#include <QtGui/QResizeEvent>

#include <QDebug>

#include <cmath>

#include "MainWindow.h"
#include "../gl/GlPainter.h"
#include "../common/const.h"


MainWindow::MainWindow(QWindow *parent) :
        QWindow(parent),
        m_backingStore(new QBackingStore(this)),
        scene(new Scene(width(), height(), this))
{
    this->camera = new Camera(QVector3D(0, 0, DEF_CAMERA_Z), DEF_CAMERA_YAW, DEF_CAMERA_PITCH,
                              DEF_MOVE_SPEED, DEF_ROTATE_SPEED, this);
    scene->setCamera(camera);

    setGeometry(0, 0, 1000, 1000);
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
    scene->setPainter(&painter);
    scene->repaint();
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
    firstRelease = true;
    keysActive += event->key();
}

void MainWindow::keyReleaseEvent(QKeyEvent *event) {
    if (firstRelease) {
        handleInput();
    }
    firstRelease = false;
    keysActive -= event->key();
}

void MainWindow::handleInput() {
    for (int key : keysActive) {
        handleKey(key);
    }
    requestUpdate();
}

void MainWindow::handleKey(int key) {
    switch (key) {
        case Qt::Key_W:
            camera->move(CameraMovement::Z, MoveDirection::FORWARD);
            break;
        case Qt::Key_S:
            camera->move(CameraMovement::Z, MoveDirection::BACKWARD);
            break;
        case Qt::Key_A:
            camera->move(CameraMovement::STRAFE, MoveDirection::BACKWARD);
            break;
        case Qt::Key_D:
            camera->move(CameraMovement::STRAFE, MoveDirection::FORWARD);
            break;
        case Qt::Key_K:
            camera->move(CameraMovement::YAW, MoveDirection::FORWARD);
            break;
        case Qt::Key_Semicolon:
            camera->move(CameraMovement::YAW, MoveDirection::BACKWARD);
            break;
        case Qt::Key_O:
            camera->move(CameraMovement::PITCH, MoveDirection::FORWARD);
            break;
        case Qt::Key_L:
            camera->move(CameraMovement::PITCH, MoveDirection::BACKWARD);
            break;

        case Qt::Key_Q:
            camera->incMovementSpeed(-DELTA_MOVE_SPEED);
            break;
        case Qt::Key_E:
            camera->incMovementSpeed(DELTA_MOVE_SPEED);
            break;
        case Qt::Key_I:
            camera->incRotateSpeed(-DELTA_ROTATE_SPEED);
            break;
        case Qt::Key_P:
            camera->incRotateSpeed(DELTA_ROTATE_SPEED);
            break;

        case Qt::Key_R:
            camera->reset(QVector3D{0, 0, DEF_CAMERA_Z}, DEF_CAMERA_YAW, DEF_CAMERA_PITCH);
            break;
    }
}

