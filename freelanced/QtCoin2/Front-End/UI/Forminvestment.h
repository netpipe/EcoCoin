#ifndef FORMINVESTMENT_H
#define FORMINVESTMENT_H

#include <QWidget>
#include <../Engine/Engine.h>


namespace Ui {
class FormInvestment;
}

class FormInvestment : public QWidget
{
    Q_OBJECT

public:
    explicit FormInvestment(QWidget *parent = nullptr);
    ~FormInvestment();

private:
    Ui::FormInvestment *ui;
private:
    void            InitUI();
    QStringList     validateUserInput();
    void            ShowUsers(QString);
    void            saveSettings();

public slots:
    // generate slots
    void on_cns_btn_Generate_clicked();


    // user management
    void on_add_btn_Insert_clicked();
    void on_add_btn_GenerateQR_clicked();
    void on_add_btn_SaveQR_clicked();
    void on_add_btn_Save_clicked();

    // user table
    void on_btn_SelectAll_clicked();
    void on_btn_Search_clicked();

    // validate
    void on_btn_validateCoin_clicked();

    // FTP
    void on_btn_FtpServer_clicked();
    void on_btn_FTPTest_clicked();
    void on_btn_SmtpSave_clicked();

private:
    Engine* m_Engine;
};

#endif // FORMINVESTMENT_H
