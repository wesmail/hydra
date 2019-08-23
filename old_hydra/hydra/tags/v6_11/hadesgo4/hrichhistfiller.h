#ifndef HRICHHISTFILLER_H
#define HRICHHISTFILLER_H
#include "TNamed.h"

class HParamList;
class HMonHistAddon;
class HMonHist;
class HMonHist2;
class HTrendHist;
class HHistArray;
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

    static HHistArray*    hpulseheight;  //!
    static HHistArray*    hpulseheightM;  //!

    static HMonHistAddon*  hpulseheightA;  //!
    static HMonHistAddon*  hpulseheightMA;  //!

    // Colors
    static TArrayI colorsSector; // colors for hists to be compared in one plane
    static TArrayI colorsCuts;   // colors for hists to be compared for cuts

public:
    HRichHistFiller(const char* name   ="RichHistFiller",
		    const char* title  ="parameters for Rich hists");
    ~HRichHistFiller();
    void   putRichParams(HParamList*,TList*);
    Bool_t getRichParams(HParamList*,TList*,TList*);
    void   fillRaw(HRichRaw*);
    void   fillCal(HRichCal*);
    void   fillHit(HRichHit*);
    void   fillTrend();
    ClassDef(HRichHistFiller,1) // Container for the Rich Hists
};

#endif  /*!HRICHHISTFILLER_H*/

