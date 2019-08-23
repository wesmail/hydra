//*-- AUTHOR : Ilse Koenig
//*-- Modified : 17/08/99

/////////////////////////////////////////////////////////////
// HOraConn
//
// Connection class to database Oracle (uses ODBC)
//
/////////////////////////////////////////////////////////////

#include "horaconn.h"
#include "hades.h"
#include "hruntimedb.h"
#include "heventfile.h"
#include <stdio.h>
#include <iostream.h>

// ODBC include directories
#include "iodbc.h"
#include "udbcext.h"
#include "isql.h"
#include "isqlext.h"

ClassImp(HOraConn)

HOraConn::HOraConn() {
  // default constructor
  // defines default values for user ("hades") and the database ("hades-test")
  // "hades-test" in the Oracle 8 version on Linux
  // User "hades" has Readonly access to the Oracle tables.
  // The connection to Oracle is not opened!
  strcpy(dbName,"hades-test");
  strcpy(userName,"hades");

  isConnected=kFALSE;
  henv=0;
  hdbc=0;
  hstmt=0;
}


HOraConn::~HOraConn() {
  // default destructor (closes connection)
  close();
}


Bool_t HOraConn::open() {
  // opens default connection with readonly access
  char* password = new char[9];
  strcpy(password,"hades");
  Bool_t rc=openConnection(password);
  if (!rc) close();
  return rc;
}

Bool_t HOraConn::open(char *uName) {
  // opens connection to database Hades for user given by name
  // asks for password
  strncpy(userName,uName,20);
  char* password=getPassword();
  Bool_t rc=openConnection(password);
  if (!rc) close();
  return rc;
}


Bool_t HOraConn::open(char *dbN, char *uN) {
  // opens connection to database with name dbName for user given by name
  // asks for password
  strncpy(dbName,dbN,20);
  strncpy(userName,uN,20);
  char* password=getPassword();
  Bool_t rc=openConnection(password);
  if (!rc) close();
  return rc;
}


void HOraConn::close() {
  // closes an open connection
  // A transaction will be automatically rolled back,
  // that means changes in the database are not stored
  // without an explicit COMMIT 
  if (hstmt) SQLFreeStmt (hstmt, SQL_DROP);
  hstmt=0;
  if (isConnected) {
    SQLDisconnect (hdbc);
    SQLTransact(henv, hdbc, SQL_ROLLBACK);
  }
  if (hdbc) SQLFreeConnect (hdbc);
  hdbc=0;
  if (henv) SQLFreeEnv (henv);
  henv=0;
  isConnected=kFALSE;
}


void HOraConn::print() {
  // prints information about the database connection
  if (isConnected)
    cout<<"Oracle-Database: "<<dbName<<"    Username: "<<userName<<'\n';
  else cout<<"*** no connection to Oracle established  ***"<<'\n';
}


void HOraConn::showErrorMessage(char* errorloc) {
  // shows SQL error messages 
  unsigned char buf[257];
  unsigned char sqlstate[15];

  cerr <<" error in " << errorloc << endl;
  // Get statement errors
  if (SQLError (henv, hdbc, hstmt, sqlstate, NULL,
      buf, sizeof(buf), NULL) == SQL_SUCCESS)
    cerr << buf << " SQLSTATE=" << sqlstate << endl;
  // Get connection errors
  if (SQLError (henv, hdbc, SQL_NULL_HSTMT, sqlstate, NULL,
      buf, sizeof(buf), NULL) == SQL_SUCCESS)
    cerr << buf << " SQLSTATE=" << sqlstate << endl;
  // Get environmental errors
  if (SQLError (henv, SQL_NULL_HDBC, SQL_NULL_HSTMT, sqlstate, NULL,
      buf, sizeof(buf), NULL) == SQL_SUCCESS)
    cerr << buf << " SQLSTATE=" << sqlstate << endl;
}


