#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) ://初始化变量
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QDEBUG;
//    ui->textBrowser;
//    ui->spinBox;
//    ui->pushButton_1;  //test
//    ui->pushButton_2;  //set
//    ui->statusBar;
    ui->setupUi(this);
    this->setWindowTitle("IPsft1.0 byJobean");
    //托盘程序
    this->quitflag = false;
    this->systemTray = new QSystemTrayIcon(this);
    this->systemTray->setIcon(QIcon(":/USER/plugs.ico"));
    this->systemTray->setToolTip("whrt-ipsft driver v1.0");
    this->showAct = new QAction("WHRT-ipsft.by Jobean", this);
    this->quitAct = new QAction("退出", this);
    QObject::connect(showAct, SIGNAL(triggered()), this, SLOT(showAgain()));
    QObject::connect(quitAct, SIGNAL(triggered()), this, SLOT(quit()));
    this->pContextMenu = new QMenu(this);
    this->pContextMenu->addAction(showAct);
    this->pContextMenu->addSeparator();
    this->pContextMenu->addAction(quitAct);
    this->systemTray->setContextMenu(pContextMenu);
    this->systemTray->show();

    this->ipConfig = new IPconfig(this);
    this->serialPort = new SerialPort(this);
    this->initDisplay();

    QObject::connect(this->serialPort,&SerialPort::readByteDataChanged,this,&MainWindow::handleSerialMassage);
    QObject::connect(this->serialPort,&SerialPort::sendMassagetoBar,this->ui->statusBar,&QStatusBar::showMessage);
    QObject::connect(this->serialPort,&SerialPort::sendMessageToSystemTray,this,&MainWindow::showTrayMassage);
    QObject::connect(this->ipConfig,&IPconfig::sendMassagetoTextBrowser,this,&MainWindow::textBrowserDisplay);
    QObject::connect(this->ipConfig,&IPconfig::sendMessageToSystemTray,this,&MainWindow::showTrayMassage);
}

MainWindow::~MainWindow()//释放指针空间
{
    QDEBUG;
    delete ui;
}
void MainWindow::initDisplay()
{
    QList<IPInfo*> ipPortList = this->ipConfig->getIPInterfaceList();
    for(int i = 0;i < ipPortList.count();i++)
    {
        IPInfo* ipPort = ipPortList.at(i);
        ui->textBrowser->append(QString(">>>%1: %2").arg(ipPort->getHumanReadableName()).arg(ipPort->getDeviceName()));
    }
    ui->statusBar->showMessage("READY: serial port disconnect!");
}
void MainWindow::textBrowserDisplay(QString msg)
{
    ui->textBrowser->append(msg);
}
void MainWindow::on_pushButton_1_clicked()//test active ip port
{
    QDEBUG;
//    ui->textBrowser->append("btn_1 clicked!");
    this->ipConfig->watchActiveIPInterface();
    ui->textBrowser->append(QString(">>>当前活动端口: %1").arg(this->ipConfig->getActiveIPInterface()->getHumanReadableName()));
}

void MainWindow::on_pushButton_2_clicked()//set ip to active ip port
{
    QDEBUG;
//    ui->textBrowser->append("btn_2 clicked!");
    this->ipConfig->setIPbyNetsh(ui->spinBox->value());
    ui->textBrowser->append(QString(">>>%1端口IP成功修改为IP-%2").arg(this->ipConfig->getActiveIPInterface()->getHumanReadableName()).arg(ui->spinBox->value()));
    this->systemTray->showMessage("提示", "当前网络端口IP地址已被修改！",QIcon(":/USER/plug.ico"),2000);//
}
void MainWindow::handleSerialMassage(QByteArray byteArray)
{
    //statusBar show massage
    int massageHEXtoInt = byteArray.toHex().toInt(0,16);
    ui->statusBar->showMessage(QString("RUN: %1 received %2").arg(this->serialPort->getSerialPortName()).arg(massageHEXtoInt));
    if(this->ipConfig->getActiveIPInterface() == NULL)//网络掉线
    {QDEBUG;
        ui->textBrowser->append(QString(">>>网络端口不可用，修改IP失败！"));
        this->systemTray->showMessage("提示", "当前网络端口不可用，修改IP失败！",QIcon(":/USER/plug.ico"),2000);//
    }
    else
    {//网络连接
        QDEBUG;
        //change IP depends on massage
        this->ipConfig->setIPbyNetsh(massageHEXtoInt);
        ui->textBrowser->append(QString(">>>%1端口IP成功修改为IP-%2").arg(this->ipConfig->getActiveIPInterface()->getHumanReadableName()).arg(massageHEXtoInt));
        this->systemTray->showMessage("提示", "当前网络端口IP地址已被修改！",QIcon(":/USER/plug.ico"),2000);//
    }
}
void MainWindow::quit()
{
    QDEBUG;
    this->quitflag = true;
    this->close();
}
void MainWindow::showAgain()
{
    QDEBUG;
    this->show();
}
void MainWindow::closeEvent(QCloseEvent *event)
{
    if(quitflag)
        event->accept();
    else
    {
        this->hide();
        this->systemTray->showMessage("温馨提示", "驱动进入后台运行!",QIcon(":/USER/plug.ico"),2000);//
        event->ignore();
    }
}
void MainWindow::showTrayMassage(QString title,QString msg)
{
    this->systemTray->showMessage(title, msg,QIcon(":/USER/plug.ico"),1000);
}
