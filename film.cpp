#include "rentalapp.h"
#include "ui_rentalapp.h"

void RentalApp::showFilmTable()
{
    ui->name->clear();
    ui->producer->clear();
    ui->year->clear();
    ui->age->clear();
    ui->price->clear();
    ui->quantity->clear();

    QSqlQuery *qry = new QSqlQuery(db);

    qry->prepare("SELECT ID_Genre, Genre_Name FROM Genre");
    if (qry->exec())
        while(qry->next())
            ui->genre->addItem(qry->value(1).toString());

    qry->prepare("SELECT Film.ID_Film AS ID, Film.Film_Name AS Название, Genre.Genre_Name AS Жанр, Film.Producer AS Продюсер, "
"Film.Release_Year AS Год, Film.Age_Restriction AS Ограничение, Film.Price_per_Day AS [Цена в день], Film.Quantity AS [Кол-во дисков] "
"FROM Film, Genre WHERE Film.ID_Genre = Genre.ID_Genre");
    if(qry->exec())
    {
        model->setQuery(*qry);
        ui->filmTable->setModel(model);
        ui->filmTable->hideColumn(0);

        ui->filmTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
        ui->filmTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
        ui->filmTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
        ui->filmTable->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
        ui->filmTable->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);
        ui->filmTable->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
        ui->filmTable->horizontalHeader()->setSectionResizeMode(6, QHeaderView::Stretch);
        ui->filmTable->horizontalHeader()->setSectionResizeMode(7, QHeaderView::ResizeToContents);
    }
    else
        QMessageBox::information(nullptr, "Ошибка", "Не удалось исполнить запрос.");

    delete qry;
}

void RentalApp::on_filmTable_clicked(const QModelIndex &index)
{
    currentRow = index.row();
    ui->name->setText(model->record(currentRow).value("Название").toString());
//    ui->name->setReadOnly(true);
    ui->genre->setCurrentText(model->record(currentRow).value("Жанр").toString());
    ui->producer->setText(model->record(currentRow).value("Продюсер").toString());
    ui->year->setText(model->record(currentRow).value("Год").toString());
    ui->age->setText(model->record(currentRow).value("Ограничение").toString());
    ui->price->setText(model->record(currentRow).value("Цена в день").toString());
    ui->quantity->setText(model->record(currentRow).value("Кол-во дисков").toString());
}

void RentalApp::on_addFilmBtn_clicked()
{
    QString name = ui->name->text();
    QString genre = ui->genre->currentText();
    QString producer = ui->producer->text();
    QString year = ui->year->text();
    QString age = ui->age->text();
    QString price = ui->price->text();
    QString quantity = ui->quantity->text();

    if (name.isEmpty() || genre.isEmpty() || producer.isEmpty() || year.isEmpty() || age.isEmpty() || price.isEmpty() || quantity.isEmpty())
    {
        QMessageBox::information(nullptr, "Ошибка", "Не все требуемые поля заполнены.");
    }
    else
    {
        QSqlQuery *qry = new QSqlQuery(db);
        qry->prepare("EXEC dbo.AddNewFilm @name = '" + name + "', @genre = " + QString::number(ui->genre->currentIndex() + 1) + ", @producer = '" + producer +
                     "', @year = " + year + ", @age = '" + age + "', @price = " + price + ", @quantity = " + quantity);
        if(qry->exec())
        {
            showFilmTable();
            delete qry;
        }
        else
            QMessageBox::information(nullptr, "Ошибка", "Не удалось исполнить запрос.");
    }
}

void RentalApp::on_UpdateFilmBtn_clicked()
{
    QString price = ui->price->text();
    QString quantity = ui->quantity->text();

    QSqlQuery *qry = new QSqlQuery(db);
    qry->prepare("EXEC dbo.UpdateFilm @id = " + model->record(currentRow).value("ID").toString() + ", @price = " + price + ", @quantity = " + quantity);
    if(qry->exec())
    {
        showFilmTable();
        delete qry;
    }
    else
        QMessageBox::information(nullptr, "Ошибка", "Не удалось исполнить запрос.");
}

void RentalApp::on_DeleteFilmBtn_clicked()
{
    QSqlQuery *qry = new QSqlQuery(db);
    qry->prepare("EXEC dbo.DeleteFilm @id = " + model->record(currentRow).value("ID").toString());
    if(qry->exec())
    {
        showFilmTable();
        delete qry;
    }
    else
        QMessageBox::information(nullptr, "Ошибка", "Не удалось исполнить запрос.");
}
