#ifndef COMMONITYDB_H
#define COMMONITYDB_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include"Config.hpp"

class CommonityDB : public QObject
{
    Q_OBJECT
public:
    static CommonityDB *getInstance();
    static void realseInstance();
    virtual ~CommonityDB();

    class AutoRelease
    {
    public:
        AutoRelease()=default;
        ~AutoRelease();
    };

    void createCommodityTable();
    bool insertCommodityAllData(const QMap<int,CommodityProperty> &data);
    bool isCommodityTableEmpty();
    bool queryCommodityAllData(QMap<int,CommodityProperty> &data);

    bool updateCommodityDataByVec(const QVector<CommodityProperty>&data);

signals:

public slots:
private:
     explicit CommonityDB(QObject *parent = nullptr);

    static CommonityDB *ch;
    static pthread_mutex_t mutex;
    static AutoRelease ar;

    QSqlDatabase database;
};

#endif // COMMONITYDB_H
