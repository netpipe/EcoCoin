#include "Engine.h"
#include "../Engine/Common.h"
#include <QDebug>
#include <QFile>
#include <QStorageInfo>
#include <QFileInfo>
#include <QFile>
#include <QMessageBox>
#include <QDate>
#include <QDir>


#include "Common.h"
#include "global.h"
#include "../Src/smtp/smtpclient.h"
#include "../Src/smtp/mimehtml.h"
#include "../Src/smtp/mimeinlinefile.h"

Engine::Engine()
{
    Common::instance()->g_Engine = this;
    m_nVersion = 1.0;
    m_isAdmin = false;
    m_ThemeManager = new StyleManager();
    m_DBManager = new DBManager();
    m_CoinManager = new CoinManager();
    m_DownloadManager = new DownloadManager();
    // m_RTThread = nullptr;
    m_FTPServer = nullptr;
    InitEngine();
}
/*
 ******************************************************
                    Private Functions
 ******************************************************
*/
QByteArray Engine::fileChecksum(const QString &fileName, QCryptographicHash::Algorithm hashAlgorithm)
{
    QFile f(fileName);
    if (f.open(QFile::ReadOnly))
    {
        QCryptographicHash hash(hashAlgorithm);
        if (hash.addData(&f))
        {
            return hash.result();
        }
    }
    return QByteArray();
}
QString Engine::StringChecksum(const QString data, QCryptographicHash::Algorithm hashAlgorithm)
{
    QString blah = QString(QCryptographicHash::hash(("myPassword"),QCryptographicHash::Md5).toHex());
    return blah;
}
QByteArray Engine::md5Checksum(QString strBytes)
{
    QCryptographicHash hash(QCryptographicHash::Md5); //QCryptographicHash::Md5
    hash.addData(strBytes.toLatin1());
    return hash.result();
}
bool Engine::loadKey(QString path)
{
    if(!QFileInfo(path).exists())
        return false;
    QFile f(path);
    QRegExp rx("[:]");
    QStringList lines;
    if (f.open(QIODevice::ReadOnly))
    {
       QTextStream in(&f);
       while (!in.atEnd())
       {
          QString line = in.readLine();
          if (line.contains(":"))
          {
              QStringList list = line.split(rx);
              if (list.count() < 2)
                  continue;
              lines.append(list.at(1).toLatin1());
          }
       }
       f.close();
    }
    if(lines.count() < 4)
        return false;
    m_Key.strMasterKey = lines.at(0);
    m_Key.strCoinKey = lines.at(1);
    m_Key.strMD5 = lines.at(2);
    m_Key.strHash = lines.at(3);


    QString checkSum = QString("masterkey:%1coinkey:%2coinsmd5:%3").arg(m_Key.strMasterKey).arg(m_Key.strCoinKey).arg((QString)fileChecksum(COIN_DB_NAME, QCryptographicHash::Md5));

    if (m_Key.strHash == md5Checksum(checkSum).toHex())
    {
        qDebug() << "md5's match";
        return true;
    }
    else
    {
        m_Key.strMasterKey = "";
        m_Key.strCoinKey = "";
        return false;
    }
}
void Engine::loadKeys()
{
    /*
    If keys are stored in the local folder, checkout then use those
    */

    qDebug() << "Getting keys...";
    QString str_keyPath = "";
#ifndef CLIENT_APP
    try {
        // Make candiates list
        QVector<QString> pathList = listUSBDrives();
        pathList.push_back(".");
        for(int i = 0; i <pathList.count(); i++)
        {
            bool bRes = loadKey(QString("%1/keys.txt").arg(pathList.at(i)));
            if(bRes)
            {
                m_isAdmin = bRes;
                break;
            }
        }
    } catch (exception e) {
        qDebug()<<e.what();
    }
#endif

}

