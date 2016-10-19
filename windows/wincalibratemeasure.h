#ifndef WINCALIBRATEMEASURE_H
#define WINCALIBRATEMEASURE_H

#include "winabstractframe.h"

class CalibrateMeasure : public WinAbstractFrame
{
    Q_OBJECT
public:
    enum ButtonId{
        Start = 0
        ,Stop
        ,Query
        ,Return
    };
    explicit CalibrateMeasure(QWidget *parent = 0);
    void init();

private slots:
    void slotStartButtonClicked();
    void slotStopButtonClicked();
    void slotQueryButtonClicked();
    void slotReadComData();

private:
    QTimer m_timer;


    void initMeasureLabel();
    void initbutton();
};

class WinCalibrateMeasure: public WinAbstractFrame
{
    Q_OBJECT
public:
    explicit WinCalibrateMeasure(QWidget *parent = 0);

private:
};
#endif // WINCALIBRATEMEASURE_H
