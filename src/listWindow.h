#ifndef LIST_WINDOW_H
#define LIST_WINDOW_H

#include <QMainWindow>
#include "ui_listWindow.h"
#include <vector>
#include "participant.h"
#include "word_game.h"
class word_game;

class listWindow : public QMainWindow
{
    Q_OBJECT

public:
    listWindow(QWidget *parent = nullptr);
    ~listWindow();

private:
    class word_game *father;
    Ui_listWindow *ui;
private slots:
    void flushList();
};

#endif