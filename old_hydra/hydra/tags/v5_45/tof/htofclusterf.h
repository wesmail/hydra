#ifndef  HTOFCLUSTERF_H
#define  HTOFCLUSTERF_H

#include "htofhitf.h"
#include "iostream.h"
#include "hlocation.h"

class HCategory;
class HIterator;
class HTofGeomPar;
class HSpecGeomPar;
class HSpectrometer;
class HRuntimeDb;
class HTofCluster;

class HTofClusterF : public HReconstructor {
protected:
  HCategory* fHitCat;  //!Pointer to the hits category
  HCategory* fClusterCat;  // Pointer to the clusters category
  HLocation fLoc;
  HLocation fpLoc;      //!
  HLocation fCLoc;
  HIterator* iterh;     //!
  HIterator* iterc;     //!
  HTofGeomPar *fTofGeometry; //!Container for TOF geometry
  HSpecGeomPar *fSpecGeometry;

  void fillGeometry(HTofCluster *);
public:
  HTofClusterF(void);
  HTofClusterF(Text_t *name,Text_t *title);
  ~HTofClusterF(void);
  Bool_t init(void);
  void initParContainer(HSpectrometer *,HRuntimeDb *);
  Bool_t finalize(void) {return kTRUE;}
  Int_t execute(void);
  ClassDef(HTofClusterF,0) //Finds TOF clusters.
};

#endif /* !HTOFCLUSTERF_H */





