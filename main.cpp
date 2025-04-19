#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QPixmap loadingPix(":/new/mainwindow/Loading.jpg");
    loadingPix = loadingPix.scaled(800,440);
    QSplashScreen splash(loadingPix);
    splash.setGeometry(1000,450,800,440);
    splash.show();
    QDateTime time = QDateTime::currentDateTime();
        QDateTime currentTime = QDateTime::currentDateTime();   //记录当前时间
        while (time.secsTo(currentTime) <= 1)                   //5为需要延时的秒数
        {
            currentTime = QDateTime::currentDateTime();
            a.processEvents();
        };
    MainWindow w;
    w.show();
    splash.finish(&w);
    return a.exec();
}
