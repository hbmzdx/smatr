#ifndef COMMODITY_H
#define COMMODITY_H

#include <QWidget>
#include "CommodityHandle.h"

namespace Ui {
class Commodity;
}

class Commodity : public QWidget
{
    Q_OBJECT

public:
    explicit Commodity(int id,QWidget *parent = nullptr);
    ~Commodity();

    CommodityProperty getCurrentData();
    bool getCountChanged();
    void setCountChanged(bool value=false);

private slots:
    void on_spinBox_count_valueChanged(int arg1);

private:
    Ui::Commodity *ui;
    CommodityProperty commProperty;
    bool isCountChanged;
};

#endif // COMMODITY_H
