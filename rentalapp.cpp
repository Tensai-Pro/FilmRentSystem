#include "rentalapp.h"
#include "ui_rentalapp.h"

RentalApp::RentalApp(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RentalApp)
{
    ui->setupUi(this);
    model = new QSqlQueryModel(this);

    ui->queries->addItems({"Коррелирующие и некоррелирующие подзапросы", "Запрос с Having", "Запрос с Any"});
    connect(ui->queries, SIGNAL(currentIndexChanged(int)), this, SLOT(showExtraTable()));
    ui->display->addItems({"По возрастанию", "По убыванию"});
    connect(ui->display, SIGNAL(currentIndexChanged(int)), this, SLOT(showViewTable()));

    QString host = "";   // Change to current host
    QString database = "CursedDb";
    db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName(QString("DRIVER={SQL Server};"
                "SERVER=%1;DATABASE=%2;Persist Security Info=true;")
              .arg(host, database));

    ui->fio->setValidator(new QRegularExpressionValidator(QRegularExpression("[а-яА-Я\\s]{1,}")));
    ui->phone->setValidator(new QRegularExpressionValidator(QRegularExpression("\\+7\\d{10}")));
    ui->bday->setValidator(new QRegularExpressionValidator(QRegularExpression("\\d{4}-\\d{2}-\\d{2}")));
    ui->passport->setValidator(new QRegularExpressionValidator(QRegularExpression("\\d{10}")));

    ui->stackedWidget->setCurrentIndex(0);

    ui->backBtn->hide();
    connect(ui->backBtn, SIGNAL(clicked()), this, SLOT(switchButton()));
}

RentalApp::~RentalApp()
{
    delete ui;
}

bool RentalApp::openConn(QString login, QString pass)
{
    db.setUserName(login);
    db.setPassword(pass);

    return db.open();
}

void RentalApp::closeConn()
{
    QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
}

void RentalApp::switchButton()
{
    ui->backBtn->hide();
    ui->exitBtn->show();
}

void RentalApp::on_exitBtn_clicked()
{
    if(true)
        db.close();
    closeConn();
    this->hide();
    emit exit();
}

void RentalApp::on_backBtn_clicked()
{
    qDebug() << ui->stackedWidget->currentIndex();
    ui->stackedWidget->setCurrentIndex(0);

    ui->backBtn->hide();
}

void RentalApp::on_clientBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);

    ui->exitBtn->hide();
    ui->backBtn->show();

    showClientTable();
}
void RentalApp::on_filmBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);

    ui->exitBtn->hide();
    ui->backBtn->show();

    showFilmTable();
}


void RentalApp::on_rentBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);

    ui->exitBtn->hide();
    ui->backBtn->show();

    showRentTable();
}

void RentalApp::on_expiredBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(5);

    ui->exitBtn->hide();
    ui->backBtn->show();

    showExpiredTable();
}

void RentalApp::on_extraBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(6);

    ui->exitBtn->hide();
    ui->backBtn->show();

    showExtraTable();
}

void RentalApp::on_viewBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(7);

    ui->exitBtn->hide();
    ui->backBtn->show();

    showViewTable();
}

void RentalApp::showExtraTable()
{
    QSqlQuery *qry = new QSqlQuery(db);

    if (ui->queries->currentIndex() == 0)
    {
        qry->prepare("SELECT subFilmTable.Film_Name AS [Навазние фильма], "
"(SELECT COUNT(Rental.ID_Rental) FROM Rental WHERE Rental.ID_Film = subFilmTable.ID_Film) AS [Кол-во прокатов] "
"FROM (SELECT ID_Film, Film_Name, Price_per_Day FROM Film WHERE Release_Year < 2010) AS subFilmTable "
"WHERE subFilmTable.Price_per_Day > (SELECT AVG(Price_per_Day) FROM Film)");
        if(qry->exec())
        {
            model->setQuery(*qry);
            ui->extraTable->setModel(model);

            ui->extraTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
            ui->extraTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
        }
        else
            QMessageBox::information(nullptr, "Ошибка", "Не удалось исполнить запрос.");
    }
    else if(ui->queries->currentIndex() == 1)
    {
        qry->prepare("SELECT Client.Client_Name AS ФИО, COUNT(ID_Rental) AS [Кол-во прокатов] "
"FROM Rental, Client "
"WHERE Client.ID_Client = Rental.ID_Client "
"GROUP BY Client.Client_Name "
"HAVING COUNT(Rental.ID_Rental) >= 3");
        if(qry->exec())
        {
            model->setQuery(*qry);
            ui->extraTable->setModel(model);

            ui->extraTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
            ui->extraTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
        }
        else
            QMessageBox::information(nullptr, "Ошибка", "Не удалось исполнить запрос.");
    }
    else if(ui->queries->currentIndex() == 2)
    {
        qry->prepare("SELECT Genre.Genre_Name AS Жанр, (SELECT COUNT(ID_Film) FROM Film WHERE Film.ID_Genre = Genre.ID_Genre) AS [Кол-во фильмов] "
"FROM Genre "
"WHERE ID_Genre = ANY "
                 "(SELECT ID_Genre FROM Film)");
        if(qry->exec())
        {
            model->setQuery(*qry);
            ui->extraTable->setModel(model);

            ui->extraTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
            ui->extraTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
        }
        else
            QMessageBox::information(nullptr, "Ошибка", "Не удалось исполнить запрос.");
    }

    delete qry;
}

void RentalApp::showViewTable()
{
    QSqlQuery *qry = new QSqlQuery(db);

    if(ui->display->currentText() == "По убыванию")
    {
        qry->prepare("SELECT Film_Name AS [Название фильма], Genre_Name AS Жанр, Giving_Date AS [Дата выдачи], Returning_Date AS [Дата возврата] "
"FROM Short_Rental ORDER BY Giving_Date DESC");

        if(qry->exec())
        {
            model->setQuery(*qry);
            ui->viewTable->setModel(model);
        }
        else
            qDebug() << "Failed to execute this query.";
    }
    else if (ui->display->currentText() == "По возрастанию")
    {
        qry->prepare("SELECT Film_Name AS [Название фильма], Genre_Name AS Жанр, Giving_Date AS [Дата выдачи], Returning_Date AS [Дата возврата] "
"FROM Short_Rental ORDER BY Giving_Date ASC");

        if(qry->exec())
        {
            model->setQuery(*qry);
            ui->viewTable->setModel(model);
        }
        else
            qDebug() << "Failed to execute this query.";
    }

    ui->viewTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->viewTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->viewTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->viewTable->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);

    delete qry;
}
