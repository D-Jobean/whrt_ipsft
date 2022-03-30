#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <ipconfig.h>
#include <serialport.h>
#include <QSystemTrayIcon>
#include <QCloseEvent>
#include <QMenu>
#include <QAction>
#define QDEBUG qDebug()<<"["<<__FILE__<<":"<<__func__<<":"<<__LINE__<<"]"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void initDisplay();
    void handleSerialMassage(QByteArray byteArray);
private slots:
    void on_pushButton_1_clicked();

    void on_pushButton_2_clicked();

    void textBrowserDisplay(QString msg);

public slots:
    void quit(); //托盘程序退出程序的槽
    void showAgain(); //重新显示app
    void showTrayMassage(QString title,QString msg);

private:
    Ui::MainWindow *ui;
    IPconfig *ipConfig;
    SerialPort *serialPort;
    QSystemTrayIcon *systemTray;//托盘程序
    QMenu *pContextMenu;//右键托盘菜单
    QAction *quitAct;//退出action
    QAction *showAct;//重新显示action
    bool quitflag;
protected:
    void closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOW_H
