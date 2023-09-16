#ifndef FRMPROGRESSDLG_H
#define FRMPROGRESSDLG_H

#include <QDialog>
#include <QCloseEvent>
namespace Ui {
class FrmProgressDlg;
}

class FrmProgressDlg : public QDialog
{
    Q_OBJECT

public:
    explicit FrmProgressDlg(QWidget *parent = nullptr);
    ~FrmProgressDlg();
    void    SetProgress(qint64 bytesSent, qint64 bytesTotal);
public slots:
    // void closeEvent(QCloseEvent *event);
private:
    Ui::FrmProgressDlg *ui;
};

#endif // FRMPROGRESSDLG_H
