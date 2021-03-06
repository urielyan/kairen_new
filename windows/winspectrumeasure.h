#ifndef SPECTRUMEASUREMENT_H
#define SPECTRUMEASUREMENT_H

#include "winabstractframe.h"
#include <QTimer>

class QTableWidget;
class WinViewSummit : public WinAbstractFrame
{
public:
    explicit WinViewSummit(QTableWidget *tableWidget, QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *);

private:
    QWidget paintFrame;
    QTableWidget *p_tableWidget;

    void drawCoordinate(QPainter &painter, int rectWidth, int valueMultiplle);
    void drawRect(QPainter &painter, int rectWidth, int valueMultiplle);
};

class WinSpectruMeasure : public WinAbstractFrame
{
    Q_OBJECT

public:
    explicit WinSpectruMeasure(QWidget *parent = 0);
    void init();

private slots:
    void slotStartButtonClicked();
    void slotStopbuttonClicked();
    void slotViewSummitButtonClicked();
    void slotReadComData();

private:
    QTimer m_timer;

    QTableWidget *p_tableWidget;
    void initTableWidget();
};

#endif // SPECTRUMEASUREMENT_H
