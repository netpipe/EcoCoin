#ifndef SYSTEM_H
#define SYSTEM_H


#include <mainwindow.h>
#include "ui_mainwindow.h"

#include <QStorageInfo>

#ifdef linux
#include <iostream>
#endif
#include <QString>
#include <QProcess>
#include <QDebug>
#include <QMessageBox>


//cat /proc/cpuinfo for extra info
QString getPSN()  //cpu serialnumber
{
int varEAX, varEBX, varECX, varEDX;
char str[9];
char PSN[30];

QString cpuid;

//%eax=1 gives most significant 32 bits in eax
//__asm__ __volatile__ ("cpuid": "=a" (varEAX), "=b" (varEBX), "=c" (varECX), "=d" (varEDX) : "a" (1));
sprintf(str, "%08X", varEAX); //i.e. XXXX-XXXX-xxxx-xxxx-xxxx-xxxx
sprintf(PSN, "%C%C%C%C-%C%C%C%C", str[0], str[1], str[2], str[3], str[4], str[5], str[6], str[7]);
//%eax=3 gives least significant 64 bits in edx and ecx [if PN is enabled]
//__asm__ __volatile__ ("cpuid": "=a" (varEAX), "=b" (varEBX), "=c" (varECX), "=d" (varEDX) : "a" (3));
sprintf(str, "%08X", varEDX); //i.e. xxxx-xxxx-XXXX-XXXX-xxxx-xxxx
sprintf(PSN, "%s-%C%C%C%C-%C%C%C%C", PSN, str[0], str[1], str[2], str[3], str[4], str[5], str[6], str[7]);
sprintf(str, "%08X", varECX); //i.e. xxxx-xxxx-xxxx-xxxx-XXXX-XXXX
sprintf(PSN, "%s-%C%C%C%C-%C%C%C%C", PSN, str[0], str[1], str[2], str[3], str[4], str[5], str[6], str[7]);

    for (int i=0; i< sizeof( PSN ); ++i)
   {
       cpuid+=PSN[i];
   }

    return cpuid.toLatin1();

}




QString getHDserial(){
#ifdef linux

    QProcess process;
    process.start( "udevadm info --name=/dev/sdd" );
   // process.start("sh", QStringList()<<"-c"<<"udevadm info --name=/dev/sdd | grep ID_SERIAL");
    process.waitForFinished(-1); // will wait forever until finished
  //  process.kill();
    QString stdout = process.readAll();
  //  QString stdout = process2.readAllStandardOutput();
   QStringList test = stdout.split(":");
   QStringList test2 = test.at(71).split("=");
//   for (int i=0;i < test.size() ; i++ ){

//      qDebug() << QString::number(i)<< i << "number:";
//      qDebug() << test.at(i).toLatin1();
//   }
   // QString stderr = process.readAllStandardError();
//qDebug()<< stdout;


    //system("hdparm -i /dev/hda | grep -i serial");
    //udevadm info --query=all --name=/dev/sdd | grep ID_SERIAL
    //lsblk --nodeps -o name,serial
    //system("wmic path win32_physicalmedia get SerialNumber");

    return test2.at(1);
#endif

#if win32
    QProcess process;
    process.start( "wmic path win32_physicalmedia get SerialNumber" );
    process.waitForFinished(-1); // will wait forever until finished
    QString stdout = process.readAll();
    QStringList test = stdout.split(":");
    return test.at(1);
#endif
}


void MainWindow::BackUptoUSB(){
#ifdef STORAGE
    //store and retrieve master encryption keys with this.

//https://stackoverflow.com/questions/40035332/how-to-get-path-to-usb-drive-on-linux-in-qt
    foreach (const QStorageInfo &storage, QStorageInfo::mountedVolumes()) {
       qDebug() << storage.rootPath();
//       QString storagestring=storage.rootPath();
//       QRegExp rx("[/]");// match a comma or a space
//       QStringList list2 = storagestring.split(rx);
//      qDebug() << storagestring.at(3);
       QString usbstring = "backupdevice";//ui->usbdrivename->text().toLatin1();

        if (storage.rootPath().contains(usbstring)){
            //qDebug() << "yep" << "/n";
            backupusbpath = storage.rootPath().contains(usbstring);

            if (storage.isReadOnly())
                qDebug() << "isReadOnly:" << storage.isReadOnly();

//                qDebug() << "name:" << storage.name();
//                qDebug() << "fileSystemType:" << storage.fileSystemType();
//                qDebug() << "size:" << storage.bytesTotal()/1000/1000 << "MB";
//                qDebug() << "availableSize:" << storage.bytesAvailable()/1000/1000 << "MB";
        } else {
            backupusbpath="";
        }

        if (backupusbpath.toLatin1() == "")
        {
            //date
            QFile::copy("/settings.txt", backupusbpath.toLatin1() );
            QFile::copy("/coins.sqlite", backupusbpath.toLatin1() );
            QFile::copy("/availableCoins.sqlite", backupusbpath.toLatin1() );
            QFile::copy("/hashes.txt", backupusbpath.toLatin1() );

            QMessageBox Msgbox;
                Msgbox.setText("drive not found: ");
                Msgbox.exec();
        }
     }
#endif
}

