#include "windows/statusbar.h"

#include "com.h"
#include "posix_qextserialport.h"

#include <QTimer>
#include <QMap>
#include <QDebug>

Com::Com(QObject *parent) : QObject(parent)
{
    m_com = new Posix_QextSerialPort("/dev/ttyUSB0",QextSerialBase::Polling);

    m_com ->open(QIODevice::ReadWrite | QIODevice::Unbuffered);
    Q_ASSERT(m_com->isOpen());

    //以读写方式打开串口
    m_com->setBaudRate(BAUD9600);//波特率设置，我们设置为9600
    m_com->setDataBits(DATA_8);//数据位设置，我们设置为8位数据位
    m_com->setParity(PAR_NONE);//奇偶校验设置，我们设置为无校验
    m_com->setStopBits(STOP_1);//停止位设置，我们设置为1位停止位
    m_com->setFlowControl(FLOW_OFF);
    m_com->setTimeout(1000);

    m_readTimer = new QTimer(this);
    //readTimer->start(3000);
    connect(m_readTimer,SIGNAL(timeout()),this,SLOT(slotReadMyCom()));
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

qint64 Com::sendOrder(Com::Order order)
{
    QMap<Com::Order, char*> orderMap;
    //char arraySpectrueMeasure[3] = {(char)0xfe, (char)0x01, (char)0xff};
    char arraySpectrueMeasure[3] = {(char)0xff, (char)0x01, (char)0xfe};
    orderMap[SpectrueMeasure] = (char *)arraySpectrueMeasure;

    char arrayCountMeasure[3] = {(char)0xfe, (char)0x02, (char)0xff};
    orderMap[CountMeasure] = (char *)arrayCountMeasure;

    char arrayCalibrateMeasure[3] = {(char)0xfe, (char)0x04, (char)0xff};
    orderMap[CalibrateMeasure] = (char *)arrayCalibrateMeasure;

    char arrayStop[3] = {(char)0xfe, (char)0x98, (char)0xff};
    orderMap[StopMeasure] = (char *)arrayStop;

    //char arrayInPlate[4] = {(char)0xfe, (char)0x06, (char)0x31, (char)0xff};
    char arrayInPlate[4] = {(char)0xfe, (char)0x06, (char)0x31, (char)0xff};
    orderMap[InPlate] = (char *)arrayInPlate;

    char arrayOutPlate[4] = {(char)0xfe, (char)0x06, (char)0x32, (char)0xff};
    orderMap[OutPlate] = (char *)arrayOutPlate;

    Q_ASSERT(orderMap.contains(order));

    m_com->flush();
    return m_com->write(orderMap[order], strlen(orderMap[order]));
}

void Com::sendSampleMeasure(uint measurementTime, uint repeatTimes)
{
    char arraySampleMeasure[5] = {(char)0xfe,
                                  (char)0x03,
                                  (char)0x00, (char)0x00,
                                  (char)0xff
                                 };
    arraySampleMeasure[3] = (char)0x30 + (char)measurementTime;
    arraySampleMeasure[4] = (char)0x30 + (char)repeatTimes;

    qDebug() << m_com->write(
                    arraySampleMeasure,
                    strlen(arraySampleMeasure)
                    );
}

QByteArray Com::slotReadMyCom()
{
    static int readCount =0;

    QByteArray readData = "";
    for(int i = 0; i < 1; i++)
    {
        m_com->setTimeout(1000);//10 seconds
        readData = m_com->readAll();    //读取串口缓冲区的所有数据给临时变量

        if(readData.isEmpty())
        {
            qDebug()<< "read count:" << readCount++;
            continue;
        }
        break;
    }
    readCount = 0;

//    QString tmp = readData.left(readData.size() - 1);
//    tmp .chop(1);
//    qint64 recvDataQint64 = tmp.toInt();
//    qDebug() << QString::number(recvDataQint64, 16);
#ifdef TEST_COM
    readData = getRecvData();
    qDebug() << QString::number(readData.toLongLong(), 16);
#endif

    return readData;
}

void Com::inPlateButtonClicked()
{
#ifdef TEST_COM
    QByteArray recvData;
    recvData[0] = (char)0xfe;
    recvData[1] = (char)0x98;
    recvData[2] = (char)0x32;
    recvData[3] = (char)0xff;
    Com::instance()->setRecvData(recvData);
#endif

    Com::instance()->sendOrder(Com::InPlate);
    StatusBar::instance()->setPlatePositionByRecvData(
                Com::instance()->slotReadMyCom()
                );
}

void Com::outPlateButtonClicked()
{
#ifdef TEST_COM
    QByteArray recvData;
    recvData[0] = (char)0xfe;
    recvData[1] = (char)0x98;
    recvData[2] = (char)0x31;
    recvData[3] = (char)0xff;
    Com::instance()->setRecvData(recvData);
#endif

    Com::instance()->sendOrder(Com::OutPlate);
    StatusBar::instance()->setPlatePositionByRecvData(
                Com::instance()->slotReadMyCom());
}

#ifdef TEST_COM
void Com::setRecvData(QByteArray data)
{
    testData = data;
}

QByteArray Com::getRecvData()
{
    return testData;
}
#endif
