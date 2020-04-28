#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include "rsa/Rsa.h"
#include "Qt-AES/qaesencryption.h"
#include <QCryptographicHash>
#include <QString>
#include <QDebug>

QString MainWindow::rsaenc(QString string2, Rsa *rsa)
{
    qDebug()<<"Encryption Start";

    string plainMsg=string2.toStdString();
    string encryptedMsg=rsaTester->rsa_MSG_encryp(plainMsg);
    return QString::fromStdString(encryptedMsg);
}

QString MainWindow::rsadec(QString string2, Rsa *rsa)
{
    qDebug()<<"Decryption Start";

    string encMsg = string2.toStdString();
    string decryptedMsg=rsa->rsa_MSG_decryp(encMsg);
    return QString(decryptedMsg.c_str());

 }


QByteArray MainWindow::aesenc(QString string2,QString skey,QString siv)
{
    QString key(skey);
    QString iv(siv);
    QByteArray hashKey = QCryptographicHash::hash(key.toLocal8Bit(), QCryptographicHash::Sha256);
    QByteArray hashIV =    QCryptographicHash::hash(iv.toLocal8Bit(), QCryptographicHash::Md5);
    QByteArray encrypted = QAESEncryption::Crypt(QAESEncryption::AES_256, QAESEncryption::CBC,string2.toLocal8Bit(), hashKey, hashIV, QAESEncryption::PKCS7);
    return encrypted;
}

QString MainWindow::aesdec(QByteArray string2,QString skey,QString siv )
{
    QAESEncryption encryption(QAESEncryption::AES_256, QAESEncryption::CBC);

    QString key(skey);
    QString iv(siv);
    QByteArray hashKey = QCryptographicHash::hash(key.toLocal8Bit(), QCryptographicHash::Sha256);
    QByteArray hashIV =    QCryptographicHash::hash(iv.toLocal8Bit(), QCryptographicHash::Md5);

    //QByteArray encrypted = QAESEncryption::Crypt(QAESEncryption::AES_256, QAESEncryption::CBC,inputStr.toLocal8Bit(), hashKey, hashIV); //... // Removal of Padding via Static function QString decodedString =
    QByteArray encrypted = QAESEncryption::Decrypt(QAESEncryption::AES_256, QAESEncryption::CBC,string2, hashKey, hashIV);
    QByteArray encrypted1 = QAESEncryption::RemovePadding(encrypted, QAESEncryption::ISO);
    //QByteArray decodeText = encryption.decode(encrypted, hashKey, hashIV);
    QString decodedString = QString(encrypted1);

    return decodedString;
}

QString MainWindow::encdec2(QString string2,int encdec)
{
//    if (encdec == 0){
//    string2=rsadec(string2);
//    string2=aesdec(string2,"123456789","your-IV-vector");
//    qDebug() << "decrypted string is " << string2;
//        }
//    else {
//    string2=rsaenc(string2);
//    string2=aesenc(string2,"123456789","your-IV-vector");

//    qDebug() << "encrypted string is " << string2;
//    }
    return string2;
}

QString MainWindow::encdec(QString string2,int encdec)
{
    qDebug()<<"Encryption Start";

    string plainMsg=string2.toStdString();
    qDebug() << QString(plainMsg.c_str());

    Rsa rsaTester;
    string encryptedMsg=rsaTester.rsa_MSG_encryp(plainMsg);
    BigInt e, n;

    qDebug() << "rsaencrypted string" << QString(encryptedMsg.c_str());

    rsaTester.publish_keys(e, n);
    qDebug()<<QString(encryptedMsg.c_str());

    QAESEncryption encryption(QAESEncryption::AES_256, QAESEncryption::CBC);

    QString inputStr(QString::fromStdString(encryptedMsg));
    QString key("123456789");
    QString iv("your-IV-vector");
    QByteArray hashKey = QCryptographicHash::hash(key.toLocal8Bit(), QCryptographicHash::Sha256);
    QByteArray hashIV =    QCryptographicHash::hash(iv.toLocal8Bit(), QCryptographicHash::Md5);

    QByteArray encrypted = QAESEncryption::Crypt(QAESEncryption::AES_256, QAESEncryption::CBC,inputStr.toLocal8Bit(), hashKey, hashIV); //... // Removal of Padding via Static function QString decodedString =
    qDebug()<<"aes_encrypted string"<<encrypted;
    QString test2=QString(encrypted);
    QString test ;//= QString( "'" + QString::fromStdString(encrypted) + "'");
    test.append("'");
    test.append( test2 );
    test.append("'");


    qDebug() <<"testing string from qbytearray"<< test;

    QByteArray decodeText = encryption.decode(encrypted, hashKey, hashIV);
    QString decodedString = QString(encryption.removePadding(decodeText));
    string decoded = decodedString.toStdString();
    string decryptedMsg=rsaTester.rsa_MSG_decryp(decoded);
    qDebug()<<"aes->rsa decrypted message"<<QString(decryptedMsg.c_str());
    return (decryptedMsg.c_str());
}

QByteArray MainWindow::EncryptMsg(QString plainMsg)
{
    qDebug()<<"Encryption Started";
    QString enc1 = rsaenc(plainMsg);
    QByteArray enc2 = aesenc(enc1, "123456789", "your-IV-vector");
    return enc2.toHex();
}

QString MainWindow::DecryptMsg(QByteArray encryptedMsg, Rsa *rsa)
{
    qDebug()<<"Decryption Started";
    QString dec1 = aesdec(QByteArray::fromHex(encryptedMsg), "123456789", "your-IV-vector");
    QString dec2 = rsadec(dec1, rsa);
    return dec2;
}

#endif


