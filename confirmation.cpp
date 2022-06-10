#include "confirmation.h"
#include "ui_confirmation.h"

Confirmation::Confirmation(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Confirmation)
{
    ui->setupUi(this);

    connect(ui->yesBtn, SIGNAL(clicked()), this, SLOT(accept()));
    connect(ui->noBtn, SIGNAL(clicked()), this, SLOT(reject()));
}

Confirmation::~Confirmation()
{
    delete ui;
}

