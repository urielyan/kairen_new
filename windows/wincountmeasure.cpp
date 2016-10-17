#include "QTableWidget"

#include "wincountmeasure.h"
#include "common/abstractfactory.h"
#include "mainwindow.h"

WinCountMeasure::WinCountMeasure(QWidget *parent)
    : WinAbstractFrame(parent)
{
    setTitle(tr("count measure"));
    p_componentFactory = new MeasureFrameComponent(this);

    initMeasureLabel();
    initbutton();
}

void WinCountMeasure::slotStartButtonClicked()
{

}

void WinCountMeasure::slotStopbuttonClicked()
{

}

void WinCountMeasure::slotQueryButtonClicked()
{
    MainWindow::instance()->slotSetWidget(&m_queryCountMeasure);
}

void WinCountMeasure::initChangeLabel()
{
    m_currentTimeLabel.setText(tr(""));
}

void WinCountMeasure::initMeasureLabel()
{
    QBoxLayout *p_layout = p_componentFactory->getBoxLayout(QBoxLayout::LeftToRight);

    QLabel *p_informationLabel = p_componentFactory->getLabel(tr("A total of 11 times, 31 seconds every time."));
    p_layout->addWidget(p_informationLabel);
    p_layout->addWidget(&m_currentTimeLabel);

    addLayout(p_layout);
    addWidget(&m_remainingTimelabel);
}

void WinCountMeasure::initbutton()
{
    QBoxLayout *p_buttonLayout = p_componentFactory->getBoxLayout(QBoxLayout::LeftToRight);
    addLayout(p_buttonLayout);

    {
        QPushButton *p_startButton = p_componentFactory->getButton(tr("Start"), this);
        connect(p_startButton, SIGNAL(clicked(bool)), this, SLOT(slotStartButtonClicked()));
        p_buttonLayout->addWidget(p_startButton);

        QPushButton *p_stopButton = p_componentFactory->getButton(tr("Stop"), this);
        connect(p_stopButton, SIGNAL(clicked(bool)), this, SLOT(slotStopbuttonClicked()));
        p_buttonLayout->addWidget(p_stopButton);

        QPushButton *p_queryButton = p_componentFactory->getButton(tr("Query"), this);
        connect(p_queryButton, SIGNAL(clicked(bool)), this, SLOT(slotQueryButtonClicked()));
        p_buttonLayout->addWidget(p_queryButton);

        p_buttonLayout->addWidget(getInPlateButton());
        p_buttonLayout->addWidget(getOutPlateButton());
        p_buttonLayout->addWidget(getReturnButton());
    }
}


QueryCountMeasure::QueryCountMeasure(QWidget *parent)
    : WinAbstractFrame(parent)
{
    setTitle(tr("Query count measure data"));
    p_mainBoxlayout = p_componentFactory->getBoxLayout(QBoxLayout::LeftToRight);
    addLayout(p_mainBoxlayout);

    initTableWidget();
    initLabel();
}

void QueryCountMeasure::setLabelText(int average, int lambda)
{
    m_average.setText(tr("Count average = ").append(QString::number(average)));
    m_lambda.setText(tr("lambda = ").append(QString::number(lambda)));
}

void QueryCountMeasure::setLabelText(QString average, QString lambda)
{
    m_average.setText(tr("Count average = ").append(average));
    m_lambda.setText(tr("λ = ").append(lambda));
}

void QueryCountMeasure::initTableWidget()
{
    p_tableWidget = new QTableWidget(11, 1, this);
//    p_tableWidget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
//    p_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
//    p_tableWidget->horizontalHeader()->setStretchLastSection(true);
//    p_tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//设置垂直滚动条显示模式
//    p_tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//设置水平滚动条显示模式
    p_mainBoxlayout->addWidget(p_tableWidget);
}

void QueryCountMeasure::initLabel()
{
    QBoxLayout *p_boxLayout = p_componentFactory->getBoxLayout(QBoxLayout::TopToBottom);

    QLabel *p_ns = new QLabel(tr("Ns:"), this);
    p_boxLayout->addWidget(p_ns);
    p_boxLayout->addWidget(&m_average);
    p_boxLayout->addWidget(&m_lambda);
    setLabelText("", "");
    p_boxLayout->addWidget(getReturnButton());

    p_mainBoxlayout->addLayout(p_boxLayout);
}
