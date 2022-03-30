#include "ipconfig.h"

IPconfig::IPconfig(QObject *parent) : QObject(parent)
{
    this->activeIPInterface = NULL ;
    QObject::connect(&timer,&QTimer::timeout,this,&IPconfig::watchActiveIPInterface);
    setIPInterfaceList();
    timer.setInterval(2000);
    timer.start();


//    watchActiveIPInterface();
    QDEBUG;
}
IPconfig::~IPconfig()
{
    QDEBUG;
}
QList<IPInfo*> IPconfig::getIPInterfaceList()
{
    return this->IPInterfaceList;
}
void IPconfig::cleanIPInterfaceList()
{
    //销毁IPInterfaceList中指针所指的对象，清空List
}
void IPconfig::setIPInterfaceList()
{
    QList<QNetworkInterface> interfaceList = QNetworkInterface::allInterfaces();
    QDEBUG << "Has found IPInterfaceList below:";
    for(int i = 0;i < interfaceList.count();i++)
    {
        QNetworkInterface interface = interfaceList.at(i);

        //将QtIP端口信息存至自定义IPInfo对象中
        IPInfo *iPinfo = new IPInfo(this);
        iPinfo->setHumanReadableName(interface.humanReadableName());
        iPinfo->setDeviceName(interface.name());
        iPinfo->setHardwareAddress(interface.hardwareAddress());
        iPinfo->setValid(interface.isValid());
        iPinfo->setActive(interface.flags().operator &(QNetworkInterface::IsUp)==QNetworkInterface::IsUp);
        iPinfo->setLoopBack(interface.flags().operator &(QNetworkInterface::IsLoopBack)==QNetworkInterface::IsLoopBack);
        //将自定义IPInfo对象添加至成员变量IPInterfaceList
        this->IPInterfaceList.append(iPinfo);

        QDEBUG << QString("##### IP Interface-%1 #####").arg(i);
        QDEBUG << "InterfaceName: " << interface.humanReadableName();
        QDEBUG << "DeviceName: " << interface.name();
        QDEBUG << "HardwareAddress: " << interface.hardwareAddress();
        QDEBUG << "isValid: " << interface.isValid();
        QDEBUG << "isActive: " << (interface.flags().operator &(QNetworkInterface::IsUp)==QNetworkInterface::IsUp);
        QDEBUG << "isLoopBack: " << (interface.flags().operator &(QNetworkInterface::IsLoopBack)==QNetworkInterface::IsLoopBack);

    }
    emit IPInterfaceListChanged(IPInterfaceList);//将更新的IPInterfaceList信号发射
}

