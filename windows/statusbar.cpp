#include "common/global.h"

#include "statusbar.h"

#include <unistd.h>

extern int measurement_flag;
#define PWM_IOCTL_SET_FREQ	1
#define PWM_IOCTL_STOP		0

StatusBar *StatusBar::instance()
{
    static StatusBar _instance;
    return &_instance;
}

StatusBar::StatusBar(QWidget *parent)
    : QWidget(parent)
    , platePosition(UnKnow)
    ,timerIsPreheat(NULL)
{
    QHBoxLayout *p_mainLayout = new QHBoxLayout(this);

    initPlatePosition();
    initPreheat();
    initDatetime();

    p_mainLayout->addWidget(&m_plateLabel);
    p_mainLayout->addWidget(&m_labelIsPreheatSampling);
    p_mainLayout->addWidget(&m_labelDateTime);
}
int StatusBar::samplingNumber = 0;

StatusBar::~StatusBar()
{
}


/**
*   @brief 接收到的数据为0xfe 0x98 0x31(32,33) 0xff
*/
StatusBar::PlatePosition StatusBar::setPlatePositionByRecvData(QByteArray recvData)
{
    if(recvData.isEmpty())
    {
        StatusBar::instance()->slotUpdatePosition(StatusBar::UnKnow);
        //WinInforListDialog::instance()->showMsg(tr("recv null"));
        platePosition = StatusBar::UnKnow;
    }else if(recvData[2] == (char)0x31)
    {
        StatusBar::instance()->slotUpdatePosition(StatusBar::Referencce);
        platePosition = StatusBar::Referencce;
    }else if(recvData[2] == (char)0x32)
    {
        StatusBar::instance()->slotUpdatePosition(StatusBar::Tested);
        platePosition = StatusBar::Tested;
    }else if(recvData[2] == (char)0x33)
    {
        StatusBar::instance()->slotUpdatePosition(StatusBar::Malfunction);
        platePosition = StatusBar::Malfunction;
    }else
    {
        StatusBar::instance()->slotUpdatePosition(StatusBar::UnKnow);
        //WinInforListDialog::instance()->showMsg(tr("recv err") + QString(recvData).toInt());
        platePosition = StatusBar::UnKnow;
    }

    return platePosition;
}

StatusBar::PlatePosition StatusBar::getPlatePosition()
{
    return platePosition;
}

void StatusBar::slotUpdatepreheat(){
    static uint ispreheat = 30;

    QString tmpstr =  QString("预热还需%1分钟").arg(--ispreheat);
    m_labelIsPreheatSampling.setText(tmpstr);
    if(ispreheat == 0){
        timerIsPreheat->stop();
        m_labelIsPreheatSampling.setText("");
    }
}

void StatusBar::initPlatePosition()
{
    slotUpdatePosition(UnKnow);
}

void StatusBar::initPreheat()
{
    m_labelIsPreheatSampling.setText(tr("预热还需30分钟"));

    blink_flag = 1;

    timerIsPreheat = new QTimer(this);
    connect(timerIsPreheat, SIGNAL(timeout()), this, SLOT(slotUpdatepreheat()));
    timerIsPreheat->start(100);//
}

void StatusBar::initDatetime()
{
    slotUpdateDateTime();

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(slotUpdateDateTime()));
    timer->start(1000);
}

void StatusBar::slotUpdateSampling(){
    if((samplingNumber >= 0 && samplingNumber <= COUNT_MEASUREMENT_DATA_COUNT) || samplingNumber == NEED_START_SAMPLING){
        if(1 == blink_flag){
            switch (samplingNumber) {
            case NEED_START_SAMPLING:
                m_labelIsPreheatSampling.setText("请移动滑板到参考样采样!");
                blink_flag  = 0;
                break;
            case 0:
                m_labelIsPreheatSampling.setStyleSheet("background-image:url(/sdcard/0.png);background-repeat:no-repeat");
                blink_flag = 0;
                break;
            case 1:
                m_labelIsPreheatSampling.setText("正在采样①");
                blink_flag = 0;
                break;
            case 2:
                m_labelIsPreheatSampling.setText("正在采样①②");
                blink_flag = 0;
                break;
            case 3:
                m_labelIsPreheatSampling.setText("正在采样①②③");
                blink_flag = 0;
                break;
            case 4:
                m_labelIsPreheatSampling.setText("正在采样①②③④");
                blink_flag = 0;
                break;
            case 5:
                m_labelIsPreheatSampling.setText("正在采样①②③④⑤");
                blink_flag = 0;
                break;
            case 6:
                m_labelIsPreheatSampling.setText("正在采样①②③④⑤⑥");
                blink_flag = 0;
                break;
            case 7:
                m_labelIsPreheatSampling.setText("正在采样①②③④⑤⑥⑦");
                blink_flag = 0;
                break;
            case 8:
                m_labelIsPreheatSampling.setText("正在采样①②③④⑤⑥⑦⑧");
                blink_flag = 0;
                break;
            case 9:
                m_labelIsPreheatSampling.setText("正在采样①②③④⑤⑥⑦⑧⑨");
                blink_flag = 0;
                break;
            case 10:
                m_labelIsPreheatSampling.setText("正在采样①②③④⑤⑥⑦⑧⑨⑩");
                blink_flag = 0;
                break;
            default:
                blink_flag = 0;
                break;
            }
        }else{
            m_labelIsPreheatSampling.setText("");
            blink_flag = 1;
        }
    }
}

void StatusBar::slotUpdatePosition(PlatePosition position)
{
    switch (position) {
    case Referencce:
        m_plateLabel.setText(REFERENCE_BE_LOCATON_TEXT);
        break;
    case Tested:
        m_plateLabel.setText(WAIT_BE_LOCATION_TEXT);
        break;
    case Malfunction:
        m_plateLabel.setText(MACHINE_MALFUNCTION_TEXT);
        break;
    case UnKnow:
        m_plateLabel.setText(SLIDING_PLATE_NO_CHANGE_TEXT);
        //ui->label->setStyleSheet("background-image:");
        break;
    default:
        break;
    }
}

void StatusBar::slotUpdateDateTime()
{
    m_labelDateTime.setText(QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss"));
}