int MainWindow::getkeys(){ //for coldstorage server or standalone server which contains all the infos

    qDebug() << "getting keys";
    //if keys are stored in the local folder and checkout then use those

    bool keyexists=0;
    QString path;
    if (ui->usbdrivename->text().toLatin1() != ""){
        ListUSB();
        QString path2;
        path2 = usbpath.toLatin1()+"/keys.txt";
        QFile MyFile2(path2);
        if ( MyFile2.exists() ){        keyexists= true;        path = usbpath.toLatin1()+"/keys.txt";    }
    } else {
        QString path3;
        path3 = "./keys.txt";
        QFile MyFile3(path3);
        if ( MyFile3.exists() ){    keyexists= true;   path = "./keys.txt"; }
    }

    QFile MyFile(path);

    if(MyFile.exists() && keyexists ){
    MyFile.open(QIODevice::ReadWrite);
    QTextStream in (&MyFile);
    QString line;
    QStringList list;
     //   QList<QString> nums;
    QStringList nums;
    QRegExp rx("[:]");
    do {
        line = in.readLine();
        if (line.contains(":")) {
            list = line.split(rx);
            nums.append(list.at(1).toLatin1());
        }
    } while (!line.isNull());


    //remove debugs later for security
    QString test;
    masterkey=nums.at(0);
    qDebug() << "masterkey" << masterkey;
    coinkey=nums.at(1);
    qDebug() << "coinkey" << coinkey;
    test=nums.at(2);
    qDebug() << "coinsmd5" << test;
    test=nums.at(3);
    qDebug() << "stringhash" << test;
    //verify string hash

    QString testmd5 = md5Checksum("masterkey:"+ masterkey.toLatin1()+"coinkey:" +coinkey.toLatin1()+"coinsmd5:" + fileChecksum("coins.sqlite",QCryptographicHash::Md5)).toHex();
    //qDebug() << "stringhash" << testmd5;
    if (test.toLatin1() == testmd5.toLatin1()){
       qDebug() << "md5's match";
    return 1;

    }else {
              qDebug() << "md5's no match";

              masterkey="";
              coinkey="";
        return 0;
    }
}
    //verify md5sum of keys file from 2 or 3 locations possibly encrypted
    //simple strings found on google have same md5sums or bruteforce could match it.

}



void MainWindow::ListUSB(){
    #ifdef STORAGE
    //store and retrieve master encryption keys with this.

//https://stackoverflow.com/questions/40035332/how-to-get-path-to-usb-drive-on-linux-in-qt
    foreach (const QStorageInfo &storage, QStorageInfo::mountedVolumes()) {

       qDebug() << storage.rootPath();

//       QString storagestring=storage.rootPath();
//       QRegExp rx("[/]");// match a comma or a space
//       QStringList list2 = storagestring.split(rx);

//      qDebug() << storagestring.at(3);
       QString usbstring = ui->usbdrivename->text().toLatin1();

        if ( storage.rootPath().contains(usbstring) && usbstring != ""){
           qDebug() << "yep found the drive match" << "/n";
            usbpath = storage.rootPath();

            if (storage.isReadOnly())
               qDebug() << "isReadOnly:" << storage.isReadOnly();

//                qDebug() << "name:" << storage.name();
//                qDebug() << "fileSystemType:" << storage.fileSystemType();
//                qDebug() << "size:" << storage.bytesTotal()/1000/1000 << "MB";
//                qDebug() << "availableSize:" << storage.bytesAvailable()/1000/1000 << "MB";
        }
     }

    if (usbpath.toLatin1() == "")
    {
        QMessageBox Msgbox;
            Msgbox.setText("drive not found: ");
            Msgbox.exec();
    }
#endif
}




#endif // SYSTEM_H