QVector<QString> Engine::listUSBDrives()
{
    QVector<QString> res;
#ifdef STORAGE
    int nCount = 0;
    foreach (const QStorageInfo &storage, QStorageInfo::mountedVolumes())
    {
        qDebug() << storage.rootPath();
        QString usbstring = Common::USB_DriveName;
        if (storage.rootPath().contains(usbstring) && usbstring != "")
        {
            qDebug() << "yep found the drive match"
                     << "/n";
            res.push_back(storage.rootPath());

            if (storage.isReadOnly())
                qDebug() << "isReadOnly:" << storage.isReadOnly();
        }
        nCount += 1;
    }
    qDebug() << nCount <<" Scanned.";
#endif
    return res;
}
bool Engine::loadCoinSettings()
{
    bool bRes = false;

    QFile file(COIN_SETTINGS);
    if (file.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        QTextStream in (&file);
        QStringList listData;
        QString line;
        QRegExp rx("[:]");
        do {
            line = in.readLine();
            if (line.contains(":"))
            {
                listData.append(line.split(rx).at(1));
            }
        } while (!line.isNull());
        file.close();
        if(listData.length() >= 16)
        {
            m_CoinInfo.strCoinName = listData[0];
            m_CoinInfo.strCoinCount = listData[1];
            m_CoinInfo.strCoinLength = listData[2];
            m_CoinInfo.strCreateDay = listData[3];
            m_CoinInfo.strCreateMonth = listData[4];
            m_CoinInfo.strCreateYear = listData[5];
            m_CoinInfo.strCreateTime = listData[6];
            m_CoinInfo.strCoinValue = listData[7];
            m_CoinInfo.strMatures = listData[8];
            m_CoinInfo.strCoinpayout = listData[9];
            m_CoinInfo.strEncrypted = listData[10];
            m_CoinInfo.strMatureDate = listData[11];
            m_CoinInfo.strMatureMonth = listData[12];
            m_CoinInfo.strMatureYear = listData[13];
            m_CoinInfo.strMatureTime = listData[14];
            m_CoinInfo.strMaturedescription = listData[15];
            m_CoinInfo.strUsbDriveName = listData[16];
            bRes = true;
        }
    }

    return bRes;
}
void Engine::saveCoinSettings()
{
    try {
        QFile file(COIN_SETTINGS);
        if (file.open(QIODevice::ReadWrite | QIODevice::Text))
        {
            QTextStream stream(&file);
            stream << "CoinName:" << m_CoinInfo.strCoinName << '\n';
            stream << "CoinCount:" << m_CoinInfo.strCoinCount << '\n';
            stream << "CoinLength:" << m_CoinInfo.strCoinLength << '\n';
            stream << "CreateDay:" << m_CoinInfo.strCreateDay << '\n';
            stream << "CreateMonth: " << m_CoinInfo.strCreateMonth << '\n';
            stream << "CreateYear:" << m_CoinInfo.strCreateYear << '\n';
            stream << "CreateTime:" << m_CoinInfo.strCreateTime << '\n';
            stream << "CoinValue:" << m_CoinInfo.strCoinValue << '\n';
            stream << "Matures:" << m_CoinInfo.strMatures << '\n';
            stream << "Coinpayout:" << m_CoinInfo.strCoinpayout << '\n';
            stream << "Encrypted:" << m_CoinInfo.strEncrypted << '\n';
            stream << "MatureDate:" << m_CoinInfo.strMatureDate << '\n';
            stream << "MatureMonth:" << m_CoinInfo.strMatureMonth << '\n';
            stream << "MatureYear:" << m_CoinInfo.strMatureYear << '\n';
            stream << "MatureTime:" << m_CoinInfo.strMatureTime << '\n';
            stream << "Maturedescription:" << m_CoinInfo.strMaturedescription << '\n';
            stream << "UsbDriveName:" << m_CoinInfo.strUsbDriveName << '\n';
            file.close();
        }
    } catch (exception e) {
        qDebug()<<"Save Generate Info Err: "<<e.what();
    }
}
/*
 ******************************************************
                    Public Functions
 ******************************************************
*/
void Engine::InitEngine()
{
    // load theme list
    m_ThemeManager->initManager();

    // make default dirs
    QDir().mkdir("DB");
    // init variables
    m_Key.strMasterKey = "";
    m_Key.strCoinKey = "";
    // load key
#ifndef CLIENT_APP
    loadKeys();
    loadSMTPInfo();
#endif
#ifdef CLIENT_APP
    load_walletUsers();
#endif
    // load FTP info
    loadFTPInfo();
}
void Engine::Release()
{
    /*if(m_RTThread != nullptr)
    {
        m_RTThread->m_bRun = false;
        m_RTThread->terminate();
    }*/

}
void Engine::ClearCoinData()
{
    if (QMessageBox::question(0, "Are you sure ?", "Remove all tables/users?", QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes)
        return;
    try {
        int nYear = QDate::currentDate().year();
        QFile::remove(QString("./%1/%2.sqlite").arg(DB_DIR).arg(nYear));
        QFile::remove(COIN_DB_NAME);
        QFile::remove(DB_NAME);

        QFile::remove(AVAIL_COINS_DB_NAME);
        QFile::remove(RCOINS_DB_NAME);

    } catch (exception e) {

    }
}
void Engine::CreateCoinTable()
{
    try {
        QString strQuery = "CREATE TABLE IF NOT EXISTS coins("
                           "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                           "addr VARCHAR(50));";
        m_DBManager->ExecuteQuery(COIN_DB_NAME, strQuery);
        m_DBManager->ExecuteQuery(AVAIL_COINS_DB_NAME, strQuery);
        strQuery = "CREATE TABLE IF NOT EXISTS coins("
                                   "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                   "origid VARCHAR(50),"
                                   "addr VARCHAR(50));";
        m_DBManager->ExecuteQuery(RCOINS_DB_NAME, strQuery);
        updateHash();
    } catch (exception e) {

    }
}
void Engine::SetWidget(QWidget *pMainWidget)
{
    if(m_FTPServer != nullptr)
        return;
    if(m_isAdmin)
    {

        m_FTPServer = new FtpServer(pMainWidget);
        m_FTPServer->StartServer();
        if(!QDir(RXTX).exists())
        {
            QDir().mkdir(RXTX);
        }
        if(!QDir(WORKSPACE).exists())
        {
            QDir().mkdir(WORKSPACE);
        }

        /*m_RTThread = new CRTThread(pMainWidget, this);
        QObject::connect(m_RTThread, SIGNAL(processed(QString)), pMainWidget, SLOT(ProcessRXTX(QString)));
        m_RTThread->start();*/
    }
    if(!loadCoinSettings())
    {
        if(m_isAdmin)
            Common::MessageBox("Can not load coin settings.");
    }
}
/*
 ***********************************************************************************************
                                    Coin parts
 ***********************************************************************************************
*/
void Engine::GenerateCoin(int nLen, int nTotal)
{
    m_CoinManager->GenerateCoin(nLen, nTotal);
    QList<QString> coins = m_CoinManager->GetRawCoins();
    QList<QString> tmp_coins = coins;
    m_DBManager->InsertData(COIN_DB_NAME, "insert into coins(addr) values(?);", coins);
    QList<QString> rand_coins;
    QList<int>     rand_index;
    while(tmp_coins.count() > 0)
    {
        int nIndex = rand() % tmp_coins.count();
        rand_coins.push_back(tmp_coins.at(nIndex));
        rand_index.push_back(coins.indexOf(tmp_coins.at(nIndex)) + 1);
        tmp_coins.erase(tmp_coins.begin() + nIndex);
    }
    // geneate availCoins
    m_DBManager->InsertData(AVAIL_COINS_DB_NAME, "INSERT INTO coins(addr) VALUES (?)", rand_coins);
    // generate rCoins
    m_DBManager->InsertData2(RCOINS_DB_NAME, "INSERT INTO coins(origid,addr) VALUES (?,?)", rand_index, rand_coins);

    m_Key.strMasterKey = GetRandomString(12, STRING_TEMPLATE);
    m_Key.strCoinKey = GetRandomString(12, STRING_TEMPLATE);
}

