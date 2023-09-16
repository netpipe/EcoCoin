#include "Forminvestment.h"
#include "ui_Forminvestment.h"
#include <QDateTime>
#include <QDebug>
#include <QMessageBox>
#include "Engine/Common.h"
#include "Engine/global.h"
#include "UI/Formftp.h"
#include <QFileDialog>
#include <QFileInfo>

FormInvestment::FormInvestment(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormInvestment)
{
    ui->setupUi(this);
    // init variales
    m_Engine = Common::instance()->g_Engine;
    // init UI
    InitUI();
    // set default tab
    ui->tabWidget->setCurrentIndex(0);
}

FormInvestment::~FormInvestment()
{
    delete ui;
}

void FormInvestment::InitUI()
{
    // Generate Coin Page
    QDateTime curTime = QDateTime::currentDateTime();
    ui->cns_sp_Day->setValue(curTime.date().day());
    ui->cns_cb_Month->setCurrentIndex(curTime.date().month() - 1);
    ui->cns_sp_Year->setValue(curTime.date().year());
    ui->cns_sp_Time->setTime(curTime.time());

    ui->cns_sp_matureDay->setValue(curTime.date().day());
    ui->cns_cb_matureMonth->setCurrentIndex(curTime.date().month() - 1);
    ui->cns_cb_matureYear->setValue(curTime.date().year()+1);
    ui->cns_cb_matureTime->setTime(curTime.time());

}

