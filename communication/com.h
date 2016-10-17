#ifndef COM_H
#define COM_H

#include <QObject>

class RecvData
{
public:
    explicit RecvData(QObject *parent = 0);

};

class QTimer;
class QextSerialBase;
class Com : public QObject
{
    Q_OBJECT
public:
    enum Order
    {
        CountMeasure = 0
        ,SpectrueMeasure
        ,CalibrateMeasure
        ,SampleMeasure
        ,StopMeasure
        ,InPlate
        ,OutPlate
    };
    static Com *instance();

    void sendOrder(Com::Order order);
signals:

public slots:
    QByteArray slotReadMyCom(); //读取串口

private:
    explicit Com(QObject *parent = 0);

    QextSerialBase *myCom;
    QTimer *readTimer;

public:
    //test
#define TEST_COM

#ifdef TEST_COM
    QByteArray testData;
    void setRecvData(QByteArray data);
    QByteArray getRecvData();
#endif
};

#endif // COM_H
