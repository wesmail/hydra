#ifndef  HTOFCLUSTERF_H
#define  HTOFCLUSTERF_H

#include "htofhitf.h"
#include "iostream.h"
#include "hlocation.h"

class HCategory;
class HIterator;
class HTofGeomPar;
class HSpecGeomPar;
class HTofClusterFPar;
class HSpectrometer;
class HRuntimeDb;
class HTofCluster;

class HTofClusterF : public HReconstructor {
protected:
  HCategory* fHitCat;  // Pointer to the hit category
  HCategory* fClusterCat;  // Pointer to the cluster category
  HLocation fLoc;
  HLocation fpLoc;      //
  HLocation fCLoc;
  HIterator* iterh;     // Iterator through hit category.
  HIterator* iterc;     // Iterator through cluster category.
  HTofGeomPar *fTofGeometry; // Container for TOF geometry.
  HSpecGeomPar *fSpecGeometry; // Container for Spectrometer geometry.
  HTofClusterFPar *fClusterFPar; // Clusterfinder parameter container.

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





