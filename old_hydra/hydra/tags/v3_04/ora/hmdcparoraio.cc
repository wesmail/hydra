//*-- AUTHOR : Ilse Koenig
//*-- Modified : 25/08/99

//////////////////////////////////////////////////////////////////////////////
// HMdcParOraIo
//
// Interface class to database Oracle for input/output of parameters needed
// by the Mdc
//
// This class replaces the former class HOraMdc 
//////////////////////////////////////////////////////////////////////////////

#include "hmdcparoraio.h"
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
#include "hmdcdigitpar.h"
#include "hmdclayergeompar.h"
#include "hmdcmboreadout.h"
#include "hmdcgeompar.h"
#include "hgeomcompositevolume.h"
#include <iostream.h>

#include "iodbc.h"
#include "udbcext.h"
#include "isql.h"
#include "isqlext.h"

ClassImp(HMdcParOraIo)

HMdcParOraIo::HMdcParOraIo(HOraConn* p) : HDetParIo() {
  // constructor
  // sets the name of the I/O class "HMdcParIo"
  // gets the pointer to the connection class
  fName="HMdcParIo";
  pConn=p;
  isActiv=kFALSE;
  viewsRunId=-1;
  pVersId=-1;
  initModules=new TArrayI(24);
}


HMdcParOraIo::~HMdcParOraIo() {
  delete initModules;
}


Bool_t HMdcParOraIo::init(HParSet* pPar,Int_t* set) {
  // calls special read-function for each parameter container 
  if (!isActiv) {
    if (readIds()==-1) return kFALSE;
  }
  if (pVersId==-1) return kFALSE;  // no modules found
  const Text_t* name=pPar->GetName();
  if (strcmp(name,"MdcRawStruct")==0) return read(((HMdcRawStruct*)pPar),set);
  if (strcmp(name,"MdcGeomStruct")==0) return read(((HMdcGeomStruct*)pPar),set);
  if (strcmp(name,"MdcCalPar")==0) return read(((HMdcCalPar*)pPar),set);
  if (strcmp(name,"MdcLookupGeom")==0) return read(((HMdcLookupGeom*)pPar),set);
  if (strcmp(name,"MdcDigitPar")==0) return read(((HMdcDigitPar*)pPar),set);
  if (strcmp(name,"MdcLayerGeomPar")==0) return read(((HMdcLayerGeomPar*)pPar),set);
  if (strcmp(name,"MdcMboReadout")==0) return read(((HMdcMboReadout*)pPar),set);
  if (strcmp(name,"MdcGeomPar")==0) return read(((HMdcGeomPar*)pPar),set);
  cout<<"initialization of "<<name<<" not possible from Oracle!"<<endl;
  return kFALSE;
}


