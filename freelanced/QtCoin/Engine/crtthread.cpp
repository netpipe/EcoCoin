#include <QMutex>
#include <QDebug>
#include "Engine/Engine.h"
#include "Engine/global.h"
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
        processFile();
        this->msleep(500);

    }
}

void CRTThread::processFile()
{
    for(int i = 0; i < m_Files.count(); i++)
    {
        if(m_Files.at(i)->nStatus != RAW)
            continue;
        m_Files.at(i)->nStatus = PROCESSING;
        emit processed(m_Files.at(i)->fileName);

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
        int nIndex = -1;
        for(int j = 0; j < m_Files.count(); j++)
        {
            if(m_Files.at(j)->fileName == filename)
            {
                nIndex = j;
                if(m_Files.at(j)->nStatus != PROCESSED)
                    m_Files.at(j)->lastTime = curtime;
                break;
            }
        }
        if(nIndex < 0)
        {
            RTFile *newfile = new RTFile;
            // tx_aaa_bbb | tx_aaa_bbb_ok
            if(filename.split('_').length() < 3)
            {
                try {
                    QDir().remove(list.at(i).filePath());
                    continue;
                } catch (exception) {

                }
            }
            newfile->fileName = filename;
            newfile->startTime = curtime;
            newfile->lastTime = curtime;

            newfile->nStatus = RAW;
            if(filename.split('_').last() == "ok")
            {
                newfile->nStatus = PROCESSED;
                QString oldname = filename.replace("_ok","");
                m_pEngine->RemoveRXTX(oldname);

            }
            m_Files.push_back(newfile);
        }
    }
    for(int i = 0; i < m_Files.size(); i++)
    {
        if(m_Files[i]->nStatus != PROCESSED)
        {
            if(m_Files.at(i)->lastTime.msecsTo(curtime) > 0)
            {
                RTFile *file = m_Files.at(i);
                m_Files.removeAt(i);
                delete file;
                i -= 1;
            }
        }else
        {
            if(m_Files.at(i)->lastTime.secsTo(curtime) > 10)
            {
                RTFile *file = m_Files.at(i);
                m_Files.removeAt(i);
                delete file;
                i -= 1;
            }
        }
    }
}
