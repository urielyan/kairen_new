#include"common/global.h"
#include "common/abstractfactory.h"
#include "common/wininforlistdialog.h"
#include "communication/com.h"

#include "winabstractframe.h"
#include "mainwindow.h"

#include <QPushButton>

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

void WinAbstractFrame::onEntry()
{
    //Do nothing
}

void WinAbstractFrame::onExit()
{
    //Do nothing
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
    QPushButton *p_returnButton = p_componentFactory->getButton(
                tr("Return"), this);
    connect(p_returnButton, SIGNAL(clicked(bool)),
            this, SLOT(slotReturnButtonClicked()));
    return p_returnButton;
}

QPushButton *WinAbstractFrame::getInPlateButton()
{
    QPushButton *p_inPlateButton = p_componentFactory->getButton(
                tr("In plate"), this);
    connect(p_inPlateButton, &QPushButton::clicked,
            Com::instance(), &Com::inPlateButtonClicked);
    return p_inPlateButton;
}

QPushButton *WinAbstractFrame::getOutPlateButton()
{
    QPushButton *p_outPlateButton = p_componentFactory->getButton(
                tr("Out plate"), this);
    connect(p_outPlateButton, &QPushButton::clicked,
            Com::instance(), &Com::outPlateButtonClicked);
    return p_outPlateButton;
}

void WinAbstractFrame::slotReturnButtonClicked()
{
    MainWindow::instance()->closeTopWidget();
}

