#ifndef WINSYSTRMSETTINGS_H
#define WINSYSTRMSETTINGS_H

#include "winabstractframe.h"
#include <QButtonGroup>
class ButtonWinMannager;



class WinSetDateTime : public WinAbstractFrame
{
    Q_OBJECT

public:
    enum ENUM_DATETIME
    {
        YEAR = 0,
        MONTH,
        DAY,
        HOUR,
        MINUTE,
    };
    explicit WinSetDateTime(QWidget *parent = 0);
    void showRefresh();

private slots:
    void slotSaveButtonClicked();

    void slotButtonUpClicked(int id);
    void slotButtonDownClicked(int id);

private:
    QButtonGroup m_buttonGroupUp, m_buttonGroupDown;
    QList<QLabel*> m_labelList;

    void setLabelCurrentDateTime();
    void examineDayLabelIsRight();
    QString getSetDateTime();
};

class WinSystrmSettings : public WinAbstractFrame
{
    Q_OBJECT

public:
    enum WinId{
        Spectrum = 0,
        Count,
        Calibrate,
        SetDateTime,
        ChangePasswd
    };
    WinSystrmSettings(QWidget *parent = 0);

private slots:
    void slotChangePasswdClicked();

private:
    ButtonWinMannager *p_buttonWinMannager;
};

#endif // WINSYSTRMSETTINGS_H
