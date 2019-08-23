// File: hrichpedestalcal.h
//
// Author: Thomas Eberl <Thomas.Eberl@physik.tu-muenchen.de>
// Date: Nov 14 1999
//
#ifndef HRICHPEDESTALCAL_H
#define HRICHPEDESTALCAL_H
#include "hreconstructor.h"
#include "hades.h"
#include "hevent.h"
#include "haddef.h"
#include <TVector.h>
#include <TArrayS.h>
#include <TH2.h>
#include <TCanvas.h>
#include <TStyle.h>
class HCategory;
class HRichPedestalCal : public HReconstructor {
 public:
  HRichPedestalCal();
  HRichPedestalCal(Text_t *name,Text_t *title, Int_t numsec);
  ~HRichPedestalCal();
  Bool_t init(void);
  Bool_t finalize(void);
  Int_t execute(void);
  void createPedestal(void);
  void createCalibration(void);
 private:
  HCategory * pRichCal;
  Int_t iSec , nCounter,rowMax,colMax;
  Double_t sum[96][96],sum2[96][96];
  TVector  *vec3,*vec5,*vec6; // Vectors
  Int_t stat[96][96][1];
 public:
  ClassDef(HRichPedestalCal,1) 
    };

#endif /* !HRICHPEDESTALCAL_H */
