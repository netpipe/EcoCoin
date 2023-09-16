#ifndef ENGINE_H
#define ENGINE_H
#include "../Engine/global.h"
#include "../Engine/dbmanager.h"
#include "../Engine/coinmanager.h"
#include "../Src/downloadmanager.h"
#include "../Engine/crtthread.h"
#include "../Engine/Stylemanager.h"
#include "../Src/ftp-server/ftpserver.h"
#include <QCryptographicHash>



class Engine
{

public:
    Engine();
    uint    GetVersion(){return m_nVersion;}
private:
    // load key file
    QByteArray          fileChecksum(const QString &fileName, QCryptographicHash::Algorithm);
    QString             StringChecksum(const QString, QCryptographicHash::Algorithm);
    QByteArray          md5Checksum(QString);
    bool                loadKey(QString);
    void                loadKeys();
    QVector<QString>    listUSBDrives();
    bool                loadCoinSettings();


    bool                validateUserID(QString);
    QString             simpledecrypt(QString string, QString key, QCryptographicHash::Algorithm hash=QCryptographicHash::Sha512);
    QString             simplecrypt(QString string, QString key, QCryptographicHash::Algorithm hash=QCryptographicHash::Sha512);
    QString             GetReallyRandomString(int length, QString chars);
    void                updateHash();

    // FTP connection info
    void                loadFTPInfo();
    void                loadSMTPInfo();

    // RX & TX
    void                processTX(QString);
    int                 processRX(RXTX_File);
    QString             YearDB2RCoin(int, QString);
    QStringList         RCoin2YearDB(int, QString);
    QString             validateCOINsign(QString, QString, QString);
    QString             validateCOINunsign(QString, QString, QString);

    // Wallet
    void                load_walletUsers();
    void                createWalletDB();

public:
    void                InitEngine();
    void                Release();
    void                ClearCoinData();
    void                CreateCoinTable();
    void                SetWidget(QWidget *);
    QString             GetRandomString(int length, QString chars);
    void                saveCoinSettings();


    StyleManager*       GetStyleManager(){return m_ThemeManager;}
    CoinManager*        GetCoinManager(){return m_CoinManager;}
    DBManager*          GetDBManager(){return m_DBManager;}
    DownloadManager*    GetDownloadManager(){return m_DownloadManager;}
    FtpServer*          GetFTPServer(){return m_FTPServer;}
    bool                IsAdmin(){return m_isAdmin;}
    QStringList         GetWalletUserList(){return m_WalletUsers;}

    // Coin parts
    void                GenerateCoin(int, int);
    bool                SaveGeneratedCoinInfo();
    void                generateTxFile(QString, QString, QString, QString);
    int                 generateRxFile(QString strPath, QString fromID, QString toID, QString strAmount, QString strExpire);
    RXTX_File           validateTxRXFile(QString);
    void                RemoveRXTX(QString filename);
    int                 ProcessRXTX(QString);
    float               checkClientBalance(QString, QString);
    float               checkServerBalance();

    // User management
    QString             GetValidUserID(QStringList);
    bool                IsEmailExists(QString);

    // FTP
    void                Download(QString);
    void                saveFTPInfo();
    void                saveSMTPInfo();

    // Wallet
    int                 InsertNewWalletUser(WalletUser);

    // SMTP
    int                 SendSMTP(QString, QString);
private:
    // version
    uint m_nVersion;
    // style manager
    StyleManager    *m_ThemeManager;
    DBManager       *m_DBManager;
    CoinManager     *m_CoinManager;
    DownloadManager *m_DownloadManager;
    FtpServer       *m_FTPServer;
    // CRTThread       *m_RTThread;
    // is admin
    bool            m_isAdmin;
    // Key File
    KeyFile         m_Key;

    // Wallet users
    QStringList     m_WalletUsers;
public:
    FTPInfo         m_FTPInfo;
    CoinInfo        m_CoinInfo;
    SMTPInfo        m_SmtpInfo;
};

#endif // ENGINE_H
