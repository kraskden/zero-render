#include <QtGui/QPainter>
#include <QtGui/QBackingStore>
#include <QtGui/QResizeEvent>

#include <cmath>

#include "MainWindow.h"
#include "../gl/GlPainter.h"


MainWindow::MainWindow(QWindow *parent) :
        QWindow(parent),
        m_backingStore(new QBackingStore(this)),
        scene(new Scene(width(), height(), this))
{
    this->setFlag(Qt::Dialog);
    this->camera = new Camera(QVector3D{0, 0, 1.0}, QVector3D{}, 0.03, this);
    connect(camera, &Camera::cameraChanged, this, &MainWindow::requestUpdate);
    scene->setCamera(camera);

    setGeometry(0, 0, 1000, 1000);
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
    switch (event->key()) {
        case Qt::Key_W:
            camera->forward();
            break;
        case Qt::Key_S:
            camera->backward();
            break;
        case Qt::Key_A:
            camera->rotate(M_PI / 30);
            break;
        case Qt::Key_D:
            camera->rotate(-M_PI / 30);
            break;
        default:
            QWindow::keyPressEvent(event);
    }
}

