#ifndef HMDCGEANTROOTREADER_H
#define HMDCGEANTROOTREADER_H

#include <TTree.h>
#include "hmdcgeantreader.h"
#include "hmatrixcategory.h"
class HGeantMdc;
class HLocation;

class HMdcGeantRootReader : public HMdcGeantReader {
private:
  TTree* t;                        //  Pointer to the root tree
  HMatrixCategory* fGeantMdcCat;   //! MDC HGeant input data

public:
  HMdcGeantRootReader(void);
  ~HMdcGeantRootReader(void);
  Bool_t init(void); 
  Bool_t execute(void); 
  inline HMatrixCategory* getGeantMdcCat(void) {return fGeantMdcCat;}
  inline HGeantMdc* getGeantMdc(HLocation locate) {
     return (HGeantMdc*)(fGeantMdcCat->getObject(locate));
  }
  ClassDef(HMdcGeantRootReader,1) // MDC reader for HGeant root file
};

#endif /* !HMDCGEANTROOTREADER_H */




