#include "abstractfactory.h"

#include "numberinput.h"

#include <QLineEdit>
#include <QBoxLayout>
#include <QPushButton>
#include <QString>
#include <QButtonGroup>

NumberInput::NumberInput(QWidget *parent)
    : QDialog(parent)
{
    p_componentFactory = new MainComponent(this);
    p_lineEdit = new QLineEdit(this);
    p_mainBoxLayout = p_componentFactory->getBoxLayout(QBoxLayout::TopToBottom, this);

    {
        QBoxLayout *p_lineEditBackspaceLayout= p_componentFactory->getBoxLayout(QBoxLayout::LeftToRight);
        p_lineEditBackspaceLayout->addWidget(p_lineEdit);

        QPushButton *p_backspaceButton = p_componentFactory->getButton(tr("Backspace"), this);
        connect(p_backspaceButton, SIGNAL(clicked(bool)), this, SLOT(slotBackspaceClicked()));
        p_lineEditBackspaceLayout->addWidget(p_backspaceButton);

        p_mainBoxLayout->addLayout(p_lineEditBackspaceLayout);
    }

    initKey();

}

QString NumberInput::getString()
{
    return p_lineEdit->text();
}

void NumberInput::slotOkClicked()
{
    this->accept();
}

void NumberInput::slotCancelClicked()
{
    this->reject();
}

void NumberInput::slotBackspaceClicked()
{
    QString lineEditStr = p_lineEdit->text();
    lineEditStr.chop(1);
    p_lineEdit->setText(lineEditStr);
}

void NumberInput::slotClearClicked()
{
    p_lineEdit->clear();
}

void NumberInput::slotNumberClicked(int index)
{
    p_lineEdit->setText(p_lineEdit->text() + QString::number(index));
}

void NumberInput::slotPointClicked()
{
    QString lineEditStr = p_lineEdit->text();
    if((lineEditStr.size() == 0)
            || (lineEditStr.contains(".")))
    {
        return;
    }
    p_lineEdit->setText(lineEditStr + ".");
}

void NumberInput::initKey()
{
    QButtonGroup *p_buttonGroup = new QButtonGroup(this);
    connect(p_buttonGroup, SIGNAL(buttonClicked(int)), this, SLOT(slotNumberClicked(int)));

    QBoxLayout *p_keyLayout = p_componentFactory->getBoxLayout(QBoxLayout::LeftToRight);
    p_mainBoxLayout->addLayout(p_keyLayout);

    {
        //初始化按钮:0-9, "."
        QBoxLayout *p_numberPointLayout = p_componentFactory->getBoxLayout(QBoxLayout::TopToBottom);
        p_keyLayout->addLayout(p_numberPointLayout);

        for (int i = 0; i < 3; i++)
        {
            QBoxLayout *p_numberLayout = p_componentFactory->getBoxLayout(QBoxLayout::LeftToRight);
            for (int j = 0; j < 3; j++)
            {
                QPushButton *p_button = p_componentFactory->getButton(QString::number(7 - 3*i + j), this);
                p_numberLayout->addWidget(p_button);
                p_buttonGroup->addButton(p_button, 7 - 3*i + j);
            }
            p_numberPointLayout->addLayout(p_numberLayout);
        }
        QBoxLayout *p_zeroPointLayout = p_componentFactory->getBoxLayout(QBoxLayout::LeftToRight);
        p_numberPointLayout->addLayout(p_zeroPointLayout);

        QPushButton *p_zero = p_componentFactory->getButton("0", this);
        p_buttonGroup->addButton(p_zero, 0);
        p_zeroPointLayout->addWidget(p_zero);

        QPushButton *p_point = p_componentFactory->getButton(".", this);
        connect(p_point, SIGNAL(clicked(bool)), this, SLOT(slotPointClicked()));
        p_zeroPointLayout->addWidget(p_point);

    }

    {
        QBoxLayout *p_editLineEditLayout = p_componentFactory->getBoxLayout(QBoxLayout::TopToBottom);
        p_keyLayout->addLayout(p_editLineEditLayout);

        QPushButton *p_BackButton = p_componentFactory->getButton(tr("Backspace"), this);
        p_editLineEditLayout->addWidget(p_BackButton);
        connect(p_BackButton, SIGNAL(clicked(bool)), this, SLOT(slotBackspaceClicked()));

        QPushButton *p_clearButton = p_componentFactory->getButton(tr("Clear"), this);
        p_editLineEditLayout->addWidget(p_clearButton);
        connect(p_clearButton, SIGNAL(clicked(bool)), this, SLOT(slotClearClicked()));

        QPushButton *p_okButton = p_componentFactory->getButton(tr("Ok"), this);
        p_editLineEditLayout->addWidget(p_okButton);
        connect(p_okButton, SIGNAL(clicked(bool)), this, SLOT(slotOkClicked()));

        QPushButton *p_cancelButton = p_componentFactory->getButton(tr("Cancel"), this);
        p_editLineEditLayout->addWidget(p_cancelButton);
        connect(p_cancelButton, SIGNAL(clicked(bool)), this, SLOT(slotCancelClicked()));
    }
}

