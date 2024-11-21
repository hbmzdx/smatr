#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <QDebug>
enum JumpType
{
    ADVERTISEMENTWIDGET =0,
    COMMODITYSHOP=0,
    LOGINWIDGET=1,
    COMMODITYQRCODE=1,
    SELECTWIDGET=2,
    COMMODITYANIMATION=2,
    ADMINWIDGET,
    SHOPWIDGET
};


class CommodityProperty
{
public:
    int id;
    QString name;
    double price;
    int type;
    int count;
};

#endif // CONFIG_HPP
