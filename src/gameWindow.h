#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

#include <QMainWindow>
#include <iostream>
#include <random>
#include "ui_gameWindow.h"
#include <vector>
#include "participant.h"
#include <QTimer>
#include <set>
#include <QtCore>
#include <QMessageBox>
#include "word_game.h"

class word_game;
#define SECOND 1000
#define SHOWTIME 2
#define ANSWERTIME 4
#define RESULTTIME 2
#define ROUND 9
enum Status
{
    UNSTART,
    PAUSE,
    SHOWWORD,
    ANSWER,
    RESULT
};

class gameWindow : public QMainWindow
{
    Q_OBJECT

public:
    gameWindow(QWidget *parent = nullptr, int level = 0);
    ~gameWindow();
    void selectLevel(int l) { level = l; }

private:
    Ui_gameWindow *ui;
    QTimer *mTimer;
    unsigned int showTime = SHOWTIME;
    unsigned int answerTime = ANSWERTIME;
    unsigned int resultTime = RESULTTIME;
    unsigned int remainRounds = ROUND;
    Status curStatus = UNSTART;
    Status preStatus;
    int level;

    QString curString;
    QList<QString> wordList;
    std::vector<QList<QString>> wordLists;
    class word_game *father;
private slots:
    void slotConfirmButtonClick();
    void slotStartButtonClick();
    void slotTimerOut();
};
#endif