QStringList FormInvestment::validateUserInput()
{
    QString strEmail = ui->add_txt_Email->text();
    QString strName = ui->add_txt_Name->text();
    QString strPass = ui->add_txt_Password->text();
    QString strPhone = ui->add_txt_Phone->text();
    QString strDate = ui->add_txt_Date->text();
    QString strInfo = ui->add_txt_Extra->text();
    QString strClass = ui->add_txt_Class->text();
    QStringList resStr;
    if (strEmail.length() == 0)
    {
        Common::MessageBox("Input valid Email.");
        return resStr;
    }
    if (strName.length() == 0)
    {
        Common::MessageBox("Input valid Name.");
        return resStr;
    }
    if (strPass.length() == 0)
    {
        Common::MessageBox("Input valid Password.");
        return resStr;
    }
    if (strPhone.length() == 0)
    {
        Common::MessageBox("Input valid Phone.");
        return resStr;
    }
    if (strDate.length() == 0)
    {
        Common::MessageBox("Input valid Date.");
        return resStr;
    }
    if (strInfo.length() == 0)
    {
        Common::MessageBox("Input valid Info.");
        return resStr;
    }
    if (strClass.length() == 0)
    {
        Common::MessageBox("Input valid Class.");
        return resStr;
    }

    resStr.push_back(strEmail);
    resStr.push_back(strName);
    resStr.push_back(strPass);
    resStr.push_back(strPhone);
    resStr.push_back(strDate);
    resStr.push_back(strInfo);
    resStr.push_back(strClass);

    return resStr;
}
/*
 ***********************************************************************************************
                                    generate slots
 ***********************************************************************************************
*/
void FormInvestment::on_cns_btn_Generate_clicked()
{
    int nAmount = ui->cns_txt_Count->text().toInt();
    if(nAmount % 2 != 0)
    {
        Common::MessageBox("Amount must be even.");
        return;
    }
    if(QFileInfo(COIN_DB_NAME).exists())
    {
        if(QMessageBox::question(this, "Already Exists", "Do you want to generate new coindb?",
                                              QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes)
        {
            return;
        }
        m_Engine->ClearCoinData();
    }
    m_Engine->CreateCoinTable();

    int nCoinLen = ui->cns_txt_Length->text().toInt();
    int nCoinCount = ui->cns_txt_Count->text().toInt();
    m_Engine->GenerateCoin(nCoinLen, nCoinCount);
    Common::MessageBox("Coin created successfully.");
    saveSettings();
}
void FormInvestment::saveSettings()
{

    m_Engine->m_CoinInfo.strCoinName = ui->cns_txt_Coin->text();
    m_Engine->m_CoinInfo.strCoinCount = ui->cns_txt_Count->text();
    m_Engine->m_CoinInfo.strCoinLength = ui->cns_txt_Length->text();
    m_Engine->m_CoinInfo.strCreateDay = ui->cns_sp_Day->text();
    m_Engine->m_CoinInfo.strCreateMonth = ui->cns_cb_Month->currentText();
    m_Engine->m_CoinInfo.strCreateYear = ui->cns_sp_Year->text();
    m_Engine->m_CoinInfo.strCreateTime = QString("%1|%2").arg(ui->cns_sp_Time->time().hour()).arg(ui->cns_sp_Time->time().minute());
    m_Engine->m_CoinInfo.strCoinValue = ui->cns_txt_Value->text();
    m_Engine->m_CoinInfo.strMatures = ui->cns_chk_mature_Yes->text();
    m_Engine->m_CoinInfo.strCoinpayout = ui->cns_sp_Payout->text();
    m_Engine->m_CoinInfo.strEncrypted = ui->cns_chk_EncYes->text();
    m_Engine->m_CoinInfo.strMatureDate = ui->cns_sp_matureDay->text();
    m_Engine->m_CoinInfo.strMatureMonth = ui->cns_cb_matureMonth->currentText();
    m_Engine->m_CoinInfo.strMatureYear = ui->cns_cb_matureYear->text();
    m_Engine->m_CoinInfo.strMatureTime = QString("%1|%2").arg(ui->cns_cb_matureTime->time().hour()).arg(ui->cns_cb_matureTime->time().minute());
    m_Engine->m_CoinInfo.strMaturedescription = ui->cns_txt_description->toPlainText();
    m_Engine->m_CoinInfo.strUsbDriveName = ui->cns_sp_DriveName->text();
    m_Engine->saveCoinSettings();
    m_Engine->SaveGeneratedCoinInfo();
}
/*
 ***********************************************************************************************
                                    user management
 ***********************************************************************************************
*/
void FormInvestment::on_add_btn_Insert_clicked()
{
    QStringList strValid = validateUserInput();
    if(strValid.length() == 0)
        return;
    QString strEmail = strValid.at(0);
    bool bExist = m_Engine->IsEmailExists(strEmail);
    if(bExist)
    {
        Common::MessageBox("Use another email.");
        return;
    }
    QString str = m_Engine->GetValidUserID(strValid);
    ui->add_txt_UserID->setText(str);
}
void FormInvestment::on_add_btn_GenerateQR_clicked()
{
    QStringList strValid = validateUserInput();
    if(strValid.length() == 0)
        return;
    QString data = "";
    for(int i = 0; i < strValid.count() - 1; i++)
    {
        data += strValid.at(i) + "::";
    }
    data += strValid.at(strValid.length() - 1);
#ifdef BARCODE
    std::wstring text(data.toStdWString());
    const wchar_t *wstr = text.c_str();
    char mbstr[4000];
    std::wcstombs(mbstr, wstr, 4000);
    const QrCode::Ecc errCorLvl = QrCode::Ecc::LOW;
    const QrCode qr = QrCode::encodeText(mbstr, errCorLvl);

    ofstream write;

    std::string filename = "tmp.svg";
    write.open(filename.c_str(), ios::out | ios::binary);
    write << qr.toSvgString(4);

    QImage *img_object = new QImage();
    img_object->load("./tmp.svg");
    QPixmap image = QPixmap::fromImage(*img_object);
    QPixmap scaled_img = image.scaled(ui->add_QRView->width(), ui->add_QRView->height(), Qt::KeepAspectRatio);
    QGraphicsScene *scene = new QGraphicsScene();
    scene->addPixmap(scaled_img);
    scene->setSceneRect(scaled_img.rect());
    ui->add_QRView->setScene(scene);
    ui->add_QRView->show();
    ui->add_btn_SaveQR->setEnabled(true);

#endif
}

void FormInvestment::on_add_btn_SaveQR_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save image", QCoreApplication::applicationDirPath(), "BMP Files (*.bmp);;JPEG (*.JPEG);;PNG (*.png)");
    if (!fileName.isNull())
    {
        QPixmap pixMap = this->ui->add_QRView->grab();
        pixMap.save(fileName);
    }
}
void FormInvestment::on_add_btn_Save_clicked()
{
    m_Engine->Download("ftp://127.0.0.1/");
}
/*
 ***********************************************************************************************
                                    user table
 ***********************************************************************************************
*/
void FormInvestment::on_btn_SelectAll_clicked()
{
    QString strQuery = "SELECT * FROM users";
    ShowUsers(strQuery);
}

