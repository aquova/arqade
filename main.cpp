#include "Window.h"

#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    ArqadeWindow w;
    w.show();
    return a.exec();
}
