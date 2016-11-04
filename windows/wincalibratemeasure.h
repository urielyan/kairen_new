#ifndef WINCALIBRATEMEASURE_H
#define WINCALIBRATEMEASURE_H

#include "statusbar.h"
#include "winabstractframe.h"
#include <QTimer>
#include <QLabel>
#include <QPushButton>
#include <QTableView>
#include <QSqlTableModel>

class ButtonWinMannager;

class InputSulfurContent : public WinAbstractFrame
{
    Q_OBJECT
public:
    explicit InputSulfurContent(QWidget *parent = 0);

private slots:
    void updateButtonClicked();
    void viewClicked(const QModelIndex &index);
private:
    QTableView m_view;
    QSqlTableModel m_model;
};

class CalibrateMeasure : public WinAbstractFrame
{
    Q_OBJECT
public:
    explicit CalibrateMeasure(QWidget *parent = 0);
    void init();

private slots:
    void startButtonClicked();
    void stopButtonClicked();
    void readComData();

private:
    QTimer m_timer;

    QPushButton *p_startButton;
    QLabel m_platePositionLabel;//滑板位置
    QLabel m_remainingTimeLabel;//当前测量所剩下的时间
    QLabel m_testedCountDataLabel;//待测样计数值
    QLabel m_referenceCountDataLabel;//参考样计数值

private:
    void setPlatePositionText(StatusBar::PlatePosition platePosition);
    void setTestedCountDataText(int countData);
    void setReferenceCountDataText(int countData);

    StatusBar::PlatePosition readPlatePositon(StatusBar::PlatePosition);
    void readCountData();
    void storeCalibrateData();

    void initMeasureLabel();
    void initbutton();
};

/**
 * @class CalibrateMeasureMainWindow
 * @brief 标定测量主界面，
 *  包含标定相关的所有按钮,清除标定数据.
 */
class CalibrateMeasureMainWindow: public WinAbstractFrame
{
    Q_OBJECT
public:
    enum WinId{
        CalibrateMeasureID = 0
        ,InputSulfurContentID
        ,CountKbValueID
    };
    explicit CalibrateMeasureMainWindow(QWidget *parent = 0);

private slots:
    void clearCalibrateDataClicked();
    void countKbValueClicked();
private:
    ButtonWinMannager *p_buttonWinMannager;
};
#endif // WINCALIBRATEMEASURE_H
