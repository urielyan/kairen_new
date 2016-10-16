#ifndef WINMAINFRAME_H
#define WINMAINFRAME_H

#include "winabstractframe.h"

class ButtonWinMannager;
class WinMainFrame : public WinAbstractFrame
{
    Q_OBJECT

public:
    enum WinId{
        SampleMeasure = 0,
        SystemSettings,
        DataQuery
    };
    WinMainFrame(QWidget *parent = 0);
    ~WinMainFrame(){}

private:
    ButtonWinMannager *p_buttonWinMannager;
};

#endif // WINMAINFRAME_H
