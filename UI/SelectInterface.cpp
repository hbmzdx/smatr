#include "SelectInterface.h"
#include "ui_SelectInterface.h"

SelectInterface::SelectInterface(QWidget *parent) :
   AbstractInterface(parent),
    ui(new Ui::SelectInterface)
{
    ui->setupUi(this);
    initToolButton();
}

SelectInterface::~SelectInterface()
{
    delete ui;
}

void SelectInterface::initToolButton()
{
    ui->toolButton_shop->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);//文本在图标下方显示
    ui->toolButton_shop->setIcon(QIcon(QString(":/Image/shopping.png")));
    ui->toolButton_shop->setIconSize(QSize(100,100));
    ui->toolButton_shop->setText("购物");

    ui->toolButton_admin->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->toolButton_admin->setIcon(QIcon(QString(":/Image/tallying.png")));
    ui->toolButton_admin->setIconSize(QSize(100,100));
    ui->toolButton_admin->setText("进货");


}

void SelectInterface::resetWidget()
{

}

void SelectInterface::on_toolButton_admin_clicked()
{
    emit signalJumpWidget(JumpType::LOGINWIDGET);
}

void SelectInterface::on_toolButton_shop_clicked()
{
    emit signalJumpWidget(JumpType::SHOPWIDGET);
}
