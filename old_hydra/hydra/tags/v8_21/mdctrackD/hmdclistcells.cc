using namespace std;
#include <iostream>
#include <iomanip>
//#include "hades.h"
#include "hevent.h"
#include "hmdclistcells.h"
#include "hiterator.h"
#include "hcategory.h"
#include "hmdcdef.h"
#include "hmdccal1sim.h"
#include "hmdctimecut.h"
#include "hmdcgetcontainers.h"
#include "hmdclistgroupcells.h"


//*-- Author : V. Pechenov
//*-- Modified : 25/06/2004 by V.Pechenov
//*-- Modified : 22/12/2006 by V.Pechenov

//_HADES_CLASS_DESCRIPTION 
///////////////////////////////////////////////////////////////////////
//  HMdcLayListCells - list of fired wires in one layer.
//                     Can keep also drift times 
//                     and geant track numbers.
//
//  HMdcModListCells - list of fired wires in one MDC 
//                     (6 HMdcLayListCells objects)
//
//  HMdcSecListCells - list of fired wires in one sector 
//                     (4 HMdcModListCells objects)
//
//  HMdcEvntListCells - list of fired wires in one sector 
//                      (6 HMdcSecListCells objects)
//
//  If you want to create an own object of HMdcEvntListCells class
//  use normal constructor.
// 
//  If you want to work with global object of HMdcEvntListCells class 
//  use static functions:
//    HMdcEvntListCells::getObject() - return pointer to the clobal 
//        object. If clobal object don't exist function will create it.
//
//    HMdcEvntListCells::getExObject() - return pointer to the existing 
//        clobal object. If clobal object don't exist - return 0.
//
//    HMdcEvntListCells::deleteCont() - deleting of global 
//        object. Can be colled any number times.
//
///////////////////////////////////////////////////////////////////////

void HMdcLayListCells::init(void) {
  arrEnd=arr+arrSize-1;
  drTime1     = 0;
  drTime2     = 0;
  tof1        = 0;
  tof2        = 0;
  nClust      = 0;
  nRClust     = 0;
  gnTrackNum1 = 0;
  gnTrackNum2 = 0;
  clear();
}

HMdcLayListCells::~HMdcLayListCells(void) {
  if(drTime1)     {delete [] drTime1;     drTime1=0;}
  if(drTime2)     {delete [] drTime2;     drTime2=0;}
  if(tof1)        {delete [] tof1;        tof1=0;}
  if(tof2)        {delete [] tof2;        tof2=0;}
  if(nClust)      {delete [] nClust;      nClust=0;}
  if(nRClust)     {delete [] nRClust;     nRClust=0;}
  if(gnTrackNum1) {delete [] gnTrackNum1; gnTrackNum1=0;}
  if(gnTrackNum2) {delete [] gnTrackNum2; gnTrackNum2=0;}
}

void HMdcLayListCells::copyListOfCells(HMdcLayListCells& lst) {
  // Copy data from "lst" to "this".
  memcpy(arr,lst.arr,arrSize);
  if(lst.drTime1     && drTime1==0    ) drTime1     = new Float_t [maxCells];
  if(lst.drTime2     && drTime2==0    ) drTime2     = new Float_t [maxCells];
  if(lst.tof1        && tof1==0       ) tof1        = new Float_t [maxCells];
  if(lst.tof2        && tof2==0       ) tof2        = new Float_t [maxCells];
  if(lst.nClust      && nClust==0     ) nClust      = new UChar_t [maxCells];
  if(lst.nRClust     && nRClust==0    ) nRClust     = new UChar_t [maxCells];
  if(lst.gnTrackNum1 && gnTrackNum1==0) gnTrackNum1 = new Int_t [maxCells];
  if(lst.gnTrackNum2 && gnTrackNum2==0) gnTrackNum2 = new Int_t [maxCells];
  if(drTime1)     memcpy(drTime1,     lst.drTime1,     maxCells*4); //Float_t
  if(drTime2)     memcpy(drTime2,     lst.drTime2,     maxCells*4); //Float_t
  if(tof1)        memcpy(tof1,        lst.tof1,        maxCells*4); //Float_t
  if(tof2)        memcpy(tof2,        lst.tof2,        maxCells*4); //Float_t
  if(nClust)      memcpy(nClust,      lst.nClust,      maxCells);   //UChar_t
  if(nRClust)     memcpy(nRClust,     lst.nRClust,     maxCells);   //UChar_t
  if(gnTrackNum1) memcpy(gnTrackNum1, lst.gnTrackNum1, maxCells*4); //Int_t
  if(gnTrackNum2) memcpy(gnTrackNum2, lst.gnTrackNum2, maxCells*4); //Int_t
}

