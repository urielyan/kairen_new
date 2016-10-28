#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QFrame>
#include <QSqlDatabase>
#include <QSqlQuery>
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
    QSqlQuery &getSqlQuery(TableName);

    uint getCalobrateDataCount();
    void insertDataToCalibraeData(uint tested, uint reference);

    QString getTableName(TableName key);
signals:

public slots:

private:
    explicit Database(QFrame *parent = 0);
    void createTable();
    QSqlDatabase db;
    QMap <int, QString> tables;
};

#endif // DATABASE_H
