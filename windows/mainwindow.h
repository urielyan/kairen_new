#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMap>
#include <QFrame>
#include <QStackedLayout>
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


class MainWindow : public QFrame
{
    Q_OBJECT

public:
    enum WinId{
        WinMain = 0,
        WinSampleMeasure,

    };
    static MainWindow* instance();
    ~MainWindow();
    void addWindget(WinId id, WinAbstractFrame* frame);

public slots:
    void slotSetWidget(QWidget *widget);
    void slotCloseTopWidget();

private:
    MainWindow(QWidget *parent = 0);

private:
    QStackedLayout m_stackedLayout;
    QMap<WinId, WinAbstractFrame*> m_winMap;
};

#endif // MAINWINDOW_H
