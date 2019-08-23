#ifndef HAnaDST_H
#define HAnaDST_H

#include "hsudummyrec.h"
#include "TROOT.h"
#include "hiterator.h"
#include "hcategory.h"
#include "TH1.h"
#include "TH2.h"
#include "TH2F.h"
#include "TH3.h"
#include "TMath.h"
#include "TNtuple.h"

#include "hypinfodef.h"
#include "hhyplist.h"


//---------------------------------------------------------------

class HAnaDST: public HSUDummyRec 
{
public: 
	HAnaDST(){};
	HAnaDST(const char *pFileName);
   ~HAnaDST(void){};

    virtual Bool_t init(void);
    virtual Int_t  execute(void);
    virtual Bool_t finalize(void);

ClassDef(HAnaDST, 0)  //	

private: 

	// pointers to the input categories and their iterators  

	HCategory *pHypList;
	HIterator *pitHypList;

    // Histograms definitions
   TH2F *Idx;

};
//---------------------------------------------------------------

ClassImp(HAnaDST)


#endif
