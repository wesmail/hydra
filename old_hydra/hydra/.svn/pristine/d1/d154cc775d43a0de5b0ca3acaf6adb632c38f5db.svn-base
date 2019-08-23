//*-- AUTHOR : Ilse Koenig
//*-- Modified : 02/11/98

/////////////////////////////////////////////////////////////
//HOraMdc
//
//  Interface class to database Oracle for input/output of
//  parameters needed by the Mdc
// 
/////////////////////////////////////////////////////////////

#include "horamdc.h"
#include "horaconn.h"
#include "hades.h"
#include "hruntimedb.h"
#include "heventfile.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hmdcdetector.h"
#include "hmdcrawstruct.h"
#include "hmdcgeomstruct.h"
#include "hmdccalpar.h"
#include "hmdclookupgeom.h"
#include <iostream.h>

#include "iodbc.h"
#include "udbcext.h"
#include "isql.h"
#include "isqlext.h"

ClassImp(HOraMdc)

HOraMdc::HOraMdc(HOraConn* p=0) : HDetParIo() {
  // constructor
  // sets the name of the I/O class "HMdcParIo"
  // gets the pointer to the connection class
  fName="HMdcParIo";
  pConn=p;
  isActiv=kFALSE;
  pVersId=-1;
  initModules=new TArrayI(24);
}

HOraMdc::~HOraMdc() {
  delete initModules;
}

Bool_t HOraMdc::init(HParSet* pPar,Int_t* set) {
  // calls special read-function for each parameter container 
  if (!isActiv) {
    if (readIds()==-1) return kFALSE;
  }
  if (pVersId==-1) return kFALSE;  // no modules found
  Text_t* name=pPar->GetName();
  if (strcmp(name,"MdcRawStruct")==0) return read(((HMdcRawStruct*)pPar),set);
  if (strcmp(name,"MdcGeomStruct")==0) return read(((HMdcGeomStruct*)pPar),set);
  if (strcmp(name,"MdcCalPar")==0) return read(((HMdcCalPar*)pPar),set);
  if (strcmp(name,"MdcLookupGeom")==0) return read(((HMdcLookupGeom*)pPar),set);
  cout<<"initialization of "<<name<<" not possible from Oracle!"<<endl;
  return kFALSE;
}


Bool_t HOraMdc::read(HMdcRawStruct* pPar,Int_t* set) {
  // reads the names (characterizes the position) of the motherboards
  // and the maximum number of TDCs on each of it and fills the 
  // MdcRawStruct container
  void* hstmt=pConn->getHstmt();
  if (!hstmt) {
    cerr<<"no connection to database";
    return kFALSE;
  }

  char statement[]=
  "select mbo_pos_id_js,n_channel_max*n_pos_max*n_dbo_pos_max,"
          "to_number(mbo_p_vers_id) "
    "from mdc_ana.mbo_placement,mdc_ana.mbo_pos,mdc_ana.mbo_obj,mdc_ana.mbo_type "
    "where mdc_obj_id=? and mbo_p_vers_id="
             "(select version_set.mbo_p_vers_id "
                 "from mdc_ana.version_set,mdc_ana.run "
                 "where run_id=? "
                    "and version_set.version_set_id=run.version_set_id) "
        "and mbo_placement.mbo_pos_id=mbo_pos.mbo_pos_id "
        "and mbo_placement.mbo_obj_id=mbo_obj.mbo_obj_id "
        "and mbo_obj.mbo_type_id=mbo_type.mbo_type_id " 
    "order by mbo_pos.mbo_pos_id";


  Long_t runId=pConn->getRunId();
  if (runId==-1L) return kFALSE;
  Long_t objId, num, vers;
  Int_t pos, n, version=-1;
  char mboName[11];
  SDWORD objId_Ind=0, run_Ind=0, num_Ind, vers_Ind, mboName_Ind;
  RETCODE rc;
  Bool_t allFound=kTRUE;

  SQLBindCol(hstmt,1,SQL_C_CHAR,mboName,sizeof(mboName),&mboName_Ind);
  SQLBindCol(hstmt,2,SQL_C_LONG,&num,0,&num_Ind);
  SQLBindCol(hstmt,3,SQL_C_LONG,&vers,0,&vers_Ind);
  SQLBindParameter(hstmt,1,SQL_PARAM_INPUT,SQL_C_LONG,SQL_INTEGER,0,0,
                   &objId,0,&objId_Ind);
  SQLBindParameter(hstmt,2,SQL_PARAM_INPUT,SQL_C_LONG,SQL_INTEGER,0,0,
                   &runId,0,&run_Ind);

  SQLPrepare(hstmt,(UCHAR *)statement,SQL_NTS);

  initModules->Reset();
  HMdcRawStruct& mdc=*pPar;
  for(Int_t i=0;i<6;i++) {
    HMdcRawSecStru& sec=mdc[i];
    for(Int_t j=0;j<4;j++) {
      HMdcRawModStru& mod=sec[j];
      objId=(Long_t)(id[i][j]);
      pos=i*4+j;
      if (set[pos]) {
        if (objId>0) {
          rc=SQLExecute(hstmt);
          if (rc!=SQL_SUCCESS && rc!=SQL_SUCCESS_WITH_INFO) {
            pConn->showErrorMessage(
                  "HOraMdc::read(HMdcRawStruct*,Int_t*): statement error");
            SQLFreeStmt(hstmt,SQL_CLOSE);
            return kFALSE;
          }
          n=0;
          while ((rc=SQLFetch(hstmt))!=SQL_NO_DATA_FOUND) {
            mod[n].SetName(mboName);
            mod[n].setNTdcs(((Int_t)num));
            version=(Int_t)vers;
            if (n==0) {
              set[pos]=0;
              initModules->AddAt(pos+1,pos);
            }
            n++;
          }
        }
        else allFound=kFALSE;
      }
    }
  }
  SQLFreeStmt(hstmt,SQL_CLOSE);
  pPar->setInputVersion(version,inputNumber);
  if (version==-1) return kFALSE;
  pPar->setChanged();
  printInfo("MdcRawStruct: module(s) initialized from Oracle: ");
  return allFound;
}


