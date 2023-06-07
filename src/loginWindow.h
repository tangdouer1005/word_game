#ifndef LOGIN_WINDOW_H
#define LOGIN_WINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include "ui_loginWindow.h"
#include "participant.h"
#include "word_game.h"
class word_game;
class loginWindow : public QMainWindow
{
    Q_OBJECT

public:
    loginWindow(QWidget *parent);
    ~loginWindow();

private:
    Ui_loginWindow *ui;
    class word_game *father;

private slots:
    void loginButtonClick();
    void registButtonClick();
signals:
    void loginSignal();
};
#endif