#include <QtCore>
#include <QtSql>
#include <QtNetwork>
#include "pigpiod_if2.h"
#include "globalobject.h"
#include "dr_sql.h"

#define DB_VER                    100
#define MAX_HISTORY               1000
#define COMMAND_MOTION            "motion-mmal"
#define COMMAND_GPIO              "scgpio"
#define CONF_FILE_MOTIONA         "config/motiona.conf"
#define CONF_FILE_MOTIONP         "config/motionp.conf"
#define DATABASE_NAME             "scserver"
#define DATABASE_DIR              ".database"

#define EVENT_VIDEO_DIR           "event-videos"
#define EVENT_MAXVIDEO_DEF        10000000000
#define EVENT_MAXVIDEO_MIN        1000000000

#define TIMEOUT_DDNS_DEF          300000
#define TIMEOUT_DDNS_MIN          60000
#define TIMEOUT_CLEANUP           3600000

#define SERVO_PULSE_PERIOD        20
#define SERVO_PULSE_RANGE_MIN     500
#define SERVO_PULSE_RANGE_MAX     1600
#define SERVO_PULSE_RANGE_DEF     1000
#define SERVO_PULSE_W_NEUTRAL     1500
#define SERVO_GPIO_TILT           23
#define SERVO_GPIO_PAN            24
#define SERVO_PORT_DEF            8888

#define TCP_PACKAGEMARK           "0123456789abcdef"
#define TCP_COMMPASS_DEF          "password"
#define TCP_PORT_DEF              50000
#define TCP_GETMOTION             "GM"
#define TCP_SETMOTIONUP           "MU"
#define TCP_SETMOTIONDOWN         "MD"
#define TCP_GETTILT               "GT"
#define TCP_SETTILT               "ST"
#define TCP_GETPAN                "GP"
#define TCP_SETPAN                "SP"
#define TCP_GETSETUP              "GS"
#define TCP_SETSETUP              "SS"
#define TCP_GETHISTORY            "GH"


