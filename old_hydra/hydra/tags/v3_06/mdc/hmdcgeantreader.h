#ifndef HMDCGEANTREADER_H
#define HMDCGEANTREADER_H

#include "hgeantreader.h"
#include "hmdcgeantcell.h"
#include "hlocation.h"

class HMdcLayerGeomPar;
class HCategory;

class HMdcGeantReader : public HGeantReader {
public:
  HCategory* fGeantCellCat;             // Pointer to the geant raw data
  HMdcLayerGeomPar* fDigitGeomPar;      // Geometry parameters of the layers
  HLocation loc;                        // Location for new object.
  HMdcGeantCell* hit;                   // Pointer to HMdcGeantCell hit
  HMdcGeantCell* hitNeigh;              // Pointer to neighbour
  Int_t fEventId;                       // Number of current event
  Float_t yDist;                        // Distance to the sence wire
  Float_t pi;                           // 3.14/180

public:
  HMdcGeantReader(void);
  ~HMdcGeantReader(void) {}
  void setParContainers();
  Bool_t finalize(void) {return kTRUE;}
  Bool_t transform(Float_t,Float_t,Float_t,Float_t,Float_t,Int_t);
  void storeCell(Float_t,Float_t,Int_t);
  ClassDef(HMdcGeantReader,1) // Base class for MdcGeantReaders classes
};

#endif /* !HMDCGEANTREADER_H */


