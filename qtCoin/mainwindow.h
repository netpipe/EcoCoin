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

private slots:
    void on_pushButtonInsertUser_clicked();

    void on_gencoininfo_btn_clicked();

    void on_actionOpenCoin_triggered();

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
};

#endif // MAINWINDOW_H
