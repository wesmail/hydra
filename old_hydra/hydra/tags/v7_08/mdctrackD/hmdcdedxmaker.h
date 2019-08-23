#ifndef HMDCDEDXMAKER_H
#define HMDCDEDXMAKER_H

#include "hreconstructor.h"
#include "TH2.h"
#include "TFile.h"

class HCategory;
class HIterator;
class HMdcDeDx;

class HMdcDeDxMaker : public HReconstructor {
protected:
  HCategory* trkcandCat;   //! pointer to the HMdcTrkCand data
  HCategory* segCat;       //! pointer to the HMdcSeg data
  Bool_t hasPrinted;       // flag is set if printStatus is called
  HIterator* trkcanditer;  //! iterator on HMdcTrkCand data.
  HMdcDeDx* mdcdedx;       //! dedx calculation parameters
  Int_t  method;           //! switch for different method of mdcdedx->calcdedx
  void   initParameters(void);
  void   setParContainers(void);

public:
  HMdcDeDxMaker(void);
  HMdcDeDxMaker(Text_t* name,Text_t* title);
  ~HMdcDeDxMaker(void);
  void   setDeDxMethod(Int_t vers) {method=vers;}
  Bool_t init(void);
  Bool_t finalize(void);
  void   printStatus();
  Int_t  execute(void);
  ClassDef(HMdcDeDxMaker,0) //  cal1 t2-t1 -> dEdx for Mdc data
};

#endif /* !HMDCDEDXMAKER_H */

