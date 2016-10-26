/**
 *1位起始位，8位数据位，1位停止位，无奇偶校验位；
仪器开机后波特率保持设定值（默认为9600b/s）；
通讯命令以FE开始，FF结束；例如FE    01    FF
命令字
详细
意图
控制板执行情况
控制板通讯回复
举例
01h
Fe  01  ff
启动能谱测量
对当前在测量位置的样品进行能谱测量
Fe  01  30  34  31  32  33  34  35  ff
  0.4v      12345
测量从0.4v到2.9v结束，共25
02h
Fe  02  ff
启动计数测量
对当前在测量位置的样品进行计数测量
Fe  02  30  31  31  32  33  34  35  ff
第1次    12345
03h
Fe  03  31  31  ff
启动含量测量
测量时间30秒，重复次数2次
测量时间(秒)
31＝30
32＝60
33＝120
34＝240
35＝480
重复次数

31＝02次
32＝03次33＝05次
34＝10次35＝50次
移动滑板，启动含量测量
Fe  98  32  ff滑板到待测样位置
Fe  03  31  32  33  34  35  ff
Fe  98  32  ff滑板到待测样位置
Fe  03  35  34  33  32  31  ff…
测量完成1次后，回答计数值12345
测量完成2次后，回答计数值54321
04h
Fe  04  ff
启动标定样测量
移动滑板，启动标定样测量
Fe  98  32  ff  待测样位于测量位置
Fe  04  30  01  31  31  32  33  34  35  ff
Fe  98  31  ff参考样位于测量位置
Fe  04  30  01  32  31  32  33  34  35  ff
测量完成1次    先回答待测样计数值，再回答参考样到位，最后参考样计数值
05h
Fe  05  ff
需采样，移动台板
移动滑板，启动参考样测量
Fe  98  31  ff    参考样位于测量位置
Fe  05  31  32  33  34  35  ff
回答计数值12345
06h
Fe  06  31  ff
Fe  06  32  ff
移进台板
移出台板
Fe  98  32  ff            待测样位于测量位置
Fe  98  31  ff              参考样位于测量位置
Fe  98  33  ff            不到位
07h
Fe  07  30  36  30  35  ff
设定计数管高压为605v
Fe  98  31  ff              参考样位于测量位置
Fe  98  32  ff              待测样位于测量位置
Fe  98  33  ff            不到位
08h
Fe  08  36  35  30  30  ff
设定光管高压为6500v
09h
Fe  09  30  35  30  30  ff
设定光管电流为500ua
98h
Fe    98  ff
停止测量
停止当前进行的能谱、计数、含量、标定测量，如是待测样，则移动滑板到参考样位置。
Fe  98  31  ff              参考样位于测量位置
Fe  98  32  ff              待测样位于测量位置
Fe  98  33  ff            不到位


*/

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

    qint64 sendOrder(Com::Order order);

    /**
     *  @brief 发送含量测量命令
     *  @param[in] measurementTime 测量时间:
     *          1＝30
     *          2＝60
     *          3＝120
     *          4＝240
     *          5＝480
     *  @param[in] repeatTimes 重复次数:
     *          1＝02次
     *          2＝03次
     *          3＝05次
     *          4＝10次
     *          5＝50次
     */
    void sendSampleMeasure(uint measurementTime, uint repeatTimes);
signals:

public slots:
    QByteArray slotReadMyCom(); //读取串口
    void inPlateButtonClicked();
    void outPlateButtonClicked();

private:
    explicit Com(QObject *parent = 0);

    QextSerialBase *myCom;
    QTimer *readTimer;


#define TEST_COM  //test
#ifdef TEST_COM
public:
    QByteArray testData;
    void setRecvData(QByteArray data);
    QByteArray getRecvData();
#endif // test end
};

#endif // COM_H
