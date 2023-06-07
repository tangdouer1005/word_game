#include "listWindow.h"

listWindow::listWindow(QWidget *parent) : QMainWindow(parent), father((class word_game *)parent), ui(new Ui_listWindow)
{
    ui->setupUi(this);
}

void listWindow::flushList()
{
    QString id;
    if (father->cInformation)
    {
        id = father->cInformation->getid();
        sort(father->gameChallengers.begin(), father->gameChallengers.end());
        for (size_t i = 0; i < father->gameChallengers.size(); i++)
        {
            if (father->gameChallengers.at(i).getid() == id)
            {
                father->cInformation = &(father->gameChallengers.at(i));
                break;
            }
        }
        sort(father->gameQuizmasters.begin(), father->gameQuizmasters.end());
    }
    else if (father->qInformation)
    {
        id = father->qInformation->getid();
        sort(father->gameQuizmasters.begin(), father->gameQuizmasters.end());
        for (size_t i = 0; i < father->gameQuizmasters.size(); i++)
        {
            if (father->gameQuizmasters.at(i).getid() == id)
            {
                father->qInformation = &(father->gameQuizmasters.at(i));
                break;
            }
        }
        sort(father->gameChallengers.begin(), father->gameChallengers.end());
    }
    else
    {
        // wrong
    }
    ui->challengerList->clear();
    ui->quizmasterList->clear();
    for (auto iter : father->gameChallengers)
    {
        ui->challengerList->append(QString::number(iter.getLevel()) + "\t" + QString::number(iter.getExperience()) + "\t" + iter.getid());
    }
    for (auto iter : father->gameQuizmasters)
    {
        ui->quizmasterList->append(QString::number(iter.getLevel()) + "\t" + QString::number(iter.getqusetionsCounts()) + "\t" + iter.getid());
    }
    show();
}
listWindow::~listWindow()
{
    delete ui;
}