#ifdef DBUS
#include        <iostream>
#include        <QtCore/QThread>
#include        <QtCore/QtCore>
#include        <QtDBus/QDBusInterface>

#define         DBUS_PATH       "/"
#define         DBUS_INTERFACE  "my.qtcoin.dbus"
#define         DBUS_NAME       "message"

class           DBusHandler : public QThread
{
  Q_OBJECT;

private:

  void          run(void)
  {
    QDBusConnection connection = QDBusConnection::sessionBus();

    connection.registerService(QString());
    connection.registerObject(DBUS_PATH, this, QDBusConnection::ExportAllSlots);
    connection.connect(QString(), DBUS_PATH, DBUS_INTERFACE, DBUS_NAME, this, SLOT(remoteCall(QString)));

    exec();
  }

public:
  DBusHandler(void) {}
  virtual ~DBusHandler(void) {}

  void          stop(void)
  {
    QDBusConnection connection = QDBusConnection::sessionBus();

    connection.unregisterObject(DBUS_PATH);
    connection.unregisterService(QString());
    connection.disconnectFromBus(connection.name());
    QThread::quit();
  }

public slots:
  void          remoteCall(QByteArray message)
  {
    std::cout << "Message size: "  << message.size() << std::endl;
  }

  void          remoteCall(QString message) {
    std::cout << "Message size: "  << message.size() << " data: " << message.toUtf8().constData() << std::endl;
  }

};
#endif
