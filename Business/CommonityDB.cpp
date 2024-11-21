#include "CommonityDB.h"
#include <QDebug>

CommonityDB *CommonityDB::ch=nullptr;
pthread_mutex_t CommonityDB::mutex =PTHREAD_MUTEX_INITIALIZER;
CommonityDB::AutoRelease CommonityDB::ar ;

CommonityDB *CommonityDB::getInstance()
{
    if(ch==nullptr)
    {
        pthread_mutex_lock(&mutex);
        if(ch==nullptr)
        {
            ch=new CommonityDB;
        }
        pthread_mutex_unlock(&mutex);
    }
    return ch;
}

void CommonityDB::realseInstance()
{
    if(ch!=nullptr)
    {
        delete ch;
        ch=nullptr;
    }
}

CommonityDB::~CommonityDB()
{
}

void CommonityDB::createCommodityTable()
{
    QSqlQuery query(database);
    query.prepare(QString("CREATE TABLE IF NOT EXISTS commodity"
                          "(id integer primary key, name text, "
                          "price real, type integer, count integer);"));
    if(!query.exec())
    {
        qDebug()<<"Create CommodityTable Failed"<<query.lastError();
    }
}

bool CommonityDB::insertCommodityAllData(const QMap<int, CommodityProperty> &data)
{
    QSqlQuery query(database);
    query.prepare(QString("INSERT INTO commodity(id,name,price,type,count) values"
                           "(:cid,:cname,:cprice,:ctype,:ccount);"));
    for(auto iter:data)
    {
        query.bindValue(":cid",iter.id);
        query.bindValue(":cname",iter.name);
        query.bindValue(":cprice",iter.price);
        query.bindValue(":ctype",iter.type);
        query.bindValue(":ccount",iter.count);
        if(!query.exec()){
            return false;
        }
    }
    return true;
}

bool CommonityDB::isCommodityTableEmpty()
{
    QSqlQuery query(database);
    query.prepare(QString("SELECT count(*) FROM commodity;"));
    if(!query.exec())
    {
        qDebug()<<"isCommodityTableEmpty Failed "<<query.lastError();
        return  false;
    }

    if(!query.next())
    {
        qDebug()<<"isCommodityTableEmpty Next Failed"<<query.lastError();
        return  false;
    }
    if(query.value(0)!=0)
    {
        return  false;
    }
    return  true;
}

bool CommonityDB::queryCommodityAllData(QMap<int, CommodityProperty> &data)
{
    QSqlQuery query(database);
    query.prepare(QString("SELECT * FROM commodity;"));
    if(!query.exec())
    {
        qDebug()<<"isCommodityAllData Failed "<<query.lastError();
        return  false;
    }
    while(query.next())
    {
        CommodityProperty tmp;
        tmp.id=query.value(0).toInt();
        tmp.name=query.value(1).toString();
        tmp.price=query.value(2).toDouble();
        tmp.type=query.value(3).toInt();
        tmp.count=query.value(4).toInt();
        data.insert(tmp.id,tmp);
    }
    return true;
}

bool CommonityDB::updateCommodityDataByVec(const QVector<CommodityProperty> &data)
{
    QSqlQuery query(database);
    query.prepare(QString("UPDATE commodity SET count = :ccount WHERE id = :cid;"));
    for(auto iter:data)
    {
        query.bindValue(":ccount",iter.count);
        query.bindValue(":cid",iter.id);
        if(!query.exec())
        {
            qDebug()<<"updateCommodityDataByVec Failed "<<query.lastError();
            return  false;
        }
    }
    return true;
}


CommonityDB::AutoRelease::~AutoRelease()
{
    if(ch!=nullptr)
    {
        delete ch;
        ch=nullptr;
    }
}
CommonityDB::CommonityDB(QObject *parent) : QObject(parent)
{
    if(!QSqlDatabase::contains("SmartVenderMachine"))
    {
        database=QSqlDatabase::addDatabase("QSQLITE","SmartVenderMachine");
    }else{
        database=QSqlDatabase::database("SmartVenderMachine");
    }
    database.setDatabaseName("SmartVenderMachine.db");
    database.open();
}
