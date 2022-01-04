#include <QtCore>
#include <QtQuick>
#include <QtNetwork>
#include "globalobject.h"

#define CRYPT_MARK               "29f57a490bc9a17d"
#define CRYPT_PASS               "secucam"

#define TCP_PACKAGEMARK          "0123456789abcdef"
#define TCP_DEFAULT_COMMPASS     "password"
#define TCP_DEFAULT_USERNAME     "user"
#define TCP_DEFAULT_PORT         50000
#define TCP_GETMOTION            "GM"
#define TCP_SETMOTIONUP          "MU"
#define TCP_SETMOTIONDOWN        "MD"
#define TCP_GETTILT              "GT"
#define TCP_SETTILT              "ST"
#define TCP_GETPAN               "GP"
#define TCP_SETPAN               "SP"
#define TCP_GETSETUP             "GS"
#define TCP_SETSETUP             "SS"
#define TCP_GETHISTORY           "GH"

CGlobalObject::CGlobalObject(QQmlEngine *pEngine) : QObject(pEngine)
{
  //Set engine for future use
  m_pEngine=pEngine;

  //Setup cryptography
  m_Crypt.setMark(QByteArray::fromHex(CRYPT_MARK));
  m_Crypt.setCompressionMode(CCrypt::CompressionNo);
  m_Crypt.setEncryptionMode(CCrypt::SIM128);
  m_Crypt.setPass(QString(CRYPT_PASS).toUtf8());

  //Load parameters from configuration file
  QSettings Settings;
  Settings.beginGroup("General");
  setCommPass(m_Crypt.decryptArray(QByteArray::fromBase64(Settings.value("commpass").toByteArray())));
  setUsername(m_Crypt.decryptArray(QByteArray::fromBase64(Settings.value("username").toByteArray())));
  m_sPassword=m_Crypt.decryptArray(QByteArray::fromBase64(Settings.value("password").toByteArray()));
  setUrlHostIP(m_Crypt.decryptArray(QByteArray::fromBase64(Settings.value("urlhost").toByteArray())));
  setTcpPortSel((quint16)m_Crypt.decryptArray(QByteArray::fromBase64(Settings.value("tcpport").toByteArray())).toInt());
  Settings.endGroup();

  //Initialize parameters for the firt time
  if (commPass().isEmpty()) setCommPass(TCP_DEFAULT_COMMPASS);
  if (username().isEmpty()) setUsername(TCP_DEFAULT_USERNAME);
  if (tcpPortSel()==0) setTcpPortSel(TCP_DEFAULT_PORT);

  //Initialize TCP socket
  m_TcpClient.setPackageMark(QByteArray::fromHex(TCP_PACKAGEMARK));

  //Setup list model - AuthMethod
  m_lstAuthMethod.append("Plain");
  m_lstAuthMethod.append("Login");
  m_pEngine->rootContext()->setContextProperty("modelAuthMethod",m_lstAuthMethod);

  //Setup list model - ConnType
  m_lstConnType.append("Plain");
  m_lstConnType.append("SSL");
  m_lstConnType.append("TSL");
  m_pEngine->rootContext()->setContextProperty("modelConnType",m_lstConnType);

  //Setup table model - History
  m_ModelHistory.setProxyModel(3);
  m_pEngine->rootContext()->setContextProperty("modelHistory",&m_ModelHistory);
}

CGlobalObject::~CGlobalObject()
{
  //Save parameters to configuration file
  QSettings Settings;
  Settings.beginGroup("General");
  Settings.setValue("commpass", m_Crypt.encryptArray(commPass().toUtf8()).toBase64());
  Settings.setValue("username", m_Crypt.encryptArray(username().toUtf8()).toBase64());
  Settings.setValue("password", m_Crypt.encryptArray(m_sPassword.toUtf8()).toBase64());
  Settings.setValue("urlhost", m_Crypt.encryptArray(urlHostIP().toUtf8()).toBase64());
  Settings.setValue("tcpport", m_Crypt.encryptArray(QByteArray::number(tcpPortSel())).toBase64());
  Settings.endGroup();
}

//********************** GLOBAL SCOPE ******************************//

