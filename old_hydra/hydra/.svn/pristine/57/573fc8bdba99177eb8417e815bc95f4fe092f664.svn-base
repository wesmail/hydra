//*-- AUTHOR : Ilse Koenig
//*-- Modified : 25/08/99

//////////////////////////////////////////////////////////////////////////////
// HOraUtil
//
// Utility class for I/O from database Oracle
//
//////////////////////////////////////////////////////////////////////////////

#include "horautil.h"
#include "horaconn.h"
#include <iostream.h>

#include "iodbc.h"
#include "udbcext.h"
#include "isql.h"
#include "isqlext.h"

ClassImp(HOraUtil)
 
HOraUtil::HOraUtil(HOraConn* p) {
  // constructor
  // gets the pointer to the connection class
  pConn=p;
}

void HOraUtil::printOdbcDriverInfo() {
  // prints information about the ODBC driver
  void* hdbc=pConn->getHdbc();
  if (!hdbc) {
    cerr<<"no connection to database";
    return;
  }
  char buffer[512];
  SWORD bufferlength;
  RETCODE rc=
    SQLGetInfo(hdbc,SQL_DRIVER_VER,buffer,sizeof(buffer),&bufferlength);
  if (rc!=SQL_SUCCESS && rc!=SQL_SUCCESS_WITH_INFO) {
    pConn->showErrorMessage("HOraUtil::printOdbcDriverInfo()");
    return;
  }
  cout<<"SQL_DRIVER_VER "<<buffer<<'\n';
  rc=SQLGetInfo(hdbc,SQL_DRIVER_ODBC_VER,buffer,sizeof(buffer),&bufferlength);
  if (rc!=SQL_SUCCESS && rc!=SQL_SUCCESS_WITH_INFO) {
    pConn->showErrorMessage("HOraUtil::printOdbcDriverInfo()");
    return;
  }
  cout<<"SQL_DRIVER_ODBC_VER "<<buffer<<'\n';
}


