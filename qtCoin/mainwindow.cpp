#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    // Connect to MySql Database
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL","mysqlconn");
    db.setHostName("localhost");
    db.setUserName("root");
    db.setPassword("");
    db.setDatabaseName("ATS");
    db = QSqlDatabase::database("mysqlconn");

    // Initialize variables for DB
    QString dbname="qtbd";
    QFileInfo check_file(dbname);
    bool checkLite=false;
    bool primary=false;
    bool modifyExists=false;
    int primaryPlace=0;
    QString primaryKey;
    QString modivalue;

    if(db.open()) {
        QSqlQuery innerqry;
        QSqlDatabase liteDb;
        liteDb = QSqlDatabase::addDatabase("QSQLITE","sqlliteconn");
        liteDb.setDatabaseName(dbname);

        bool fileExists = check_file.exists() && check_file.isFile();

        QSqlQuery qry(db);
        if(qry.exec("SHOW TABLES"))
        {
            while(qry.next())
            {
                /**--------------------Create Tables in sqliteDB--------------------------**/
                QString table;
                table = qry.value(0).toString();
                QStringList columns;
                QString liteCreate = "CREATE TABLE "+table+" (";
                QSqlQuery innerqry(db);
                if(innerqry.exec("DESCRIBE "+table))
                {
                    int loopCount=0;
                    while(innerqry.next())
                    {
                        liteCreate = liteCreate + innerqry.value(0).toString()+" "+innerqry.value(1).toString()+",";
                        columns.append(innerqry.value(0).toString());
                        if(innerqry.value(3).toString()=="PRI")
                        {
                            primaryPlace=loopCount;
                            primary=true;
                            primaryKey=innerqry.value(0).toString();
                        }
                        loopCount++;
                    }
                    int pos = liteCreate.lastIndexOf(QChar(','));
                    liteCreate=liteCreate.left(pos);
                    if(primary)
                    {
                        liteCreate=liteCreate+", PRIMARY KEY ("+primaryKey+")";
                        primary=false;
                    }
                    liteCreate=liteCreate+");";
                    if(!fileExists)
                    {
                        if(liteDb.open())
                        {
                            QSqlQuery liteQry(liteDb);
                            liteQry.exec(liteCreate);
                            QSqlQuery liteQry2(liteDb);
                            liteQry2.exec("CREATE TABLE sync(last_modified datetime, table_name varchar(50));");
                        }
                    }
                }
                /**--------------------Create Tables in sqliteDB--------------------------**/
                /**--------------------Copy data from Mysql to sqliteDB-------------------**/
                QSqlQuery selectqry(db);
                QString data = "SELECT * from " + table;
                if(selectqry.exec(data))
                {
                    liteDb.open();
                    while(selectqry.next()) {

                        QSqlQuery insertlite(liteDb);
                        int nOfColumns = columns.size();
                        QString fields="INSERT INTO "+table+" ( ";
                        for (int i=0;i<nOfColumns;i++) {
                            fields=fields+columns[i]+" , ";
                        }
                        int poss = fields.lastIndexOf(QChar(','));
                        fields=fields.left(poss);
                        fields=fields+" ) VALUES (";

                        for (int i=0;i<nOfColumns;i++) {
                            fields=fields+"'"+selectqry.value(i).toString()+ "' , ";
                        }
                        int posi = fields.lastIndexOf(QChar(','));
                        fields=fields.left(posi);
                        fields=fields+" ) ";
                        qDebug() << fields;
                        insertlite.exec(fields);
                    }
                }
                /**--------------------Copy data from Mysql to sqliteDB-------------------**/
            }
        }
    }
}

void MainWindow::sleep(qint64 msec)
{
    QEventLoop loop;
    QTimer::singleShot(msec, &loop, SLOT(quit()));
    loop.exec();
}
