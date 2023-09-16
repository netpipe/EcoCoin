#include "dbmanager.h"
#include <QDebug>
#include <Engine/global.h>

DBManager::DBManager()
{
    m_DB = QSqlDatabase::addDatabase("QSQLITE");
}

void DBManager::ExecuteQuery(QString dbName, QString str_query)
{

    try {
        m_DB.setDatabaseName(dbName);
        if(m_DB.open())
        {
            QSqlQuery query;
            m_DB.transaction();
            query.prepare(str_query);
            query.exec();

            m_DB.commit();
            query.clear();
        }
        m_DB.close();
    } catch (exception e) {
        qDebug() << e.what();
    }

}

int DBManager::InsertData(QString dbName, QString strQuery, QList<QString> listData)
{
    int nCount = 0;
    try {
        m_DB.setDatabaseName(dbName);
        if(m_DB.open())
        {
            m_DB.transaction();
            QVariantList coins;
            for(int i = 0; i < listData.count(); i++)
            {
                coins << listData.at(i);
            }
            QSqlQuery insertQuery;
            insertQuery.prepare(strQuery);
            insertQuery.addBindValue(coins);
            if(insertQuery.execBatch())
            {
                qDebug() << "Insert into"<< dbName << "successed.";
            }
            m_DB.commit();
            insertQuery.clear();
        }
        m_DB.close();
        /*QFile f("coins2.txt");
        if (f.open(QIODevice::ReadWrite | QIODevice::Append | QIODevice::Text)) // QIODevice::Append |
        {
            QTextStream stream(&f);
            f.seek(0);
            for (int i = 0; i < listData.count(); i++)
            {
                stream << QString::number(i) << ":" << listData[i] << endl;
            }
        }
        f.close();*/
    } catch (exception e) {
        qDebug() << e.what();
    }
    return nCount;
}


int DBManager::InsertData2(QString dbName, QString strQuery, QList<int> listIndex, QList<QString> listData)
{
    int nCount = 0;
    try {
        m_DB.setDatabaseName(dbName);
        if(m_DB.open())
        {
            m_DB.transaction();
            QVariantList indexes;
            QVariantList coins;
            for(int i = 0; i < listData.count(); i++)
            {
                indexes << listIndex.at(i);
                coins << listData.at(i);
            }
            QSqlQuery insertQuery;
            insertQuery.prepare(strQuery);
            insertQuery.addBindValue(indexes);
            insertQuery.addBindValue(coins);
            if(insertQuery.execBatch())
            {
                qDebug() << "Insert into"<< dbName << "successed.";
            }
            m_DB.commit();
            insertQuery.clear();
        }
        m_DB.close();
        /*QFile f("coins2.txt");
        if (f.open(QIODevice::ReadWrite | QIODevice::Append | QIODevice::Text)) // QIODevice::Append |
        {
            QTextStream stream(&f);
            f.seek(0);
            for (int i = 0; i < listData.count(); i++)
            {
                stream << QString::number(i) << ":" << listData[i] << endl;
            }
        }
        f.close();*/
    } catch (exception e) {
        qDebug() << e.what();
    }
    return nCount;
}
int DBManager::InsertData3(QString dbName, QString strQuery, QList<QVariantList> data, int nNum)
{
    int nCount = 0;
    try {
        m_DB.setDatabaseName(dbName);
        if(m_DB.open())
        {
            m_DB.transaction();

            QSqlQuery insertQuery;
            insertQuery.prepare(strQuery);
            for(int i = 0; i < nNum; i++)
                insertQuery.addBindValue(data[i]);
            if(insertQuery.execBatch())
            {
                qDebug() << "Insert into"<< dbName << "successed.";
            }
            m_DB.commit();
            insertQuery.clear();
            nCount = data[0].length();
        }
        m_DB.close();
    } catch (exception e) {
        qDebug() << e.what();
    }
    return nCount;
}
void DBManager::CreateUserTable()
{
    m_DB.setDatabaseName(DB_NAME);
    if (m_DB.open())
    {
        qDebug() << "Successful database connection";
    }
    else
    {
        qDebug() << "Error: failed database connection";
    }
    QString query;

    query.append("CREATE TABLE IF NOT EXISTS users("
                 "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                 "email VARCHAR(100),"
                 "userid VARCHAR(100),"
                 "name VARCHAR(100),"
                 "password VARCHAR(100),"
                 "phone INTEGER," //                    "phone INTEGER NOT NULL,"
                 "datetime INTEGER NOT NULL,"
                 "ekey VARCHAR(100),"
                 "total VARCHAR(100),"
                 "extra VARCHAR(100),"
                 "class INTEGER"
                 ");");

    QSqlQuery create;
    create.prepare(query);

    if (create.exec())
    {
        qDebug() << "Table exists or has been created";
    }
    else
    {
        qDebug() << "Table not exists or has not been created";
        qDebug() << "ERROR! " << create.lastError();
    }
    query.clear();
    m_DB.close();
}

QList<QList<QString>>  DBManager::SelectData(QString strDB, QString strQuery, int nColCount)
{
    QList<QList<QString>> resList;
    try {
        m_DB.setDatabaseName(strDB);
        m_DB.open();
        QSqlQuery query;
        query.prepare(strQuery);
        query.exec();
        while (query.next())
        {
            QList<QString> strRow;
            for(int i = 0; i < nColCount; i++)
            {
                strRow.push_back(query.value(i).toString());
            }
            resList.push_back(strRow);
        }
        QSqlDatabase::database().commit();
        m_DB.close();
    } catch (exception e) {
        qDebug()<<"Select Data Err:"<<e.what();
    }
    return resList;
}
