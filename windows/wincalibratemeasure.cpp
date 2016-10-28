#include "common/abstractfactory.h"
#include "common/datasave.h"
#include "common/wininforlistdialog.h"
#include "communication/com.h"
#include "common/database.h"

#include "wincalibratemeasure.h"
#include "buttonwinmannager.h"

#include <QPushButton>
#include <QDebug>

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

        QPushButton *p_inputSulfurContentButton = p_componentFactory->getButton(
                    tr("Input sulfur content"), this);
        p_hBoxLayout->addWidget(p_inputSulfurContentButton);

        addLayout(p_hBoxLayout);

        p_buttonWinMannager->addButtonWindow(
                    p_calibrateMeasureButton,
                    new CalibrateMeasure(this),
                    CalibrateMeasureID
                    );
        p_buttonWinMannager->addButtonWindow(
                    p_inputSulfurContentButton,
                    new InputSulfurContent(this),
                    InputSulfurContent
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
    m_platePositionLabel.clear();
    m_remainingTimeLabel.clear();
    m_referenceCountDataLabel.clear();
    m_testedCountDataLabel.clear();
}

void CalibrateMeasure::startButtonClicked()
{
    if(m_timer.isActive())
    {
        return;
    }
    init();

    Com::instance()->sendOrder(Com::CalibrateMeasure);

    if(readPlatePositon(StatusBar::Tested) == StatusBar::Tested)
    {
        setPlatePositionText(StatusBar::Tested);
        m_timer.start(50);
    }
}

void CalibrateMeasure::stopButtonClicked()
{
    init();
    m_timer.stop();
}

void CalibrateMeasure::readComData()
{
    uint remaingTime = 180;
    if(m_remainingTimeLabel.text().size() != 0)
    {
        remaingTime = m_remainingTimeLabel.text().toInt() - 1;
    }
    m_remainingTimeLabel.setText(QString::number(remaingTime));
    if(remaingTime == 0)
    {
        m_remainingTimeLabel.setText("180");
        readCountData();
    }
}

void CalibrateMeasure::setPlatePositionText(StatusBar::PlatePosition platePosition)
{
    Q_ASSERT(platePosition == StatusBar::Tested ||
             platePosition == StatusBar::Referencce
             );
    if(platePosition == StatusBar::Tested)
    {
        m_platePositionLabel.setText(tr("Measuring tested"));
    }else
    {
        m_platePositionLabel.setText(tr("Measuring reference"));
    }
}

StatusBar::PlatePosition CalibrateMeasure::readPlatePositon(
        StatusBar::PlatePosition platePosition)
{
#ifdef TEST_COM
    QByteArray recvData;
    recvData[0] = (char)0xfe;
    recvData[1] = (char)0x98;
    if(platePosition == StatusBar::Tested)
    {
        recvData[2] = (char)0x32;
    }
    else if(platePosition == StatusBar::Referencce)
    {
        recvData[2] = (char)0x31;
    }
    recvData[3] = (char)0xff;
    Com::instance()->setRecvData(recvData);
#endif

    return StatusBar::instance()->setPlatePositionByRecvData(
                Com::instance()->slotReadMyCom());
}

void CalibrateMeasure::readCountData()
{
#ifdef TEST_COM
    QByteArray data;
    data[0] = (char)0xfe;
    data[1] = (char)0x04;

    data[2] = (char)0x30;
    data[3] = (char)0x01;

    if(StatusBar::instance()->getPlatePosition() == StatusBar::Tested)
    {
        data[4] = (char)0x30 + (char)0x01;//tested
    }
    else
    {
        data[4] = (char)0x30 + (char)0x02;//reference
    }

    data[5] = (char)0x31;
    data[6] = (char)0x31;
    data[7] = (char)0x31;
    data[8] = (char)0x31;
    data[9] = (char)0x31;

    data[10] = (char)0xff;
    Com::instance()->setRecvData(data);
#endif
    QByteArray recvData = Com::instance()->slotReadMyCom();
    if(recvData == NULL
            || recvData.size() != 11
            || recvData[1] != (char)0x04
            )
    {
        WinInforListDialog::instance()->showMsg(tr("err") + recvData);
        ErrorCountSave::instance()->addCount(1);
        m_timer.stop();
        return;
    }

    if(recvData[2] == (char)0x31) //tested, first
    {
        m_testedCountDataLabel.setText(recvData.mid(3, 5));
        readPlatePositon(StatusBar::Referencce);
    }
    else if(recvData[2] == (char)0x32) //reference second
    {
        m_referenceCountDataLabel.setText(recvData.mid(3, 5));
        storeCalibrateData();
        m_timer.stop();
        m_remainingTimeLabel.clear();
        m_platePositionLabel.clear();
    }
    else
    {
        qDebug() << "recv Data err  :  " << recvData;
        stopButtonClicked();
    }
}

void CalibrateMeasure::storeCalibrateData()
{
    Q_ASSERT(m_referenceCountDataLabel.text().size() != 0);
    Q_ASSERT(m_testedCountDataLabel.text().size() != 0);
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

InputSulfurContent::InputSulfurContent(QWidget *parent)
    : WinAbstractFrame(parent)
    , m_model(this, Database::instance()->getDb())
{
    setTitle(tr("Input Sulfur content"));
    m_view.setModel(m_model);
    m_model.setTable(Database::instance()->getTableName(
                         Database::CalibrateData));
    p_SqlTableModel->select();

    addWidget(&m_view);
    addWidget(getReturnButton());
}
