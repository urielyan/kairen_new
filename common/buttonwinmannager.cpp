#include <QButtonGroup>

#include "buttonwinmannager.h"
#include "windows/mainwindow.h"

ButtonWinMannager::ButtonWinMannager(QObject *parent) : QObject(parent)
{
    p_buttonGroup = new QButtonGroup(this);
    connect(p_buttonGroup, SIGNAL(buttonClicked(int)), this, SLOT(slotbuttonClicked(int)));
}

void ButtonWinMannager::addButtonWin(QAbstractButton *button, WinAbstractFrame *win, int index)
{
    Q_ASSERT(button != NULL);
    Q_ASSERT(win != NULL);
    if(index == 0)
    {
        index = m_winMap.count();
    }

    p_buttonGroup->addButton(button, index);
    m_winMap[index] = win;
}

void ButtonWinMannager::slotbuttonClicked(int id)
{
    if(m_winMap.contains(id))
    {
        MainWindow::instance()->slotSetWidget(m_winMap.value(id));
    }
}