bool Engine::SaveGeneratedCoinInfo()
{
    QVector<QString> pathList = listUSBDrives();

    QString path;
    if (pathList.size() > 0)
    {
        path = pathList.at(0) + "/keys.txt";
    }
    else
    {
        qDebug() << "local keys path set.";
        path = "./keys.txt";
    }
    QFile file2(path);
    if (file2.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        QTextStream stream(&file2);
        stream << "masterkey:" << m_Key.strMasterKey<< '\n';
        stream << "coinkey:" << m_Key.strCoinKey<< '\n';
        stream << "coinsmd5:" << fileChecksum(COIN_DB_NAME, QCryptographicHash::Md5).toHex() << '\n';
        QString checkSum = QString("masterkey:%1coinkey:%2coinsmd5:%3").arg(m_Key.strMasterKey).arg(m_Key.strCoinKey).arg((QString)fileChecksum(COIN_DB_NAME, QCryptographicHash::Md5));
        stream << "checksum:" << md5Checksum(checkSum).toHex() << '\n';
        file2.close();
    }
    return true;
}

void Engine::generateTxFile(QString strPath, QString frmEamil, QString frmAmount, QString toEmail)
{
    try {
        QFile file(strPath);
        if(file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
           QTextStream stream(&file);
           stream << "sender:" << frmEamil.toLatin1()<<":";  // sender
           stream << "receiver:" << toEmail.toLatin1()<<":";
           stream << "ammount:"<< frmAmount<<":"; // ammount

           QTime starttime(QTime::currentTime().hour(),QTime::currentTime().minute());
           QDate dNow(QDate::currentDate());
           stream << "datetime:"<< dNow.toString("dd.MM.yyyy")+"T"+starttime.toString("hh.mm.ss")<<":";
           file.close();
        }
        QByteArray md5 = fileChecksum(strPath, QCryptographicHash::Md5).toHex();
        if(file.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Append))
        {
            QTextStream stream(&file);
             stream << "md5:" << md5;
            file.close();
        }
    } catch (exception e) {
        qDebug() << "Generate TX err:" << e.what();
    }
}
/**********************************************
Function: generateRxFile
    Saves receive request file into file.
Params:
    strPath(QString):       full-path of file to save
    rcvID(QString):         receive from userID
    rcvAmount(QString):     receive amount
    strExpireTime(QString): expiring time

Format:
    rx:
    sender:
    reciver:
    amount:
    expire:
    created:
    md5:[sender to expire]
Returns:
    1: success
    0: failed
***********************************************/
int Engine::generateRxFile(QString strPath, QString fromID, QString toID, QString rxAmount, QString strExpireTime)
{
    int nRes = 0;
    try {
        QFile file(strPath);
        QDateTime curTime = QDateTime::currentDateTime();
        if(file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
           QTextStream stream(&file);
           stream << "rx"<<RTXT_DELIMITER;
           stream << fromID << RTXT_DELIMITER;
           stream << toID <<RTXT_DELIMITER;
           stream << rxAmount <<RTXT_DELIMITER;
           stream << strExpireTime << RTXT_DELIMITER;
           stream << curTime.toString("yyyy-MM-ddTHH:mm:ss");
           file.close();
        }
        QByteArray md5 = fileChecksum(strPath, QCryptographicHash::Md5).toHex();
        if(file.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Append))
        {
            QTextStream stream(&file);
            stream << "md5:" << md5;
            file.close();
        }
        nRes = 1;
    } catch (exception e) {
        qDebug() << "Generate RX err:" << e.what();
    }
    return nRes = 1;
}
/**********************************************
Function: validateTxRXFile
    check if Tx/Rx is made according to format.
Params:
    strPath(QString):       full-path of file to save

Returns:
    1: success
    0: failed
***********************************************/
RXTX_File Engine::validateTxRXFile(QString strPath)
{
    RXTX_File res;
    res.nType = -1;
    try {
        QFile file(strPath);
        QStringList fileData;
        QString strContent;
        QString strMD5;
        if(file.open(QIODevice::ReadOnly))
        {
            QString strData = QLatin1String(file.readAll());
            int index = strData.indexOf("md5:");
            strContent = strData.left(index);
            strMD5 = strData.mid(index + 4);
            file.close();
        }
        file.setFileName("temp");
        if(file.open(QIODevice::WriteOnly))
        {
            file.write(strContent.toUtf8());
            file.close();
        }
        QByteArray md5 = fileChecksum("temp", QCryptographicHash::Md5).toHex();
        if(md5 == strMD5)
        {
            QStringList strList = strContent.split(RTXT_DELIMITER);
            if(strList[0] == "rx")
            {
                res.nType = 0;
            }else if(strList[0] == "tx"){
                res.nType = 1;
            }
            if (res.nType >= 0)
            {
                res.strFrom = strList[1];
                res.strTo = strList[2];
                res.strAmount = strList[3];
                res.strExpires = strList[4];
                res.strCreated = strList[5];
            }

        }
        QFile tmp("temp");
        tmp.remove();
    } catch (exception e) {
        qDebug()<<e.what();
    }
    return res;
}
void Engine::RemoveRXTX(QString filename)
{
    try {
        QFile file(QString("%1/%2").arg(RXTX).arg(filename));
        file.remove();
    } catch (exception e) {
        qDebug() << "File delete error" << e.what();
    }
}
int Engine::ProcessRXTX(QString strDirPath)
{
    // check if 'client' file exist
    QString strRXTX = QString("%1/%2").arg(strDirPath).arg(CLIENT_FILE);
    if(!QFileInfo(strRXTX).exists())
    {
        return -1;
    }
    RXTX_File res = validateTxRXFile(strRXTX);
    if(res.nType < 0)
    {
        return -2;
    }
    if(res.nType == 0)
    {
        return processRX(res);
    }else if(res.nType == 1)
    {
        // processTX(filename);
    }
    return 0;
}
float Engine::checkClientBalance(QString strYear, QString strID)
{
    float balance = 0;
    QString dbName = QString("./%1/%2.sqlite").arg(DB_DIR).arg(strYear);
    QString strQuery = QString("Select * from '%1'").arg(strID);
    int nCount = m_DBManager->SelectData(dbName, strQuery, 1).count();
    float fCoinValue = m_CoinInfo.strCoinValue.toFloat();
    balance = nCount * fCoinValue;
    return balance;
}

