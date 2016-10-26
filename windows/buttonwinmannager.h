#ifndef BUTTONWINMANNAGER_H
#define BUTTONWINMANNAGER_H

#include <QObject>
#include <QAbstractButton>
#include <QMap>

class WinAbstractFrame;
class QButtonGroup;
class ButtonWinMannager : public QObject
{
    Q_OBJECT
public:
    explicit ButtonWinMannager(QObject *parent = 0);
    void addButtonWindow(QAbstractButton *button, WinAbstractFrame *win,
                      int index = 0);
    QAbstractButton *getButton(int index);
    WinAbstractFrame *getWindow(int index);
signals:

private slots:
    void slotbuttonClicked(int id);

private:
    QButtonGroup *p_buttonGroup;
    QMap <int, WinAbstractFrame*> m_winMap;
};

#endif // BUTTONWINMANNAGER_H