void HMdcLayListCells::clearClustCounter(void) {
  if(nClust)  memset(nClust,0,maxCells);
  if(nRClust) memset(nRClust,0,maxCells);
}

Bool_t HMdcLayListCells::addCell(Int_t c,UChar_t tm,Float_t dt1,Float_t dt2) {
  // tm = 1  - in HMdcCal1 object time1 valid only
  // tm = 2  - in HMdcCal1 object time2 valid only
  // tm = 3  - in HMdcCal1 object time1 and time2 valid
  if(!boundsOk(c) || tm==0 || tm>3) return kFALSE;
  HMdcTBArray::set(arr,c,tm);
  if(tm&1) {
    if(drTime1==0) drTime1 =new Float_t [maxCells];
    drTime1[c]=dt1;
  }
  if(tm&2) {
    if(drTime2==0) drTime2 =new Float_t [maxCells];
    drTime2[c]=dt2;
  }
  return kTRUE;
}

Bool_t HMdcLayListCells::addCellSim(Int_t c,UChar_t tm,Float_t dt1,Float_t dt2,
        Int_t tr1,Int_t tr2,Float_t tf1,Float_t tf2) {
  // tm = 1  - in HMdcCal1 object time1 valid only
  // tm = 2  - in HMdcCal1 object time2 valid only
  // tm = 3  - in HMdcCal1 object time1 and time2 valid
  if(!boundsOk(c) || tm==0 || tm>3) return kFALSE;
  HMdcTBArray::set(arr,c,tm);
  if(tm&1) {
    if(drTime1==0) drTime1 = new Float_t [maxCells];
    drTime1[c]=dt1;
    if(gnTrackNum1==0) gnTrackNum1 = new Int_t [maxCells];
    gnTrackNum1[c]=tr1;
    if(tof1==0) tof1 = new Float_t [maxCells];
    tof1[c]=tf1;
  }
  if(tm&2) {
    if(drTime2==0) drTime2 =new Float_t [maxCells];
    drTime2[c]=dt2;
    if(gnTrackNum2==0) gnTrackNum2 = new Int_t [maxCells];
    gnTrackNum2[c]=tr2;
    if(tof2==0) tof2 = new Float_t [maxCells];
    tof2[c]=tf2;
  }
  return kTRUE;
}

UChar_t HMdcLayListCells::nextCell(Int_t& c,Float_t& dt1,Float_t& dt2) const {
  UChar_t tm=HMdcTBArray::next2Bits(arr,arrEnd,c);
  dt1 = (tm&1 && drTime1) ? drTime1[c]:-999.F;
  dt2 = (tm&2 && drTime2) ? drTime2[c]:-999.F;
  return tm;
}

UChar_t HMdcLayListCells::nextCellSim(Int_t& c,Float_t& dt1,Float_t& dt2,
    Int_t& tr1,Int_t& tr2,Float_t& tf1,Float_t& tf2) const {
  UChar_t tm=HMdcTBArray::next2Bits(arr,arrEnd,c);
  if(tm&1) {
    dt1 = (drTime1)     ? drTime1[c]     : -999.F;
    tr1 = (gnTrackNum1) ? gnTrackNum1[c] : 0;
    tf1 = (tof1)        ? tof1[c]        : -99.F;
  } else {
    dt1 = -999.F;
    tr1 = 0;
    tf1 = -99.F;
  }
  if(tm&2) {
    dt2 = (drTime2)     ? drTime2[c]     : -999.F;
    tr2 = (gnTrackNum2) ? gnTrackNum2[c] : 0;
    tf2 = (tof2)        ? tof2[c]        : -99.F;
  } else {
    dt2 = -999.F;
    tr2 = 0;
    tf2 = -99.F;
  }
  return tm;
}