void IPconfig::watchActiveIPInterface()
{
    QList<QNetworkInterface> interfaceList = QNetworkInterface::allInterfaces();
    int active_flag = 0;
    for(int i = 0;i < interfaceList.count();i++)
    {
        QNetworkInterface interface = interfaceList.at(i);
        if((interface.flags().operator &(QNetworkInterface::IsUp)==QNetworkInterface::IsUp))
        {
            if(!(interface.flags().operator &(QNetworkInterface::IsLoopBack)==QNetworkInterface::IsLoopBack))
            {
                //找到了非回送的活动网络
                active_flag = 1;
                if(this->activeIPInterface != NULL)//如果当前保存的活动端口不为空但已发现新端口则重新设置
                {
                    if(interface.name() != this->activeIPInterface->getDeviceName())
                    {
                        //delete()
                        delete this->activeIPInterface;
                        this->activeIPInterface = NULL;
                        //set()
                        IPInfo *iPinfo = new IPInfo(this);
                        iPinfo->setHumanReadableName(interface.humanReadableName());
                        iPinfo->setDeviceName(interface.name());
                        iPinfo->setHardwareAddress(interface.hardwareAddress());
                        iPinfo->setValid(interface.isValid());
                        iPinfo->setActive(interface.flags().operator &(QNetworkInterface::IsUp)==QNetworkInterface::IsUp);
                        iPinfo->setLoopBack(interface.flags().operator &(QNetworkInterface::IsLoopBack)==QNetworkInterface::IsLoopBack);

                        //重新设置活动IP端口
                        this->setActiveIPInterface(iPinfo);
                        emit sendMassagetoTextBrowser(QString(">>>当前活动端口: %1").arg(this->activeIPInterface->getHumanReadableName()));
                        QDEBUG << QString("Has change the DeviceName: %1 is current activeIPInterface !").arg(this->activeIPInterface->getDeviceName());
                    }
                }else{//如果当前保存的活动端口为空则直接设置
                    //set()
                    IPInfo *iPinfo = new IPInfo(this);
                    iPinfo->setHumanReadableName(interface.humanReadableName());
                    iPinfo->setDeviceName(interface.name());
                    iPinfo->setHardwareAddress(interface.hardwareAddress());
                    iPinfo->setValid(interface.isValid());
                    iPinfo->setActive(interface.flags().operator &(QNetworkInterface::IsUp)==QNetworkInterface::IsUp);
                    iPinfo->setLoopBack(interface.flags().operator &(QNetworkInterface::IsLoopBack)==QNetworkInterface::IsLoopBack);
                    //设置活动IP端口
                    this->setActiveIPInterface(iPinfo);
                    QDEBUG << QString("Has set the DeviceName: %1 is current activeIPInterface !").arg(this->activeIPInterface->getDeviceName());
                    emit sendMassagetoTextBrowser(QString(">>>当前活动端口: %1").arg(this->activeIPInterface->getHumanReadableName()));
                    emit sendMessageToSystemTray("提示","网络端口已连接！");
                }
            }
        }
    }
    if(active_flag == 0 && this->activeIPInterface != NULL)//当前活动网络丢失
    {
        //delete()
        delete this->activeIPInterface;
        this->activeIPInterface = NULL;
        emit sendMassagetoTextBrowser(QString(">>>当前活动端口: NULL"));
        emit sendMessageToSystemTray("提示","网络端口已断开！");
    }
}
IPInfo* IPconfig::getActiveIPInterface()
{
    return this->activeIPInterface;
}
void IPconfig::setActiveIPInterface(IPInfo* IPInterface)
{
    if(this->activeIPInterface == NULL || ( IPInterface->getDeviceName() != this->activeIPInterface->getDeviceName()))
    {
        this->activeIPInterface = IPInterface;
        emit activeIPInterfaceChanged(IPInterface);
    }

}
void IPconfig::setIPbyNetsh(int index)
{
/*
1.修改IP地址addr和子网掩码mask:netsh interface ip set address name="本地连接" source=static addr=192.168.0.106 mask=255.255.255.0
2.修改默认网关gateway:netsh interface ip set address name="本地连接" gateway=192.168.0.1 gwmetric=0
3.修改首选(PRIMARY)的DNS:netsh interface ip set dns name="本地连接" source=static addr=202.96.128.66 register=PRIMARY
4.禁用无线网卡:netsh interface set interface wlan0 disabled
5.启用无线网卡:netsh interface set interface wlan0 enabled
*/
    QString ip = IP1;
    QString netmask = MASK1;
    QString gateway = GATEWAY1;
    switch (index) {
    case 1:
        ip = IP1;
        netmask = MASK1;
        gateway = GATEWAY1;
        break;
    case 2:
        ip = IP2;
        netmask = MASK2;
        gateway = GATEWAY2;
        break;
    case 3:
        ip = IP3;
        netmask = MASK3;
        gateway = GATEWAY3;
        break;
    case 4:
        ip = IP4;
        netmask = MASK4;
        gateway = GATEWAY4;
        break;
    default:
        break;
    }

    QProcess cmd(this);
    QString name = this->activeIPInterface->getHumanReadableName();//.humanReadableName();

    // netsh 命令如下，需要获取管理员权限才能执行
    // netsh interface ipv4 set address name = "以太网" source = static
    // address = 192.168.0.106 mask = 255.255.255.0 gateway = 192.168.0.1 gwmetric = 0
    QString command = "netsh interface ipv4 set address name = \""
            + name + "\" source = static address = " + ip
            + " mask = " + netmask
            +" gateway = " + gateway + " gwmetric = 0 " ;
    QDEBUG << command;
    cmd.start(command);
    cmd.waitForFinished();
}
