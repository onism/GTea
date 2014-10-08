#include "sqlhelper.h"


#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QFile>
#include <QDir>
#include <QDebug>

SQLHelper::SQLHelper()
{
}



bool SQLHelper::openSQLiteDB()
{
    // SQL Driver
    dbLink = QSqlDatabase::addDatabase("QSQLITE");


    QString dbPath(QDir::current().path());
    dbPath.append(QDir::separator()).append("db").append(QDir::separator()).append("db.sqlite");
    dbPath = QDir::toNativeSeparators(dbPath);


    qDebug() << dbPath;


    dbLink.setDatabaseName(dbPath);


    return dbLink.open();
}

void SQLHelper::closeSQLiteDB()
{

    dbLink.close();
}

bool SQLHelper::createTable(QString sqlstring)
{
    bool ret = false;


    if (dbLink.isOpen())
    {
        QSqlQuery query;
        query.prepare(sqlstring);
        if (!query.exec())
            qDebug() << "Create table: " << dbLink.lastError().text();
        else
            ret = 1;
    }

    return ret;
}

bool SQLHelper::insertRow(QString  sqlstring)
{
    bool ret = false;


    if (dbLink.isOpen())
    {
        QSqlQuery query;
        query.prepare(sqlstring);
        if (!query.exec())
            qDebug() << "Insert: " << ret << this->lastError();
        else
            ret = 1;
    }

    return ret;
}

bool SQLHelper::selectAll()
{
    return true;
}

bool SQLHelper::deleteRow(QString sqlstring)
{
    bool ret = false;


    if (dbLink.isOpen())
    {
        QSqlQuery query;
        query.prepare(sqlstring);
        if (!query.exec())
            qDebug() << "delete: " << ret << this->lastError();
        else
            ret = 1;
    }

    return ret;
}

bool SQLHelper::deleteAll()
{
    return true;
}

QSqlError SQLHelper::lastError()
{
    return dbLink.lastError();
}
