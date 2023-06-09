#include "tcpServer.h"

std::random_device rd;  // 用于生成种子的随机数引擎，一般使用系统提供的硬件随机数生成器
std::mt19937 gen(rd()); // 使用Mersenne Twister随机数引擎，随机数种子使用rd生成的
tcpServer::tcpServer(QWidget *parent)
    : QMainWindow(parent), father((class word_game *)parent), ui(new Ui_tcpServer)
{
    ui->setupUi(this);

    Server = new QTcpServer(this);

    bool ok = Server->listen(QHostAddress::Any, 1005);
    if (ok)
    {
        QMessageBox::information(this,
                                 tr("提示!"),
                                 tr("正在监听的端口为1005"),
                                 QMessageBox::Ok | QMessageBox::Cancel,
                                 QMessageBox::Ok);
    }

    connect(Server, SIGNAL(newConnection()), this, SLOT(NewConnectionSlot()));
    // connect(ui->btnSend, SIGNAL(clicked()), this, SLOT(on_btnSend_clicked()));
}
void tcpServer::NewConnectionSlot()
{
    ui->information->append("链接请求");
    currentClient = Server->nextPendingConnection();
    currentClient->write("connected");
    tcpClient.append(currentClient);
    ui->cbxConnection->addItem(tr("%1:%2").arg(currentClient->peerAddress().toString().split("::ffff:")[1]).arg(currentClient->peerPort()));
    connect(currentClient, SIGNAL(readyRead()), this, SLOT(ReadData()));
    connect(currentClient, SIGNAL(disconnected()), this, SLOT(disconnectedSlot()));
}
void tcpServer::disconnectedSlot()
{
    // 由于disconnected信号并未提供SocketDescriptor，所以需要遍历寻找
    for (int i = 0; i < tcpClient.length(); i++)
    {
        if (tcpClient[i]->state() == QAbstractSocket::UnconnectedState)
        {
            // 删除存储在combox中的客户端信息
            ui->cbxConnection->removeItem(ui->cbxConnection->findText(tr("%1:%2")
                                                                          .arg(tcpClient[i]->peerAddress().toString().split("::ffff:")[1])
                                                                          .arg(tcpClient[i]->peerPort())));
            // 删除存储在tcpClient列表中的客户端信息
            tcpClient[i]->destroyed();
            tcpClient.removeAt(i);
        }
    } // 由于disconnected信号并未提供SocketDescriptor，所以需要遍历寻找
    for (int i = 0; i < tcpClient.length(); i++)
    {
        if (tcpClient[i]->state() == QAbstractSocket::UnconnectedState)
        {
            // 删除存储在combox中的客户端信息
            ui->cbxConnection->removeItem(ui->cbxConnection->findText(tr("%1:%2")
                                                                          .arg(tcpClient[i]->peerAddress().toString().split("::ffff:")[1])
                                                                          .arg(tcpClient[i]->peerPort())));
            // 删除存储在tcpClient列表中的客户端信息
            tcpClient[i]->destroyed();
            tcpClient.removeAt(i);
        }
    }
}
void tcpServer::ReadData()
{
    // 由于readyRead信号并未提供SocketDecriptor，所以需要遍历所有客户端
    for (int i = 0; i < tcpClient.length(); i++)
    {
        QByteArray buffer = tcpClient[i]->readAll();
        if (buffer.isEmpty())
            continue;

        static QString IP_Port, IP_Port_Pre;
        IP_Port = tr("[%1:%2]:").arg(tcpClient[i]->peerAddress().toString().split("::ffff:")[1]).arg(tcpClient[i]->peerPort());
        if (IP_Port != IP_Port_Pre)
            ui->edtRecv->append(IP_Port);

        ui->edtRecv->append(buffer);
        IP_Port_Pre = IP_Port;
        QString ret = QString::fromUtf8(buffer);
        QStringList list = ret.split(" ");
        if (list[0] == QString("login"))
        {
            QString id = list[2], password = list[3];
            if (list[1] == QString("gamemaster"))
            {
                if (ChallengerL.count(id))
                {
                    tcpClient[i]->write("failLogin idNoExist");
                }
                for (size_t j = 0; j < father->gameQuizmasters.size(); j++)
                {
                    if ((father->gameQuizmasters.at(j)).getid() == id)
                    {
                        if ((father->gameQuizmasters.at(j)).getpassword() == password)
                        {
                            tcpClient[i]->write("successLogin");
                            ChallengerL.insert(id);
                        }
                        else
                        {
                            tcpClient[i]->write("failLogin passwordWrong");
                        }
                        return;
                    }
                }
                tcpClient[i]->write("failLogin idNoExist");
                return;
            }
            else if (list[1] == QString("challenger"))
            {
                if (QuizmasterL.count(id))
                {
                    tcpClient[i]->write("failLogin idNoExist");
                }
                for (size_t j = 0; j < father->gameChallengers.size(); j++)
                {
                    if ((father->gameChallengers.at(j)).getid() == id)
                    {
                        if ((father->gameChallengers.at(j)).getpassword() == password)
                        {
                            tcpClient[i]->write("successLogin");
                            QuizmasterL.insert(id);
                        }
                        else
                        {
                            tcpClient[i]->write("failLogin passwordWrong");
                        }
                        return;
                    }
                }
                tcpClient[i]->write("failLogin idNoExist");
                return;
            }
            else
            {
                QMessageBox::information(this,
                                         tr("wrong"),
                                         tr("收到无法解析信息"),
                                         QMessageBox::Ok | QMessageBox::Cancel,
                                         QMessageBox::Ok);
            }
        }
        else if (list[0] == QString("play"))
        {
            QString to_sent;
            for (size_t k = 0; k < 3; k++)
            {
                QList<QString> wordList = father->wordLists[k].toList();
                for (size_t j = 0; j < 3; j++)
                {
                    std::uniform_int_distribution<> dis(0, wordList.size() - 1);
                    int index = dis(gen);
                    to_sent += " " + wordList[index];
                    wordList.removeAll(wordList[index]);
                }
            }
            tcpClient[i]->write("dir" + to_sent.toUtf8());
        }
        else if (list[0] == QString("regist"))
        {
            QString id = list[2], password = list[3];
            if (list[1] == QString("challenger"))
            {
                for (size_t j = 0; j < father->gameChallengers.size(); j++)
                {
                    if ((father->gameChallengers.at(j)).getid() == id)
                    {
                        tcpClient[i]->write("registFail");
                        return;
                    }
                }
                father->gameChallengers.push_back({id, password, 0, 0, 0});
                tcpClient[i]->write("registSuccess");
            }
            else if (list[1] == QString("gamemaster"))
            {
                for (size_t j = 0; j < father->gameQuizmasters.size(); j++)
                {
                    if ((father->gameQuizmasters.at(j)).getid() == id)
                    {
                        tcpClient[i]->write("registFail");
                        return;
                    }
                }
                father->gameQuizmasters.push_back({id, password, 0, 0});
                tcpClient[i]->write("registSuccess");
            }
            else
            {
            }
        }
        else if (list[0] == QString("addWord"))
        {
            QString tmp = list[1];
            if (tmp.size() <= 4)
            {
                if ((father->wordLists.at(0)).contains(tmp))
                {
                    tcpClient[i]->write("addFail");
                    return;
                }
                else
                {
                    (father->wordLists.at(0)).insert(tmp);
                    tcpClient[i]->write("addSuccess");
                }
            }
            else if (tmp.size() > 4 && tmp.size() < 9)
            {
                if ((father->wordLists.at(1)).contains(tmp))
                {
                    tcpClient[i]->write("addFail");
                    return;
                }
                else
                {
                    (father->wordLists.at(1)).insert(tmp);
                    tcpClient[i]->write("addSuccess");
                }
            }
            else
            {
                if ((father->wordLists.at(2)).contains(tmp))
                {
                    tcpClient[i]->write("addFail");
                    return;
                }
                else
                {
                    (father->wordLists.at(2)).insert(tmp);
                    tcpClient[i]->write("addSuccess");
                }
            }
        }
        else if (list[0] == QString("addScore"))
        {
            QString id = list[1];
            int exp = list[2].toInt();
            for (auto &iter : father->gameChallengers)
            {
                if (iter.getid() == id)
                {
                    iter.addExperience(exp);
                    return;
                }
            }
            QMessageBox::information(this,
                                     tr("分数增加失败!"),
                                     tr(id.toUtf8() + "未找到"),
                                     QMessageBox::Ok | QMessageBox::Cancel,
                                     QMessageBox::Ok);
        }
        else if (list[0] == QString("addWordNum"))
        {
            QString id = list[1];
            for (auto &iter : father->gameQuizmasters)
            {
                if (iter.getid() == id)
                {
                    iter.incQusetionCounts();
                    return;
                }
            }
            QMessageBox::information(this,
                                     tr("单词数增加失败!"),
                                     tr(id.toUtf8() + "未找到"),
                                     QMessageBox::Ok | QMessageBox::Cancel,
                                     QMessageBox::Ok);
        }
        else if (list[0] == QString("Rank"))
        {
            sort(father->gameChallengers.begin(), father->gameChallengers.end());
            sort(father->gameQuizmasters.begin(), father->gameQuizmasters.end());
            QString rank = "RANK ";
            for (auto iter : father->gameChallengers)
            {
                rank += QString::number(iter.getLevel()) + "\t" + QString::number(iter.getExperience()) + "\t" + iter.getid() + "\n";
            }
            rank += " ";
            for (auto iter : father->gameQuizmasters)
            {
                rank += QString::number(iter.getLevel()) + "\t" + QString::number(iter.getqusetionsCounts()) + "\t" + iter.getid() + "\n";
            }

            tcpClient[i]->write(rank.toUtf8());
        }
        else if (list[0] == QString("search"))
        {
            QString id = list[2];
            QString send;
            if (list[1] == QString("Challenger"))
            {
                for (auto iter : father->gameChallengers)
                {
                    if (iter.getid() == id)
                    {
                        send += "searchSuccess ";
                        send += QString::number(iter.getLevel()) + "\n" + QString::number(iter.getExperience()) + "\n" + iter.getid() + "\n";
                        tcpClient[i]->write(send.toUtf8());
                        return;
                    }
                }
                send += "searchFail";
                tcpClient[i]->write(send.toUtf8());
            }
            else if (list[1] == QString("Quizmaster"))
            {
                for (auto iter : father->gameQuizmasters)
                {
                    if (iter.getid() == id)
                    {
                        send += "searchSuccess ";
                        send += QString::number(iter.getLevel()) + "\n" + QString::number(iter.getqusetionsCounts()) + "\n" + iter.getid() + "\n";
                        tcpClient[i]->write(send.toUtf8());
                        return;
                    }
                }
                send += "searchFail";
                tcpClient[i]->write(send.toUtf8());
            }
            else
            {
            }
        }
    }
}

// void tcpServer::on_btnSend_clicked()
// {
//     QString data = ui->edtSend->toPlainText();
//     QString clientIP = ui->cbxConnection->currentText().split(":")[0];
//     int clientPort = ui->cbxConnection->currentText().split(":")[1].toInt();
//     for (int i = 0; i < tcpClient.length(); i++)
//     {
//         if (tcpClient[i]->peerAddress().toString().split("::ffff:")[1] == clientIP && tcpClient[i]->peerPort() == clientPort)
//         {
//             tcpClient[i]->write(data.toLatin1());
//             return; // ip:port唯一，无需继续检索
//         }
//     }
// }
// void tcpServer::on_btnClear_clicked()
// {
//     for (int i = 0; i < tcpClient.length(); i++) // 断开所有连接
//     {
//         tcpClient[i]->disconnectFromHost();
//         bool ok = tcpClient[i]->waitForDisconnected(1000);
//         if (!ok)
//         {
//             // 处理异常
//         }
//         tcpClient.removeAt(i); // 从保存的客户端列表中取去除
//     }
//     Server->close(); // 不再监听端口
// }
tcpServer::~tcpServer()
{

    delete ui;
}