UChar_t HMdcLayListCells::nextCellSim(Int_t& c,
    Int_t& tr1,Int_t& tr2,Float_t& tf1,Float_t& tf2) const {
  UChar_t tm=HMdcTBArray::next2Bits(arr,arrEnd,c);
  if(tm&1) {
    tr1 = (gnTrackNum1) ? gnTrackNum1[c] : 0;
    tf1 = (tof1)        ? tof1[c]        : -99.F;
  } else {
    tr1 = 0;
    tf1 = -99.F;
  }
  if(tm&2) {
    tr2 = (gnTrackNum2) ? gnTrackNum2[c] : 0;
    tf2 = (tof2)        ? tof2[c]        : -99.F;
  } else {
    tr2 = 0;
    tf2 = -99.F;
  }
  return tm;
}
      
void HMdcLayListCells::print(void) const {
  printf("%i cells:",getNCells());
  Int_t cell=-1;
  while((cell=next(cell)) >= 0) {
    if(drTime1==0) printf(" %i(%i)",cell+1,getTime(cell));
    else printf(" %i/%.1f",cell+1,drTime1[cell]);
  }
  printf("\n");
}

Bool_t HMdcLayListCells::increaseClusCount(Int_t cell) {
  if(!boundsOk(cell)) return kFALSE;
  if(nClust==0) {
    nClust = new UChar_t [maxCells];
    memset(nClust,0,maxCells);
  } else if(nClust[cell]==255) {
    Warning("increaseClusCount","Clusters counter for cell %i overloaded\n",cell);
    return kFALSE;
  }
  nClust[cell]++;
  return kTRUE;
}

Bool_t HMdcLayListCells::increaseRClusCount(Int_t cell) {
  if(!boundsOk(cell)) return kFALSE;
  if(nRClust==0) {
    nRClust = new UChar_t [maxCells];
    memset(nRClust,0,maxCells);
  } else if(nRClust[cell]==255) {
    Warning("increaseRClusCount","Clusters counter for cell %i overloaded\n",cell);
    return kFALSE;
  }
  nRClust[cell]++;
  return kTRUE;
}

Bool_t HMdcLayListCells::decreaseClusCount(Int_t cell) {
  if(!boundsOk(cell) || nClust==0) return kFALSE;
  if(nClust[cell]==0) {
    Warning("decreaseClusCount","Clusters counter for cell %i =0\n",cell);
    return kFALSE;
  }
  nClust[cell]--;
  return kTRUE;
}

Bool_t HMdcLayListCells::decreaseRClusCount(Int_t cell) {
  if(!boundsOk(cell) || nRClust==0) return kFALSE;
  if(nRClust[cell]==0) {
    Warning("decreaseRClusCount","Clusters counter for cell %i =0\n",cell);
    return kFALSE;
  }
  nRClust[cell]--;
  return kTRUE;
}

//---------------------------------------------------------------

void HMdcModListCells::print(void) const {
  for(Int_t l=0;l<6;l++) {
    if(list[l].getFirstCell()<0) continue;
    printf(" %i layer:",l+1);
    list[l].print();
  }
}

UChar_t HMdcModListCells::nextCell(Int_t& l,Int_t& c,
    Float_t& dt1,Float_t& dt2) const {
  if(l<0) {
    l=0;
    c=-1;
  }
  for(;l<6;l++) {
    UChar_t tm=list[l].nextCell(c,dt1,dt2);
    if(tm) return tm;
  }
  l=-1;
  return 0;
}