void HOraUtil::printOdbcFunctionInfo() {
  // lists all functions supported by the actual ODBC driver
  void* hdbc=pConn->getHdbc();
  if (!hdbc) {
    cerr<<"no connection to database";
    return;
  }
  // test for not supported functions
  Int_t i1[]={1,40};
  Int_t i2[]={23,72};
  UWORD fExists;
  RETCODE rc;
  cout<<"Functions supported by the ODBC driver"<<'\n';
  for(Int_t j=0;j<2;j++) {
    for(Int_t i=i1[j];i<=i2[j];i++) {
      fExists=0;
      rc=SQLGetFunctions(hdbc,(UWORD)i,&fExists);
      if (rc!=SQL_SUCCESS && rc!=SQL_SUCCESS_WITH_INFO) {
        pConn->showErrorMessage("HOraUtil::printOdbcFunctionInfo()");
        return;
      }
      if (fExists!=0) {
        switch(i) {
	  case 1: { cout<<i<<"  "<<"SQLAllocConnect"<<endl; break; }
	  case 2: { cout<<i<<"  "<<"SQLAllocEnv"<<endl; break; }
	  case 3: { cout<<i<<"  "<<"SQLAllocStmt"<<endl; break; }
	  case 4: { cout<<i<<"  "<<"SQLBindCol"<<endl; break; }
	  case 5: { cout<<i<<"  "<<"SQLCancel"<<endl; break; }
	  case 6: { cout<<i<<"  "<<"SQLColAttributes"<<endl; break; }
	  case 7: { cout<<i<<"  "<<"SQLConnect"<<endl; break; }
	  case 8: { cout<<i<<"  "<<"SQLDescribeCol"<<endl; break; }
	  case 9: { cout<<i<<"  "<<"SQLDisconnect"<<endl; break; }
	  case 10: { cout<<i<<"  "<<"SQLError"<<endl; break; }
	  case 11: { cout<<i<<"  "<<"SQLExecDirect"<<endl; break; }
	  case 12: { cout<<i<<"  "<<"SQLExecute"<<endl; break; }
	  case 13: { cout<<i<<"  "<<"SQLFetch"<<endl; break; }
	  case 14: { cout<<i<<"  "<<"SQLFreeConnect"<<endl; break; }
	  case 15: { cout<<i<<"  "<<"SQLFreeEnv"<<endl; break; }
	  case 16: { cout<<i<<"  "<<"SQLFreeStmt"<<endl; break; }
	  case 17: { cout<<i<<"  "<<"SQLGetCursorName"<<endl; break; }
	  case 18: { cout<<i<<"  "<<"SQLNumResultCols"<<endl; break; }
	  case 19: { cout<<i<<"  "<<"SQLPrepare"<<endl; break; }
	  case 20: { cout<<i<<"  "<<"SQLRowCount"<<endl; break; }
	  case 21: { cout<<i<<"  "<<"SQLSetCursorName"<<endl; break; }
	  case 22: { cout<<i<<"  "<<"SQLSetParam"<<endl; break; }
	  case 23: { cout<<i<<"  "<<"SQLTransact"<<endl; break; }
	  case 40: { cout<<i<<"  "<<"SQLColumns"<<endl; break; }
	  case 41: { cout<<i<<"  "<<"SQLDriverConnect"<<endl; break; }
	  case 42: { cout<<i<<"  "<<"SQLGetConnectOption"<<endl; break; }
	  case 43: { cout<<i<<"  "<<"SQLGetData"<<endl; break; }
	  case 44: { cout<<i<<"  "<<"SQLGetFunctions"<<endl; break; }
	  case 45: { cout<<i<<"  "<<"SQLGetInfo"<<endl; break; }
	  case 46: { cout<<i<<"  "<<"SQLGetStmtOption"<<endl; break; }
	  case 47: { cout<<i<<"  "<<"SQLGetTypeInfo"<<endl; break; }
	  case 48: { cout<<i<<"  "<<"SQLParamData"<<endl; break; }
	  case 49: { cout<<i<<"  "<<"SQLPutData"<<endl; break; }
	  case 50: { cout<<i<<"  "<<"SQLSetConnectOption"<<endl; break; }
	  case 51: { cout<<i<<"  "<<"SQLSetStmtOption"<<endl; break; }
	  case 52: { cout<<i<<"  "<<"SQLSpecialColumns"<<endl; break; }
	  case 53: { cout<<i<<"  "<<"SQLStatistics"<<endl; break; }
	  case 54: { cout<<i<<"  "<<"SQLTables"<<endl; break; }
	  case 55: { cout<<i<<"  "<<"SQLBrowseConnect"<<endl; break; }
	  case 56: { cout<<i<<"  "<<"SQLColumnPrivileges"<<endl; break; }
	  case 57: { cout<<i<<"  "<<"SQLDataSources"<<endl; break; }
	  case 58: { cout<<i<<"  "<<"SQLDescribeParam"<<endl; break; }
	  case 59: { cout<<i<<"  "<<"SQLExtendedFetch"<<endl; break; }
	  case 60: { cout<<i<<"  "<<"SQLForeignKeys"<<endl; break; }
	  case 61: { cout<<i<<"  "<<"SQLMoreResults"<<endl; break; }
	  case 62: { cout<<i<<"  "<<"SQLNativeSql"<<endl; break; }
	  case 63: { cout<<i<<"  "<<"SQLNumParams"<<endl; break; }
	  case 64: { cout<<i<<"  "<<"SQLParamOptions"<<endl; break; }
	  case 65: { cout<<i<<"  "<<"SQLPrimaryKeys"<<endl; break; }
	  case 66: { cout<<i<<"  "<<"SQLProcedureColumns"<<endl; break; }
	  case 67: { cout<<i<<"  "<<"SQLProcedures"<<endl; break; }
	  case 68: { cout<<i<<"  "<<"SQLSetPos"<<endl; break; }
	  case 69: { cout<<i<<"  "<<"SQLSetScrollOptions"<<endl; break; }
	  case 70: { cout<<i<<"  "<<"SQLTablePrivileges"<<endl; break; }
	  case 71: { cout<<i<<"  "<<"SQLDrivers"<<endl; break; }
	  case 72: { cout<<i<<"  "<<"SQLBindParameter"<<endl; break; }
	  default: { cout<<i<<"  "<<i<<endl;}
        }
      }
    }
  }
  cout<<endl;
}  

