#ifndef HSHOWERHISTFILLER_H
#define HSHOWERHISTFILLER_H
#include "TNamed.h"

class HParamList;
class HMonHistAddon;
class HMonHist;
class HMonHist2;
class HTrendHist;
class HHistArray;
class HHistArray2;
class HShowerRaw;
class HShowerCal;
class HShowerHit;
class TList;
class TArrayI;
class TString;

class HShowerHistFiller : public TNamed {

protected:
    // Histpointers

    static HMonHist2*    hhitsRaw;  //!
    static HMonHist2*    hhitsRawM;  //!

    static HMonHistAddon*  hhitsRawA;  //!
    static HMonHistAddon*  hhitsRawMA;  //!

    // Colors
    static TArrayI colorsSector; // colors for hists to be compared in one plane
    static TArrayI colorsModule; // colors for hists to be compared in one sector
    static TArrayI colorsCuts;   // colors for hists to be compared for cuts

public:
    HShowerHistFiller(const char* name   ="ShowerHistFiller",
		      const char* title  ="parameters for Shower hists");
    ~HShowerHistFiller();
    void   putShowerParams(HParamList*,TList*);
    Bool_t getShowerParams(HParamList*,TList*,TList*);
    void   fillRaw(HShowerRaw*);
    void   fillCal(HShowerCal*);
    void   fillHit(HShowerHit*);
    void   fillTrend();
    ClassDef(HShowerHistFiller,1) // Container for the Shower Hists
};

#endif  /*!HSHOWERHISTFILLER_H*/

