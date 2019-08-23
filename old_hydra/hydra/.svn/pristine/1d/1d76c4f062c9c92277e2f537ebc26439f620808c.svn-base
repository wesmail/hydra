#ifndef HMDCGEANTREADER_H
#define HMDCGEANTREADER_H

#include "hgeantreader.h"
#include "hlocation.h"
#include "hmdcgeantraw.h"
#include "TTree.h"

class HCategory;
class HMdcLayerGeomPar;
class HMdcGeantRaw;

class HMdcGeantReader : public HGeantReader {
private:
  HCategory* fGeantRawCat;              // Pointer to the geant raw data
  HLocation loc;                        // Location for new object.
  HMdcLayerGeomPar* fDigitGeomPar;      // Geometry parameters of the layers
  Int_t fEventId;                       // Number of current event
  Int_t nHit;                           // Number of Hhits in one cell
  Float_t yDist;                        // Distance to the sence wire
  Float_t pi;                           // 3.14
  TTree *tr[24];                        // Root trees
  HMdcGeantRaw* hit;                    // Pointer to HMdcGeantRaw category
  HMdcGeantRaw* hitNeigh;               // Pointer to HMdcGeantRaw category
  struct mdc_tuple{
    Int_t idevt, nhit, mdctrk[100];
    Float_t mdcx[100][6],mdcy[100][6],mdctheta[100][6];
    Float_t mdcphi[100][6],mdctof[100][6];
  } mdc;                                //! MDC GEANT ntuple
public:
  HMdcGeantReader(void);
//  HMdcGeantReader(Text_t* name,Text_t* title);
  ~HMdcGeantReader(void) {}
  Bool_t init(void);
  void setParContainers();
  Bool_t finalize(void) {return kTRUE;}
  Bool_t execute(void);
  Float_t perp(Int_t, Int_t, Int_t,Float_t, Float_t, Float_t, Float_t); // Calculate the shortest distance to the sense wire
  Int_t cellNum(Int_t, Int_t, Float_t, Float_t); // Define the number of fired wire
  ClassDef(HMdcGeantReader,1) // GeantReader for Geant-ntuple files with Mdc data
};

#endif /* !HMDCGEANTREADER_H */


