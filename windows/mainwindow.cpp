#include <QVBoxLayout>
#include <QPushButton>
#include <QDebug>

#include "common/global.h"
#include "common/abstractfactory.h"
#include "common/buttonwinmannager.h"

#include "statusbar.h"
#include "mainwindow.h"
#include "winsystrmsettings.h"


MainWindow *MainWindow::instance()
{
    static MainWindow _instance;
    return &_instance;
}

MainWindow::MainWindow(QWidget *parent)
    : QFrame(parent)
{
    this->setFixedHeight(DESKTOP_HEIGHT / 2);
    this->setFixedWidth(DESKTOP_WIDTH / 2);

    QVBoxLayout *p_mainLayout = new QVBoxLayout(this);
    p_mainLayout->addLayout(&m_stackedLayout);
    p_mainLayout->addStretch();
    p_mainLayout->addWidget(StatusBar::instance());

    WinMainFrame *p_mainFrame = new WinMainFrame(this);
    slotSetWidget(p_mainFrame);
}

MainWindow::~MainWindow()
{

}

void MainWindow::addWindget(MainWindow::WinId id, WinAbstractFrame *frame)
{
    m_winMap[id] = frame;
}

void MainWindow::slotSetWidget(QWidget *widget)
{
    Q_ASSERT(widget != NULL);
    m_stackedLayout.addWidget(widget);
    m_stackedLayout.setCurrentWidget(widget);
}

void MainWindow::slotCloseTopWidget()
{
    m_stackedLayout.removeWidget(m_stackedLayout.widget(m_stackedLayout.count() - 1));
}

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
