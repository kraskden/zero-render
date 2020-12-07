#include <QtGui/QPainter>
#include <QtGui/QBackingStore>
#include <QtGui/QResizeEvent>
#include <QFileDialog>

#include <QDebug>
#include <cmath>

#include "MainWindow.h"
#include "../gl/GlPainter.h"
#include "../common/const.h"

#include <X11/Xlib.h>
#include <X11/keysym.h>

#include "../math/Matrix4D.h"
#include "../obj/Parser.h"

MainWindow::MainWindow(QWindow *parent) :
        QWindow(parent),
        m_backingStore(new QBackingStore(this)),
        scene(new Scene(width(), height(), this))
{
    // TODO: time duration for better camera moving

    scene->setModel(new Model3D("../resources/Ship"));

    camera = new Camera(QVector3D(0, 0, DEF_CAMERA_Z), DEF_CAMERA_YAW, DEF_CAMERA_PITCH,
                              DEF_MOVE_SPEED, DEF_ROTATE_SPEED, this);
    scene->setCamera(camera);

    lightSource = new LightSource(QVector3D(0, 0, 0), LIGHT_RADIUS, 0,
                                  0, DEF_ROTATE_SPEED, this);
    scene->setLightSource(lightSource);

    dpy = XOpenDisplay(nullptr);
    XSetInputFocus(dpy, this->winId(), RevertToNone, CurrentTime);

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
    };
}

void MainWindow::initHandlers() {
    moveHandlers.insert(key2code(XK_w), [this]() -> void {camera->move(CamMov::Z, CamSide::FORWARD); });
    moveHandlers.insert(key2code(XK_s), [this]() -> void {camera->move(CamMov::Z, CamSide::BACKWARD); });
    moveHandlers.insert(key2code(XK_a), [this]() -> void {camera->move(CamMov::STRAFE, CamSide::BACKWARD); });
    moveHandlers.insert(key2code(XK_d), [this]() -> void {camera->move(CamMov::STRAFE, CamSide::FORWARD); });
    moveHandlers.insert(key2code(XK_k), [this]() -> void{camera->move(CamMov::YAW, CamSide::BACKWARD);});
    moveHandlers.insert(key2code(XK_semicolon), [this]() -> void{camera->move(CamMov::YAW, CamSide::FORWARD);});
    moveHandlers.insert(key2code(XK_o), [this]() -> void{camera->move(CamMov::PITCH, CamSide::FORWARD);});
    moveHandlers.insert(key2code(XK_l), [this]() -> void{camera->move(CamMov::PITCH, CamSide::BACKWARD);});
    // Light source
    moveHandlers.insert(key2code(XK_Left), [this]() -> void {lightSource->changeAzimuth(-1);});
    moveHandlers.insert(key2code(XK_Right), [this]() -> void {lightSource->changeAzimuth(1); });
    moveHandlers.insert(key2code(XK_Up), [this]() -> void {lightSource->changeAltitude(1); });
    moveHandlers.insert(key2code(XK_Down), [this]() -> void {lightSource->changeAltitude(-1); });

    controlHandlers.insert(Qt::Key_Q, [this]() -> void {camera->incMovementSpeed(-DELTA_MOVE_SPEED);});
    controlHandlers.insert(Qt::Key_E, [this]() -> void {camera->incMovementSpeed(DELTA_MOVE_SPEED);});
    controlHandlers.insert(Qt::Key_I, [this]() -> void {camera->incRotateSpeed(-DELTA_ROTATE_SPEED);});
    controlHandlers.insert(Qt::Key_P, [this]() -> void {camera->incRotateSpeed(DELTA_ROTATE_SPEED);});
    controlHandlers.insert(Qt::Key_R, [this]() -> void {camera->reset(QVector3D{0, 0, DEF_CAMERA_Z}, DEF_CAMERA_YAW, DEF_CAMERA_PITCH);});
    controlHandlers.insert(Qt::Key_Shift, [this]() -> void{lightSource->reset(LIGHT_RADIUS, 0, 0);});

    controlHandlers.insert(Qt::Key_F2, [this]() -> void{this->loadModel();});
}

void MainWindow::onUpdateTimer() {

    XQueryKeymap(dpy, keysActive);
    bool isReqUpdate = false;
    for (KeyCode code : moveHandlers.keys()) {
        if (!!(keysActive[code >> 3] & (1 << (code & 7)))) {
            moveHandlers[code]();
            isReqUpdate = true;
        }
    }
    if (isReqUpdate) {
        requestUpdate();
    }
}

void MainWindow::loadModel() {
    QString modelDir =  QFileDialog::getExistingDirectory(nullptr, "Open model directory");
    scene->setModel(new Model3D(modelDir));
}