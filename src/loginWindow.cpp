#include "loginWindow.h"

loginWindow::loginWindow(QWidget *parent) : QMainWindow(parent), father((class word_game *)parent), ui(new Ui_loginWindow)
{
    ui->setupUi(this);
    QRegExp regExp("[a-zA-Z]+");
    QValidator *validator = new QRegExpValidator(regExp, this);
    ui->passwordLine->setValidator(validator);
    ui->idLine->setValidator(validator);
    connect(ui->loginButton, SIGNAL(clicked()), this, SLOT(loginButtonClick()));
    connect(ui->registButton, SIGNAL(clicked()), this, SLOT(registButtonClick()));
}

loginWindow::~loginWindow()
{
    delete ui;
}

void loginWindow::loginButtonClick()
{
    if (ui->challengerRadio->isChecked() ^ ui->quizmasterRadio->isChecked())
    {
        QString id = ui->idLine->text();
        QString password = ui->passwordLine->text();
        if (id.size() <= 5 || password.size() <= 5)
        {
            QMessageBox::information(this,
                                     tr("失败!"),
                                     tr("账号或密码为大于5的英文字母"),
                                     QMessageBox::Ok | QMessageBox::Cancel,
                                     QMessageBox::Ok);
            return;
        }
        if (ui->challengerRadio->isChecked())
        {
            for (size_t i = 0; i < father->gameChallengers.size(); i++)
            {
                if ((father->gameChallengers.at(i)).getid() == id)
                {
                    if ((father->gameChallengers.at(i)).getpassword() == password)
                    {
                        father->me = CHALLENGER;
                        (*father).cInformation = &(father->gameChallengers.at(i));
                        QMessageBox::information(this,
                                                 tr("登录成功"),
                                                 tr("yes"),
                                                 QMessageBox::Ok | QMessageBox::Cancel,
                                                 QMessageBox::Ok);
                        this->hide();
                        emit loginSignal();
                        return;
                    }
                    else
                    {
                        QMessageBox::information(this,
                                                 tr("密码错误"),
                                                 tr("请重新输入"),
                                                 QMessageBox::Ok | QMessageBox::Cancel,
                                                 QMessageBox::Ok);
                        return;
                    }
                }
            }
            QMessageBox::information(this,
                                     tr("没有此ID"),
                                     tr("请重新输入"),
                                     QMessageBox::Ok | QMessageBox::Cancel,
                                     QMessageBox::Ok);
        }
        else
        {
            for (size_t i = 0; i < father->gameQuizmasters.size(); i++)
            {
                if ((father->gameQuizmasters.at(i)).getid() == id)
                {
                    if ((father->gameQuizmasters.at(i)).getpassword() == password)
                    {
                        father->me = GAMEMASTER;
                        father->qInformation = &(father->gameQuizmasters.at(i));
                        QMessageBox::information(this,
                                                 tr("登录成功"),
                                                 tr("yes"),
                                                 QMessageBox::Ok | QMessageBox::Cancel,
                                                 QMessageBox::Ok);
                        this->hide();
                        emit loginSignal();
                        return;
                    }
                    else
                    {
                        QMessageBox::information(this,
                                                 tr("密码错误"),
                                                 tr("请重新输入"),
                                                 QMessageBox::Ok | QMessageBox::Cancel,
                                                 QMessageBox::Ok);
                        return;
                    }
                }
            }
            QMessageBox::information(this,
                                     tr("没有此ID"),
                                     tr("请重新输入"),
                                     QMessageBox::Ok | QMessageBox::Cancel,
                                     QMessageBox::Ok);
        }
    }
    else
    {
        QMessageBox::information(this,
                                 tr("请选择一种参与者"),
                                 tr("请重新选择"),
                                 QMessageBox::Ok | QMessageBox::Cancel,
                                 QMessageBox::Ok);
    }
}
void loginWindow::registButtonClick()
{
    if (ui->challengerRadio->isChecked() ^ ui->quizmasterRadio->isChecked())
    {

        QString id = ui->idLine->text();
        QString password = ui->passwordLine->text();
        if (id.size() <= 5 || password.size() <= 5)
        {
            QMessageBox::information(this,
                                     tr("失败!"),
                                     tr("账号或密码为大于5的英文字母"),
                                     QMessageBox::Ok | QMessageBox::Cancel,
                                     QMessageBox::Ok);
            return;
        }
        if (ui->challengerRadio->isChecked())
        {
            for (size_t i = 0; i < father->gameChallengers.size(); i++)
            {
                if ((father->gameChallengers.at(i)).getid() == id)
                {
                    QMessageBox::information(this,
                                             tr("此ID已经存在"),
                                             tr("请重新输入"),
                                             QMessageBox::Ok | QMessageBox::Cancel,
                                             QMessageBox::Ok);
                    return;
                }
            }
            father->gameChallengers.push_back({id, password, 0, 0, 0});
            QMessageBox::information(this,
                                     tr("注册成功"),
                                     tr("yes"),
                                     QMessageBox::Ok | QMessageBox::Cancel,
                                     QMessageBox::Ok);
        }
        else
        {
            for (size_t i = 0; i < father->gameQuizmasters.size(); i++)
            {
                if ((father->gameQuizmasters.at(i)).getid() == id)
                {
                    QMessageBox::information(this,
                                             tr("此ID已经存在"),
                                             tr("请重新输入"),
                                             QMessageBox::Ok | QMessageBox::Cancel,
                                             QMessageBox::Ok);
                    return;
                }
            }
            father->gameQuizmasters.push_back({id, password, 0, 0});
            QMessageBox::information(this,
                                     tr("注册成功"),
                                     tr("yes"),
                                     QMessageBox::Ok | QMessageBox::Cancel,
                                     QMessageBox::Ok);
        }
    }
    else
    {
        QMessageBox::information(this,
                                 tr("请选择一种参与者"),
                                 tr("请重新选择"),
                                 QMessageBox::Ok | QMessageBox::Cancel,
                                 QMessageBox::Ok);
    }
}