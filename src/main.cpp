#include <QtGui/QGuiApplication>
#include "gui/MainWindow.h"


int main(int argc, char** argv) {
    QGuiApplication app(argc, argv);
    MainWindow mainWindow;
    mainWindow.show();
    return QGuiApplication::exec();
}