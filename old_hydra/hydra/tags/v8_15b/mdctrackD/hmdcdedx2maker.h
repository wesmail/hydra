#ifndef HMDCDEDX2MAKER_H
#define HMDCDEDX2MAKER_H

#include "hreconstructor.h"
#include "TH2.h"
#include "TFile.h"

class HCategory;
class HIterator;
class HMdcDeDx2;

class HMdcDeDx2Maker : public HReconstructor {
protected:
  HCategory* trkcandCat;   //! pointer to the HMdcTrkCand data
  HCategory* segCat;       //! pointer to the HMdcSeg data
  HCategory* clsCat;       //! pointer to the HMdcClus data
  HCategory* clsInfCat;    //! pointer to the HMdcInfClus data
  HCategory* cal1Cat;      //! pointer to the HMdcCal1 data
  Bool_t hasPrinted;       // flag is set if printStatus is called
  HIterator* trkcanditer;  //! iterator on HMdcTrkCand data.
  HMdcDeDx2* mdcdedx;      //! dedx calculation parameters
  Int_t module;            // switch for caldedx()
  void   initParameters(void);
  void   setParContainers(void);

public:
  HMdcDeDx2Maker(void);
  HMdcDeDx2Maker(Text_t* name,Text_t* title);
  ~HMdcDeDx2Maker(void);
  Bool_t init(void);
  Bool_t finalize(void);
  void   printStatus();
  Int_t  execute(void);
  void   setUseModule(Int_t m){module=m;}
  ClassDef(HMdcDeDx2Maker,0) //  cal1 t2-t1 -> dEdx for Mdc data
};

#endif /* !HMDCDEDX2MAKER_H */

