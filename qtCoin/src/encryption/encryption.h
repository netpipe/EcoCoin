#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include "rsa/Rsa.h"
#include "Qt-AES/qaesencryption.h"
#include <QCryptographicHash>
#include <QString>
#include <QDebug>
#include <QFile>
#include "../../mainwindow.h"

//QString rot13( const QString & input )
//{
//    QString r = input;
//    int i = r.length();
//    while( i-- ) {
//        if ( r[i] >= QChar('A') && r[i] <= QChar('M') ||
//             r[i] >= QChar('a') && r[i] <= QChar('m') )
//            r[i] = (char)((int)QChar(r[i]) + 13);
//        else if  ( r[i] >= QChar('N') && r[i] <= QChar('Z') ||
//                   r[i] >= QChar('n') && r[i] <= QChar('z') )
//            r[i] = (char)((int)QChar(r[i]) - 13);
//    }
//    return r;
//}

QByteArray md5Checksum(QString stuff)
{
    //https://stackoverflow.com/questions/16383392/how-to-get-the-sha-1-md5-checksum-of-a-file-with-qt
    // Returns empty QByteArray() on failure.

    QCryptographicHash hash(QCryptographicHash::Md5); //QCryptographicHash::Md5
    hash.addData(stuff.toLatin1());
    return hash.result();

        //return QByteArray();
}

QByteArray fileChecksum(const QString &fileName,QCryptographicHash::Algorithm hashAlgorithm)
{
    //https://stackoverflow.com/questions/16383392/how-to-get-the-sha-1-md5-checksum-of-a-file-with-qt
    // Returns empty QByteArray() on failure.
        QFile f(fileName);
        if (f.open(QFile::ReadOnly)) {
            QCryptographicHash hash(hashAlgorithm); //QCryptographicHash::Md5
            if (hash.addData(&f)) {
                return hash.result();
            }
        }
        return QByteArray();
}

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

QByteArray MainWindow::EncryptMsg(QString plainMsg,QString aeskey1,QString aeskey2)
{
    qDebug()<<"Encryption Started";
    QString enc1 = rsaenc(plainMsg);
   // QByteArray enc2 = aesenc(enc1, "123456789", "your-IV-vector");
    QByteArray enc2 = aesenc(enc1, aeskey1.toLatin1(), aeskey2.toLatin1());
    return enc2.toHex();
}

QString MainWindow::DecryptMsg(QByteArray encryptedMsg, Rsa *rsa,QString aeskey1,QString aeskey2)
{
    qDebug()<<"Decryption Started";
   // QString dec1 = aesdec(QByteArray::fromHex(encryptedMsg), "123456789", "your-IV-vector");
    QString dec1 = aesdec(QByteArray::fromHex(encryptedMsg), aeskey1.toLatin1(), aeskey2.toLatin1() );
    QString dec2 = rsadec(dec1, rsa);
    return dec2;
}


//still trying to get xor working

//QString XORencryptDecrypt(QString toEncrypt) {
//    QChar key = 'K'; //Any char will work
//  //  QString output = toEncrypt;

//    QBbytearray output = qstringtobyte (toEncrypt);
//    for (int i = 0; i < toEncrypt.size(); i++)
//        output[i] = toEncrypt[i] ^ key;

//    return output;
//}

string XOR(string value,string key)
{
    string retval(value);

    short unsigned int klen=key.length();
    short unsigned int vlen=value.length();
    short unsigned int k=0;
    short unsigned int v=0;

    for(v = 0;v<vlen;v++)
    {
        retval[v]=value[v]^key[k];
        k=(++k<klen?k:0);
    }

    return retval;
}

QString XORencryptDecrypt2(QString toEncrypt, QString key2)
{
    //QByteArray::toHex()
QByteArray arr = toEncrypt.toLatin1();
char key[3] = {'K', 'C', 'Q'};
for (int i = 0; i < arr.size(); i++)
    arr[i] = arr[i] ^ key[i % (sizeof(key) / sizeof(char))];

}

QString XORencryptDecrypt(QString toEncrypt, QString key2)
{

    const char* input = toEncrypt.toLatin1();
    int inputLength = toEncrypt.size(); //m_textEdit->toPlainText().toLatin1().length();
    const char* key = key2.toLatin1();//m_keyLineEdit->text().toLatin1().data();
    int keyLength = key2.size();//m_keyLineEdit->text().toLatin1().length();

    char output[inputLength];

    for (int i = 0; i < inputLength + 1; ++i)
    {
        output[i] = input[i] ^ key[i % keyLength + 1];
    }

//    if (strinfo.length () > 0) {
//    Qtextcodec *codec = Qtextcodec::codecforname ("Utf-8");
//    result = Codec->fromunicode (strinfo);
//    }


    //m_textEdit->setText(QString::fromLatin1(output, inputLength));
    return QString::fromLatin1(output, inputLength);
}



#endif


