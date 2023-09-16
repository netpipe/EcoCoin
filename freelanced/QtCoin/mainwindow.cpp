#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Engine/Common.h"
#include <QFileInfo>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // init instances
    m_Engine = Common::instance()->g_Engine;

    printf("Version: %d\n", m_Engine->GetVersion());
    // UI Init
    ui->actionMatureInvestment->setVisible(m_Engine->IsAdmin());
    // set default page to wallet
    on_Page(0);
    // load theme menu manger
    on_ThemeMenu();
    m_Engine->SetWidget(this);
    // ~UI Init
}

MainWindow::~MainWindow()
{
    m_Engine->Release();
    delete ui;
}

void MainWindow::on_action_Exit_triggered()
{
    QApplication::quit();
}
void MainWindow::on_actionWallet_triggered()
{
    on_Page(0);
}
void MainWindow::on_actionMatureInvestment_triggered()
{
    on_Page(1);
}

void MainWindow::on_Page(int nPage)
{
    ui->wdt_Wallet->setVisible(nPage==0);
    ui->wdt_Investment->setVisible(nPage==1);
}
void MainWindow::on_ThemeMenu()
{
    try {
        StyleManager* themeManager = m_Engine->GetStyleManager();
        QMenu *menu = new QMenu("Themes", this);

        for(int i = 0; i < themeManager->m_aStyles.count(); i++)
        {
            QString styleName = QFileInfo(themeManager->m_aStyles.at(i)).fileName().split('.')[0];

            QAction *dummyaction = new QAction(styleName, this);
            QObject::connect(dummyaction, SIGNAL(triggered()), this, SLOT(on_MenuItem_clicked()));
            menu->addAction(dummyaction);
        }
        ui->menuSettings->addMenu(menu);
    } catch (exception e) {
        qDebug() << e.what();
    }
}

void MainWindow::on_MenuItem_clicked()
{
    try {
        QAction *action = (QAction *)this->sender();
        m_Engine->GetStyleManager()->loadStyleSheet(action->text());
    }catch (exception e) {
        qDebug() << e.what();
    }
}

void MainWindow::ProcessRXTX(QString str)
{
    m_Engine->ProcessRXTX(str);
}
