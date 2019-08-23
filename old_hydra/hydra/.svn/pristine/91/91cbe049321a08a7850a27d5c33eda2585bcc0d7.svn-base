#ifndef HMDCGEANTROOTREADER_H
#define HMDCGEANTROOTREADER_H

#include "hgeantreader.h"
#include "hlocation.h"
#include "hmdcdigitgeompar.h"
#include <TTree.h>

class HCategory;
class HMdcDigitGeomPar;
class HMdcGeantRaw;

class HMdcGeantRootReader : public HGeantReader {
private:
  HCategory* fGeantRawCat;              //  Pointer to the geant raw data
  HLocation loc;                        //  Location for new object.
  HMdcDigitGeomPar* fDigitGeomPar;      //  Digitisation parameters
  Int_t fEventId;                       //  Number of current event
  Int_t nHit;                           //  Number of hits per cell
  Float_t yDist;                        //  Distance from the hit to the first wire in y-direction  
  Float_t pi;                           //  3.14
  TTree* t;                             //  Pointer to the root tree
  HMdcGeantRaw *hit;                    //  Pointer to the HMdcGeantRaw
  HMdcGeantRaw *hitNeigh;               //  Pointer to the HMdcGeantRaw

public:
  HMdcGeantRootReader(void);
//  HMdcGeantRootReader(Text_t* name,Text_t* title);
  ~HMdcGeantRootReader(void);
  Bool_t init(void);
  void setParContainers();
  Bool_t finalize(void) {return kTRUE;}
  Bool_t execute(void);
  Float_t perp(Int_t, Int_t, Int_t,Float_t, Float_t, Float_t, Float_t); // Calculate the shortest dostance to the sence wire
  Int_t cellNum(Int_t, Int_t, Float_t, Float_t);// Define the number of fired wire
  ClassDef(HMdcGeantRootReader,1) // GeantReader for Geant-root files with Mdc data
};

#endif /* !HMDCGEANTROOTREADER_H */




