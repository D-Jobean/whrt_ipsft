#include "serialport.h"

SerialPort::SerialPort(QObject *parent) : QObject(parent),serialPort(0),timer(0)
{
    QDEBUG;
    QObject::connect(&serialPort,&QSerialPort::readyRead,this,&SerialPort::handleReadyRead);
    QObject::connect(&timer,&QTimer::timeout,this,&SerialPort::handleWhenTimeout);

//    this->initSerialPort();
    this->timer.setInterval(1000);
    this->timer.start();
    COMisConnected = false;
}
SerialPort::~SerialPort()
{
    serialPort.close();//关闭串口连接
    QDEBUG;
}
void SerialPort::initSerialPort()
{
    if(this->openSerialPort(this->getSerialPortName()))
    {
        emit sendMassagetoBar(QString("%1 Has Connected!").arg(serialPort.portName()));
        emit sendMessageToSystemTray("提示","COM端口已连接！");
        COMisConnected = true;
        QDEBUG << QString("%1 Has Connected!").arg(serialPort.portName());
    }
    else
        QDEBUG << "No Serial Prot is Found!";
}
//找到第一个可用的串口端口，返回端口名
QString SerialPort::getSerialPortName()
{
    if(!serialPort.portName().isEmpty())
    {
        return serialPort.portName();
    }
    QList<QSerialPortInfo> serialPortInfoList = QSerialPortInfo::availablePorts();
    QString serialPortName; //create a empty string name
    for(int i = 0;i<serialPortInfoList.count();i++)
    {
        if(serialPortInfoList.at(i).isNull()||serialPortInfoList.at(i).isBusy()){}
        else{
            serialPortName = serialPortInfoList.at(i).portName();
            QDEBUG << "Available Serial Port has been found：" << serialPortName;
            return serialPortName;//找到第一个可用的串口端口就返回了
        }
    }
    return serialPortName;
}
//根据端口名称以N81-9600模式打开端口
bool SerialPort::openSerialPort(QString spName)
{
    QDEBUG;
    if(!spName.isNull())
    {
        serialPort.setPortName(spName);
        serialPort.setBaudRate(QSerialPort::Baud9600);//default: no-parity 8-dataBits 1-stopBit
        if(serialPort.open(QIODevice::ReadOnly))
            return true;
    }
    return false;
}
//当串口收到消息时候，更新readByteData的值
void SerialPort::handleReadyRead()
{
    readByteData.clear();
    readByteData.append(serialPort.readAll());
    QDEBUG << QString("Data successfully received from port %1").arg(serialPort.portName());
    QDEBUG << readByteData ;
    emit readByteDataChanged(readByteData);
}

void SerialPort::handleWhenTimeout()
 {
    //CHECK THE PORT
    QDEBUG << "Current Port:" << serialPort.portName() ;
    //monitor serial port and update the port
    QList<QSerialPortInfo> serialPortInfoList = QSerialPortInfo::availablePorts();
    QString serialPortName; //create a empty string name
    for(int i = 0;i<serialPortInfoList.count();i++)
    {
        if(serialPortInfoList.at(i).isNull()){}
        else{
            if(serialPortInfoList.at(i).portName() == this->serialPort.portName())
            {
                serialPortName = serialPortInfoList.at(i).portName();
                QDEBUG << "Monitor Serial Port：" << serialPortName;
                break;//搜索到正在占用的端口则跳出循环
            }
        }
    }
    if(serialPortName.isEmpty())//未找到占用的端口，解除绑定重新开辟端口
    {
        QDEBUG << "Monitor Serial Port：No Port";
            if(!this->serialPort.portName().isEmpty())
            {
                this->serialPort.close();
                this->serialPort.setPortName("");
                QDEBUG << "Old Serial Port Has Closed" << serialPortName;
            }

        //通知Mainwidget->statusbar
            if(COMisConnected == true)
            {
                emit sendMassagetoBar(QString("Serial Port Has Disconnected!"));
                emit sendMessageToSystemTray("提示","COM端口已断开！");
                COMisConnected = false;
            }

        //重新打开串口端口
        this->initSerialPort();
    }
 }
