#ifndef CGLOBALOBJECT_H
#define CGLOBALOBJECT_H

#include <QObject>
#include <QQmlEngine>
#include "dr_crypt.h"
#include "dr_standarditemmodel.h"
#include "dr_tcpclient.h"

class CGlobalObject : public QObject
{
  Q_OBJECT

  //Global scope - Properties
  Q_PROPERTY(QString commPass READ commPass WRITE setCommPass NOTIFY commPassChanged)
  Q_PROPERTY(QString username READ username WRITE setUsername NOTIFY usernameChanged)
  Q_PROPERTY(QString urlHostIP READ urlHostIP WRITE setUrlHostIP NOTIFY urlHostIPChanged)

  //Initpage scope - Properties
  Q_PROPERTY(QString urlHostIPSel READ urlHostIPSel WRITE setUrlHostIPSel NOTIFY urlHostIPSelChanged)
  Q_PROPERTY(QString commPassSel READ commPassSel WRITE setCommPassSel NOTIFY commPassSelChanged)
  Q_PROPERTY(quint16 tcpPortSel READ tcpPortSel WRITE setTcpPortSel NOTIFY tcpPortSelChanged)

  //Control Panel scope - Properties
  Q_PROPERTY(bool motion READ motion WRITE setMotion NOTIFY motionChanged)
  Q_PROPERTY(bool active READ active WRITE setActive NOTIFY activeChanged)
  Q_PROPERTY(int tilt READ tilt WRITE setTilt NOTIFY tiltChanged)
  Q_PROPERTY(int pan READ pan WRITE setPan NOTIFY panChanged)

  //Settings scope - Properties
  Q_PROPERTY(QString urlDDNS READ urlDDNS WRITE setUrlDDNS NOTIFY urlDDNSChanged)
  Q_PROPERTY(QString smtpHost READ smtpHost WRITE setSmtpHost NOTIFY smtpHostChanged)
  Q_PROPERTY(QString smtpUser READ smtpUser WRITE setSmtpUser NOTIFY smtpUserChanged)
  Q_PROPERTY(QString smtpPass READ smtpPass WRITE setSmtpPass NOTIFY smtpPassChanged)
  Q_PROPERTY(QString smtpConnType READ smtpConnType WRITE setSmtpConnType NOTIFY smtpConnTypeChanged)
  Q_PROPERTY(QString smtpAuthMethod READ smtpAuthMethod WRITE setSmtpAuthMethod NOTIFY smtpAuthMethodChanged)
  Q_PROPERTY(quint16 smtpPort READ smtpPort WRITE setSmtpPort NOTIFY smtpPortChanged)

  //Global scope - Properties
  QString m_sCommPass;
  QString m_sUsername;
  QString m_sUrlHostIP;

  //Initpage scope - Properties
  QString m_sUrlHostIPSel;
  QString m_sCommPassSel;
  quint16 m_u16TcpPortSel;

  //Control Panel scope - Properties
  bool m_bMotion;
  bool m_bActive;
  int m_iTilt;
  int m_iPan;

  //Settings scope - Properties
  QString m_sUrlDDNS;
  QString m_sSmtpHost;
  QString m_sSmtpUser;
  QString m_sSmtpPass;
  QString m_sSmtpConnType;
  QString m_sSmtpAuthMethod;
  quint16 m_u16SmtpPort;

  //General basic members
  QString m_sPassword;
  QString m_sMessage;
  quint16 m_u16SmtpConnType;
  quint16 m_u16SmtpAuthMethod;

  //General object members
  QQmlEngine *m_pEngine;
  CCrypt m_Crypt;
  CTcpClient m_TcpClient;

  //Model for lists
  QStringList m_lstConnType;
  QStringList m_lstAuthMethod;
  CStandardItemModel m_ModelHistory;

public:
  explicit CGlobalObject(QQmlEngine *pEngine);
  ~CGlobalObject();

  //Global scope - Properties
  void setCommPass(const QString &sCommPass) {m_sCommPass=sCommPass; emit commPassChanged();}
  QString commPass() const {return m_sCommPass;}
  void setUsername(const QString &sUsername) {m_sUsername=sUsername; emit usernameChanged();}
  QString username() const {return m_sUsername;}
  void setUrlHostIP(const QString &sUrlHostIP) {m_sUrlHostIP=sUrlHostIP; emit urlHostIPChanged();}
  QString urlHostIP() const {return m_sUrlHostIP;}

  //Initpage scope - Properties
  void setUrlHostIPSel(const QString &sUrlHostIPSel) {m_sUrlHostIPSel=sUrlHostIPSel; emit urlHostIPSelChanged();}
  QString urlHostIPSel() const {return m_sUrlHostIPSel;}
  void setCommPassSel(const QString &sCommPassSel) {m_sCommPassSel=sCommPassSel; emit commPassSelChanged();}
  QString commPassSel() const {return m_sCommPassSel;}
  void setTcpPortSel(quint16 u16TcpPortSel) {m_u16TcpPortSel=u16TcpPortSel; emit tcpPortSelChanged();}
  quint16 tcpPortSel() const {return m_u16TcpPortSel;}

  //Control Panel scope - Properties
  void setMotion(bool bMotion) {m_bMotion=bMotion; emit motionChanged();}
  bool motion() const {return m_bMotion;}
  void setActive(bool bActive) {m_bActive=bActive; emit activeChanged();}
  bool active() const {return m_bActive;}
  void setTilt(int iTilt) {m_iTilt=iTilt; emit tiltChanged();}
  int tilt() const {return m_iTilt;}
  void setPan(int iPan) {m_iPan=iPan; emit panChanged();}
  int pan() const {return m_iPan;}

