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
#include "Src/encryption/simpleCrypt/simplecrypt.h"
#include "Src/QRCode/QrCode.hpp"

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

#define FTP_SETTING             "ftp.ini"
#define FTP_CLIENT_SETTING      "ftp_client.ini"
#define FTP_USER_SETTING        "ftp_users.txt"

#define COIN_TXT_NAME           "coins.txt"
#define HASH_TXT_NAME           "hashes.txt"
#define STRING_TEMPLATE         "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890"
#define COIN_SETTINGS           "settings.txt"
#define RXTX                    "rxtx"
#endif // GLOBAL_H
