#ifndef SERIALPORT_H
#define SERIALPORT_H
#include <QObject>
#include <QtSerialPort/QtSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include <QIcon>
#define QDEBUG qDebug()<<"["<<__FILE__<<":"<<__func__<<":"<<__LINE__<<"]"
class SerialPort : public QObject
{
    Q_OBJECT
    QSerialPort serialPort;
    QByteArray readByteData;
    QTimer      timer;

    bool COMisConnected;

public:
    explicit SerialPort(QObject *parent = nullptr);
    ~SerialPort();
    QString getSerialPortName();
    bool openSerialPort(QString spName);
    void initSerialPort();
    void handleTimeout();

signals:
    void readByteDataChanged(QByteArray byteArray);
    void sendMassagetoBar(QString massage,int to=0);
    //    void sendMessageToSystemTray("温馨提示", "驱动进入后台运行!",QIcon(":/USER/plug.ico"),2000);
    void sendMessageToSystemTray(QString,QString);
public slots:
    void handleReadyRead();
    void handleWhenTimeout();

};

#endif // SERIALPORT_H
