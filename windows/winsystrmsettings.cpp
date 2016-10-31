#include "common/numberinput.h"
#include "common/global.h"
#include "common/wininforlistdialog.h"
#include "common/abstractfactory.h"

#include "buttonwinmannager.h"
#include "winsystrmsettings.h"
#include "mainwindow.h"
#include "winspectrumeasure.h"
#include "wincountmeasure.h"
#include "wincalibratemeasure.h"

#include <QButtonGroup>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QDateTime>

WinSystrmSettings::WinSystrmSettings(QWidget *parent)
    :WinAbstractFrame(parent)
{
    setTitle(tr("System settings"));

    p_buttonWinMannager = new ButtonWinMannager(this);
    {
        QBoxLayout *p_hBoxLayout = p_componentFactory->getBoxLayout(QBoxLayout::LeftToRight);

        QPushButton *p_spectrumMeasureButton = p_componentFactory->getButton(tr("Spectrum measure"), this);
        p_hBoxLayout->addWidget(p_spectrumMeasureButton);

        QPushButton *p_countButton = p_componentFactory->getButton(tr("Count measure"), this);
        p_hBoxLayout->addWidget(p_countButton);

        QPushButton *p_calibrateButton = p_componentFactory->getButton(tr("Calibrate measure"), this);
        p_hBoxLayout->addWidget(p_calibrateButton);

        addLayout(p_hBoxLayout);

        p_buttonWinMannager->addButtonWindow(p_spectrumMeasureButton, new WinSpectruMeasure(this), Spectrum);
        p_buttonWinMannager->addButtonWindow(p_countButton, new WinCountMeasure(this), Count);
        p_buttonWinMannager->addButtonWindow(p_calibrateButton, new CalibrateMeasureMainWindow(this), Calibrate);
    }

    {
        QBoxLayout *p_hBoxLayout = p_componentFactory->getBoxLayout(QBoxLayout::LeftToRight);

        QPushButton *p_setDateTimeButton = p_componentFactory->getButton(tr("Set date time"), this);
        p_hBoxLayout->addWidget(p_setDateTimeButton);

        QPushButton *p_changePasswdButton = p_componentFactory->getButton(tr("Change passwd"), this);
        connect(p_changePasswdButton, SIGNAL(clicked(bool)), this, SLOT(slotChangePasswdClicked()));
        p_hBoxLayout->addWidget(p_changePasswdButton);

        p_hBoxLayout->addWidget(getReturnButton());

        addLayout(p_hBoxLayout);

        p_buttonWinMannager->addButtonWindow(p_setDateTimeButton, new WinSetDateTime(this), SetDateTime);
    }
}

void WinSystrmSettings::slotChangePasswdClicked()
{
    NumberInput input(this);
    if(input.exec() == QDialog::Accepted)
    {
        QString inputString = input.getString();
        if(inputString.size() != 6)
        {
            WinInforListDialog::instance()->showMsg(tr("input err"));
            slotChangePasswdClicked();
        }
        //TODO: change passwd
    }
}

WinSetDateTime::WinSetDateTime(QWidget *parent) :
    WinAbstractFrame(parent)
{
    setTitle(tr("Set Date time"));
    QHBoxLayout *p_HLayoutSetDateTime = new QHBoxLayout;

    QList<QLabel *> m_labelListUnit;

    for(int i = 0; i < 5; i++)
    {
        QPushButton *p_upButton =  p_componentFactory->getButton(tr("Start"), this);
        p_upButton->setObjectName("upbutton");
        m_buttonGroupUp.addButton(p_upButton, i);
        p_upButton->setText("+");


        QFrame *p_middleFrame = new QFrame(this);
        QBoxLayout *middleFrameLayout=  p_componentFactory->getBoxLayout(QBoxLayout::LeftToRight, p_middleFrame);
        QLabel *p_Label = new QLabel(p_middleFrame);
        p_Label->setFont(QFont(FONT_NAME, FONT_SIZE));
        m_labelList.append(p_Label);
        p_Label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        QLabel *p_LabelUnit = new QLabel(p_middleFrame);
        m_labelListUnit.append(p_LabelUnit);
        p_LabelUnit->setFont(QFont(FONT_NAME, FONT_SIZE - 10));
        p_LabelUnit->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        middleFrameLayout->addWidget(p_Label);
        middleFrameLayout->addWidget(p_LabelUnit);

//        QBoxLayout *p_HBoxLabel =  p_componentFactory->getBoxLayout(QBoxLayout::LeftToRight);
//        p_HBoxLabel->addWidget(p_Label);
//        p_HBoxLabel->addWidget(p_LabelUnit);

        QPushButton *p_bottomButton = p_componentFactory->getButton(tr(""), this);
        m_buttonGroupDown.addButton(p_bottomButton, i);
        p_bottomButton->setObjectName("bottombutton");
        p_bottomButton->setText("-");

        QBoxLayout *p_VLayout = p_componentFactory->getBoxLayout(QBoxLayout::TopToBottom);
        p_VLayout->addWidget(p_upButton);
        p_VLayout->addWidget(p_middleFrame);
        p_VLayout->addWidget(p_bottomButton);

        p_HLayoutSetDateTime->addLayout(p_VLayout);

    }
    m_labelListUnit[YEAR]->setText(tr("年"));
    m_labelListUnit[MONTH]->setText(tr("月"));
    m_labelListUnit[DAY]->setText(tr("日"));
    m_labelListUnit[HOUR]->setText(tr("时"));
    m_labelListUnit[MINUTE]->setText(tr("分"));

    connect(&m_buttonGroupUp, SIGNAL(buttonClicked(int)), this, SLOT(slotButtonUpClicked(int)));
    connect(&m_buttonGroupDown, SIGNAL(buttonClicked(int)), this, SLOT(slotButtonDownClicked(int)));

    QBoxLayout *p_HLayoutCancelSave =p_componentFactory->getBoxLayout(QBoxLayout::LeftToRight);
    QPushButton *p_SaveButton = p_componentFactory->getButton(tr("Save"), this);
    connect(p_SaveButton, SIGNAL(clicked(bool)), this, SLOT(slotSaveButtonClicked()));
    p_HLayoutCancelSave->addWidget(p_SaveButton);
    p_HLayoutCancelSave->addWidget(getReturnButton());

    addLayout(p_HLayoutSetDateTime);
    addLayout(p_HLayoutCancelSave);

    setLabelCurrentDateTime();
}

