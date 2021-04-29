#include "Common.h"
Common* Common::_instance = 0;
QString Common::USB_DriveName = "MyUSB";

Common::Common()
{

}

Common* Common::instance()
{
    if(!_instance)
        makeInstance();
    return _instance;
}

void Common::makeInstance(){
    _instance = new Common;
}
void Common::deleteInstance(){ delete _instance; }

void Common::MessageBox(QString str)
{
    QMessageBox Msgbox;
    Msgbox.setText(str);
    Msgbox.exec();
}