bool CGlobalObject::isPassword(const QString& sPass)
{
  //First start - Set up the password
  if (m_sPassword.isEmpty()) m_sPassword=sPass;

  //Validate password
  if (sPass!=m_sPassword)
  {
    m_sMessage=tr("Wrong password. Please try again.");
    return false;
  }

  return true;
}

void CGlobalObject::setPassword(const QString& sPass)
{
  m_sPassword=sPass;
}

bool CGlobalObject::existPassword()
{
  return !m_sPassword.isEmpty();
}

QString CGlobalObject::message()
{
  return m_sMessage;
}

//********************** INITPAGE SCOPE ******************************//

void CGlobalObject::initInitpage()
{
  setUrlHostIPSel(urlHostIP());
  setCommPassSel(commPass());
}

bool CGlobalObject::validateParam()
{
  if (urlHostIPSel().isEmpty() || commPassSel().isEmpty() || tcpPortSel()==0) return false;
  return true;
}

//********************** CONTROL PANEL SCOPE ******************************//

bool CGlobalObject::readMotion()
{
  //Prepare the request to the server
  QByteArray BAData;
  QDataStream DataRequest(&BAData, QIODevice::WriteOnly);
  DataRequest.setVersion(QDataStream::Qt_5_0);
  DataRequest << QString(TCP_GETMOTION);

  //Send request
  if (!serverRequest(BAData)) return false;

  //Read the reply from the server
  QDataStream DataReply(m_TcpClient.serverReply());
  DataReply.setVersion(QDataStream::Qt_5_0);
  QString sRead;
  DataReply >> sRead;

  //Server has replied ERR
  if (sRead!="OK")
  {
    DataReply >> sRead;
    m_sMessage=sRead;
    return false;
  }

  //Server has replied OK
  bool bActive;
  DataReply >> sRead;
  DataReply >> bActive;
  setActive(bActive);
  if (sRead=="Yes")
    setMotion(true);
  else
    setMotion(false);

  return true;
}

bool CGlobalObject::writeMotion(bool bMotion, bool bActive)
{
  //Prepare the request to the server
  QByteArray BAData;
  QDataStream DataRequest(&BAData, QIODevice::WriteOnly);
  DataRequest.setVersion(QDataStream::Qt_5_0);
  if (bMotion)
  {
    DataRequest << QString(TCP_SETMOTIONUP);
    DataRequest << username();
    DataRequest << bActive;
  }
  else
  {
    DataRequest << QString(TCP_SETMOTIONDOWN);
    DataRequest << username();
  }

  //Send request
  if (!serverRequest(BAData)) return false;

  //Read the reply from the server
  QDataStream DataReply(m_TcpClient.serverReply());
  DataReply.setVersion(QDataStream::Qt_5_0);
  QString sRead;
  DataReply >> sRead;

  //Server has replied ERR
  if (sRead!="OK")
  {
    DataReply >> sRead;
    m_sMessage=sRead;
    return false;
  }

  //Server has replied OK
  return true;
}

bool CGlobalObject::readTilt()
{
  //Prepare the request to the server
  QByteArray BAData;
  QDataStream DataRequest(&BAData, QIODevice::WriteOnly);
  DataRequest.setVersion(QDataStream::Qt_5_0);
  DataRequest << QString(TCP_GETTILT);

  //Send request
  if (!serverRequest(BAData)) return false;

  //Read the reply from the server
  QDataStream DataReply(m_TcpClient.serverReply());
  DataReply.setVersion(QDataStream::Qt_5_0);
  QString sRead;
  DataReply >> sRead;

  //Server has replied ERR
  if (sRead!="OK")
  {
    DataReply >> sRead;
    m_sMessage=sRead;
    return false;
  }

  //Server has replied OK
  qint16 i16Tilt;
  DataReply >> i16Tilt;
  setTilt(i16Tilt);
  return true;
}

