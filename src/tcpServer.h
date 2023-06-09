#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QtCore>
#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QNetworkInterface>
#include <QMessageBox>
#include "ui_tcpServer.h"
#include "word_game.h"
#include "word_game.h"
#include <random>
class word_game;
class tcpServer : public QMainWindow
{
    Q_OBJECT

public:
    explicit tcpServer(QWidget *parent = 0);
    ~tcpServer();

private:
    Ui_tcpServer *ui;
    QTcpServer *Server;
    QList<QTcpSocket *> tcpClient;
    QTcpSocket *currentClient;
    std::set<QString> ChallengerL;
    std::set<QString> QuizmasterL;
    class word_game *father;
private slots:
    void NewConnectionSlot();
    void disconnectedSlot();
    void ReadData();

    // void on_btnSend_clicked();
    // void on_btnClear_clicked();
};
#endif // tcpServer_H