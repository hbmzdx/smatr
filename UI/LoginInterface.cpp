#include "LoginInterface.h"
#include "ui_LoginInterface.h"

LoginInterface::LoginInterface(QWidget *parent) :
    AbstractInterface(parent),
    ui(new Ui::LoginInterface)
{
    ui->setupUi(this);
}

LoginInterface::~LoginInterface()
{
    delete ui;
}

void LoginInterface::resetWidget()
{
    ui->lineEdit_account->clear();
    ui->lineEdit_password->clear();
}

void LoginInterface::on_pushButton_login_clicked()
{
    emit signalJumpWidget(JumpType::ADMINWIDGET);
}
