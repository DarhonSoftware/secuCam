#include <QtGui>
#include <QtQuick>
#include <QQuickStyle>
#include "globalobject.h"
#include "about.h"

int main(int argc, char *argv[])
{
#ifdef Q_OS_ANDROID
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

  //Initiate main objects
  QGuiApplication App(argc, argv);

  //Initialise modules
  QQuickStyle::setStyle("Material");

  //Set application names
  QCoreApplication::setOrganizationName("Darhon");
  QCoreApplication::setApplicationName("secuCam-client");
  QCoreApplication::setApplicationVersion("1.2.0");
  QCoreApplication::setOrganizationDomain("www.darhon.com");

  //Setup translations
//  QTranslator Translator;
//  QString sLanguage = QLocale::system().name();
//  Translator.load(QString("safebox_") + sLanguage,":/translations");
//  App.installTranslator(&Translator);

  //Register QML Object Types
  qmlRegisterType<CAbout>("com.darhon", 1, 0, "About");

  //Initiate Engine
  QQmlApplicationEngine Engine;

  //Initiate global objects
  CGlobalObject *pGlobalObject= new CGlobalObject(&Engine);

  //Set up context
  Engine.rootContext()->setContextProperty("globalObject",pGlobalObject);

  //Initiate GUI
  Engine.load(QUrl("qrc:/src/qml/qtcontrols/main.qml"));

  return App.exec();
}
