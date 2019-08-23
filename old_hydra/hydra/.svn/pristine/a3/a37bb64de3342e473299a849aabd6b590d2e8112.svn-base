// File: hrichpedestalraw.h
//
// Author: Thomas Eberl <Thomas.Eberl@physik.tu-muenchen.de>
// Last update by Thomas Eberl: 00/05/12 16:27:44
//

#ifndef HRICHPEDESTALRAW_H
#define HRICHPEDESTALRAW_H
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
class HIterator;
class HRichPedestalRaw : public HReconstructor {
public:
    HRichPedestalRaw();
    HRichPedestalRaw(Text_t *name,Text_t *title, Int_t numsec);
    ~HRichPedestalRaw();
    Bool_t init(void);
    Bool_t finalize(void);
    Int_t execute(void);
    void createPedestal(void);
    void createCalibration(void);
private:
    HCategory * pRichRaw;
    Int_t iSec , nCounter,rowMax,colMax;
    Double_t sum[96][96],sum2[96][96];
    Int_t stat[96][96][1];
    TVector  *vec3,*vec5,*vec6; 
    HIterator* pIter;    //!
public:
    ClassDef(HRichPedestalRaw,1) 
	};

#endif /* !HRICHPEDESTALRAW_H */

