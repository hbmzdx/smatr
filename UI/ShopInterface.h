#ifndef SHOPINTERFACE_H
#define SHOPINTERFACE_H

#include "AbstractInterface.h"
#include"CommodityHandle.h"
#include<QListWidgetItem>
#include <QThread>
#include "AlipayHandle.h"
#include<QQuickItem>

namespace Ui {
class ShopInterface;
}

class ShopInterface : public AbstractInterface
{
    Q_OBJECT

public:
    explicit ShopInterface(QWidget *parent = nullptr);
    virtual ~ShopInterface() override;

    virtual void resetWidget() override;
    void initListWidget();
    void insertCommodityIntoListWidget();//插入

signals:
    void signalSendCommodityIDtoNetWork(int id);
    void signalSendImagePath(QString path);

public slots:
    void slotRecvQrImageAndPrice(QImage image,double price);
    void slotPaySuccess(QString name);
    void slotRecvAnimationFinish();

private slots:
    void on_listWidget_itemClicked(QListWidgetItem *item);

private:
    Ui::ShopInterface *ui;
    AlipayHandle *alipay;
    QThread *thr;
};

#endif // SHOPINTERFACE_H
