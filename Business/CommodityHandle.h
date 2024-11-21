#ifndef COMMODITYHANDLE_H
#define COMMODITYHANDLE_H

#include <QObject>
#include <QDebug>
#include <pthread.h>
#include <QMap>
#include"Config.hpp"

#include<QJsonDocument>
#include<QJsonParseError>
#include<QJsonObject>
#include<QJsonArray>

#include "CommonityDB.h"
class CommodityHandle : public QObject
{
    Q_OBJECT
public:
    static CommodityHandle *getInstance();
    static void realseInstance();
    virtual ~CommodityHandle();
    class AutoRelease
    {
    public:
        AutoRelease()=default;
        ~AutoRelease();
    };

    void initCommMap();

    CommodityProperty commMapGetValueByID(int id);
    QList<CommodityProperty> commMapGetAllvalues();
    QList<int> commMapGetAllKeys();

    bool saveCommodityCountChange(const QVector<CommodityProperty>&data);

signals:

public slots:
private:
     explicit CommodityHandle(QObject *parent = nullptr);

    static CommodityHandle *ch;
    static pthread_mutex_t mutex;
    static AutoRelease ar;

    QMap<int,CommodityProperty> CommMap;
};

#endif // COMMODITYHANDLE_H
