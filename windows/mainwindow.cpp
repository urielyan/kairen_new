#include "common/global.h"

#include "winmainframe.h"
#include "winabstractframe.h"
#include "statusbar.h"
#include "mainwindow.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QDebug>




MainWindow *MainWindow::instance()
{
    static MainWindow _instance;
    return &_instance;
}

MainWindow::MainWindow(QWidget *parent)
    : QFrame(parent)
{
    this->setFixedHeight(480);
    this->setFixedWidth(800);

    QVBoxLayout *p_mainLayout = new QVBoxLayout(this);
    p_mainLayout->addLayout(&m_stackedLayout, 9);
    p_mainLayout->addWidget(StatusBar::instance(), 1);

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
