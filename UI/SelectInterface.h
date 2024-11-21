#ifndef SELECTINTERFACE_H
#define SELECTINTERFACE_H

#include "AbstractInterface.h"

namespace Ui {
class SelectInterface;
}

class SelectInterface : public AbstractInterface
{
    Q_OBJECT

public:
    explicit SelectInterface(QWidget *parent = nullptr);
    ~SelectInterface();
    void initToolButton();
    void resetWidget() override;

private slots:
    void on_toolButton_admin_clicked();

    void on_toolButton_shop_clicked();

private:
    Ui::SelectInterface *ui;
};

#endif // SELECTINTERFACE_H
