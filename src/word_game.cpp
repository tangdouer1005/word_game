#include "word_game.h"

word_game::word_game(QWidget *parent)
    : QMainWindow(parent), ui(new Ui_word_game)
{
    ui->setupUi(this);
    init();
    mlistWindow = new listWindow(this);
    mLoginWindow = new loginWindow(this);
    mGameWindow = new gameWindow(this, 1);

    mWordAddWindow = new wordAddWindow(this);
    mLoginWindow->show();

    // ui->showInformation->append("iter");
    // for (auto iter : wordLists[0])
    // {
    //     ui->showInformation->append(iter);
    // }

    connect(mLoginWindow, SIGNAL(loginSignal()), this, SLOT(show()));
    connect(mGameWindow, SIGNAL(gameOverSignal()), this, SLOT(slotTest()));
    connect(ui->showListButton, SIGNAL(clicked()), mlistWindow, SLOT(flushList()));
    connect(ui->playButton, SIGNAL(clicked()), this, SLOT(slotPlayButtonClick()));
    connect(ui->searchButton, SIGNAL(clicked()), this, SLOT(slotSearch()));
}

void word_game::slotTest()
{
    ui->showInformation->append(QString::number(((Challenger *)cInformation)->getExperience()));
}
word_game::~word_game()
{
    QFile wordListFile_1("./data/wordList_1.txt");
    if (!wordListFile_1.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        ui->showInformation->append("open wrong");
    }
    QTextStream outWordListFile_1(&wordListFile_1);
    // outWordListFile_1.setCodec("UTF-8"); // 确定编码格式
    for (auto iter : wordLists[0])
    {
        outWordListFile_1 << iter << " ";
    }
    wordListFile_1.close();

    QFile wordListFile_2("./data/wordList_2.txt");
    if (!wordListFile_2.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        ui->showInformation->append("open wrong");
    }
    QTextStream outWordListFile_2(&wordListFile_2);
    // outWordListFile_2.setCodec("UTF-8"); // 确定编码格式
    for (auto iter : wordLists[1])
    {
        outWordListFile_2 << iter << " ";
    }
    wordListFile_2.close();

    QFile wordListFile_3("./data/wordList_3.txt");
    if (!wordListFile_3.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        ui->showInformation->append("open wrong");
    }
    QTextStream outWordListFile_3(&wordListFile_3);
    // outWordListFile_3.setCodec("UTF-8"); // 确定编码格式
    for (auto iter : wordLists[2])
    {
        outWordListFile_3 << iter << " ";
    }
    wordListFile_3.close();

    QFile challengerFile("./data/challenger_ranking_list.txt");
    if (!challengerFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        ui->showInformation->append("open wrong");
    }
    QTextStream challengerOut(&challengerFile);
    for (auto iter : gameChallengers)
    {
        challengerOut << iter.getid() << " "
                      << iter.getpassword() << " "
                      << iter.getLevel() << " "
                      << iter.getExperience() << " "
                      << iter.getPassedLevels() << " \n";
    }
    challengerFile.close();

    QFile quizmasterFile("./data/quizmaster_ranking_list.txt");
    if (!quizmasterFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        ui->showInformation->append("open wrong quizmasterFile");
    }
    QTextStream quizmasterOut(&quizmasterFile);
    for (auto iter : gameQuizmasters)
    {
        quizmasterOut << iter.getid() << " "
                      << iter.getpassword() << " "
                      << iter.getLevel() << " "
                      << iter.getqusetionsCounts() << " \n";
    }
    quizmasterFile.close();
    delete ui;
}
void word_game::init()
{
    QFile challengerFile("./data/challenger_ranking_list.txt");
    if (!challengerFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
    }
    QTextStream challengerIn(&challengerFile);
    challengerIn.setCodec("UTF-8"); // 确定编码格式
    while (!challengerIn.atEnd())
    {
        QString id, password;
        int level, exp, passedLevels;
        challengerIn >> id >> password >> passedLevels >> exp >> level;
        if (id != QString(""))
        {
            gameChallengers.push_back({id, password, level, exp, passedLevels});
        }
    }
    challengerFile.close();

    QFile quizmasterFile("./data/quizmaster_ranking_list.txt");
    if (!quizmasterFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
    }
    QTextStream quizmasterIn(&quizmasterFile);
    quizmasterIn.setCodec("UTF-8"); // 确定编码格式
    while (!quizmasterIn.atEnd())
    {
        QString id, password;
        int level, questionsCount;
        quizmasterIn >> id >> password >> level >> questionsCount;
        if (id != QString(""))
        {
            gameQuizmasters.push_back({id, password, questionsCount, level});
        }
    }
    quizmasterFile.close();

    sort(gameChallengers.begin(), gameChallengers.end());
    sort(gameQuizmasters.begin(), gameQuizmasters.end());
    for (auto iter : gameChallengers)
    {
        challengerId.insert(iter.getid());
    }

    for (auto iter : gameQuizmasters)
    {
        quizmastersId.insert(iter.getid());
    }

    // level 1单词表导入
    QFile wordListFile_1("./data/wordList_1.txt");
    if (!wordListFile_1.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        ui->showInformation->append("open wrong");
    }
    QTextStream inWordListFile_1(&wordListFile_1);
    inWordListFile_1.setCodec("UTF-8"); // 确定编码格式
    wordLists.push_back(QSet<QString>());
    while (!inWordListFile_1.atEnd())
    {
        QString tmp;
        inWordListFile_1 >> tmp;
        if (tmp != QString(""))
            wordLists[0].insert(tmp);
    }
    wordListFile_1.close();

    // level 2单词表导入
    QFile wordListFile_2("./data/wordList_2.txt");
    if (!wordListFile_2.open(QIODevice::ReadOnly | QIODevice::Text))
    {
    }
    QTextStream inWordListFile_2(&wordListFile_2);
    inWordListFile_2.setCodec("UTF-8"); // 确定编码格式
    wordLists.push_back(QSet<QString>());
    while (!inWordListFile_2.atEnd())
    {
        QString tmp;
        inWordListFile_2 >> tmp;
        if (tmp != QString(""))
            wordLists[1].insert(tmp);
    }
    wordListFile_2.close();

    // level 3单词表导入
    QFile wordListFile_3("./data/wordList_3.txt");
    if (!wordListFile_3.open(QIODevice::ReadOnly | QIODevice::Text))
    {
    }
    QTextStream inWordListFile_3(&wordListFile_3);
    inWordListFile_3.setCodec("UTF-8"); // 确定编码格式
    wordLists.push_back(QSet<QString>());
    while (!inWordListFile_3.atEnd())
    {
        QString tmp;
        inWordListFile_3 >> tmp;
        if (tmp != QString(""))
            wordLists[2].insert(tmp);
    }
    wordListFile_3.close();
}

