#include "windows/buttonwinmannager.h"
#include "common/abstractfactory.h"

#include "winmainframe.h"
#include "winsystrmsettings.h"
#include "winquerydata.h"

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
        p_buttonWinMannager->addButtonWindow(p_syetemButton, new WinSystrmSettings(this), SystemSettings);
        p_hBoxLayout->addWidget(p_syetemButton);

        p_hBoxLayout->addWidget(getInPlateButton());

        addLayout(p_hBoxLayout);
    }

    {
        QBoxLayout *p_hBoxLayout = p_componentFactory->getBoxLayout(QBoxLayout::LeftToRight);

        QPushButton *p_DataQueryButton = p_componentFactory->getButton(tr("Data query"), this);
        p_buttonWinMannager->addButtonWindow(p_DataQueryButton, new WinQueryData(this), DataQuery);
        p_hBoxLayout->addWidget(p_DataQueryButton);

        QPushButton *p_syetemButton =p_componentFactory->getButton(tr("Self test"), this);
        p_hBoxLayout->addWidget(p_syetemButton);

        p_hBoxLayout->addWidget(getOutPlateButton());

        addLayout(p_hBoxLayout);
    }
}
