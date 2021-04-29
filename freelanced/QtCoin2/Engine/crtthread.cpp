#include <QMutex>
#include <QDebug>
#include "../Engine/Engine.h"
#include "../Engine/global.h"
#include "crtthread.h"

CRTThread::CRTThread(QObject *parent, Engine *pEngine):QThread(parent)
{
    m_pEngine = pEngine;
}

void CRTThread::run()
{
    m_bRun = true;
    while(m_bRun)
    {
        QMutex mutex;
        mutex.lock();
        if(!m_bRun)
            break;
        mutex.unlock();

        scanFile();
        this->msleep(500);
    }
}

void CRTThread::scanFile()
{
    QDir dir(RXTX);
    QFileInfoList list = dir.entryInfoList(QDir::AllEntries);
    QDateTime curtime = QDateTime::currentDateTime();
    for(int i = 0; i < list.count(); i++)
    {
        QString filename = list.at(i).fileName();
        if(filename == "." || filename == "..")
            continue;
        QString strPath = list.at(i).filePath();
        processFile(strPath, filename);
    }
}
void CRTThread::processFile(QString strPath, QString strName)
{
    // validate the rx file
    RXTX_File res = m_pEngine->validateTxRXFile(strPath);
    if(res.nType < 0)
    {
        QDir().remove(strPath);
        return;
    }
    // make workspace folder
    QString workspace_dir = QString("%1/%2_%3").arg(WORKSPACE).arg(strName).arg(res.strCreated);
    // check if already exists
    if(QDir(workspace_dir).exists())
    {
        qDebug()<<"Parse RX Error!";
        QDir().remove(strPath);
        return;
    }
    QDir().mkdir(workspace_dir);
    QDir().rename(strPath, QString("%1/%2").arg(workspace_dir).arg(CLIENT_FILE));
}
