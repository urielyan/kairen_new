#ifndef COM_H
#define COM_H

#include <QObject>

class QTimer;
class Posix_QextSerialPort;
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

private slots:
    void slotReadMyCom(); //读取串口

private:
    explicit Com(QObject *parent = 0);

    Posix_QextSerialPort *myCom;
    QTimer *readTimer;

};

#endif // COM_H
