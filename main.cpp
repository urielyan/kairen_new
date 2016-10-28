#include "common/database.h"

#include "windows/mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow::instance()->showFullScreen();

    Database::instance()->insertDataToCalibraeData(100, 2000);

    return a.exec();
}
