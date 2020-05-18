#ifndef LOADTHEME_H
#define LOADTHEME_H

//#include <QWidget>
#include <mainwindow.h>
#include "ui_mainwindow.h"
#include <QCoreApplication>
#include "src/qstylesheetmanager.h"
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <QFile>
#include <QDebug>
#include <QDirIterator>

void MainWindow::themeInit(){

    QDir directory("./Resource/themes/");
    QStringList themes = directory.entryList(QStringList() << "*.qss" << "*.qss",QDir::Files);

    for (int i=0;i < themes.size(); i++){
        QString themetmp = themes.at(i);
        ui->cmbTheme->addItem((themetmp.toLatin1() ));
        qDebug()<<themetmp << "/n";
    }


    QDirIterator it("./Resource/themes/", QStringList() << "*.qss", QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext()){
      //  QFileInfo fileInfo(f.fileName());
        ui->cmbTheme->addItem(it.next().toLatin1());
    }

    //        QFile file("themes.txt");
    //        if (file.open(QIODevice::ReadOnly))
    //        {
    //           QTextStream stream(&file);
    //           QString entry = stream.readLine();
    //           while (!(stream.atEnd()))
    //           {
    //              ui->cmbTheme->addItem(entry);
    //              entry = stream.readLine();
    //           }
    //           ui->cmbTheme->addItem(entry);
    //           file.close();
    //        }

    //    ui->cmbTheme->itemText(ui->cmbTheme->count());

    if (ui->cmbTheme->currentText().toLatin1() != ""){
        //QStyleSheetManager::loadStyleSheet( ui->cmbTheme->currentText().toLatin1());
        QStyleSheetManager::loadStyleSheet(  ui->cmbTheme->itemText(ui->cmbTheme->count()-1));
    }

    QFile file("themes.txt");
        if(file.open(QIODevice::ReadWrite | QIODevice::Text))// QIODevice::Append |
        {
                QTextStream stream(&file);
                for (int i = 0; i < ui->cmbTheme->count(); i++)
                {
                 stream << ui->cmbTheme->itemText(i) << endl;
                }
            //                file.write("\n");
               file.close();
        }
}

void MainWindow::on_btnAddThemeFromFile_clicked()
{
    QString newFile = QFileDialog ::getOpenFileName(0,"Select File","","Files (*.qss)");
    ui->cmbTheme->addItem(newFile);

    QFile file("themes.txt");
        if(file.open(QIODevice::ReadWrite | QIODevice::Text))// QIODevice::Append |
        {
                QTextStream stream(&file);
                for (int i = 0; i < ui->cmbTheme->count(); i++)
                {
                 stream << ui->cmbTheme->itemText(i) << endl;
                }
            //                file.write("\n");
               file.close();
        }
}

void MainWindow::on_btnRemoveThemeFromFile_clicked()
{
    ui->cmbTheme->removeItem(ui->cmbTheme->currentIndex());
    QFile file("themes.txt");
    if(file.open(QIODevice::ReadWrite | QIODevice::Text))// QIODevice::Append |
    {
        QTextStream stream(&file);
        for (int i = 0; i < ui->cmbTheme->count(); i++)
        {
            stream << ui->cmbTheme->itemText(i) << endl;
        }
     //file.write("\n");
     file.close();
     }
}


#endif // LOADTHEME_H