///////////////////////////////////////////////////////////////////////////////
// the following function tests the ODBC funtion SQLExtendedFetch(...)
// which allows to writter into an array instead of using a cursor
/*
Bool_t HOraUtil::testExtendedFetch() {
  void* hstmt=pConn->getHstmt();
  if (!hstmt) {
    cerr<<"no connection to database";
    return kFALSE;
  }
  char statement[]=
    "select p3d_corner, P3D_X, P3D_Y, P3D_Z from hades_geom.geom_3dpoints "
      "where OBJ_INDEX=8 and vers_index=2 and p3d_corner>0 "
      "order by p3d_corner";

#define POINTS_AS 12

  long np[POINTS_AS];
  double x[POINTS_AS], y[POINTS_AS], z[POINTS_AS];
  SDWORD np_Ind[POINTS_AS], x_Ind[POINTS_AS], y_Ind[POINTS_AS], z_Ind[POINTS_AS];
  UDWORD numRows;
  UWORD rowStatus[POINTS_AS];
  RETCODE rc;

  for(int i=0;i<POINTS_AS;i++) {
    np[i]=0;
    x[i]=y[i]=z[i]=0;
  }

  SQLSetStmtOption(hstmt,SQL_BIND_TYPE,SQL_BIND_BY_COLUMN);
  SQLSetStmtOption(hstmt,SQL_ROWSET_SIZE,POINTS_AS);

  rc=SQLBindCol(hstmt,1,SQL_C_LONG,np,sizeof(np[0]),np_Ind);
  rc=SQLBindCol(hstmt,2,SQL_C_DOUBLE,x,sizeof(x[0]),x_Ind);
  rc=SQLBindCol(hstmt,3,SQL_C_DOUBLE,y,sizeof(y[0]),y_Ind);
  rc=SQLBindCol(hstmt,4,SQL_C_DOUBLE,z,sizeof(z[0]),z_Ind);

  rc=SQLExecDirect(hstmt,(UCHAR *)statement,SQL_NTS);
  if (rc!=SQL_SUCCESS && rc!=SQL_SUCCESS_WITH_INFO) goto rwe;

  rc=SQLExtendedFetch(hstmt,SQL_FETCH_NEXT,0,&numRows,rowStatus);
  if (rc!=SQL_SUCCESS && rc!=SQL_SUCCESS_WITH_INFO) goto rwe;
  else {
cout<<"rc: "<<rc<<endl;
cout<<"numRows: "<<numRows<<endl;
    for(int i=0;i<((int)numRows);i++) {
      //cout<<i<<" status: "<<rowStatus[i]<<endl;
      if (rowStatus[i]==SQL_ROW_SUCCESS)
        cout<<np_Ind[i]<<"  "<<x_Ind[i]<<"  "<<y_Ind[i]<<"  "<<z_Ind[i]<<"    "
            <<np[i]<<"  "<<x[i]<<"  "<<y[i]<<"  "<<z[i]<<'\n';
    }
    cout<<"---------------------------------------------------------"<<'\n';
    SQLFreeStmt(hstmt,SQL_CLOSE);
    return kTRUE;
  }

rwe:
  pConn->showErrorMessage("HOraTest::testExtendedFetch");
  cout<<rc<<endl;
  SQLFreeStmt(hstmt,SQL_CLOSE);
  return kFALSE;
}
*/



