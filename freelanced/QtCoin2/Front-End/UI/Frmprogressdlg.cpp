#include "Frmprogressdlg.h"
#include "ui_Frmprogressdlg.h"

FrmProgressDlg::FrmProgressDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmProgressDlg)
{
    ui->setupUi(this);

}

FrmProgressDlg::~FrmProgressDlg()
{
    delete ui;
}

void FrmProgressDlg::SetProgress(qint64 bytesSent, qint64 bytesTotal)
{
    QString str = QString("%1/%2 bytes uploaded.").arg(bytesSent).arg(bytesTotal);
    ui->lblInfo->setText(str);
    ui->progressBar->setValue(100 * bytesSent/bytesTotal);

}
