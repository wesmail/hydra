#ifndef HTRENDHIST_H
#define HTRENDHIST_H
#include "TNamed.h"
#include "TString.h"
#include "TArrayI.h"
#include "TH1.h"
#include "hmonhist.h"

class HTrendHist: public HMonHist {
protected:
public:
    HTrendHist(const char* name   ="",
	       const char* title  ="",
	       Int_t active=0,Int_t resetable=0,
	       Int_t nBinsX=0,Float_t Xlo=0,Float_t Xup=0);
    HTrendHist(HMonHistAddon&);
    void    fill   (Float_t i,Float_t j);
    ~HTrendHist();
    ClassDef(HTrendHist,1) // trend hist
};

#endif  /*!HTRENDHIST_H*/
