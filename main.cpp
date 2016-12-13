#include <QApplication>
#include "v2xplatform.h"
#include <QDebug>
#include "UI/mainwindow.h"

int main(int argc, char *argv[])
{
//    QCoreApplication a(argc, argv);
    QApplication a(argc, argv);

    GV2XPlatform = new V2XPlatform();
    GV2XPlatform->Initialize(argc, argv);

    GMainWindow->show();

    int ret = a.exec();

    qDebug() << "Platform exited.";

    return ret;
}

