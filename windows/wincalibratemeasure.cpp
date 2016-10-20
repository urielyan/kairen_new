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
        QBoxLayout *p_hBoxLayout = p_componentFactory->getBoxLayout(QBoxLayout::LeftToRight);

        QPushButton *p_clearCalibrateDataButton = p_componentFactory->getButton(tr("Clear calibrate data"), this);
        //connect(p_clearCalibrateDataButton, &QPushButton::clicked, this, &WinCalibrateMeasure::clearCalibrateDataClicked());
        p_hBoxLayout->addWidget(p_clearCalibrateDataButton);

        QPushButton *p_calibrateMeasureButton = p_componentFactory->getButton(tr("calibrate measure"), this);
        p_hBoxLayout->addWidget(p_calibrateMeasureButton);

        QPushButton *p_inPlateButton = p_componentFactory->getButton(tr("Input sample content"), this);
        p_hBoxLayout->addWidget(p_inPlateButton);

        addLayout(p_hBoxLayout);

        p_buttonWinMannager->addButtonWin(p_calibrateMeasureButton, new CalibrateMeasure(this), CalibrateMeasureID);
    }

    {
        QBoxLayout *p_hBoxLayout = p_componentFactory->getBoxLayout(QBoxLayout::LeftToRight);

        QPushButton *p_sampleMeasureButton = p_componentFactory->getButton(tr("Count kb value"), this);
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

    connect(&m_timer, SIGNAL(timeout()), this, SLOT(slotReadComData()));

}

void CalibrateMeasure::init()
{

}

void CalibrateMeasure::slotStartButtonClicked()
{
    if(m_timer.isActive())
    {
        return;
    }
    init();

    Com::instance()->sendOrder(Com::CountMeasure);
    m_timer.start(1);
}

void CalibrateMeasure::slotStopButtonClicked()
{

}

void CalibrateMeasure::slotQueryButtonClicked()
{

}

void CalibrateMeasure::slotReadComData()
{

}

void CalibrateMeasure::initMeasureLabel()
{
    QBoxLayout *p_layout = p_componentFactory->getBoxLayout(QBoxLayout::LeftToRight);


    addLayout(p_layout);
}

void CalibrateMeasure::initbutton()
{
    QBoxLayout *p_buttonLayout = p_componentFactory->getBoxLayout(QBoxLayout::LeftToRight);
    addLayout(p_buttonLayout);

    {
        QPushButton *p_startButton = p_componentFactory->getButton(tr("Start"), this);
        connect(p_startButton, SIGNAL(clicked(bool)), this, SLOT(slotStartButtonClicked()));
        p_buttonLayout->addWidget(p_startButton);


        QPushButton *p_stopButton = p_componentFactory->getButton(tr("Stop"), this);
        connect(p_stopButton, SIGNAL(clicked(bool)), this, SLOT(slotStopButtonClicked()));
        p_buttonLayout->addWidget(p_stopButton);

        QPushButton *p_returnButton = getReturnButton();
        p_buttonLayout->addWidget(p_returnButton);
    }
}
