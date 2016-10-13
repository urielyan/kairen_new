#include "wincalibratemeasure.h"
#include "common/abstractfactory.h"

WinCalibrateMeasure::WinCalibrateMeasure(QWidget *parent)
    : WinAbstractFrame(parent)
{
    setTitle(tr("Calibrate"));

    {
        QBoxLayout *p_hBoxLayout = p_componentFactory->getBoxLayout(QBoxLayout::LeftToRight);

        QPushButton *p_sampleMeasureButton = p_componentFactory->getButton(tr("Clear calibrate data"), this);
        //connect(p_sampleMeasureButton, SIGNAL(clicked(bool)), this, SLOT(slotSampleMeasurebuttonClicked()));
        p_hBoxLayout->addWidget(p_sampleMeasureButton);

        QPushButton *p_syetemButton = p_componentFactory->getButton(tr("calibrate measure"), this);
        p_hBoxLayout->addWidget(p_syetemButton);

        QPushButton *p_inPlateButton = p_componentFactory->getButton(tr("Input sample content"), this);
        p_hBoxLayout->addWidget(p_inPlateButton);

        addLayout(p_hBoxLayout);
    }

    {
        QBoxLayout *p_hBoxLayout = p_componentFactory->getBoxLayout(QBoxLayout::LeftToRight);

        QPushButton *p_sampleMeasureButton = p_componentFactory->getButton(tr("Count kb value"), this);
        //connect(p_sampleMeasureButton, SIGNAL(clicked(bool)), this, SLOT(slotSampleMeasurebuttonClicked()));
        p_hBoxLayout->addWidget(p_sampleMeasureButton);

        p_hBoxLayout->addWidget(getReturnButton());
//        p_hBoxLayout->addStretch();

        addLayout(p_hBoxLayout);
    }
}
