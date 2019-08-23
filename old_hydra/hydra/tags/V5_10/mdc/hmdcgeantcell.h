#ifndef HMDCGEANTCELL_H
#define HMDCGEANTCELL_H

#include "hdataobject.h"

class HMdcGeantCell : public HDataObject {
private:
  Short_t sector;             // sector number
  Short_t module;             // module number
  Short_t layer;              // layer number
  Short_t cell;               // cell number
  Short_t fNumHits;           // Number of tracks per cell
  Float_t fMinDist[15];       // Shortest distance from particle trajectory to the sense wire
  Float_t fTimeFlight[15];    // Time of flight
  Int_t nTrack[15];           // Track number
public:
   HMdcGeantCell() {clear();}
  ~HMdcGeantCell() {}
   void clear();
   void setSector(const Int_t s) { sector = s; }
   void setModule(const Int_t m) { module = m; }
   void setLayer(const Int_t l) { layer = l; }
   void setCell(const Int_t c) { cell = c; }
   void setNumHits(Int_t nhit) {fNumHits=nhit;}
   void setMinDist(Float_t md, Int_t j) {fMinDist[j] = md;}
   void setTimeFlight(Float_t tof, Int_t j) {fTimeFlight[j] = tof;}
   void setNTrack(const Int_t t, const Int_t j) {nTrack[j] = t;}

   Int_t getSector() const { return sector; }
   Int_t getModule() const { return module; }
   Int_t getLayer() const { return layer; }
   Int_t getCell() const { return cell; }
   Int_t getNumHits(void) {return fNumHits;}
   Float_t getMinDist(Int_t j) {return fMinDist[j];}
   Float_t getTimeFlight(Int_t j) {return fTimeFlight[j];}
   Int_t getNTrack(const Int_t j) const {return nTrack[j];}

  ClassDef(HMdcGeantCell,1) //Intermediate MDC data (derived from HGeant output).
};

#endif









