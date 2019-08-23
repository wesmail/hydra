#include "hmdcstoreevents.h"
#include "hmdclistcells.h"
#include "hmdcclus.h"

//*-- AUTHOR : Pechenov Vladimir
//*-- Modified : 14/05/2004 by V.Pechenov

////////////////////////////////////////////////////////////////
// HMdcStoreWires
//
// HMdcStoreEvents
//
// HMdcStoreClusters
//
////////////////////////////////////////////////////////////////

ClassImp(HMdcStoreWires)
ClassImp(HMdcStoreEvents)
ClassImp(HMdcStoreClusters)

HMdcStoreWires::HMdcStoreWires(void) {
  nDTPerEvent.setArrType(kUShort);
  iterEvents=nDTPerEvent.makeIterator();
  mdcWireAddr.setArrType(kUShort);
  iterWires=mdcWireAddr.makeIterator();
  nEvents=0;
  nDrTimes=0;
  nRdEvents=0;
  nRdDrTimes=0;
  maxMemSize=200*1024*1024;
}

HMdcStoreWires::~HMdcStoreWires(void) {
  if(iterEvents) delete iterEvents;
  if(iterWires) delete iterWires;
  iterEvents=0;
  iterWires=0;
}

void HMdcStoreWires::setMaxMemSize(UInt_t mSize) {
  maxMemSize=mSize*1024*1024;
}

UInt_t HMdcStoreWires::getMemSize(void) const {
  return mdcWireAddr.getMemSize()+nDTPerEvent.getMemSize();
}

Bool_t HMdcStoreWires::testMemSize(void) const {
  return (getMemSize()<maxMemSize) ? kTRUE:kFALSE;
}

void HMdcStoreWires::setEndEvent(void) {
  if(nDrTimes) {
    nDTPerEvent.addNext(nDrTimes);
    nDrTimes=0;
    nEvents++;
  }
}

void HMdcStoreWires::addWireAddr(Int_t s,Int_t m,Int_t l,Int_t c) {
  mdcWireAddr.addNext(packAddress(s,m,l,c));
  nDrTimes++;
}

void HMdcStoreWires::resetIter(void) {
  iterEvents->reset();
  iterWires->reset();
  nRdEvents=0;
  nRdDrTimes=0;
  nDrTmPerEv=0;
}

Bool_t HMdcStoreWires::getNextEvent(void) {
  if(nRdEvents==nEvents) return kFALSE;
  if(!iterEvents->getNext(nDrTmPerEv)) return kFALSE;
  nRdEvents++;
  nRdDrTimes=0;
  return kTRUE;
}

Bool_t HMdcStoreWires::getNextWireAddr(Int_t& s, Int_t& m, 
    Int_t& l, Int_t& c) {
  UShort_t addr; 
  if(nRdDrTimes==nDrTmPerEv || !iterWires->getNext(addr)) return kFALSE;
  unpackAddress(addr,s,m,l,c);
  nRdDrTimes++;
  return kTRUE;
}

void HMdcStoreWires::addEvent(const HMdcEvntListCells& event) {
  Int_t s,m,l,c;
  s=m=l=c=-1;
  while(event.nextCell(s,m,l,c)) addWireAddr(s,m,l,c);
  setEndEvent();
}

Bool_t HMdcStoreWires::getNextEvent(HMdcEvntListCells& event) {
  event.clear();
  if(!getNextEvent()) return kFALSE;
  Int_t s,m,l,c;
  s=m=l=c=-1;
  while(getNextWireAddr(s,m,l,c)) event.setTime(s,m,l,c,1);  // tim1 only !!!
  return kTRUE;
}

//====================================================

HMdcStoreEvents::HMdcStoreEvents(void) {
  nDTPerEvent.setArrType(kUShort);
  iterEvents=nDTPerEvent.makeIterator();
  mdcCells.setArrType(kUInt);
  iterWires=mdcCells.makeIterator();
  nEvents=0;
  nDrTimes=0;
  nRdEvents=0;
  nRdDrTimes=0;
  maxMemSize=200*1024*1024;
}

HMdcStoreEvents::~HMdcStoreEvents(void) {
  if(iterEvents) delete iterEvents;
  if(iterWires) delete iterWires;
  iterEvents=0;
  iterWires=0;
}

void HMdcStoreEvents::setMaxMemSize(UInt_t mSize) {
  maxMemSize=mSize*1024*1024;
}

UInt_t HMdcStoreEvents::getMemSize(void) const {
  return mdcCells.getMemSize()+nDTPerEvent.getMemSize();
}

Bool_t HMdcStoreEvents::testMemSize(void) const {
  return (getMemSize()<maxMemSize) ? kTRUE:kFALSE;
}

void HMdcStoreEvents::setEndEvent(void) {
  if(nDrTimes) {
    nDTPerEvent.addNext(nDrTimes);
    nDrTimes=0;
    nEvents++;
  }
}

void HMdcStoreEvents::addWire(Int_t s,Int_t m,Int_t l,Int_t c,
    Float_t tm) {
  mdcCells.addNext(packCell(s,m,l,c,tm));
  nDrTimes++;
}

