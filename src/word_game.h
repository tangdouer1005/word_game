#ifndef WORD_GAME_H
#define WORD_GAME_H

#include "ui_word_game.h"
#include <QMainWindow>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QTextCodec>
#include <QtCore>

#include <vector>
#include <string>
#include <set>
#include <algorithm>

#include "participant.h"
#include "listWindow.h"
#include "gameWindow.h"
#include "loginWindow.h"
#include "wordAddWindow.h"
#include "tcpServer.h"
class tcpServer;
class loginWindow;
class gameWindow;
class wordAddWindow;
class listWindow;
class word_game : public QMainWindow
{
    Q_OBJECT

public:
    word_game(QWidget *parent = nullptr);
    ~word_game();

    friend class loginWindow;
    friend class gameWindow;
    friend class wordAddWindow;
    friend class listWindow;
    friend class tcpServer;

private:
    std::vector<Challenger> gameChallengers;
    std::vector<Quizmaster> gameQuizmasters;
    std::set<QString> challengerId;
    std::set<QString> quizmastersId;

    listWindow *mlistWindow;
    gameWindow *mGameWindow;
    loginWindow *mLoginWindow;
    wordAddWindow *mWordAddWindow;
    class tcpServer *mServer;
    ParticipantKind me;
    Challenger *cInformation = NULL;
    Quizmaster *qInformation = NULL;
    std::vector<QSet<QString>> wordLists;
    Ui_word_game *ui;

    void printfChalenger()
    {
        ui->showInformation->clear();
        ui->showInformation->append(cInformation->getid());
        ui->showInformation->append(QString::number(cInformation->getExperience()));
        ui->showInformation->append(QString::number(cInformation->getLevel()));
        ui->showInformation->append(QString::number(cInformation->getPassedLevels()));
        ui->showInformation->append("");
    }
    void printfQuizmaster()
    {
        ui->showInformation->clear();
        ui->showInformation->append(qInformation->getid());
        ui->showInformation->append(QString::number(qInformation->getLevel()));
        ui->showInformation->append(QString::number(qInformation->getqusetionsCounts()));
        ui->showInformation->append("");
    }
    void init();
private slots:
    void slotPlayButtonClick();
    void slotTest();
    void slotSearch();
};
#endif