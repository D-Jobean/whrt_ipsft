#ifndef IPINFO_H
#define IPINFO_H

#include <QObject>
#include <QDebug>
#define QDEBUG qDebug()<<"["<<__FILE__<<":"<<__func__<<":"<<__LINE__<<"]"
class IPInfo : public QObject
{
    Q_OBJECT
    QString HumanReadableName;
    QString DeviceName;
    QString HardwareAddress;
    bool Valid;
    bool Active;  //对于有线网络，连接线缆即为true，对于无线网络要连接成功才true
    bool LoopBack;
public:
    explicit IPInfo(QObject *parent = nullptr);
    ~IPInfo();
//setter
    void setHumanReadableName(QString h)
    {
        this->HumanReadableName = h;
    }
    void setDeviceName(QString d)
    {
        this->DeviceName = d;
    }
    void setHardwareAddress(QString h)
    {
        this->HardwareAddress = h;
    }
    void setValid(bool v)
    {
        this->Valid = v;
    }
    void setActive(bool a)
    {
        this->Active = a;
    }
    void setLoopBack(bool l)
    {
        this->LoopBack = l;
    }
//getter
    QString getHumanReadableName()
    {
        return this->HumanReadableName;
    }
    QString getDeviceName()
    {
        return this->DeviceName;
    }
    QString getHardwareAddress()
    {
        return this->HardwareAddress;
    }
    bool isValid()
    {
        return this->Valid;
    }
    bool isActive()
    {
        return this->Active;
    }
    bool isLoopBack()
    {
        return this->LoopBack;
    }
signals:

public slots:
};

#endif // IPINFO_H
