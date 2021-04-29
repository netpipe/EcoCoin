#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#ifdef SOUND
#include <QMediaPlayer>
#endif
//#include "src/encryption/rsa/Rsa.h"
#include <QEvent>
#include <QThread>
#include <QDebug>
#include <QCryptographicHash>
#include <QGraphicsView>
#include <QTabWidget>
#include "src/downloadmanager.h"
#include "src/dbus/dbushandler.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void createUserTable();
    void insertUser();
    void selectUsers();
    void GenerateCoins1();
    void GenerateCoins2();
    void GenerateCoins3(int length,int total);
    void combinationUtil(QString arr, int n, int r, int index, QString data, int i);
    QString GetRandomString(int length,QString chars);
    QString GetReallyRandomString(int length,QString chars);
    QString getClientAddress();
    QString year;
    QString yeardb;//for extracting year from ID's
    QString usbpath;
    QString backupusbpath;
    int trycount;

    QString validatepassword;
    bool vpublickey;
    void createFreeCoinTable(QString DBname);
    int placeCoins(QString euserid,QString ammount);
    void generateRCoins();
    int checkAllCoins(QString db2);

    void createyearly(QString ownerID);
    void searchyearly(QString ownerID);
    void selectUsersCoins(QString userid,QString yeardb);
    void cleartablesusers();

    void serverusbtxrx();
    void clientusbtxrx();


    int md5verifydb();
    void generateCoins();
    void BackUptoUSB();
    int coini;
    bool gentotext;

    float version;
    void RandomizeCoins();
    void insertCoins();
    void createCoinTable(QString DBname);
    void themeInit();
    QList<QString> _coins;

    void ListUSB();
#ifdef SOUND
    QMediaPlayer* player;
#endif
    void SQLTest(QString dbname,QString Query);
    //coingenerator3
    int _count;
    int _total;
    int _length;
    bool _state;

    QString mainID;
    QString mainEKEY;
    QString mainAmmount;

    void createWalletTable(QString ID);
    void createWalletCoinsTable(QString ID);
    void WalletAddressInsert(QString Email,QString Name,QString classid,QString datetime,QString address);
    void walletCoinInsert(QString ID,QString CoinAddress,QString Owner,QString cid,QString date);
    void createWalletHistoryTable();
    void HistoryInsert(QString datetime,QString RXTX,QString ID,QString Ammount,QString contactname) ;
    void listwalletcoins(QString ID);
    QString WordListGenerator(int,QString);
    void createEmailTable();
    void EmailInsertWallet();
    void getEmailSettings();
    void writeAdminFrontendHashes();
    void writeWalletHashes();
    QString decodetxQR();

    QString generateRXfile(QString suserid,QString ruserid,QString etxcoins);
    QString generateTXfile(QString suserid,QString ruserid,QString etxcoins);
    int processRXTXfile(QString file);
    QString validateCOINsign(QString coin,QString euserID);
    float checkBalance(QString euserID);
    QString validateID(QString userid);
    int checkAvailableCoins(QString ID ,QString db,QString needed);
    int smtpsend(QString toemail,QString Message);
    #if DOWNLOAD
        void Download(QString URL);
        DownloadManager manager;
    #endif
    QString validateCOINsignWallet(QString ID,QString Coin);
    int pickupCoinsInsert(QString eownerID,QString Coin);
    int createPickupCoinTable(QString eownerID);

    int getkeys();
    void playsound(QString);
    void AddRemoveTab(QWidget *tab,QString,int tabname);
    QWidget * removedTab;
    void unCompress(QString filename , QString ofilename);
    void Compress(QString filename , QString ofilename);
int adminftp=0;



    //encryption
    QString encryptxor(QString test,QString key);
    QString decryptxor(QString string,QString key);

    QByteArray md5Checksum(QString stuff);
    QByteArray fileChecksum(const QString &fileName,QCryptographicHash::Algorithm hashAlgorithm);
    QString rot13( const QString & input );

    QString simplecrypt(QString string,QString key,QCryptographicHash::Algorithm hash);
    QString simpledecrypt(QString string,QString key,QCryptographicHash::Algorithm hash);

#ifdef ENCRYPTION
    Rsa *rsaTester;
    BigInt m_e, m_n;
    QString aesKey;

#endif
    QString currentUser;
    bool admin=false;
    QString masterkey;
    QString coinkey;
int tabindex;
bool walletexists;
    QString fileName;
    bool loaded=false; //theme


    QString encdec(QString ,int );
    QString encdec2(QString ,int );
    #ifdef ENCRYPTION
    QString rsaenc(QString input, Rsa *rsa = NULL);
    QString rsadec(QString input, Rsa *rsa);
    #endif
    QByteArray aesenc(QString input,QString,QString);
    QString aesdec(QByteArray input,QString,QString);

    QByteArray EncryptMsg(QString plainMsg,QString aeskey1,QString aeskey2);
    #ifdef ENCRYPTION
    QString DecryptMsg(QByteArray encryptedMsg, Rsa *rsa,QString aeskey1,QString aeskey2);
#endif
    void GenerateQRCode(QString data,QGraphicsView *view);
    void EAN13(QString productname,QString country,QString ean,QGraphicsView *graphicsView);
    QString decodeqr(QString image);


private slots:
    void on_pushButtonInsertUser_clicked();

    void on_gencoininfo_btn_clicked();

    void on_actionOpenCoin_triggered();

    void on_pushButton_clicked();

    void on_actionSyncUSB_triggered();

    void on_actionExit_triggered();

    void on_btnApply_clicked();

    void on_btnAddThemeFromFile_clicked();

    void on_btnRemoveThemeFromFile_clicked();

    void on_matureradio_yes_clicked();

    void on_encrypted_yes_clicked();

    void on_matureradio_no_clicked();

    void on_encrypted_no_clicked();

    void on_placeCoins_clicked();

    void on_SendCoins_clicked();

    void on_pushButton_2_clicked();

    void on_randomSearch_clicked();

    void on_test_clicked();

    void on_smtpsave_clicked();

    void on_smtptestmessage_clicked();

    void on_saveuserinfo_clicked();

    void on_placeCoinsopenfile_clicked();

    void on_validatecoins_clicked();

    void on_scantheme_clicked();

    void on_cmbTheme_currentIndexChanged(const QString &arg1);

    void on_GenerateRequest_clicked();

    void on_coinsrefresh_clicked();

    void on_walletCreateAddress_clicked();

    void on_receivesaveqr_clicked();

    void on_sendSaveqr_clicked();

    void on_generatetx_clicked();

    void on_userssearch_clicked();


    void on_balancetest_clicked();

    void on_CreateWallet_clicked();

    void on_usergenerateQr_clicked();

    void on_addresssearch_clicked();

    void on_addUserQR_clicked();

    void on_adminmode_clicked();

    void on_ftpserver_clicked();

    void on_CheckAvailability_clicked();

    void on_updateVerify_clicked();
    void          remoteCall(QByteArray message);
    void          remoteCall(QString message);

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
    QSqlDatabase coinDB;

    DBusHandler *m_hDbus;
};

#endif // MAINWINDOW_H
