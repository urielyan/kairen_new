#ifndef ISPREHEAT_H
#define ISPREHEAT_H

#include <QHBoxLayout>
#include <QWidget>
#include <QLabel>
#include <QDateTime>
#include <QTimer>

#define NEED_START_SAMPLING 9999
class StatusBar : public QWidget
{
    Q_OBJECT
    Q_ENUMS(PlatePosition)
public:
    enum PlatePosition{
        Tested = 0,
        Referencce,
        Malfunction,
        UnKnow
    };
    static StatusBar* instance();
    ~StatusBar();
    static int is_sampling_num;


public slots:
    void slotUpdatePosition(PlatePosition position);
    void slotUpdateDateTime();
    void slotUpdateSampling();
    void slotUpdatepreheat();

private:
    explicit StatusBar(QWidget *parent = 0);

    bool blink_flag;

    QTimer *timerIsPreheat;

    QLabel m_labelIsPreheatSampling;
    QLabel m_labelDateTime;
    QLabel m_plateLabel;

    void initPlatePosition();
    void initPreheat();
    void initDatetime();
};

class DateTimeWidget : public QLabel
{
    Q_OBJECT
public:
    explicit DateTimeWidget(QWidget *parent = 0);

private slots:
    void slotUpdateDateTime()
    {
        setText(QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss"));
    }
};

class PlatePosition : public QLabel
{
    Q_OBJECT
public:
    explicit PlatePosition(QWidget *parent = 0);

};
#endif // ISPREHEAT_H
