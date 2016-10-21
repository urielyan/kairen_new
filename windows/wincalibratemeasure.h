#ifndef WINCALIBRATEMEASURE_H
#define WINCALIBRATEMEASURE_H

#include "winabstractframe.h"
#include <QTimer>
#include <QLabel>
#include <QPushButton>

class CalibrateMeasure : public WinAbstractFrame
{
    Q_OBJECT
public:
    explicit CalibrateMeasure(QWidget *parent = 0);
    void init();

private slots:
    void slotStartButtonClicked();
    void slotStopButtonClicked();
    void slotQueryButtonClicked();
    void slotReadComData();

private:
    QTimer m_timer;

    QPushButton *p_startButton;
    QLabel m_platePosition;
    QLabel m_remainingTime;
    QLabel m_testedCountData;
    QLabel m_referenceCountData;

    void setPlatePositionText(int platePosition);
    void addRemainingTimeText();
    void setTestedCountDataText(int countData);
    void setReferenceCountDataText(int countData);

    void initMeasureLabel();
    void initbutton();
};

class ButtonWinMannager;
class WinCalibrateMeasure: public WinAbstractFrame
{
    Q_OBJECT
public:
    enum WinId{
        CalibrateMeasureID = 0
        ,InputS
        ,CountKbValue
    };
    explicit WinCalibrateMeasure(QWidget *parent = 0);

private slots:
    void clearCalibrateDataClicked();
private:
    ButtonWinMannager *p_buttonWinMannager;
};
#endif // WINCALIBRATEMEASURE_H
