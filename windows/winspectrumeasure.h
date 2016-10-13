#ifndef SPECTRUMEASUREMENT_H
#define SPECTRUMEASUREMENT_H

#include "winabstractframe.h"

class QTableWidget;
class WinViewSummit : public WinAbstractFrame
{
public:
    explicit WinViewSummit(QTableWidget *tableWidget, QWidget *parent = 0);

private:
};

class WinSpectruMeasure : public WinAbstractFrame
{
    Q_OBJECT

public:
    explicit WinSpectruMeasure(QWidget *parent = 0);

private slots:
    void slotStartButtonClicked();
    void slotStopbuttonClicked();
    void slotViewSummitButtonClicked();

private:
    QTableWidget *p_tableWidget;
    void initTableWidget();
};

#endif // SPECTRUMEASUREMENT_H