Bool_t HMdcParOraIo::read(HMdcRawStruct* pPar,Int_t* set) {
  // reads the names (characterizes the position) of the motherboards
  // and the maximum number of TDCs on each of it and fills the 
  // MdcRawStruct container
  void* hstmt=pConn->getHstmt();
  if (!hstmt) {
    cerr<<"no connection to database";
    return kFALSE;
  }

  char statement[]=
  "select to_number(mbo_pos_id),n_channel_max*n_pos_max*n_dbo_pos_max,"
          "to_number(mbo_p_vers_id) "
    "from mdc_ana.mbo_placement,mdc_ana.mbo_obj,mdc_ana.mbo_type "
    "where mdc_obj_id=? and mbo_p_vers_id="
             "(select version_set.mbo_p_vers_id "
                 "from mdc_ana.version_set,mdc_ana.run "
                 "where run_id=? "
                    "and version_set.version_set_id=run.version_set_id) "
        "and mbo_placement.mbo_obj_id=mbo_obj.mbo_obj_id "
        "and mbo_obj.mbo_type_id=mbo_type.mbo_type_id " 
    "order by mbo_pos_id";


  Long_t runId=pConn->getRunId();
  if (runId==-1L) return kFALSE;
  Long_t objId, mbo, num, vers;
  Int_t pos, n, version=-1;
  char mboName[4];
  SDWORD objId_Ind=0, run_Ind=0, num_Ind, vers_Ind, mbo_Ind;
  RETCODE rc;
  Bool_t allFound=kTRUE;

  SQLBindCol(hstmt,1,SQL_C_LONG,&mbo,0,&mbo_Ind);
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
                "HMdcParOraIo::read(HMdcRawStruct*,Int_t*): statement error");
            SQLFreeStmt(hstmt,SQL_CLOSE);
            return kFALSE;
          }
          n=0;
          while ((rc=SQLFetch(hstmt))!=SQL_NO_DATA_FOUND) {
            sprintf(mboName,"%3i",(int)mbo);
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


Bool_t HMdcParOraIo::read(HMdcGeomStruct* pPar,Int_t* set) {
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
            pConn->showErrorMessage(
                 "HMdcParOraIo::readNCells: statement error");
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


Bool_t HMdcParOraIo::read(HMdcCalPar* pCalPar, Int_t* set) {
  // reads calibration parameters and fill MdcCalPar container
  void* hstmt=pConn->getHstmt();
  if (!hstmt) {
    cerr<<"no connection to database";
    return kFALSE;
  }

  Long_t runId=pConn->getRunId();
  if (runId==-1L) return kFALSE;

  Int_t version, contVers;
  HParVersion* pv=(HParVersion*)gHades->getRuntimeDb()->getCurrentEventFile()
                        ->getParVersion((char*)pCalPar->GetName());
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

  char statement[]=
    "select layer_nr,wire_nr,slope,offset "
    "from (select signal_id as si,wire_nr,layer_nr,cal_pt_vers_id as vi "
           "from mdc_ana.signal, mdc_ana.module_version_set "
           "where module_version_set.mdc_obj_id=? "
              "and signal.mdc_obj_id=module_version_set.mdc_obj_id "
              "and to_number(module_version_set_id)=?"
         "),mdc_ana.cal_par_time "
     "where cal_par_time.signal_id=si "
     "and cal_pt_vers_id=vi";

  Long_t vers=(Long_t)version;
  Long_t objId, layer, cell;
  Float_t slope, offset;
  SDWORD objId_Ind=0, vers_Ind=0;
  SDWORD  layer_Ind, cell_Ind, slope_Ind, offset_Ind;
  RETCODE rc;

  SQLBindCol(hstmt,1,SQL_C_LONG,&layer,0,&layer_Ind);
  SQLBindCol(hstmt,2,SQL_C_LONG,&cell,0,&cell_Ind);
  SQLBindCol(hstmt,3,SQL_C_FLOAT,&slope,0,&slope_Ind);
  SQLBindCol(hstmt,4,SQL_C_FLOAT,&offset,0,&offset_Ind);

  SQLBindParameter(hstmt,1,SQL_PARAM_INPUT,SQL_C_LONG,SQL_INTEGER,0,0,
                   &objId,0,&objId_Ind);
  SQLBindParameter(hstmt,2,SQL_PARAM_INPUT,SQL_C_LONG,SQL_INTEGER,0,0,
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
            pConn->showErrorMessage(
                 "HMdcParOraIo::readCalpar: statement error");
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


Bool_t HMdcParOraIo::read(HMdcLookupGeom* pPar, Int_t* set) {
  // reads and fills the container "MdcLookupGeom" for mapping
  void* hstmt=pConn->getHstmt();
  if (!hstmt) {
    cerr<<"no connection to database";
    return kFALSE;
  }

  Long_t runId=pConn->getRunId();
  if (runId==-1L) return kFALSE;

  Int_t version, contVers;
  HParVersion* pv=(HParVersion*)gHades->getRuntimeDb()->getCurrentEventFile()
                        ->getParVersion((char*)pPar->GetName());
  version=pv->getInputVersion(inputNumber);
  contVers=pPar->getInputVersion(inputNumber);
  if (version!=-1) {
    if (contVers==version) return kTRUE;
    else pPar->setInputVersion(version,inputNumber);
  } else {
    version=getLookupVersion(runId);
    if (version==-1) return kFALSE;
    if (contVers==version) return kTRUE;
    pPar->setInputVersion(version,inputNumber);
  }

  if (viewsRunId!=runId) {
    if (execProcRuns(runId)==kFALSE) return kFALSE;
  }

  char statement[]=
  "select to_number(mbo_pos_id),tdc_pos,tdc_chan,layer_nr,wire_nr "
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
          rc=SQLExecute(hstmt);
          if (rc!=SQL_SUCCESS && rc!=SQL_SUCCESS_WITH_INFO) {
            pConn->showErrorMessage(
                  "HMdcParOraIo::readLookup: statement error");
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
              mo=atoi(mod[count].GetName());
	      if (mo!=ml) {
                char buf[80];
                sprintf(buf,
                    "HMdcParOraIo::readLookup: motherboard %2i not found",mo);
                pConn->showErrorMessage(buf);
                return kFALSE;
              }    
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
  printInfo("MdcLookupGeom: module(s) initialized from Oracle: ");
  return allFound;
}


Bool_t HMdcParOraIo::read(HMdcDigitPar* pPar,Int_t* set) {
  // reads physical parameters for digitisation procedure

  void* hstmt=pConn->getHstmt();
  if (!hstmt) {
    cerr<<"no connection to database";
    return kFALSE;
  }

  Long_t runId=pConn->getRunId();
  if (runId==-1L) return kFALSE;

  Int_t version, contVers;
  Text_t* name=(char*)pPar->GetName();
  HParVersion* pv=(HParVersion*)gHades->getRuntimeDb()->getCurrentEventFile()
                        ->getParVersion(name);
  version=pv->getInputVersion(inputNumber);
  contVers=pPar->getInputVersion(inputNumber);
  if (version!=-1) {
    if (contVers==version) return kTRUE;
    else pPar->setInputVersion(version,inputNumber);
  } else {
    version=getDigiVersion(runId);
    if (version==-1) return kFALSE;
    if (contVers==version) return kTRUE;
    pPar->setInputVersion(version,inputNumber);
  }

  char statement[]=
    "select layer_nr,driftvel,mdceff,sr1, sr2, sr3 "
    "from mdc_ana.cal_digitisation "
    "where mdc_obj_id=? and cal_digi_vers_id=?";

  Int_t pos,n;
  Long_t vers=(Long_t)version;
  Long_t objId, layNum;
  Float_t driftVel, mdcEff, sP1, sP2, sP3, sP4, sP5;
  SDWORD objId_Ind=0, vers_Ind=0;
  SDWORD layNum_Ind, driftVel_Ind, mdcEff_Ind, sP1_Ind, sP2_Ind, sP3_Ind;
  SDWORD sP4_Ind, sP5_Ind;
  RETCODE rc;
  Bool_t allFound=kTRUE;

  SQLBindCol(hstmt,1,SQL_C_LONG,&layNum,0,&layNum_Ind);
  SQLBindCol(hstmt,2,SQL_C_FLOAT,&driftVel,0,&driftVel_Ind);
  SQLBindCol(hstmt,3,SQL_C_FLOAT,&mdcEff,0,&mdcEff_Ind);
  SQLBindCol(hstmt,4,SQL_C_FLOAT,&sP1,0,&sP1_Ind);
  SQLBindCol(hstmt,5,SQL_C_FLOAT,&sP2,0,&sP2_Ind);
  SQLBindCol(hstmt,6,SQL_C_FLOAT,&sP3,0,&sP3_Ind);
  SQLBindCol(hstmt,7,SQL_C_FLOAT,&sP4,0,&sP4_Ind);
  SQLBindCol(hstmt,8,SQL_C_FLOAT,&sP5,0,&sP5_Ind);

  SQLBindParameter(hstmt,1,SQL_PARAM_INPUT,SQL_C_LONG,SQL_INTEGER,0,0,
                   &objId,0,&objId_Ind);
  SQLBindParameter(hstmt,2,SQL_PARAM_INPUT,SQL_C_LONG,SQL_INTEGER,0,0,
                   &vers,0,&vers_Ind);
  SQLPrepare(hstmt,(UCHAR*)statement,SQL_NTS);

  initModules->Reset();
  HMdcDigitPar& mdc=*pPar;
  for(Int_t i=0;i<mdc.getSize();i++) {
    HMdcDigitParSec& sec=mdc[i];
    for(Int_t j=0;j<sec.getSize();j++) {
      HMdcDigitParMod& mod=sec[j];
      objId=(Long_t)(id[i][j]);
      pos=i*4+j;
      if (set[pos]) {
        if (objId>0) {
          rc=SQLExecute(hstmt);
          if (rc!=SQL_SUCCESS && rc!=SQL_SUCCESS_WITH_INFO) {
            pConn->showErrorMessage(
                  "HMdcParOraIo::read(HMdcDigitPar*,Int_t*): statement error");
            SQLFreeStmt(hstmt,SQL_CLOSE);
            return kFALSE;
          }
          n=0;
          while ((rc=SQLFetch(hstmt))!=SQL_NO_DATA_FOUND) {
            mod[((Int_t)(layNum-1))].fill(driftVel, mdcEff, sP1, sP2, sP3,
                                           sP4, sP5);
            n++;
          }
          if (n==6) {
            set[pos]=0;
            initModules->AddAt(pos+1,pos);
          } else allFound=kFALSE;
        } else allFound=kFALSE;
      }
    }
  }
  SQLFreeStmt(hstmt,SQL_CLOSE);
  pPar->setChanged();
  printInfo("MdcDigitPar: module(s) initialized from Oracle: ");
  return allFound;
}


Bool_t HMdcParOraIo::read(HMdcLayerGeomPar* pPar,Int_t* set) {
// reads geometry parameters for digitisation procedure

  void* hstmt=pConn->getHstmt();
  if (!hstmt) {
    cerr<<"no connection to database";
    return kFALSE;
  }

  char statement[]=
    "select layer_nr,cell_pitch,cell_thickness,"
           "inclination_angle,central_wire_nr "
    "from mdc_ana.mdc_obj, mdc_ana.mdc_type, mdc_ana.layer_type "
    "where mdc_obj.mdc_obj_id=? "
    "and ((mdc_type.mdc_type_id=mdc_obj.mdc_type_id) "
    "and (layer_type.mdc_type_id=mdc_type.mdc_type_id))";

  Int_t pos, n;
  Long_t objId, layNum;
  Float_t pitch, cathDist, wireOrient, centralWire;
  SDWORD objId_Ind=0;
  SDWORD layNum_Ind, pitch_Ind, cathDist_Ind, wireOrient_Ind, centalWire_Ind;
  RETCODE rc;
  Bool_t allFound=kTRUE;

  SQLBindCol(hstmt,1,SQL_C_LONG,&layNum,0,&layNum_Ind);
  SQLBindCol(hstmt,2,SQL_C_FLOAT,&pitch,0,&pitch_Ind);
  SQLBindCol(hstmt,3,SQL_C_FLOAT,&cathDist,0,&cathDist_Ind);
  SQLBindCol(hstmt,4,SQL_C_FLOAT,&wireOrient,0,&wireOrient_Ind);
  SQLBindCol(hstmt,5,SQL_C_FLOAT,&centralWire,0,&centalWire_Ind);

  SQLBindParameter(hstmt,1,SQL_PARAM_INPUT,SQL_C_LONG,SQL_INTEGER,0,0,
                   &objId,0,&objId_Ind);
  SQLPrepare(hstmt,(UCHAR*)statement,SQL_NTS);

  initModules->Reset();
  HMdcLayerGeomPar& mdc=*pPar;
  for(Int_t i=0;i<mdc.getSize();i++) {
    HMdcLayerGeomParSec& sec=mdc[i];
    for(Int_t j=0;j<sec.getSize();j++) {
      HMdcLayerGeomParMod& mod=sec[j];
      objId=(Long_t)(id[i][j]);
      pos=i*4+j;
      if (set[pos]) {
        if (objId>0) {
          rc=SQLExecute(hstmt);
          if (rc!=SQL_SUCCESS && rc!=SQL_SUCCESS_WITH_INFO) {
            pConn->showErrorMessage(
              "HMdcParOraIo::read(HMdcDigitGeomPar*,Int_t*): statement error");
            SQLFreeStmt(hstmt,SQL_CLOSE);
            return kFALSE;
          }
          n=0;
          while ((rc=SQLFetch(hstmt))!=SQL_NO_DATA_FOUND) {
            mod[((Int_t)(layNum-1))].fill((pitch/10.F),(cathDist/10.F),
                                           wireOrient,centralWire);
            n++;
          }
          if (n==6) {
            set[pos]=0;
            initModules->AddAt(pos+1,pos);
          }
        }
        else allFound=kFALSE;
      }
    }
  }
  SQLFreeStmt(hstmt,SQL_CLOSE);
  pPar->setInputVersion(pVersId,inputNumber);
  pPar->setChanged();
  printInfo("MdcLayerGeomPar: module(s) initialized from Oracle: ");
  return allFound;
}


Bool_t HMdcParOraIo::read(HMdcMboReadout* pPar, Int_t* set) {
  void* hstmt=pConn->getHstmt();
  if (!hstmt) {
    cerr<<"no connection to database";
    return kFALSE;
  }

  Long_t runId=pConn->getRunId();
  if (runId==-1L) return kFALSE;

  Int_t version, contVers;
  HParVersion* pv=(HParVersion*)gHades->getRuntimeDb()->getCurrentEventFile()
                        ->getParVersion((char*)pPar->GetName());
  version=pv->getInputVersion(inputNumber);
  contVers=pPar->getInputVersion(inputNumber);
  if (version!=-1) {
    if (contVers==version) return kTRUE;
    else pPar->setInputVersion(version,inputNumber);
  } else {
    version=getLookupVersion(runId);  // will change later!!!!!!!
    if (version==-1) return kFALSE;
    if (contVers==version) return kTRUE;
    pPar->setInputVersion(version,inputNumber);
  }

  if (viewsRunId!=runId) {
    if (execProcRuns(runId)==kFALSE) return kFALSE;
  }

  char statement[]=
    "select to_number(mbo_pos_id),pos_in_cable,roc_input_con,"
           "roc_pos_in_crate_id,sam_input_con,sam_nr "
    "from mdc_ana.signal_lookup_unpack_r "
    "where mdc_obj_id=? "
    "order by mbo_pos_id";
  
  Long_t in1, out1=0, out2=0, out3=0, out4=0, out5=0, out6=0;
  SDWORD in1_Ind=0, out1_Ind, out2_Ind, out3_Ind, out4_Ind, out5_Ind, out6_Ind;
  RETCODE rc;

  SQLBindCol(hstmt,1,SQL_C_LONG,&out1,0,&out1_Ind);
  SQLBindCol(hstmt,2,SQL_C_LONG,&out2,0,&out2_Ind);
  SQLBindCol(hstmt,3,SQL_C_LONG,&out3,0,&out3_Ind);
  SQLBindCol(hstmt,4,SQL_C_LONG,&out4,0,&out4_Ind);
  SQLBindCol(hstmt,5,SQL_C_LONG,&out5,0,&out5_Ind);
  SQLBindCol(hstmt,6,SQL_C_LONG,&out6,0,&out6_Ind);
  SQLBindParameter(hstmt,1,SQL_PARAM_INPUT,SQL_C_LONG,SQL_INTEGER,0,0,
                   &in1,0,&in1_Ind);
  SQLPrepare(hstmt,(UCHAR *)statement,SQL_NTS);

  initModules->Reset();
  Bool_t allFound=kTRUE;
  HMdcMboReadout& mdc=*pPar;
  Int_t pos, count, mo, ml;
  for(Int_t i=0;i<6;i++) {
    HMdcMboReadoutSec& sec=mdc[i];
    for(Int_t j=0;j<4;j++) {
      HMdcMboReadoutMod& mod=sec[j];
      in1=(Long_t)(id[i][j]);
      pos=i*4+j;
      if (set[pos]) {
        if (in1>0) {
          rc=SQLExecute(hstmt);
          if (rc!=SQL_SUCCESS && rc!=SQL_SUCCESS_WITH_INFO) {
            pConn->showErrorMessage(
                "HMdcParOraIo::readMboReadout: statement error");
            cout<<"return code   "<<rc<<endl;
            SQLFreeStmt(hstmt,SQL_CLOSE);
            return kFALSE;

          }
          count=-1;
          ml=-1;
          while ((rc=SQLFetch(hstmt))!=SQL_NO_DATA_FOUND) {
            mo=(Int_t)out1;
            count++;
            HMdcMboReadoutMoth& moth=mod[count];
            ml=mo;
            mo=atoi(moth.GetName());
            if (mo!=ml) {
              char buf[80];
              sprintf(buf,
                "HMdcParOraIo::readMboReadout: motherboard %2i not found",mo);
              pConn->showErrorMessage(buf);
              return kFALSE;
            }    
            moth.setBus((Int_t)out2);
            moth.setBusPos((Int_t)out3);
            moth.setRoc((Int_t)out4);
            moth.setSam((Int_t)out5);
            moth.setSamPos((Int_t)out6);
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
  printInfo("MdcMboReadout: module(s) initialized from Oracle: ");
  return allFound;
}


Bool_t HMdcParOraIo::read(HMdcGeomPar* pPar, Int_t* set) {
  // reads the geometry of the MDC and fills the MdcGeomPar container
  Long_t runId=pConn->getRunId();
  if (runId==-1L) return kFALSE;

  Int_t version, contVers;
  Text_t* name=(char*)pPar->GetName();
  HParVersion* pv=(HParVersion*)gHades->getRuntimeDb()->getCurrentEventFile()
                        ->getParVersion(name);
  version=pv->getInputVersion(inputNumber);
  contVers=pPar->getInputVersion(inputNumber);
  if (version!=-1) {
    if (contVers==version) return kTRUE;
    else pPar->setInputVersion(version,inputNumber);
  } else {
    version=getGeomVersion(runId);
    if (version==-1) return kFALSE;
    if (contVers==version) return kTRUE;
    pPar->setInputVersion(version,inputNumber);
  }

  Bool_t allFound=readModGeomNames(pPar,set);
  readLayerGeomNames(pPar,set);

  Long_t gVers=(Long_t)version;
  for(Int_t pos=0;pos<pPar->getNumModules();pos++) {
    HModGeomPar* pMod=pPar->getModule(pos);
    if (pMod && set[pos] && initModules->At(pos)) {
      Long_t geomId=readGeomIndex(pMod->GetName());
      HGeomTransform& tp=pMod->getLabTransform();
      char mother[10];
      if (readGeomTransform(&tp,mother,geomId,gVers)==kTRUE) {
        set[pos]=0;
      }
      else {
        allFound=kFALSE;
        initModules->AddAt(0,pos);
      }
    }
  }
  for(Int_t i=0;i<pPar->getNumRefModules();i++) {  
    HGeomCompositeVolume* refMod=pPar->getRefVolume(i);
    if (refMod && refMod->getNumPoints()==0) {
      HGeomTransform& tm=refMod->getTransform();
      Long_t mInd=readGeomIndex(refMod->GetName());
      char mother[10];
      readGeomTransform(&tm,mother,mInd,gVers);
      refMod->setMother(mother);
      readGeomShape(refMod,mInd,gVers);
      readGeomPoints(refMod,mInd,gVers);
      for(Int_t l=0;l<refMod->getNumComponents();l++) {
        HGeomVolume* comp=refMod->getComponent(l);
        comp->setMother(refMod->GetName());
        HGeomTransform& tc=comp->getTransform();
        Long_t cInd=readGeomIndex(comp->GetName());
        readGeomTransform(&tc,mother,cInd,gVers);
        tc.transTo(tm);
        readGeomShape(comp,cInd,gVers);
        readGeomPoints(comp,cInd,gVers);
      }
    }
  } 
  pPar->setInputVersion(version,inputNumber);
  pPar->setChanged();
  printInfo("MdcGeomPar: module(s) initialized from Oracle: ");
  return allFound;
}


Bool_t HMdcParOraIo::readModGeomNames(HMdcGeomPar* pPar,Int_t* set) {
  // reads the mdc_pos_id of all modules
  // mdc_pos_id is identical with the name of the geometry volumes

  void* hstmt=pConn->getHstmt();
  char statement[]=
    "select mdc_pos_id "
    "from (select mdc_obj_id as oi,max(mdc_p_vers_id) as vi "
          "from mdc_ana.mdc_placement "
          "where mdc_obj_id=? and mdc_p_vers_id<=? "
          "group by mdc_obj_id"
         "),mdc_ana.mdc_placement "
     "where mdc_obj_id=oi and mdc_p_vers_id=vi";

  char out1[10];
  Long_t in1, in2;
  SDWORD in1_Ind=0, in2_Ind=0, out1_Ind;
  SQLBindCol(hstmt,1,SQL_C_CHAR,out1,(SDWORD)sizeof(out1),&out1_Ind);
  SQLBindParameter(hstmt,1,SQL_PARAM_INPUT,SQL_C_LONG,SQL_INTEGER,0,0,
                   &in1,0,&in1_Ind);
  SQLBindParameter(hstmt,2,SQL_PARAM_INPUT,SQL_C_LONG,SQL_INTEGER,0,0,
                   &in2,0,&in2_Ind);
  SQLPrepare(hstmt,(UCHAR *)statement,SQL_NTS);

  in2=(Long_t)pVersId;
  Bool_t allFound=kTRUE;
  initModules->Reset();
  RETCODE rc;
  for(Int_t i=0;i<6;i++) {
    for(Int_t j=0;j<4;j++) {
      HModGeomPar* pMod=pPar->getModule(i,j);
      Int_t pos=i*4+j;
      if (set[pos] && pMod) {
        in1=(Long_t)(id[i][j]);
        if (in1>0) {
          rc=SQLExecute(hstmt);
          if (rc!=SQL_SUCCESS && rc!=SQL_SUCCESS_WITH_INFO) {
             pConn->showErrorMessage(
                   "HMdcParOraIo::readModGeomNames(...): statement error");
             SQLFreeStmt(hstmt,SQL_CLOSE);
             return kFALSE;
          }
          while ((rc=SQLFetch(hstmt))!=SQL_NO_DATA_FOUND) {
            initModules->AddAt(pos+1,pos);
            pMod->SetName(out1);
            out1[4]='1'; // reference module in sector 1
	                 // not yet introduced in database
            pMod->setRefName(out1);
            Int_t mr=pPar->getModNumInMod(out1);
            HGeomCompositeVolume* refMod=pPar->getRefVolume(mr);
            if (refMod==0) {
              refMod=new HGeomCompositeVolume(pPar->getNumComponents());
	      //              refMod->SetName(ref);
              refMod->SetName(out1);
              pPar->addRefVolume(refMod,mr);
            }
            pMod->setVolume(refMod);
          }
        } else allFound=kFALSE;
      }
    }
  }
  SQLFreeStmt(hstmt,SQL_CLOSE);
  return allFound;
}


Bool_t HMdcParOraIo::readLayerGeomNames(HMdcGeomPar* pPar,Int_t* set) {
  // reads the names of all layers in the geometry tables

  void* hstmt=pConn->getHstmt();

  char statement[]=
    "select obj_name_short,layer_nr "
    "from mdc_ana.mdc_obj,mdc_ana.mdc_type,mdc_ana.layer_type "
    "where mdc_obj.mdc_obj_id=? "
    "and ((mdc_type.mdc_type_id=mdc_obj.mdc_type_id) "
    "and (layer_type.mdc_type_id=mdc_type.mdc_type_id))"; 

  char out1[10];
  Long_t out2;
  Long_t in1;
  SDWORD in1_Ind=0, out1_Ind, out2_Ind;
  SQLBindCol(hstmt,1,SQL_C_CHAR,out1,(SDWORD)sizeof(out1),&out1_Ind);
  SQLBindCol(hstmt,2,SQL_C_LONG,&out2,0,&out2_Ind);
  SQLBindParameter(hstmt,1,SQL_PARAM_INPUT,SQL_C_LONG,SQL_INTEGER,0,0,
                   &in1,0,&in1_Ind);

  SQLPrepare(hstmt,(UCHAR *)statement,SQL_NTS);
  RETCODE rc;
  for(Int_t i=0;i<6;i++) {
    for(Int_t j=0;j<4;j++) {
      HModGeomPar* pMod=pPar->getModule(i,j);
      Int_t pos=i*4+j;
      if (set[pos] && pMod) {
        HGeomCompositeVolume* pRefMod=pMod->getRefVolume();
        in1=(Long_t)(id[i][j]);
        if (in1>0) {
          rc=SQLExecute(hstmt);
          if (rc!=SQL_SUCCESS && rc!=SQL_SUCCESS_WITH_INFO) {
             pConn->showErrorMessage(
                   "HMdcParOraIo::getModuleIndices(...): statement error");
             SQLFreeStmt(hstmt,SQL_CLOSE);
             return kFALSE;
          }
          while ((rc=SQLFetch(hstmt))!=SQL_NO_DATA_FOUND) {
            HGeomVolume* volu=pRefMod->getComponent((Int_t)(out2-1));
            volu->SetName(out1);
          }
        }
      }
    }
  }
  SQLFreeStmt(hstmt,SQL_CLOSE);
  return kTRUE;
}


Long_t HMdcParOraIo::readGeomIndex(const Text_t* in1) {
  void* hstmt=pConn->getHstmt();
  char statement[]=
    "select obj_index from hades_geom.geom_objects "
    "where obj_name_short=?";
  Long_t out1;
  SDWORD in1_Ind=SQL_NTS, out1_Ind;
  RETCODE rc;
  SQLBindCol(hstmt,1,SQL_C_LONG,&out1,0,&out1_Ind);
  SQLBindParameter(hstmt,1,SQL_PARAM_INPUT,SQL_C_CHAR,SQL_VARCHAR,
         (SDWORD)sizeof(in1),0,(UCHAR *)in1,0,&in1_Ind);
  rc=SQLExecDirect(hstmt,(UCHAR *)statement,SQL_NTS);
  if (rc!=SQL_SUCCESS && rc!=SQL_SUCCESS_WITH_INFO) {
    pConn->showErrorMessage(
           "HMdcParOraIo::getGeomIndex(Text_t*): statement error");
    SQLFreeStmt(hstmt,SQL_CLOSE);
    return -1L;
  }
  rc=SQLFetch(hstmt);
  SQLFreeStmt(hstmt,SQL_CLOSE);
  return out1;
}


Bool_t HMdcParOraIo::readGeomShape(HGeomVolume* volu, Long_t in1,  Long_t in2) {
  // reads the GEANT shape of a volume

  void* hstmt=pConn->getHstmt();
  char statement[]=
    "select geant_shape "
    "from (select obj_index as oi, max(vers_index) as vi "
          "from hades_geom.geom_properties "
          "where obj_index=? and vers_index<=? "
          "group by obj_index"
         "),hades_geom.geom_properties "
    "where geom_properties.obj_index=oi and geom_properties.vers_index=vi";

  char out1[10];
  SDWORD in1_Ind=0, in2_Ind=0, out1_Ind;
  RETCODE rc;
  SQLBindCol(hstmt,1,SQL_C_CHAR,out1,(SDWORD)sizeof(out1),&out1_Ind);
  SQLBindParameter(hstmt,1,SQL_PARAM_INPUT,SQL_C_LONG,SQL_INTEGER,0,0,
                   &in1,0,&in1_Ind);
  SQLBindParameter(hstmt,2,SQL_PARAM_INPUT,SQL_C_LONG,SQL_INTEGER,0,0,
                   &in2,0,&in2_Ind);
  rc=SQLExecDirect(hstmt,(UCHAR *)statement,SQL_NTS);
  if (rc!=SQL_SUCCESS && rc!=SQL_SUCCESS_WITH_INFO) {
    pConn->showErrorMessage(
        "HMdcParOraIo::getGeomShape(...): statement error");
    SQLFreeStmt(hstmt,SQL_CLOSE);
    return kFALSE;
  }
  while ((rc=SQLFetch(hstmt))!=SQL_NO_DATA_FOUND) {
    volu->setShape(out1);
  }
  SQLFreeStmt(hstmt,SQL_CLOSE);
  return kTRUE;
} 


Int_t HMdcParOraIo::readGeomPoints(HGeomVolume* volu, Long_t in1, Long_t in2) {
  // reads the points of a volume
  // returns the number of points of a volume
  void* hstmt=pConn->getHstmt();
  char statement[]=
    "select np,p3d_corner,p3d_x,p3d_y,p3d_z "
    "from (select obj_index as oi,max(vers_index) as vi,max(p3d_corner) as np "
          "from hades_geom.geom_3dpoints "
          "where obj_index=? and vers_index<=? "
          "group by obj_index"
         "),hades_geom.geom_3dpoints "
    "where geom_3dpoints.obj_index=oi and geom_3dpoints.vers_index=vi "
          "and p3d_corner>0";

  Long_t out1, out2;
  Double_t out3, out4, out5;
  SDWORD in1_Ind=0, in2_Ind=0, out1_Ind, out2_Ind, out3_Ind, out4_Ind, out5_Ind;
  RETCODE rc;

  SQLBindCol(hstmt,1,SQL_C_LONG,&out1,0,&out1_Ind);
  SQLBindCol(hstmt,2,SQL_C_LONG,&out2,0,&out2_Ind);
  SQLBindCol(hstmt,3,SQL_C_DOUBLE,&out3,0,&out3_Ind);
  SQLBindCol(hstmt,4,SQL_C_DOUBLE,&out4,0,&out4_Ind);
  SQLBindCol(hstmt,5,SQL_C_DOUBLE,&out5,0,&out5_Ind);
  SQLBindParameter(hstmt,1,SQL_PARAM_INPUT,SQL_C_LONG,SQL_INTEGER,0,0,
                   &in1,0,&in1_Ind);
  SQLBindParameter(hstmt,2,SQL_PARAM_INPUT,SQL_C_LONG,SQL_INTEGER,0,0,
                   &in2,0,&in2_Ind);
  rc=SQLExecDirect(hstmt,(UCHAR *)statement,SQL_NTS);
  if (rc!=SQL_SUCCESS && rc!=SQL_SUCCESS_WITH_INFO) {
    pConn->showErrorMessage(
          "HMdcParOraIo::getGeomPoints(...): statement error");
    SQLFreeStmt(hstmt,SQL_CLOSE);
    return 0;
  }
  Int_t nPoints=0;
  Bool_t first=kTRUE;
  while ((rc=SQLFetch(hstmt))!=SQL_NO_DATA_FOUND) {
    if (first) {
      nPoints=(Int_t)out1;
      volu->createPoints(nPoints);
      first=kFALSE;
    }
    volu->setPoint(
      ((Int_t)(out2-1)),(Double_t)out3,(Double_t)out4,(Double_t)out5);
  }
  if (nPoints==0) {
    pConn->showErrorMessage(
        "HMdcParOraIo::getGeomPoints(...): no data found");
    SQLFreeStmt(hstmt,SQL_CLOSE);
    return 0;
  }
  SQLFreeStmt(hstmt,SQL_CLOSE);
  return nPoints;
}


Bool_t HMdcParOraIo::readGeomTransform(HGeomTransform* transform,
                                 Text_t* mother, Long_t in1, Long_t in2) {
  void* hstmt=pConn->getHstmt();
  char statement[]=
    "select trans_t1,trans_t2,trans_t3,"
           "trans_r11,trans_r12,trans_r13,"
           "trans_r21,trans_r22,trans_r23,"
           "trans_r31,trans_r32,trans_r33,"
           "ref_obj_name_short "
    "from (select obj_index as oi, max(vers_index) as vi "
          "from hades_geom.geom_transformations "
          "where obj_index=? and vers_index<=? "
          "group by obj_index"
         "),hades_geom.geom_transformations "
    "where geom_transformations.obj_index=oi "
          "and geom_transformations.vers_index=vi";

  Double_t out1, out2, out3,
           out4, out5, out6, out7, out8, out9, out10, out11, out12;
  char out13[10];
  SDWORD in1_Ind=0, in2_Ind=0, out1_Ind, out2_Ind, out3_Ind,
         out4_Ind, out5_Ind, out6_Ind, out7_Ind, out8_Ind, out9_Ind,
         out10_Ind, out11_Ind, out12_Ind, out13_Ind;
  RETCODE rc;
  SQLBindCol(hstmt,1,SQL_C_DOUBLE,&out1,0,&out1_Ind);
  SQLBindCol(hstmt,2,SQL_C_DOUBLE,&out2,0,&out2_Ind);
  SQLBindCol(hstmt,3,SQL_C_DOUBLE,&out3,0,&out3_Ind);
  SQLBindCol(hstmt,4,SQL_C_DOUBLE,&out4,0,&out4_Ind);
  SQLBindCol(hstmt,5,SQL_C_DOUBLE,&out5,0,&out5_Ind);
  SQLBindCol(hstmt,6,SQL_C_DOUBLE,&out6,0,&out6_Ind);
  SQLBindCol(hstmt,7,SQL_C_DOUBLE,&out7,0,&out7_Ind);
  SQLBindCol(hstmt,8,SQL_C_DOUBLE,&out8,0,&out8_Ind);
  SQLBindCol(hstmt,9,SQL_C_DOUBLE,&out9,0,&out9_Ind);
  SQLBindCol(hstmt,10,SQL_C_DOUBLE,&out10,0,&out10_Ind);
  SQLBindCol(hstmt,11,SQL_C_DOUBLE,&out11,0,&out11_Ind);
  SQLBindCol(hstmt,12,SQL_C_DOUBLE,&out12,0,&out12_Ind);
  SQLBindCol(hstmt,13,SQL_C_CHAR,out13,(SDWORD)sizeof(out13),&out13_Ind);
  SQLBindParameter(hstmt,1,SQL_PARAM_INPUT,SQL_C_LONG,SQL_INTEGER,0,0,
                   &in1,0,&in1_Ind);
  SQLBindParameter(hstmt,2,SQL_PARAM_INPUT,SQL_C_LONG,SQL_INTEGER,0,0,
                   &in2,0,&in2_Ind);

  rc=SQLExecDirect(hstmt,(UCHAR *)statement,SQL_NTS);
  if (rc!=SQL_SUCCESS && rc!=SQL_SUCCESS_WITH_INFO) {
    pConn->showErrorMessage(
         "HMdcParOraIo::getGeomTransform(...): statement error");
    SQLFreeStmt(hstmt,SQL_CLOSE);
    return kFALSE;
  }
  while ((rc=SQLFetch(hstmt))!=SQL_NO_DATA_FOUND) {
    strcpy(mother,out13);
    Double_t t[9];
    t[0]=(Double_t)out1;
    t[1]=(Double_t)out2;
    t[2]=(Double_t)out3;
    transform->setTransVector(t);
    Double_t r[9];
    r[0]=(Double_t)out4;
    r[1]=(Double_t)out5;
    r[2]=(Double_t)out6;
    r[3]=(Double_t)out7;
    r[4]=(Double_t)out8;
    r[5]=(Double_t)out9;
    r[6]=(Double_t)out10;
    r[7]=(Double_t)out11;
    r[8]=(Double_t)out12;
    transform->setRotMatrix(r);
  }
  SQLFreeStmt(hstmt,SQL_CLOSE);
  if (mother[0]=='\0') {
    pConn->showErrorMessage(
          "HMdcParOraIo::getGeomTransform(...): no data found");
    return kFALSE;
  }
  return kTRUE;
}


Bool_t HMdcParOraIo::write(HMdcDetector*) {
  // nothing implemented!
  return 0;
}


Int_t HMdcParOraIo::write(HParSet* pPar) {
  // calls special update-function for each container type, but only for
  // these containers which are allowed to change the information via
  // the analysis (e.g. MdcCalPar)
  // does nothing for the other containers
  if (strcmp(pPar->GetName(),"MdcCalPar")==0) return update(((HMdcCalPar*) pPar));
  return 0;
}


Int_t HMdcParOraIo::update(HMdcCalPar*) {
  // writes MdcPar container to Oracle
  // nothing implemented yet!
  cout<<"HMdcParOraIo::write(HMdcCalPar*) not yet implemented"<<endl;
  return -1;
}


void HMdcParOraIo::printIds() {
  // prints the id numbers of all modules
  for(Int_t i=0;i<6;i++) {
    cout<<"sector: "<<i+1<<"   mdc ids:";
    for(Int_t j=0;j<4;j++) { cout<<" "<<id[i][j]; }
    cout<<'\n';
  }
  cout<<'\n';
}


Int_t HMdcParOraIo::readIds() {
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
    "select oi,vi,sector,plane "
    "from (select mdc_placement.mdc_obj_id as oi,"
                "max(mdc_placement.mdc_p_vers_id) as vi "
          "from mdc_ana.mdc_placement,mdc_ana.mdc_pos "
          "where mdc_placement.mdc_p_vers_id<="
             "(select version_set.mdc_p_vers_id "
              "from mdc_ana.version_set,mdc_ana.run "
              "where run_id=? "
              "and version_set.version_set_id=run.version_set_id) "
              "group by mdc_obj_id"
         "),mdc_ana.mdc_placement,mdc_ana.mdc_pos "
    "where mdc_placement.mdc_obj_id=oi and mdc_placement.mdc_p_vers_id=vi "
    "and mdc_placement.mdc_pos_id=mdc_pos.mdc_pos_id";

  Long_t objId, sec, mod, vers;
  SDWORD runId_Ind=0, objId_Ind, sec_Ind, mod_Ind, vers_Ind;
  RETCODE rc;

  SQLBindCol(hstmt,1,SQL_C_LONG,&objId,0,&objId_Ind);
  SQLBindCol(hstmt,2,SQL_C_LONG,&vers,0,&vers_Ind);
  SQLBindCol(hstmt,3,SQL_C_LONG,&sec,0,&sec_Ind);
  SQLBindCol(hstmt,4,SQL_C_LONG,&mod,0,&mod_Ind);
  SQLBindParameter(hstmt,1,SQL_PARAM_INPUT,SQL_C_LONG,SQL_INTEGER,0,0,
                   &runId,0,&runId_Ind);

  SQLPrepare(hstmt,(UCHAR *)statement,SQL_NTS);

  rc=SQLExecute(hstmt);
  if (rc!=SQL_SUCCESS && rc!=SQL_SUCCESS_WITH_INFO) {
    pConn->showErrorMessage("HMdcParOraIo::readIds: statement error");
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


Int_t HMdcParOraIo::getCalparVersion(Long_t runId) {
  // reads the newest version number for the calibration parameters
  // valid for the current event file

  char statement[]=
    "select to_number(module_version_set_id) "
    "from mdc_ana.cal_version_set,"
          "(select max(version) as vi, run_id as ri "
           "from mdc_ana.cal_version_set "
           "where run_id=(select max(run_id) "
                         "from mdc_ana.cal_version_set where run_id<=?) "
           "group by run_id)"
    "where run_id=ri and version=vi";

  return ((Int_t)readNumber(statement,runId));
}


Int_t HMdcParOraIo::getLookupVersion(Long_t runId) {
  // reads version_set_id  for a run given by run_id
  // if the version_set_id changes the lookup container must be
  // reinitialized
  char statement[]=
    "select to_number(version_set_id) from mdc_ana.run where run_id=?";
  return ((Int_t)readNumber(statement,runId));
}


Int_t HMdcParOraIo::getDigiVersion(Long_t runId) {
  // reads the newest version number for the digitization parameters
  // valid for the current event file

  char statement[]=
    "select cal_digi_vers_id "
    "from mdc_ana.cal_version_set,"
          "(select max(version) as vi, run_id as ri "
           "from mdc_ana.cal_version_set "
           "where run_id=(select max(run_id) "
                         "from mdc_ana.cal_version_set where run_id<=?) "
           "group by run_id)"
    "where run_id=ri and version=vi";

  return ((Int_t)readNumber(statement,runId));
}


Int_t HMdcParOraIo::getGeomVersion(Long_t runId) {
  // reads the newest version number for the digitization parameters
  // valid for the current event file

  char statement[]=
    "select geom_vers_id "
    "from mdc_ana.version_set,mdc_ana.run "
    "where run_id=? and version_set.version_set_id=run.version_set_id";
 
  return ((Int_t)readNumber(statement,runId));
}


Long_t HMdcParOraIo::readNumber(char* statement, Long_t in1) {
  // reads a Long_t number
  // takes a SQL statement and one Long_t number as input parameter
  // is used to read versions

  void* hstmt=pConn->getHstmt();
  if (!hstmt) {
    cerr<<"no connection to database";
    return kFALSE;
  }
  Long_t out1=-1L;
  SDWORD in1_Ind=0, out1_Ind;
  RETCODE rc;
  SQLBindCol(hstmt,1,SQL_C_LONG,&out1,0,&out1_Ind);
  SQLBindParameter(hstmt,1,SQL_PARAM_INPUT,SQL_C_LONG,SQL_INTEGER,0,0,
                   &in1,0,&in1_Ind);
  rc=SQLExecDirect(hstmt,(UCHAR *)statement,SQL_NTS);
  if (rc!=SQL_SUCCESS && rc!=SQL_SUCCESS_WITH_INFO) {
    pConn->showErrorMessage(
             "HMdcParOraIo::getNumber(char*,Long_t): statement error");
    SQLFreeStmt(hstmt,SQL_CLOSE);
    return -1L;
  }
  Int_t n=0;
  while ((rc=SQLFetch(hstmt))!=SQL_NO_DATA_FOUND) {
     n++;
  }
  if (n>1) {
    char buf[120];
    sprintf(buf,"HMdcParOraIo::getNumber(char*,Long_t): too many data: %3i",n);
    pConn->showErrorMessage(buf);
    return -1L;
  } 
  SQLFreeStmt(hstmt,SQL_CLOSE);
  return out1;
}


void HMdcParOraIo::printInfo(Text_t* msg) {
  // prints the modules initialized from Oracle
  // will later go to the log file
  Bool_t first=kTRUE;
  for(Int_t i=0;i<24;i++) {
    if (initModules->At(i)) {
      if (first) {
        cout<<msg;
        first=kFALSE;
      }
      cout<<(initModules->At(i)-1)<<" ";
    }
  }
  cout<<'\n';
}


Bool_t HMdcParOraIo::execProcRuns(Long_t runId) {
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
    pConn->showErrorMessage("HMdcParOraIo::execProcRuns: statement error");
    r=kFALSE;
  }
  else viewsRunId=runId;
  SQLFreeStmt(hstmt,SQL_CLOSE);
  return r;
}
