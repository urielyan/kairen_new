#ifndef WINABSTRACTFRAME_H
#define WINABSTRACTFRAME_H

#include <QFrame>
#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>

class QPushButton;
class AbstractComponentFactory;
class WinAbstractFrame : public QFrame
{
    Q_OBJECT

public:
    explicit WinAbstractFrame(QWidget *parent = 0);

public:
    void setTitle(QString title);
    void addWidget(QWidget *widget);
    void addLayout(QLayout *layout);

    QPushButton * getReturnButton();
    QPushButton * getInPlateButton();
    QPushButton * getOutPlateButton();

protected:
    AbstractComponentFactory *p_componentFactory;

protected slots:
    void slotReturnButtonClicked();
    void slotInPlatebuttonClicked();
    void slotOutPlatebuttonClicked();
private:
    QLabel *p_title;
    QVBoxLayout m_mainVBoxLayout;

    StatusBar::PlatePosition setPlatePosition(QByteArray recvData);
};

#endif // WINABSTRACTFRAME_H
