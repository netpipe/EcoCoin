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
#include <QGraphicsSvgItem>
using qrcodegen::QrCode;
//minimum 2 coin verify for transactions
//client wallet.sqlite

void MainWindow::GenerateQRCode(QString test) {

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


        QGraphicsScene *scene= new QGraphicsScene();
        scene->addItem(new QGraphicsSvgItem("./tmp.svg"));
        ui->graphicsView->setScene(scene);
        ui->graphicsView->show();


}




#endif // WALLET_H
