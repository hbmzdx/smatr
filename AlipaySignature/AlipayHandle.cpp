#include "AlipayHandle.h"
#include<QDebug>
AlipayHandle::AlipayHandle(QObject *parent) : QObject(parent),
    timer(new QTimer(this))
{
    connect(timer,&QTimer::timeout,this,&AlipayHandle::slotTimeOut);
}

QByteArray AlipayHandle::httpHandle()
{
    QString url(("https://openapi-sandbox.dl.alipaydev.com/gateway.do?"));
    QString formData=packageFormData();
    url+=formData;
    url+=QString("&sign=");
    QString filePath=QCoreApplication::applicationDirPath()+QString("/private.pem");
    RSA *prsa=AlipaySignature::getRsaFromFile(filePath.toStdString());
    QString signData=QString ::fromStdString(AlipaySignature::sign(prsa,formData.toStdString()));
    signData=QString(QUrl::toPercentEncoding(signData));
    url +=signData;

    QNetworkRequest request(url);
    QNetworkAccessManager manager;
    QNetworkReply *reply=manager.get(request);

    QEventLoop eventLoop;
    connect(reply,&QNetworkReply::finished,&eventLoop,&QEventLoop::quit);
    eventLoop.exec();
    QByteArray responseData=reply->readAll();
    return responseData;

}

QString AlipayHandle::packageJsonData()
{
    QJsonObject data;
    data.insert(QString("out_trade_no"),myTime);
    data.insert(QString("total_amount"),netWorkCommData.price);
    data.insert(QString("subject"),netWorkCommData.name);

    QJsonDocument document(data);
    QString jsonData = document.toJson();
    jsonData.remove(QString("\n"));
    jsonData.remove(QString(" "));
    return jsonData;
}

QString AlipayHandle::packageFormData()
{
    QString formData=QString("app_id=9021000141612935");
    formData +=QString("&biz_content=");
    formData +=packageJsonData();
    formData +=QString("&charset=UTF-8&method=");
    formData +=tradeType;
    formData +=QString("&sign_type=RSA2&timestamp=");
    formData +=myTime;
    formData +=QString("&version=1.0");
    return formData;
}

QString AlipayHandle::parseJsonData()
{
    QJsonParseError parseError;
    QByteArray responseData=httpHandle();
    qDebug()<<responseData;
    QJsonDocument document=QJsonDocument::fromJson(responseData,&parseError);
    if(parseError.error != QJsonParseError::NoError)
    {
        qDebug()<<"Parse Json Document Fail"<<parseError.errorString();
        return QString{};
    }

    QJsonObject totalObject=document.object();
    if(tradeType == QString("alipay.trade.precreate")){
        QJsonObject eachObject=totalObject["alipay_trade_precreate_response"].toObject();
        QString code0=eachObject["code"].toString();
        QString msg=eachObject["msg"].toString();
        QString qrCode=eachObject["qr_code"].toString();
        //qDebug()<<code<<msg<<qrCode;
        return  qrCode;
    }
    else if(tradeType == QString("alipay.trade.query")){
        QJsonObject eachObject=totalObject["alipay_trade_query_response"].toObject();
        QString code=eachObject["code"].toString();
        QString msg=eachObject["msg"].toString();
        if((code == QString("10000")) && (msg==QString("Success")))
        {
            QString status=eachObject["trade_status"].toString();
            return status;
        }else if(msg==QString("Service Currently Unavailable")){
            return QString("PAY_SUCCESS");
        }
        QString status=eachObject["trade_status"].toString();
        //qDebug()<<code<<msg<<status;
    }
    return QString {};
}

void AlipayHandle::slotRecvCommodityID(int id)
{
    CommodityHandle *ch=CommodityHandle::getInstance();
    netWorkCommData =ch ->commMapGetValueByID(id);
    myTime =QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    tradeType =QString ("alipay.trade.precreate");
    // httpHandle();
    //parseJsonData();
    emit signalSendQrImageAndPrice(AlipaySignature::show2DBarcode(parseJsonData().toUtf8())
                                   ,netWorkCommData.price);
    timer ->start(2000);
}

void AlipayHandle::slotTimeOut()
{
    tradeType =QString("alipay.trade.query");
    if( parseJsonData() ==QString("TRADE_SUCCESS")){
        timer->stop();
        qDebug()<<"Pay success";
        CommodityHandle *ch=CommodityHandle::getInstance();
        QVector<CommodityProperty> tmp;
        --netWorkCommData.count;
        tmp.push_back(netWorkCommData);
        ch->saveCommodityCountChange(tmp);
        emit signalPaySuccess(netWorkCommData.name);
    }

}
