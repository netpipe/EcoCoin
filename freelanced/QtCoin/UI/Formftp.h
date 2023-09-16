#ifndef FORMFTP_H
#define FORMFTP_H

#include <QDialog>
#include "Engine/Engine.h"
#include "Engine/Common.h"

namespace Ui {
class FormFTP;
}

class FormFTP : public QDialog
{
    Q_OBJECT

public:
    explicit FormFTP(QWidget *parent = nullptr);
    ~FormFTP();

private:
    Ui::FormFTP *ui;
    Engine  *m_Engine;
private slots:
    void on_btnAdd_clicked();
    void on_btnRemove_clicked();
    void on_btnStart_clicked();
    void on_btnDebug_clicked();
    void on_btnSave_clicked();
    void on_btnExit_clicked();

private:
    int getIndex(QString);
};

#endif // FORMFTP_H
