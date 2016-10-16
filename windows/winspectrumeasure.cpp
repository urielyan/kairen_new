#include <QTableWidget>
#include <QHeaderView>
#include <QTableWidgetItem>
#include <QDebug>
#include <QByteArray>

#include "mainwindow.h"
#include "winspectrumeasure.h"

#include "common/abstractfactory.h"
#include "common/wininforlistdialog.h"
#include "common/datasave.h"

#include "communication/com.h"

WinSpectruMeasure::WinSpectruMeasure(QWidget *parent)
    :WinAbstractFrame(parent)
{
    setTitle(tr("Spectrum measurement"));

    initTableWidget();

    QBoxLayout *p_buttonLayout = p_componentFactory->getBoxLayout(QBoxLayout::LeftToRight);
    addLayout(p_buttonLayout);

    {
        QPushButton *p_startButton = p_componentFactory->getButton(tr("Start"), this);
        connect(p_startButton, SIGNAL(clicked(bool)), this, SLOT(slotStartButtonClicked()));
        p_buttonLayout->addWidget(p_startButton);

        QPushButton *p_stopButton = p_componentFactory->getButton(tr("Stop"), this);
        connect(p_stopButton, SIGNAL(clicked(bool)), this, SLOT(slotStopbuttonClicked()));
        p_buttonLayout->addWidget(p_stopButton);

        QPushButton *p_viewSummitButton = p_componentFactory->getButton(tr("View summit"), this);
        connect(p_viewSummitButton, SIGNAL(clicked(bool)), this, SLOT(slotViewSummitButtonClicked()));
        p_buttonLayout->addWidget(p_viewSummitButton);

        p_buttonLayout->addWidget(getInPlateButton());
        p_buttonLayout->addWidget(getOutPlateButton());
        p_buttonLayout->addWidget(getReturnButton());
    }

    connect(&m_timer, SIGNAL(timeout()), this, SLOT(slotReadComData()));
}

void WinSpectruMeasure::slotStartButtonClicked()
{
    Com::instance()->sendOrder(Com::SpectrueMeasure);
    m_timer.start(2000);
}

void WinSpectruMeasure::slotStopbuttonClicked()
{
    qDebug() << __FILE__ << __LINE__;
    Com::instance()->sendOrder(Com::StopMeasure);
    m_timer.stop();

}

void WinSpectruMeasure::slotViewSummitButtonClicked()
{
    WinViewSummit *viewSummit = new WinViewSummit(p_tableWidget, this);
    MainWindow::instance()->slotSetWidget(viewSummit);
}

void WinSpectruMeasure::slotReadComData()
{
    QByteArray recvData = Com::instance()->slotReadMyCom();
    if(recvData == NULL || recvData.size() != 8|| recvData[1] != (char)0x01)
    {
        WinInforListDialog::instance()->showMsg(tr("err") + recvData);
        ErrorCountSave::instance()->addCount(1);
        m_timer.stop();
        return;
    }

    uint which = (int)recvData[2] * 10 + (int)recvData[3] - 4;
    int row = which / 10;
    int column = which % 10;
    QString value = QString::number((double)(which + 4)/10);
    QString count = recvData.mid(4, 5);
    p_tableWidget->item(row, column)->setText(value);
    p_tableWidget->item(row, column + 1)->setText(count);

    if(row == 9 && column == 6)
    {
        m_timer.stop();
    }
}

void WinSpectruMeasure::initTableWidget()
{
    p_tableWidget = new QTableWidget(10, 8, this);
//    p_tableWidget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
//    p_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
//    p_tableWidget->horizontalHeader()->setStretchLastSection(true);
//    p_tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//设置垂直滚动条显示模式
//    p_tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//设置水平滚动条显示模式
    for(int i = 0; i < 8; i++)
    {
        if(i % 2 == 0)
        {
            p_tableWidget->setHorizontalHeaderItem(i, new QTableWidgetItem(tr("Threshold")));
        }else
        {
            p_tableWidget->setHorizontalHeaderItem(i, new QTableWidgetItem(tr("count")));
        }
    }
    addWidget(p_tableWidget);
}

WinViewSummit::WinViewSummit(QTableWidget *tableWidget, QWidget *parent)
    : WinAbstractFrame(parent)
{
    Q_ASSERT(tableWidget->rowCount() == 10);
    Q_ASSERT(tableWidget->columnCount() == 8);

    setTitle(tr("View spectrum summit"));

    qDebug() << tableWidget->rowCount();

    addWidget(getReturnButton());
}
