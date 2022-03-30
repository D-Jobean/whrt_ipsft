#ifndef IPCONFIG_H
#define IPCONFIG_H

#include <QObject>
#include <QDebug>
#include <QNetworkInterface>
#include <ipinfo.h>
#include <QProcess>
#include <QTimer>
#include <QIcon>
#define QDEBUG qDebug()<<"["<<__FILE__<<":"<<__func__<<":"<<__LINE__<<"]"
//NET IP1 FOR
#define IP1 "192.168.1.16";
#define MASK1 "255.255.255.0";
#define GATEWAY1 "192.168.1.1";
//NET IP2 FOR
#define IP2 "192.168.1.17";
#define MASK2 "255.255.255.0";
#define GATEWAY2 "192.168.1.1";
//NET IP3 FOR
#define IP3 "192.168.1.18";
#define MASK3 "255.255.255.0";
#define GATEWAY3 "192.168.1.1";
//NET IP4 FOR
#define IP4 "192.168.1.19";
#define MASK4 "255.255.255.0";
#define GATEWAY4 "192.168.1.1";

class IPconfig : public QObject
{
    Q_OBJECT
public:
    explicit IPconfig(QObject *parent = nullptr);
    ~IPconfig();

    //setter&getter for IPInterfaceList
    QList<IPInfo*> getIPInterfaceList();
    void setIPInterfaceList();
    void cleanIPInterfaceList();

    //setter&getter for activeIP
    void setActiveIPInterface(IPInfo*);
    void watchActiveIPInterface();
    IPInfo* getActiveIPInterface();

signals:
    //about IPInterfaceList
    void IPInterfaceListChanged(QList<IPInfo*>);
    //about IPInterfaceList
    void activeIPInterfaceChanged(IPInfo*);

    void sendMassagetoTextBrowser(QString massage);
//    void sendMessageToSystemTray("温馨提示", "驱动进入后台运行!",QIcon(":/USER/plug.ico"),2000);
    void sendMessageToSystemTray(QString,QString);
public slots:
    //set ip
    void setIPbyNetsh(int index);
private:
    QList<IPInfo*> IPInterfaceList;
    IPInfo* activeIPInterface;
    QTimer timer;

};

#endif // IPCONFIG_H
