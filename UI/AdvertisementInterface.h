#ifndef ADVERTISEMENTINTERFACE_H
#define ADVERTISEMENTINTERFACE_H
//广告界面
#include "AbstractInterface.h"
#include"Config.hpp"

#include<QGraphicsScene>
#include<QGraphicsPixmapItem>
#include<QGraphicsVideoItem>
#include <QTimer>
#include <QMediaPlayer>
namespace Ui {
class AdvertisementInterface;
}

class AdvertisementInterface : public AbstractInterface
{
    Q_OBJECT

public:
    explicit AdvertisementInterface(QWidget *parent = nullptr);
    virtual ~AdvertisementInterface() override;

    virtual void  resetWidget() override;

    void initAdvertisementAllItem();

protected:
    void mousePressEvent(QMouseEvent *event) override;

public slots:
    void slotTimeOut();
    void slotMediaStatusChanged(QMediaPlayer::MediaStatus status);


private:
    Ui::AdvertisementInterface *ui;
    QGraphicsScene *scene;
    QGraphicsPixmapItem *pixmapItem;
    QGraphicsVideoItem *videoItem;

    QStringList pixmapList;
    QStringList videoList;
    QTimer *timer;
    int pixmapIndex;
    int videoIndex;
    QMediaPlayer *player;
};

#endif // ADVERTISEMENTINTERFACE_H