float Engine::checkServerBalance()
{
    float balance = 0;
    int nCount = m_DBManager->SelectData(RCOINS_DB_NAME, "Select * from coins", 1).count();
    float fCoinValue = m_CoinInfo.strCoinValue.toFloat();
    balance = nCount * fCoinValue;
    return balance;
}
QString Engine::GetRandomString(int length, QString chars)
{
    //https://stackoverflow.com/questions/18862963/qt-c-random-string-generation/18866593
    if (chars == "")
    {
        chars = STRING_TEMPLATE;
    }
    QString possibleCharacters(chars.toLatin1());
    const int randomStringLength = length; //12 // assuming you want random strings of 12 characters

    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());

    QString randomString;
    for (int i = 0; i < randomStringLength; ++i)
    {
        int index = qrand() % possibleCharacters.length();
        QChar nextChar = possibleCharacters.at(index);
        possibleCharacters.remove(index, 1); //line.replace(QString(nextChar), QString(""));
        randomString.append(nextChar);
    }
    return randomString;
}

bool Engine::validateUserID(QString userId)
{
    // can validate public encrypted and master encrypted ID's
    // also sets QString yeardb globally for other functions

    QString ekey;
    QString euserid;
    QString datetime;

    euserid = simplecrypt(userId, m_Key.strMasterKey, QCryptographicHash::Sha512);
    QList<QList<QString>> dbData = m_DBManager->SelectData(DB_NAME,
                                                          "SELECT userid FROM users",
                                                          1);
    QString yearId = simplecrypt(euserid, m_Key.strMasterKey, QCryptographicHash::Sha512);
    bool bExist = false;
    for(int i = 0; i < dbData.size(); i++)
    {
        if(yearId == dbData.at(i).at(0))
        {
            bExist = true;
            break;
        }
    }
    if(bExist)
        return bExist;
    QString strYearDb = QString("./%1/%2.sqlite").arg(DB_DIR).arg(userId.left(4));
    dbData = m_DBManager->SelectData(strYearDb,
                                                          "SELECT * FROM sqlite_master WHERE type='table'",
                                                          2);

    for(int i = 0; i < dbData.count(); i++)
    {
        QString user = dbData.at(i).at(1);
        if(user == euserid)
        {
            bExist = true;
            break;
        }
    }
    return bExist;
}

QString Engine::simplecrypt(QString string, QString key, QCryptographicHash::Algorithm hash)
{
    QByteArray test = key.toUtf8();     // 0x0c2ad4a4acb9f023;
    uint test2 = test.toHex().toInt();

    SimpleCrypt crypto(test2);          // some random number
                                        // SimpleCrypt crypto(0x0c2ad4a4acb9f023);
    QString result = crypto.encryptToString(string, hash);
    return result;
}

