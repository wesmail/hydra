#ifndef HTRENDHIST_H
#define HTRENDHIST_H
#include "TNamed.h"
#include "TString.h"
#include "TArrayI.h"
#include "TH1.h"
#include "hmonhist.h"

/** 1-Dim Trend Histogram.
 * Each call of the fill function moves the
 * content of the histogram by one bin to the left
 * and the new value is added in the right most bin.
*/
class HTrendHist: public HMonHist {
protected:
public:
    HTrendHist(const Char_t* name   ="",
	       const Char_t* title  ="",
	       Int_t active=0,Int_t resetable=0,
	       Int_t nBinsX=0,Float_t Xlo=0,Float_t Xup=0);
    HTrendHist(HMonHistAddon&);
    /// fill value (including moving the content)
    void    fill   (Float_t i,Float_t j);
    ~HTrendHist();
    ClassDef(HTrendHist,1) // trend hist
};

#endif  /*!HTRENDHIST_H*/
