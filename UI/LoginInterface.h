#ifndef LOGININTERFACE_H
#define LOGININTERFACE_H
//登录界面
#include "AbstractInterface.h"
#include"Config.hpp"

namespace Ui {
class LoginInterface;
}

class LoginInterface : public AbstractInterface
{
    Q_OBJECT

public:
    explicit LoginInterface(QWidget *parent = nullptr);
    virtual ~LoginInterface() override;
   void resetWidget() override;

private slots:
    void on_pushButton_login_clicked();

private:
    Ui::LoginInterface *ui;
};

#endif // LOGININTERFACE_H
