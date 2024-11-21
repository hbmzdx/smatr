#include "AdminInterface.h"
#include "ui_AdminInterface.h"


AdminInterface::AdminInterface(QWidget *parent) :
    AbstractInterface(parent),
    ui(new Ui::AdminInterface)
{
    ui->setupUi(this);
    //initTableWidget();
    //initCommodity();
}

AdminInterface::~AdminInterface()
{
    delete ui;
}

void AdminInterface::resetWidget()
{
    ui->tableWidget->clearContents();
    CommodityHandle::getInstance()->realseInstance();
    ui->tableWidget->setRowCount(0);
    commAllAddress.clear();
}

void AdminInterface::initTableWidget()
{
        ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑
        ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);//一次选一行
        ui->tableWidget->setFocusPolicy(Qt::NoFocus);	//去掉虚线框
        ui->tableWidget->setSortingEnabled(true);	//增加表头排序功能

        ui->tableWidget->horizontalHeader()->setFixedHeight(30);    //固定列头高度为30像素
        ui->tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}");   //设置tableWidget列头QSS样式表，背景为天蓝
        ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//表头拉伸
        ui->tableWidget->horizontalHeader()->setHighlightSections(false);       //选中列头不在高亮

        ui->tableWidget->verticalHeader()->hide();//隐藏行头
        ui->tableWidget->verticalHeader()->setHighlightSections(false);         //选中行头不在高亮



}

void AdminInterface::initCommodity()
{
    CommodityHandle *ch=CommodityHandle::getInstance();
    QList<int> keys=ch->commMapGetAllKeys();

    for(auto i:keys)
    {
    Commodity *cd=new Commodity(i);
    ui->tableWidget->insertRow(i-1);
    ui->tableWidget->setCellWidget(i-1,0,cd);
    ui->tableWidget->setRowHeight(i-1,120);
    commAllAddress.push_back(cd);
    }

}



void AdminInterface::on_pushButton_save_clicked()
{
    QVector<CommodityProperty> vecData;
    for(auto iter:commAllAddress)
    {
        if(iter ->getCountChanged())
        {
            CommodityProperty data=iter->getCurrentData();
            vecData.push_back(data);
            //qDebug()<<data.id<<data.name<<data.count;
        }
    }
    CommodityHandle *ch=CommodityHandle::getInstance();
    ch->saveCommodityCountChange(vecData);
    emit signalJumpWidget(JumpType::SELECTWIDGET);
    //emit signalJumpWidget(JumpType::SELECTWIDGET);
}
