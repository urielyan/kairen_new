#include "wininforlistdialog.h"

#include "database.h"

#include <QLatin1String>
#include <QMapIterator>
#include <QtWidgets/QMessageBox>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>

Database *Database::instance()
{
    static Database _instance;
    return &_instance;
}

Database::Database(QFrame *parent)
    : QFrame(parent)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("./new.db");
    bool ok = db.open();
    if(ok == false)
    {
        QMessageBox::warning(this,QString("db open err"),db.lastError().text());
        return;
    }

    m_tables[Sample] = QString("sample_data");
    m_tables[CalibrateData] = QString("calibrate_data");

    createTable();
}

void Database::createTable()
{
    if(!db.isValid())
    {
        return;
    }

    QMap <int, QString> createTableString;
    createTableString[Sample] = QString("CREATE TABLE %1("
                                        "people_id,"
                                        "sample_serial,"
                                        "date_time,"
                                        "work_curve,"
                                        "measurement_time,"
                                        "repeat_time,"
                                        "average,"
                                        "deviation,"
                                        "is_auto,"
                                        "current_coefficient);").arg(
                m_tables[Sample]);
    createTableString[CalibrateData] = QString("CREATE TABLE %1("
                                               "id primary key,"
                                               "tested,"
                                               "reference,"
                                               "sulfur_content);").arg(
                m_tables[CalibrateData]);

    QStringList databaseTableList = db.tables();
    QMapIterator <int, QString> tablesIterator(m_tables);
    while (tablesIterator.hasNext())
    {
        tablesIterator.next();
        QString tableName = tablesIterator.value();
        if(!databaseTableList.contains(tableName))
        {
            QSqlQuery query(db);
            bool ok = query.exec(createTableString.value(tablesIterator.key()));
            if(ok == false){
                QMessageBox::warning(this,"create table err",query.lastError().text());
                return;
            }
        }
    }
}
QSqlDatabase Database::getDb() const
{
    if(!db.isValid())
    {
        return QSqlDatabase();
    }
    return db;
}

void Database::deleteTableData(Database::TableName key)
{
    Q_ASSERT(m_tables.contains(key));

    if(!db.isValid())
    {
        return;
    }
    bool ok = false;

    QSqlQuery query(db);
    ok = query.exec("delete from " + m_tables[key] + " where 1;");
    if(ok == false)
    {
        WinInforListDialog::instance()->showMsg(
                    tr("deleteTableData error!"),
                    query.lastError().text());
    }
}

uint Database::getCalibrateDataCount()
{
    if(!db.isValid())
    {
        return 0;
    }
    bool ok = false;
    int count = 0;

    QSqlQuery query(db);
    ok = query.exec("select * from " + m_tables[CalibrateData]);
    if(ok == false)
    {
        count = 0;
    }
    while (query.next())
    {
        ++count;
    }
    return ++count;
}

void Database::insertDataToCalibraeData(uint tested, uint reference)
{
    if(!db.isValid())
    {
        return;
    }
    bool ok = false;
    int id = getCalibrateDataCount();
    QSqlQuery query(db);
    query.clear();//":id, :tested, :reference, :sulfur_content);").arg(
    ok = query.exec(QString("insert into %1("
                            "id, tested, reference, sulfur_content) values("
                            "%2, %3, %4, %5);").arg(
                        m_tables[CalibrateData]).arg(
                        id).arg(
                        tested).arg(
                        reference).arg(
                        "00000")
                    );
    //    query.bindValue(0, id);
    //    query.bindValue(1, tested);
    //    query.bindValue(2, reference);
    //    query.bindValue(3, "00000");

    //    query.bindValue(":id", id);
    //    query.bindValue(":tested", tested);
    //    query.bindValue(":reference", reference);
    //    query.bindValue(":sulfur_content", "00000");
    //query.exec();
    if(ok == false)
    {
        WinInforListDialog::instance()->showMsg(
                    tr("insertDataToCalibraeData error!"),
                    query.lastError().text());
        return;
    }
    return;
}

QString Database::getTableName(Database::TableName key)
{
    Q_ASSERT(m_tables.contains(key));

    return m_tables.value(key);
}
