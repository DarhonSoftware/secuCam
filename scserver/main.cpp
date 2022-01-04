#include <QtCore>
#include "globalobject.h"

int main(int argc, char *argv[])
{
  QCoreApplication App(argc, argv);

  //Set application names
  QCoreApplication::setOrganizationName("Darhon");
  QCoreApplication::setApplicationName("secuCam-server");
  QCoreApplication::setApplicationVersion("1.1.0");
  QCoreApplication::setOrganizationDomain("www.darhon.com");

  //Read arguments
  QStringList lstArguments=App.arguments();

  //Argument - Version
  if (lstArguments.contains("-v"))
  {
    QTextStream out(stdout);
    out<<"\n"+QString("%1 %2 (Qt - %3)").arg("secuCam").arg(qApp->applicationVersion()).arg(QT_VERSION_STR)+"\n\n";
    out.flush();
    return 0;
  }

  //Argument - Help
  if (lstArguments.contains("-h"))
  {
    QTextStream out(stdout);
    out<<"\n"+QCoreApplication::translate("main","usage:")+" scserver "+QCoreApplication::translate("main","[options]")+"\n\n";
    out<<QCoreApplication::translate("main","Possible options:")+"\n\n";
    out<<"   -h   "<<QCoreApplication::translate("main","Show this help.")+"\n";
    out<<"   -v   "<<QCoreApplication::translate("main","Version of secuCam server.")+"\n";
    out<<"   -p   "<<QCoreApplication::translate("main","Port where the server will listen.")+"\n";
    out<<"        "<<QCoreApplication::translate("main","Default 50000.")+"\n";
    out<<"   -i   "<<QCoreApplication::translate("main","Time interval refresh for your dynamic DNS (in miliseconds).")+"\n";
    out<<"        "<<QCoreApplication::translate("main","Default 300000. Minimum 60000.")+"\n";
    out<<"   -s   "<<QCoreApplication::translate("main","Maximum size of disk space to store video events (in bytes).")+"\n";
    out<<"        "<<QCoreApplication::translate("main","Default 10000000000 (10GB). Minimum=1000000000 (1GB).")+"\n";
    out<<"   -w   "<<QCoreApplication::translate("main","Enable servo capability. Two GPIOs will be configured to control tilt and pan.")+"\n";
    out<<"   -r   "<<QCoreApplication::translate("main","Range of servo's pulse width. Max Width - Min Width (in microseconds). Only valid if -w is set.")+"\n";
    out<<"        "<<QCoreApplication::translate("main","Default 1000 (1ms). Min 500 and Max 1600.")+"\n";
    out<<"   -t   "<<QCoreApplication::translate("main","Port where PiGpio will listen.")+"\n";
    out<<"        "<<QCoreApplication::translate("main","Default 8888.")+"\n";
    out<<"   -d   "<<QCoreApplication::translate("main","Debug mode to get additional info in the terminal.")+"\n\n";
    out.flush();
    return 0;
  }

  //Argument - Port
  quint16 u16Port=0;
  int iIndex=lstArguments.indexOf("-p");
  if (lstArguments.size()>=iIndex+2) u16Port=lstArguments.at(iIndex+1).toInt();

  //Argument - RefreshDDNSInterval
  qint64 i64RefreshDDNSInterval=0;
  iIndex=lstArguments.indexOf("-i");
  if (lstArguments.size()>=iIndex+2) i64RefreshDDNSInterval=lstArguments.at(iIndex+1).toLongLong();

  //Argument - MaxVideoEvents
  qint64 i64MaxVideoEvents=0;
  iIndex=lstArguments.indexOf("-s");
  if (lstArguments.size()>=iIndex+2) i64MaxVideoEvents=lstArguments.at(iIndex+1).toLongLong();

  //Argument - Servo capability
  bool bServo=false;
  if (lstArguments.contains("-w")) bServo=true;

  //Argument - ServoPulseRange
  int iServoPulseRange=0;
  iIndex=lstArguments.indexOf("-r");
  if (lstArguments.size()>=iIndex+2) iServoPulseRange=lstArguments.at(iIndex+1).toInt();

  //Argument - PiGpio Port
  quint16 u16PigpioPort=0;
  int iIndex=lstArguments.indexOf("-t");
  if (lstArguments.size()>=iIndex+2) u16PigpioPort=lstArguments.at(iIndex+1).toInt();

  //Argument - Debug mode
  bool bDebug=false;
  if (lstArguments.contains("-d")) bDebug=true;

  //Create global object
  CGlobalObject GlobalObject(u16Port,u16PigpioPort,i64RefreshDDNSInterval,i64MaxVideoEvents,bServo,iServoPulseRange,bDebug);

  //Exit application in case of unsuccessfull initialization
  if (!GlobalObject.initSuccessful()) return -1;

  return App.exec();
}
