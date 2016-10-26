#include <QButtonGroup>

#include "buttonwinmannager.h"
#include "windows/mainwindow.h"
#include "windows/winabstractframe.h"
ButtonWinMannager::ButtonWinMannager(QObject *parent) : QObject(parent)
{
    p_buttonGroup = new QButtonGroup(this);
    connect(p_buttonGroup, SIGNAL(buttonClicked(int)), this, SLOT(slotbuttonClicked(int)));
}

QAbstractButton *ButtonWinMannager::getButton(int index)
{
    return p_buttonGroup->button(index);
}

WinAbstractFrame *ButtonWinMannager::getWindow(int index)
{
    if(m_winMap.contains(index))
    {
        return m_winMap[index];
    }

    return NULL;
}

void ButtonWinMannager::addButtonWindow(QAbstractButton *button, WinAbstractFrame *win, int index)
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

