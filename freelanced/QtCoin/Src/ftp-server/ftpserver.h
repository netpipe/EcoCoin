#ifdef FTP
#ifndef FTPSERVER_H
#define FTPSERVER_H

#include <QObject>
#include <QSet>
#include "Engine/global.h"
#include <QSettings>

class SslServer;

// The ftp server. Listens on a port, and starts a new control connection each
// time it gets connected.

class FtpServer : public QObject
{
    Q_OBJECT
public:
    explicit FtpServer(QObject *parent);
    /*explicit FtpServer(QObject *parent, const QString &rootPath, int port = 21,
                       const QString &userName = QString(), const QString &password = QString(),
                       bool readOnly = false, bool onlyOneIpAllowed = false, bool userslist = false);
*/
    // Whether or not the server is listening for incoming connections. If it
    // is not currently listening then there was an error - probably no
    // internet connection is available, or the port address might require root
    // priviledges (on Linux).
    bool    isListening();
    void    StartServer();
    void    loadSettings();
    void    saveSettings();
    void    setSettings(QString, QString, int, QString, bool, bool, bool, bool, QStringList);
signals:
    // A connection from a new IP has been established. This signal is emitted
    // when the FTP server is connected by a new IP. The new IP will then be
    // stored and will not cause this FTP server instance to emit this signal
    // any more.
    void newPeerIp(const QString &ip);

private slots:
    // Called by the SSL server when we have received a new connection.
    void startNewControlConnection();

public:
    QString     m_strUser;
    QString     m_strPass;
    int         m_Port;
    QString     m_strPath;
    bool        m_bAnony;
    bool        m_bReadOnly;
    bool        m_bOnlyOneIpAllowed;
    bool        m_bUserList;

    QStringList m_Users;
private:
    SslServer *server;
    QObject     *m_Parent;
};

#endif // FTPSERVER_H
#endif
