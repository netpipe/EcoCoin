#pragma once


#include        <iostream>
#include        <QtCore/QThread>
#include        <QtCore/QtCore>
#include        <QtDBus/QDBusInterface>

class           DBusHandler : public QThread
{
  Q_OBJECT;

private:

  void          run(void)
  {
    QDBusConnection connection = QDBusConnection::sessionBus();

    connection.registerService("my.qdbus.example");
    connection.registerObject("/", this, QDBusConnection::ExportAllSlots);
    exec();
  }

public:
  DBusHandler(void) {}
  virtual ~DBusHandler(void) {}

  void          stop(void)
  {
    QDBusConnection connection = QDBusConnection::sessionBus();

    connection.unregisterObject("/");
    connection.unregisterService("my.qdbus.example");
    connection.disconnectFromBus(connection.name());
    QThread::quit();
  }

public slots:
  void          remoteCall(QByteArray message)
  {
    std::cout << "Message size: "  << message.size() << std::endl;
  }

};
