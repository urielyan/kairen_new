#include "wininforlistdialog.h"

#include "database.h"

#include <QLatin1String>
#include <QMapIterator>
#include <QVariant>
#include <QtWidgets/QMessageBox>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>

Database *Database::instance()
{
    static Database _instance;
    return &_instance;
}

QSqlDatabase Database::getDb() const
{
    if (!m_database.isValid())
    {
        return QSqlDatabase();
    }
    return m_database;
}

QString Database::getTableName(Database::TableName key)
{
    Q_ASSERT(m_tables.contains(key));

    return m_tables.value(key);
}

void Database::deleteTableDatas(Database::TableName key)
{
    Q_ASSERT(m_tables.contains(key));

    if (!m_database.isValid())
    {
        return;
    }
    bool ok = false;

    QSqlQuery query(m_database);
    ok = query.exec("delete from " + m_tables[key] + " where 1;");
    if (ok == false)
    {
        WinInforListDialog::instance()->showMsg(
                    tr("deleteTableData error!"),
                    query.lastError().text());
    }
}

uint Database::getTableDataCount(Database::TableName key)
{
    if (!m_database.isValid())
    {
        return 0;
    }
    bool ok = false;
    int count = 0;

    QSqlQuery query(m_database);
    ok = query.exec("select * from " + m_tables[key]);
    if (ok == false)
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
    if (!m_database.isValid())
    {
        return;
    }
    bool ok = false;
    int id = getTableDataCount(CalibrateData);
    QSqlQuery query(m_database);
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

#if 0
    query.bindValue(0, id);
    query.bindValue(1, tested);
    query.bindValue(2, reference);
    query.bindValue(3, "00000");

    query.bindValue(":id", id);
    query.bindValue(":tested", tested);
    query.bindValue(":reference", reference);
    query.bindValue(":sulfur_content", "00000");
    query.exec();
#endif

    if (ok == false)
    {
        WinInforListDialog::instance()->showMsg(
                    tr("insertDataToCalibraeData error!"),
                    query.lastError().text());
        return;
    }
    return;
}

bool Database::countKbValue(uint key)
{
    if (!m_database.isValid())
    {
        return false;
    }

    Q_ASSERT(key <= 9 && key >0);

    uint calibrateDataCount = getTableDataCount(CalibrateData);
    if (
            calibrateDataCount < 3
            || calibrateDataCount > 12
            || getEffectiveCalibrateDataCount() < 3
            || getEffectiveCalibrateDataCount() > 12
            )
    {
        return false;
    }

    QSqlQuery query(m_database);
    bool ok = query.exec("select * from " + m_tables[CalibrateData]);
    if (ok == false)
    {
        return false;
    }
    QList<double> testedReferenceList;//带测样和参考样的比
    QList<double> sulfurContentList;
    while (query.next())
    {
        bool testedOk = false;
        bool referencedOk = false;
        bool sulfurContentOk = false;
        double testedValue = 0;
        double referencedValue = 0;
        double sulfurContentValue = 0.0;

        testedValue = query.value(1).toDouble(&testedOk);
        referencedValue = query.value(2).toDouble(&referencedOk);
        sulfurContentValue = query.value(3).toDouble(&sulfurContentOk);
        if(
                false ==testedOk
                || false == referencedOk
                || false == sulfurContentOk
                )
        {
            return false;
        }
        double referenceProportionTested = testedValue / referencedValue;
        testedReferenceList.append(referenceProportionTested);
        sulfurContentList.append(sulfurContentValue);
    }

    return true;
}

int Database::getEffectiveCalibrateDataCount()
{
    if (!m_database.isValid())
    {
        return 0;
    }

    int effectiveCount = 0;
    bool ok = false;

    QSqlQuery query(m_database);
    ok = query.exec("select sulfur_content from " + m_tables[CalibrateData]);
    if (ok == false)
    {
        effectiveCount = 0;
    }
    while (query.next())
    {
        double sulfurContent = query.value(0).toDouble(&ok);
        if (true == ok && sulfurContent != 0)
        {
            ++effectiveCount;
        }
    }

    return effectiveCount;
}

Database::Database(QFrame *parent)
    : QFrame(parent)
{
    m_database = QSqlDatabase::addDatabase("QSQLITE");
    m_database.setDatabaseName("./new.db");
    bool ok = m_database.open();
    if (ok == false)
    {
        QMessageBox::warning(this, QString("db open err"),
                             m_database.lastError().text());
        return;
    }

    m_tables[Sample] = QString("sample_data");
    m_tables[CalibrateData] = QString("calibrate_data");
    m_tables[CountData] = QString("count_data");

    createTable();
}

void Database::createTable()
{
    if (!m_database.isValid())
    {
        return;
    }

    QMap <int, QString> createTableString;
    createTableString[Sample] = QString(
                "CREATE TABLE %1("
                "people_id, sample_serial, "
                "date_time, "
                "work_curve, measurement_time, repeat_time, "
                "average, deviation, "
                "is_auto, "
                "current_coefficient);")
            .arg(m_tables[Sample]);
    createTableString[CalibrateData] = QString(
                "CREATE TABLE %1("
                "id primary key, "
                "tested, reference, "
                "sulfur_content);")
            .arg(m_tables[CalibrateData]);

    createTableString[CountData] = QString(
                "CREATE TABLE %1("
                "id primary key, "
                "dateTime, average, "
                "lambda);")
            .arg(m_tables[CountData]);

    QStringList databaseTableList = m_database.tables();
    QMapIterator <int, QString> tablesIterator(m_tables);
    while (tablesIterator.hasNext())
    {
        tablesIterator.next();
        QString tableName = tablesIterator.value();
        if (!databaseTableList.contains(tableName))
        {
            QSqlQuery query(m_database);
            bool ok = query.exec(createTableString.value(tablesIterator.key()));
            if (ok == false)
            {
                QMessageBox::warning(
                            this,
                            "create table err" + m_tables[tablesIterator.key()],
                        query.lastError().text()
                        );
                return;
            }
        }
    }
}
