#ifndef CRTTHREAD_H
#define CRTTHREAD_H

#include <QThread>
#include <QDateTime>


class Engine;

class CRTThread : public QThread
{
    Q_OBJECT
public:
    explicit CRTThread(QObject*parent=0, Engine*engine=0);
public:
    bool    m_bRun;
    void    run();
private:
    void    scanFile();
    void    processFile(QString strPath, QString strName);
signals:
    void    processed(QString);
private:
    Engine              *m_pEngine;
};

#endif // CRTTHREAD_H
