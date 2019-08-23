#ifndef HTOFHISTFILLER_H
#define HTOFHISTFILLER_H
#include "TNamed.h"

class HParamList;
class HMonHistAddon;
class HMonHist;
class HMonHist2;
class HTrendHist;
class HHistArray;
class HHistArray2;
class HTofRaw;
class HTofCal;
class HTofHit;
class TList;
class TArrayI;
class TString;

class HTofHistFiller : public TNamed {

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
    HTofHistFiller(const char* name   ="TofHistFiller",
		   const char* title  ="parameters for Tof hists");
    ~HTofHistFiller();
    void   putTofParams(HParamList*,TList*);
    Bool_t getTofParams(HParamList*,TList*,TList*);
    void   fillRaw(HTofRaw*);
    void   fillCal(HTofCal*);
    void   fillHit(HTofHit*);
    void   fillTrend();
    ClassDef(HTofHistFiller,1) // Container for the Tof Hists
};

#endif  /*!HTOFHISTFILLER_H*/