UChar_t HMdcModListCells::nextCellSim(Int_t& l,Int_t& c,
    Float_t& dt1,Float_t& dt2,
    Int_t& tr1,Int_t& tr2,Float_t& tf1,Float_t& tf2) const {
  if(l<0) {
    l=0;
    c=-1;
  }
  for(;l<6;l++) {
    UChar_t tm=list[l].nextCellSim(c,dt1,dt2,tr1,tr2,tf1,tf2);
    if(tm) return tm;
  }
  l=-1; 
  return 0;
}

UChar_t HMdcModListCells::nextCellSim(Int_t& l,Int_t& c,
    Int_t& tr1,Int_t& tr2,Float_t& tf1,Float_t& tf2) const {
  if(l<0) {
    l=0;
    c=-1;
  }
  for(;l<6;l++) {
    UChar_t tm=list[l].nextCellSim(c,tr1,tr2,tf1,tf2);
    if(tm) return tm;
  }
  l=-1; 
  return 0;
}

//---------------------------------------------------------------
    
void HMdcSecListCells::print(void) const {
  for(Int_t m=0;m<4;m++) {
    printf(" ---- %imod.:  %i cells in %i layers\n",
        m+1,list[m].getNCells(),list[m].getNLayers());
    list[m].print();
  }
}

UChar_t HMdcSecListCells::nextCell(Int_t& m, Int_t& l, Int_t& c,
    Float_t& dt1,Float_t& dt2) const {
  if(m<0) {
    m=l=0;
    c=-1;
  }
  for(;m<4;m++) {
    UChar_t tm=list[m].nextCell(l,c,dt1,dt2);
    if(tm) return tm;
  }
  m=-1;
  return 0;
}

UChar_t HMdcSecListCells::nextCellSim(Int_t& m,Int_t& l,Int_t& c,
    Float_t& dt1,Float_t& dt2,
    Int_t& tr1,Int_t& tr2,Float_t& tf1,Float_t& tf2) const {
  if(m<0) {
    m=l=0;
    c=-1;
  }
  for(;m<4;m++) {
    UChar_t tm=list[m].nextCellSim(l,c,dt1,dt2,tr1,tr2,tf1,tf2);
    if(tm) return tm;
  }
  m=-1;
  return 0;
}

UChar_t HMdcSecListCells::nextCellSim(Int_t& m,Int_t& l,Int_t& c,
    Int_t& tr1,Int_t& tr2,Float_t& tf1,Float_t& tf2) const {
  if(m<0) {
    m=l=0;
    c=-1;
  }
  for(;m<4;m++) {
    UChar_t tm=list[m].nextCellSim(l,c,tr1,tr2,tf1,tf2);
    if(tm) return tm;
  }
  m=-1;
  return 0;
}

void HMdcSecListCells::clearClustCounter(Int_t seg) {
  if(seg<0 || seg>1) return;
  Int_t m  = seg*2;
  Int_t ml = m+2;
  for(;m<ml;m++) list[m].clearClustCounter();
}

Bool_t HMdcSecListCells::addToClusCounters(Int_t seg,
     HMdcList12GroupCells& listCells) {
  if(seg<0 || seg>1) return kFALSE;
  Int_t layInd    = -1;
  Int_t cell      = -1;
  Bool_t exitFlag = kTRUE;
  while(listCells.getNextCell(layInd,cell)) {
    Int_t mod = layInd/6+seg*2;
    if(!list[mod].increaseClusCount(layInd%6,cell)) exitFlag = kFALSE;
  }
  return exitFlag;
}

Bool_t HMdcSecListCells::addToRClusCounters(Int_t seg,
     HMdcList12GroupCells& listCells) {
  if(seg<0 || seg>1) return kFALSE;
  Int_t layInd    = -1;
  Int_t cell      = -1;
  Bool_t exitFlag = kTRUE;
  while(listCells.getNextCell(layInd,cell)) {
    Int_t mod = layInd/6+seg*2;
    if(!list[mod].increaseRClusCount(layInd%6,cell)) exitFlag = kFALSE;
  }
  return exitFlag;
}

