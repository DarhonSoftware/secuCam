#include <QtCore>
#include <QtSql>
#include "dr_smtpclient.h"

#define DATABASE_NAME             "scserver"
#define DATABASE_DIR              ".database"

int main(int argc, char *argv[])
{
  QCoreApplication App(argc, argv);

  //Set application names
  QCoreApplication::setOrganizationName("Darhon");
  QCoreApplication::setApplicationName("secuCam-server");
  QCoreApplication::setApplicationVersion("1.1.0");
  QCoreApplication::setOrganizationDomain("www.darhon.com");

  //Get database file
  QString sFile, sAppDir;
  sAppDir=QCoreApplication::applicationDirPath();
  sFile=QDir::cleanPath(QString("%1/%2/%3").arg(sAppDir).arg(DATABASE_DIR).arg(DATABASE_NAME));

  //Setup and open database
  QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
  db.setDatabaseName(QDir::toNativeSeparators(sFile));
  if (!db.open()) return -1;

  //Open Table to collect parameters
  QSqlTableModel Table;
  Table.setTable("settings");
  Table.select();
  QSqlRecord Record=Table.record(0);

  QString sHost=Record.value(4).toString();
  QString sUser=Record.value(5).toString();
  QString sPass=Record.value(6).toString();
  quint16 u16ConnType=Record.value(7).toInt();
  quint16 u16AuthMethod=Record.value(8).toInt();
  quint16 u16Port=Record.value(9).toInt();

  //Close database
  db.close();

  //Validate parameters
  if ((sHost.isEmpty())||(sUser.isEmpty())||(sPass.isEmpty())||(u16Port==0)) return -1;

  //Initialise ConnType
  CSmtpClient SmtpClient;
  switch (u16ConnType)
  {
    case 0:
      SmtpClient.setConnectionType(CSmtpClient::TcpConnection);
      break;
    case 1:
      SmtpClient.setConnectionType(CSmtpClient::SslConnection);
      break;
    case 2:
      SmtpClient.setConnectionType(CSmtpClient::TlsConnection);
      break;
    default:
      break;
  }

  //Initialise AuthMethod
  switch (u16AuthMethod)
  {
    case 0:
      SmtpClient.setAuthMethod(CSmtpClient::AuthPlain);
      break;
    case 1:
      SmtpClient.setAuthMethod(CSmtpClient::AuthLogin);
      break;
    default:
      break;
  }

  //Initialise remaining
  SmtpClient.setHost(sHost);
  SmtpClient.setPort(u16Port);
  SmtpClient.setName("secucam");
  SmtpClient.setUser(sUser);
  SmtpClient.setPassword(sPass);

  //Prepare body of email
  QDateTime DateTime=QDateTime::currentDateTime();
  QString sBody=QCoreApplication::translate("Main","Motion was detected and the camera was activated.")+"\n"+
                QCoreApplication::translate("Main","A video will be created from the time indicated below.")+"\n"+
                DateTime.toString(Qt::SystemLocaleLongDate);
  QString sSubject=QString("Motion detected %1  %2").arg(DateTime.toString(Qt::ISODate).left(10)).arg(DateTime.toString(Qt::ISODate).right(8));

  //Send email
  if (!SmtpClient.connectToHost()) return -1;
  if (!SmtpClient.login()) return -1;
  if (!SmtpClient.sendMail(sUser,"secuCam",sUser,"secuCam",sSubject,sBody)) return -1;
  if (!SmtpClient.disconnectFromHost()) return -1;
}

