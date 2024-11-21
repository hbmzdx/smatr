#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDebug"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    ,timer(new QTimer(this))
    ,countDown(15)
{
    ui->setupUi(this);

    //**********
    //基类转为派生类-> dynamic_cast
    for(int i=0;i < ui->stackedWidget->count();++i)
    {
        AbstractInterface *abstract =
                dynamic_cast<AbstractInterface *>(ui->stackedWidget->widget(i));


    if(abstract==nullptr)
    {
        qDebug()<<"Index" << i<< "Connect Failed";
    }
    else
    {
        connect(abstract,&AbstractInterface::signalJumpWidget,
                this,&MainWindow::slotJumpWidget);
    }
    }
    //connect(ui->advertisementWidget,&AdvertisementInterface::signalJumpWidget,
           // this,&MainWindow::slotJumpWidget);

    connect(ui->adminWidget,&AdminInterface::signalJumpWidget,
                this,&MainWindow::slotJumpWidget);
    connect(timer,&QTimer::timeout,this,&MainWindow::slotTimeOut);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotJumpWidget(int index)
{
    AbstractInterface *abstract =
            dynamic_cast<AbstractInterface *>(ui->stackedWidget->widget(index));
    if(abstract==nullptr)
    {
        qDebug()<<"Index" << index<< "Connect Failed";
    }
    abstract->resetWidget();

    if(timer->isActive())
    {
        timer->stop();
        countDown = 15;
        ui->label_countTime->clear();
    }
    switch (index) {
    case JumpType::SHOPWIDGET:
        ui->shopwidget->initListWidget();
        ui->shopwidget->insertCommodityIntoListWidget();
        break;
    case JumpType::ADMINWIDGET:
        ui->adminWidget->resetWidget();
        ui->adminWidget->initTableWidget();
        ui->adminWidget->initCommodity();
        timer->start(1000);
        break;
    case JumpType::LOGINWIDGET:
        break;
    case JumpType::SELECTWIDGET:
        break;
    default:
        break;
    }
    ui->stackedWidget->setCurrentIndex(index);
}

void MainWindow::slotTimeOut()
{
    ui->label_countTime->setText(QString("剩余时间仅剩%1秒").arg(--countDown));
    if(countDown == 0)
    {
        slotJumpWidget(JumpType::SELECTWIDGET);
    }
}


void MainWindow::on_pushButton_reset_clicked()
{
    for(int i=0;i < ui->stackedWidget->count();++i)
    {
        AbstractInterface *abstract =
                dynamic_cast<AbstractInterface *>(ui->stackedWidget->widget(i));


    if(abstract==nullptr)
    {
        qDebug()<<"Index" << i<< "Reset Failed";
    }
    else
    {
        abstract->resetWidget();
    }
    }
    slotJumpWidget(JumpType::ADVERTISEMENTWIDGET);
}