void FormInvestment::on_btn_Search_clicked()
{
    QString strQuery = QString("SELECT * FROM users WHERE name ='%1'").arg(ui->userid->text());
    ShowUsers(strQuery);
}

void FormInvestment::ShowUsers(QString strQuery)
{
    QList<QList<QString>> queryData = m_Engine->GetDBManager()->SelectData(DB_NAME, strQuery, 11);
    ui->tableWidgetUsers->setRowCount(0);
    int nRow = 0;
    for(int i = 0; i < queryData.count(); i++)
    {
        ui->tableWidgetUsers->insertRow(0);
        ui->tableWidgetUsers->setItem(nRow, 0, new QTableWidgetItem(queryData.at(i).at(1)));
        ui->tableWidgetUsers->setItem(nRow, 1, new QTableWidgetItem(queryData.at(i).at(2)));
        ui->tableWidgetUsers->setItem(nRow, 2, new QTableWidgetItem(queryData.at(i).at(3)));
        ui->tableWidgetUsers->setItem(nRow, 3, new QTableWidgetItem(queryData.at(i).at(10)));
        ui->tableWidgetUsers->setItem(nRow, 4, new QTableWidgetItem(queryData.at(i).at(6)));
    }
    ui->tableWidgetUsers->setColumnWidth(1, 350);
}


/*
 ***********************************************************************************************
                                    validate
 ***********************************************************************************************
*/
void FormInvestment::on_btn_validateCoin_clicked()
{
    if (m_Engine->IsAdmin())
    {
        QList<QList<QString>> dbCoin = m_Engine->GetDBManager()->SelectData(COIN_DB_NAME, "Select addr from coins;", 1);
        QList<QList<QString>> dbAvailableCoin = m_Engine->GetDBManager()->SelectData(AVAIL_COINS_DB_NAME, "Select addr from coins;", 1);
        while(dbCoin.count() > 0)
        {
            QString coin = dbCoin.at(0).at(0);
            QList<QString> tmp;
            tmp.push_back(coin);
            int nIndex = dbAvailableCoin.indexOf(tmp);
            if(nIndex >= 0)
                dbAvailableCoin.erase(dbAvailableCoin.begin() + nIndex);
            dbCoin.erase(dbCoin.begin());
        }
        QString strMsg = "validation failed";
        if(dbAvailableCoin.size() == 0)
        {
            strMsg = "validation successed.";
        }
        Common::MessageBox(strMsg);
    }
    else
    {
        //download pickup.sqlite and verify against current coin holdings
        // Download("ftp://admin:qt@127.0.0.1:8285/pickup.sqlite");
    }
}
/*
 ***********************************************************************************************
                                    validate
 ***********************************************************************************************
*/

void FormInvestment::on_btn_FtpServer_clicked()
{
#ifdef FTP
    if(m_Engine->IsAdmin())
    {
        FormFTP dlg;
        dlg.exec();
    }
    else
    {
        Common::MessageBox("You are not server.");
    }
#endif
}
void FormInvestment::on_btn_FTPTest_clicked()
{

}

