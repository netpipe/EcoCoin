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
private:
    void    InitUI();

public slots:
    // Send
    void    on_snd_btn_GenerateTX_clicked();
    void    on_snd_btn_SendCoins_clicked();

    // Receive
    void    on_rcv_btn_ProcessRX_clicked();

    // FTP
    void    on_btn_FTPUpdate_clicked();
    void    uploadFinished(QNetworkReply*);
    void    uploadProgress(qint64 bytesSent, qint64 bytesTotal);
    void    downloadFinished(QNetworkReply*);

    void    on_WaitTimeout();
private:
    Ui::FormWallet *ui;

private:
    Engine* m_Engine;
    QNetworkAccessManager   *m_pFTPUploader;
    QFile*  m_pFile;
    FrmProgressDlg          *m_pProgressDlg;
    QString                 m_tmpRTName;

    QTimer                  m_WaitTimer;
    int                     m_nWaitCounter;
    int                     m_nUploadState;
};

#endif // FORMWALLET_H
