#include "ShopInterface.h"
#include "ui_ShopInterface.h"

ShopInterface::ShopInterface(QWidget *parent) :
    AbstractInterface(parent),
    ui(new Ui::ShopInterface),
    alipay(new AlipayHandle),
    thr(new QThread(this))
{
    ui->setupUi(this);
    connect(this,&ShopInterface::signalSendCommodityIDtoNetWork,alipay,&AlipayHandle::slotRecvCommodityID);
    connect(alipay,&AlipayHandle::signalSendQrImageAndPrice,
            this,&ShopInterface::slotRecvQrImageAndPrice);
    connect(alipay,&AlipayHandle::signalPaySuccess,
            this,&ShopInterface::slotPaySuccess);

    ui->quickWidget->setSource(QUrl(QString("qrc:/qml.qml")));
    QQuickItem *item=ui->quickWidget->rootObject();
    connect(this,SIGNAL(signalSendImagePath(QString)),
            item,SIGNAL(signalRecvImagePath(QString)));
    connect(item,SIGNAL(signalSendAnimationFinish()),
            this,SLOT(slotRecvAnimationFinish()));
    thr->start();
    alipay->moveToThread(thr);
}

ShopInterface::~ShopInterface()
{
    thr->quit();
    thr->wait();
    delete ui;
    delete alipay;
}

void ShopInterface::resetWidget()
{
    ui->listWidget->clear();
    ui->label_price->clear();
    ui->label_qrcode->clear();
}

void ShopInterface::initListWidget()
{
    ui->listWidget->setIconSize(QSize(100, 100));  //修改图标大小
    ui->listWidget->setViewMode(QListView::IconMode); //设置为图标模式，上图下字
}

void ShopInterface::insertCommodityIntoListWidget()
{
    ui->stackedWidget->setCurrentIndex(JumpType::COMMODITYSHOP);
    CommodityHandle *ch=CommodityHandle::getInstance();
    QList<CommodityProperty> tmp=ch->commMapGetAllvalues();
    for(auto iter:tmp)
    {
        if(iter.count == 0)
            continue;

//    QString path =QCoreApplication::applicationDirPath();
//       path +=QString("/Commodity/Img/");
//       path += commProperty.name;
//       path +=QString(".png");
    QString path=QString("%1%2%3%4").arg(QCoreApplication::applicationDirPath()).
            arg("/Commodity/Img/").arg(iter.name).arg(".png");
    qDebug()<<path;
   // path +="\r\n";
    QString name=QString("%1%2%3%4").arg(iter.name)
            .arg("单价为").arg(iter.price).arg("$");

    QListWidgetItem *item = new QListWidgetItem(QIcon(path), QString(name));  //初始化条目
    item->setSizeHint(QSize(150, 150)); //设置条目大小
    item->setFlags(Qt::ItemIsEnabled); //使能条目，可以与用户交互
    item->setData(Qt::UserRole, iter.id);  //设置一个用户数据

    ui->listWidget->addItem(item); //添加条目到列表中
    }
}

void ShopInterface::slotRecvQrImageAndPrice(QImage image, double price)
{
    ui->label_price->setText(QString("总价为%1元").arg(price));
    QPixmap pixmap=QPixmap::fromImage(image);
    pixmap =pixmap.scaled(ui->label_qrcode->size(),Qt::IgnoreAspectRatio);
    ui->label_qrcode->setPixmap(pixmap);
}

void ShopInterface::slotPaySuccess(QString name)
{
    QString path=QString("file:///")+QCoreApplication::applicationDirPath()
            +QString("/Commodity/Img/");
    path +=QString("%1.png").arg(name);
    ui->stackedWidget->setCurrentIndex(JumpType::COMMODITYANIMATION);
    emit signalSendImagePath(path);

}

void ShopInterface::slotRecvAnimationFinish()
{
    resetWidget();
    initListWidget();
    insertCommodityIntoListWidget();
    ui->stackedWidget->setCurrentIndex(JumpType::COMMODITYSHOP);
}



void ShopInterface::on_listWidget_itemClicked(QListWidgetItem *item)
{
    ui->stackedWidget->setCurrentIndex(JumpType::COMMODITYQRCODE);
    // qDebug()<<item->data(Qt::UserRole).toInt();  //使用用户数据
    emit signalSendCommodityIDtoNetWork(item->data(Qt::UserRole).toInt());
}
