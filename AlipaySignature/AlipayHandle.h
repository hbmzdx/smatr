#ifndef ALIPAYHANDLE_H
#define ALIPAYHANDLE_H
//网络处理文件
#include <QObject>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include<QNetworkReply>
#include <QEventLoop>

#include"CommodityHandle.h"

#include<QJsonDocument>
#include<QJsonParseError>
#include<QJsonObject>
#include<QJsonArray>

#include"AlipaySignature.h"
#include<QCoreApplication>

#include<QTimer>
class AlipayHandle : public QObject
{
    Q_OBJECT
public:
    explicit AlipayHandle(QObject *parent = nullptr);
    QByteArray httpHandle();

    QString packageJsonData();
    QString packageFormData(); //表单数据

    QString parseJsonData();

signals:
    void signalSendQrImageAndPrice(QImage image,double price);
    void signalPaySuccess(QString name);

public slots:
    void slotRecvCommodityID(int id);
    void slotTimeOut();
private:
    CommodityProperty netWorkCommData;
    QString myTime;
    QString tradeType; //交易类型
    QTimer *timer;
};

#endif // ALIPAYHANDLE_H
