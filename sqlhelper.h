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
    static  bool openSQLiteDB();
    static  void closeSQLiteDB();
    static bool createTable(QString sqlstring);
    static bool insertRow(QString sqlstring);
    static  bool selectAll();
    static   bool deleteRow(QString sqlstring);
    static  bool deleteAll();
    static  QSqlError lastError();
    static  bool  update(QString sqlstring);
    static QList<QString> selectbysql(QString sqlstring);
    static QSqlDatabase dbLink;

  signals:

  public slots:
};

#endif // SQLHELPER_H
