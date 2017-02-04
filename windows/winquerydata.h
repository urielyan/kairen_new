#ifndef QUERYDATA_H
#define QUERYDATA_H

#include "winabstractframe.h"

#include <QTableView>
#include <QSqlTableModel>

class ButtonWinMannager;
class QSqlTableModel;
class QTableWidget;

class CountDataModel : public QSqlTableModel
{
    Q_OBJECT

public:
    explicit CountDataModel (QObject *parent = 0);
    ~CountDataModel();

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
};

/**
 * @class 所有数据查询界面，需要设置model和标题在进入的时候
 */
class WinQueryDataBaseWidget : public WinAbstractFrame
{
    Q_OBJECT

public:
    explicit WinQueryDataBaseWidget(QWidget *parent = 0);

    void onEntry() Q_DECL_OVERRIDE;

    QSqlTableModel *model() const;
    void setModel(QSqlTableModel *model);

private:
    QTableView m_tableView;
    QSqlTableModel *m_model;

    void initViewAndModel();
};

class WinQuerySampleData: public WinAbstractFrame
{
    Q_OBJECT

public:
    explicit WinQuerySampleData(QWidget *parent = 0);

    void onEntry() Q_DECL_OVERRIDE;

private:
    QTableView m_tableView;
    QSqlTableModel *m_model;

    void initViewAndModel();
};

class WinQueryCountData : public WinAbstractFrame
{
    Q_OBJECT

public:
    explicit WinQueryCountData(QWidget *parent = 0);

    void onEntry() Q_DECL_OVERRIDE;

private:
    QTableView m_tableView;
    QSqlTableModel *m_model;

    void initViewAndModel();
};

class WinQueryData : public WinAbstractFrame
{
    Q_OBJECT

public:
    enum WinId
    {
        Sample = 0
        ,Count
        ,Calibrate
    };
    explicit WinQueryData(QWidget *parent = 0);

private slots:

private:
    ButtonWinMannager *p_buttonWinMannager;
};

#endif // QUERYDATA_H
