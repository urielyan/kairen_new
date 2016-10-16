#include "winmainframe.h"

#include "common/buttonwinmannager.h"
#include "common/abstractfactory.h"

#include "winsystrmsettings.h"

WinMainFrame::WinMainFrame(QWidget *parent)
    : WinAbstractFrame(parent)
{
    setTitle(tr("Main"));
    p_buttonWinMannager = new ButtonWinMannager(this);

    {
        QBoxLayout *p_hBoxLayout = p_componentFactory->getBoxLayout(QBoxLayout::LeftToRight);

        QPushButton *p_sampleMeasureButton = p_componentFactory->getButton(tr("Sample measure"), this);
        //p_buttonWinMannager->addButtonWin(p_sampleMeasureButton, new WinSpectruMeasure(this), SampleMeasure);
        p_hBoxLayout->addWidget(p_sampleMeasureButton);


        QPushButton *p_syetemButton = p_componentFactory->getButton(tr("System"), this);
        p_buttonWinMannager->addButtonWin(p_syetemButton, new WinSystrmSettings(this), SystemSettings);
        p_hBoxLayout->addWidget(p_syetemButton);

        QPushButton *p_inPlateButton = p_componentFactory->getButton(tr("In plate"), this);
        p_hBoxLayout->addWidget(p_inPlateButton);

        addLayout(p_hBoxLayout);
    }

    {
        QBoxLayout *p_hBoxLayout = p_componentFactory->getBoxLayout(QBoxLayout::LeftToRight);

        QPushButton *p_sampleMeasureButton = p_componentFactory->getButton(tr("Data query"), this);
        p_hBoxLayout->addWidget(p_sampleMeasureButton);

        QPushButton *p_syetemButton =p_componentFactory->getButton(tr("Self test"), this);
        p_hBoxLayout->addWidget(p_syetemButton);

        QPushButton *p_outPlateButton = p_componentFactory->getButton(tr("Out plate"), this);
        p_hBoxLayout->addWidget(p_outPlateButton);

        addLayout(p_hBoxLayout);
    }
}
