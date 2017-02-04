#include "common/abstractfactory.h"
#include "common/datasave.h"
#include "common/database.h"
#include "common/wininforlistdialog.h"
#include "communication/com.h"

#include "mainwindow.h"
#include "wincountmeasure.h"

#include <QTableWidget>
#include <QtMath>
#include <QDateTime>

WinCountMeasure::WinCountMeasure(QWidget *parent)
    : WinAbstractFrame(parent)
    ,m_flagRepeatMeasure(false)
{
    setTitle(tr("count measure"));
    p_componentFactory = new MeasureFrameComponent(this);

    initMeasureLabel();
    initbutton();

    connect(&m_timer, SIGNAL(timeout()), this, SLOT(slotReadComData()));

    {
        m_timeMap[1] = tr("First");
        m_timeMap[2] = tr("Second");
        m_timeMap[3] = tr("Third");
        m_timeMap[4] = tr("Fourth");
        m_timeMap[5] = tr("Fifth");
        m_timeMap[6] = tr("Sixth");
        m_timeMap[7] = tr("Seventh");
        m_timeMap[8] = tr("Eighth");
        m_timeMap[9] = tr("Ninth");
        m_timeMap[10] = tr("Tenth");
        m_timeMap[11] = tr("Eleventh");
    }
}

void WinCountMeasure::init()
{
    m_timer.stop();
    clearChangeLabel();

    setButtonEnabled(true);
}

void WinCountMeasure::slotStartButtonClicked()
{
    if(m_timer.isActive())
    {
        return;
    }
    init();
    setButtonEnabled(false);
    m_countData.clear();

    Com::instance()->sendOrder(Com::CountMeasure);
    m_timer.start(1);
}

void WinCountMeasure::slotRepeatMeasureButtonClicked()
{
    slotStartButtonClicked();
    m_flagRepeatMeasure = true;
}

void WinCountMeasure::slotStopButtonClicked()
{
    Com::instance()->sendOrder(Com::StopMeasure);
    init();
    m_flagRepeatMeasure = false;
}

void WinCountMeasure::slotQueryButtonClicked()
{
    m_queryCountMeasure.setTableWidgetData(m_countData);
    MainWindow::instance()->slotSetWidget(&m_queryCountMeasure);
}

void WinCountMeasure::slotReadComData()
{
    //得到当前次数
    QString currentTimeString = m_currentTimeLabel.text();
    uint currentTime = m_timeMap.key(currentTimeString, 1);

    //得到当前测量剩余时间
    QString remainTimeString = m_remainingTimelabel.text();
    uint remainTime = 0;
    if(remainTimeString.size() == 0)
    {
        remainTime = 31;
    }else
    {
        remainTime = remainTimeString.remove(tr("Remaining")).remove(tr("seconds")).toUInt();
    }

    //若当前剩余时间为0，则读取串口数据
    if(remainTime == 0)
    {
        readComData();
        currentTime++;

        //若当前次数等于12次则停止测量保存数据，显示测量结果。
        if(currentTime == 12)
        {
            slotQueryButtonClicked();
            init();

            //若是重复测量则重新开始新的计数测量。
            if(m_flagRepeatMeasure)
            {
                slotStopButtonClicked();
                slotRepeatMeasureButtonClicked();
                MainWindow::instance()->closeTopWidget();
            }
            return;
        }
        remainTime = 31;
    }

    setChangeLabel(currentTime, --remainTime);
}

void WinCountMeasure::setChangeLabel(uint currentTime, uint remainingTime)
{
    Q_ASSERT(currentTime < 12);
    Q_ASSERT(remainingTime < 32);

    m_currentTimeLabel.setText(m_timeMap[currentTime]);
    m_remainingTimelabel.setText(
                tr("Remaining %1 seconds").arg(remainingTime));
}


void WinCountMeasure::clearChangeLabel()
{
    m_currentTimeLabel.clear();
    m_remainingTimelabel.clear();
}

void WinCountMeasure::setButtonEnabled(bool value)
{
    foreach (QAbstractButton *button, m_buttonGroup.buttons()) {
        button->setEnabled(value);
    }
}

void WinCountMeasure::readComData()
{
#ifdef TEST_COM
    QByteArray data;
    data[0] = (char)0xfe;
    data[1] = (char)0x02;
    data[2] = (char)0x30 + (char)0x01;
    data[3] = (char)0x30 + (char)0x02;

    data[4] = (char)0x31;
    data[5] = (char)0x32;
    data[6] = (char)0x33;
    data[7] = (char)0x30;
    data[8] = (char)0x30;

    data[9] = (char)0xff;
    Com::instance()->setRecvData(data);
#endif
    QByteArray recvData = Com::instance()->slotReadMyCom();
    if(recvData == NULL || recvData.size() != 10|| recvData[1] != (char)0x02)
    {
        WinInforListDialog::instance()->showMsg(tr("err") + recvData);
        ErrorCountSave::instance()->addCount(1);
        m_timer.stop();
        return;
    }

    //uint which = (int)recvData[2] * 10 + (int)recvData[3] - 4;

    QString count = recvData.mid(4, 5);
    m_countData.append(count.toUInt());
}


void WinCountMeasure::initMeasureLabel()
{
    QBoxLayout *p_layout = p_componentFactory->getBoxLayout(QBoxLayout::LeftToRight);

    QLabel *p_informationLabel = p_componentFactory->getLabel(tr("A total of 11 times, 31 seconds every time."));
    p_layout->addWidget(p_informationLabel);
    p_layout->addWidget(&m_currentTimeLabel);

    addLayout(p_layout);
    addWidget(&m_remainingTimelabel);
    m_remainingTimelabel.setAlignment(Qt::AlignHCenter);
}

