#if !defined HDETSET_H
#define HDETSET_H

//-------------------------------------------------------------------------
//
// File name:       hdetset.h
// Subject:         Detector sets.
// Author:          D.Vasiliev (INFN-LNS, Catania) "vassili@lns.infn.it"
// Date:            January 2000
//
// Remarks:         The concept of detector sets was initially proposed 
//                  and developed by
//                  P.Finocchiaro (INFN-LNS,Catania, "finocchiaro@lns.infn.it")
//
//-------------------------------------------------------------------------
//
// Description:
//
// This file contains the declaration of the classes HDetSet, HDetSetOut,
// HShowerDetSet, HMdcDetSet, HRichDetSet, HTofDetSet.
// These classes provide all functions to maintain the online objects of 
// the type "detector set".
//
//-------------------------------------------------------------------------

#include <TNamed.h>
#include <TArrayI.h>
#include <TList.h>

enum DetName {

  TOF = 10,
  MDC,
  RICH,
  SHOWER,
  START,
  TOFINO

};


class HDetSetOut : public TObject {

 protected:

  TList *fList;

 public:

  HDetSetOut() : TObject() { fList = 0; }
  virtual ~HDetSetOut() { if(fList) { fList->Delete(); delete fList; } }
  TList* getList() const { return fList; }
  Bool_t init() {
    if(fList) { delete fList; fList = 0; }
    fList = new TList();
    if(!fList) return kFALSE;
    return kTRUE;
  }
  void clearList();

  ClassDef(HDetSetOut,0)

};


class HDetSet : public TNamed {

 protected:

  Int_t        fDetName;
  Int_t        fCategory;
  Bool_t       kMulti;
  Int_t        fIter;
  Int_t        fMinCell;
  Int_t        fMaxCell;
  TArrayI      fMultiAddress;
  TArrayI      fHitAddress;
  TArrayI      fSingleAddress;
  Bool_t       kEvaluated;
  HDetSetOut  *fOut; //!
    

 public:

  HDetSet();
  virtual ~HDetSet() { if(fOut) delete fOut; }
  virtual void evaluate();
  Bool_t isEvaluated() const { return kEvaluated; }
  HDetSetOut* getOutput() const { return fOut; }
  const char* getUserWord();
  void setDetName(Int_t aname) { fDetName = aname; }
  Int_t getDetName() { return fDetName; }
  void setCategory(Int_t aname) { fCategory = aname; }
  Int_t getCategory() { return fCategory; }
  void setMulti(Bool_t aflag=kTRUE) { kMulti = aflag; }
  Bool_t isMulti() { return kMulti; }
  void resetIter() { fIter = -1; }
  Int_t getMinCell() { return fMinCell; }
  Int_t getMaxCell() { return fMaxCell; }
  virtual void calcMinMax();
  virtual Bool_t iterate(Int_t &n, Int_t* idx) { return kFALSE; }
  virtual Bool_t belongs(Int_t dim1, Int_t dim2=0, Int_t dim3=0, 
			   Int_t dim4=0) { return kFALSE; }
  virtual Int_t* iterateHit(Int_t &n) { return 0; }
  virtual Bool_t isEqual(HDetSet *pSet);
  virtual Bool_t isGeomEqual(HDetSet *pSet);
  Bool_t setSingleAddress(Int_t *idx, Int_t n);
  TArrayI getSingleAddress() const { return fSingleAddress; }
  Bool_t setHitAddress(Int_t *idx, Int_t n);
  TArrayI getHitAddress() const { return fHitAddress; }
  Bool_t setMultiAddress(Int_t *idx, Int_t n);
  TArrayI getMultiAddress() const { return fMultiAddress; }
  void clear();

 public:
  
  ClassDef(HDetSet,1)
    
};



class HShowerDetSet : public HDetSet {

 public:

  HShowerDetSet();
  virtual ~HShowerDetSet() {}
  Bool_t iterate(Int_t &n, Int_t* idx);
  Int_t* iterateHit(Int_t &n);
  Bool_t belongs(Int_t dim1, Int_t dim2=0, Int_t dim3=0, Int_t dim4=0) {
    if(kMulti) {
      if(fMultiAddress[192*dim1+64*dim2+8*(dim3/4)+dim4/4] == 1) return kTRUE;
    }
    else {
      if(fSingleAddress[0]==dim1 && fSingleAddress[1]==dim2 &&
	 fSingleAddress[2]==dim3 && fSingleAddress[3]==dim4) return kTRUE;
    }
    return kFALSE;
  }

 public:

  ClassDef(HShowerDetSet,1)

};


class HMdcDetSet : public HDetSet {

 public:

  HMdcDetSet();
  virtual ~HMdcDetSet() {}
  Bool_t iterate(Int_t &n, Int_t* idx);
  Int_t* iterateHit(Int_t &n);
  Bool_t belongs(Int_t dim1, Int_t dim2=0, Int_t dim3=0, Int_t dim4=0) {
    if(kMulti) {
      if(fMultiAddress[192*dim1+48*dim2+8*dim3+dim4/25] == 1) return kTRUE;
    }
    else {
      if(fSingleAddress[0]==dim1 && fSingleAddress[1]==dim2 &&
	 fSingleAddress[2]==dim3 && fSingleAddress[3]==dim4) return kTRUE;
    }
    return kFALSE;
  }

 public:

  ClassDef(HMdcDetSet,1)

};


class HTofDetSet : public HDetSet {

 public:

  HTofDetSet();
  virtual ~HTofDetSet() {}
  Bool_t iterate(Int_t &n, Int_t* idx);
  Int_t* iterateHit(Int_t &n);
  Bool_t belongs(Int_t dim1, Int_t dim2=0, Int_t dim3=0, Int_t dim4=0) {
    if(kMulti) {
      if(fMultiAddress[8*dim1+dim2] == 1) return kTRUE;
    }
    else {
      if(fSingleAddress[0]==dim1 && fSingleAddress[1]==dim2 &&
	 fSingleAddress[2]==dim3) return kTRUE;
    }
    return kFALSE;
  }

 public:

  ClassDef(HTofDetSet,1)

};


class HRichDetSet : public HDetSet {

 public:

  HRichDetSet();
  virtual ~HRichDetSet() {}
  Bool_t iterate(Int_t &n, Int_t* idx);
  Int_t* iterateHit(Int_t &n);
  Bool_t belongs(Int_t dim1, Int_t dim2=0, Int_t dim3=0, Int_t dim4=0) {
    if(kMulti) {
      if(fMultiAddress[64*dim1+8*(dim2/12)+dim3/12] == 1) return kTRUE;
    }
    else {
      if(fSingleAddress[0]==dim1 && fSingleAddress[1]==dim2 &&
	 fSingleAddress[2]==dim3) return kTRUE;
    }
    return kFALSE;
  }

 public:

  ClassDef(HRichDetSet,1)

};

#endif













