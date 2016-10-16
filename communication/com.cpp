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

//本pc机采用的是小端架构的，而arm的机械一般是大端的，所以所有的命令都是倒序的
#define STOP_ORDER              0xff98feUL
#define CURRENT_POSITION        0XFF98FEUL
#define ACTIVATING_SPECTRUM     0XFF01FEUL
#define ACTIVATINE_COUNT        0XFF02FEUL
//#define ACTIVATING_CONTENT      0XFF03FEUL
#define ACTIVATING_CALIBRATE    0XFF04FEUL
#define IN_SLIDING_PLATE        0XFF3106FEUL
#define OUT_SLIDING_PLATE       0XFF3206FEUL

void Com::sendOrder(Com::Order order)
{
    QMap<Com::Order, char*> orderMap;
    char arraySpectrueMeasure[3] = {0xfe, 0x01, 0xff};
    orderMap[SpectrueMeasure] = (char *)arraySpectrueMeasure;

    char arrayCountMeasure[3] = {0xfe, 0x02, 0xff};
    orderMap[CountMeasure] = (char *)arrayCountMeasure;


    qDebug() << strlen(arraySpectrueMeasure[order]) << arraySpectrueMeasure[order];
    myCom->write(arraySpectrueMeasure[order], strlen(arraySpectrueMeasure[order]));
}

QByteArray Com::slotReadMyCom()
{
    static int readCount =0;

    QByteArray readData;
    for(int i = 0; i < 3; i++)
    {
        myCom->setTimeout(1000);
        readData = myCom->readAll();    //读取串口缓冲区的所有数据给临时变量

        if(readData.isEmpty())
        {
            qDebug()<<readCount++;
            continue;
        }
        break;
    }


    qDebug() << readData;

    return readData;
}