Bool_t HMdcSecListCells::subFrClusCounters(Int_t seg,
     HMdcList12GroupCells& listCells) {
  if(seg<0 || seg>1) return kFALSE;
  Int_t layInd    = -1;
  Int_t cell      = -1;
  Bool_t exitFlag = kTRUE;
  while(listCells.getNextCell(layInd,cell)) {
    Int_t mod = layInd/6+seg*2;
    if(!list[mod].decreaseClusCount(layInd%6,cell)) exitFlag = kFALSE;
  }
  return exitFlag;
}

Bool_t HMdcSecListCells::subFrRClusCounters(Int_t seg,
     HMdcList12GroupCells& listCells) {
  if(seg<0 || seg>1) return kFALSE;
  Int_t layInd    = -1;
  Int_t cell      = -1;
  Bool_t exitFlag = kTRUE;
  while(listCells.getNextCell(layInd,cell)) {
    Int_t mod = layInd/6+seg*2;
    if(!list[mod].decreaseRClusCount(layInd%6,cell)) exitFlag = kFALSE;
  }
  return exitFlag;
}

Int_t HMdcSecListCells::getNUniqueWires(Int_t seg,
     HMdcList12GroupCells& listCells) const {
  // call it if "listCells" was added to HMdcSecListCells
  if(seg<0 || seg>1) return kFALSE;
  Int_t layInd    = -1;
  Int_t cell      = -1;
  Int_t nUnWires  = 0;
  while(listCells.getNextCell(layInd,cell)) {
    Int_t mod = layInd/6+seg*2;
    UChar_t nCls = list[mod].getNClusters(layInd%6,cell);
    if(nCls==0) Warning("getNUniqueWires","Num.clusters == 0!");
    if(nCls==1) nUnWires++;
  }
  return nUnWires;
}

Int_t HMdcSecListCells::getNUniqueAndRWires(Int_t seg,
     HMdcList12GroupCells& listCells,Int_t& nWInReal) const {
  // call it if "listCells" was added to HMdcSecListCells
  if(seg<0 || seg>1) return kFALSE;
  Int_t layInd    = -1;
  Int_t cell      = -1;
  Int_t nUnWires  = 0;
  nWInReal        = 0;
  while(listCells.getNextCell(layInd,cell)) {
    Int_t mod = layInd/6+seg*2;
    UChar_t nCls = list[mod].getNClusters(layInd%6,cell);
    if(nCls==0) Warning("getNUniqueWires","Num.clusters == 0!");
    else if(nCls==1) nUnWires++;
    else if(list[mod].getNRClusters(layInd%6,cell) > 0) nWInReal++; // nCls>1 !
  }
  return nUnWires;
}

Int_t HMdcSecListCells::getNUniqueWires2(Int_t seg,
     HMdcList12GroupCells& listCells) const {
  // call it if "listCells" was NOT added to HMdcSecListCells
  if(seg<0 || seg>1) return kFALSE;
  Int_t layInd    = -1;
  Int_t cell      = -1;
  Int_t nUnWires  = 0;
  while(listCells.getNextCell(layInd,cell)) {
    Int_t mod = layInd/6+seg*2;
    UChar_t nCls = list[mod].getNClusters(layInd%6,cell);
    if(nCls==0) nUnWires++;
  }
  return nUnWires;
}

Int_t HMdcSecListCells::getNUniqueWires(Int_t seg,
     HMdcList12GroupCells& listCells, Int_t& nLays) const {
  // call it if "listCells" was added to HMdcSecListCells
  if(seg<0 || seg>1) return kFALSE;
  Int_t   layInd     = -1;
  Int_t   cell       = -1;
  Int_t   nUnWires   = 0;
  UChar_t layList[2] = {0,0};
  while(listCells.getNextCell(layInd,cell)) {
    Int_t modInd = layInd/6;
    Int_t layer  = layInd%6;
    Int_t mod    = modInd+seg*2;
    UChar_t nCls = list[mod].getNClusters(layer,cell);
    if(nCls==0) Warning("getNUniqueWires","Num.clusters == 0!");
    if(nCls==1) {
      nUnWires++;
      layList[modInd] |= 1<<layer;
    }
  }
//  nLays = HMdcBArray::getNSet(layList[0])+HMdcBArray::getNSet(layList[1]);
nLays = HMdcBArray::getNLayOrientation(layList[0] | layList[1]);
  return nUnWires;
}

