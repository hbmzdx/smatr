#include "Commodity.h"
#include "ui_Commodity.h"
#include <QDebug>
Commodity::Commodity(int id,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Commodity),
    isCountChanged(false)
{
    ui->setupUi(this);
    QString path =QCoreApplication::applicationDirPath();
       path +=QString("/Commodity/Img/");
       CommodityHandle *ch=CommodityHandle::getInstance();
       commProperty =ch->commMapGetValueByID(id);


       path += commProperty.name;
       path +=QString(".png");
        ui->label_name->setText(QString(commProperty.name));
        ui->spinBox_count->setValue(commProperty.count);
        QPixmap pixmap(path);
        pixmap = pixmap.scaled(ui->label_image->width(),ui->label_image->height()
                               ,Qt::KeepAspectRatioByExpanding);
        ui->label_image->setPixmap(pixmap);
}

Commodity::~Commodity()
{
    delete ui;
}

CommodityProperty Commodity::getCurrentData()
{
    return commProperty;
}

bool Commodity::getCountChanged()
{
    return isCountChanged;
}

void Commodity::setCountChanged(bool value)
{
    isCountChanged=value;
}

void Commodity::on_spinBox_count_valueChanged(int arg1)
{
    if(arg1 !=commProperty.count)
    {
        commProperty.count=arg1;
        isCountChanged=true;
    }
}
