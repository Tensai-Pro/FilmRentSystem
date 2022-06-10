#ifndef USERSIDE_H
#define USERSIDE_H

#include <QWidget>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>

namespace Ui {
class UserSide;
}

class UserSide : public QWidget
{
    Q_OBJECT

public:
    explicit UserSide(QWidget *parent = nullptr);
    ~UserSide();

    bool openConn(QString login, QString pass);
    void closeConn();
    void showFilmTable();

signals:
    void exit();

private slots:
    void on_exitBtn_clicked();

private:
    Ui::UserSide *ui;

    QSqlDatabase db;
    QSqlQueryModel *model;
};

#endif // USERSIDE_H
