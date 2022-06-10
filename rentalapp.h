#ifndef RENTALAPP_H
#define RENTALAPP_H

#include <QWidget>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QDate>

#include "confirmation.h"

namespace Ui {
class RentalApp;
}

class RentalApp : public QWidget
{
    Q_OBJECT

public:
    explicit RentalApp(QWidget *parent = nullptr);
    ~RentalApp();

    bool openConn(QString login, QString pass);
    void closeConn();

private slots:
    void on_exitBtn_clicked();
    void on_backBtn_clicked();
    void switchButton();

    void on_clientBtn_clicked();
    void on_filmBtn_clicked();
    void on_rentBtn_clicked();
    void on_expiredBtn_clicked();
    void on_extraBtn_clicked();
    void on_viewBtn_clicked();

    void showClientTable();
    void on_clientTable_clicked(const QModelIndex &index);
    void on_addClientBtn_clicked();
    void on_UpdateClientBtn_clicked();
    void on_DeleteClientBtn_clicked();

    void showFilmTable();
    void on_filmTable_clicked(const QModelIndex &index);
    void on_addFilmBtn_clicked();
    void on_UpdateFilmBtn_clicked();
    void on_DeleteFilmBtn_clicked();

    void on_makeRent_1_clicked();
    void on_selectionTable_clicked(const QModelIndex &index);
    void showRentTable();
    void on_makeRent_2_clicked();
    void on_rentTable_doubleClicked(const QModelIndex &index);

    void showExpiredTable();

    void showExtraTable();

    void showViewTable();

signals:
    void exit();

private:
    Ui::RentalApp *ui;

    int currentRow;
    int filmID;

    QSqlDatabase db;
    QSqlQueryModel *model;
};

#endif // RENTALAPP_H
