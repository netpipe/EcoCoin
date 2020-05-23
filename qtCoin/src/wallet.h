#ifndef WALLET_H
#define WALLET_H
#include <mainwindow.h>
#include "ui_mainwindow.h"
#include  <math.h>
#include <QCoreApplication>
#include <QFile>
#include <QDebug>
//#include <QMainWindow>
#include <QCryptographicHash>
#include <QMessageBox>
#include <QTextCodec>
#include <QFileDialog>
#include "QRCode/QrCode.hpp"

#include <iostream>
#include <fstream>
#ifdef BARCODE
#include <QGraphicsSvgItem>
#include "Barcode/functii.h"
#include "Barcode/quirc/tests/inspect.h"
#endif
#ifdef BARCODE
using qrcodegen::QrCode;
#endif
//minimum 2 coin verify for transactions
//client wallet.sqlite


void MainWindow::createWalletTable()
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

    query.append("CREATE TABLE IF NOT EXISTS address("
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

void MainWindow::createWalletCoinsTable()
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

    query.append("CREATE TABLE IF NOT EXISTS wallet("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "userid VARCHAR(100),"
          //      "phone INTEGER NOT NULL," //incase lost ?
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

void MainWindow::WalletAddressInsert() //strictly a db to hold all userid's for verification
{
    createUserTable();

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


void MainWindow::GenerateQRCode(QString test) {
#ifdef BARCODE
    std::wstring text ( test.toStdWString() );

    //char *text2 = text.c_str();
    const wchar_t* wstr = text.c_str() ;
        char mbstr[4000];
        std::wcstombs(mbstr, wstr, 4000);


    //	const char *text = //"Hello, world!";              // User-supplied text
    //"tes1234567ajsl;dkfjasdlk;fj;lsadjf;lsdakjf;lsdkajf;alsdkjfs;dlakjfsdal;kjfsadlkjfsdaljfl;sdkaja;lskd \
    //fjlskdjflskdajflksa;djflksjdaf;lkjsda;lfkjsdalkfjsd;lkfjsda;ljf;lsdakjfl;skadjf;lksdjfl;ksajdfl;ksdjlk\
    //fj";

        const QrCode::Ecc errCorLvl = QrCode::Ecc::LOW;  // Error correction level

        // Make and print the QR Code symbol
        //const QrCode qr = QrCode::encodeText( text.c_str() , errCorLvl);
            const QrCode qr = QrCode::encodeText( mbstr , errCorLvl);
        //printQr(qr);
       // std::cout << qr.toSvgString(4) << std::endl;

        //system ( "" );
        ofstream write;

        std::string   filename = "tmp.svg";
        write.open(filename.c_str(), ios::out | ios::binary);
        write << qr.toSvgString(4);


        QImage *img_object = new QImage();
        img_object->load("./tmp.svg");
        QPixmap image = QPixmap::fromImage(*img_object);
        QPixmap scaled_img = image.scaled(ui->graphicsView->width(), ui->graphicsView->height(), Qt::KeepAspectRatio);
        QGraphicsScene *scene= new QGraphicsScene();
       // scene->addItem(new QGraphicsSvgItem("./tmp.svg"));
        scene->addPixmap(scaled_img);
        scene->setSceneRect(scaled_img.rect());
        ui->graphicsView->setScene(scene);
        ui->graphicsView->show();
#endif

}


void MainWindow::EAN13(QString productname,QString country,QString ean){ //barcode not used
#ifdef BARCODE
//    std::string code13 = EAN13::appendChecksum("123", "123456789"); //countrycode 3 letters,European Article Number 9 digits no spaces
//    std::string svg = EAN13::createSvg("productName test", code13);

    std::string code13 = EAN13::appendChecksum(country.toLatin1(), ean.toLatin1()); //countrycode 3 letters,European Article Number 9 digits no spaces
    std::string svg = EAN13::createSvg(productname.toStdString(), code13);

    ofstream write;
    std::string   filename = "tmp.svg";
    write.open(filename.c_str(), ios::out | ios::binary);
    write << svg.c_str();

    QImage *img_object = new QImage();
    img_object->load("./tmp.svg");
    QPixmap image = QPixmap::fromImage(*img_object);
 //   QPixmap scaled_img = image.scaled(this->width(), this->height(), Qt::KeepAspectRatio);
    QPixmap scaled_img = image.scaled(ui->graphicsView->width(), ui->graphicsView->height(), Qt::KeepAspectRatio);
    QGraphicsScene *scene= new QGraphicsScene();
   // scene->addItem(new QGraphicsSvgItem("./tmp.svg"));
    scene->addPixmap(scaled_img);
    scene->setSceneRect(scaled_img.rect());
    ui->graphicsView->setScene(scene);
    ui->graphicsView->show();
#endif
}




#endif // WALLET_H