Bool_t HOraMdc::read(HMdcGeomStruct* pPar,Int_t* set) {
  // reads the number of cells in each layer and fills the 
  // MdcGeomStruct container
  void* hstmt=pConn->getHstmt();
  if (!hstmt) {
    cerr<<"no connection to database";
    return kFALSE;
  }

  char statement[]=
    "select layer_nr, n_wires "
    "from mdc_ana.mdc_obj, mdc_ana.mdc_type, mdc_ana.layer_type "
    "where mdc_obj.mdc_obj_id=? "
    "and ((mdc_type.mdc_type_id=mdc_obj.mdc_type_id) "
    "and (layer_type.mdc_type_id=mdc_type.mdc_type_id))";

  Long_t objId, layer, ncell;
  Int_t pos, count;
  SDWORD objId_Ind=0, layer_Ind, ncell_Ind;
  RETCODE rc;
  Bool_t allFound=kTRUE;

  SQLBindCol(hstmt,1,SQL_C_LONG,&layer,0,&layer_Ind);
  SQLBindCol(hstmt,2,SQL_C_LONG,&ncell,0,&ncell_Ind);
  SQLBindParameter(hstmt,1,SQL_PARAM_INPUT,SQL_C_LONG,SQL_INTEGER,0,0,
                   &objId,0,&objId_Ind);

  SQLPrepare(hstmt,(UCHAR *)statement,SQL_NTS);

  initModules->Reset();
  HMdcGeomStruct& mdc=*pPar;
  for(Int_t i=0;i<6;i++) {
    HMdcGeomSecStru& sec=mdc[i];
    for(Int_t j=0;j<4;j++) {
      HMdcGeomModStru& mod=sec[j];
      objId=(Long_t)(id[i][j]);
      pos=i*4+j;
      if (set[pos]) {
        if (objId>0) {
          rc=SQLExecute(hstmt);
          if (rc!=SQL_SUCCESS && rc!=SQL_SUCCESS_WITH_INFO) {
            pConn->showErrorMessage("HOraMdc::readNCells: statement error");
            SQLFreeStmt(hstmt,SQL_CLOSE);
            return kFALSE;
          }
          count=0;
          while ((rc=SQLFetch(hstmt))!=SQL_NO_DATA_FOUND) {
            mod.fill(((Int_t)(layer-1)),((Int_t)ncell));
            count++;
          }
          if (count==6) {
            set[pos]=0;
            initModules->AddAt(pos+1,pos);
          }
          else allFound=kFALSE;
        }
        else allFound=kFALSE;
      }
    }
  }
  SQLFreeStmt(hstmt,SQL_CLOSE);
  pPar->setInputVersion(pVersId,inputNumber);
  pPar->setChanged();
  printInfo("MdcGeomStruct: module(s) initialized from Oracle: ");
  return allFound;
}


