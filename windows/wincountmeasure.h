#ifndef WINCOUNTMEASURE_H
#define WINCOUNTMEASURE_H

#include "winabstractframe.h"
#include <QLabel>
#include <QTimer>
#include <QMap>
#include <QButtonGroup>

class QTableWidget;
class QueryCountMeasure : public WinAbstractFrame
{
    Q_OBJECT

public:
    explicit QueryCountMeasure(QWidget *parent = 0);
    void setTableWidgetData(const QList<uint> &list);
    void setLabelText(int average, double lambda);
    void setLabelText(QString average, QString lambda);

private slots:

private:
    QBoxLayout *p_mainBoxlayout;

    QTableWidget *p_tableWidget;
    QLabel m_average;
    QLabel m_lambda;

    void initTableWidget();
    void initLabel();

    void saveData(int average, double lambda);
};

class WinCountMeasure : public WinAbstractFrame
{
    Q_OBJECT

public:
    enum ButtonId{
        Start = 0
        ,Stop
        ,Query
        ,InPlate
        ,OutPlate
        ,Return
    };
    explicit WinCountMeasure(QWidget *parent = 0);
    void init();

private slots:
    void slotStartButtonClicked();
    void slotStopbuttonClicked();
    void slotQueryButtonClicked();
    void slotReadComData();

private:
    QTimer m_timer;
    QueryCountMeasure m_queryCountMeasure;

    QMap <int, QString> m_timeMap;
    QLabel m_currentTimeLabel;
    QLabel m_remainingTimelabel;
    void setChangeLabel(uint currentTime = 1, uint remainingTime = 31);
    void addChangeLabel();
    void clearChangeLabel();

    QButtonGroup m_buttonGroup;
    void setButtonEnabled(bool value);

    QList <uint> m_countData;
    void readComData();

    void initMeasureLabel();
    void initbutton();

};

#endif // WINCOUNTMEASURE_H