QString Engine::simpledecrypt(QString string, QString key, QCryptographicHash::Algorithm hash)
{
    QByteArray test = key.toUtf8(); //0x0c2ad4a4acb9f023;
                                    //convert to hex
    uint test2 = test.toHex().toInt();

    SimpleCrypt crypto(test2); //some random number
                               //  SimpleCrypt crypto(0x0c2ad4a4acb9f023);
    QString decrypted = crypto.decryptToString(string, hash);
    return decrypted;
}

QString Engine::GetReallyRandomString(int length, QString chars)
{
    if (chars == "")
    {
        chars = STRING_TEMPLATE;
    }
    QString possibleCharacters(chars.toLatin1());
    const int randomStringLength = length;

    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());

    QString randomString;
    for (int i = 0; i < randomStringLength; ++i)
    {
        int index = qrand() % possibleCharacters.length();
        QChar nextChar = possibleCharacters.at(index);
        randomString.append(nextChar);
    }
    return randomString;
}
void Engine::updateHash()
{
    QByteArray coinsmd5 =  fileChecksum(COIN_DB_NAME, QCryptographicHash::Md5);
    QByteArray Rcoinsmd5 =  fileChecksum(RCOINS_DB_NAME, QCryptographicHash::Md5);
    QByteArray Acoinsmd5 =  fileChecksum(AVAIL_COINS_DB_NAME, QCryptographicHash::Md5);

    QFile hashfile(HASH_TXT_NAME);
    if(hashfile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream stream(&hashfile);
        stream << "coinsdb:" << coinsmd5.toHex() << endl;
        stream << "rcoinsdb:" << Rcoinsmd5.toHex() << endl;
        stream << "availableCoins:" << Acoinsmd5.toHex() << endl;
    }
    hashfile.close();
}

