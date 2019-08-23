#ifndef HRICHPATTERN_H
#define HRICHPATTERN_H
using namespace std;
#include "TNamed.h"
#include "TList.h"
#include "TString.h"
#include "TArrayI.h"
#include "TLine.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"
#include <iostream> 
#include <iomanip>

class HRichPattern: public TNamed{
protected:
    Int_t fIsActive;
    Int_t fisResetable;
    Int_t refreshrate;
    TString Option;
    TString directory;
    TString format;
    Int_t stat;
    Int_t max;
    Int_t min;
    TList* oldPattern;
    TList* newPattern;
public:
    HRichPattern(const Char_t* name   ="",
	       const Char_t* title  ="",
	       Int_t active=0,Int_t resetable=0);
	       
    ~HRichPattern();
    TList*   getNewList() {return newPattern;}
    TList*   getOldList() {return oldPattern;}
    void     setActive   (Int_t active)  {fIsActive=active;}
    Int_t    getActive   ()              {return fIsActive;}
    void     setResetable(Int_t reset)   {fisResetable=reset;}
    Int_t    getResetable()              {return fisResetable;}
    void     setRefreshRate(Int_t fresh) {refreshrate=fresh;}
    Int_t    getRefreshRate()            {return refreshrate;}
    void    reset();
    ClassDef(HRichPattern,1) // Pattern rich display
};

#endif  /*!HRICHPATTERN_H*/
