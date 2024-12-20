#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QTimer>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void slotJumpWidget(int index);
    void slotTimeOut();
private slots:

    void on_pushButton_reset_clicked();

private:
    Ui::MainWindow *ui;
    QTimer *timer;
    int countDown;
};
#endif // MAINWINDOW_H
