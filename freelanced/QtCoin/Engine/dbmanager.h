#ifndef DBMANAGER_H
#define DBMANAGER_H
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QVariantList>

class DBManager
{
public:
    DBManager();
    void    ExecuteQuery(QString, QString);

    int     InsertData(QString, QString, QList<QString>);
    int     InsertData2(QString, QString, QList<int>, QList<QString>);
    int     InsertData3(QString, QString, QList<QVariantList>, int);
    void    CreateUserTable();

    QList<QList<QString>>  SelectData(QString, QString, int);
private:
    QSqlDatabase m_DB;
};

#endif // DBMANAGER_H