void Engine::loadFTPInfo()
{
    QString setting_path = QString("%1/%2").arg(QDir::currentPath()).arg(FTP_CLIENT_SETTING);
    QSettings settings(setting_path, QSettings::IniFormat);
    m_FTPInfo.strUser = settings.value("settings/username", "admin").toString();
    m_FTPInfo.strPass= settings.value("settings/password", "qt").toString();
    m_FTPInfo.nPort = settings.value("settings/port", 8258).toInt();
    m_FTPInfo.strHost = settings.value("settings/host", "127.0.0.1").toString();
}
void Engine::loadSMTPInfo()
{
    QString setting_path = QString("%1/%2").arg(QDir::currentPath()).arg(SMTP_SETTING);
    QSettings settings(setting_path, QSettings::IniFormat);
    m_SmtpInfo.strHost = settings.value("settings/host", "smtp.gmail.com").toString();
    m_SmtpInfo.nPort = settings.value("settings/port", 465).toInt();
    m_SmtpInfo.strEmail = settings.value("settings/email", "your_username@gmail.com").toString();
    m_SmtpInfo.strPass = settings.value("settings/password", "").toString();
    m_SmtpInfo.bEncrypted = settings.value("settings/encrypted", true).toBool();
}
void Engine::saveFTPInfo()
{
    QString setting_path = QString("%1/%2").arg(QDir::currentPath()).arg(FTP_CLIENT_SETTING);
    QSettings settings(setting_path, QSettings::IniFormat);
    settings.setValue("settings/username", m_FTPInfo.strUser);
    settings.setValue("settings/password", m_FTPInfo.strPass);
    settings.setValue("settings/port", m_FTPInfo.nPort);
    settings.setValue("settings/host", m_FTPInfo.strHost);
}
void Engine::saveSMTPInfo()
{
    QString setting_path = QString("%1/%2").arg(QDir::currentPath()).arg(SMTP_SETTING);
    QSettings settings(setting_path, QSettings::IniFormat);
    settings.setValue("settings/host", m_SmtpInfo.strHost);
    settings.setValue("settings/port", m_SmtpInfo.nPort);
    settings.setValue("settings/email", m_SmtpInfo.strEmail);
    settings.setValue("settings/password", m_SmtpInfo.strPass);
    settings.setValue("settings/encrypted", m_SmtpInfo.bEncrypted);
}
void Engine::processTX(QString strFile)
{
    QString strRes = "";
    QString fullPath = QString("%1/%2").arg(RXTX).arg(strFile);
    if(validateTxRXFile(fullPath).nType < 0)
    {
        strRes = "state:MD5 checksum failed.";
        return;
    }
    QFile file(fullPath);
    QString fileData;
    if(file.open(QIODevice::ReadOnly))
    {
        QString strData = QLatin1String(file.readAll());
        int index = strData.indexOf("md5:");
        fileData = strData.left(index);
        file.close();
    }
    QStringList list = fileData.split(":");
    QString strSender, strReceiver;
    int nAmount;
    QDateTime requestTime;
    bool bParsed = false;
    try {
        strSender = list[1];
        strReceiver = list[3];
        nAmount = list[5].toInt();
        requestTime = QDateTime::fromString(list[7], "dd.MM.yyyyTHH.mm.ss");
        bParsed = true;
    } catch (exception e) {
        qDebug()<<"Parse TX file err"<<e.what();
        strRes = QString("state:%1").arg(e.what());
    }
    // if failed to parse return
    if(!bParsed)
    {
        return;
    }
    QDateTime curTime = QDateTime::currentDateTime();
    int elapsedSec = requestTime.secsTo(curTime);
    // if 5 secs had paased, ignore it.
    if(elapsedSec > 5)
    {
        /*strRes = QString("state:%1").arg("too long from request");
        makeResult(strFile, "strRes");
        return;*/
    }
    if(strSender == strReceiver)
    {
        strRes = QString("state:%1").arg("send email and rec email are same.");
        return;
    }
    // sender email check
    QString strSendID = "";
    if(strSendID.length() == 0)
    {
        strRes = QString("state:%1").arg("sender email does not exist.");
        return;
    }
    QString strRecvID = "";
    // receiver email check
    if(strReceiver.length() > 0)
    {
        strRecvID = ""; // GetUserIDFromEmail(strReceiver);
        if(strRecvID.length() == 0)
        {
            strRes = QString("state:%1").arg("receiver email does not exist.");
            return;
        }
    }
    // client-client transaction
    qDebug()<<strSender<<", "<<strReceiver<<", "<<nAmount;

    QString strSendYearID = simpledecrypt(strSendID, m_Key.strMasterKey, QCryptographicHash::Sha512);
    float coinVal = m_CoinInfo.strCoinValue.toFloat();
    int nCoins = (int)(nAmount / coinVal);
    if(strReceiver.length() > 0)
    {
        QString strSendDBID = simpledecrypt(strSendYearID, m_Key.strMasterKey, QCryptographicHash::Sha512);
        QString str_SendID_createdYear = simpledecrypt(strSendYearID, m_Key.strMasterKey, QCryptographicHash::Sha512).left(4);
        float balance = checkClientBalance(str_SendID_createdYear, strSendYearID);
        if(balance < nAmount)
        {
            strRes = QString("state:%1").arg("no enough client balance.");
            return;
        }
        qDebug() << "sender balance:" << balance;

        QString strRes = YearDB2RCoin(nCoins, strSendID);
        if(strRes.indexOf(RXTX_RESSTR)< 0)
        {
            return;
        }
        // strRes = RCoin2YearDB(nCoins, strRecvID);
        return;
    }
    // server-client transaction
    else
    {
        float balance = checkServerBalance();
        if(balance < nAmount)
        {
            strRes = QString("state:%1").arg("no enough server balance.");
            return;
        }
        // strRes = RCoin2YearDB(nCoins, strSendID);
    }
}
int Engine::processRX(RXTX_File rxFile)
{
    float balance = checkServerBalance();
    int nAmount = rxFile.strAmount.toInt();
    if(balance < nAmount)
    {
        return -3;
    }
    float coinVal = m_CoinInfo.strCoinValue.toFloat();
    int nCoins = (int)(nAmount / coinVal);
    if(rxFile.strTo != "")
    {
        return 2;
    }
    RCoin2YearDB(nCoins, rxFile.strFrom);
    return 1;

}
QString Engine:: YearDB2RCoin(int nCoins, QString strUserID)
{
    QString strRes;
    QString strUserDBID;
    QString strUserYearID = simpledecrypt(strUserID, m_Key.strMasterKey, QCryptographicHash::Sha512);
    QString str_UserID_createdYear = simpledecrypt(strUserYearID, m_Key.strMasterKey, QCryptographicHash::Sha512).left(4);
    // Get User info from DB
    QString strQuery = QString("select ekey, userid from users where userid='%1'").arg(strUserID);
    QList<QList<QString>> dbData = m_DBManager->SelectData(DB_NAME, strQuery, 2);
    QString str_eKey = "";
    if(dbData.length() > 0)
    {
        str_eKey = dbData.at(0).at(0);
        strUserDBID = dbData.at(0).at(1);
    }

    QVariantList coins;
    QVariantList origindex;
    QVariantList signedCoins;

    // get coins from users year db
    QString strDB = QString("./%1/%2.sqlite").arg(DB_DIR).arg(str_UserID_createdYear);
    strQuery = QString("SELECT origindex, addr FROM '%1' ORDER BY random() LIMIT %2").arg(strUserYearID).arg(nCoins);
    dbData = m_DBManager->SelectData(strDB, strQuery, 2);
    QString strOrg = "";
    for(int i = 0; i < dbData.count(); i++)
    {
        QString ori = dbData[i][0];
        QString signed_coin = dbData[i][1];
        signedCoins<<signed_coin;
        origindex<<dbData[i][0];
        QString coin = validateCOINunsign( signed_coin, strUserDBID, str_eKey);
        coins<<coin;
        strOrg += QString("'%1',").arg(ori);
    }
    strOrg = strOrg.left(strOrg.length() - 1);

    strQuery = QString("delete from '%1' where origindex in (%2)").arg(strUserYearID).arg(strOrg);
    m_DBManager->ExecuteQuery(strDB, strQuery);
    strRes = QString("state:%1").arg(RXTX_RESSTR);
    return strRes;
}
QStringList Engine::RCoin2YearDB(int nCoins, QString strUserYearID)
{
    QString strRes;
    QString str_UserID_createdYear = strUserYearID.left(4);
    QString strUserID = simplecrypt(strUserYearID, m_Key.strMasterKey);
    strUserID = simplecrypt(strUserID, m_Key.strMasterKey);
    // Get User info from DB
    QString strQuery = QString("select ekey from users where userid='%1'").arg(strUserID);
    QList<QList<QString>> dbData = m_DBManager->SelectData(DB_NAME, strQuery, 2);

    QStringList processedCoins;
    QString str_eKey = "";
    if(dbData.length() == 0)
    {
        return processedCoins;
    }
    str_eKey = dbData.at(0).at(0);

    // Sign-up with receiver user id and eKey
    strQuery = QString("SELECT origid, addr FROM coins ORDER BY random() LIMIT %1").arg(nCoins);
    dbData = m_DBManager->SelectData(RCOINS_DB_NAME, strQuery, 2);
    // QVariantList coins;
    QVariantList origindex;
    QVariantList signedCoins;
    for(int i = 0; i < dbData.count(); i++)
    {
        QString coin = dbData.at(i).at(1);
        origindex<<dbData.at(i).at(0);
        processedCoins<<coin;
        QString coin_signed = validateCOINsign( coin, strUserID, str_eKey);
        if(coin_signed.length() > 0)
            signedCoins<<coin_signed;
    }
    if(signedCoins.length() != processedCoins.length())
    {
        processedCoins.clear();
        return processedCoins;
    }

    // put into users year db
    QString strDB = QString("./%1/%2.sqlite").arg(DB_DIR).arg(str_UserID_createdYear);
    strQuery = QString("INSERT INTO '%1'(origindex,addr,datetime,class,hold) VALUES (?,?,1,0,0)").arg(strUserYearID);
    QList<QVariantList> insertData;
    insertData.push_back(origindex);
    insertData.push_back(signedCoins);
    int nInserted = m_DBManager->InsertData3(strDB, strQuery, insertData, 2);
    if(nInserted != processedCoins.length())
    {
        processedCoins.clear();
        return processedCoins;
    }
    // delete from RCoins
    QString strOrg = "";
    for(int i = 0; i < nInserted-1; i++)
    {
        strOrg += QString("'%1',").arg(processedCoins[i]);
    }
    strOrg += QString("'%1'").arg(processedCoins[nInserted-1]);
    strQuery = QString("delete from coins where addr in (%1)").arg(strOrg);
    m_DBManager->ExecuteQuery(RCOINS_DB_NAME, strQuery);

    return processedCoins;
}