Int_t HMdcSecListCells::getNUniqueWires2(Int_t seg,
     HMdcList12GroupCells& listCells, Int_t& nLays) const {
  // call it if "listCells" was NOT added to HMdcSecListCells
  if(seg<0 || seg>1) return kFALSE;
  Int_t   layInd     = -1;
  Int_t   cell       = -1;
  Int_t   nUnWires   = 0;
  UChar_t layList[2] = {0,0};
  while(listCells.getNextCell(layInd,cell)) {
    Int_t modInd = layInd/6;
    Int_t layer  = layInd%6;
    Int_t mod    = modInd+seg*2;
    UChar_t nCls = list[mod].getNClusters(layer,cell);
    if(nCls==0) {
      nUnWires++;
      layList[modInd] |= 1<<layer;
    }
  }
//  nLays = HMdcBArray::getNSet(layList[0])+HMdcBArray::getNSet(layList[1]);
nLays = HMdcBArray::getNLayOrientation(layList[0] | layList[1]);
  return nUnWires;
}

//---------------------------------------------------------------

HMdcEvntListCells* HMdcEvntListCells::pGlobalLCells=0;

HMdcEvntListCells* HMdcEvntListCells::getObject(Bool_t& isCreated) {
  // Return isCreated = kTRUE if new object was created
  if(pGlobalLCells==0) {
    pGlobalLCells = new HMdcEvntListCells();
    isCreated     = kTRUE;
  } else isCreated = kFALSE;
  return pGlobalLCells;
}

void HMdcEvntListCells::deleteCont(void) {
  if(pGlobalLCells) delete pGlobalLCells;
  pGlobalLCells = 0;
}

HMdcEvntListCells::HMdcEvntListCells(void) {
  clear();
  pMdcCal1Cat = 0;
  iterMdcCal1 = 0;
}

HMdcEvntListCells::~HMdcEvntListCells(void) {
  if(iterMdcCal1) delete iterMdcCal1;
  iterMdcCal1=0;
  if(this==pGlobalLCells) pGlobalLCells=0;
}
    
void HMdcEvntListCells::print(void) const {
  printf("==== Event: %i cells ===============================\n",getNCells());
  for(Int_t s=0;s<6;s++) {
    printf("%i sector:  %i cells in %i layers \n",
        s+1,list[s].getNCells(),list[s].getNLayers());
    if(list[s].getNCells()) list[s].print();
  }
}

UChar_t HMdcEvntListCells::nextCell(Int_t& s,Int_t& m,Int_t& l,Int_t& c,
        Float_t& dt1,Float_t& dt2) const {
  if(s<0) {
    s=m=l=0;
    c=-1;
  }
  for(;s<6;s++) {
    UChar_t tm=list[s].nextCell(m,l,c,dt1,dt2);
    if(tm) return tm;
  }
  return 0;
}

UChar_t HMdcEvntListCells::nextCellSim(Int_t& s,Int_t& m,Int_t& l,Int_t& c,
    Float_t& dt1,Float_t& dt2,
    Int_t& tr1,Int_t& tr2,Float_t& tf1,Float_t& tf2) const {
  if(s<0) {
    s=m=l=0;
    c=-1;
  }
  for(;s<6;s++) {
    UChar_t tm=list[s].nextCellSim(m,l,c,dt1,dt2,tr1,tr2,tf1,tf2);
    if(tm) return tm;
  }
  return 0;
}

