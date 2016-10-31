#include "global.h"

#include "wininforlistdialog.h"

#include <QtWidgets/QMessageBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QStackedLayout>

WinInforListDialog *WinInforListDialog::instance()
{
  static WinInforListDialog Instance;
  return &Instance;
}

void WinInforListDialog::showMsg(QString text, QString informativeText)
{
  QMessageBox msgbox;
  msgbox.setFont(QFont(FONT_NAME, FONT_SIZE ,QFont::Normal));
  msgbox.setText(text);
  msgbox.setInformativeText(informativeText);
  msgbox.exec();
}

WinInforListDialog::WinInforListDialog(QWidget *parent) : QDialog(parent)
{
  this->setFixedHeight(DESKTOP_HEIGHT - 30);
  this->setFixedWidth(DESKTOP_WIDTH- 30);
  setStylesheet();
}

void WinInforListDialog::setStylesheet()
{
  //this->setStyleSheet("QMessageBox{min-height: 50}");
}


WinSureOperateDialog::WinSureOperateDialog(QString text, QWidget *parent)
    : QDialog(parent)
{
    init();

    p_labelTitle->setText(tr("Confirmation"));
    p_labelContent->setText(text);

    QPushButton *okBtn = new QPushButton(this);
    okBtn->setObjectName("OkButton");
    okBtn->setFocusPolicy(Qt::NoFocus);
    okBtn->setText(tr("OK"));
    okBtn->setFixedSize(124, 52);
    connect(okBtn, SIGNAL(clicked()), this, SLOT(accept()));

    QPushButton *cancelBtn = new QPushButton(this);
    cancelBtn->setObjectName("CancelButton");
    cancelBtn->setFocusPolicy(Qt::NoFocus);
    cancelBtn->setText(tr("Cancel"));
    cancelBtn->setFixedSize(124, 52);
    connect(cancelBtn, SIGNAL(clicked()), this, SLOT(reject()));


    p_VLayoutBottom->addWidget(okBtn, 0, Qt::AlignVCenter);
    p_VLayoutBottom->addStretch();
    p_VLayoutBottom->addWidget(cancelBtn, 0, Qt::AlignVCenter);
}

void WinSureOperateDialog::init()
{
    QFrame *p_mainFrame = new QFrame(this);
    p_mainFrame->setObjectName("BackgrounFrame");
    p_mainFrame->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    QStackedLayout *layout = new QStackedLayout(this);
    layout->addWidget(p_mainFrame);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);


    p_labelContent = new QLabel(p_mainFrame);
    p_labelContent->setObjectName("TextLabel");

    p_labelTitle = new QLabel(p_mainFrame);
    p_labelTitle->setObjectName("TitleLabel");

    p_VLayoutBottom = new QHBoxLayout;
    p_VLayoutBottom->setContentsMargins(24, 0, 24, 0);
    p_VLayoutBottom->setSpacing(0);


    QPushButton *closeBtn = new QPushButton(p_mainFrame);
    closeBtn->setObjectName("CloseButton");
    closeBtn->setFocusPolicy(Qt::NoFocus);
    connect(closeBtn, SIGNAL(clicked()), this, SLOT(close()));
    connect(closeBtn, SIGNAL(clicked()), this, SLOT(reject()));

    QHBoxLayout * titleLayout = new QHBoxLayout();
    titleLayout->setContentsMargins(12, 0, 12, 0);
    titleLayout->setSpacing(0);
    titleLayout->addWidget(p_labelTitle, 0, Qt::AlignVCenter);
    titleLayout->addSpacerItem(
                new QSpacerItem(
                    40, 59,
                    QSizePolicy::Expanding, QSizePolicy::Fixed)
                );
    titleLayout->addWidget(closeBtn, 0, Qt::AlignVCenter);


    QVBoxLayout * contextLayout = new QVBoxLayout(p_mainFrame);
    contextLayout->setContentsMargins(0, 0, 0, 20);
    contextLayout->setSpacing(0);
    contextLayout->addLayout(titleLayout);
    contextLayout->addWidget(p_labelContent, 0, Qt::AlignLeft);
    contextLayout->addLayout(p_VLayoutBottom);

    p_mainFrame->setStyleSheet("QFrame#BackgrounFrame{\
                        min-width:722px;\
                        max-width:722px;\
                        border: 1px solid #D3D3D3;\
                        background-color:rgb(255, 255, 255);\
                        background-image:url(:/relay/Image/relay/title_bg.png);\
                        background-repeat:repeat-x;\
                        background-position: top left;\
                        background-origin: content;\
                        border-top-left-radius:5px;\
                        border-top-right-radius:5px;\
                        }\
                        QPushButton#CloseButton{\
                        min-width:34px;\
                        max-width:34px;\
                        width:34px;\
                        height:34px;\
                        background-image:url(:/relay/Image/relay/close_normal.png);\
                        background-repeat: no-repeat;\
                        background-position:center;\
                        border:none;\
                        }\
                        QPushButton#CloseButton:pressed{\
                        background-image:url(:/relay/Image/relay/close_pressed.png);\
                        }\
                        QLabel#TitleLabel{\
                        color:rgb(255, 255, 255);\
                        font-size:32px;\
                        }\
                        QLabel#TextLabel{"
                            "text-align:left;"
                        "color:black;\
                        margin-top:30px;\
                        margin-bottom:30px;\
                        margin-left:20px;\
                        font-size:28px;\
                        }\
                        QPushButton{"
                        "font-size:24px;\
                        color:black;\
                        border: 1px solid #D3D3D3;\
                        background-color:none;\
                        }\
                        QPushButton:pressed{"
                                            "background-color:rgb(0, 0, 0, 20%);"
                                           "}");
}
