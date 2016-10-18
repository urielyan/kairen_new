#ifndef QUERYDATA_H
#define QUERYDATA_H

#include "winabstractframe.h"

class ButtonWinMannager;

class WinQueryCountData : public WinAbstractFrame
{
    Q_OBJECT

public:
    explicit WinQueryCountData(QWidget *parent = 0);

private:

};

class QueryData : public WinAbstractFrame
{
    Q_OBJECT

public:
    enum WinId{
        Sample = 0
        ,Count
        ,Calibrate
    };
    explicit QueryData(QWidget *parent = 0);

private slots:

private:
    ButtonWinMannager *p_buttonWinMannager;

};

#endif // QUERYDATA_H