void word_game::slotPlayButtonClick()
{
    if (me == CHALLENGER)
    {
        if (ui->levelBox->currentText() == QString("level 1"))
        {
            mGameWindow->selectLevel(1);
        }
        else if (ui->levelBox->currentText() == QString("level 2"))
        {
            mGameWindow->selectLevel(2);
        }
        else if (ui->levelBox->currentText() == QString("level 3"))
        {
            mGameWindow->selectLevel(3);
        }
        else
        {
            // wrong
        }
        mGameWindow->show();
    }
    else
    {

        mWordAddWindow->show();
    }
}

// void word_game::slotRankButtonClick()
// {
//     mlistWindow->flushList();
//     mlistWindow->show();
// }
void word_game::slotSearch()
{
    QString id = ui->searchLine->text();
    if (ui->ch->isChecked() ^ ui->qu->isChecked())
    {
        do
        {
            if (ui->ch->isChecked())
            {
                for (auto iter : gameChallengers)
                {
                    if (iter.getid() == id)
                    {
                        ui->showInformation->clear();
                        ui->showInformation->append(iter.getid());
                        // ui->showInformation->append(cInformation->getpassword());
                        ui->showInformation->append(QString::number(iter.getExperience()));
                        ui->showInformation->append(QString::number(iter.getLevel()));
                        ui->showInformation->append(QString::number(iter.getPassedLevels()));
                        QMessageBox::information(this,
                                                 tr("查找成功"),
                                                 tr("显示成功"),
                                                 QMessageBox::Ok | QMessageBox::Cancel,
                                                 QMessageBox::Ok);
                        return;
                    }
                }
                QMessageBox::information(this,
                                         tr("查找失败"),
                                         tr("没有此用户"),
                                         QMessageBox::Ok | QMessageBox::Cancel,
                                         QMessageBox::Ok);
            }
            else
            {
                for (auto iter : gameQuizmasters)
                {
                    if (iter.getid() == id)
                    {
                        ui->showInformation->clear();
                        ui->showInformation->append(iter.getid());
                        // ui->showInformation->append(cInformation->getpassword());
                        ui->showInformation->append(QString::number(iter.getLevel()));
                        ui->showInformation->append(QString::number(iter.getqusetionsCounts()));
                        QMessageBox::information(this,
                                                 tr("查找成功"),
                                                 tr("显示成功"),
                                                 QMessageBox::Ok | QMessageBox::Cancel,
                                                 QMessageBox::Ok);
                        return;
                    }
                }
                QMessageBox::information(this,
                                         tr("查找失败"),
                                         tr("没有此用户"),
                                         QMessageBox::Ok | QMessageBox::Cancel,
                                         QMessageBox::Ok);
            }
        } while (0);
    }
    else
    {
        QMessageBox::information(this,
                                 tr("wrong"),
                                 tr("请选择一种查找对象"),
                                 QMessageBox::Ok | QMessageBox::Cancel,
                                 QMessageBox::Ok);
    }
}