import QtQuick 2.0

Item {
    id:root
    height: 476
    width: 476
    Image {
        id: image1
        width:100
        height: 100
        anchors.horizontalCenter: parent.horizontalCenter
        visible: true
    }
    signal signalRecvImagePath(string path)
    signal signalSendAnimationFinish()
    function slotRecvImagePath(path)
    {
        image1.source=path
        para1.running=true
        timer1.start()
    }

    Component.onCompleted: {
    root.signalRecvImagePath.connect(slotRecvImagePath)
    }
    ParallelAnimation{

        id:para1
        RotationAnimation
        {
            id:rotaAni1
            target: image1
            from:0
            to:360
            duration: 2000
        }
        NumberAnimation
        {
            id:numAnil
            target: image1
            from:0
            to:409
            duration: 2000
        }
        running: false
    }
    Timer
    {
        id:timer1
        interval: 2000
        onTriggered:
        {
            root.signalSendAnimationFinish()
            timer1.stop()
        }
    }

}
