#ifndef WINCOUNTMEASURE_H
#define WINCOUNTMEASURE_H

#include "winabstractframe.h"
#include <QLabel>

class QTableWidget;
class QueryCountMeasure : public WinAbstractFrame
{
    Q_OBJECT

public:
    explicit QueryCountMeasure(QWidget *parent = 0);
    void setLabelText(int average, int lambda);
    void setLabelText(QString average, QString lambda);

private slots:

private:
    QBoxLayout *p_mainBoxlayout;

    QTableWidget *p_tableWidget;
    QLabel m_average;
    QLabel m_lambda;

    void initTableWidget();
    void initLabel();
};

class WinCountMeasure : public WinAbstractFrame
{
    Q_OBJECT

public:
    explicit WinCountMeasure(QWidget *parent = 0);

private slots:
    void slotStartButtonClicked();
    void slotStopbuttonClicked();
    void slotQueryButtonClicked();

private:
    QLabel m_currentTimeLabel;
    QLabel m_remainingTimelabel;

    QueryCountMeasure m_queryCountMeasure;

    void initChangeLabel();

    void initMeasureLabel();
    void initbutton();
};

#endif // WINCOUNTMEASURE_H