void WinCountMeasure::initbutton()
{
    QBoxLayout *p_buttonLayout = p_componentFactory->getBoxLayout(QBoxLayout::LeftToRight);
    addLayout(p_buttonLayout);

    {
        QPushButton *p_startButton = p_componentFactory->getButton(tr("Start"), this);
        connect(p_startButton, SIGNAL(clicked(bool)), this, SLOT(slotStartButtonClicked()));
        p_buttonLayout->addWidget(p_startButton);

        QPushButton *p_repeatMeasureButton = p_componentFactory->getButton(tr("Repeat measure"), this);
        connect(p_repeatMeasureButton, SIGNAL(clicked(bool)), this, SLOT(slotRepeatMeasureButtonClicked()));
        p_buttonLayout->addWidget(p_repeatMeasureButton);

        QPushButton *p_stopButton = p_componentFactory->getButton(tr("Stop"), this);
        connect(p_stopButton, SIGNAL(clicked(bool)), this, SLOT(slotStopButtonClicked()));
        p_buttonLayout->addWidget(p_stopButton);

        QPushButton *p_queryButton = p_componentFactory->getButton(tr("Query"), this);
        connect(p_queryButton, SIGNAL(clicked(bool)), this, SLOT(slotQueryButtonClicked()));
        p_buttonLayout->addWidget(p_queryButton);

        QPushButton *p_inPlatebutton = getInPlateButton();
        p_buttonLayout->addWidget(p_inPlatebutton);

        QPushButton *p_outPlatebutton = getOutPlateButton();
        p_buttonLayout->addWidget(p_outPlatebutton);

        QPushButton *p_returnButton = getReturnButton();
        p_buttonLayout->addWidget(p_returnButton);

        m_buttonGroup.addButton(p_startButton, Start);
        m_buttonGroup.addButton(p_repeatMeasureButton, RepeatMeasure);
//        m_buttonGroup.addButton(p_stopButton, Stop);
        m_buttonGroup.addButton(p_inPlatebutton, InPlate);
        m_buttonGroup.addButton(p_outPlatebutton, OutPlate);
        m_buttonGroup.addButton(p_returnButton, Return);
    }
}


QueryCountMeasure::QueryCountMeasure(QWidget *parent)
    : WinAbstractFrame(parent)
{
    setTitle(tr("Query count measure data"));
    p_mainBoxlayout = p_componentFactory->getBoxLayout(QBoxLayout::LeftToRight);
    addLayout(p_mainBoxlayout);

    initTableWidget();
    initLabel();
}

void QueryCountMeasure::setTableWidgetData(const QList<uint> &list)
{
    p_tableWidget->clearContents();
    for(int i = 0; i < list.size(); i++)
    {
        p_tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(list[i])));
    }

    long long sum = 0;
    double sumSub = 0;
    double lambda = 0.0;
    if(list.size() == 11)
    {
        for(int i = 0; i < list.size(); i++)
        {
            sum += list[i];
        }

        for(int i = 0; i < list.size(); i++)
        {
            sumSub += qPow((double)sum/11 - list[i],2);
        }

        lambda = qPow((sumSub / (double)(sum/11)) * ((double)31/10),0.5);

        setLabelText(sum / 11, lambda);
        saveData(sum / 11, lambda);
    }
}

void QueryCountMeasure::setLabelText(int average, double lambda)
{
    m_average.setText(tr("Count average = ").append(QString::number(average)));
    m_lambda.setText(tr("lambda = ").append(QString::number(lambda)));
}

void QueryCountMeasure::setLabelText(QString average, QString lambda)
{
    m_average.setText(tr("Count average = ").append(average));
    m_lambda.setText(tr("λ = ").append(lambda));
}

void QueryCountMeasure::initTableWidget()
{
    p_tableWidget = new QTableWidget(11, 1, this);
    p_tableWidget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
//    p_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
//    p_tableWidget->horizontalHeader()->setStretchLastSection(true);
//    p_tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//设置垂直滚动条显示模式
//    p_tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//设置水平滚动条显示模式


    p_tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("count")));

    p_mainBoxlayout->addWidget(p_tableWidget);
}

void QueryCountMeasure::initLabel()
{
    QBoxLayout *p_boxLayout = p_componentFactory->getBoxLayout(QBoxLayout::TopToBottom);

    QLabel *p_ns = new QLabel(tr("Ns:"), this);
    p_boxLayout->addWidget(p_ns);
    p_boxLayout->addWidget(&m_average);
    p_boxLayout->addWidget(&m_lambda);
    setLabelText("", "");
    p_boxLayout->addWidget(getReturnButton());

    p_mainBoxlayout->addLayout(p_boxLayout);
}

void QueryCountMeasure::saveData(int average, double lambda)
{
    uint count =  CountDataSave::instance()->value(MYSETTINGS_COUNT_COUNT).toUInt() + 1;
    if(count > MYSETTINGS_COUNT_MAX_COUNT)
    {
        count = 1;
    }
    CountDataSave::instance()->setValue(MYSETTINGS_COUNT_DATA_AVERAGE(count), average);
    CountDataSave::instance()->setValue(MYSETTINGS_COUNT_DATA_LAMBDA(count), lambda);
    CountDataSave::instance()->setValue(MYSETTINGS_COUNT_DATA_DATETIME(count),
                                        QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss"));
    CountDataSave::instance()->setValue(MYSETTINGS_COUNT_COUNT,count);

    Database::instance()->insertDataToCountData(average, lambda);
}
