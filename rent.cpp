#include "rentalapp.h"
#include "ui_rentalapp.h"

void RentalApp::showRentTable()
{
    QSqlQuery *qry = new QSqlQuery(db);
    qry->prepare("SELECT Rental.ID_Rental AS ID, Client.Client_Name AS ФИО, Film.Film_Name AS [Название фильма], Rental.Giving_Date AS [Дата выдачи], "
"Rental.Returning_Date AS [Дата возврата], Rental.Days [Дни], "
"Rental.Total_Sum + dbo.ExpiredDaysSum(Rental.Giving_Date, Rental.Returning_Date, Rental.Days) AS [Полная сумма], "
"(Rental.Total_Sum * 0.8) AS Депозит,"
"CASE "
    "WHEN Rental.Returning_Date IS NOT NULL THEN 'Возвращен' "
    "WHEN (DATEDIFF(DAY, Rental.Giving_Date, CAST((SELECT GETDATE()) AS DATE)) - Rental.Days) <= 0 THEN 'В прокате' "
    "WHEN (DATEDIFF(DAY, Rental.Giving_Date, CAST((SELECT GETDATE()) AS DATE)) - Rental.Days) > 0 THEN 'Просрочен' "
"END AS Rental_Status "
"FROM Rental "
"INNER JOIN Client ON Client.ID_Client = Rental.ID_Client "
"INNER JOIN Film ON Film.ID_Film = Rental.ID_Film ");

    if(qry->exec())
    {
        model->setQuery(*qry);
        ui->rentTable->setModel(model);

        ui->rentTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
        ui->rentTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
        ui->rentTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
        ui->rentTable->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
        ui->rentTable->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);
        ui->rentTable->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Stretch);
        ui->rentTable->horizontalHeader()->setSectionResizeMode(6, QHeaderView::Stretch);
        ui->rentTable->horizontalHeader()->setSectionResizeMode(7, QHeaderView::Stretch);
        ui->rentTable->horizontalHeader()->setSectionResizeMode(8, QHeaderView::Stretch);
    }
    else
        QMessageBox::information(nullptr, "Ошибка", "Не удалось исполнить запрос.");

    delete qry;
}

void RentalApp::on_rentTable_doubleClicked(const QModelIndex &index)
{
    currentRow = index.row();

    if(model->record(currentRow).value("Дата возврата").toString() == "")
    {
        Confirmation *conf = new Confirmation();
        conf->setModal(true);
        conf->exec();

        if(conf->result() == 1)
        {
            QSqlQuery *qry = new QSqlQuery(db);
            qry->prepare("EXEC dbo.UpdateRentDate @id = " + model->record(currentRow).value("ID").toString());
            if(qry->exec())
            {

                showRentTable();
                delete qry;
            }
            else
                QMessageBox::information(nullptr, "Ошибка", "Не удалось исполнить запрос.");
        }

        delete conf;
    }
}

void RentalApp::on_makeRent_1_clicked()
{
    if (currentRow != 0)
    {
        if (model->record(currentRow).value("Кол-во дисков").toInt() != 0)
        {
            filmID = model->record(currentRow).value("ID").toInt();

            ui->stackedWidget->setCurrentIndex(4);

            QSqlQuery *qry = new QSqlQuery(db);
            qry->prepare("SELECT ID_Client AS ID, Client_Name AS ФИО, Phone_Number AS [Номер телефона], Birthday AS [День рождения], Passport AS Паспорт FROM Client");
            if(qry->exec())
            {
                model->setQuery(*qry);
                ui->selectionTable->setModel(model);
                ui->selectionTable->hideColumn(0);

                ui->selectionTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
                ui->selectionTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
                ui->selectionTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
                ui->selectionTable->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
                ui->selectionTable->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);
            }
            else
                QMessageBox::information(nullptr, "Ошибка", "Не удалось исполнить запрос.");

            delete qry;
        }
        else
            QMessageBox::information(nullptr, "Ошибка", "Отсутствуют диски с этим фильмом.");
    }
    else
        QMessageBox::information(nullptr, "Ошибка", "Не выбран фильм.");
}

void RentalApp::on_selectionTable_clicked(const QModelIndex &index)
{
    currentRow = index.row();
}


void RentalApp::on_makeRent_2_clicked()
{
    if (currentRow != 0 && ui->days->text().isEmpty() == false)
    {
        QSqlQuery *qry = new QSqlQuery(db);
        qry->prepare("EXEC dbo.AddNewRent @client = " + model->record(currentRow).value("ID").toString() + ", @film = " + QString::number(filmID) +
                     ", @givingDate = '" + QDate::currentDate().toString(Qt::ISODate) + "', @days = " + ui->days->text());
        if(qry->exec())
        {
            ui->stackedWidget->setCurrentIndex(0);
        }
        else
            QMessageBox::information(nullptr, "Ошибка", "Не удалось исполнить запрос.");

        delete qry;
    }
    else
        QMessageBox::information(nullptr, "Ошибка", "Проверьте выбранные данные.");

    ui->days->clear();
}

void RentalApp::showExpiredTable()
{
    QSqlQuery *qry = new QSqlQuery(db);
    qry->prepare("SELECT Client.Client_Name AS ФИО, Film.Film_Name AS [Название фильма], Rental.Giving_Date AS [Дата выдачи], Rental.Days AS [Дни], "
"(DATEDIFF(DAY, Rental.Giving_Date, CAST((SELECT GETDATE()) AS DATE)) - Rental.Days) AS [Просроченные дни], "
"dbo.ExpiredDaysSum(Rental.Giving_Date, Rental.Returning_Date, Rental.Days) AS Штраф "
"FROM Rental "
"INNER JOIN Client ON Client.ID_Client = Rental.ID_Client "
"INNER JOIN Film ON Film.ID_Film = Rental.ID_Film "
"WHERE (DATEDIFF(DAY, Rental.Giving_Date, CAST((SELECT GETDATE()) AS DATE)) - Rental.Days) > 0 AND (Rental.Returning_Date IS NULL)");

    if(qry->exec())
    {
        model->setQuery(*qry);
        ui->expiredTable->setModel(model);

        ui->expiredTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
        ui->expiredTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
        ui->expiredTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
        ui->expiredTable->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
        ui->expiredTable->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);
        ui->expiredTable->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Stretch);
    }
    else
        QMessageBox::information(nullptr, "Ошибка", "Не удалось исполнить запрос.");

    delete qry;
}
