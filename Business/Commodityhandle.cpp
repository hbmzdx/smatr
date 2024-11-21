#include "CommodityHandle.h"
#include <QFile>
#include <QCoreApplication>

CommodityHandle *CommodityHandle::ch=nullptr;
pthread_mutex_t CommodityHandle::mutex =PTHREAD_MUTEX_INITIALIZER;
CommodityHandle::AutoRelease CommodityHandle::ar ;

CommodityHandle *CommodityHandle::getInstance()
{
    if(ch==nullptr)
    {
        pthread_mutex_lock(&mutex);
        if(ch==nullptr)
        {
            ch=new CommodityHandle;
        }
        pthread_mutex_unlock(&mutex);
    }
    return ch;
}

void CommodityHandle::realseInstance()
{
    if(ch!=nullptr)
    {
        delete ch;
        ch=nullptr;
    }
}

CommodityHandle::~CommodityHandle()
{

    CommMap.clear();
}

void CommodityHandle::initCommMap()
{
    QString path=QCoreApplication::applicationDirPath();
    path += QString("/Commodity/Commodity.json");

    QFile file(path);
    file.open(QIODevice::ReadOnly);
    QByteArray data = file.readAll();


    //解析json数据
    QJsonParseError parseError;
    QJsonDocument document=QJsonDocument::fromJson(data,&parseError);
    if(parseError.error != QJsonParseError::NoError)
    {
        qDebug()<<"Parse Json Document Fail"<<parseError.errorString();
    }

    QJsonArray totalArray=document.array();//如果最上面是大括号就是object，中括号是arry
    for(int i=0;i<totalArray.size();++i)
    {
        QJsonObject eachObj = totalArray[i].toObject();
        CommodityProperty tmp;
        tmp.id =eachObj["id"].toInt();
        tmp.name=eachObj["name"].toString();
        tmp.price=eachObj["price"].toDouble();
        tmp.type=eachObj["type"].toInt();
        CommMap.insert(tmp.id,tmp);
    }
    //qDebug()<<CommMap.size();
}

CommodityProperty  CommodityHandle::commMapGetValueByID(int id)
{
    return CommMap.value(id);
}

QList<CommodityProperty> CommodityHandle::commMapGetAllvalues()
{
    return CommMap.values();
}

QList<int> CommodityHandle::commMapGetAllKeys()
{
    return CommMap.keys();
}

bool CommodityHandle::saveCommodityCountChange(const QVector<CommodityProperty> &data)
{
    CommonityDB *cdb=CommonityDB::getInstance();
    if(data.size()==0)
    {
        return  false;
    }
    for(auto iter:data)
    {
        CommMap.find(iter.id)->count =iter.count;
    }
    return cdb->updateCommodityDataByVec(data);
}

CommodityHandle::CommodityHandle(QObject *parent) : QObject(parent)
{
    //initCommMap();
    CommonityDB *cdb=CommonityDB::getInstance();
    cdb->createCommodityTable();
    if(cdb->isCommodityTableEmpty())
    {
        initCommMap();
        cdb->insertCommodityAllData(CommMap);
        qDebug()<<"Empty";
    }else{
        cdb->queryCommodityAllData(CommMap);
        qDebug()<<"Not Empty";
    }
}

CommodityHandle::AutoRelease::~AutoRelease()
{
    if(ch!=nullptr)
    {
        delete ch;
        ch=nullptr;
    }
}
