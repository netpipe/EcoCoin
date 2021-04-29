#ifndef CRTTHREAD_H
#define CRTTHREAD_H

#include <QThread>
#include <QDateTime>


class Engine;
enum RXSTATUS{
    RAW,
    PROCESSING,
    PROCESSED
};
struct RTFile
{
    QString     fileName;
    QDateTime   startTime;
    QDateTime   lastTime;
    int         nStatus;
};

class CRTThread : public QThread
{
    Q_OBJECT
public:
    explicit CRTThread(QObject*parent=0, Engine*engine=0);
public:
    bool    m_bRun;
    void    run();
private:
    void    processFile();
    void    scanFile();
signals:
    void    processed(QString);
private:
    Engine              *m_pEngine;
    QList<RTFile*>      m_Files;
};

#endif // CRTTHREAD_H