Bool_t HOraMdc::read(HMdcCalPar* pCalPar, Int_t* set) {
  // reads calibration parameters and fill MdcCalPar container
  void* hstmt=pConn->getHstmt();
  if (!hstmt) {
    cerr<<"no connection to database";
    return kFALSE;
  }

  Long_t runId=pConn->getRunId();
  if (runId==-1L) return kFALSE;

  Int_t version, contVers;
  Text_t* name=pCalPar->GetName();
  HParVersion* pv=(HParVersion*)gHades->getRuntimeDb()->getCurrentEventFile()
                        ->getParVersion(name);
  version=pv->getInputVersion(inputNumber);
  contVers=pCalPar->getInputVersion(inputNumber);
  if (version!=-1) {
    if (contVers==version) return kTRUE;
    else pCalPar->setInputVersion(version,inputNumber);
  } else {
    version=getCalparVersion(runId);
    if (version==-1) return kFALSE;
    if (contVers==version) return kTRUE;
    pCalPar->setInputVersion(version,inputNumber);
  }

/*
  char statement[]=
    "select layer_nr,wire_nr,slope,offset "
    "from (select signal_id as si,max(cal_pt_vers_id) as oi "
          "from mdc_ana.cal_par_time "
          "where cal_pt_vers_id<=? group by signal_id),"
       "mdc_ana.signal,mdc_ana.cal_par_time "
    "where mdc_obj_id=? "
       "and ((signal.signal_id=si) "
       "and (cal_par_time.signal_id=si) "
       "and (cal_par_time.cal_pt_vers_id=oi))";
*/

  char statement[]=
    "select layer_nr,wire_nr,slope,offset "
    "from mdc_ana.signal,mdc_ana.cal_par_time "
    "where mdc_obj_id=? "
      "and signal.signal_id=cal_par_time.signal_id "
      "and cal_par_time.cal_pt_vers_id="
      "(select cal_pt_vers_id "
          "from mdc_ana.module_version_set,mdc_ana.cal_version_set,mdc_ana.run "
          "where module_version_set.module_version_set_id="
                "cal_version_set.module_version_set_id "
          "and cal_version_set.run_id=run.run_id "
          "and run.run_id=? and to_number(cal_version_set.version)=?)";

  Long_t vers=(Long_t)version;
  Long_t objId, layer, cell;
  Float_t slope, offset;
  SDWORD objId_Ind=0, runId_Ind=0, vers_Ind=0;
  SDWORD  layer_Ind, cell_Ind, slope_Ind, offset_Ind;
  RETCODE rc;

  SQLBindCol(hstmt,1,SQL_C_LONG,&layer,0,&layer_Ind);
  SQLBindCol(hstmt,2,SQL_C_LONG,&cell,0,&cell_Ind);
  SQLBindCol(hstmt,3,SQL_C_FLOAT,&slope,0,&slope_Ind);
  SQLBindCol(hstmt,4,SQL_C_FLOAT,&offset,0,&offset_Ind);

  SQLBindParameter(hstmt,1,SQL_PARAM_INPUT,SQL_C_LONG,SQL_INTEGER,0,0,
                   &objId,0,&objId_Ind);
  SQLBindParameter(hstmt,2,SQL_PARAM_INPUT,SQL_C_LONG,SQL_INTEGER,0,0,
                   &runId,0,&runId_Ind);
  SQLBindParameter(hstmt,3,SQL_PARAM_INPUT,SQL_C_LONG,SQL_INTEGER,0,0,
                   &vers,0,&vers_Ind);

  SQLPrepare(hstmt,(UCHAR *)statement,SQL_NTS);


  initModules->Reset();
  Bool_t allFound=kTRUE;
  HMdcCalPar& mdc=*pCalPar;
  Int_t pos, count, l, c;
  for(Int_t i=0;i<6;i++) {
    HMdcCalParSec& sec=mdc[i];
    for(Int_t j=0;j<4;j++) {
      HMdcCalParMod& mod=sec[j];
      objId=(Long_t)(id[i][j]);
      pos=i*4+j;
      if (set[pos]) {
        if (objId>0) {
          rc=SQLExecute(hstmt);
          if (rc!=SQL_SUCCESS && rc!=SQL_SUCCESS_WITH_INFO) {
            pConn->showErrorMessage("HOraMdc::readCalpar: statement error");
            cout<<"return code   "<<rc<<endl;
            SQLFreeStmt(hstmt,SQL_CLOSE);
            return kFALSE;
          }
          count=0;
          while ((rc=SQLFetch(hstmt))!=SQL_NO_DATA_FOUND) {
            l=(Int_t)(layer-1);
            c=(Int_t)(cell-1);
            mod[l][c].fill(slope,offset);
            if (c==1) count++;
          }
          if (count==6) {
            set[pos]=0;
            initModules->AddAt(pos+1,pos);
          }
          else allFound=kFALSE;
        }
        else allFound=kFALSE;
      }
    }
  }
  SQLFreeStmt(hstmt,SQL_CLOSE);
  pCalPar->setChanged();
  printInfo("MdcCalPar: module(s) initialized from Oracle: ");
  return allFound;
}


