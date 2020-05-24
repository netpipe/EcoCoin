#ifndef WALLET_H
#define WALLET_H

#include <mainwindow.h>
#include "ui_mainwindow.h"
//#include <QCoreApplication>
#include <QFile>
#include <QDebug>
//#include <QMainWindow>
#include <QCryptographicHash>
#include <QMessageBox>
#include <QTextCodec>
#include <QFileDialog>
#include <QClipboard>



void MainWindow::searchyearly(QString ownerID)
{
    db.setDatabaseName("database.sqlite");
    if(db.open())
    {
       qDebug()<<"Successful database connection";
    }
    else
    {
       qDebug()<<"Error: failed database connection";
    }

    QString query;
    query.append("SELECT * FROM "+ownerID);

    QSqlQuery select;
    select.prepare(query);

    if (select.exec())
    {
        qDebug()<<"The user is properly selected";
    }
    else
    {
        qDebug()<<"The user is not selected correctly";
        qDebug()<<"ERROR! "<< select.lastError();
    }

    int row = 0;
    ui->tableWidgetUsers->setRowCount(0);

    while (select.next())
    {
        ui->tableWidgetUsers->insertRow(row);
        ui->tableWidgetUsers->setItem(row,0,new QTableWidgetItem(select.value(1).toByteArray().constData()));
        ui->tableWidgetUsers->setItem(row,1,new QTableWidgetItem(select.value(2).toByteArray().constData()));
        ui->tableWidgetUsers->setItem(row,2,new QTableWidgetItem(select.value(3).toByteArray().constData()));
        ui->tableWidgetUsers->setItem(row,3,new QTableWidgetItem(select.value(4).toByteArray().constData()));
        row++;
    }

    query.clear();
    db.close();
}

void MainWindow::createyearly(QString eownerID)
{
    //holds users generated from each new year and their coins pulled from rcoins.sqlite

    db.setDatabaseName("./db/"+year+".sqlite");

    if(db.open())    {       qDebug()<<"Successful database connection";    }
    else    {       qDebug()<<"Error: failed database connection";    }

    QString query;

    qDebug() << "test" << eownerID.toLatin1();

    query.append("CREATE TABLE IF NOT EXISTS ""'"+eownerID.toLatin1()+"'""("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "origindex VARCHAR(100)," //rcoins index then coins.sqlite is stored on usbdrive as part of key/verify
                    "addr VARCHAR(100),"
                    "datetime INTEGER,"
                    "class INTEGER"
                    ");");


    QSqlQuery create;
    create.prepare(query);

    if (create.exec())
    {
        qDebug()<<"Table exists or has been created";
    }
    else
    {
        qDebug()<<"Table not exists or has not been created";
        qDebug()<<"ERROR! "<< create.lastError();
    }
    query.clear();
    db.close();
}


void MainWindow::createUserTable()
{
    db.setDatabaseName("database.sqlite");
    if(db.open())
    {
       qDebug()<<"Successful database connection";
    }
    else
    {
       qDebug()<<"Error: failed database connection";
    }
    QString query;

    query.append("CREATE TABLE IF NOT EXISTS users("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "userid VARCHAR(100),"
                    "name VARCHAR(100),"
                    "password VARCHAR(100),"
                    "phone INTEGER,"//                    "phone INTEGER NOT NULL,"
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
        qDebug()<<"Table exists or has been created";
    }
    else
    {
        qDebug()<<"Table not exists or has not been created";
        qDebug()<<"ERROR! "<< create.lastError();
    }
    query.clear();
    db.close();
}


void MainWindow::selectUsers()
{
    db.setDatabaseName("database.sqlite");
    if(db.open())
    {
       qDebug()<<"Successful database connection";
    }
    else
    {
       qDebug()<<"Error: failed database connection";
    }


    QString query;
    query.append("SELECT * FROM users");

    QSqlQuery select;
    select.prepare(query);

    if (select.exec())
    {
        qDebug()<<"The user is properly selected";
    }
    else
    {
        qDebug()<<"The user is not selected correctly";
        qDebug()<<"ERROR! "<< select.lastError();
    }

    int row = 0;
    ui->tableWidgetUsers->setRowCount(0);

    while (select.next())
    {
        ui->tableWidgetUsers->insertRow(row);
        ui->tableWidgetUsers->setItem(row,0,new QTableWidgetItem(select.value(1).toByteArray().constData()));
        ui->tableWidgetUsers->setItem(row,1,new QTableWidgetItem(select.value(2).toByteArray().constData()));
        ui->tableWidgetUsers->setItem(row,2,new QTableWidgetItem(select.value(3).toByteArray().constData()));
        ui->tableWidgetUsers->setItem(row,3,new QTableWidgetItem(select.value(4).toByteArray().constData()));
        row++;
    }

    query.clear();
    db.close();
}


