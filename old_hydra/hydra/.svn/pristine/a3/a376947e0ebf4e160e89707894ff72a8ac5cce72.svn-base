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
#include "hmdcdigitpar.h"
#include "hmdcdigitgeompar.h"
#include "hmdcmboreadout.h"
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
  const Text_t* name=pPar->GetName();
  if (strcmp(name,"MdcRawStruct")==0) return read(((HMdcRawStruct*)pPar),set);
  if (strcmp(name,"MdcGeomStruct")==0) return read(((HMdcGeomStruct*)pPar),set);
  if (strcmp(name,"MdcCalPar")==0) return read(((HMdcCalPar*)pPar),set);
  if (strcmp(name,"MdcLookupGeom")==0) return read(((HMdcLookupGeom*)pPar),set);
  if (strcmp(name,"MdcDigitPar")==0) return read(((HMdcDigitPar*)pPar),set);
  if (strcmp(name,"MdcDigitGeomPar")==0) return read(((HMdcDigitGeomPar*)pPar),set);
  if (strcmp(name,"MdcMboReadout")==0) return read(((HMdcMboReadout*)pPar),set);
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
  "select mbo_pos_id,n_channel_max*n_pos_max*n_dbo_pos_max,"
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
                  "HOraMdc::read(HMdcRawStruct*,Int_t*): statement error");
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

  if (execProcRuns(runId)==kFALSE) return kFALSE;

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
          rc=SQLExecute(hstmt);
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
              mo=atoi(mod[count].GetName());
	      if (mo!=ml) {
                char buf[80];
                sprintf(buf,"HOraMdc::readLookup: motherboard %2i not found",mo);
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


Bool_t HOraMdc::read(HMdcDigitPar* pPar,Int_t* set) {
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
  Float_t driftVel, mdcEff, sP1, sP2, sP3;
  SDWORD objId_Ind=0, vers_Ind=0;
  SDWORD layNum_Ind, driftVel_Ind, mdcEff_Ind, sP1_Ind, sP2_Ind, sP3_Ind;
  RETCODE rc;
  Bool_t allFound=kTRUE;

  SQLBindCol(hstmt,1,SQL_C_LONG,&layNum,0,&layNum_Ind);
  SQLBindCol(hstmt,2,SQL_C_FLOAT,&driftVel,0,&driftVel_Ind);
  SQLBindCol(hstmt,3,SQL_C_FLOAT,&mdcEff,0,&mdcEff_Ind);
  SQLBindCol(hstmt,4,SQL_C_FLOAT,&sP1,0,&sP1_Ind);
  SQLBindCol(hstmt,5,SQL_C_FLOAT,&sP2,0,&sP2_Ind);
  SQLBindCol(hstmt,6,SQL_C_FLOAT,&sP3,0,&sP3_Ind);

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
                  "HOraMdc::read(HMdcDigitPar*,Int_t*): statement error");
            SQLFreeStmt(hstmt,SQL_CLOSE);
            return kFALSE;
          }
          n=0;
          while ((rc=SQLFetch(hstmt))!=SQL_NO_DATA_FOUND) {
            mod[((Int_t)(layNum-1))].fill(driftVel, mdcEff, sP1, sP2, sP3);
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


Bool_t HOraMdc::read(HMdcDigitGeomPar* pPar,Int_t* set) {
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
  HMdcDigitGeomPar& mdc=*pPar;
  for(Int_t i=0;i<mdc.getSize();i++) {
    HMdcDigitGeomParSec& sec=mdc[i];
    for(Int_t j=0;j<sec.getSize();j++) {
      HMdcDigitGeomParMod& mod=sec[j];
      objId=(Long_t)(id[i][j]);
      pos=i*4+j;
      if (set[pos]) {
        if (objId>0) {
          rc=SQLExecute(hstmt);
          if (rc!=SQL_SUCCESS && rc!=SQL_SUCCESS_WITH_INFO) {
            pConn->showErrorMessage(
                  "HOraMdc::read(HMdcDigitGeomPar*,Int_t*): statement error");
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
  printInfo("MdcDigitGeomPar: module(s) initialized from Oracle: ");
  return allFound;
}

Bool_t HOraMdc::read(HMdcMboReadout*, Int_t*) {
  cout<<"read(HMdcMboReadout* pPar, Int_t* set) not yet implemented"<<endl;
  return kFALSE;
}


Bool_t HOraMdc::write(HMdcDetector*) {
  // nothing implemented!
  return 0;
}

Int_t HOraMdc::write(HParSet* pPar) {
  // calls special update-function for each container type, but only for
  // these containers which are allowed to change the information via
  // the analysis (e.g. MdcCalPar)
  // does nothing for the other containers
  if (strcmp(pPar->GetName(),"MdcCalPar")==0) return update(((HMdcCalPar*) pPar));
  return 0;
}


Int_t HOraMdc::update(HMdcCalPar*) {
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


Int_t HOraMdc::getLookupVersion(Long_t runId) {
  // reads version_set_id  for a run given by run_id
  // if the version_set_id changes the lookup container must be
  // reinitialized
  char statement[]=
    "select to_number(version_set_id) from mdc_ana.run where run_id=?";
  return ((Int_t)readNumber(statement,runId));
}


Int_t HOraMdc::getDigiVersion(Long_t runId) {
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


Long_t HOraMdc::readNumber(char* statement, Long_t in1) {
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
    pConn->showErrorMessage("HOraMdc::getNumber(char*,Long_t): statement error");
    SQLFreeStmt(hstmt,SQL_CLOSE);
    return -1L;
  }
  Int_t n=0;
  while ((rc=SQLFetch(hstmt))!=SQL_NO_DATA_FOUND) {
     n++;
  }
  if (n>1) {
    char buf[120];
    sprintf(buf,"HOraMdc::getNumber(char*,Long_t): too many data: %3i",n);
    pConn->showErrorMessage(buf);
    return -1L;
  } 
  SQLFreeStmt(hstmt,SQL_CLOSE);
  return out1;
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
      cout<<(initModules->At(i)-1)<<" ";
    }
  }
  cout<<'\n';
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
