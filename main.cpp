#include <QApplication>

#include "MainWindow.h"

#undef main
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow main;
    main.show();

    return app.exec();
}
