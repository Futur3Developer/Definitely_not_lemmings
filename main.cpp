#include "mainwindow.h"
#include "game.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow main_menu;
    Game::Get().set_main_window(main_menu);
    main_menu.show();

    return a.exec();
}