  //Settings scope - Properties
  void setUrlDDNS(const QString &sUrlDDNS) {m_sUrlDDNS=sUrlDDNS; emit urlDDNSChanged();}
  QString urlDDNS() const {return m_sUrlDDNS;}
  void setSmtpHost(const QString &sSmtpHost) {m_sSmtpHost=sSmtpHost; emit smtpHostChanged();}
  QString smtpHost() const {return m_sSmtpHost;}
  void setSmtpUser(const QString &sSmtpUser) {m_sSmtpUser=sSmtpUser; emit smtpUserChanged();}
  QString smtpUser() const {return m_sSmtpUser;}
  void setSmtpPass(const QString &sSmtpPass) {m_sSmtpPass=sSmtpPass; emit smtpPassChanged();}
  QString smtpPass() const {return m_sSmtpPass;}
  void setSmtpConnType(const QString &sSmtpConnType) {m_sSmtpConnType=sSmtpConnType; emit smtpConnTypeChanged();}
  QString smtpConnType() const {return m_sSmtpConnType;}
  void setSmtpAuthMethod(const QString &sSmtpAuthMethod) {m_sSmtpAuthMethod=sSmtpAuthMethod; emit smtpAuthMethodChanged();}
  QString smtpAuthMethod() const {return m_sSmtpAuthMethod;}
  void setSmtpPort(quint16 u16SmtpPort) {m_u16SmtpPort=u16SmtpPort; emit smtpPortChanged();}
  quint16 smtpPort() const {return m_u16SmtpPort;}

signals:
  //Global scope - Properties
  void commPassChanged();
  void usernameChanged();
  void urlHostIPChanged();

  //Initpage scope - Properties
  void urlHostIPSelChanged();
  void commPassSelChanged();
  void tcpPortSelChanged();

  //Control Panel scope - Properties
  void motionChanged();
  void activeChanged();
  void tiltChanged();
  void panChanged();

  //Settings scope - Properties
  void urlDDNSChanged();
  void smtpHostChanged();
  void smtpUserChanged();
  void smtpPassChanged();
  void smtpConnTypeChanged();
  void smtpAuthMethodChanged();
  void smtpPortChanged();

public slots:
  //Global scope - Slots
  bool isPassword(const QString &sPass);
  void setPassword(const QString &sPass);
  bool existPassword();
  QString message();

  //Initpage scope - Slots
  void initInitpage();
  bool validateParam();

  //Control Panel scope - Slots
  bool readMotion();
  bool writeMotion(bool bMotion, bool bActive);
  bool readTilt();
  bool writeTilt(int iTilt);
  bool readPan();
  bool writePan(int iPan);

  //Settings scope - Slots
  bool readSetup();
  bool writeSetup(const QString &sUrlDDNS, const QString &sSmtpHost, const QString &sSmtpUser, const QString &sSmtpPass, quint16 u16SmtpPort, const QString &sCommPass, const QString &sUsername, const QString &sUrlHostIP);

  //History scope - Slots
  bool readHistory();
  QString formatDate(qint64 i64Msecs) const;
  QString formatTime(qint64 i64Msecs) const;

  //Setters to save item number in a list
  void setSmtpConnType(quint16 u16SmtpConnType) {m_u16SmtpConnType=u16SmtpConnType;}
  void setSmtpAuthMethod(quint16 u16SmtpAuthMethod) {m_u16SmtpAuthMethod=u16SmtpAuthMethod;}

private:
  bool serverRequest(const QByteArray &BAData);
};


//********************************************************* Communication protocol ***************************************************************//
// readMotion    -->  str(I)
// readMotion    <--  str(OK)+str(Yes||No)+bool(active) || str(ERR)+str(error)
// writeMotion   -->  str(I)+str(username)+bool(*active) *only for SETMOTIONUP
// writeMotion   <--  str(OK) || str(ERR)+str(error)
// readTilt      -->  str(I)
// readTilt      <--  str(OK)+qint16(tilt) || str(ERR)+str(error)
// writeTilt     -->  str(I)+str(username)+qint16(tilt)
// writeTilt     <--  str(OK) || str(ERR)+str(error)
// readPan       -->  str(I)
// readPan       <--  str(OK)+qint16(tilt) || str(ERR)+str(error)
// writePan      -->  str(I)+str(username)+qint16(tilt)
// writePan      <--  str(OK) || str(ERR)+str(error)
// readSetup    -->   str(I)
// readSetup    <--   str(OK)+str(urlddns)+str(smtphost)+str(smtpuser)+str(smtppass)+quint16(smtpconntype)+quint16(smtpauthmethod)+quint16(smtport)
//                    || str(ERR)+str(error)
// writeSetup   -->   str(I)+str(username)+str(urlddns)+str(smtphost)+str(smtpuser)+str(smtppass)+quint16(smtpconntype)+quint16(smtpauthmethod)+quint16(smtport)+str(commpass)
// writeSetup   <--   str(OK) || str(ERR)+str(error)
// readHistory  -->   str(I)
// readHistory  <--   str(OK)+qint64(size)+[qint64(datetime)+str(user)+str(desc)] || str(ERR)+str(error)
//***********************************************************************************************************************************************//


#endif // CGLOBALOBJECT_H
