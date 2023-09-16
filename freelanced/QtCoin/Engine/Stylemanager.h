#ifndef STYLEMANAGER_H
#define STYLEMANAGER_H
#include <QString>
#include <QVector>

class StyleManager
{
public:
    StyleManager();

public:
    QVector<QString> m_aStyles;
    void loadStyleSheet(QString);
    void initManager();
};

#endif // STYLEMANAGER_H
