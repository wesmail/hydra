#ifndef HRTPARAMFINDER_H
#define HRTPARAMFINDER_H
using namespace std;
#include "TObject.h"
#include "TString.h"
#include "TObjArray.h"
#include <iostream> 
#include <iomanip>
#include "hrtmatchingpar.h"

class HRtMatchCell : public TObject {
 public:
  HRtMatchCell(void) {nGood = 0; nFakes=0; weight=0; color=0;}
  Float_t getSignalBackground(void) const 
    { if (nGood + nFakes ==0) return 0.;
      return float(nGood) / float(nFakes); }
  Int_t Compare(const TObject *obj) const;
  Bool_t IsSortable(void) const { return kTRUE; }

  Int_t nGood;
  Int_t nFakes;
  Char_t weight;
  UChar_t color;
 public:
  ClassDef(HRtMatchCell,1)
};

class HRtMatchGrid : public TObject {
 public:
  HRtMatchGrid(void);
  ~HRtMatchGrid(void);
  HRtMatchCell &bin(Int_t i[]);
  HRtMatchCell &bin(Float_t a[]);
  HRtMatchCell &getLastBin(void) { return fCells[fTotalBins]; }
  void setDimension(Int_t n,Int_t bins[]);
  void setRange(UInt_t i,Float_t min, Float_t max);
  void copyTo(TObjArray &list);
  HRtMatchingPar *syntetize(void);
 protected:
  HRtMatchCell *fCells; //[fSize] 
  UInt_t fSize;
  UInt_t fTotalBins;
  UInt_t fDimension;
  Float_t *fMin; //[fDimension]
  Float_t *fMax; //[fDimension]
  Float_t *fBinSize; //[fDimension]
  Int_t *fBins;  //[fDimension]
 public:
  ClassDef(HRtMatchGrid,1)
};

class HRtParamFinder : public TObject {
 public:
  HRtParamFinder(void);
  ~HRtParamFinder(void);
  void loadFile(const Text_t fileName[], Int_t maxEntries=kMaxInt);
  void setValidRange(const Text_t name[], Float_t min, Float_t max);
  void computeWeights(Float_t targetEfficiency=-1);
  void setDimension(Int_t n, ...);
  void setDimensionVector(Int_t n, Int_t arr[]);
  void setRange(UInt_t i,const Text_t name[],Float_t min, Float_t max);
  void setTuple(const Text_t name[]) { stTupleName = name; }
  HRtMatchingPar *syntetize(void) { return fParamSpace.syntetize(); }
 protected:
  HRtMatchGrid fParamSpace;
  TObjArray    fCellList;
  UInt_t fDimension;
  TString *bnVar;  //[fDimension]
  Float_t *lsVar;  //[fDimension]
  Float_t fMinValid,fMaxValid;
  TString bnValid;
  TString stTupleName;
  Int_t fTotalGood;
 public:
  ClassDef(HRtParamFinder,1)
};

/////////////////////////////////////////



#endif