CGlobalObject::CGlobalObject(quint16 u16Port, quint16 u16PigpioPort, qint64 i64RefreshDDNSInterval, qint64 i64MaxVideoEvents, bool bServo, int iServoPulseRange, bool bDebug, QObject *pObject) : QObject(pObject)
{
  //Set InitSuccessfull in case an error is found
  m_bInitSuccessful=false;

  //Set the debug mode
  m_bDebug=bDebug;

  //Set active mode
  m_bActive=false;

  //Init database
  m_db = QSqlDatabase::addDatabase("QSQLITE");
  if (!initDatabase()) return;
  if (m_bDebug) printMessage(tr("Debug"),tr("Database was loaded successfully."));

  //Read table settings
  QSqlTableModel Table;
  Table.setTable("settings");
  Table.select();

  //Init TCP socket
  m_TcpServer.setPackageMark(QByteArray::fromHex(TCP_PACKAGEMARK));
  m_TcpServer.setPassword(Table.record(0).value(2).toString());
  m_TcpServer.setHost(QHostAddress::Any);
  if (u16Port==0)
    m_TcpServer.setPort(TCP_PORT_DEF);
  else
    m_TcpServer.setPort(u16Port);
  if (m_bDebug) printMessage(tr("Debug"),tr("Port is set to ")+QString::number(m_TcpServer.port()));
  if (!m_TcpServer.startServer())
  {
    printMessage(tr("Warning"),tr("Socket error while starting: ")+QString::number(m_TcpServer.lastError()));
    return;
  }
  if (m_bDebug) printMessage(tr("Debug"),tr("Socket is listening for connections."));

  //Set Working directory for motion
  m_ProcessMotion.setWorkingDirectory(QCoreApplication::applicationDirPath());

  //Set timer - DDNS refresh
  if (i64RefreshDDNSInterval<TIMEOUT_DDNS_MIN)
  {
    m_TimerDDNS.start(TIMEOUT_DDNS_DEF);
    if (m_bDebug) printMessage(tr("Debug"),tr("DDNS refresh interval is set to [miliseconds] ")+QString::number(TIMEOUT_DDNS_DEF));
  }
  else
  {
    m_TimerDDNS.start(i64RefreshDDNSInterval);
    if (m_bDebug) printMessage(tr("Debug"),tr("DDNS refresh interval is set to [miliseconds] ")+QString::number(i64RefreshDDNSInterval));
  }

  //Set maximum size allowed for video events
  if (i64MaxVideoEvents<EVENT_MAXVIDEO_MIN)
    m_i64MaxVideoEvents=EVENT_MAXVIDEO_DEF;
  else
    m_i64MaxVideoEvents=i64MaxVideoEvents;
  if (m_bDebug) printMessage(tr("Debug"),tr("Maximum disk space for video events is set to [bytes] ")+QString::number(m_i64MaxVideoEvents));

  //Set timer - remove videos
  m_TimerCleanup.start(TIMEOUT_CLEANUP);

  //Initialise servo control
  if (bServo)
  {
    if (u16PigpioPort==0) u16PigpioPort=SERVO_PORT_DEF;
    m_iPigpioID=pigpio_start(NULL,QString::number(u16PigpioPort).data());
    if (m_iPigpioID>=0)
    {
      if (m_bDebug) printMessage(tr("Debug"),tr("Servo capability was successfully initialized at port")+" "+sPigpioPort);
    }
    else
    {
        printMessage(tr("Warning"),tr("Attempt to initialize servo capability was unsuccessful.");
    }
  }
  else
  {
    m_iPigpioID=-1;
    if (m_bDebug) printMessage(tr("Debug"),tr("Servo capability is disabled."));
  }

  //Set maximum pulse width (RIGHT)
  if ((iServoPulseRange<SERVO_PULSE_RANGE_MIN)||(iServoPulseRange>SERVO_PULSE_RANGE_MAX))
    m_iServoPulseRange=SERVO_PULSE_RANGE_DEF;
  else
    m_iServoPulseRange=iServoPulseRange;
  if ((m_bDebug) && (bServo)) printMessage(tr("Debug"),tr("Range of servo's pulse width is set to [microseconds] ")+QString::number(m_iServoPulseRange));

  //Set tilt and pan
  m_iTilt=0;
  m_iPan=0;

  //Connect signals to slots
  connect(&m_TcpServer,&CTcpServer::messageReceived,this,&CGlobalObject::message);
  connect(&m_Manager,&QNetworkAccessManager::finished,this,&CGlobalObject::replyFinished);
  connect(&m_TimerDDNS,&QTimer::timeout,this,&CGlobalObject::refreshDDNS);
  connect(&m_TimerCleanup,&QTimer::timeout,this,&CGlobalObject::cleanupEvents);

  //Set InitSuccessfull as true after initialization was successfull
  m_bInitSuccessful=true;
  if (m_bDebug) printMessage(tr("Debug"),tr("Initialization has been successful."));
}

CGlobalObject::~CGlobalObject()
{
  //End timers
  m_TimerDDNS.stop();
  m_TimerCleanup.stop();

  //End 'motion' if running
  if (m_ProcessMotion.state()==QProcess::Running) m_ProcessMotion.kill();

  //Stop servo control
  if (m_iPigpioID>=0) pigpio_stop(m_iPigpioID);

  //End TCP socket
  m_TcpServer.stopServer();

  //End database
  endDatabase();
}

void CGlobalObject::printMessage(const QString &sType, const QString &sMessage)
{
  QTextStream out(stdout);
  out<<sType<<":   "<<sMessage<<"\n";
  out.flush();
}

//*************************** DATABASE SCOPE *****************************************//

bool CGlobalObject::initDatabase()
{
  //Validate database driver
  if (!QSqlDatabase::drivers().contains("QSQLITE"))
  {
    printMessage(tr("Warning"),tr("Sqlite database driver wasn't found."));
    return false;
  }

  //Create database handler
  QString sDir;
  QString sFile=databaseLocation(&sDir);
  m_db.setDatabaseName(QDir::toNativeSeparators(sFile));

  //Create directory for the fisrt time
  if (!QFile::exists(sFile))
  {
    QDir().mkpath(sDir);
  }

  //Open database
  if (!m_db.open())
  {
    printMessage(tr("Warning"),tr("Error reported by the driver while opening the database."));
    return false;
  }

  //EXISTING DATABASE
  if (m_db.tables().count()>0)
  {
    if (!upgradeDatabase()) return false;
    return true;
  }

  //NEW DATABASE
  bool bError=false;

  //Create database tables
  QSqlQuery Query;
  Query.setForwardOnly(true);
  bError|=!Query.exec("CREATE TABLE history (idx INTEGER PRIMARY KEY, datetime NUMERIC, user TEXT, desc TEXT);");

  bError|=!Query.exec("CREATE TABLE settings (idx INTEGER PRIMARY KEY, version NUMERIC, commpass TEXT, ddns TEXT, smtp_host TEXT, smtp_user TEXT, smtp_pass TEXT, smtp_conntype NUMERIC, smtp_authmethod NUMERIC, smtp_port NUMERIC);");
  bError|=!Query.exec("INSERT INTO settings VALUES (0,"+QString::number(DB_VER)+",'"+TCP_COMMPASS_DEF+"',NULL,NULL,NULL,NULL,0,0,0);");

  if (bError)
  {
    QFile::remove(sFile);
    m_db.close();
    printMessage(tr("Warning"),tr("Error reported by the driver while creating the database."));
    return false;
  }

  return true;
}

bool CGlobalObject::upgradeDatabase()
{
  //Initiate query variable for all steps
  QSqlQuery Query;
  Query.setForwardOnly(true);

  //Validate database
  QStringList lsts=m_db.tables();
  if ((!lsts.contains("history"))||(!lsts.contains("settings")))
  {
    m_db.close();
    printMessage(tr("Warning"),tr("Your database seems to be corrupted."));
    return false;
  }

  //Retrieves the database and application versions
  Query.exec("SELECT version FROM settings WHERE idx=0;");
  if (!Query.next())
  {
    m_db.close();
    printMessage(tr("Warning"),tr("Your database seems to be corrupted."));
    return false;
  }

  int iDBVersion=Query.value(0).toInt();
  Query.finish();
  int iAppVersion=DB_VER;

  //Database is same version as application
  if (iDBVersion==iAppVersion) return true;

  //Database is newer version than application
  if (iDBVersion>iAppVersion)
  {
    m_db.close();
    printMessage(tr("Warning"),tr("The database's version isn't supported.\nYou will need to upgrade your application."));
    return false;
  }

  //BEGIN TRANSACTION
  Query.exec("BEGIN TRANSACTION;");
  bool bError=false;

  /***************************************************************************************************************************************/
  //AVAILABLE ACTIONS:
  //ALTER TABLE table-name RENAME TO new-table-name
  //ALTER TABLE table-name ADD COLUMN column-name TEXT/NUMERIC

  //Upgrade database: 100 to 101
  if (iDBVersion<101)
  {
//    bError|=!Query.exec("ALTER TABLE settings ADD COLUMN xx NUMERIC;");
//    bError|=!Query.exec("UPDATE settings SET xx=0 WHERE idx=0;");
  }

  /***************************************************************************************************************************************/

  //Rollback to the original state in case errors were detected
  if (bError)
  {
    Query.exec("ROLLBACK TRANSACTION;");
    m_db.close();
    printMessage(tr("Warning"),tr("There was a problem while upgrading your database.\nNo changes were committed."));
    return false;
  }

  //Update the database version
  Query.exec("UPDATE settings SET version="+QString::number(DB_VER)+" WHERE idx=0;");

  //COMMIT TRANSACTION: After all steps are succesfully completed
  Query.exec("COMMIT TRANSACTION;");

  return true;
}

void CGlobalObject::endDatabase()
{
  m_db.close();
}

QString CGlobalObject::databaseLocation(QString *psDir)
{
  QString sDir;

  sDir=QDir::cleanPath(QString("%1/%2").arg(QCoreApplication::applicationDirPath()).arg(DATABASE_DIR));

  if (psDir) *psDir=sDir;
  return QDir::cleanPath(QString("%1/%2").arg(sDir).arg(DATABASE_NAME));
}

qulonglong CGlobalObject::generateId(const QString &sTable)
{
  QSqlQuery Query;
  Query.setForwardOnly(true);
  Query.exec("SELECT MAX(idx) FROM " + sTable);
  qulonglong ulIdx=0;
  if (Query.next())
    ulIdx = (Query.value(0).toULongLong() + 1);

  return ulIdx;
}

void CGlobalObject::addHistory(const QString &sUser,const QString &sDesc)
{
  //Set model to apply changes
  QSqlTableModel Table;
  Table.setEditStrategy(QSqlTableModel::OnManualSubmit);
  Table.setTable("history");
  Table.select();

  //Write data to model
  quint16 u16Id=generateId("history");
  Table.insertRow(0);
  Table.setData(Table.index(0,0),u16Id);
  Table.setData(Table.index(0,1),QDateTime::currentMSecsSinceEpoch());
  Table.setData(Table.index(0,2),sUser);
  Table.setData(Table.index(0,3),sDesc);
  Table.submitAll();

  //Trim database in case is oversized
  int iSize=g_rowCount(Table);
  if (iSize>MAX_HISTORY)
  {
    Table.removeRows(0,iSize-MAX_HISTORY);
    Table.submitAll();
  }
}

QString CGlobalObject::commandServo(int iGpio, int iNewVal, int *piCurrentVal)
{
  if (m_iPigpioID>=0)
  {
    int iPulseWidth=qRound(double(m_iServoPulseRange)/180.0*double(iNewVal))+SERVO_PULSE_W_NEUTRAL;
    if (set_servo_pulsewidth(m_iPigpioID,iGpio,iPulseWidth)==0)
    {
      *piCurrentVal=iNewVal;
      return QString("OK");
    }
    else
    {
      return QString(tr("Command to servo was not permitted"));
    }
  }

  //Daemon is not active
  return tr("Servo capability is not enabled in the security camera.");
}

//*************************** MESSAGE SLOT *****************************************//

void CGlobalObject::message(QTcpSocket* pSocket,const QByteArray &BAData)
{
  //Define environment to read request
  QDataStream DataRequest(BAData);
  DataRequest.setVersion(QDataStream::Qt_5_0);

  //Define environment to write reply
  QByteArray BADataReply;
  QDataStream DataReply(&BADataReply, QIODevice::WriteOnly);
  DataReply.setVersion(QDataStream::Qt_5_0);
  QString sCommPass="";

  //Read Instruction
  QString sInstruction;
  DataRequest >> sInstruction;

  if (sInstruction==TCP_GETMOTION)
  {
    //Read motion's status
    QString sMotion="No";
    if (m_ProcessMotion.state()==QProcess::Running) sMotion="Yes";

    //Write data
    DataReply << QString("OK");
    DataReply << sMotion;
    DataReply << m_bActive;
    if (m_bDebug) printMessage(tr("Debug"),tr("Request received: GET CAMERA status %1 [%2] ").arg(sMotion=="Yes" ? tr("ON") : tr("OFF")).arg(m_bActive ? tr("Active") : tr("Pasive")));
  }

  if (sInstruction==TCP_SETMOTIONUP)
  {
    //Read data - User
    QString sUser;
    bool bActive;
    DataRequest >> sUser;
    DataRequest >> bActive;

    //Process already running
    if (m_ProcessMotion.state()==QProcess::Running)
    {
      //Write data
      DataReply << QString("ERR");
      DataReply << tr("The camera is already ON.");
    }
    //Process not running
    else
    {
      //Attempt to run motion
      QString sAppDirPath=QCoreApplication::applicationDirPath();
      QStringList lsArguments;
      lsArguments.append("-c");
      lsArguments.append(QDir::cleanPath(QString("%1/%2").arg(sAppDirPath).arg((bActive) ? CONF_FILE_MOTIONA : CONF_FILE_MOTIONP)));
      m_ProcessMotion.start(QDir::cleanPath(QString("%1/%2").arg(sAppDirPath).arg(COMMAND_MOTION)),lsArguments);
      if (m_ProcessMotion.waitForStarted())
      {
        //Set active parameter
        m_bActive=bActive;
        //Record history
        addHistory(sUser,tr("Camera ON [%1]").arg(bActive ? tr("active") : tr("passive")));
        //Write data
        DataReply << QString("OK");
      }
      else
      {
        //Write data
        DataReply << QString("ERR");
        DataReply << tr("The camera could not start.\n").arg(m_ProcessMotion.errorString());
      }
    }
    if (m_bDebug) printMessage(tr("Debug"),tr("Request received: SET CAMERA ON [%1]").arg(bActive ? tr("Active") : tr("Pasive")));
  }

  if (sInstruction==TCP_SETMOTIONDOWN)
  {
    //Read data - User
    QString sUser;
    DataRequest >> sUser;

    //Process running
    if (m_ProcessMotion.state()==QProcess::Running)
    {
      //Terminate motion
      m_ProcessMotion.kill();
      //Record history
      addHistory(sUser,tr("Camera OFF"));
      //Write data
      DataReply << QString("OK");
    }
    //Process already not running
    else
    {
      //Write data
      DataReply << QString("ERR");
      DataReply << tr("The camera is already OFF.");
    }
    if (m_bDebug) printMessage(tr("Debug"),tr("Request received: SET CAMERA OFF"));
  }

  if (sInstruction==TCP_GETTILT)
  {
    //Write data
    DataReply << QString("OK");
    DataReply << qint16(m_iTilt);
    if (m_bDebug) printMessage(tr("Debug"),tr("Request received: GET TILT ") + QString::number(m_iTilt));
  }

  if (sInstruction==TCP_SETTILT)
  {
    //Read data - User
    QString sUser;
    DataRequest >> sUser;

    //Read data
    qint16 i16Tilt;
    DataRequest >> i16Tilt;

    //Process command
    QString sResult=commandServo(SERVO_GPIO_TILT,i16Tilt,&m_iTilt);

    if (sResult=="OK")
    {
      //Record history
      QString sHistory;
      QTextStream Stream(&sHistory);
      Stream<<tr("Change Tilt ")<<(m_iTilt>0 ? "+" : (m_iTilt<0 ? "-" : ""))<<qSetPadChar('0')<<qSetFieldWidth(2)<<qAbs(m_iTilt);
      addHistory(sUser,sHistory);
      //Write data
      DataReply << QString("OK");
    }
    else
    {
      //Write data
      DataReply << QString("ERR");
      DataReply << sResult;
    }
    if (m_bDebug) printMessage(tr("Debug"),tr("Request received: SET TILT ") + QString::number(m_iTilt));
  }

  if (sInstruction==TCP_GETPAN)
  {
    //Write data
    DataReply << QString("OK");
    DataReply << qint16(m_iPan);
    if (m_bDebug) printMessage(tr("Debug"),tr("Request received: GET PAN ") + QString::number(m_iPan));
  }

  if (sInstruction==TCP_SETPAN)
  {
    //Read data - User
    QString sUser;
    DataRequest >> sUser;

    //Read data
    qint16 i16Pan;
    DataRequest >> i16Pan;

    //Process command
    QString sResult=commandServo(SERVO_GPIO_PAN,i16Pan,&m_iPan);

    if (sResult=="OK")
    {
      //Record history
      QString sHistory;
      QTextStream Stream(&sHistory);
      Stream<<tr("Change Pan ")<<(m_iPan>0 ? "+" : (m_iPan<0 ? "-" : ""))<<qSetPadChar('0')<<qSetFieldWidth(2)<<qAbs(m_iPan);
      addHistory(sUser,sHistory);
      //Write data
      DataReply << QString("OK");
    }
    else
    {
      //Write data
      DataReply << QString("ERR");
      DataReply << sResult;
    }
    if (m_bDebug) printMessage(tr("Debug"),tr("Request received: SET PAN ") + QString::number(m_iPan));
  }

  if (sInstruction==TCP_GETSETUP)
  {
    //Set model to read settings
    QSqlTableModel Table;
    Table.setTable("settings");
    Table.select();
    QSqlRecord Record=Table.record(0);

    //Write data
    DataReply << QString("OK");
    DataReply << Record.value(3).toString(); //UrlDDNS
    DataReply << Record.value(4).toString(); //SmtpHost
    DataReply << Record.value(5).toString(); //SmtpUser
    DataReply << Record.value(6).toString(); //SmtpPass
    DataReply << quint16(Record.value(7).toInt()); //ConnType
    DataReply << quint16(Record.value(8).toInt()); //AuthMethod
    DataReply << quint16(Record.value(9).toInt()); //SmtpPort
    if (m_bDebug) printMessage(tr("Debug"),tr("Request received: GET SETUP"));
  }

  if (sInstruction==TCP_SETSETUP)
  {
    //Read data - User
    QString sUser;
    DataRequest >> sUser;

    //Set model to apply changes
    QSqlTableModel Table;
    Table.setEditStrategy(QSqlTableModel::OnManualSubmit);
    Table.setTable("settings");
    Table.select();
    QSqlRecord Record=Table.record(0);

    //Read remaining data
    QString sRead;
    quint16 u16Read;
    DataRequest >> sRead;
    Record.setValue(3,sRead); //UrlDDNS
    DataRequest >> sRead;
    Record.setValue(4,sRead); //SmtpHost
    DataRequest >> sRead;
    Record.setValue(5,sRead); //SmtpUser
    DataRequest >> sRead;
    Record.setValue(6,sRead); //SmtpPass
    DataRequest >> u16Read;
    Record.setValue(7,u16Read); //ConnType
    DataRequest >> u16Read;
    Record.setValue(8,u16Read); //AuthMethod
    DataRequest >> u16Read;
    Record.setValue(9,u16Read); //SmtpPort
    DataRequest >> sRead;
    Record.setValue(2,sRead); //CommPass

    //Attempt to update database
    Table.setRecord(0,Record);
    if (Table.submitAll())
    {
      //Keep the new commpass
      sCommPass=sRead;
      //Record history
      addHistory(sUser,tr("Setup update"));
      //Write data
      DataReply << QString("OK");
    }
    else
    {
      //Write data
      DataReply << QString("ERR");
      DataReply << tr("The settings could not be updated.\n%1").arg(Table.lastError().text());
    }
    if (m_bDebug) printMessage(tr("Debug"),tr("Request received: UPDATE SETUP"));
  }

  if (sInstruction==TCP_GETHISTORY)
  {
    //Set model to read history
    QSqlTableModel Table;
    Table.setTable("history");
    Table.setSort(1,Qt::DescendingOrder);
    Table.select();
    qint64 i64Size=g_rowCount(Table);

    //Write data
    DataReply << QString("OK");
    DataReply << i64Size;

    for (qint64 i=0;i<i64Size;i++)
    {
      QSqlRecord Record=Table.record(i);
      DataReply << Record.value(1).toLongLong();
      DataReply << Record.value(2).toString();
      DataReply << Record.value(3).toString();
    }
    if (m_bDebug) printMessage(tr("Debug"),tr("Request received: GET HISTORY"));
  }

  //Reply result
  m_TcpServer.sendMessage(pSocket,BADataReply);
  if (!sCommPass.isEmpty()) m_TcpServer.setPassword(sCommPass);
}

//*************************** PROCESS SLOTS *****************************************//

void CGlobalObject::refreshDDNS()
{
  //Set model to get data
  QSqlTableModel Table;
  Table.setTable("settings");
  Table.select();
  QSqlRecord Record=Table.record(0);
  QString sUrlDDNS=Record.value(3).toString();

  if (!sUrlDDNS.isEmpty())
  {
    //Setup and send request
    QNetworkRequest Request;
    Request.setUrl(QUrl(Record.value(3).toString()));
    m_Manager.get(Request);
  }
}

void CGlobalObject::cleanupEvents()
{
  //Setup and configure directory
  QString sAppDirPath=QCoreApplication::applicationDirPath();
  QDir Dir(QDir::cleanPath(QString("%1/%2").arg(sAppDirPath).arg(EVENT_VIDEO_DIR)));
  Dir.setNameFilters(QStringList()<<"*.*");
  Dir.setFilter(QDir::Files);
  Dir.setSorting(QDir::Time);

  //Get list of video files
  QFileInfoList lstFileInfo=Dir.entryInfoList();
  qint64 i64Size;

  //Remove all files over the quota
  do
  {
    //Evaluate total size in use
    i64Size=0;
    foreach (QFileInfo FileInfo, lstFileInfo)
    {
      i64Size+=FileInfo.size();
    }

    //Remove oldest file
    if (i64Size>m_i64MaxVideoEvents)
    {
      QString sFile=lstFileInfo.takeLast().absoluteFilePath();
      Dir.remove(sFile);
      if (m_bDebug) printMessage(tr("Debug"),tr("Cleanup: Remove video over the quota"));
    }
  } while (i64Size>m_i64MaxVideoEvents);
}

void CGlobalObject::replyFinished(QNetworkReply *pReply)
{
  if (m_bDebug) printMessage(tr("Debug"),tr("DDNS - reply"));
  if (m_bDebug) printMessage(tr("Debug"),pReply->readAll());
}