bool CGlobalObject::writeTilt(int iTilt)
{
  //Prepare the request to the server
  QByteArray BAData;
  QDataStream DataRequest(&BAData, QIODevice::WriteOnly);
  DataRequest.setVersion(QDataStream::Qt_5_0);
  DataRequest << QString(TCP_SETTILT);
  DataRequest << username();
  DataRequest << qint16(iTilt);

  //Send request
  if (!serverRequest(BAData)) return false;

  //Read the reply from the server
  QDataStream DataReply(m_TcpClient.serverReply());
  DataReply.setVersion(QDataStream::Qt_5_0);
  QString sRead;
  DataReply >> sRead;

  //Server has replied ERR
  if (sRead!="OK")
  {
    DataReply >> sRead;
    m_sMessage=sRead;
    return false;
  }

  //Server has replied OK
  return true;
}

bool CGlobalObject::readPan()
{
  //Prepare the request to the server
  QByteArray BAData;
  QDataStream DataRequest(&BAData, QIODevice::WriteOnly);
  DataRequest.setVersion(QDataStream::Qt_5_0);
  DataRequest << QString(TCP_GETPAN);

  //Send request
  if (!serverRequest(BAData)) return false;

  //Read the reply from the server
  QDataStream DataReply(m_TcpClient.serverReply());
  DataReply.setVersion(QDataStream::Qt_5_0);
  QString sRead;
  DataReply >> sRead;

  //Server has replied ERR
  if (sRead!="OK")
  {
    DataReply >> sRead;
    m_sMessage=sRead;
    return false;
  }

  //Server has replied OK
  qint16 i16Pan;
  DataReply >> i16Pan;
  setPan(i16Pan);
  return true;
}

bool CGlobalObject::writePan(int iPan)
{
  //Prepare the request to the server
  QByteArray BAData;
  QDataStream DataRequest(&BAData, QIODevice::WriteOnly);
  DataRequest.setVersion(QDataStream::Qt_5_0);
  DataRequest << QString(TCP_SETPAN);
  DataRequest << username();
  DataRequest << qint16(iPan);

  //Send request
  if (!serverRequest(BAData)) return false;

  //Read the reply from the server
  QDataStream DataReply(m_TcpClient.serverReply());
  DataReply.setVersion(QDataStream::Qt_5_0);
  QString sRead;
  DataReply >> sRead;

  //Server has replied ERR
  if (sRead!="OK")
  {
    DataReply >> sRead;
    m_sMessage=sRead;
    return false;
  }

  //Server has replied OK
  return true;
}

//********************** SETTINGS SCOPE ******************************//

bool CGlobalObject::readSetup()
{
  //Prepare the request to the server
  QByteArray BAData;
  QDataStream DataRequest(&BAData, QIODevice::WriteOnly);
  DataRequest.setVersion(QDataStream::Qt_5_0);
  DataRequest << QString(TCP_GETSETUP);

  //Send request
  if (!serverRequest(BAData)) return false;

  //Read the reply from the server
  QDataStream DataReply(m_TcpClient.serverReply());
  DataReply.setVersion(QDataStream::Qt_5_0);
  QString sRead;
  DataReply >> sRead;

  //Server has replied ERR
  if (sRead!="OK")
  {
    DataReply >> sRead;
    m_sMessage=sRead;
    return false;
  }

  //Server has replied OK
  quint16 u16Read;
  DataReply >> sRead;
  setUrlDDNS(sRead);
  DataReply >> sRead;
  setSmtpHost(sRead);
  DataReply >> sRead;
  setSmtpUser(sRead);
  DataReply >> sRead;
  setSmtpPass(sRead);
  DataReply >> u16Read;
  m_u16SmtpConnType=u16Read;
  setSmtpConnType(m_lstConnType.at(u16Read));
  DataReply >> u16Read;
  m_u16SmtpAuthMethod=u16Read;
  setSmtpAuthMethod(m_lstAuthMethod.at(u16Read));
  DataReply >> u16Read;
  setSmtpPort(u16Read);
  return true;
}

