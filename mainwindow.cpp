#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_CancelBtn_clicked()
{
    qApp->quit();
}

void MainWindow::on_LogBtn_clicked()
{
    QString login = ui->LoginLine->text();
    QString pass = ui->PassLine->text();

    if (pass != "")
    {
        rent = new RentalApp();
        connect(rent, SIGNAL(exit()), this, SLOT(deleteRent()));

        if(rent->openConn(login, pass))
        {
            rent->show();
            this->hide();
        }
        else
            QMessageBox::information(nullptr, "Ошибка", "Не удалось войти в БД.");
    }
    else
    {
        user = new UserSide();
        connect(user, SIGNAL(exit()), this, SLOT(deleteUser()));

        if(user->openConn(login, pass))
        {
            user->show();
            user->showFilmTable();
            this->hide();
        }
        else
            QMessageBox::information(nullptr, "Ошибка", "Не удалось войти в БД.");
    }
}

void MainWindow::deleteUser()
{
    this->show();
    delete user;
}

void MainWindow::deleteRent()
{
    this->show();
    delete rent;
}
