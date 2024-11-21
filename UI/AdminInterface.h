  #ifndef ADMININTERFACE_H
#define ADMININTERFACE_H

#include "AbstractInterface.h"
#include "Commodity.h"
#include "CommodityHandle.h"
#include"Config.hpp"
namespace Ui {
class AdminInterface;
}

class AdminInterface : public AbstractInterface
{
    Q_OBJECT

public:
    explicit AdminInterface(QWidget *parent = nullptr);
    virtual ~AdminInterface() override;
    void resetWidget() override;

    void initTableWidget();

    void initCommodity();

signals:
    void signalJumpWidget(int index);

private slots:


    void on_pushButton_save_clicked();

private:
    Ui::AdminInterface *ui;
    QVector<Commodity *> commAllAddress;
};

#endif // ADMININTERFACE_H
