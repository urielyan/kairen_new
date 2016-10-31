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
        CalibrateData
    };
    static Database* instance();

    QSqlDatabase getDb() const;

    QString getTableName(TableName key);
    //QSqlQuery &getSqlQuery(TableName key);
    void deleteTableData(TableName key);

    uint getCalibrateDataCount();
    void insertDataToCalibraeData(uint tested, uint reference);

signals:

public slots:

private:
    explicit Database(QFrame *parent = 0);
    void createTable();
    QSqlDatabase db;
    QMap <int, QString> m_tables;
};

#endif // DATABASE_H
