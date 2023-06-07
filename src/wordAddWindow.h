#ifndef WORD_ADD_WINDOW_H
#define WORD_ADD_WINDOW_H

#include <QMainWindow>
#include "ui_wordAddWindow.h"
#include <vector>
#include "participant.h"
#include <QTimer>
#include <set>
#include <QtCore>
#include <QMessageBox>
#include "word_game.h"

class word_game;
class wordAddWindow : public QMainWindow
{
    Q_OBJECT

public:
    // wordAddWindow(QWidget *parent = nullptr, std::vector<QSet<QString>> *wordLists = NULL);
    wordAddWindow(QWidget *parent = nullptr);
    ~wordAddWindow();

private:
    class word_game *father;
    Ui_wordAddWindow *ui;
    int level = 0;
    // std::vector<QSet<QString>> *wordLists;
private slots:
    void slotConfirmButtonClick();
};
#endif