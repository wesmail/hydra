#ifndef HMDCGEANTROOTREADER_H
#define HMDCGEANTROOTREADER_H

#include "hgeantreader.h"
#include "hlocation.h"
#include "hmatrixcategory.h"
#include "hindextable.h"
#include "hlocation.h"
#include <TTree.h>

class HCategory;
class HMdcLayerGeomPar;
class HMdcGeantRaw;

class HMdcGeantRootReader : public HGeantReader {
private:
  HCategory* fGeantRawCat;              //  Pointer to the geant raw data
  HLocation loc;                        //  Location for new object.
  HMdcLayerGeomPar* fDigitGeomPar;      //  Geometry parameters of a layer
  Int_t fEventId;                       //  Number of current event
  Int_t nHit;                           //  Number of hits per cell
  Float_t yDist;                        //  Distance from the hit to the first wire in y-direction  
  Float_t pi;                           //  3.14
  TTree* t;                             //  Pointer to the root tree
  HMdcGeantRaw *hit;                    //  Pointer to the HMdcGeantRaw
  HMdcGeantRaw *hitNeigh;               //  Pointer to the HMdcGeantRaw
  struct geant_root {
    HMatrixCategory oHMatrixCategory;
    HIndexTable     oHIndexTable; 
    Int_t           fNDataObjs;
    Int_t   trackNumber[28800];            // GEANT track number
    Float_t xHit[28800];                   // x of hit  (in cm)
    Float_t yHit[28800];                   // y of hit  (in cm)
    Float_t thetaHit[28800];               // theta of hit  (0-180 deg)
    Float_t phiHit[28800];                 // phi of hit  (0-360 deg)
    Float_t tofHit[28800];                 // time of flight of hit  (in ns)
    Char_t  sector[28800];                 // sector number  (0-5)
    Char_t  module[28800];                 // module number  (0-3)
    Char_t  layer[28800];                  // layer number  (0-5)
  } mdc_root;                    //! MDC GEANT input
public:
  HMdcGeantRootReader(void);
//  HMdcGeantRootReader(Text_t* name,Text_t* title);
  ~HMdcGeantRootReader(void) {}
  Bool_t init(void);
  void setParContainers();
  Bool_t finalize(void) {return kTRUE;}
  Bool_t execute(void);
  Float_t perp(Int_t, Int_t, Int_t,Float_t, Float_t, Float_t, Float_t); // Calculate the shortest dostance to the sence wire
  Int_t cellNum(Int_t, Int_t, Float_t, Float_t);// Define the number of fired wire
  ClassDef(HMdcGeantRootReader,1) // GeantReader for Geant-root files with Mdc data
};

#endif /* !HMDCGEANTROOTREADER_H */