Bool_t HOraMdc::read(HMdcLookupGeom* pPar, Int_t* set) {
  // reads and fills the container "MdcLookupGeom" for mapping
  void* hstmt=pConn->getHstmt();
  if (!hstmt) {
    cerr<<"no connection to database";
    return kFALSE;
  }

  Long_t runId=pConn->getRunId();
  if (runId==-1L) return kFALSE;

  Int_t version, contVers;
  Text_t* name=pPar->GetName();
  HParVersion* pv=(HParVersion*)gHades->getRuntimeDb()->getCurrentEventFile()
                        ->getParVersion(name);
  version=pv->getInputVersion(inputNumber);
  contVers=pPar->getInputVersion(inputNumber);
  if (version!=-1) {
    if (contVers==version) return kTRUE;
    else pPar->setInputVersion(version,inputNumber);
  } else {
    version=getLookupVersion(runId);
cout<<endl<<"lookup version: "<<version<<endl;
    if (version==-1) return kFALSE;
    if (contVers==version) return kTRUE;
    pPar->setInputVersion(version,inputNumber);
  }

  if (execProcRuns(runId)==kFALSE) return kFALSE;
cout<<"procedure executed"<<endl;

  char statement[]=
  "select mbo_pos_id,tdc_pos,tdc_chan,layer_nr,wire_nr "
  "from mdc_ana.signal_lookup_tdc_r,mdc_ana.signal_lookup_mdc_r "
  "where mdc_obj_id=? "
     "and signal_lookup_tdc_r.signal_id=signal_lookup_mdc_r.signal_id "
     "and wire_nr>0 "
  "order by mbo_pos_id";

  Long_t objId, mbo, tdc, chan, layer, cell;
  SDWORD objId_Ind=0;
  SDWORD mbo_Ind, tdc_Ind, chan_Ind, layer_Ind, cell_Ind;
  RETCODE rc;

  SQLBindCol(hstmt,1,SQL_C_LONG,&mbo,0,&mbo_Ind);
  SQLBindCol(hstmt,2,SQL_C_LONG,&tdc,0,&tdc_Ind);
  SQLBindCol(hstmt,3,SQL_C_LONG,&chan,0,&chan_Ind);
  SQLBindCol(hstmt,4,SQL_C_LONG,&layer,0,&layer_Ind);
  SQLBindCol(hstmt,5,SQL_C_LONG,&cell,0,&cell_Ind);

  SQLBindParameter(hstmt,1,SQL_PARAM_INPUT,SQL_C_LONG,SQL_INTEGER,0,0,
                   &objId,0,&objId_Ind);

  SQLPrepare(hstmt,(UCHAR *)statement,SQL_NTS);

  initModules->Reset();
  Bool_t allFound=kTRUE;
  HMdcLookupGeom& mdc=*pPar;
  Int_t pos, count, mo, ml, ch, l, c;
  for(Int_t i=0;i<6;i++) {
    HMdcLookupGSec& sec=mdc[i];
    for(Int_t j=0;j<4;j++) {
      HMdcLookupGMod& mod=sec[j];
      objId=(Long_t)(id[i][j]);
      pos=i*4+j;
      if (set[pos]) {
        if (objId>0) {
cout<<"start execute"<<endl;
          rc=SQLExecute(hstmt);
cout<<"back from execute"<<endl;
          if (rc!=SQL_SUCCESS && rc!=SQL_SUCCESS_WITH_INFO) {
            pConn->showErrorMessage("HOraMdc::readLookup: statement error");
            cout<<"return code   "<<rc<<endl;
            SQLFreeStmt(hstmt,SQL_CLOSE);
            return kFALSE;
          }
          count=-1;
          ml=-1;
          while ((rc=SQLFetch(hstmt))!=SQL_NO_DATA_FOUND) {
            mo=(Int_t)mbo;
            if (mo>ml) {
              count++;
              ml=mo;
cout<<count<<"  "<<ml<<endl;
            }
            ch=(((Int_t)tdc)-1)*8+(Int_t)chan;
            l=((Int_t)layer)-1;
            c=((Int_t)cell)-1;
            mod[count][ch].fill(l,c);
          }
          if (count>0) {
            set[pos]=0;
            initModules->AddAt(pos+1,pos);
          }
          else allFound=kFALSE;
        }
        else allFound=kFALSE;
      }
    }
  }
  SQLFreeStmt(hstmt,SQL_CLOSE);
  pPar->setChanged();
  printInfo("MdcPar: module(s) initialized from Oracle: ");
  return allFound;
}