bool CGlobalObject::writeSetup(const QString &sUrlDDNS, const QString &sSmtpHost, const QString &sSmtpUser, const QString &sSmtpPass, quint16 u16SmtpPort, const QString &sCommPass, const QString &sUsername, const QString &sUrlHostIP)
{
  //Prepare the request to the server
  QByteArray BAData;
  QDataStream DataRequest(&BAData, QIODevice::WriteOnly);
  DataRequest.setVersion(QDataStream::Qt_5_0);
  DataRequest << QString(TCP_SETSETUP);
  DataRequest << username();
  DataRequest << sUrlDDNS;
  DataRequest << sSmtpHost;
  DataRequest << sSmtpUser;
  DataRequest << sSmtpPass;
  DataRequest << m_u16SmtpConnType;
  DataRequest << m_u16SmtpAuthMethod;
  DataRequest << u16SmtpPort;
  DataRequest << sCommPass;

  //Send request
  if (!serverRequest(BAData)) return false;

  //Read the reply from the server
  QDataStream DataReply(m_TcpClient.serverReply());
  DataReply.setVersion(QDataStream::Qt_5_0);
  QString sRead;
  DataReply >> sRead;

  //Server has replied ERR
  if (sRead!="OK")
  {
    DataReply >> sRead;
    m_sMessage=sRead;
    return false;
  }

  //Server has replied OK
  setCommPass(sCommPass);
  setUsername(sUsername);
  setUrlHostIP(sUrlHostIP);

  return true;
}

//********************** HISTORY SCOPE ******************************//

bool CGlobalObject::readHistory()
{
  //Reset model
  m_ModelHistory.clear();

  //Prepare the request to the server
  QByteArray BAData;
  QDataStream DataRequest(&BAData, QIODevice::WriteOnly);
  DataRequest.setVersion(QDataStream::Qt_5_0);
  DataRequest << QString(TCP_GETHISTORY);

  //Send request
  if (!serverRequest(BAData)) return false;

  //Read the reply from the server
  QDataStream DataReply(m_TcpClient.serverReply());
  DataReply.setVersion(QDataStream::Qt_5_0);
  QString sRead;
  DataReply >> sRead;

  //Server has replied ERR
  if (sRead!="OK")
  {
    DataReply >> sRead;
    m_sMessage=sRead;
    return false;
  }

  //Server has replied OK
  qint64 i64Size;
  DataReply >> i64Size;
  for (qint64 i=0;i<i64Size;i++)
  {
    QList<QStandardItem*> lstRow;
    qlonglong lRead;
    DataReply >> lRead;
    lstRow.append(new QStandardItem(QString::number(lRead)));
    DataReply >> sRead;
    lstRow.append(new QStandardItem(sRead));
    DataReply >> sRead;
    lstRow.append(new QStandardItem(sRead));
    m_ModelHistory.insertRow(i,lstRow);
  }

  return true;
}

QString CGlobalObject::formatDate(qint64 i64Msecs) const
{
  QDateTime DateTime = QDateTime::fromMSecsSinceEpoch(i64Msecs);
  return DateTime.toString(Qt::ISODate).left(10);
}

QString CGlobalObject::formatTime(qint64 i64Msecs) const
{
  QDateTime DateTime = QDateTime::fromMSecsSinceEpoch(i64Msecs);
  return DateTime.toString(Qt::ISODate).right(8);
}

//***********************************************************************//

bool CGlobalObject::serverRequest(const QByteArray &BAData)
{
  m_TcpClient.setPassword(commPassSel());
  if (!m_TcpClient.connectToServer(urlHostIPSel(),tcpPortSel()))
  {
    m_sMessage=tr("The security camera is out of reach.\nThe internet connection might be down in either side or the URL is wrong.");
    return false;
  }

  if (!m_TcpClient.sendMessage(BAData))
  {
    m_sMessage=tr("The instruction could not be delivered.\nYour internet connection might be down or the URL is wrong.");
    m_TcpClient.disconnectFromServer();
    return false;
  }

  if (!m_TcpClient.waitForResponse())
  {
    switch (m_TcpClient.lastError())
    {
      case CTcpClient::ErrorSocketTimeoutReadyRead:
        m_sMessage=tr("The request was sent, but the password is incorrect.\nOr the security camera took longer than expected.");
        break;
      case CTcpClient::ErrorSocketPassword:
      case CTcpClient::ErrorSocketPackageMark:
        m_sMessage=tr("The request was sent, but the package received is not correct or it is corrupted.");
        break;
      default:
        m_sMessage.clear();
        break;
    }

    m_TcpClient.disconnectFromServer();
    return false;
  }

  m_TcpClient.disconnectFromServer();
  return true;
}
