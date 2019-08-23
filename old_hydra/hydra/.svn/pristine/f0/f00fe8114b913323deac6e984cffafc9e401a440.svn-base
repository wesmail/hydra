#include "hmdcstoreevents.h"
#include "hmdclistcells.h"

//*-- AUTHOR : Pechenov Vladimir
//*-- Modified : 14/05/2004 by V.Pechenov

////////////////////////////////////////////////////////////////
// HMdcEventColl
//
// Stack for claster finder
//
////////////////////////////////////////////////////////////////

ClassImp(HMdcStoreWires)
ClassImp(HMdcStoreEvents)

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
