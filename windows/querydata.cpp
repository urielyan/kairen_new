#include "querydata.h"

#include <QButtonGroup>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QDateTime>

#include "common/global.h"
#include "common/wininforlistdialog.h"
#include "common/abstractfactory.h"
#include "common/buttonwinmannager.h"


QueryData::QueryData(QWidget *parent)
    : WinAbstractFrame(parent)
{
    setTitle(tr("Query data"));

    p_buttonWinMannager = new ButtonWinMannager(this);
    {
        QBoxLayout *p_hBoxLayout = p_componentFactory->getBoxLayout(QBoxLayout::LeftToRight);

        QPushButton *p_querySampleButton = p_componentFactory->getButton(tr("Query sample data"), this);
        p_hBoxLayout->addWidget(p_querySampleButton);

        QPushButton *p_queryCalibrateButton = p_componentFactory->getButton(tr("Query calibrate data"), this);
        p_hBoxLayout->addWidget(p_queryCalibrateButton);

        addLayout(p_hBoxLayout);

//        p_buttonWinMannager->addButtonWin(p_querySampleButton, new WinSpectruMeasure(this), Sample);
//        p_buttonWinMannager->addButtonWin(p_queryCalibrateButton, new WinCountMeasure(this), Calibrate);
    }

    {
        QBoxLayout *p_hBoxLayout = p_componentFactory->getBoxLayout(QBoxLayout::LeftToRight);

        QPushButton *p_queryCountButton = p_componentFactory->getButton(tr("Query count data"), this);
        p_hBoxLayout->addWidget(p_queryCountButton);

        p_hBoxLayout->addWidget(getReturnButton());

        addLayout(p_hBoxLayout);

        p_buttonWinMannager->addButtonWin(p_queryCountButton, new WinQueryCountData(this), Count);
    }
}


WinQueryCountData::WinQueryCountData(QWidget *parent)
    : WinAbstractFrame(parent)
{

}