Bool_t HOraMdc::write(HMdcDetector* p) {
  // nothing implemented!
  return 0;
}

Int_t HOraMdc::write(HParSet* pPar) {
  // calls special update-function for each container type, but only for
  // these containers which are allowed to change the information via
  // the analysis (e.g. MdcCalPar)
  // does nothing for the other containers
  Text_t* name=pPar->GetName();
  if (strcmp(name,"MdcCalPar")==0) return update(((HMdcCalPar*) pPar));
  return 0;
}


Int_t HOraMdc::update(HMdcCalPar* pCalPar) {
  // writes MdcPar container to Oracle
  // nothing implemented yet!
  cout<<"HOraMdc::write(HMdcCalPar*) not yet implemented"<<endl;
  return -1;
}


void HOraMdc::printIds() {
  // prints the id numbers of all modules
  for(Int_t i=0;i<6;i++) {
    cout<<"sector: "<<i+1<<"   mdc ids:";
    for(Int_t j=0;j<4;j++) { cout<<" "<<id[i][j]; }
    cout<<'\n';
  }
  cout<<'\n';
}


Int_t HOraMdc::readIds() {
  // reads the id numbers of all modules
  // and stores them in array id
  // return mdc_p_vers_id (unique number for placement)
  // return -1 if nothing found
  void* hstmt=pConn->getHstmt();
  if (!hstmt) {
    cerr<<"no connection to database";
    return -1;
  }

  for(Int_t i=0;i<6;i++) {
    for(Int_t j=0;j<4;j++) { id[i][j]=0; }
  }

  
  Long_t runId=-1L;
  if ((runId=pConn->getRunId())==-1L) return -1L;

  char statement[]=
    "select mdc_placement.mdc_obj_id,sector,plane,"
            "to_number(mdc_placement.mdc_p_vers_id) "
    "from mdc_ana.mdc_placement,mdc_ana.mdc_pos "
    "where  mdc_placement.mdc_p_vers_id="
          "(select version_set.mdc_p_vers_id "
             "from mdc_ana.version_set,mdc_ana.run "
             "where run_id=? and version_set.version_set_id=run.version_set_id) "
          "and mdc_placement.mdc_pos_id=mdc_pos.mdc_pos_id";

  Long_t objId, sec, mod, vers;
  SDWORD runId_Ind=0, objId_Ind, sec_Ind, mod_Ind, vers_Ind;
  RETCODE rc;

  SQLBindCol(hstmt,1,SQL_C_LONG,&objId,0,&objId_Ind);
  SQLBindCol(hstmt,2,SQL_C_LONG,&sec,0,&sec_Ind);
  SQLBindCol(hstmt,3,SQL_C_LONG,&mod,0,&mod_Ind);
  SQLBindCol(hstmt,4,SQL_C_LONG,&vers,0,&vers_Ind);
  SQLBindParameter(hstmt,1,SQL_PARAM_INPUT,SQL_C_LONG,SQL_INTEGER,0,0,
                   &runId,0,&runId_Ind);

  SQLPrepare(hstmt,(UCHAR *)statement,SQL_NTS);

  rc=SQLExecute(hstmt);
  if (rc!=SQL_SUCCESS && rc!=SQL_SUCCESS_WITH_INFO) {
    pConn->showErrorMessage("HOraMdc::readIds: statement error");
    SQLFreeStmt(hstmt,SQL_CLOSE);
    return -1L;
  }

  Int_t s, m;
  while ((rc=SQLFetch(hstmt))!=SQL_NO_DATA_FOUND) {
    s=(Int_t)(sec-1);
    m=(Int_t)(mod-1);
    id[s][m]=(Int_t)objId;
    pVersId=(Int_t)vers;
  }
  SQLFreeStmt(hstmt,SQL_CLOSE);
  isActiv=kTRUE;
  return pVersId;
}