void MainWindow::on_userssearch_clicked() //search button
{
    db.setDatabaseName("database.sqlite");
    if(db.open())
    {
       qDebug()<<"Successful database connection";
    }
    else
    {
       qDebug()<<"Error: failed database connection";
    }
        QString query;

        //testing save the keys maybe shorten the encryption length ?

    if (ui->encrypted_yes->text() == "Yes" ){
#ifdef ENCRYPTION
            QByteArray bFname = EncryptMsg(ui->userid->text(),"123456789", "your-IV-vector");
            QString mykey1 = BigInt2Str(m_e); //rsa keys
            QString mykey2 = BigInt2Str(m_n); //rsa keys

            query.append("SELECT * FROM users WHERE name =" "'" + bFname  + "'" );
#endif
    }else {
        query.append("SELECT * FROM users WHERE name =" "'" + ui->userid->text()  + "'" );
}

    //search for coin owner / validity

    QSqlQuery select;
    select.prepare(query);

    if (select.exec())
    {
        qDebug()<<"The user is properly selected";
    }
    else
    {
        qDebug()<<"The user is not selected correctly";
        qDebug()<<"ERROR! "<< select.lastError();
    }

    int row = 0;
    ui->tableWidgetUsers->setRowCount(0);
#ifdef ENCRYPTION
    QString mykey1 = BigInt2Str(m_e); //rsa keys
    QString mykey2 = BigInt2Str(m_n); //rsa keys
#endif
    if (ui->encrypted_yes->text() == "Yes" ){

        while (select.next())
        {
#ifdef ENCRYPTION
            Rsa *rsa = new Rsa(BigInt(mykey1.toStdString()), BigInt(mykey2.toStdString()));
            QString strMsg = DecryptMsg(select.value(1).toByteArray().constData(), rsa,"123456789", "your-IV-vector");
           // QString strDate = DecryptMsg(bFname, rsa,"123456789", "your-IV-vector");
            delete rsa;

            ui->tableWidgetUsers->insertRow(row);
            ui->tableWidgetUsers->setItem(row,0,new QTableWidgetItem(strMsg));
            ui->tableWidgetUsers->setItem(row,1,new QTableWidgetItem(select.value(2).toByteArray().constData()));
            ui->tableWidgetUsers->setItem(row,2,new QTableWidgetItem(select.value(3).toByteArray().constData()));
            ui->tableWidgetUsers->setItem(row,3,new QTableWidgetItem(select.value(4).toByteArray().constData()));
            row++;
            #endif
        }
    }else{
        while (select.next())
        {
            ui->tableWidgetUsers->insertRow(row);
            ui->tableWidgetUsers->setItem(row,0,new QTableWidgetItem(select.value(1).toByteArray().constData()));
            ui->tableWidgetUsers->setItem(row,1,new QTableWidgetItem(select.value(2).toByteArray().constData()));
            ui->tableWidgetUsers->setItem(row,2,new QTableWidgetItem(select.value(3).toByteArray().constData()));
            ui->tableWidgetUsers->setItem(row,3,new QTableWidgetItem(select.value(4).toByteArray().constData()));
            row++;
        }
    }
    query.clear();
    db.close();
}


void MainWindow::SQLTest(QString dbname,QString Query)
{
    db.setDatabaseName(dbname.toLatin1());
    if(db.open())
    {       qDebug()<<"Successful database connection";    }
    else
    {       qDebug()<<"Error: failed database connection";    }

    QString query;
    query.append(Query.toLatin1());

    QSqlQuery select;
    select.prepare(query);

    if (select.exec())
    {
        qDebug()<<"The user is properly selected";
    }
    else
    {
        qDebug()<<"The user is not selected correctly";
        qDebug()<<"ERROR! "<< select.lastError();
    }

    int row = 0;
    ui->tableWidgetUsers->setRowCount(0);

    while (select.next())
    {
        ui->tableWidgetUsers->insertRow(row);
        ui->tableWidgetUsers->setItem(row,0,new QTableWidgetItem(select.value(0).toByteArray().constData()));
        ui->tableWidgetUsers->setItem(row,1,new QTableWidgetItem(""));
        ui->tableWidgetUsers->setItem(row,2,new QTableWidgetItem(""));
        ui->tableWidgetUsers->setItem(row,3,new QTableWidgetItem(""));
        row++;
    }

    query.clear();
    db.close();
}










void MainWindow::insertUser() //strictly a db to hold all userid's for verification
{
    createUserTable();

    db.setDatabaseName("database.sqlite");
    if(db.open())
    {
       qDebug()<<"Successful database connection";
    }
    else
    {
       qDebug()<<"Error: failed database connection";
    }

    QString query;

      QString euserid = ui->lineEditName->text();

//    QByteArray bFname = EncryptMsg(ui->lineEditName->text(),"123456789", "your-IV-vector");
//    QString mykey1 = BigInt2Str(m_e); //rsa keys
//    QString mykey2 = BigInt2Str(m_n); //rsa keys

    query.append("INSERT INTO users("
                 "userid,"
                 "name,"
                 "password,"
                 "phone,"//
                 "datetime,"
                 "ekey,"
                 "total,"
                 "extra,"
                 "class)"
                 "VALUES("
                 "'"+euserid.toLatin1()+"',"
                 "'"+ui->lineEditSurname->text().toLatin1()+"',"
                 "'"+ui->lineEditPassword->text().toLatin1()+"',"
                 "'"+ui->lineEditPhone->text().toLatin1()+"',"
                 "'"+ui->createuserdatetime->text()+"',"
                 "'""'," //ekey
                 "'""'," //ammount
                 "'"+ui->createextra->text().toLatin1()+"',"
                 "'"+ui->createclass->text()+"'"
                 ");");

    qDebug()<< euserid.toLatin1();

    QSqlQuery insert;
    insert.prepare(query);

    if (insert.exec())
    {
        qDebug()<<"The user is properly inserted";
    }
    else
    {
        qDebug()<<"The user is not inserted correctly";
        qDebug()<<"ERROR! "<< insert.lastError();
    }

    query.clear();
    db.close();

}




#endif // WALLET_H