Long_t HOraConn::getRunId() {
  // gets the run id from the current event file
  // if this id is -1 it reads it from Oracle  and store it in the setup
  // container
  HEventFile* set=(HEventFile*)gHades->getRuntimeDb()->getCurrentEventFile();
  if (!set) {
    cerr<<"current event file not found in runtime database"<<endl;
    return -1L;
  }
  Long_t runId=set->getFileId();
//  if (runId>0) return runId;         later!!!
  if (runId!=-1L) return runId;

  if (!hstmt) {
    cerr<<"no connection to database";
    return -1L;
  }

  char statement[]=
    "select run_id from mdc_ana.run where run_file_name=?";
  char filename[250];
  SDWORD runId_Ind;
  SDWORD filename_Ind=SQL_NTS;
  RETCODE rc;
  strcpy(filename,set->GetName());
  SQLBindCol(hstmt,1,SQL_C_LONG,&runId,0,&runId_Ind);
  SQLBindParameter(hstmt,1,SQL_PARAM_INPUT,SQL_C_CHAR,SQL_VARCHAR,
         sizeof(filename),0,(UCHAR *)filename,0,&filename_Ind);
  rc=SQLExecDirect(hstmt,(UCHAR *)statement,SQL_NTS);
  if (rc!=SQL_SUCCESS && rc!=SQL_SUCCESS_WITH_INFO) {
    showErrorMessage("HOraConn::getRunId(): statement error");
    SQLFreeStmt(hstmt,SQL_CLOSE);
    return -1L;
  }
  while ((rc=SQLFetch(hstmt))!=SQL_NO_DATA_FOUND) {
    set->setFileId(runId);
  }
  if (runId==-1L) showErrorMessage("HOraConn::getRunId:run_file_name not found");
  SQLFreeStmt(hstmt,SQL_CLOSE);
  return runId;
}


char* HOraConn::getPassword() {
  // asks for the password
  char* password = new char[9];
  cout<<"enter password for user "<<userName<<" > ";
  scanf("%[^\n]%*c",password);
  return password;
}


Bool_t HOraConn::openConnection(char* password) {
  // opens connection (contains the SQL-statements)
  if (isConnected) close();

  char connStr[60];
// e.g."READONLY=N;DSN=Hades;UID=HADES;PWD=HADES"
  strcpy(connStr,"READONLY=N;DSN=");
//  strcpy(connStr,"DSN=");
  strcat(connStr,dbName);
  strcat(connStr,";UID=");
  strcat(connStr,userName);
  strcat(connStr,";PWD=");
  strcat(connStr,password);

  // environment and connection handle
  if ((SQLAllocEnv(&henv)!=SQL_SUCCESS) ||
          (SQLAllocConnect(henv,&hdbc)!=SQL_SUCCESS)) {
    cout<<"***  Failed to allocate database environment  ***"<<endl;
    delete password;
    close();
    return kFALSE;
  }

  // driver connection
  short buflen;
  char buf[257];
  Int_t status=SQLDriverConnect(hdbc,0,(UCHAR *)connStr,SQL_NTS,(UCHAR *)buf,
                              sizeof(buf),&buflen,SQL_DRIVER_COMPLETE);
  delete password;
  if (status!=SQL_SUCCESS && status!=SQL_SUCCESS_WITH_INFO) {
    cerr<<"***  Failed to allocate driver connection  ***"<<endl;
    close();
    return kFALSE;
  }

  // reset of default commit mode AUTOCOMMIT
  SQLSetConnectOption(hdbc, SQL_AUTOCOMMIT, SQL_AUTOCOMMIT_OFF);

  // statement handle
  if (SQLAllocStmt(hdbc,&hstmt)!=SQL_SUCCESS) {
    cout<<"***  Failed to allocate statement handle  ***"<<endl;
    cout<<"***  Database disconnected  ***"<<endl;
    close();
    return kFALSE;
  }

  isConnected=kTRUE;
  return isConnected;
}
