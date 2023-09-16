#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Engine/Engine.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_action_Exit_triggered();
    void on_actionWallet_triggered();
    void on_actionMatureInvestment_triggered();
    void on_MenuItem_clicked();
public slots:
    void ProcessRXTX(QString);
private:
    void on_Page(int );
    void on_ThemeMenu();
private:
    Ui::MainWindow *ui;

private:
    Engine*     m_Engine;
};
#endif // MAINWINDOW_H
