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
    if(!m_Engine->validateTxRXFile(txtPath))
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

void FormWallet::on_rcv_btn_GenerateRX_clicked()
{
    QString rcvEmail = ui->rcv_txt_fromUserID->text();
    QString rcvAmount = ui->rcv_txt_fromAmount->text();
    if(rcvEmail.length() == 0)
    {
        Common::MessageBox("Input valid recv email address");
        return;
    }
    if(rcvAmount.length() == 0)
    {
        Common::MessageBox("Input valid amount");
        return;
    }

    QString txtPath;
    txtPath = QFileDialog::getSaveFileName(this,  "Save RX", "",  "SaveRX/TX File (*.txt);;All Files (*)");

    if(txtPath.length() == 0)
        return;
    // m_Engine->generateRxFile(txtPath, rcvEmail, rcvAmount);
    Common::MessageBox(QString("%1 created.").arg(txtPath));
}
void FormWallet::on_rcv_btn_InputRX_clicked()
{
    QString txtPath;
    txtPath = QFileDialog::getOpenFileName(this,  "Read RX", "",  "ReadRX File (*.rx);;All Files (*)");
    if(txtPath.length() == 0)
        return;
    // validate
    if(!m_Engine->validateTxRXFile(txtPath))
    {
        Common::MessageBox("Invalid RX file!");
        return;
    }

    // parse the file
    QFile file(txtPath);
    QString fileData;
    if(file.open(QIODevice::ReadOnly))
    {
        QString strData = QLatin1String(file.readAll());
        int index = strData.indexOf("md5:");
        fileData = strData.left(index);
        file.close();
    }
    // confirm created time exists
    QStringList list = fileData.split(RTXT_DELIMITER);
    QString strNewPath = QString("%1/rx_%2").arg(RXTX).arg(list[1]);
    QFile::copy(txtPath, strNewPath);
    Common::MessageBox("Successfully accepted.");
}
void FormWallet::on_rcv_ReceiveCoins_clicked()
{
    m_Engine->SendSMTP("filipnikifirov001@gmail.com","Hi");
    /*QString txtPath;
    txtPath = QFileDialog::getOpenFileName(this,  "Read RX", "",  "SaveRX/TX File (*.txt);;All Files (*)");
    if(txtPath.length() == 0)
        return;
    if(!m_Engine->validateTxRXFile(txtPath))
    {
        Common::MessageBox("Invalid RX file!");
        return;
    }
    if(m_pProgressDlg->isVisible())
        return;
    QFile file(txtPath);
    file.open(QIODevice::ReadOnly|QIODevice::Text);
    QString strData = file.readAll().split(':')[1];
    file.close();
    m_tmpRTName = QString("rx_%1_%2").arg(strData).arg(m_Engine->GetRandomString(4,""));
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
    }*/
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
