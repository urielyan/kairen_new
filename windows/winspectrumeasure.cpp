#include "common/abstractfactory.h"
#include "common/wininforlistdialog.h"
#include "common/datasave.h"
#include "communication/com.h"

#include "mainwindow.h"
#include "winspectrumeasure.h"

#include <QTableWidget>
#include <QHeaderView>
#include <QTableWidgetItem>
#include <QDebug>
#include <QByteArray>
#include <QPaintEvent>
#include <QPainter>

WinSpectruMeasure::WinSpectruMeasure(QWidget *parent)
    :WinAbstractFrame(parent)
{
    setTitle(tr("Spectrum measurement"));
    p_componentFactory = new MeasureFrameComponent(this);

    initTableWidget();

    QBoxLayout *p_buttonLayout = p_componentFactory->getBoxLayout(QBoxLayout::LeftToRight);
    addLayout(p_buttonLayout);

    {
        QPushButton *p_startButton = p_componentFactory->getButton(tr("Start"), this);
        connect(p_startButton, SIGNAL(clicked(bool)), this, SLOT(slotStartButtonClicked()));
        p_buttonLayout->addWidget(p_startButton);

        QPushButton *p_stopButton = p_componentFactory->getButton(tr("Stop"), this);
        connect(p_stopButton, SIGNAL(clicked(bool)), this, SLOT(slotStopbuttonClicked()));
        p_buttonLayout->addWidget(p_stopButton);

        QPushButton *p_viewSummitButton = p_componentFactory->getButton(tr("View summit"), this);
        connect(p_viewSummitButton, SIGNAL(clicked(bool)), this, SLOT(slotViewSummitButtonClicked()));
        p_buttonLayout->addWidget(p_viewSummitButton);

        p_buttonLayout->addWidget(getInPlateButton());
        p_buttonLayout->addWidget(getOutPlateButton());
        p_buttonLayout->addWidget(getReturnButton());
    }

    connect(&m_timer, SIGNAL(timeout()), this, SLOT(slotReadComData()));
}

void WinSpectruMeasure::init()
{
    m_timer.stop();
    p_tableWidget->clearContents();
}

void WinSpectruMeasure::slotStartButtonClicked()
{
    if(m_timer.isActive())
    {
        return;
    }
    init();

    Com::instance()->sendOrder(Com::SpectrueMeasure);
    m_timer.start(2000);
}

void WinSpectruMeasure::slotStopbuttonClicked()
{
    Com::instance()->sendOrder(Com::StopMeasure);
    m_timer.stop();
}

void WinSpectruMeasure::slotViewSummitButtonClicked()
{
    WinViewSummit *viewSummit = new WinViewSummit(p_tableWidget, this);
    MainWindow::instance()->slotSetWidget(viewSummit);
}

void WinSpectruMeasure::slotReadComData()
{
#ifdef TEST_COM
    static uint valueCount = 4;
    QByteArray data;
    data[0] = (char)0xfe;
    data[1] = (char)0x01;
    data[2] = (char)0x00 + (char)valueCount/10;
    data[3] = (char)0x00 + (char)valueCount%10;
    data[4] = (char)0x31;
    data[5] = (char)0x32;
    data[6] = (char)0x33;
    data[7] = data[2] + (char)0x30;
    data[8] = data[3] + (char)0x30;
    data[9] = (char)0xff;
    valueCount++;
    if(valueCount >= 44)
    {
        valueCount = 4;
    }
    Com::instance()->setRecvData(data);
#endif
    QByteArray recvData = Com::instance()->slotReadMyCom();
    if(recvData == NULL || recvData.size() != 10|| recvData[1] != (char)0x01)
    {
        WinInforListDialog::instance()->showMsg(tr("err") + recvData);
        ErrorCountSave::instance()->addCount(1);
        m_timer.stop();
        return;
    }

    uint which = (int)recvData[2] * 10 + (int)recvData[3] - 4;
    int column = which / 10 * 2;
    int  row = which % 10;
    QString value = QString::number((double)(which + 4)/10, 'f', 1);//以小树形式显示阈值,只显示一位小数点.
    QString count = recvData.mid(4, 5);
    p_tableWidget->setItem(row, column, new QTableWidgetItem(value));
    p_tableWidget->setItem(row, column + 1, new QTableWidgetItem(count));

    if(row == 9 && column == 6)
    {
        m_timer.stop();
    }
}

