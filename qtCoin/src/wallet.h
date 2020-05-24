#ifndef POUCH_H
#define POUCH_H

//had to rename to pouch because wallet was causing compile errors

#include <mainwindow.h>
#include "ui_mainwindow.h"

//#include <QCoreApplication>
#include <QFile>
#include <QDebug>
#include <QCryptographicHash>
#include <QMessageBox>
#include <QTextCodec>
#include <QFileDialog>

#ifdef BARCODE
    #include <iostream>
    #include <fstream>
    #include "QRCode/QrCode.hpp"
    #include <QGraphicsSvgItem>
    #include <QGraphicsView>
    #include "Barcode/functii.h"
    #include "Barcode/quirc/tests/inspect.h"
    using qrcodegen::QrCode;
#endif

//minimum 2 coin verify for transactions
//client wallet.sqlite



void MainWindow::walletCoinInsert(QString ID,QString CoinAddress,QString Owner,QString cid,QString date) //QString ID,QString CoinAddress,QString Owner,QString cid,QString date
{

    createWalletCoinsTable(ID);

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

    query.append("INSERT INTO "+ID.toLatin1()+"("
                 "userid,"
                 "CoinAddress,"
                 "Owner,"
                 "Class,"//
                 "DateTime)" "VALUES("
                 "'"+ID.toLatin1()+"',"
                 "'"+CoinAddress.toLatin1()+"',"
                 "'"+Owner.toLatin1()+"',"
                 "'"+cid.toLatin1()+"',"
                 "'"+date.toLatin1()+"'"
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

void MainWindow::createWalletHistoryTable()
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

    query.append("CREATE TABLE IF NOT EXISTS history("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "DateTime INTEGER NOT NULL,"
                    "RXTX VARCHAR(50),"
                    "ID VARCHAR(100),"
                    "Ammount VARCHAR(100),"
                    "ContactName VARCHAR(100)"
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


void MainWindow::HistoryInsert(QString datetime,QString RXTX,QString ID,QString Ammount,QString contactname) //strictly a db to hold all userid's for verification
{
    createWalletHistoryTable();

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
    query.append("INSERT INTO history("
                 "DateTime,"
                 "RXTX,"
                 "ID,"
                 "Ammount,"//
                 "ContactName,"//
                 "VALUES("
                 "'"+datetime.toLatin1()+"',"
                 "'"+RXTX.toLatin1()+"',"
                 "'"+ID.toLatin1()+"',"
                 "'"+Ammount.toLatin1()+"',"
                 "'"+contactname.toLatin1()+"',"
                 ");");

   // qDebug()<< euserid.toLatin1();

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


void MainWindow::createWalletTable(QString ID)
{
    db.setDatabaseName("wallet.sqlite");
    if(db.open())
    {
       qDebug() << "Successful database connection";
    }
    else
    {
       qDebug() << "Error: failed database connection";
    }

    QString query;

    query.append("CREATE TABLE IF NOT EXISTS "+ID.toLatin1()+"("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "userid VARCHAR(100),"
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

void MainWindow::createWalletCoinsTable(QString ID) //place to hold users coins
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

    query.append("CREATE TABLE IF NOT EXISTS "+ID.toLatin1()+"(" //put address here
                 "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                 "CoinAddress VARCHAR(100),"
                 "Owner VARCHAR(100),"
                 "Class VARCHAR(100),"
                 "Date INTEGER NOT NULL"
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




void MainWindow::WalletAddressInsert(QString Email,QString Name,QString classid,QString datetime,QString address)
{
    //createWalletTable();

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

    query.append("INSERT INTO addresses("
                 "Email,"
                 "Name,"
                 "class,"
                 "datetime,"
                 "address,"
                 "class)""VALUES("
                 "'"+Email.toLatin1()+"',"
                 "'"+Name.toLatin1()+"',"
                 "'"+classid.toLatin1()+"',"
                 "'"+datetime.toLatin1()+"',"
                 "'"+address.toLatin1()+"',"
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


void MainWindow::GenerateQRCode(QString data,QGraphicsView *view) {
#ifdef BARCODE
    std::wstring text ( data.toStdWString() );

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
        QPixmap scaled_img = image.scaled(view->width(), view->height(), Qt::KeepAspectRatio);
        QGraphicsScene *scene= new QGraphicsScene();
       // scene->addItem(new QGraphicsSvgItem("./tmp.svg"));
        scene->addPixmap(scaled_img);
        scene->setSceneRect(scaled_img.rect());
        view->setScene(scene);
        view->show();
#endif

}


void MainWindow::EAN13(QString productname,QString country,QString ean,QGraphicsView *graphicsView){ //barcode not used
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
    QPixmap scaled_img = image.scaled(graphicsView->width(), graphicsView->height(), Qt::KeepAspectRatio);
    QGraphicsScene *scene= new QGraphicsScene();
   // scene->addItem(new QGraphicsSvgItem("./tmp.svg"));
    scene->addPixmap(scaled_img);
    scene->setSceneRect(scaled_img.rect());
    graphicsView->setScene(scene);
    graphicsView->show();
#endif
}




#endif // WALLET_H
