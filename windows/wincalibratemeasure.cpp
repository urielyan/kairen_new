#include "common/abstractfactory.h"
#include "common/datasave.h"
#include "common/wininforlistdialog.h"
#include "communication/com.h"

#include "wincalibratemeasure.h"
#include "buttonwinmannager.h"

#include <QPushButton>

WinCalibrateMeasure::WinCalibrateMeasure(QWidget *parent)
    : WinAbstractFrame(parent)
{
    setTitle(tr("Calibrate"));

    p_buttonWinMannager = new ButtonWinMannager(this);

    {
        QBoxLayout *p_hBoxLayout = p_componentFactory->getBoxLayout(
                    QBoxLayout::LeftToRight);

        QPushButton *p_clearCalibrateDataButton = p_componentFactory->getButton(
                    tr("Clear calibrate data"), this);
//        connect(p_clearCalibrateDataButton, &QPushButton::clicked,
//                this, &WinCalibrateMeasure::clearCalibrateDataClicked());
        p_hBoxLayout->addWidget(p_clearCalibrateDataButton);

        QPushButton *p_calibrateMeasureButton = p_componentFactory->getButton(
                    tr("calibrate measure"), this);
        p_hBoxLayout->addWidget(p_calibrateMeasureButton);

        QPushButton *p_inPlateButton = p_componentFactory->getButton(
                    tr("Input sample content"), this);
        p_hBoxLayout->addWidget(p_inPlateButton);

        addLayout(p_hBoxLayout);

        p_buttonWinMannager->addButtonWin(
                    p_calibrateMeasureButton,
                    new CalibrateMeasure(this),
                    CalibrateMeasureID
                    );
    }

    {
        QBoxLayout *p_hBoxLayout = p_componentFactory->getBoxLayout(
                    QBoxLayout::LeftToRight);

        QPushButton *p_sampleMeasureButton = p_componentFactory->getButton(
                    tr("Count kb value"), this);
        p_hBoxLayout->addWidget(p_sampleMeasureButton);

        p_hBoxLayout->addWidget(getReturnButton());
//        p_hBoxLayout->addStretch();

        addLayout(p_hBoxLayout);
    }
}

void WinCalibrateMeasure::clearCalibrateDataClicked()
{

}

CalibrateMeasure::CalibrateMeasure(QWidget *parent)
    :WinAbstractFrame(parent)
{
    setTitle(tr("Calibrate measure"));
    p_componentFactory = new MeasureFrameComponent(this);

    initMeasureLabel();
    initbutton();

    connect(&m_timer, SIGNAL(timeout()), this, SLOT(readComData()));

}

void CalibrateMeasure::init()
{

}

void CalibrateMeasure::startButtonClicked()
{
    if(m_timer.isActive())
    {
        return;
    }
    init();

    Com::instance()->sendOrder(Com::CountMeasure);
    m_timer.start(1);
}

void CalibrateMeasure::stopButtonClicked()
{

}

void CalibrateMeasure::queryButtonClicked()
{

}

void CalibrateMeasure::readComData()
{

}

bool CalibrateMeasure::readPlatePositon()
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

void CalibrateMeasure::initMeasureLabel()
{
    QBoxLayout *p_layout = p_componentFactory->getBoxLayout(
                QBoxLayout::LeftToRight);
    QBoxLayout *p_measureLayout = p_componentFactory->getBoxLayout(
                QBoxLayout::TopToBottom);
    QBoxLayout *p_dataLayout = p_componentFactory->getBoxLayout(
                QBoxLayout::TopToBottom);
    p_layout->addLayout(p_measureLayout);
    p_layout->addLayout(p_dataLayout);

    p_measureLayout->addWidget(&m_platePositionLabel);
    p_measureLayout->addWidget(&m_remainingTimeLabel);

    p_dataLayout->addWidget(&m_testedCountDataLabel);
    p_dataLayout->addWidget(&m_referenceCountDataLabel);

    addLayout(p_layout);
}

void CalibrateMeasure::initbutton()
{
    QBoxLayout *p_buttonLayout = p_componentFactory->getBoxLayout(
                QBoxLayout::LeftToRight);
    addLayout(p_buttonLayout);

    {
        QPushButton *p_startButton = p_componentFactory->getButton(
                    tr("Start"), this);
        connect(p_startButton, SIGNAL(clicked(bool)),
                this, SLOT(startButtonClicked()));
        p_buttonLayout->addWidget(p_startButton);


        QPushButton *p_stopButton = p_componentFactory->getButton(
                    tr("Stop"), this);
        connect(p_stopButton, SIGNAL(clicked(bool)),
                this, SLOT(stopButtonClicked()));
        p_buttonLayout->addWidget(p_stopButton);

        QPushButton *p_returnButton = getReturnButton();
        p_buttonLayout->addWidget(p_returnButton);
    }
}
