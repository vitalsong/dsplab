#include "main-window.h"

#include <QApplication>
#include <QTextCodec>

int main(int argc, char* argv[])
{
    QTextCodec* codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);

    QApplication a(argc, argv);
    // a.setStyle("fusion");
    MainWindow w;
    w.showMaximized();

    return a.exec();
}
