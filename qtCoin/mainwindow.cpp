#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "src/encryption/encryption.h"
#include "src/encryption/rsa/Rsa.h"
#include "src/qstylesheetmanager.h"
#include "src/downloadmanager.h"

#include <coingenerator.h>

#include <QDebug>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qDebug()<<"Application initialized...";

    QString name;
    name.append("database.sqlite");

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(name);
    if(db.open())
    {
       qDebug()<<"Successful database connection";
    }
    else
    {
       qDebug()<<"Error: failed database connection";
    }

    createUserTable();
    selectUsers();

    player=new QMediaPlayer();
   // player->setMedia(QUrl("qrc:/sounds/ec1_mono.ogg"));
   // player->setMedia(QUrl::fromLocalFile("./paddle_hit.wav"));
    //or play from memory
     QFile file("Resource/sounds/ec1_mono.ogg");
     file.open(QIODevice::ReadOnly);
     QByteArray* arr = new QByteArray(file.readAll());
     file.close();
     QBuffer* buffer = new QBuffer(arr);
     buffer->open(QIODevice::ReadOnly);
     buffer->seek(0);

//    media->setMedia("sound.mp3");
    player->setMedia(QMediaContent(), buffer);
    player->play();

// qDebug("text");
// qDebug()<<"test";
// cout << "testing";
// printf("testing");
// qInfo() << "Is this working?";

//    QString s = QDate::currentDate().toString();
//    QDate::currentDate().day();
//    QDate::currentDate().month();
//    QDate::currentDate().year();

    ui->progress->setValue(50);
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ListUSB(){ // only need the drive we have named USBKEY (could put random number with it for verify)
//https://stackoverflow.com/questions/40035332/how-to-get-path-to-usb-drive-on-linux-in-qt
    foreach (const QStorageInfo &storage, QStorageInfo::mountedVolumes()) {

       qDebug() << storage.rootPath();
       if (storage.isReadOnly())
           qDebug() << "isReadOnly:" << storage.isReadOnly();

       qDebug() << "name:" << storage.name();
       qDebug() << "fileSystemType:" << storage.fileSystemType();
       qDebug() << "size:" << storage.bytesTotal()/1000/1000 << "MB";
       qDebug() << "availableSize:" << storage.bytesAvailable()/1000/1000 << "MB";
    }
}

void MainWindow::createUserTable()
{
    QString query;
    query.append("CREATE TABLE IF NOT EXISTS users("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "name VARCHAR(100),"
                    "surname VARCHAR(100),"
                    "age INTEGER NOT NULL,"
                    "class INTEGER NOT NULL"
                    ");");

    QSqlQuery create;
    create.prepare(query);

    if (create.exec())
    {
        qDebug()<<"Table exists or has been created";
    }
    else
    {
        qDebug()<<"Table not exists or has not been created";
        qDebug()<<"ERROR! "<< create.lastError();
    }
}

void MainWindow::insertUser()
{
    QString query;

//        QByteArray bFname = EncryptMsg(fname);
//        QString mykey1 = BigInt2Str(m_e);
//        QString mykey2 = BigInt2Str(m_n);

    query.append("INSERT INTO users("
                    "name,"
                    "surname,"
                    "age,"
                    "class)"
                    "VALUES("
                    "'"+ui->lineEditName->text()+"',"
                    "'"+ui->lineEditSurname->text()+"',"
                    ""+ui->lineEditAge->text()+","
                    ""+ui->lineEditClass->text()+""
                    ");");

    QSqlQuery insert;
    insert.prepare(query);

    if (insert.exec())
    {
        qDebug()<<"The user is properly inserted";
    }
    else
    {
        qDebug()<<"The user is not inserted correctly";
        qDebug()<<"ERROR! "<< insert.lastError();
    }
}

void MainWindow::selectUsers()
{
    QString query;
    query.append("SELECT * FROM users");

    QSqlQuery select;
    select.prepare(query);

    if (select.exec())
    {
        qDebug()<<"The user is properly selected";
    }
    else
    {
        qDebug()<<"The user is not selected correctly";
        qDebug()<<"ERROR! "<< select.lastError();
    }

    int row = 0;
    ui->tableWidgetUsers->setRowCount(0);

    while (select.next())
    {
        ui->tableWidgetUsers->insertRow(row);
        ui->tableWidgetUsers->setItem(row,0,new QTableWidgetItem(select.value(1).toByteArray().constData()));
        ui->tableWidgetUsers->setItem(row,1,new QTableWidgetItem(select.value(2).toByteArray().constData()));
        ui->tableWidgetUsers->setItem(row,2,new QTableWidgetItem(select.value(3).toByteArray().constData()));
        ui->tableWidgetUsers->setItem(row,3,new QTableWidgetItem(select.value(4).toByteArray().constData()));
        row++;
    }
}

void MainWindow::on_pushButtonInsertUser_clicked()
{
    insertUser();
    selectUsers();
}

void MainWindow::on_gencoininfo_btn_clicked()
{
    QFile file("settings.txt");
      //    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
          if(file.open(QIODevice::ReadWrite | QIODevice::Text))
          {
              QTextStream stream(&file);
              stream << "CoinName:" << ui->coinname->text() <<'\n';
              stream << "CoinCount:" << ui->coincount->text() <<'\n';
              stream << "CoinLength:" << ui->coinlength->text() <<'\n';
              stream << "createday:" << ui->createday->text() <<'\n';
              stream << "CreateMonth: " << ui->createmonth->currentText() <<'\n';
              stream << "createyear:" << ui->createyear->text() <<'\n';
              stream << "createtime:" << ui->createtime->text() <<'\n';
              stream << "coinvalue:" << ui->coinvalue->text() <<'\n';
              stream << "matures:" << ui->matureradio_yes->text() <<'\n';
              stream << "coinpayout: " << ui->coinpayout->text() <<'\n';

              stream << "maturedate:" << ui->matureday->text() <<'\n';
              stream << "maturemonth:" << ui->maturemonth->currentText() <<'\n';
              stream << "matureyear: " << ui->matureyear->text() <<'\n';
              stream << "maturetime:" << ui->maturetime->text() <<'\n';
              stream << "maturedescription:" << ui->maturedescription->toPlainText() <<'\n';
              file.close();
          }
}

void MainWindow::on_actionOpenCoin_triggered()
{//https://stackoverflow.com/questions/31384273/how-to-search-for-a-string-in-a-text-file-using-qt
    QString searchString("the string I am looking for");
    QFile MyFile("settings.txt");
    MyFile.open(QIODevice::ReadWrite);
    QTextStream in (&MyFile);
    QString line;
    do {
        line = in.readLine();
        if (!line.contains(searchString, Qt::CaseSensitive)) {
            // do something
        }
    } while (!line.isNull());
}

void MainWindow::on_pushButton_clicked()
{
     qDebug()<<"Error: failed database connection";
    GenerateCoins3();
}

void MainWindow::on_actionSyncUSB_triggered()
{

}

void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}