Int_t HOraMdc::getCalparVersion(Long_t runId) {
  // reads the newest version number for the calibration parameters
  // valid for the current event file
  void* hstmt=pConn->getHstmt();
  if (!hstmt) {
    cerr<<"no connection to database";
    return kFALSE;
  }

//  char statement[]=
//    "select max(version) "
//    "from mdc_ana.cal_version_set,mdc_ana.run "
//    "where run_nr=? "
//      "and cal_version_set.run_id=run.run_id";

    char statement[]=
    "select max(version) from mdc_ana.cal_version_set where run_id=?";

  Int_t version=-1;
  Long_t vers=-1L;
  SDWORD runId_Ind=0, vers_Ind;
  RETCODE rc;

  SQLBindCol(hstmt,1,SQL_C_LONG,&vers,0,&vers_Ind);
  SQLBindParameter(hstmt,1,SQL_PARAM_INPUT,SQL_C_LONG,SQL_INTEGER,0,0,
                   &runId,0,&runId_Ind);

  rc=SQLExecDirect(hstmt,(UCHAR *)statement,SQL_NTS);
  if (rc!=SQL_SUCCESS && rc!=SQL_SUCCESS_WITH_INFO) {
    pConn->showErrorMessage("HOraMdc::getCalparVersion: statement error");
    SQLFreeStmt(hstmt,SQL_CLOSE);
    return -1;
  }

  if (SQLFetch(hstmt)!=SQL_NO_DATA_FOUND) {
    version=(Int_t)vers;
  }
  SQLFreeStmt(hstmt,SQL_CLOSE);
  return version;
}


void HOraMdc::printInfo(Text_t* msg) {
  // prints the modules initialized from Oracle
  // will later go to the log file
  Bool_t first=kTRUE;
  for(Int_t i=0;i<24;i++) {
    if (initModules->At(i)) {
      if (first) {
        cout<<msg;
        first=kFALSE;
      }
      cout<<(initModules->At(i)-1);
    }
  }
  cout<<'\n';
}


Int_t HOraMdc::getLookupVersion(Long_t runId) {
  // reads version_set_id  for a run given by run_id
  // if the version_set_id changes the lookup container must be
  // reinitialized
  void* hstmt=pConn->getHstmt();
  if (!hstmt) {
    cerr<<"no connection to database";
    return kFALSE;
  }
    char statement[]=
    "select to_number(version_set_id) from mdc_ana.run where run_id=?";

  Int_t version=-1;
  Long_t vers=-1L;
  SDWORD runId_Ind=0, vers_Ind;
  RETCODE rc;

  SQLBindCol(hstmt,1,SQL_C_LONG,&vers,0,&vers_Ind);
  SQLBindParameter(hstmt,1,SQL_PARAM_INPUT,SQL_C_LONG,SQL_INTEGER,0,0,
                   &runId,0,&runId_Ind);

  rc=SQLExecDirect(hstmt,(UCHAR *)statement,SQL_NTS);
  if (rc!=SQL_SUCCESS && rc!=SQL_SUCCESS_WITH_INFO) {
    pConn->showErrorMessage("HOraMdc::getLookupVersion(): statement error");
    SQLFreeStmt(hstmt,SQL_CLOSE);
    return -1;
  }

  if (SQLFetch(hstmt)!=SQL_NO_DATA_FOUND) {
    version=(Int_t)vers;
  }
  SQLFreeStmt(hstmt,SQL_CLOSE);
  return version;
}


