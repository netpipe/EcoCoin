//#ifndef SSL_H
//#define SSL_H


//https://forum.qt.io/topic/70616/ssl-client-server-handshake
//#include "ServerExample.h"

//#include "SslServer.h"

//#include <QCoreApplication>
//#include <QHostAddress>
//#include <QSslSocket>
//#include <QThread>

//ServerExample::ServerExample(QObject *parent) : QObject(parent)
//{
//}


//void ServerExample::run()
//{
//    QHostAddress address = QHostAddress::Any;
//    quint16 port = 22333;

//    SslServer sslServer;
//    sslServer.setSslLocalCertificate("~/Downloads/Qt-SslServer-master/example/Server/Debug/debug/sslserver.pem");
//    sslServer.setSslPrivateKey("~/Downloads/Qt-SslServer-master/example/Server/Debug/debug/sslserver.key");
//    sslServer.setSslProtocol(QSsl::TlsV1_2);

//    if (sslServer.listen(address, port))
//        qDebug().nospace() << "Now listening on " << qPrintable(/*address.toString()*/ "127.0.0.1") << ":" << port;
//    else
//        qDebug().nospace() << "ERROR: could not bind to " << qPrintable(address.toString()) << ":" << port;

//    if (sslServer.waitForNewConnection(-1))    // Wait until a new connection is received, -1 means no timeout
//    {
//        qDebug() << "New connection";
//        QSslSocket *sslSocket = dynamic_cast<QSslSocket*>(sslServer.nextPendingConnection());

//        if (sslSocket->waitForReadyRead(5000))    // Wait until some data is received, 5000 ms timeout (-1 doesn't work here)
//        {
//            QByteArray message = sslSocket->readAll();    // Read message
//            qDebug() << "Message:" << QString(message);

////this is the area where the program will check if Steve is there and reply back to the client

//            sslSocket->disconnectFromHost();    // Disconnect
//            sslSocket->waitForDisconnected();    // Wait until disconnected
//            qDebug() << "Disconnected";
//        }

//        else
//        {
//            qDebug().nospace() << "ERROR: could not receive message (" << qPrintable(sslSocket->errorString()) << ")";
//        }
//    }

//    else
//    {
//        qDebug().nospace() << "ERROR: could not establish encrypted connection (" << qPrintable(sslServer.errorString()) << ")";
//    }

//    this->deleteLater();
//    QThread::currentThread()->quit();
//    qApp->exit();
//}

//#include "ClientExample.h"

//#include <QCoreApplication>
//#include <QString>
//#include <QSslSocket>
//#include <QThread>

//ClientExample::ClientExample(QObject *parent) : QObject(parent)
//{
//}


//void ClientExample::run()
//{
//    QString hostName = "127.0.0.1";    // DO NOT CHANGE THIS AS IT MUST MATCH THE FQDN OF THE CERTIFICATE (you MUST create your own certificate in order to change this)
//    quint16 port = 22333;

//    QSslSocket sslSocket;
//    sslSocket.addCaCertificates("~/Downloads/Qt-SslServer-master/example/Client/Debug/debug/sslserver.pem");
//    sslSocket.connectToHostEncrypted(hostName, port);

//    if (sslSocket.waitForEncrypted(-1))    // Wait until encrypted connection is established, -1 means no timeout
//    {
//        qDebug() << "Connected";
//        sslSocket.write("Hello, Steve!");    // Send message to the server

//        if (sslSocket.waitForBytesWritten(-1))    // Wait until message is sent (also makes QSslSocket flush the buffer)
//            qDebug() << "Message sent";
//        else
//            qDebug().nospace() << "ERROR: could not send message (" << qPrintable(sslSocket.errorString()) << ")";

//        while (!sslSocket.waitForDisconnected())    // Wait until disconnected
//            QThread::msleep(10);

//        qDebug() << "Disconnected";
//    }

//    else
//    {
//        qDebug().nospace() << "ERROR: could not establish encrypted connection (" << qPrintable(sslSocket.errorString()) << ")";
//    }

//    this->deleteLater();
//    QThread::currentThread()->quit();
//    qApp->exit();
//}

//sslSocket::sslSocket(QObject *parent) : QSslSocket(parent) //generate ssl certificate
//{
//https://forum.qt.io/topic/45728/generating-cert-key-during-run-time-for-qsslsocket/6
//    EVP_PKEY * pkey = nullptr;
//    RSA * rsa = nullptr;
//    X509 * x509 = nullptr;
//    X509_NAME * name = nullptr;
//    BIO * bp_public = nullptr, * bp_private = nullptr;
//    const char * buffer = nullptr;
//    long size;

//    pkey = EVP_PKEY_new();
//    q_check_ptr(pkey);
//    rsa = RSA_generate_key(2048, RSA_F4, nullptr, nullptr);
//    q_check_ptr(rsa);
//    EVP_PKEY_assign_RSA(pkey, rsa);
//    x509 = X509_new();
//    q_check_ptr(x509);
//    ASN1_INTEGER_set(X509_get_serialNumber(x509), 1);
//    X509_gmtime_adj(X509_get_notBefore(x509), 0); // not before current time
//    X509_gmtime_adj(X509_get_notAfter(x509), 31536000L); // not after a year from this point
//    X509_set_pubkey(x509, pkey);
//    name = X509_get_subject_name(x509);
//    q_check_ptr(name);
//    X509_NAME_add_entry_by_txt(name, "C", MBSTRING_ASC, (unsigned char *)"US", -1, -1, 0);
//    X509_NAME_add_entry_by_txt(name, "O", MBSTRING_ASC, (unsigned char *)"My Organization", -1, -1, 0);
//    X509_NAME_add_entry_by_txt(name, "CN", MBSTRING_ASC, (unsigned char *)"My Common Name", -1, -1, 0);
//    X509_set_issuer_name(x509, name);
//    X509_sign(x509, pkey, EVP_sha1());
//    bp_private = BIO_new(BIO_s_mem());
//    q_check_ptr(bp_private);

//    if(PEM_write_bio_PrivateKey(bp_private, pkey, nullptr, nullptr, 0, nullptr, nullptr) != 1)
//    {
//        EVP_PKEY_free(pkey);
//        X509_free(x509);
//        BIO_free_all(bp_private);
//        qFatal("PEM_write_bio_PrivateKey");
//    }

//    bp_public = BIO_new(BIO_s_mem());
//    q_check_ptr(bp_public);

//    if(PEM_write_bio_X509(bp_public, x509) != 1)
//    {
//        EVP_PKEY_free(pkey);
//        X509_free(x509);
//        BIO_free_all(bp_public);
//        BIO_free_all(bp_private);
//        qFatal("PEM_write_bio_PrivateKey");
//    }

//    size = BIO_get_mem_data(bp_public, &buffer);
//    q_check_ptr(buffer);
//    setLocalCertificate(QSslCertificate(QByteArray(buffer, size)));

//    if(localCertificate().isNull())
//    {
//        qFatal("Failed to generate a random client certificate");
//    }

//    size = BIO_get_mem_data(bp_private, &buffer);
//    q_check_ptr(buffer);
//    setPrivateKey(QSslKey(QByteArray(buffer, size), QSsl::Rsa));

//    if(privateKey().isNull())
//    {
//        qFatal("Failed to generate a random private key");
//    }

//    EVP_PKEY_free(pkey); // this will also free the rsa key
//    X509_free(x509);
//    BIO_free_all(bp_public);
//    BIO_free_all(bp_private);
//}


//#endif // SSL_H
