#ifdef FTP
#include "ftpserver.h"
#include "ftpcontrolconnection.h"
#include "sslserver.h"


#include <QDebug>
#include <QSslSocket>
FtpServer::FtpServer(QObject *parent):QObject(parent)
{
    m_Parent = parent;
    server = nullptr;
    loadSettings();
}
void FtpServer::StartServer()
{
    if (server != nullptr)
    {
        delete server;
    }
    server = new SslServer(m_Parent);
#if QT_VERSION >= 0x050000
    server->listen(QHostAddress::AnyIPv4, m_Port);
#else
    server->listen(QHostAddress::Any, m_Port);
#endif
    connect(server, SIGNAL(newConnection()), this, SLOT(startNewControlConnection()));
}

void FtpServer::loadSettings()
{
    QString defaultPort = "8258";

    QString setting_path = QString("%1/%2").arg(QDir::currentPath()).arg(FTP_SETTING);
    QSettings settings(setting_path, QSettings::IniFormat);
    m_strUser = settings.value("settings/username", "admin").toString();
    m_strPass = settings.value("settings/password", "qt").toString();
    m_Port = settings.value("settings/port", 8258).toInt();
    m_strPath = settings.value("settings/rootpath", QDir::currentPath()).toString();
    m_bAnony = settings.value("settings/anonymous", false).toBool();
    m_bReadOnly = settings.value("settings/readonly", false).toBool();
    m_bOnlyOneIpAllowed = settings.value("settings/oneip", false).toBool();
    m_bUserList = settings.value("settings/userslist", false).toBool();

    QFile MyFile(FTP_USER_SETTING);
    MyFile.open(QIODevice::ReadWrite);
    QTextStream in (&MyFile);
    QString line;
    do {
        line = in.readLine();
        if(line.length() == 0)
            continue;
       m_Users.append(line);
    } while (!line.isNull());
}
void FtpServer::saveSettings()
{
    QString setting_path = QString("%1/%2").arg(QDir::currentPath()).arg(FTP_SETTING);
    QSettings settings(setting_path, QSettings::IniFormat);
    settings.setValue("settings/port", m_Port);
    settings.setValue("settings/username", m_strUser);
    settings.setValue("settings/password", m_strPass);
    settings.setValue("settings/rootpath", m_strPath);
    settings.setValue("settings/anonymous", m_bAnony);
    settings.setValue("settings/readonly", m_bReadOnly);
    settings.setValue("settings/oneip", m_bOnlyOneIpAllowed);
    settings.setValue("settings/userslist", m_bUserList);
    QFile file(FTP_USER_SETTING);
    if(file.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        QTextStream stream(&file);
        int sized = m_Users.count();
        for (int i=0; i < sized; i++){
            stream << m_Users.at(i) << endl;
        }
        file.close();
    }
}
void FtpServer::setSettings(QString strUser,
                            QString strPass,
                            int nPort,
                            QString strPath,
                            bool bAnony,
                            bool bReadOnly,
                            bool bOnlyOne,
                            bool bUserList,
                            QStringList users)
{
    m_strUser = strUser;
    m_strPass = strPass;
    m_Port = nPort;
    m_strPath = strPath;
    m_bAnony = bAnony;
    m_bReadOnly = bReadOnly;
    m_bOnlyOneIpAllowed = bOnlyOne;
    m_bUserList = bUserList;
    m_Users = users;
}
bool FtpServer::isListening()
{
    return server->isListening();
}

void FtpServer::startNewControlConnection()
{
    QSslSocket *socket = (QSslSocket *) server->nextPendingConnection();
    QString peerIp = socket->peerAddress().toString();
    qDebug() << "connection from" << peerIp;
    if (m_Users.indexOf(peerIp) < 0) {
        if (m_bOnlyOneIpAllowed && m_Users.length() != 0) {
            delete socket;
            return;
        }
        emit newPeerIp(peerIp);
        m_Users.push_back(peerIp);
    }
    // Create a new FTP control connection on this socket.
    new FtpControlConnection(this, socket, m_strPath, m_strUser, m_strPass, m_bReadOnly, m_bUserList);
}
#endif