Bool_t HOraMdc::execProcRuns(Long_t runId) {
  // executes the PL/SQL procedure runs for given runId
  // This procedure creates view in Oracle.
  void* hstmt=pConn->getHstmt();
  if (!hstmt) {
    cerr<<"no connection to database";
    return kFALSE;
  }
  SDWORD runId_Ind=0;
  Bool_t r=kTRUE;
  SQLBindParameter(hstmt,1,SQL_PARAM_INPUT,SQL_C_LONG,SQL_INTEGER,0,0,
                   &runId,0,&runId_Ind);
  RETCODE rc=SQLExecDirect(hstmt,
               (UCHAR *)("{call mdc_ana.runs.set_run_id(?)}"),SQL_NTS);
  if (rc!=SQL_SUCCESS && rc!=SQL_SUCCESS_WITH_INFO) {
    pConn->showErrorMessage("HOraMdc::execProcRuns: statement error");
    r=kFALSE;
  }
  SQLFreeStmt(hstmt,SQL_CLOSE);
  return r;
}

///////////////////////////////////////////////////////////////////////
/////////////////  funtions not used anymore !  ///////////////////////
///////////////////////////////////////////////////////////////////////

/*
void HOraMdc::printVersions() {
  cout<<"Mdc Versions for Run "<<runId<<'\n';
  cout<<"position: "<<posVersion<<'\n';
  cout<<"calpar:   "<<calparVersion<<'\n'<<'\n';
}
*/

/*
Bool_t HOraMdc::readVersions() {
  // maximum length of Mdc version-columns in Oracle
  static const int MDC_VERS_LEN=40;
  void* hstmt=pConn->getHstmt();
  if (!hstmt) {
    cerr<<"no connection to database";
    return kFALSE;
  }

// must be changed later: not run_id but run_nr  + Mdc-version!!!!!!!!!!!

  char statement[]=
    "select mdc_p_vers_id,cal_pt_vers_id "
    "from mdc_ana.version_set,mdc_ana.run "
    "where run_id=? and version_set.version_set_id=run.version_set_id";

  Int_t runNr;
  char posVers[MDC_VERS_LEN];
  char calparVers[MDC_VERS_LEN];
  SDWORD runNr_Ind=0;
  SDWORD posVers_Ind;
  SDWORD calparVers_Ind;
  RETCODE rc;

  SQLBindCol(hstmt,1,SQL_C_CHAR,posVers,sizeof(posVers),&posVers_Ind);
  SQLBindCol(hstmt,2,SQL_C_CHAR,calparVers,sizeof(calparVers),&calparVers_Ind);
  SQLBindParameter(hstmt,1,SQL_PARAM_INPUT,SQL_C_LONG,SQL_INTEGER,10,0,
                   &runNr,0,&runNr_Ind);

  runNr=runId;

  rc=SQLExecDirect(hstmt,(UCHAR *)statement,SQL_NTS);
  if (rc!=SQL_SUCCESS && rc!=SQL_SUCCESS_WITH_INFO) {
    pConn->showErrorMessage("HOraMdc::readVersions: statement error");
    SQLFreeStmt(hstmt,SQL_CLOSE);
    return kFALSE;
  }
  while ((rc=SQLFetch(hstmt))!=SQL_NO_DATA_FOUND) {
     strcpy(posVersion,posVers);
     strcpy(calparVersion,calparVers);
  }
  SQLFreeStmt(hstmt,SQL_CLOSE);
  return kTRUE;
}

// was part of other function
  HMdcDetector* set=(HMdcDetector*)gHades->getSetup()->getDetector("Mdc");
  if (set) {
    Int_t notfound[24], k=0;
    for(Int_t i=0;i<6;i++) {
      for(Int_t j=0;j<4;j++) {
        if (!set->getModule(i,j)) id[i][j]=0;
        else { if (!id[i][j]) notfound[k++]=i*4+j; }
      }
    }
    if (k) {
      cout<<"modules not found in database: ";
      for(Int_t i=0;i<k;i++) { cout<<notfound[i]<<" "; }
      cout<<'\n';
      allFound=kFALSE;
    }
    else allFound=kTRUE;
  }

*/

