#ifndef HMDCGEANTREADER_H
#define HMDCGEANTREADER_H

#include "hgeantreader.h"
#include "hlocation.h"
#include "hmdcdigitgeompar.h"
#include "hmdcgeantraw.h"

class HCategory;
class HMdcDigitGeomPar;



class HMdcGeantReader : public HGeantReader {
private:
  HCategory* fGeantRawCat;              // Pointer to the geant raw data
  HLocation loc;                        // Location for new object.
  HMdcDigitGeomPar* fDigitGeomPar;      // Digitization parameters
  Int_t fEventId;                       // Number of current event
  Int_t nHit;                           // Number of Hhits in one cell
  Float_t yDist;                        // Distance to the sence wire
  Float_t pi;                           // 3.14
  TTree *tr[24];                        // Root trees
  HMdcGeantRaw *hit;                    // Pointer to HMdcGeantRaw category
  HMdcGeantRaw *hitNeigh;               // Pointer to HMdcGeantRaw category

public:
  HMdcGeantReader(void);
//  HMdcGeantReader(Text_t* name,Text_t* title);
  ~HMdcGeantReader(void);
  Bool_t init(void);
  void setParContainers();
  Bool_t finalize(void) {return kTRUE;}
  Bool_t execute(void);
  Float_t perp(Int_t, Int_t, Int_t,Float_t, Float_t, Float_t, Float_t); // Calculate the shortest dostance to the sence wire
  Int_t cellNum(Int_t, Int_t, Float_t, Float_t); // Define the number of fired wire
  ClassDef(HMdcGeantReader,1) // GeantReader for Geant-ntuple files with Mdc data
};

#endif /* !HMDCGEANTREADER_H */


