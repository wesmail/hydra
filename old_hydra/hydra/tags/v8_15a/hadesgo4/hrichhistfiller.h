#ifndef HRICHHISTFILLER_H
#define HRICHHISTFILLER_H
#include "TNamed.h"

class HParamList;
class HMonHistAddon;
class HMonHist;
class HRichFancy;
class HRichPattern;
class HMonHist2;
class HTrendHist;
class HHistArray;
class HTrendArray;
class HHistArray2;
class HRichRaw;
class HRichCal;
class HRichHit;
class TList;
class TArrayI;
class TString;

class HRichHistFiller : public TNamed {

protected:
    // Histpointers

    static HHistArray*    hRichPulseDist;  //!
    static HTrendArray*   hRichMultiplicity; //!
    static HTrendArray*   hRichChargedParticles; //!
    static HHistArray2*   hRichPulseheight; //!
    static HMonHist2*     hRichmult_raw;  //!
    static HMonHist2*     hRichmulttemp_raw;  //!
    static HTrendArray*   hRichmulttrend_raw;  //!

    static HRichPattern*   hRichMyPattern;
    static HRichFancy*     hRichFancyDisplay;
    static HMonHistAddon*  hRichPulseDistAddon;  //!
    static HMonHistAddon*  hRichMultiplicityAddon; //!
    static HMonHistAddon*  hRichChargedParticlesAddon; //!
    static HMonHistAddon*  hRichPulseheightAddon; //!
    static HMonHistAddon*  hRichmult_rawA;  //!
    static HMonHistAddon*  hRichmulttemp_rawA;  //!
    static HMonHistAddon*  hRichmulttrend_rawA;  //!

    // Colors
    static TArrayI colorsSector; // colors for hists to be compared in one plane
    static TArrayI colorsCuts;   // colors for hists to be compared for cuts

public:
    HRichHistFiller(const char* name   ="RichHistFiller",
		    const char* title  ="parameters for Rich hists");
    ~HRichHistFiller();
    void   putRichParams(HParamList*,TList*);
    Bool_t getRichParams(HParamList*,TList*,TList*);
    HRichPattern* getRichPattern(){return hRichMyPattern;}
    HRichFancy* getRichFancy(){return hRichFancyDisplay;}
    void   fillRawCategory(TIterator*);
    void   fillCalCategory(TIterator*);
    void   fillRaw(HRichRaw*);
    void   fillCal(HRichCal*);
    void   fillHit(HRichHit*);
    void   fillTrend();
    ClassDef(HRichHistFiller,1) // Container for the Rich Hists
};

#endif  /*!HRICHHISTFILLER_H*/

