#include "database.h"

#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include <QLatin1String>
#include <QMap>

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
  if(ok == false){
      QMessageBox::warning(this,QString("db open err"),db.lastError().text());
      return;
    }

  createTable();
}

void Database::createTable()
{
    if(!db.isValid())
    {
        return;
    }

    QStringList databaseTableList = db.tables();
    QMap <TableName, QLatin1String> tables;
    tables[Sample] = "sample_data";
    tables[CalibrateData] = "calibrate";

    QMap <TableName, QLatin1String> createTableString;
    createTableString[Sample] = "CREATE TABLE sample_data("
                                "people_id,"
                                "sample_serial,"
                                "date_time,"
                                "work_curve,"
                                "measurement_time,"
                                "repeat_time,"
                                "average,"
                                "deviation,"
                                "is_auto,"
                                "current_coefficient);";
    createTableString[CalibrateData] = "CREATE TABLE calibrateData("
                                   "id,"
                                   "tested,"
                                   "reference,"
                                   "sulfur_content);";
    QMapIterator <TableName, QLatin1String> tablesIterator = tables.begin();
    while (tablesIterator.hasNext())
    {
        if(!databaseTableList.contains(tablesIterator.value()))
        {
            QSqlQuery query(db);
            ok = query.exec(createTableString.value(tablesIterator.key()));
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
