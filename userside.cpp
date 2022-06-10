#include "userside.h"
#include "ui_userside.h"

UserSide::UserSide(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserSide)
{
    ui->setupUi(this);

    model = new QSqlQueryModel(this);

    QString host = "";   // Change to current host
    QString database = "CursedDb";
    db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName(QString("DRIVER={SQL Server};"
                "SERVER=%1;DATABASE=%2;Persist Security Info=true;")
              .arg(host, database));
}

UserSide::~UserSide()
{
    delete ui;
}

bool UserSide::openConn(QString login, QString pass)
{
    db.setUserName(login);
    db.setPassword(pass);

    return db.open();
}

void UserSide::closeConn()
{
    QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
}

void UserSide::on_exitBtn_clicked()
{
    if(true)
        db.close();
    closeConn();
    this->hide();
    emit exit();
}

void UserSide::showFilmTable()
{
    QSqlQuery *qry = new QSqlQuery(db);
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
