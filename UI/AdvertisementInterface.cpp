#include "AdvertisementInterface.h"
#include "ui_AdvertisementInterface.h"

#include <QDir>

AdvertisementInterface::AdvertisementInterface(QWidget *parent) :
    AbstractInterface(parent),
    ui(new Ui::AdvertisementInterface),
    scene(new QGraphicsScene(this)),
    pixmapItem(new QGraphicsPixmapItem),
    videoItem(new QGraphicsVideoItem),
    timer(new QTimer(this)),
    pixmapIndex(0),
    videoIndex(0),
    player(new QMediaPlayer(this))
{
    ui->setupUi(this);
    connect(timer,&QTimer::timeout,this,&AdvertisementInterface::slotTimeOut);
    connect(player,&QMediaPlayer::mediaStatusChanged,
            this,&AdvertisementInterface::slotMediaStatusChanged);

    ui->graphicsView->setScene(scene);
    pixmapItem->setPos(0,0);
    videoItem->setPos(0,(ui->graphicsView->height()-5)/2);
    scene->setSceneRect(0,0,ui->graphicsView->width()-5,
                        ui->graphicsView->height()-5);

    scene->addItem(pixmapItem);
    scene->addItem(videoItem);
    player->setVideoOutput(videoItem);
    videoItem->setSize(QSizeF(ui->graphicsView->width()-5,
                       (ui->graphicsView->height()-5)/2));
    videoItem->setAspectRatioMode(Qt::IgnoreAspectRatio);

    timer->start(2000);
    initAdvertisementAllItem();
    slotTimeOut();
    slotMediaStatusChanged(QMediaPlayer::MediaStatus::EndOfMedia);
}

AdvertisementInterface::~AdvertisementInterface()
{
    delete pixmapItem;
    delete videoItem;
    delete ui;
}

void AdvertisementInterface::resetWidget()
{
    timer->start(2000);
    slotMediaStatusChanged(QMediaPlayer::MediaStatus::EndOfMedia);

}

void AdvertisementInterface::initAdvertisementAllItem()
{
    QString pixmapPath=QCoreApplication::applicationDirPath()
            +QString("/AD/pixmap/");
    QDir pathDir(pixmapPath);
    QFileInfoList pixmapList =pathDir.entryInfoList(QDir::Files);
    for(auto iter:pixmapList)
    {
        this->pixmapList.push_back(iter.filePath());
    }


    QString videoPath=QCoreApplication::applicationDirPath()
            +QString("/AD/video/");
    QDir videoDir(videoPath);
    QFileInfoList videoList =videoDir.entryInfoList(QDir::Files);
    for(auto iter:videoList)
    {
        this->videoList.push_back(iter.filePath());
    }

}

void AdvertisementInterface::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    timer->stop();
    player->stop();
    emit signalJumpWidget(JumpType::SELECTWIDGET);
}

void AdvertisementInterface::slotTimeOut()
{

    QPixmap pixmap(pixmapList[++pixmapIndex %pixmapList.size()]);
    pixmap =pixmap.scaled(ui->graphicsView->width()-5,(ui->graphicsView->height()-5)/2);
    pixmapItem->setPixmap(pixmap);
}

void AdvertisementInterface::slotMediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    if(status == QMediaPlayer::MediaStatus::EndOfMedia)
    {
        player->setMedia(QUrl(videoList[++videoIndex %videoList.size()]));
        player->play();
    }else if(status == QMediaPlayer::MediaStatus::InvalidMedia)
    {
        player->stop();
    }
}