void WinSpectruMeasure::initTableWidget()
{
    p_tableWidget = new QTableWidget(10, 8, this);
    p_tableWidget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
//    p_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
//    p_tableWidget->horizontalHeader()->setStretchLastSection(true);
//    p_tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//设置垂直滚动条显示模式
//    p_tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//设置水平滚动条显示模式
    for(int i = 0; i < p_tableWidget->columnCount(); i++)
    {
        if(i % 2 == 0)
        {
            p_tableWidget->setHorizontalHeaderItem(i, new QTableWidgetItem(tr("Threshold")));
        }else
        {
            p_tableWidget->setHorizontalHeaderItem(i, new QTableWidgetItem(tr("count")));
        }
    }
    addWidget(p_tableWidget);
}

WinViewSummit::WinViewSummit(QTableWidget *tableWidget, QWidget *parent)
    : WinAbstractFrame(parent)
{
    Q_ASSERT(tableWidget != NULL);
    Q_ASSERT(tableWidget->rowCount() == 10);
    Q_ASSERT(tableWidget->columnCount() == 8);
    p_tableWidget = tableWidget;

    setTitle(tr("View spectrum summit"));
    p_componentFactory = new MeasureFrameComponent(this);

    addWidget(&paintFrame);
    paintFrame.setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    addWidget(getReturnButton());
}

void WinViewSummit::paintEvent(QPaintEvent */*event*/)
{
    int side = qMin(width(), height());                                           //创建窗口宽高参数
    QPainter painter(this);
    painter.setViewport(paintFrame.rect());
    //painter.setWindow(paintFrame.rect());
    painter.setRenderHint(QPainter::Antialiasing,true);                //开启抗锯齿
    painter.translate(50, height());                               //坐标系统平移变换，把原点平移到SUIT POSITION
    painter.scale(side / 300.0, side / 300.0);            //坐标系统比例变换，使绘制的图形随窗口的放大而放大
    //painter.scale(1, -1);                                          //Y轴向上翻转，翻转成正常平面直角坐标系

    painter.setPen(QPen(Qt::black, height() /1000));
    painter.setBrush(Qt::cyan);
    painter.setFont(QFont("Times", 10));

    int rectWidth = width() / 100;
    int valueMultiplle = 20;
    drawCoordinate(painter, rectWidth, valueMultiplle);
    drawRect(painter, rectWidth, valueMultiplle);
}

void WinViewSummit::drawCoordinate(QPainter &painter, int rectWidth, int valueMultiplle)
{
    painter.drawLine(0, 0, width(), 0);
    painter.drawLine(0, 0, 0, -height());

    for(int i = 0; i < 9; i++){
        // y axes;
        int yPoint = - i * height() / 12;
        painter.drawText(-30, yPoint  + 5, QString::number(- yPoint * valueMultiplle));//ｙ轴的倍数为五百当绘制柱状图时需要把得到的计数值除以５００
        painter.drawLine(0, yPoint, 5, yPoint);
      }

    for(int i = 0; i < 50; i+=5){
        // x axes;
        int xPoint = (double)i * rectWidth;
        painter.drawText(xPoint - 5, 30, QString::number((double)i/10, 'f', 1));
        painter.drawLine(xPoint, 0, xPoint, -5);
    }
}

void WinViewSummit::drawRect(QPainter &painter, int rectWidth, int valueMultiplle)
{
#ifdef TEST_COM
    for(int column = 0; column < p_tableWidget->columnCount(); column += 2)
    {
        for(int row = 0; row < p_tableWidget->rowCount(); row++)
        {
            p_tableWidget->setItem(row, column, new QTableWidgetItem(QString::number((double)(column * 10 / 2 + row + 4)/10)));
            p_tableWidget->setItem(row, column + 1, new QTableWidgetItem(QString::number(row * 200 + column * 300)));
        }
    }
#endif
    for(int row = 0; row < p_tableWidget->rowCount(); row++)
    {
        for(int column = 0; column < p_tableWidget->columnCount(); column += 2)
        {
            double value = 0.0;
            double count = 0;
            if(p_tableWidget->item(row, column) != NULL)
            {
                value = p_tableWidget->item(row, column)->text().toDouble();
                count = p_tableWidget->item(row, column + 1)->text().toInt();
            }
            int xPoint = value * 10 * rectWidth;
            int yPoint = - count / valueMultiplle ;
            painter.drawRect(QRect(QPoint(xPoint - 2, yPoint), QPoint(xPoint + 2, 0)));
        }
    }
}
