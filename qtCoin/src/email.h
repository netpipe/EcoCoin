#ifndef EMAIL_H
#define EMAIL_H
#include <mainwindow.h>
#include "ui_mainwindow.h"
//#include <QCoreApplication>
#include "src/qstylesheetmanager.h"
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <QFile>
#include <QDebug>
#include <QDirIterator>
#ifdef SMTP
#include "src/smtp/SmtpMime"
#endif


int MainWindow::smtpsend(QString toemail,QString Message){
#ifdef SMTP
    bool ssl;
    if ( ui->smtpssl->isChecked() == 1 ){ ssl = 1;    }else{ ssl = 0; }

    SmtpClient smtp (ui->smtphost->text().toLatin1(), ui->smtpport->text().toInt(), ssl ? SmtpClient::SslConnection : SmtpClient::TcpConnection);
    smtp.setUser(ui->smtpemail->text());
    smtp.setPassword(ui->smtppassword->text());

    MimeMessage message;
    EmailAddress sender(ui->smtpemail->text(), ui->smtpemail->text()); //email,name
    message.setSender(&sender);
    EmailAddress to(toemail.toLatin1(), toemail.toLatin1()); // email, name
    message.addRecipient(&to);
    message.setSubject(ui->coinname->text());

    MimeText text;
  //  text.setText("Hi,\nThis is a simple email message.\n");
    text.setText(Message.toLatin1());
    // Now add it to the mail
    message.addPart(&text);

    // Now we can send the mail
    if (!smtp.connectToHost()) {
        qDebug() << "Failed to connect to host!" << endl;
        return 1;
    }

    if (!smtp.login()) {
        qDebug() << "Failed to login!" << endl;
        return 2;
    }

    if (!smtp.sendMail(message)) {
        qDebug() << "Failed to send mail!" << endl;
        return 3;
    }
    smtp.quit();
#endif
}


void MainWindow::createEmailTable()
{
    db.setDatabaseName("wallet.sqlite");
    if(db.open())
    {
       qDebug()<<"Successful database connection";
    }
    else
    {
       qDebug()<<"Error: failed database connection";
    }

    QString query;


    query.append("CREATE TABLE IF NOT EXISTS email("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "Host VARCHAR(50),"
                    "Port VARCHAR(50),"
                    "Email VARCHAR(50),"
                    "Password VARCHAR(100),"
                    "Encrypted VARCHAR(100)"
                    ");");

//qDebug () << query.toLatin1();

    QSqlQuery create;
    QSqlQuery drop;
    drop.prepare ("DROP TABLE IF EXISTS email");
    create.prepare(query);
drop.exec();

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

void MainWindow::getEmailSettings(){

    db.setDatabaseName("wallet.sqlite");
    if(db.open())
    {
       qDebug()<<"Successful database connection";
    }
    else
    {
       qDebug()<<"Error: failed database connection";
    }
        QString query;

         query.append("SELECT * FROM email" );


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

    while (select.next())
    {
               ui->smtphost->setText(select.value(1).toByteArray().constData()) ;
               qDebug() << select.value(1).toByteArray().constData();
        ui->smtpport->setText(select.value(2).toByteArray().constData()) ;
 qDebug() << select.value(2).toByteArray().constData();
    }
}

void MainWindow::EmailInsertWallet() //QString ID,QString CoinAddress,QString Owner,QString cid,QString date
{
    createEmailTable();
   // createWalletCoinsTable(ID);

    db.setDatabaseName("wallet.sqlite");
    if(db.open())
    {
       qDebug()<<"Successful database connection";
    }
    else
    {
       qDebug()<<"Error: failed database connection";
    }

    QString query;

 //   query.append("DROP table email");
  //  query.append("DELETE FROM email WHERE id=1");


    query.append("INSERT INTO email("
                 "Host,"
                 "Port,"
                 "Email,"
                 "Password,"//
                 "Encrypted)" "VALUES("
                 "'"+ui->smtphost->text().toLatin1()+"',"
                 "'"+ui->smtpport->text().toLatin1()+"',"
                 "'"+ui->smtpemail->text().toLatin1()+"',"
                 "'"+ui->smtppassword->text().toLatin1()+"',"
                 "'"+ui->smtpssl->text().toLatin1()+"'"
                 ");");

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


void MainWindow::on_smtpsave_clicked()
{
    EmailInsertWallet();
}

void MainWindow::on_smtptestmessage_clicked()
{

   //  text.setText("Hi,\nThis is a simple email message.\n");
#ifdef SMTP
 //  smtpsend(QString toemail,QString Message)
#else
    QMessageBox Msgbox;
        Msgbox.setText("nosmtp enabled");
        Msgbox.exec();
#endif
}


#endif // EMAIL_H
