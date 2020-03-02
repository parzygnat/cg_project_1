#include "photoeditor.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PhotoEditor w;
    w.show();
    return a.exec();
}
