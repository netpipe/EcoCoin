#ifndef COMMON_H
#define COMMON_H
#include "Engine/Engine.h"
#include <QString>
#include <QMessageBox>

class Common
{
public:
    Common();
    static Common* instance();
    static void makeInstance();
    static void deleteInstance();
    static void MessageBox(QString str);

    static QString USB_DriveName;
public:
    static Common* _instance;
    Engine *g_Engine;
};

#endif // COMMON_H
