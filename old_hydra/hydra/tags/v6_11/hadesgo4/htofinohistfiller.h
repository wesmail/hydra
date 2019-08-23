#ifndef HTOFINOHISTFILLER_H
#define HTOFINOHISTFILLER_H
#include "TNamed.h"

class HParamList;
class HMonHistAddon;
class HMonHist;
class HMonHist2;
class HTrendHist;
class HHistArray;
class HHistArray2;
class HTofinoRaw;
class HTofinoCal;
class TList;
class TArrayI;
class TString;

class HTofinoHistFiller : public TNamed {

protected:
    // Histpointers

    static HHistArray*    hhits_raw;  //!
    static HHistArray*    hhits_rawM;  //!

    static HMonHistAddon*  hhits_rawA;  //!
    static HMonHistAddon*  hhits_rawMA;  //!

    // Colors
    static TArrayI colorsSector; // colors for hists to be compared in one plane
    static TArrayI colorsModule; // colors for hists to be compared in one sector
    static TArrayI colorsCuts;   // colors for hists to be compared for cuts

public:
    HTofinoHistFiller(const char* name   ="TofinoHistFiller",
		      const char* title  ="parameters for Tofino hists");
    ~HTofinoHistFiller();
    void   putTofinoParams(HParamList*,TList*);
    Bool_t getTofinoParams(HParamList*,TList*,TList*);
    void   fillRaw(HTofinoRaw*);
    void   fillCal(HTofinoCal*);
    void   fillTrend();
    ClassDef(HTofinoHistFiller,1) // Container for the Tofino Hists
};

#endif  /*!HTOFINOHISTFILLER_H*/