QString Engine::validateCOINsign(QString strCoin, QString struserID, QString strEkey)
{
    QString ecoin = simplecrypt(strCoin,  m_Key.strCoinKey);
    QString ecoinuser = simplecrypt(ecoin, struserID);
    return simplecrypt(ecoinuser, strEkey);
}
QString Engine::validateCOINunsign(QString strSignedCoin, QString struserID, QString strEkey)
{
    QString ecoinuser = simpledecrypt(strSignedCoin,  strEkey);
    QString ecoin = simpledecrypt(ecoinuser, struserID);
    return simpledecrypt(ecoin, m_Key.strCoinKey);
}
/*
 ***********************************************************************************************
                                    Wallet management
 ***********************************************************************************************
*/
void Engine::load_walletUsers()
{
    /*
    loads sqlite and lists table names into m_WalletUsers
    */
    m_WalletUsers.clear();
    try {
        if(!QFileInfo(WALLET_DB).exists())
            return;
        QString strQuery = "select userid from users";
        QList<QList<QString>> dbData = m_DBManager->SelectData(WALLET_DB, strQuery, 1);
        for(int i = 0; i < dbData.size(); i++)
        {
            m_WalletUsers.append(dbData[i][0]);
        }

    } catch (exception e) {
        qDebug() << "Load Wallet error:" << e.what();
    }
}
int Engine::InsertNewWalletUser(WalletUser user)
{
    /*
    insert new user into wallet if it is not exist
    only available for CLIENT

    Return value:
        1: Success
        0: Exists already
        -1: Failed, DB Error
    */
    if(!QDir(WALLET_DB).exists())
    {
        createWalletDB();
    }
    int nRes = -1;
    try {
        QString strQuery = QString("select userid from users where userid='%1'").arg(user.strID);
        QList<QList<QString>> dbData = m_DBManager->SelectData(WALLET_DB, strQuery, 1);
        if(dbData.length() > 0)
        {
            nRes = 0;
            return 0;
        }
        strQuery = QString("insert into users(userid, name, password, phone, extra) values('%1','%2','%3','%4','%5');")
                .arg(user.strID)
                .arg(user.strName)
                .arg(user.strPassword)
                .arg(user.strPhone)
                .arg(user.strExtra);
       m_DBManager->ExecuteQuery(WALLET_DB, strQuery);
       strQuery = QString("select userid from users where userid='%1'").arg(user.strID);
       dbData = m_DBManager->SelectData(WALLET_DB, strQuery, 1);
       if(dbData.length() > 0)
       {
           nRes = 1;
           load_walletUsers();
       }
    } catch (exception e) {
        qDebug()<<"Insert new wallet user error:"<<e.what();
        nRes = -1;
    }
    return nRes;
}
void Engine::createWalletDB()
{
    /*
    creates new wallet sqlite
    */
    try {
        QString strQuery = "CREATE TABLE IF NOT EXISTS users("
                            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                            "userid VARCHAR(100),"
                            "name VARCHAR(100),"
                            "password VARCHAR(100),"
                            "phone VARCHAR(100),"
                            "extra VARCHAR(100));";
        m_DBManager->ExecuteQuery(WALLET_DB, strQuery);
    } catch (exception e) {
        qDebug()<<"Create wallet db error:"<<e.what();
    }
}

