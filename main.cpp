#include <QApplication>
#include "mainwindow.h"
int main(int argc, char *argv[])
{
    QApplication prog(argc, argv);
    MainWindow w;;
    w.setWindowTitle("GTSA");
    w.setWindowIcon(QIcon(":res/gtsaLogo.png"));
    w.show();
    return prog.exec();
}
