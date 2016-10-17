#include <QPushButton>

#include "winabstractframe.h"
#include "mainwindow.h"
#include "statusbar.h"

#include"common/global.h"
#include "common/abstractfactory.h"
#include "common/wininforlistdialog.h"

#include "communication/com.h"

WinAbstractFrame::WinAbstractFrame(QWidget *parent)
    : QFrame(parent)
{
    p_componentFactory = new MainComponent(this);

    setLayout(&m_mainVBoxLayout);
    p_title = new QLabel("title", this);
    p_title->setText("title");
    p_title->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    p_title->setFont(QFont("Times", 20));
    p_title->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);

    //m_mainVBoxLayout.addStretch();
    addWidget(p_title);
}

void WinAbstractFrame::setTitle(QString title)
{
    p_title->setText(title);
}

void WinAbstractFrame::addWidget(QWidget *widget)
{
    m_mainVBoxLayout.insertWidget(m_mainVBoxLayout.count(), widget, 1);
}

void WinAbstractFrame::addLayout(QLayout *layout)
{
    m_mainVBoxLayout.insertLayout(m_mainVBoxLayout.count(), layout, 1);
}

QPushButton *WinAbstractFrame::getReturnButton()
{
    QPushButton *p_returnButton = p_componentFactory->getButton(tr("Return"), this);
    connect(p_returnButton, SIGNAL(clicked(bool)), this, SLOT(slotReturnButtonClicked()));
    return p_returnButton;
}

QPushButton *WinAbstractFrame::getInPlateButton()
{
    QPushButton *p_inPlateButton = p_componentFactory->getButton(tr("In plate"), this);
    connect(p_inPlateButton, SIGNAL(clicked()), this, SLOT(slotInPlatebuttonClicked()));
    return p_inPlateButton;
}

QPushButton *WinAbstractFrame::getOutPlateButton()
{
    QPushButton *p_outPlateButton = p_componentFactory->getButton(tr("Out plate"), this);
    connect(p_outPlateButton, SIGNAL(clicked()), this, SLOT(slotOutPlatebuttonClicked()));
    return p_outPlateButton;
}

void WinAbstractFrame::slotReturnButtonClicked()
{
    MainWindow::instance()->slotCloseTopWidget();
}

void WinAbstractFrame::slotInPlatebuttonClicked()
{
#ifdef TEST_COM
    QByteArray recvData;
    recvData[0] = (char)0xfe;
    recvData[1] = (char)0x98;
    recvData[2] = (char)0x32;
    recvData[3] = (char)0xff;
    Com::instance()->setRecvData(recvData);
#endif


    Com::instance()->sendOrder(Com::InPlate);
    setPlatePosition(Com::instance()->slotReadMyCom());
}

void WinAbstractFrame::slotOutPlatebuttonClicked()
{
#ifdef TEST_COM
    QByteArray recvData;
    recvData[0] = (char)0xfe;
    recvData[1] = (char)0x98;
    recvData[2] = (char)0x31;
    recvData[3] = (char)0xff;
    Com::instance()->setRecvData(recvData);
#endif

    Com::instance()->sendOrder(Com::OutPlate);
    setPlatePosition(Com::instance()->slotReadMyCom());
}

/**
*   @brief 接收到的数据为0xfe 0x98 0x31(32,33) 0xff
*/
void WinAbstractFrame::setPlatePosition(QByteArray recvData)
{
    if(recvData.isEmpty())
    {
        StatusBar::instance()->slotUpdatePosition(StatusBar::UnKnow);
        WinInforListDialog::instance()->showMsg(tr("recv null"));
        return;
    }else if(recvData[2] == (char)0x31)
    {
        StatusBar::instance()->slotUpdatePosition(StatusBar::Referencce);
    }else if(recvData[2] == (char)0x32)
    {
        StatusBar::instance()->slotUpdatePosition(StatusBar::Tested);
    }else if(recvData[2] == (char)0x33)
    {
        StatusBar::instance()->slotUpdatePosition(StatusBar::Malfunction);
    }else
    {
        StatusBar::instance()->slotUpdatePosition(StatusBar::UnKnow);
        WinInforListDialog::instance()->showMsg(tr("recv err") + QString(recvData).toInt());
    }
}

