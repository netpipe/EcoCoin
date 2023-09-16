#ifndef FORMWALLET_H
#define FORMWALLET_H

#include <QWidget>
#include <QTimer>
#include "../Engine/Common.h"
#include "../Engine/global.h"
#include "../Engine/Engine.h"
#include "UI/Frmprogressdlg.h"

namespace Ui {
class FormWallet;
}


class FormWallet : public QWidget
{
    Q_OBJECT

public:
    explicit FormWallet(QWidget *parent = nullptr);
    ~FormWallet();

    void    SetUserID(QString);
private:
    void    InitUI();
    void    EnableControls(bool);

public slots:
    // Send
    void    on_snd_btn_GenerateTX_clicked();
    void    on_snd_btn_SendCoins_clicked();

    // Receive
    void    on_rcv_btn_GenerateRX_clicked();
    void    on_rcv_btn_SendRX_clicked();
    void    on_rcv_btn_ReceiveCoins_clicked();

    // FTP
    void    on_btn_FTPUpdate_clicked();
    void    uploadFinished(QNetworkReply*);
    void    uploadProgress(qint64 bytesSent, qint64 bytesTotal);

    // Wallet
    void    on_sts_btn_Create_clicked();


signals:
    void    WalletUserInserted();
private:
    Ui::FormWallet *ui;

private:
    Engine* m_Engine;
    QNetworkAccessManager   *m_pFTPUploader;
    int                     m_nFTPState;
    QFile*  m_pFile;
    FrmProgressDlg          *m_pProgressDlg;
    QString                 m_tmpRTName;

    QString                 m_strCurrentUserID;
};

#endif // FORMWALLET_H
