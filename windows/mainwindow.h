#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMap>
#include <QFrame>
#include <QStackedLayout>

class WinAbstractFrame;
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
    void closeTopWidget();

private:
    MainWindow(QWidget *parent = 0);

private:
    QStackedLayout m_stackedLayout;
    QMap<WinId, WinAbstractFrame*> m_winMap;
};

#endif // MAINWINDOW_H
