#include "Formftp.h"
#include "ui_Formftp.h"

FormFTP::FormFTP(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormFTP)
{
    ui->setupUi(this);
    m_Engine = Common::instance()->g_Engine;

    ui->txtPort->setText(QString("%1").arg(m_Engine->GetFTPServer()->m_Port));
    ui->txtUser->setText(m_Engine->GetFTPServer()->m_strUser);
    ui->txtPassword->setText(m_Engine->GetFTPServer()->m_strPass);
    ui->txtPath->setText(m_Engine->GetFTPServer()->m_strPath);
    ui->chk_Annony->setChecked(m_Engine->GetFTPServer()->m_bAnony);
    ui->chk_IPAllow->setChecked(m_Engine->GetFTPServer()->m_bOnlyOneIpAllowed);
    ui->chk_UserList->setChecked(m_Engine->GetFTPServer()->m_bUserList);
    ui->chk_ReadOnly->setChecked(m_Engine->GetFTPServer()->m_bReadOnly);
    ui->userlist->clear();
    QStringList list = m_Engine->GetFTPServer()->m_Users;
    for(int i = 0; i < list.count(); i++)
    {
        ui->userlist->addItem(list.at(i));
    }
}

FormFTP::~FormFTP()
{
    delete ui;
}

void FormFTP::on_btnAdd_clicked()
{
    QString strUser = ui->txtUser_2->text();
    if(strUser.length() == 0)
    {
        Common::MessageBox("Inpust valid username and password.");
        return;
    }
    if(getIndex(strUser) >= 0)
    {
        Common::MessageBox("Already exist.");
        return;
    }
    ui->userlist->addItem(strUser);
    ui->txtUser_2->setText("");
}
void FormFTP::on_btnRemove_clicked()
{
    int index = ui->userlist->currentIndex().row();
    if(index < 0)
        return;
    qDeleteAll(ui->userlist->selectedItems());
}
int FormFTP::getIndex(QString strUser)
{
    int index = -1;
    for(int i = 0; i < ui->userlist->count(); i++)
    {
        if(ui->userlist->item(i)->text() == strUser)
        {
            index = i;
            break;
        }
    }
    return index;

}
void FormFTP::on_btnStart_clicked()
{
    on_btnSave_clicked();
    m_Engine->GetFTPServer()->StartServer();
}
void FormFTP::on_btnDebug_clicked()
{

}
void FormFTP::on_btnSave_clicked()
{
    int nPort = ui->txtPort->text().toInt();
    QString strUser = ui->txtUser->text();
    QString strPass = ui->txtPassword->text();
    QString strPath = ui->txtPath->text();
    bool bAnony = ui->chk_Annony->isChecked();
    bool bOnlyOne = ui->chk_IPAllow->isChecked();
    bool bUserList = ui->chk_UserList->isChecked();
    bool bReadOnly = ui->chk_ReadOnly->isChecked();
    QStringList userlist;
    for(int i = 0; i < ui->userlist->count(); i++)
    {
        userlist.push_back(ui->userlist->item(i)->text());
    }
    m_Engine->GetFTPServer()->setSettings(strUser, strPass, nPort, strPath, bAnony, bReadOnly, bOnlyOne, bUserList, userlist);
    m_Engine->GetFTPServer()->saveSettings();
}
void FormFTP::on_btnExit_clicked()
{
    reject();
}
