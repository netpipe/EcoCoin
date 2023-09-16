#ifndef ENGINE_H
#define ENGINE_H
#include "Engine/global.h"
#include "Engine/dbmanager.h"
#include "Engine/coinmanager.h"
#include "Src/downloadmanager.h"
#include "Engine/crtthread.h"
#include <Engine/Stylemanager.h>
#include "Src/ftp-server/ftpserver.h"
#include <QCryptographicHash>

struct KeyFile{
    QString strMasterKey;
    QString strCoinKey;
    QString strMD5;
    QString strHash;
} ;

struct FTPInfo{
    QString     strHost;
    int         nPort;
    QString     strUser;
    QString     strPass;
};
struct CoinInfo{
    QString strCoinName;
    QString strCoinCount;
    QString strCoinLength;
    QString strCreateDay;
    QString strCreateMonth;
    QString strCreateYear;
    QString strCreateTime;
    QString strCoinValue;
    QString strMatures;
    QString strCoinpayout;
    QString strEncrypted;
    QString strMatureDate;
    QString strMatureMonth;
    QString strMatureYear;
    QString strMatureTime;
    QString strMaturedescription;
    QString strUsbDriveName;
};

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
    QString             simpledecrypt(QString string, QString key, QCryptographicHash::Algorithm hash);
    QString             simplecrypt(QString string, QString key, QCryptographicHash::Algorithm hash);
    QString             GetReallyRandomString(int length, QString chars);
    void                updateHash();

    // FTP connection info
    void                loadFTPInfo();

    // RX & TX
    void                processTX(QString, QString);
    void                processRX(QString, QString);
    QString             RCoin2YearDB(int, QString);
    void                makeResult(QString, QString);
    QString             validateCOINsign(QString, QString, QString);
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

    // Coin parts
    void                GenerateCoin(int, int);
    bool                SaveGeneratedCoinInfo();
    void                generateTxFile(QString, QString, QString, QString);
    bool                validateTxFile(QString);
    void                RemoveRXTX(QString filename);
    void                ProcessRXTX(QString);
    float               checkClientBalance(QString, QString);
    float               checkServerBalance();

    // User management
    QString             GetValidUserID(QStringList);
    bool                IsEmailExists(QString);
    QString             GetUserIDFromEmail(QString);

    // FTP
    void                Download(QString);
    void                saveFTPInfo();
private:
    // version
    uint m_nVersion;
    // style manager
    StyleManager    *m_ThemeManager;
    DBManager       *m_DBManager;
    CoinManager     *m_CoinManager;
    DownloadManager *m_DownloadManager;
    FtpServer       *m_FTPServer;
    CRTThread       *m_RTThread;
    // is admin
    bool            m_isAdmin;
    // Key File
    KeyFile         m_Key;

public:
    FTPInfo         m_FTPInfo;
    CoinInfo        m_CoinInfo;
};

#endif // ENGINE_H
