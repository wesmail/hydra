#ifndef HMDCWIREOFFSET_H
#define HMDCWIREOFFSET_H

#include "hreconstructor.h"
#include "hlocation.h"
#include "TRandom.h"
#include "hmdcsizescells.h"

class HCategory;
class HIterator;
class HMdcLookupGeom;
class HMdcLookupRaw;
class HMdcGeomStruct;
class HMdcGeomPar;
class HMdcLayerGeomPar;
class HMdcSizesCells;
class TH1F;
class TH2D;

class TDirectory;


class HMdcWireOffset : public HReconstructor {
protected:
  HCategory* calCat;       //! pointer to the cal data
  HCategory* hitCat;       //! pointer to the hit data
  Int_t mysector;          //! sector index of analyzed module
  Int_t mymodule;          //! module index of analyzed module
  Char_t *fNameRootHists;   // file name of root output file

  TH1F *hmdctime_layer[6][200][3];
  Int_t windowWidth[4];
  HLocation loccal1;           //! location for new object.
  HLocation lochit;           //! location for new object.

  HIterator* itercal1;         //! iterator on cal1 data.
  HIterator* iterhit;         //! iterator on cal1 data.

  HMdcLookupGeom* lookup;  //! lookup table for mapping
  HMdcLookupRaw* lookupRaw;//! lookup table for mapping
  HMdcGeomStruct* geomstruct;//! lookup table for max num wires/layer
  HMdcGeomPar *mdcGeomPar; //! pointer to MDC geometry container
  HMdcLayerGeomPar *mdcLayerGeomPar; //! pointer to geometry container for layers
  HMdcSizesCells *fsizescells;

public:
  HMdcWireOffset(void);
  HMdcWireOffset(Text_t* name,Text_t* title);
  ~HMdcWireOffset(void);
  void setOutputRoot  (Char_t*);
  void setWindowWidth (Int_t w0,Int_t w1,Int_t w2,Int_t w3)
  {
      windowWidth[0]=w0;
      windowWidth[1]=w1;
      windowWidth[2]=w2;
      windowWidth[3]=w3;
  };
  Bool_t init(void);
  Bool_t reinit(void);
  TDirectory *Mkdir(TDirectory *, Char_t *, Int_t, Int_t p=1);
  void setParContainers(void);
  void setMdcMod(Int_t s,Int_t m){mysector=s;mymodule=m;};
  Double_t calcLength(Double_t,Double_t,Double_t,Double_t);
  void printStatus();
  Bool_t finalize();
  Int_t execute();
  ClassDef(HMdcWireOffset,0) // fills different hists for cal1 data
};

#endif /* !HMDCWIREOFFSET_H */
