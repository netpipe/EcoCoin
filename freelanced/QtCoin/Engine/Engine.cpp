#include "Engine.h"
#include "Engine/Common.h"
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

Engine::Engine()
{
    Common::instance()->g_Engine = this;
    m_nVersion = 1.0;

    m_ThemeManager = new StyleManager();
    m_DBManager = new DBManager();
    m_CoinManager = new CoinManager();
    m_DownloadManager = new DownloadManager();
    m_RTThread = nullptr;
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
    m_isAdmin = false;
    qDebug() << "Getting keys...";
    QString str_keyPath = "";

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
    try {

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
            file.close();
            bRes = true;
        }
    } catch (exception e) {
        qDebug()<<"Load Coin Settings Err: "<<e.what();
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
    loadKeys();
    // load FTP info
    loadFTPInfo();
}
void Engine::Release()
{
    if(m_RTThread != nullptr)
    {
        m_RTThread->m_bRun = false;
        m_RTThread->terminate();
    }

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

        m_RTThread = new CRTThread(pMainWidget, this);
        QObject::connect(m_RTThread, SIGNAL(processed(QString)), pMainWidget, SLOT(ProcessRXTX(QString)));
        m_RTThread->start();
        if(!loadCoinSettings())
        {
            Common::MessageBox("Can not load coin settings.");
        }
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
        qDebug() << "Generate RX err:" << e.what();
    }
}
bool Engine::validateTxFile(QString strPath)
{
    bool res = false;
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
            res = true;
        }
    } catch (exception e) {
        qDebug()<<e.what();
        res = false;
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
void Engine::ProcessRXTX(QString filename)
{
    QString strType = ""; //
    QString strID = "";
    try {
        strType = filename.split('_')[0];
        strID = filename.split('_')[1];
    } catch (exception e) {

    }
    if (strType == "" || strType == "")
    {
        RemoveRXTX(filename);
        return;
    }
    if(strType == "tx")
    {
        processTX(filename, strID);
    }else if(strType == "rx")
    {
        processRX(filename, strID);
    }
    else
    {
        RemoveRXTX(filename);
    }
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
    QString defaultPort = "8258";

    QString setting_path = QString("%1/%2").arg(QDir::currentPath()).arg(FTP_CLIENT_SETTING);
    QSettings settings(setting_path, QSettings::IniFormat);
    m_FTPInfo.strUser = settings.value("settings/username", "admin").toString();
    m_FTPInfo.strPass= settings.value("settings/password", "qt").toString();
    m_FTPInfo.nPort = settings.value("settings/port", 8258).toInt();
    m_FTPInfo.strHost = settings.value("settings/host", "127.0.0.1").toString();
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

void Engine::processTX(QString strFile, QString strId)
{
    QString strRes = "";
    QString fullPath = QString("%1/%2").arg(RXTX).arg(strFile);
    if(!validateTxFile(fullPath))
    {
        strRes = "state:MD5 checksum failed.";
        makeResult(strFile, strRes);
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
        makeResult(strFile, strRes);
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
        /*strRes = QString("state:%1").arg("send email and rec email are same.");
        makeResult(strFile, strRes);
        return;*/
    }
    // sender email check
    QString strSendID = GetUserIDFromEmail(strSender);
    if(strSendID.length() == 0)
    {
        strRes = QString("state:%1").arg("sender email does not exist.");
        makeResult(strFile, strRes);
        return;
    }
    QString strRecvID = "";
    // receiver email check
    if(strReceiver.length() > 0)
    {
        strRecvID = IsEmailExists(strReceiver);
        if(strRecvID.length() == 0)
        {
            strRes = QString("state:%1").arg("receiver email does not exist.");
            makeResult(strFile, strRes);
            return;
        }
    }
    // client-client transaction
    qDebug()<<strSender<<", "<<strReceiver<<", "<<nAmount;

    QString strSendYearID = simpledecrypt(strSendID, m_Key.strMasterKey, QCryptographicHash::Sha512);
    QString strSendDBID = simpledecrypt(strSendYearID, m_Key.strMasterKey, QCryptographicHash::Sha512);
    QString str_SendID_createdYear = simpledecrypt(strSendYearID, m_Key.strMasterKey, QCryptographicHash::Sha512).left(4);
    if(strReceiver.length() > 0)
    {
        float balance = checkClientBalance(str_SendID_createdYear, strSendYearID);
        if(balance < nAmount)
        {
            strRes = QString("state:%1").arg("no enough client balance.");
            makeResult(strFile, strRes);
            return;
        }
        qDebug() << "sender balance:" << balance;

        // placeCoins(result,"receive"); //send to rcoins
        // placeCoins(result2,ui->givecoinsammount->text()); //place into user account after validating id
    }
    // server-client transaction
    else
    {
        float balance = checkServerBalance();
        if(balance < nAmount)
        {
            strRes = QString("state:%1").arg("no enough server balance.");
            makeResult(strFile, strRes);
            return;
        }
        float coinVal = m_CoinInfo.strCoinValue.toFloat();
        int nCoins = (int)(nAmount / coinVal);

        QString strRes = RCoin2YearDB(nCoins, strSendID);
        makeResult(strFile, strRes);
        return;
    }

}
void Engine::processRX(QString strFile, QString strId)
{

}
QString Engine::RCoin2YearDB(int nCoins, QString strUserID)
{
    QString strRes;
    QString strUserDBID;
    QString strUserYearID = simpledecrypt(strUserID, m_Key.strMasterKey, QCryptographicHash::Sha512);
    QString str_UserID_createdYear = simpledecrypt(strUserYearID, m_Key.strMasterKey, QCryptographicHash::Sha512).left(4);
    // Take out from RCoin
    QString strQuery = QString("select ekey, userid from users where userid='%1'").arg(strUserID);
    QList<QList<QString>> dbData = m_DBManager->SelectData(DB_NAME, strQuery, 2);
    QString str_eKey = "";
    if(dbData.length() > 0)
    {
        str_eKey = dbData.at(0).at(0);
        strUserDBID = dbData.at(0).at(1);
    }
    if(str_eKey.length() == 0)
    {
        strRes = QString("state:%1").arg("cant get ekey.");
        return strRes;
    }
    // Sign-up with receiver user id and eKey
    strQuery = QString("SELECT origid, addr FROM coins ORDER BY random() LIMIT %1").arg(nCoins);
    dbData = m_DBManager->SelectData(RCOINS_DB_NAME, strQuery, 2);
    QVariantList coins;
    QVariantList origindex;
    QVariantList signedCoins;
    for(int i = 0; i < dbData.count(); i++)
    {
        QString coin = dbData.at(i).at(1);
        origindex<<dbData.at(i).at(0);
        coins<<coin;
        QString coin_signed = validateCOINsign( coin, strUserDBID, str_eKey);
        if(coin_signed.length() > 0)
            signedCoins<<coin_signed;
    }
    if(signedCoins.length() != coins.length())
    {
        strRes = QString("state:%1").arg("coin sign error");
        return strRes;
    }
    // put into users year db
    QString strDB = QString("./%1/%2.sqlite").arg(DB_DIR).arg(str_UserID_createdYear);
    strQuery = QString("INSERT INTO '%1'(origindex,addr,datetime,class,hold) VALUES (?,?,1,0,0)").arg(strUserYearID);
    QList<QVariantList> insertData;
    insertData.push_back(origindex);
    insertData.push_back(signedCoins);
    int nInserted = m_DBManager->InsertData3(strDB, strQuery, insertData, 2);
    if(nInserted != coins.length())
    {
        strRes = QString("state:%1").arg("transaction error");
        return strRes;
    }
    // delete from RCoins
    QString strOrg = "";
    for(int i = 0; i < nInserted-1; i++)
    {
        strOrg += QString("'%1',").arg(coins[i].toString());
    }
    strOrg += QString("'%1'").arg(coins[nInserted-1].toString());
    strQuery = QString("delete from coins where addr in (%1)").arg(strOrg);
    m_DBManager->ExecuteQuery(RCOINS_DB_NAME, strQuery);
    strRes = QString("state:%1").arg("ok");
    return strRes;
}
void Engine::makeResult(QString strFile, QString strContent)
{
    QString fullPath = QString("%1/%2_ok").arg(RXTX).arg(strFile);
    try {
        QFile file(fullPath);
        if(file.open(QIODevice::WriteOnly|QIODevice::Text))
        {
            file.write(strContent.toUtf8());
            file.close();
        }
    } catch (exception e) {
        qDebug() << "create rxtx response error" << e.what();
    }
}

QString Engine::validateCOINsign(QString strCoin, QString struserID, QString strEkey)
{
    QString ecoin = simplecrypt(strCoin,  m_Key.strCoinKey, QCryptographicHash::Sha512);
    QString ecoinuser = simplecrypt(struserID, ecoin, QCryptographicHash::Sha512);
    return simplecrypt(ecoinuser, strEkey, QCryptographicHash::Sha512);
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
    QString strQuery = "Select email from users;";
    QList<QList<QString>> dbData = m_DBManager->SelectData(DB_NAME, strQuery, 1);
    bool bRes = false;
    for(int i = 0; i < dbData.length(); i++)
    {
        if(dbData.at(i).at(0) == strEmail)
        {
            bRes = true;
            break;
        }
    }
    return bRes;
}
QString Engine::GetUserIDFromEmail(QString strEmail)
{
    QString strQuery = "Select email, userid from users;";
    QList<QList<QString>> dbData = m_DBManager->SelectData(DB_NAME, strQuery, 2);
    QString strID = "";
    for(int i = 0; i < dbData.length(); i++)
    {
        if(dbData.at(i).at(0) == strEmail)
        {
            strID = dbData.at(i).at(1);
            break;
        }
    }
    return strID;
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
