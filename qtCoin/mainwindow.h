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
    QString GetRandomString(int length);
    QString GenerateClientAddress(int length);
    QString year;
void insertCoins();
void createCoinTable();
void themeInit();
QList<QString> _coins;

    void ListUSB();
    QMediaPlayer*player;
    Rsa *rsaTester;
    BigInt m_e, m_n;
    QString aesKey;
    QString currentUser;

    QString encdec(QString ,int );
    QString encdec2(QString ,int );
    QString rsaenc(QString input, Rsa *rsa = NULL);
    QString rsadec(QString input, Rsa *rsa);
    QByteArray aesenc(QString input,QString,QString);
    QString aesdec(QByteArray input,QString,QString);
    QByteArray EncryptMsg(QString plainMsg);
    QString DecryptMsg(QByteArray encryptedMsg, Rsa *rsa);

    //coingenerator3
    int _count;
    int _total;
    int _length;
    bool _state;


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

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
    QSqlDatabase coinDB;
};

#endif // MAINWINDOW_H
