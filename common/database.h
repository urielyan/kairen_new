#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QFrame>
#include <QSqlDatabase>

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

signals:

public slots:

private:
  explicit Database(QFrame *parent = 0);
  void createTable();
  QSqlDatabase db;
};

#endif // DATABASE_H