/*
 ***********************************************************************************************
                                    STMP management
 ***********************************************************************************************
*/
int Engine::SendSMTP(QString strEmail, QString strMsg)
{
#ifdef SMTP
    bool ssl = m_SmtpInfo.bEncrypted;

    SmtpClient smtp (m_SmtpInfo.strHost, m_SmtpInfo.nPort, ssl ? SmtpClient::SslConnection : SmtpClient::TcpConnection);

    smtp.setUser(m_SmtpInfo.strEmail);
    smtp.setPassword(m_SmtpInfo.strPass);

    MimeMessage message;
    EmailAddress sender(m_SmtpInfo.strEmail, m_SmtpInfo.strEmail); //email,name
    message.setSender(&sender);
    EmailAddress to(strEmail, strEmail); // email, name
    message.addRecipient(&to);
    message.setSubject(m_CoinInfo.strCoinName);

    MimeHtml html;

    html.setHtml("<h1>"+m_CoinInfo.strCoinName+ "</h1>"
                 "<h2>"+strMsg+"</h2>"
                 "<img src='cid:image1' />"
                 "<h2> This is the second image </h2>"
                 "test");

    // Create a MimeInlineFile object for each image
    MimeInlineFile image1 (new QFile("Resource/qtcoin.png"));

    // An unique content id must be setted
    image1.setContentId("image1");
    image1.setContentType("image/png"); //"image/jpg"   //text/plain  // application/octet-stream

    MimeInlineFile image2 (new QFile("Resource/ecocoin.png"));
    image2.setContentId("image2");
    image2.setContentType("image/png");

    message.addPart(&html);
    message.addPart(&image1);
    message.addPart(&image2);

    // Now we can send the mail
    if (!smtp.connectToHost()) {
        QMessageBox Msgbox;
            Msgbox.setText("Failed to connect to host.");
            Msgbox.exec();
        return 1;
    }

    if (!smtp.login()) {
        QMessageBox Msgbox;
            Msgbox.setText("not able to login");
            Msgbox.exec();
        return 2;
    }

    if (!smtp.sendMail(message)) {
        QMessageBox Msgbox;
            Msgbox.setText("Failes to send mail.");
            Msgbox.exec();
        return 3;
    }
    smtp.quit();
    Common::MessageBox("Email Sent");
#endif
}
/*
 ***********************************************************************************************
                                    User management
 ***********************************************************************************************
*/
QString Engine::GetValidUserID(QStringList usrInfo)
{
    bool bExist = true;
    QString userID;
    int nYear = QDate::currentDate().year();
    do{
        userID = QString("%1%2").arg(nYear).arg(GetRandomString(8, STRING_TEMPLATE));
        bExist = validateUserID(userID);
    }while(bExist);
    QString crypted2 = simplecrypt(userID, m_Key.strMasterKey, QCryptographicHash::Sha512);

    // create yearly

    QString db_path = QString("./%1/%2.sqlite").arg(DB_DIR).arg(nYear);
    QString strQuery = QString("CREATE TABLE IF NOT EXISTS "
                               "'%1'"
                                 "("
                                 "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                 "origindex VARCHAR(100),"
                                 "addr VARCHAR(100),"
                                 "datetime INTEGER,"
                                 "class INTEGER,"
                                 "hold INTEGER"
                                 ");").arg(crypted2);
    m_DBManager->ExecuteQuery(db_path, strQuery);
    crypted2 = simplecrypt(crypted2, m_Key.strMasterKey, QCryptographicHash::Sha512);
    // insert user
    m_DBManager->CreateUserTable();
    strQuery = QString("INSERT INTO users("
                               "email,"
                               "userid,"
                               "name,"
                               "password,"
                               "phone,"
                               "datetime,"
                               "ekey,"
                               "total,"
                               "extra,"
                               "class)"
                               "VALUES("
                               "'%1','%2','%3','%4','%5','%6','%7','%8', '%9','%10');")\
            .arg(usrInfo.at(0))\
            .arg(crypted2)\
            .arg(usrInfo.at(1))\
            .arg(usrInfo.at(2))\
            .arg(usrInfo.at(3))\
            .arg(usrInfo.at(4))\
            .arg(GetReallyRandomString(8, ""))\
            .arg("")\
            .arg(usrInfo.at(5))\
            .arg(usrInfo.at(6));
    m_DBManager->ExecuteQuery(DB_NAME, strQuery);
    return userID;
}
bool Engine::IsEmailExists(QString strEmail)
{
    QString strQuery = QString("Select email from users where email='%1';").arg(strEmail);
    QList<QList<QString>> dbData = m_DBManager->SelectData(DB_NAME, strQuery, 1);
    bool bRes = dbData.length() > 0;
    return bRes;
}

/*
 ***********************************************************************************************
                                    FTP
 ***********************************************************************************************
*/

void Engine::Download(QString URL)
{
#if DOWNLOAD
    QUrl url{URL};
    m_DownloadManager->doDownload(url);
#endif
}
