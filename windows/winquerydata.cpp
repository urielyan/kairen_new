#include "common/global.h"
#include "common/wininforlistdialog.h"
#include "common/abstractfactory.h"
#include "windows/buttonwinmannager.h"
#include "common/datasave.h"
#include "common/database.h"

#include "winquerydata.h"

#include <QDebug>
#include <QButtonGroup>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QTableWidget>
#include <QDateTime>
#include <QModelIndex>
#include <QHeaderView>

WinQueryData::WinQueryData(QWidget *parent)
    : WinAbstractFrame(parent)
{
    setTitle(tr("Query history data"));

    p_buttonWinMannager = new ButtonWinMannager(this);

    {
        QBoxLayout *p_hBoxLayout = p_componentFactory->getBoxLayout(QBoxLayout::LeftToRight);

        QPushButton *p_querySampleButton = p_componentFactory->getButton(tr("Query sample data"), this);
        p_hBoxLayout->addWidget(p_querySampleButton);

        QPushButton *p_queryCalibrateButton = p_componentFactory->getButton(tr("Query calibrate data"), this);
        p_hBoxLayout->addWidget(p_queryCalibrateButton);

        addLayout(p_hBoxLayout);

//        p_buttonWinMannager->addButtonWin(p_querySampleButton, new WinSpectruMeasure(this), Sample);
//        p_buttonWinMannager->addButtonWin(p_queryCalibrateButton, new WinCountMeasure(this), Calibrate);
    }

    {
        QBoxLayout *p_hBoxLayout = p_componentFactory->getBoxLayout(QBoxLayout::LeftToRight);

        QPushButton *p_queryCountButton = p_componentFactory->getButton(tr("Query count data"), this);
        p_hBoxLayout->addWidget(p_queryCountButton);

        p_hBoxLayout->addWidget(getReturnButton());

        addLayout(p_hBoxLayout);

        p_buttonWinMannager->addButtonWindow(p_queryCountButton, new WinQueryCountData(this), Count);
    }
}


WinQueryCountData::WinQueryCountData(QWidget *parent)
    : WinAbstractFrame(parent)
{
    setTitle(tr("Query count data"));

    p_componentFactory = new MeasureFrameComponent(this);

    initViewAndModel();
    addWidget(getReturnButton());
}

void WinQueryCountData::onEntry()
{
    m_model->select();
}


void WinQueryCountData::initViewAndModel()
{
    m_model = new CountDataModel;
    m_model->setTable(Database::instance()->getTableName(Database::CountData));
//    m_model->setHeaderData(0, Qt::Horizontal, tr("Id"));
//    m_model->setHeaderData(1, Qt::Horizontal, tr("DateTime"));
//    m_model->setHeaderData(2, Qt::Horizontal, tr("Average"));
//    m_model->setHeaderData(3, Qt::Horizontal, tr("Lambda"));

    m_tableView.setModel(m_model);

    m_tableView.horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_tableView.horizontalHeader()->setStretchLastSection(true); //设置充满表宽度

    addWidget(&m_tableView);
}

CountDataModel::CountDataModel(QObject *parent)
 : QSqlTableModel(parent)
{
    setTable(Database::instance()->getTableName(Database::CountData));
    setHeaderData(0, Qt::Horizontal, tr("Id"));
    setHeaderData(1, Qt::Horizontal, tr("DateTime"));
    setHeaderData(2, Qt::Horizontal, tr("Average"));
    setHeaderData(3, Qt::Horizontal, tr("Lambda"));
}

CountDataModel::~CountDataModel()
{
}

QVariant CountDataModel::data(const QModelIndex &index, int role) const
{
    Q_ASSERT(index.isValid());

    //当为时间差列时，将时间差改为正常时间，
    if (index.column() == 1 && role == Qt::DisplayRole)
    {
        quint64 timeInterVal = QSqlTableModel::data(index, role).toLongLong();
        return QDateTime::fromTime_t(timeInterVal).toString("yyyy/MM/dd hh:mm:ss");
    }

    //设置所有文字居中显示。
    if(role == Qt::TextAlignmentRole)
    {
        return Qt::AlignCenter;
    }

    return QSqlTableModel::data(index, role);
}

WinQueryDataBaseWidget::WinQueryDataBaseWidget(QWidget *parent)
    : WinAbstractFrame(parent)
    , m_model(NULL)
{
    initViewAndModel();

    p_componentFactory = new MeasureFrameComponent(this);
    addWidget(getReturnButton());
}

void WinQueryDataBaseWidget::onEntry()
{
    Q_ASSERT(m_model != NULL);

    m_model->select();
}

QSqlTableModel *WinQueryDataBaseWidget::model() const
{
    return m_model;
}

void WinQueryDataBaseWidget::setModel(QSqlTableModel *model)
{
    m_model = model;
    m_tableView.setModel(m_model);
}

void WinQueryDataBaseWidget::initViewAndModel()
{
    m_tableView.horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_tableView.horizontalHeader()->setStretchLastSection(true); //设置充满表宽度

    addWidget(&m_tableView);
}

WinQuerySampleData::WinQuerySampleData(QWidget *parent)
{

}

void WinQuerySampleData::onEntry()
{

}

void WinQuerySampleData::initViewAndModel()
{

}
