#ifndef HKICKGRID_H
#define HKICKGRID_H

#include <TObject.h>
#include <math.h>
#include <iostream.h>

class HKickGrid : public TObject {
private:
  friend class HKickGridTrainer;
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
private:
  Int_t calcDimX(void) { return int(floor((fMaxX-fMinX)/(fGranularity))) + 1; }
  Int_t calcDimY(void) { return int(floor((fMaxY-fMidY)/(fGranularity))) + 1; }
  void init(void);
  Float_t &bin(Int_t x,Int_t y) { return fValues[y*fDimX+x]; }
  Float_t getBinContents(Int_t x,Int_t y) const {return fValues[y*fDimX+x];}
  Int_t getBinX(Double_t x) {  return int( floor( (x-fMinX) / fGranularity)); }
  Int_t getBinY(Double_t y) {  return int( floor( (y-fMidY) / fGranularity)); }
public:
  HKickGrid(void) {fValues=0;}
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
  void addPoint(Double_t theta,Double_t phi,Double_t apt);
  void endInput(void);
  ClassDef(HKickGridTrainer,0)
};

#endif
