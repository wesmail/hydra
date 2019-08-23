#ifndef HSTARTHISTFILLER_H
#define HSTARTHISTFILLER_H
#include "TNamed.h"

class HParamList;
class HMonHistAddon;
class HMonHist;
class HMonHist2;
class HTrendHist;
class HHistArray;
class HHistArray2;
class HStartRaw;
class HStartCal;
class HStartHit;
class TList;
class TArrayI;
class TString;

class HStartHistFiller : public TNamed {

protected:
    // Histpointers

    static HHistArray*    hstrips_start_veto_raw;  //!
    static HHistArray*    hstrips_start_veto_rawM;  //!

    static HMonHistAddon*  hstrips_start_veto_rawA;  //!
    static HMonHistAddon*  hstrips_start_veto_rawMA;  //!

    // Colors
    static TArrayI colorsModule; // colors for hists to be compared in one sector
    static TArrayI colorsCuts;   // colors for hists to be compared for cuts

public:
    HStartHistFiller(const char* name   ="StartHistFiller",
		     const char* title  ="parameters for Start hists");
    ~HStartHistFiller();
    void   putStartParams(HParamList*,TList*);
    Bool_t getStartParams(HParamList*,TList*,TList*);
    void   fillRaw(HStartRaw*);
    void   fillCal(HStartCal*);
    void   fillHit(HStartHit*);
    void   fillTrend();
    ClassDef(HStartHistFiller,1) // Container for the Start Hists
};

#endif  /*!HSTARTHISTFILLER_H*/

