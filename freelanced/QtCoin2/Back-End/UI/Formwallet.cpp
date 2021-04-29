#include <QFileDialog>
#include "Formwallet.h"
#include "ui_Formwallet.h"

FormWallet::FormWallet(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormWallet)
{
    ui->setupUi(this);
    // init variales
    m_Engine = Common::instance()->g_Engine;
    // init UI
    InitUI();
    // set default tab
    ui->tabWidget->setCurrentIndex(0);
}

FormWallet::~FormWallet()
{
    delete ui;
    delete m_pFTPUploader;

}
/*
 ***********************************************************************************************
                                    initial functions
 ***********************************************************************************************
*/
void FormWallet::InitUI()
{
    ui->txt_ftp_host->setText(m_Engine->m_FTPInfo.strHost);
    ui->txt_ftp_port->setText(QString("%1").arg(m_Engine->m_FTPInfo.nPort));
    ui->txt_ftp_username->setText(m_Engine->m_FTPInfo.strUser);
    ui->txt_ftp_password->setText(m_Engine->m_FTPInfo.strPass);

    ui->snd_txt_fromAmount->setValidator( new QIntValidator(0, 99999, this) );

    m_pFile = nullptr;
    m_pFTPUploader = new QNetworkAccessManager(this);
    connect(m_pFTPUploader, &QNetworkAccessManager::finished, this, &FormWallet::uploadFinished);
    m_pProgressDlg = new FrmProgressDlg(this);
    connect(&m_WaitTimer, &QTimer::timeout, this, &FormWallet::on_WaitTimeout);

    m_nUploadState = 0;
    m_nWaitCounter = 0;

}
/*
 ***********************************************************************************************
                                    Send Page
 ***********************************************************************************************
*/
void FormWallet::on_snd_btn_GenerateTX_clicked()
{
    QString frmEmail = ui->snd_txt_fromUserEmail->text();
    QString frmAmount = ui->snd_txt_fromAmount->text();
    QString toEmail = ui->snd_txt_toUserEmail->text();
    if(frmEmail.length() == 0)
    {
        Common::MessageBox("Input valid from email address");
        return;
    }
    if(frmAmount.length() == 0)
    {
        Common::MessageBox("Input valid amount");
        return;
    }
    if(frmEmail == toEmail)
    {
        Common::MessageBox("Send email and Recv email are same.");
        return;
    }

    QString txtPath;
    txtPath = QFileDialog::getSaveFileName(this,  "Save TX", "",  "SaveRX/TX File (*.txt);;All Files (*)");

    if(txtPath.length() == 0)
        return;
    m_Engine->generateTxFile(txtPath, frmEmail, frmAmount, toEmail);
    Common::MessageBox(QString("%1 created.").arg(txtPath));

}
void FormWallet::on_snd_btn_SendCoins_clicked()
{
    QString txtPath;
    txtPath = QFileDialog::getOpenFileName(this,  "Read TX", "",  "SaveRX/TX File (*.txt);;All Files (*)");
    if(txtPath.length() == 0)
        return;
    RXTX_File res = m_Engine->validateTxRXFile(txtPath);
    if(res.nType < 0)
    {
        Common::MessageBox("Invalid TX file!");
        return;
    }
    if(m_pProgressDlg->isVisible())
        return;
    QFile file(txtPath);
    file.open(QIODevice::ReadOnly|QIODevice::Text);
    QString strData = file.readAll().split(':')[1];
    file.close();
    m_tmpRTName = QString("tx_%1_%2").arg(strData).arg(m_Engine->GetRandomString(4,""));
    QUrl url(QString("ftp://%1/rxtx/%2").arg(m_Engine->m_FTPInfo.strHost).arg(m_tmpRTName));
    url.setUserName(m_Engine->m_FTPInfo.strUser);
    url.setPassword(m_Engine->m_FTPInfo.strPass);
    url.setPort(m_Engine->m_FTPInfo.nPort);

    m_pFile = new QFile(txtPath);
    if (m_pFile->open(QIODevice::ReadOnly))
    {
        // Start upload
        m_nUploadState = 1;
        QNetworkReply *reply = m_pFTPUploader->put(QNetworkRequest(url), m_pFile);
        // And connect to the progress upload signal
        connect(reply, &QNetworkReply::uploadProgress, this, &FormWallet::uploadProgress);
        m_pProgressDlg->open();
    }
}

