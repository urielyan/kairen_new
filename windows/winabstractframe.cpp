#include <QPushButton>

#include"common/global.h"
#include "winabstractframe.h"
#include "mainwindow.h"
#include "common/abstractfactory.h"

WinAbstractFrame::WinAbstractFrame(QWidget *parent)
    : QFrame(parent)
{
    p_componentFactory = new MainComponent(this);

    setLayout(&m_mainVBoxLayout);
    p_title = new QLabel("title", this);
    p_title->setText("title");
    p_title->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    p_title->setFont(QFont("Times", 20));
    p_title->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    //m_mainVBoxLayout.addStretch();
    addWidget(p_title);
}

void WinAbstractFrame::setTitle(QString title)
{
    p_title->setText(title);
}

void WinAbstractFrame::addWidget(QWidget *widget)
{
    m_mainVBoxLayout.insertWidget(m_mainVBoxLayout.count(), widget);
}

void WinAbstractFrame::addLayout(QLayout *layout)
{
    m_mainVBoxLayout.insertLayout(m_mainVBoxLayout.count(), layout);
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

    return p_inPlateButton;
}

QPushButton *WinAbstractFrame::getOutPlateButton()
{
    QPushButton *p_outPlateButton = p_componentFactory->getButton(tr("Out plate"), this);

    return p_outPlateButton;
}

void WinAbstractFrame::slotReturnButtonClicked()
{
    MainWindow::instance()->slotCloseTopWidget();
}