UChar_t HMdcEvntListCells::nextCellSim(Int_t& s,Int_t& m,Int_t& l,Int_t& c,
    Int_t& tr1,Int_t& tr2,Float_t& tf1,Float_t& tf2) const {
  if(s<0) {
    s=m=l=0;
    c=-1;
  }
  for(;s<6;s++) {
    UChar_t tm=list[s].nextCellSim(m,l,c,tr1,tr2,tf1,tf2);
    if(tm) return tm;
  }
  return 0;
}

Bool_t HMdcEvntListCells::setMdcCal1Cat(void) {
  // use this function when you read tree without creating Hades object
  HCategory* cat = HMdcGetContainers::getObject()->getCatMdcCal1();
  if(pMdcCal1Cat && pMdcCal1Cat==cat) return kTRUE;
  pMdcCal1Cat=cat;
  if(pMdcCal1Cat==0) {
    Error("setMdcCal1Cat","Can't get catMdcCal1 category!");
    return kFALSE;
  }
  if(iterMdcCal1) delete iterMdcCal1;
  isGeant = pMdcCal1Cat->getClass()==HMdcCal1Sim::Class();
  iterMdcCal1 = (HIterator*)pMdcCal1Cat->MakeIterator("native");
  pDrTimeCut=HMdcTimeCut::getExObject();
  if(pDrTimeCut && pDrTimeCut->getIsUsed()) pDrTimeCut=0;
  return kTRUE;
}

Int_t HMdcEvntListCells::collectWires(Int_t sec) {
  //if sec >=0 && sec<6 - collect wires from one sector only
  if(sec>5) sec=-1;
  if(!setMdcCal1Cat()) return 0;
  clear();
  Int_t nwires=0;
  iterMdcCal1->Reset();
  if(isGeant) {
    HMdcCal1Sim* pMdcCal1;
    while((pMdcCal1=(HMdcCal1Sim*)iterMdcCal1->Next())) {
      Int_t nHits   = pMdcCal1->getNHits();
      if(nHits==0) continue;
      UChar_t times = (nHits==-2) ? 3:1;
      if((times&1) && pMdcCal1->getStatus1()<0) times &= 2;
      if((times&2) && pMdcCal1->getStatus2()<0) times &= 1;
      if(times==0) continue;
      Int_t s,m,l,c;
      pMdcCal1->getAddress(s,m,l,c);
      if(sec>=0 && sec!=s) continue;
      Float_t t1   = pMdcCal1->getTime1();
      Float_t t2   = pMdcCal1->getTime2();
      Int_t   tr1  = pMdcCal1->getNTrack1();
      Int_t   tr2  = pMdcCal1->getNTrack2();
      Float_t tf1 = pMdcCal1->getTof1();
      Float_t tf2 = pMdcCal1->getTof2();
      addCellSim(s,m,l,c,times,t1,t2,tr1,tr2,tf1,tf2);
      nwires++;
    }
  } else {
    HMdcCal1 * pMdcCal1;
    while((pMdcCal1=(HMdcCal1*)iterMdcCal1->Next())) {
      if(pDrTimeCut && pDrTimeCut->cut(pMdcCal1)) continue;
      Int_t nHits   = pMdcCal1->getNHits();
      if(nHits==0) continue;
      UChar_t times = (nHits==-2) ? 3:1;       
      Int_t s,m,l,c;
      pMdcCal1->getAddress(s,m,l,c);
      if(sec>=0 && sec!=s) continue;
      Float_t t1=pMdcCal1->getTime1();
      Float_t t2=pMdcCal1->getTime2();
      addCell(s,m,l,c,times,t1,t2);
      nwires++;
    }
  }
  return nwires;
}

void HMdcEvntListCells::clearClustCounter(Int_t seg) {
  if(seg<0 || seg>1) return;
  for(Int_t s=0;s<6;s++) list[s].clearClustCounter(seg);
}

ClassImp(HMdcLayListCells)
ClassImp(HMdcModListCells)
ClassImp(HMdcSecListCells)
ClassImp(HMdcEvntListCells)
