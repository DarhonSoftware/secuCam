#ifndef CGLOBALOBJECT_H
#define CGLOBALOBJECT_H

#include <QObject>
#include <QSqlDatabase>
#include <QProcess>
#include <QTimer>
#include <QNetworkAccessManager>
#include "dr_tcpserver.h"

class CGlobalObject : public QObject
{
  Q_OBJECT

  QNetworkAccessManager m_Manager;
  QSqlDatabase m_db;
  CTcpServer m_TcpServer;
  QProcess m_ProcessMotion;
  QTimer m_TimerCleanup;
  QTimer m_TimerDDNS;
  qint64 m_i64MaxVideoEvents;
  int m_iPigpioID;
  int m_iServoPulseRange;
  int m_iTilt;
  int m_iPan;
  bool m_bDebug;
  bool m_bInitSuccessful;
  bool m_bActive;

public:
  explicit CGlobalObject(quint16 u16Port, quint16 u16PigpioPort, qint64 i64RefreshDDNSInterval, qint64 i64MaxVideoEvents, bool bServo, int iServoPulseRange, bool bDebug, QObject *pObject = 0);
  ~CGlobalObject();
  bool initSuccessful() {return m_bInitSuccessful;}
  void printMessage(const QString &sType, const QString &sMessage);

private:
  //Database scope
  bool initDatabase();
  bool upgradeDatabase();
  void endDatabase();
  QString databaseLocation(QString *psDir);
  qulonglong generateId(const QString &sTable);
  void addHistory(const QString &sUser, const QString &sDesc);

  //Servo control
  QString commandServo(int iGpio, int iNewVal, int *piCurrentVal);

signals:

private slots:
  void message(QTcpSocket* pSocket,const QByteArray &BAData);
  void refreshDDNS();
  void cleanupEvents();
  void replyFinished(QNetworkReply*pReply);
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
