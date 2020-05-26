//#ifndef MainWindow2_H
//#define MainWindow2_H

//#include <QMainWindow>

//namespace Ui {
//    class MainWindow2;
//}

//class FtpServer;

//class MainWindow2 : public QMainWindow
//{
//    Q_OBJECT
//public:
//    enum ScreenOrientation {
//        ScreenOrientationLockPortrait,
//        ScreenOrientationLockLandscape,
//        ScreenOrientationAuto
//    };

//    explicit MainWindow2(QWidget *parent = 0);
//    virtual ~MainWindow2();

//    // Note that this will only have an effect on Symbian and Fremantle.
//    void setOrientation(ScreenOrientation orientation);

//    void showExpanded();

//private slots:
//    void on_pushButtonRestartServer_clicked();

//    void on_toolButtonBrowse_clicked();

//    void onPeerIpChanged(const QString &peerIp);

//    void on_pushButtonShowDebugLog_clicked();

//    void on_pushButtonExit_clicked();

//private:
//    Ui::MainWindow2 *ui;

//    // This is the FTP server object.
//    FtpServer *server;

//    // Load the stored settings.
//    void loadSettings();

//    // Save the current settings.
//    void saveSettings();

//    // Restart the FTP server.
//    void startServer();

//    // Get the LAN IP of the host, e.g. "192.168.1.10".
//    QString lanIp();
//};

//#endif // MainWindow2_H
