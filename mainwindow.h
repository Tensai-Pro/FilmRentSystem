#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QSqlDatabase>

#include "rentalapp.h"
#include "userside.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_CancelBtn_clicked();

    void on_LogBtn_clicked();

    void deleteUser();
    void deleteRent();

signals:

private:
    Ui::MainWindow *ui;

    RentalApp *rent;
    UserSide *user;
};
#endif // MAINWINDOW_H
