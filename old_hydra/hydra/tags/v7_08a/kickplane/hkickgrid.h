#ifndef HKICKGRID_H
#define HKICKGRID_H
using namespace std;
#include <TObject.h>
#include <math.h>
#include <iostream> 
#include <iomanip>
#include <TH2.h>

class HKickGrid : public TObject {
private:
  friend class HKickGridTrainer;
  friend class HKickGridTrainerPol1;
  friend class HKickGridTrainerCor2;
  friend class HKickGridTrainerPol2;
  friend class HKickParOraIo;
  Float_t fMinX;
  Float_t fMaxX;
  Float_t fMinY;
  Float_t fMaxY;
  Float_t fMidY;
  Float_t fGranularity;
  Int_t fDimX; //Dimension in X
  Int_t fDimY; //Dimension in Y
  Int_t fEntries; //Nb of entries in grid
  Float_t *fValues; //[fEntries] Actual values
  Float_t fSim; //Is simetric?
public:
  Int_t calcDimX(void) { return int(floor((fMaxX-fMinX)/(fGranularity))) + 1; }
  Int_t calcDimY(void) { return int(floor((fMaxY-fMidY)/(fGranularity))) + 1; }
  void init(void);
  Float_t &bin(Int_t x,Int_t y) { return fValues[y*fDimX+x]; }
  Float_t getBinContents(Int_t x,Int_t y) const {return fValues[y*fDimX+x];}
  Int_t getBinX(Double_t x) {  return int( rint( (x-fMinX) / fGranularity)); }
  Int_t getBinY(Double_t y) {  return int( rint( (y-fMidY) / fGranularity)); }
public:
  HKickGrid(void) {fValues=0;fDimX=fDimY=0;}
  HKickGrid(HKickGrid *grid);
  HKickGrid(Float_t xMin,Float_t xMax,Float_t yMin,Float_t yMax,Float_t gran,
	    Float_t sim=1.);
  ~HKickGrid(void);
  Float_t getSim(void) { return fSim;}
  Int_t getDimX(void) { return fDimX; }
  Int_t getDimY(void) { return 2 * fDimY; }
  Double_t getValue(const Double_t x,const Double_t y);
  ostream &print(ostream &out);
  istream &read(istream &in);
  void clear(void);
  void readFrom(const HKickGrid &grid);
  ClassDef(HKickGrid,1)
};


class HKickGridTrainer : public TObject {
protected:
  HKickGrid *fGrid; // The grid being trained
  HKickGrid *fEntries; //Nb of entries in each bin of the grid
public:
  HKickGridTrainer(void);
  HKickGridTrainer(HKickGrid *grid);
  ~HKickGridTrainer(void);
  void reset(void);
  void addPoint(Double_t theta,Double_t phi,Double_t apt);
  void endInput(void);
  TH2F *fControl;
  ClassDef(HKickGridTrainer,0)
};

class HKickGridTrainerPol1 : public TObject {
protected:
  HKickGrid *fGridSlope; // The grid being trained
  HKickGrid *fGridOffset;
  HKickGrid *fEntries;
  HKickGrid *fSumX, *fSumX2, *fSumY, *fSumXY;
public:
  HKickGridTrainerPol1(void);
  HKickGridTrainerPol1(HKickGrid *slope, HKickGrid *ofs);
  ~HKickGridTrainerPol1(void);
  void addPoint(Double_t theta,Double_t phi,Double_t f,Double_t v,Double_t w=1.);
  void endInput(void);
  ClassDef(HKickGridTrainerPol1,1)
};

class HKickGridTrainerCor2 : public TObject {
protected:
  HKickGrid *fGridA; // The grid being trained
  HKickGrid *fGridB;
  HKickGrid *fGridC;
  HKickGrid *fSx2,*fS1,*fSx,*fSyx,*fSx_2,*fSx_1,*fSyx_1,*fSy;
  Bool_t fDebug;
public:
  HKickGridTrainerCor2(void);
  HKickGridTrainerCor2(HKickGrid *slope, HKickGrid *ofs, HKickGrid *C);
  ~HKickGridTrainerCor2(void);
  void addPoint(Double_t theta,Double_t phi,Double_t f,Double_t v,Double_t w=1.);
  void endInput(void);
  void reset(void);
  void setDebug(Bool_t t=kTRUE) { fDebug = t; }
  ClassDef(HKickGridTrainerCor2,1)
};

class HKickGridTrainerPol2 : public TObject {
protected:
  HKickGrid *fGridA,*fGridB,*fGridC; //The grids being trained
  HKickGrid *fSy,*fS1,*fSx_2,*fSx_3,*fSx,*fSyx_2,*fSx_4,*fSyx;
  Bool_t fDebug;
 public:
  HKickGridTrainerPol2(void);
  HKickGridTrainerPol2(HKickGrid *a,HKickGrid *b, HKickGrid *c);
  ~HKickGridTrainerPol2(void);
  void addPoint(Double_t theta,Double_t phi,Double_t f,Double_t v,Double_t w=1.);
  void endInput(void);
  void reset(void);
  void finalize(void);
  void setDebug(Bool_t t=kTRUE) { fDebug = t; }
  ClassDef(HKickGridTrainerPol2,1)
};

#endif
