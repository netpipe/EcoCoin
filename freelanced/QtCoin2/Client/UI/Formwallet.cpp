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
    // ~init UI

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
void FormWallet::SetUserID(QString strID)
{
    m_strCurrentUserID = strID;
    EnableControls(true);
}
void FormWallet::EnableControls(bool bEnable)
{
    ui->snd_btn_GenerateTX->setEnabled(bEnable);
    ui->snd_btn_SendCoins->setEnabled(bEnable);
    ui->rcv_btn_GenerateRX->setEnabled(bEnable);
    ui->rcv_btn_SendRX->setEnabled(bEnable);
    ui->rcv_btn_ReceiveCoins->setEnabled(bEnable);
}
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

    m_strCurrentUserID = "";

    // set default tab
    ui->tabWidget->setCurrentIndex(2);

    EnableControls(false);

    QDateTime curTime = QDateTime::currentDateTime().addDays(1);
    ui->rcv_spDay->setValue(curTime.date().day());
    ui->rcv_spMonth->setCurrentIndex(curTime.date().month()-1);
    ui->rcv_spYear->setValue(curTime.date().year());
    ui->rcv_spTime->setTime(curTime.time());
}
/*
 ***********************************************************************************************
                                    Send Page
 ***********************************************************************************************
*/
void FormWallet::on_snd_btn_GenerateTX_clicked()
{
    QString frmID = ui->snd_txt_fromUserID->text();
    QString frmAmount = ui->snd_txt_fromAmount->text();
    QString toID = ui->snd_txt_toUserID->text();
    if(frmID.length() == 0)
    {
        Common::MessageBox("Input valid from email address");
        return;
    }
    if(frmAmount.length() == 0)
    {
        Common::MessageBox("Input valid amount");
        return;
    }
    if(frmID == toID)
    {
        Common::MessageBox("Send email and Recv email are same.");
        return;
    }
    // if user to user, check out balance first
    if(toID.length() > 0)
    {

    }
    QString txtPath;
    txtPath = QFileDialog::getSaveFileName(this,  "Save TX", "",  "SaveRX/TX File (*.txt);;All Files (*)");

    if(txtPath.length() == 0)
        return;
    m_Engine->generateTxFile(txtPath, frmID, frmAmount, toID);
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
        QNetworkReply *reply = m_pFTPUploader->put(QNetworkRequest(url), m_pFile);
        // And connect to the progress upload signal
        connect(reply, &QNetworkReply::uploadProgress, this, &FormWallet::uploadProgress);
        m_pProgressDlg->open();
    }
}

