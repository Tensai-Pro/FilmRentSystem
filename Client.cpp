#include "rentalapp.h"
#include "ui_rentalapp.h"

void RentalApp::showClientTable()
{
    ui->fio->clear();
    ui->phone->clear();
    ui->bday->clear();
    ui->passport->clear();

    QSqlQuery *qry = new QSqlQuery(db);
    qry->prepare("SELECT ID_Client AS ID, Client_Name AS ФИО, Phone_Number AS [Номер телефона], Birthday AS [День рождения], Passport AS Паспорт FROM Client");
    if(qry->exec())
    {
        model->setQuery(*qry);
        ui->clientTable->setModel(model);
        ui->clientTable->hideColumn(0);

        ui->clientTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
        ui->clientTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
        ui->clientTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
        ui->clientTable->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
        ui->clientTable->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);
    }
    else
        QMessageBox::information(nullptr, "Ошибка", "Не удалось исполнить запрос.");

    delete qry;
}

void RentalApp::on_clientTable_clicked(const QModelIndex &index)
{
    currentRow = index.row();
    ui->fio->setText(model->record(currentRow).value("ФИО").toString());
    ui->phone->setText(model->record(currentRow).value("Номер телефона").toString());
    ui->bday->setText(model->record(currentRow).value("День рождения").toString());
    ui->passport->setText(model->record(currentRow).value("Паспорт").toString());
}

void RentalApp::on_addClientBtn_clicked()
{
    QString fio = ui->fio->text();
    QString phone = ui->phone->text();
    QString bday = ui->bday->text();
    QString passport = ui->passport->text();

    if (fio.isEmpty() || phone.isEmpty() || bday.isEmpty())
    {
        QMessageBox::information(nullptr, "Ошибка", "Не все требуемые поля заполнены.");
    }
    else
    {
        QSqlQuery *qry = new QSqlQuery(db);
        qry->prepare("EXEC dbo.AddNewClient @name = '" + fio + "', @phone = '" + phone + "', @bday = '" + bday + "', @passport = '" + passport + "'");
        if(qry->exec())
        {
            showClientTable();
            delete qry;
        }
        else
            QMessageBox::information(nullptr, "Ошибка", "Не удалось исполнить запрос.");
    }
}

void RentalApp::on_UpdateClientBtn_clicked()
{
    QString fio = ui->fio->text();
    QString phone = ui->phone->text();
    QString bday = ui->bday->text();
    QString passport = ui->passport->text();

    QSqlQuery *qry = new QSqlQuery(db);
    qry->prepare("EXEC dbo.UpdateClient @id = " + model->record(currentRow).value("ID").toString() + ", "
                    "@name = '" + fio + "', @phone = '" + phone + "', @bday = '" + bday + "', @passport = '" + passport + "'");
    if(qry->exec())
    {
        showClientTable();
        delete qry;
    }
    else
        QMessageBox::information(nullptr, "Ошибка", "Не удалось исполнить запрос.");
}

void RentalApp::on_DeleteClientBtn_clicked()
{
    QSqlQuery *qry = new QSqlQuery(db);
    qry->prepare("EXEC dbo.DeleteClient @id = " + model->record(currentRow).value("ID").toString());
    if(qry->exec())
    {
        showClientTable();
        delete qry;
    }
    else
        QMessageBox::information(nullptr, "Ошибка", "Не удалось исполнить запрос.");
}
