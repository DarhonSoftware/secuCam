#include <QtGui>
#include <QtQuick>
#include "about.h"
#include "globalobject.h"

CAbout::CAbout(QObject *pObject) :  QObject(pObject)
{
  init();
}

CAbout::~CAbout()
{
}

void CAbout::init()
{
  setIconAbout("qrc:/image/icons/about.png");
  setIconBack("qrc:/image/icons/button-back.png");
  setAppName(QString("%1 %2 (Qt - %3)").arg("secuCam",qApp->applicationVersion(),QT_VERSION_STR));
  setCaption(tr("Remote control for security camera"));
  setDescription(tr("secuCam (server) powers your Raspberry Pi to detect motion and email events.\n")+
                 tr("secuCam (client) communicates with the camera to access all features."));
  setIconDarhon("qrc:/image/icons/darhon.png");
  setCopyright(tr("Copyright 2015-2016 - Darhon Software"));
  setSupport(tr("Help & Support") + " - " + QCoreApplication::organizationDomain());
  setPrivacy(QString("<a href='http://www.darhon.com/faq/345'>%1</a>").arg(tr("Privacy Policy")));
  setAppCode("");
  setVersion(tr("Gold version"));
  setActCode("");
  setCodesEnabled(false);
}


