#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QFrame>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QMap>

class Database : public QFrame
{
    Q_OBJECT
public:
    enum TableName
    {
        Sample = 0,
        CalibrateData,
        CalibrateResults,
        CountData
    };
    static Database* instance();

    QSqlDatabase getDb() const;

    QString getTableName(TableName key);
    //QSqlQuery &getSqlQuery(TableName key);
    void deleteTableDatas(TableName key);
    uint getTableDataCount(TableName key);

    void insertDataToCalibraeData(uint tested, uint reference);

    /**
     * 计算kb值：将kb值存入工作曲线1-9中。
     *  可以计算kb值得必要条件是：需要有3-11组有效标定数据。
     * @param[in] key: 工作曲线
     * @note:
     */
    bool countKbValue(uint key);
    int getEffectiveCalibrateDataCount();

signals:

public slots:

private:
    explicit Database(QFrame *parent = 0);
    void createTable();

private:
    QSqlDatabase m_database;
    QMap <int, QString> m_tables;
};

#endif // DATABASE_H
