///////////////////////////////////////////////////////////
/// Annika Diekmann, Sven Fröhling, Ove von Stackelberg ///
///////////////////////////////////////////////////////////

#include "mainwindow.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