void FormWallet::on_rcv_btn_GenerateRX_clicked()
{
    QString rcvID = ui->rcv_txt_fromUserID->text();
    QString rcvAmount = ui->rcv_txt_fromAmount->text();
    // receive amout shoulbe be above 0
    if(rcvAmount.length() == 0)
    {
        Common::MessageBox("Input valid amount");
        return;
    }
    QTime curTime = ui->rcv_spTime->time();
    QDate curDate = QDate(ui->rcv_spYear->value(), ui->rcv_spMonth->currentIndex()+1, ui->rcv_spDay->value());
    QDateTime expireTime = QDateTime(curDate, curTime);
    if (QDateTime::currentDateTime().secsTo(expireTime) < 60 * 24)
    {
        Common::MessageBox("Expired time should be at least one day.");
        return;
    }
    QString txtPath;
    txtPath = QFileDialog::getSaveFileName(this,  "Save RX", "",  "SaveRX(*.rx);;All Files (*)");

    if(txtPath.length() == 0)
        return;
    if (txtPath.indexOf('.') < 0)
        txtPath += ".rx";
    if(m_Engine->generateRxFile(txtPath, m_strCurrentUserID, rcvID, rcvAmount, expireTime.toString("yyyy-MM-ddTHH:mm:ss")) == 1)
        Common::MessageBox(QString("Successfuly created as %1.").arg(txtPath));
    else
        Common::MessageBox(QString("Can not created"));
}
void FormWallet::on_rcv_btn_SendRX_clicked()
{
    QString txtPath;
    txtPath = QFileDialog::getOpenFileName(this,  "Read RX", "",  "ReadRX File (*.rx);;All Files (*)");
    if(txtPath.length() == 0)
        return;
    if(!m_Engine->validateTxRXFile(txtPath))
    {
        Common::MessageBox("Invalid RX file!");
        return;
    }
    QString strUrl = QString("rx_%1").arg(m_strCurrentUserID);
    QUrl url(QString("ftp://%1/%2/%3").arg(m_Engine->m_FTPInfo.strHost).arg(RXTX).arg(strUrl));
    url.setUserName(m_Engine->m_FTPInfo.strUser);
    url.setPassword(m_Engine->m_FTPInfo.strPass);
    url.setPort(m_Engine->m_FTPInfo.nPort);
    m_pFile = new QFile(txtPath);
    if (m_pFile->open(QIODevice::ReadOnly))
    {
        // Start upload
        m_nFTPState = PUT;
        QNetworkReply *reply = m_pFTPUploader->put(QNetworkRequest(url), m_pFile);
        // And connect to the progress upload signal
        connect(reply, &QNetworkReply::uploadProgress, this, &FormWallet::uploadProgress);
        m_pProgressDlg->open();
    }
}
void FormWallet::on_rcv_btn_ReceiveCoins_clicked()
{
    QUrl url(QString("ftp://%1/%2/%3").arg(m_Engine->m_FTPInfo.strHost).arg(WORKSPACE).arg(m_tmpRTName));
    url.setUserName(m_Engine->m_FTPInfo.strUser);
    url.setPassword(m_Engine->m_FTPInfo.strPass);
    url.setPort(m_Engine->m_FTPInfo.nPort);
    m_nFTPState = GET;
    QNetworkReply *reply = m_pFTPUploader->get(QNetworkRequest(url));
    connect(reply, &QNetworkReply::uploadProgress, this, &FormWallet::uploadProgress);
    m_pProgressDlg->open();
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
    if(m_nFTPState == PUT)
    {
        if (!reply->error())
        {
            m_pFile->close();
            m_pFile->deleteLater();
            reply->deleteLater();
            Common::MessageBox("Uploaded to server successfully.");
            m_pProgressDlg->close();
        }
        else
        {
            Common::MessageBox(QString("Can not upload:%1").arg(reply->error()));
            m_pProgressDlg->close();
        }
    }
    else if(m_nFTPState == GET)
    {
        if (!reply->error())
        {
            QString strRes = reply->readAll();
            strRes = strRes.replace("state:","");
            Common::MessageBox(strRes);
            reply->deleteLater();

            Common::MessageBox(QString("Successfully downloaded %1").arg(reply->error()));
            m_pProgressDlg->close();
        }
        else
        {
            Common::MessageBox(QString("Can not download:%1").arg(reply->error()));
            m_pProgressDlg->close();
        }
    }

}
void FormWallet::uploadProgress(qint64 bytesSent, qint64 bytesTotal)
{
    if(bytesTotal > 0)
        m_pProgressDlg->SetProgress(bytesSent, bytesTotal);
}


void FormWallet::on_sts_btn_Create_clicked()
{
    QString strId = ui->sts_txt_UserID->text();
    QString strName = ui->sts_txt_Name->text();
    QString strPass = ui->sts_txt_Password->text();
    QString strPhone = ui->sts_txt_Phone->text();
    QString strExtra = ui->sts_txt_Extra->text();
    if(strId.length() == 0)
    {
        Common::MessageBox("Input the valid user id.");
        return;
    }
    if(strName.length() == 0)
    {
        Common::MessageBox("Input the valid usre name.");
        return;
    }
    if (strPass.length() == 0)
    {
        Common::MessageBox("Input the password.");
        return;
    }
    if (strPhone.length() == 0)
    {
        Common::MessageBox("Input the phone number to recover password");
        return;
    }
    WalletUser user;
    user.strID = strId;
    user.strName = strName;
    user.strPhone = strPhone;
    user.strPassword = strPass;
    user.strExtra = strExtra;
    int nRes = m_Engine->InsertNewWalletUser(user);
    QString strMsg = "DB error";
    if(nRes == 0)
    {
        strMsg = "Already exist with userid";
    }else if(nRes == 1)
    {
        strMsg = "Successfully created";
        emit WalletUserInserted();
    }
    Common::MessageBox(strMsg);
}
