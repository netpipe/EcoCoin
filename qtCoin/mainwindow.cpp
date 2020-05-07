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

    db = QSqlDatabase::addDatabase("QSQLITE");
  //  db.setDatabaseName(name);
    db.setDatabaseName("database.sqlite");
    if(db.open())
    {
       qDebug()<<"Successful database connection";
    }
    else
    {
       qDebug()<<"Error: failed database connection";
    }

  //  if ( db.isOpen() )
  //      std::cout << "the database is:  open" << std::endl;

    db.close();

    createUserTable();
    selectUsers();


 //   coinDB = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("coins.sqlite");
    if(db.open())
    {
        qDebug()<<"Successful coin database connection";
    }
    else
    {
        qDebug()<<"Error: failed database connection";
    }
    db.close();

    createCoinTable();

    db.setDatabaseName("database.sqlite");

   // if ( coinDB.isOpen() )
   //     std::cout << "the database is:  open" << std::endl;

//QSqlDatabase::removeDatabase( QSqlDatabase::defaultConnection );




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
    db.setDatabaseName("database.sqlite");
    if(db.open())
    {
       qDebug()<<"Successful database connection";
    }
    else
    {
       qDebug()<<"Error: failed database connection";
    }

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
    query.clear();
   // db.close();
}

void MainWindow::createCoinTable()
{
    db.setDatabaseName("coins.sqlite");
    if(db.open())
    {
        qDebug()<<"Successful coin database connection";
    }
    else
    {
        qDebug()<<"Error: failed database connection";
    }

    QString query;
    query.append("CREATE TABLE IF NOT EXISTS coins("
                 "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                 "addr VARCHAR(50));");

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

query.clear();

    db.close();
//QSqlDatabase::removeDatabase( QSqlDatabase::defaultConnection );
//    if ( coinDB.isOpen() )
//        std::cout << "the database is:  open" << std::endl;

}



void MainWindow::insertUser()
{
    db.setDatabaseName("database.sqlite");
    if(db.open())
    {
       qDebug()<<"Successful database connection";
    }
    else
    {
       qDebug()<<"Error: failed database connection";
    }

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
query.clear();
    db.close();

}

void MainWindow::selectUsers()
{
    db.setDatabaseName("database.sqlite");
    if(db.open())
    {
       qDebug()<<"Successful database connection";
    }
    else
    {
       qDebug()<<"Error: failed database connection";
    }


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
    query.clear();
    db.close();

}

void MainWindow::on_pushButtonInsertUser_clicked()
{
    insertUser();
  //  selectUsers();
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
  //  int ii=0;
    QStringList list;
     //   QList<QString> nums;
    QStringList nums;


    do {
        line = in.readLine();
        searchString=":";
        if (line.contains(searchString)) { //, Qt::CaseSensitive
            // do something
            QRegExp rx("[:]");// match a comma or a space
            list = line.split(rx);
            nums.append(list.at(1).toLatin1());
        }
    } while (!line.isNull());

               QString coinname=nums.at(0);
               qDebug("%s", qUtf8Printable(coinname));
               QString coincount=nums.at(1);
               qDebug("%s", qUtf8Printable(coincount));

    //  QString coincount=nums.at(0).toLocal8Bit().constData();

                QString CoinLength=nums.at(2);
                qDebug("%s", qUtf8Printable(CoinLength));
                QString createday=nums.at(3);
                qDebug("%s", qUtf8Printable(createday));
                QString CreateMonth=nums.at(4);
                qDebug("%s", qUtf8Printable(CreateMonth));
                QString createyear=nums.at(5);
                qDebug("%s", qUtf8Printable(createyear));
                QString createtime=nums.at(6);
                qDebug("%s", qUtf8Printable(createtime));
                QString coinvalue=nums.at(7);
                qDebug("%s", qUtf8Printable(coinvalue));

                QString matures=nums.at(8);
                qDebug("%s", qUtf8Printable(matures));
                QString coinpayout=nums.at(0);
                qDebug("%s", qUtf8Printable(coinpayout));
                QString maturedate=nums.at(10);
                qDebug("%s", qUtf8Printable(CoinLength));
                QString maturemonth=nums.at(11);
                qDebug("%s", qUtf8Printable(CoinLength));
                QString matureyear=nums.at(12);
                qDebug("%s", qUtf8Printable(CoinLength));
                 QString maturetime=nums.at(13);
                 qDebug("%s", qUtf8Printable(maturetime));
                 QString maturedescription=nums.at(14);
                qDebug("%s", qUtf8Printable(maturedescription));

}

void MainWindow::on_pushButton_clicked() //generate coins button
{
    GenerateCoins3(8,10000);
}

void MainWindow::on_actionSyncUSB_triggered()
{

}

void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_pushButton_3_clicked() //search button
{
    db.open();
    QString query;
    query.append("SELECT "+ ui->userid->text() +" FROM users");

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
    db.close();
}