void HMdcStoreEvents::resetIter(void) {
  iterEvents->reset();
  iterWires->reset();
  nRdEvents=0;
  nRdDrTimes=0;
  nDrTmPerEv=0;
}

Bool_t HMdcStoreEvents::getNextEvent(void) {
  if(nRdEvents==nEvents) return kFALSE;
  if(!iterEvents->getNext(nDrTmPerEv)) return kFALSE;
  nRdEvents++;
  nRdDrTimes=0;
  return kTRUE;
}

Bool_t HMdcStoreEvents::getNextCell(Int_t& s, Int_t& m, 
    Int_t& l, Int_t& c, Float_t& tm) {
  UInt_t addr; 
  if(nRdDrTimes==nDrTmPerEv || !iterWires->getNext(addr)) return kFALSE;
  unpackCell(addr,s,m,l,c,tm);
  nRdDrTimes++;
  return kTRUE;
}

void HMdcStoreEvents::addEvent(const HMdcEvntListCellsAndTimes& event) {
  Int_t s,m,l,c;
  Float_t tm;
  s=m=l=c=-1;
  while(event.nextTime(s,m,l,c,tm)) addWire(s,m,l,c,tm);
  setEndEvent();
}

Bool_t HMdcStoreEvents::getNextEvent(HMdcEvntListCellsAndTimes& event) {
  event.clear();
  if(!getNextEvent()) return kFALSE;
  Int_t s,m,l,c;
  Float_t tm;
  s=m=l=c=-1;
  while(getNextCell(s,m,l,c,tm)) event.addTime(s,m,l,c,tm);  // tim1 only !!!
  return kTRUE;
}

HMdcStoreClusters::HMdcStoreClusters(void) {
  clustPos.setArrType(kShort);
  iterClustPos=clustPos.makeIterator();
}

HMdcStoreClusters::~HMdcStoreClusters(void) {
  if(iterClustPos) delete iterClustPos;
  iterClustPos=0;
}
    
UInt_t HMdcStoreClusters::getMemSize(void) const {
  return HMdcStoreEvents::getMemSize()+clustPos.getMemSize();
}

void HMdcStoreClusters::resetIter(void) {
  HMdcStoreEvents::resetIter();
  iterClustPos->reset();
}

void HMdcStoreClusters::setClusterPar(Float_t xt,Float_t yt,
                                      Float_t xp,Float_t yp) {
  x1=xt;
  y1=yt;
  x2=xp;
  y2=yp;
}

void HMdcStoreClusters::setEndCluster(void) {
  iterClustPos->setNext(packCoor(x1));
  iterClustPos->setNext(packCoor(y1));
  iterClustPos->setNext(packCoor(x2));
  iterClustPos->setNext(packCoor(y2));
  setEndEvent();
}

Bool_t HMdcStoreClusters::getNextCluster(void) {
  if(!HMdcStoreEvents::getNextEvent()) return kFALSE;
  return getClustPar();
}

Bool_t HMdcStoreClusters::getClustPar(void) {
  Short_t c;
  if(!iterClustPos->getNext(c)) return kFALSE;
  x1=unpackCoor(c);
  if(!iterClustPos->getNext(c)) return kFALSE;
  y1=unpackCoor(c);
  if(!iterClustPos->getNext(c)) return kFALSE;
  x2=unpackCoor(c);
  if(!iterClustPos->getNext(c)) return kFALSE;
  y2=unpackCoor(c);
  return kTRUE;
}

Bool_t HMdcStoreClusters::setNewClustPar(Float_t xt,Float_t yt,
                                      Float_t xp,Float_t yp) {
  UInt_t cursor = iterClustPos->getCursor()+1;
  if((cursor%4) > 0 || cursor<4) return kFALSE; 
  setClusterPar(xt,yt,xp,yp);
  iterClustPos->setCursor(cursor-4);
  iterClustPos->setNext(packCoor(x1));
  iterClustPos->setNext(packCoor(y1));
  iterClustPos->setNext(packCoor(x2));
  iterClustPos->setNext(packCoor(y2));
  return kTRUE;
}

void HMdcStoreClusters::getClusterPar(Float_t& xt,Float_t& yt,
                                      Float_t& xp,Float_t& yp) const {
  xt=x1;
  yt=y1;
  xp=x2;
  yp=y2;
}

void HMdcStoreClusters::addClustWires(const HMdcEvntListCellsAndTimes& event, 
        const HMdcClus* cl1, const HMdcClus* cl2) {
  addClustWires(event,cl1);
  addClustWires(event,cl2);
}

void HMdcStoreClusters::addClustWires(const HMdcEvntListCellsAndTimes& event, 
        const HMdcClus* cl) {
  // store wires list only
  if(cl==0) return;
  Int_t sec=cl->getSec();
  Int_t seg=cl->getIOSeg();
  Int_t lay=-1;
  Int_t cell=-1;
  while(cl->getNextCell(lay,cell)) {
    Int_t l=lay%6;
    Int_t mod=(lay/6)+seg*2;
    addWire(sec,mod,l,cell,event.getTimeValue(sec,mod,l,cell));
  }   
}

Bool_t HMdcStoreClusters::getNextCluster(HMdcEvntListCellsAndTimes& event) {
  if(!getNextEvent(event)) return kFALSE;
  return getClustPar();
}
