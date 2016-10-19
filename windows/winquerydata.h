#ifndef QUERYDATA_H
#define QUERYDATA_H

#include "winabstractframe.h"

class ButtonWinMannager;
class QTableWidget;

class WinQueryCountData : public WinAbstractFrame
{
    Q_OBJECT

public:
    explicit WinQueryCountData(QWidget *parent = 0);

private slots:
    void slotUpdateButtonClicked();
    void slotNetxButtonClicked();
    void slotPreviousButtonClicked();

private:
    QTableWidget *p_tableWidget;
    void initTableWidget();
    void setTableWidget(uint id);
};

class WinQueryData : public WinAbstractFrame
{
    Q_OBJECT

public:
    enum WinId{
        Sample = 0
        ,Count
        ,Calibrate
    };
    explicit WinQueryData(QWidget *parent = 0);

private slots:

private:
    ButtonWinMannager *p_buttonWinMannager;

};

#endif // QUERYDATA_H
