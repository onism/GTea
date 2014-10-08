#ifndef SQLHELPER_H
#define SQLHELPER_H

#include <QObject>

#include <QFile>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
class SQLHelper
{
public:
    SQLHelper();
    bool openSQLiteDB();
    void closeSQLiteDB();
    bool createTable(QString sqlstring);
    bool insertRow(QString sqlstring);
    bool selectAll();
    bool deleteRow(QString sqlstring);
    bool deleteAll();
    QSqlError lastError();

  private:
      QSqlDatabase dbLink;

  signals:

  public slots:
};

#endif // SQLHELPER_H
