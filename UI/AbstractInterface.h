#ifndef ABSTRACTINTERFACE_H
#define ABSTRACTINTERFACE_H
//信号文件继承
#include <QWidget>
#include "Config.hpp"
class AbstractInterface : public QWidget
{
    Q_OBJECT
public:
    explicit AbstractInterface(QWidget *parent = nullptr);
    virtual ~AbstractInterface();
     virtual void resetWidget()=0;

signals:
    void signalJumpWidget(int index);

public slots:
};

#endif // ABSTRACTINTERFACE_H
