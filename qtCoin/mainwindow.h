#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QMediaPlayer>
#include "src/encryption/rsa/Rsa.h"
#include <QEvent>
#include <QThread>
#include <QDebug>
#include <QCryptographicHash>

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
    QMediaPlayer*player;

    void SQLTest(QString dbname,QString Query);
    //coingenerator3
    int _count;
    int _total;
    int _length;
    bool _state;

void generateRXfile(QString suserid,QString ruserid,QString etxcoins);
void generateTXfile(QString suserid,QString ruserid,QString etxcoins);
int processRXTXfile(QString file);
QString validateCOINsign(QString coin,QString euserID);
float checkBalance(QString euserID);
QString validateID(QString userid);
int checkAvailableCoins(QString db,QString needed);
int smtpsend(QString toemail,QString Message);
int getkeys();
void playsound(QString);
    //encryption
    QString encryptxor(QString test,QString key);
    QString decryptxor(QString string,QString key);

    QByteArray md5Checksum(QString stuff);
    QByteArray fileChecksum(const QString &fileName,QCryptographicHash::Algorithm hashAlgorithm);
    QString rot13( const QString & input );

    QString simplecrypt(QString string,QString key,QCryptographicHash::Algorithm hash);
    QString simpledecrypt(QString string,QString key,QCryptographicHash::Algorithm hash);

    Rsa *rsaTester;
    BigInt m_e, m_n;
    QString aesKey;
    QString currentUser;

    QString masterkey;
    QString coinkey;

    QString fileName;
    bool loaded=false; //theme


    QString encdec(QString ,int );
    QString encdec2(QString ,int );
    QString rsaenc(QString input, Rsa *rsa = NULL);
    QString rsadec(QString input, Rsa *rsa);
    QByteArray aesenc(QString input,QString,QString);
    QString aesdec(QByteArray input,QString,QString);
    QByteArray EncryptMsg(QString plainMsg,QString aeskey1,QString aeskey2);
    QString DecryptMsg(QByteArray encryptedMsg, Rsa *rsa,QString aeskey1,QString aeskey2);

void GenerateQRCode(QString);
void EAN13(QString productname,QString country,QString ean);
QString decodeqr(QString image);
private slots:
    void on_pushButtonInsertUser_clicked();

    void on_gencoininfo_btn_clicked();

    void on_actionOpenCoin_triggered();

    void on_pushButton_clicked();

    void on_actionSyncUSB_triggered();

    void on_actionExit_triggered();

    void on_pushButton_3_clicked();

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

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
    QSqlDatabase coinDB;
};

#endif // MAINWINDOW_H
