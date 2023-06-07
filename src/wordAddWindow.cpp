#include "wordAddWindow.h"

wordAddWindow::wordAddWindow(QWidget *parent) : QMainWindow(parent), father((class word_game *)parent), ui(new Ui_wordAddWindow)
{
    ui->setupUi(this);

    connect(ui->confirmButton, SIGNAL(clicked()), this, SLOT(slotConfirmButtonClick()));
}

wordAddWindow::~wordAddWindow()
{
    delete ui;
}

void wordAddWindow::slotConfirmButtonClick()
{
    QString tmp = ui->wordLine->text();
    if ((father->wordLists.at(0)).contains(tmp))
    {
        QMessageBox::information(this,
                                 tr("单词重复"),
                                 tr("单词添加失败存在于level 1"),
                                 QMessageBox::Ok | QMessageBox::Cancel,
                                 QMessageBox::Ok);
        return;
    }
    else if ((father->wordLists.at(1)).contains(tmp))
    {
        QMessageBox::information(this,
                                 tr("单词重复"),
                                 tr("单词添加失败存在于level 2"),
                                 QMessageBox::Ok | QMessageBox::Cancel,
                                 QMessageBox::Ok);
        return;
    }
    else if ((father->wordLists.at(2)).contains(tmp))
    {
        QMessageBox::information(this,
                                 tr("单词重复"),
                                 tr("单词添加失败存在于level 3"),
                                 QMessageBox::Ok | QMessageBox::Cancel,
                                 QMessageBox::Ok);
        return;
    }

    if (father->ui->levelBox->currentText() == QString("level 1"))
    {

        (father->wordLists.at(0)).insert(tmp);
        QMessageBox::information(this,
                                 tr("成功"),
                                 tr("该单词添加到level 1"),
                                 QMessageBox::Ok | QMessageBox::Cancel,
                                 QMessageBox::Ok);
        father->qInformation->incQusetionCounts();
        father->printfQuizmaster();
        return;
    }
    else if (father->ui->levelBox->currentText() == QString("level 2"))
    {
        (father->wordLists.at(1)).insert(tmp);
        QMessageBox::information(this,
                                 tr("成功"),
                                 tr("该单词添加到level 2"),
                                 QMessageBox::Ok | QMessageBox::Cancel,
                                 QMessageBox::Ok);
        father->qInformation->incQusetionCounts();
        father->printfQuizmaster();

        return;
    }
    else if (father->ui->levelBox->currentText() == QString("level 3"))
    {
        (father->wordLists.at(2)).insert(tmp);
        QMessageBox::information(this,
                                 tr("成功"),
                                 tr("该单词添加到level 3"),
                                 QMessageBox::Ok | QMessageBox::Cancel,
                                 QMessageBox::Ok);
        father->qInformation->incQusetionCounts();
        father->printfQuizmaster();
        return;
    }
    else
    {
        ui->wordLine->setText("没识别出来");
    }
}