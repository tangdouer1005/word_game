#include "word_game.h"

#include <QApplication>
#pragma comment(lib, "user32.lib")

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    word_game w;
    // w.show();
    return a.exec();
}