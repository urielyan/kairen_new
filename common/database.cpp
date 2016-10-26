#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>

#include "database.h"

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
  QStringList tableList = db.tables();
  if(!tableList.contains("sample_data"))
    {
      QSqlQuery query(db);
      ok = query.exec("CREATE TABLE sample_data(people_id,sample_serial,date_time,work_curve,measurement_time,repeat_time,average,deviation,is_auto,current_coefficient);");
      if(ok == false){
          QMessageBox::warning(this,"create table err",query.lastError().text());
          return;
        }
    }
}
QSqlDatabase Database::getDb() const
{
  return db;
}
