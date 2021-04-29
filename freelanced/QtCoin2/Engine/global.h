#ifndef GLOBAL_H
#define GLOBAL_H

#include <QString>
#include <QTime>
#include <QDebug>
#include <QFile>
#include <QUrl>
#include <QDir>
#include <iostream>
#include <fstream>
#include "../Src/encryption/simpleCrypt/simplecrypt.h"
#include "../Src/QRCode/QrCode.hpp"

typedef unsigned int uint;
using namespace std;
#ifdef BARCODE
using qrcodegen::QrCode;
#endif


#define DB_DIR      "DB"
#define COIN_DB_NAME            "coins.sqlite"
#define DB_NAME                 "database.sqlite"
#define AVAIL_COINS_DB_NAME     "availableCoins.sqlite"
#define RCOINS_DB_NAME          "rcoins.sqlite"
#define WALLET_DB               "wallet.sqlite"

#define FTP_SETTING             "ftp.ini"
#define FTP_CLIENT_SETTING      "ftp_client.ini"
#define FTP_USER_SETTING        "ftp_users.txt"

#define SMTP_SETTING            "smtp.ini"

#define COIN_TXT_NAME           "coins.txt"
#define HASH_TXT_NAME           "hashes.txt"
#define STRING_TEMPLATE         "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890"
#define COIN_SETTINGS           "settings.txt"
#define RXTX                    "rxtx"
#define WORKSPACE               "workspace"
#define RTXT_DELIMITER          ";"
#define RXTX_RESSTR             "ok"

#define CLIENT_FILE             "client"
enum FTP_TRANSACTION {GET=0, PUT=1};

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

struct SMTPInfo{
    QString     strHost;
    int         nPort;
    QString     strEmail;
    QString     strPass;
    bool        bEncrypted;
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
struct WalletUser{
    QString strID;
    QString strName;
    QString strPhone;
    QString strPassword;
    QString strExtra;
};

struct RXTX_File{
    int         nType;
    QString     strFrom;
    QString     strTo;
    QString     strAmount;
    QString     strExpires;
    QString     strCreated;
};

#endif // GLOBAL_H
