#include "Stylemanager.h"
#include <QFile>
#include <QTextStream>
#include <QDirIterator>
#include <exception>
#include <QDebug>
#include <QApplication>
#include "global.h"

StyleManager::StyleManager()
{
    initManager();
}
void StyleManager::loadStyleSheet(QString strTheme)
{
    QString sheet_name = QString("%1.qss").arg(strTheme);
    QString full_path = "";
    for (int i = 0; i < m_aStyles.count(); i++)
    {
        if(m_aStyles.at(i).contains(sheet_name))
        {
            full_path = m_aStyles.at(i);
            break;
        }
    }
    if (full_path.length() == 0)
        return;
    QFile file(full_path); // Modified by RCU 2016.07.17
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());

    qApp->setStyleSheet(styleSheet);
}
void StyleManager::initManager()
{
    try {
        m_aStyles.clear();
        QDirIterator it("./Resource/themes/", QStringList() << "*.qss", QDir::Files, QDirIterator::Subdirectories);
        while (it.hasNext())
        {
            m_aStyles.push_back(it.next());
        }
    } catch (exception e) {
        qDebug("themeInit Err:%s", e.what());
    }
}
