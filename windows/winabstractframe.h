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

    /**
     * 当进入离开本界面时需要完成的操作，根据每个界面实现不同。
     */
    virtual void onEntry();
    virtual void onExit();

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

private:
    QLabel *p_title;
    QVBoxLayout m_mainVBoxLayout;

};

#endif // WINABSTRACTFRAME_H
