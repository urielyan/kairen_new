#include <QTimer>
#include <QMap>
#include <QDebug>

#include "com.h"
#include "posix_qextserialport.h"

Com::Com(QObject *parent) : QObject(parent)
{
    myCom = new Posix_QextSerialPort("/dev/ttySAC3",QextSerialBase::Polling);

    myCom ->open(QIODevice::ReadWrite);
    //以读写方式打开串口
    myCom->setBaudRate(BAUD115200);
    //波特率设置，我们设置为9600
    myCom->setDataBits(DATA_8);
    //数据位设置，我们设置为8位数据位
    myCom->setParity(PAR_NONE);
    //奇偶校验设置，我们设置为无校验
    myCom->setStopBits(STOP_1);
    //停止位设置，我们设置为1位停止位
    myCom->setFlowControl(FLOW_OFF);
    myCom->setTimeout(1000);
    readTimer = new QTimer(this);
    readTimer->start(3000);
    connect(readTimer,SIGNAL(timeout()),this,SLOT(slotReadMyCom()));

    sendOrder(SpectrueMeasure);
}

Com *Com::instance()
{
    static Com _instance;
    return &_instance;
}

void Com::sendOrder(Com::Order order)
{
    QMap<Com::Order, char*> orderMap;
    char array[3] = {0x32, 0x31, 0xdd};
    orderMap[SpectrueMeasure] = (char *)array;

    qDebug() << array;
}

void Com::slotReadMyCom()
{
    static int i =0;
    qDebug()<<i++;
    QByteArray temp = myCom->readAll();
    //读取串口缓冲区的所有数据给临时变量temp

    qDebug() << temp;
}

