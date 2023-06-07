#include "gameWindow.h"

gameWindow::gameWindow(QWidget *parent, int level) : QMainWindow(parent), ui(new Ui_gameWindow), level(level), father((class word_game *)parent)
{

    ui->setupUi(this);
    mTimer = new QTimer(this);

    // father->printfChalenger();
    ui->answerLine->setReadOnly(true);
    // 设置 QLineEdit 只能输入字母
    QRegExp regExp("[a-zA-Z]+");
    QValidator *validator = new QRegExpValidator(regExp, this);
    ui->answerLine->setValidator(validator);
    connect(mTimer, SIGNAL(timeout()), this, SLOT(slotTimerOut()));
    connect(ui->startButton, SIGNAL(clicked()), this, SLOT(slotStartButtonClick()));
    connect(ui->confirmButton, SIGNAL(clicked()), this, SLOT(slotConfirmButtonClick()));
}

gameWindow::~gameWindow()
{
    delete ui;
}
void gameWindow::slotStartButtonClick()
{
    wordLists = {father->wordLists[0].toList(), father->wordLists[1].toList(), father->wordLists[2].toList()};
    for (int i = 0; i < 3; i++)
    {
        if (wordLists[i].size() < 5)
        {
            QMessageBox::information(this,
                                     tr("词库单词太少"),
                                     tr("请增加词库数量后再开始"),
                                     QMessageBox::Ok | QMessageBox::Cancel,
                                     QMessageBox::Ok);
            return;
        }
    }
    ui->show_level->setText("level 1");

    wordList = wordLists[0];
    ui->answerLine->setReadOnly(true);
    if (ui->startButton->text() == QString("开始"))
    {
        if (curStatus == PAUSE)
        {
            curStatus = preStatus;
        }
        else
        {
            std::random_device rd;  // 用于生成种子的随机数引擎，一般使用系统提供的硬件随机数生成器
            std::mt19937 gen(rd()); // 使用Mersenne Twister随机数引擎，随机数种子使用rd生成的
            std::uniform_int_distribution<> dis(0, wordList.size() - 1);
            int index = dis(gen);
            ui->showWordLabel->setText(wordList[index]);
            curString = wordList[index];
            wordList.removeAll(curString);
        }
        ui->startButton->setText(QString("暂停"));
        mTimer->start(SECOND);
        curStatus = SHOWWORD;
    }
    else
    {
        preStatus = curStatus;
        ui->startButton->setText(QString("开始"));
        curStatus = PAUSE;
        mTimer->stop();
    }
}
void gameWindow::slotTimerOut()
{
    switch (curStatus)
    {
    case UNSTART:
    case PAUSE:
        // 不应该出现
        // 啥事不该干
        break;
    case SHOWWORD:
        showTime--;
        if (0 == showTime)
        {
            showTime = SHOWTIME;
            curStatus = ANSWER;
            ui->answerLine->setReadOnly(false);
            ui->showWordLabel->setText(QString("请输入答案"));
        }
        break;
    case ANSWER:
        answerTime--;
        if (0 == answerTime)
        {
            answerTime = ANSWERTIME;
            curStatus = RESULT;
            ui->answerLine->setReadOnly(true);
            if (ui->answerLine->text() == curString)
            {
                ui->showWordLabel->setText(QString("回答正确"));
                (father->cInformation)->addExperience(level * 10 + (answerTime - SHOWTIME) * 2);
                father->printfChalenger();
                // 进行奖励
            }
            else
            {
                ui->showWordLabel->setText(QString("回答错误"));
            }
            ui->answerLine->clear();
        }
        break;
    case RESULT:
        resultTime--;
        if (0 == resultTime)
        {
            resultTime = RESULTTIME;
            curStatus = SHOWWORD;
            remainRounds--;
            if (remainRounds == 6)
            {
                wordList = wordLists[1];
                ui->show_level->setText("level 2");
            }
            else if (remainRounds == 3)
            {
                wordList = wordLists[2];
                ui->show_level->setText("level 3");
            }
            if (0 == remainRounds)
            {
                // 关闭界面进行结算
                QMessageBox::information(this,
                                         tr("游戏结束"),
                                         tr("您的结果"),
                                         QMessageBox::Ok | QMessageBox::Cancel,
                                         QMessageBox::Ok);
                ui->startButton->setText(QString("开始"));
                mTimer->stop();
                this->close();
                father->printfChalenger();
            }
            std::random_device rd;  // 用于生成种子的随机数引擎，一般使用系统提供的硬件随机数生成器
            std::mt19937 gen(rd()); // 使用Mersenne Twister随机数引擎，随机数种子使用rd生成的
            std::uniform_int_distribution<> dis(0, wordList.size() - 1);
            int index = dis(gen);
            ui->showWordLabel->setText(wordList[index]);
            curString = wordList[index];
            wordList.removeAll(curString);
        }
        break;
    default:
        break;
    }
}
void gameWindow::slotConfirmButtonClick()
{
    if (ui->answerLine->isReadOnly())
    {
        return;
    }
    answerTime = ANSWERTIME;
    curStatus = RESULT;
    ui->answerLine->setReadOnly(true);
    if (ui->answerLine->text() == curString)
    {
        ui->showWordLabel->setText(QString("回答正确"));
        (father->cInformation)->addExperience(level * 10 + (answerTime - SHOWTIME) * 2);
        father->printfChalenger();
        // 进行奖励
    }
    else
    {
        ui->showWordLabel->setText(QString("回答错误"));
    }

    father->printfChalenger();
    ui->answerLine->clear();
}