#include "common/global.h"
#include "common/wininforlistdialog.h"
#include "common/abstractfactory.h"
#include "windows/buttonwinmannager.h"
#include "common/datasave.h"

#include "winquerydata.h"

#include <QButtonGroup>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QTableWidget>

WinQueryData::WinQueryData(QWidget *parent)
    : WinAbstractFrame(parent)
{
    setTitle(tr("Query history data"));

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
    setTitle(tr("Query count data"));
    p_componentFactory = new MeasureFrameComponent(this);

    initTableWidget();

    QBoxLayout *p_buttonLayout = p_componentFactory->getBoxLayout(QBoxLayout::LeftToRight);
    addLayout(p_buttonLayout);
    {
        QPushButton *p_updateButton = p_componentFactory->getButton(tr("Update"), this);
        connect(p_updateButton, SIGNAL(clicked(bool)), this, SLOT(slotUpdateButtonClicked()));
        p_buttonLayout->addWidget(p_updateButton);

        QPushButton *p_previousButton = p_componentFactory->getButton(tr("Previous"), this);
        connect(p_previousButton, SIGNAL(clicked(bool)), this, SLOT(slotPreviousButtonClicked()));
        p_buttonLayout->addWidget(p_previousButton);

        QPushButton *p_netxButton = p_componentFactory->getButton(tr("Netx"), this);
        connect(p_netxButton, SIGNAL(clicked(bool)), this, SLOT(slotNetxButtonClicked()));
        p_buttonLayout->addWidget(p_netxButton);

        p_buttonLayout->addWidget(getReturnButton());
    }

    slotUpdateButtonClicked();
}

void WinQueryCountData::slotUpdateButtonClicked()
{
    uint id = CountDataSave::instance()->value(MYSETTINGS_COUNT_COUNT).toUInt();
    Q_ASSERT(id <= MYSETTINGS_COUNT_MAX_COUNT);

    setTableWidget(id);
}

void WinQueryCountData::slotNetxButtonClicked()
{
    uint currentMinId = 0;
    for (int row = p_tableWidget->rowCount() - 1; row >= 0; row--)
    {
         if(p_tableWidget->item(row, 0) != NULL)
         {
             currentMinId = p_tableWidget->item(row, 0)->text().toUInt();
         }else
         {
             continue;
         }

         if(CountDataSave::instance()->value(MYSETTINGS_COUNT_DATA_DATETIME(currentMinId)).toString().size() != 0)
         {
             break;
         }
    }
    int displayId = currentMinId - 1;
    for(; displayId > 0; displayId--)
    {
        if(CountDataSave::instance()->value(MYSETTINGS_COUNT_DATA_DATETIME(displayId)).toString().size() != 0)
        {
            break;
        }
    }
    if(displayId == 0)
    {
        return;
    }
    setTableWidget(displayId);
}

void WinQueryCountData::slotPreviousButtonClicked()
{
    uint currentMaxId = 0;
    if(p_tableWidget->item(0, 0) != NULL)
    {
        currentMaxId = p_tableWidget->item(0, 0)->text().toUInt();
    }

    int displayId = currentMaxId + p_tableWidget->rowCount();
    for(; displayId > 0; displayId--)
    {
        if(CountDataSave::instance()->value(MYSETTINGS_COUNT_DATA_DATETIME(displayId)).toString().size() != 0)
        {
            break;
        }
    }

    setTableWidget(displayId);
}

void WinQueryCountData::initTableWidget()
{
    p_tableWidget = new QTableWidget(6, 4, this);
    p_tableWidget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    p_tableWidget->setColumnWidth(1, DESKTOP_WIDTH / 3);

    p_tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("ID")));
    p_tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem(tr("Date time")));
    p_tableWidget->setHorizontalHeaderItem(2, new QTableWidgetItem(tr("Average")));
    p_tableWidget->setHorizontalHeaderItem(3, new QTableWidgetItem(tr("Lambda")));

    addWidget(p_tableWidget);
}

void WinQueryCountData::setTableWidget(uint id)
{
    p_tableWidget->clearContents();
    for (int row = 0; row < p_tableWidget->rowCount(); row++)
    {
        int displayId = id - row;
        if(displayId <= 0)
        {
            displayId = MYSETTINGS_COUNT_MAX_COUNT + displayId;
        }
        if(CountDataSave::instance()->value(MYSETTINGS_COUNT_DATA_DATETIME(displayId)).toString().size() == 0)
        {
            continue;
        }
        p_tableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(displayId)));
        p_tableWidget->setItem(row, 1, new QTableWidgetItem(
                                   CountDataSave::instance()->value(MYSETTINGS_COUNT_DATA_DATETIME(displayId)).toString()));
        p_tableWidget->setItem(row, 2, new QTableWidgetItem(
                                   CountDataSave::instance()->value(MYSETTINGS_COUNT_DATA_AVERAGE(displayId)).toString()));
        p_tableWidget->setItem(row, 3, new QTableWidgetItem(
                                   CountDataSave::instance()->value(MYSETTINGS_COUNT_DATA_LAMBDA(displayId)).toString()));
    }
}