void FormWallet::on_rcv_btn_ProcessRX_clicked()
{
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setOption(QFileDialog::ShowDirsOnly);
    dialog.exec();
    QString strPath = dialog.selectedFiles()[0];
    if (strPath.length() == 0)
    {
        return;
    }
    int nRes = m_Engine->ProcessRXTX(strPath);
    QString strRes = "";
    if (nRes == -1)
    {
        Common::MessageBox(QString("No %1 exists.").arg(CLIENT_FILE));
        return;
    }else if(nRes == -2)
    {
        Common::MessageBox(QString("Invalid %1 file.").arg(CLIENT_FILE));
        return;
    }
    if(nRes == -3)
    {
        strRes = QString("Not enough ").arg(CLIENT_FILE);
    }
    else if(nRes == 1)
    {
        strRes = QString("Processed successfully.");
    }
    else if(nRes == 2)
    {
        strRes = QString("No available on backend");
    }

}
/*
 ***********************************************************************************************
                                    FTP Page
 ***********************************************************************************************
*/

void FormWallet::on_btn_FTPUpdate_clicked()
{
    m_Engine->m_FTPInfo.strHost = ui->txt_ftp_host->text();
    m_Engine->m_FTPInfo.nPort = ui->txt_ftp_port->text().toInt();
    m_Engine->m_FTPInfo.strUser = ui->txt_ftp_username->text();
    m_Engine->m_FTPInfo.strPass = ui->txt_ftp_password->text();
    m_Engine->saveFTPInfo();
}
void FormWallet::uploadFinished(QNetworkReply *reply)
{
    if(m_nUploadState == 1)
    {
        if (!reply->error())
        {
            m_pFile->close();
            m_pFile->deleteLater();  // delete object of file
            reply->deleteLater();   // delete object of reply

            m_nWaitCounter = 0;
            m_WaitTimer.start(1000);
        }
        else
        {
            Common::MessageBox(QString("Can not upload:%1").arg(reply->error()));
            m_pProgressDlg->close();
        }
    }
    else if(m_nUploadState == 0)
    {
        if (!reply->error())
        {
            QString strRes = reply->readAll();
            strRes = strRes.replace("state:","");
            Common::MessageBox(strRes);
            reply->deleteLater();

            m_WaitTimer.stop();
            m_pProgressDlg->close();
        }
        else
        {
            m_WaitTimer.start();
            reply->deleteLater();
        }
    }
    else
    {
        m_nUploadState = 0;
        qDebug()<<reply->errorString();
    }
}
void FormWallet::uploadProgress(qint64 bytesSent, qint64 bytesTotal)
{
    if(bytesTotal > 0)
        m_pProgressDlg->SetProgress(bytesSent, bytesTotal);
}

void FormWallet::downloadFinished(QNetworkReply*reply)
{
    QString strContent = reply->readAll();
    qDebug()<<strContent;
    m_WaitTimer.stop();
    m_pProgressDlg->close();
}
void FormWallet::on_WaitTimeout()
{
    m_nWaitCounter += 1;
    try {
        QUrl url(QString("ftp://%1/rxtx/%2_ok").arg(m_Engine->m_FTPInfo.strHost).arg(m_tmpRTName));
        url.setUserName(m_Engine->m_FTPInfo.strUser);
        url.setPassword(m_Engine->m_FTPInfo.strPass);
        url.setPort(m_Engine->m_FTPInfo.nPort);
        m_nUploadState = 0;
        m_pFTPUploader->get(QNetworkRequest(url));
        m_WaitTimer.stop();
    } catch (exception e) {

    }

    if(m_nWaitCounter > 10)
    {
        m_WaitTimer.stop();
        m_pProgressDlg->close();
    }
}