void WinSetDateTime::slotSaveButtonClicked()
{
    QMessageBox box;
    box.setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
    box.setText(tr("需要保存此次修改的日期和时间么？\n") + getSetDateTime());
    box.setStandardButtons(QMessageBox::Cancel|QMessageBox::Yes);
    int res = box.exec();
    if(res == QMessageBox::Yes){
        QString tmpstr("date -s ");
        tmpstr += " \"";
        tmpstr.append(getSetDateTime());
        tmpstr += "\"";
        int a = system(tmpstr.toLocal8Bit());
        int b = system("hwclock --systohc");
        if(a== -1 || b == -1){
            WinInforListDialog::instance()->showMsg(tr("设置时间不成功"));
        }else{
            WinInforListDialog::instance()->showMsg(tr("设置时间成功"));
        }
    }
}

void WinSetDateTime::showRefresh(){
    setLabelCurrentDateTime();
    examineDayLabelIsRight();
    this->showFullScreen();
}


void WinSetDateTime::slotButtonUpClicked(int id)
{
    int m_labelNumber = m_labelList[id]->text().toInt() + 1;
    switch (id) {
    case YEAR:
    {
        if(m_labelNumber > 2020)
            m_labelNumber = 2010;
        break;
    }
    case MONTH:
    {
        if(m_labelNumber > 12)
            m_labelNumber = 1;
        break;
    }
    case DAY:
    {
        int monthAry[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        int year = m_labelList[YEAR]->text().toInt();
        int month = m_labelList[MONTH]->text().toInt() - 1;
        if((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
        {
            monthAry[1] += 1;
        }

        if(m_labelNumber > monthAry[month])
            m_labelNumber = 1;
        break;
    }
    case HOUR:
    {
        if(m_labelNumber > 23)
            m_labelNumber = 0;
        break;
    }
    case MINUTE:
    {
        if(m_labelNumber > 59)
            m_labelNumber = 0;
        break;
    }

    default:
        break;
    }
    m_labelList[id]->setText(QString::number(m_labelNumber));
    examineDayLabelIsRight();
}

void WinSetDateTime::slotButtonDownClicked(int id)
{
    int m_labelNumber = m_labelList[id]->text().toInt() - 1;
    switch (id) {
    case YEAR:
    {
        if(m_labelNumber  < 2010)
            m_labelNumber = 2020;
        break;
    }
    case MONTH:
    {
        if(m_labelNumber < 1)
            m_labelNumber = 12;
        break;
    }
    case DAY:
    {
        int monthAry[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        int year = m_labelList[YEAR]->text().toInt();
        int month = m_labelList[MONTH]->text().toInt() - 1;
        if((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
        {
            monthAry[1] += 1;
        }

        if(m_labelNumber < 1)
            m_labelNumber = monthAry[month];
        break;
    }
    case HOUR:
    {
        if(m_labelNumber < 0)
            m_labelNumber = 23;
        break;
    }
    case MINUTE:
    {
        if(m_labelNumber < 0)
            m_labelNumber = 59;
        break;
    }

    default:
        break;
    }
    m_labelList[id]->setText(QString::number(m_labelNumber));
    examineDayLabelIsRight();
}

void WinSetDateTime::setLabelCurrentDateTime()
{
    m_labelList[YEAR]->setText(QString::number(QDate::currentDate().year()));
    m_labelList[MONTH]->setText(QString::number(QDate::currentDate().month()));
    m_labelList[DAY]->setText(QString::number(QDate::currentDate().day()));
    m_labelList[HOUR]->setText(QString::number(QTime::currentTime().hour()));
    m_labelList[MINUTE]->setText(QString::number(QTime::currentTime().minute()));
}

void WinSetDateTime::examineDayLabelIsRight()
{
    if(m_labelList[DAY]->text().toInt() >= 29)
    {
        int year = m_labelList[YEAR]->text().toInt();
        int month = m_labelList[MONTH]->text().toInt();

        //不是闰年：二月最大的数是28
        if((2 ==  month)&&
                !((year % 4 == 0 && year % 100 != 0) || year % 400 == 0))
        {
            m_labelList[DAY]->setText("28");
        }
    }
}

QString WinSetDateTime::getSetDateTime()
{
    return QString("%1-%2-%3 %4:%5:00").arg(m_labelList[YEAR]->text()).arg(m_labelList[MONTH]->text())
            .arg(m_labelList[DAY]->text()).arg(m_labelList[HOUR]->text()).arg(m_labelList[MINUTE]->text());
}
