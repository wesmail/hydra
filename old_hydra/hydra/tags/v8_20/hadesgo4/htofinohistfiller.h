#ifndef HTOFINOHISTFILLER_H
#define HTOFINOHISTFILLER_H
#include "TNamed.h"

class HParamList;
class HMonHistAddon;
class HMonHist;
class HMonHist2;
class HTrendHist;
class HHistArray;
class HTrendArray;
class HHistArray2;
class HTofinoRaw;
class HTofinoCal;
class TList;
class TArrayI;
class TString;
class HIterator;

/** Class to fill Tofino histograms.
 * This class is responible of booking, creating
 * and filling of the Tofino histograms. The fill
 * functions are called by HTofinoGo4Mon or TGo4Hades.
 * All actions on the histograms have to go through
 * HTofinoHistBooker and HTofinoHistFiller.
*/

class HTofinoHistFiller : public TNamed {

protected:
    // Histpointers

    static HHistArray*    hhits_raw;  //!< hist pointer
    static HHistArray*    hprof;  //!< hist pointer
    static HMonHist*      hmulttdcall;  //!< hist pointer
    static HMonHist2*     hAdc;  //!< hist pointer
    static HMonHist2*     hTdc;  //!< hist pointer
    static HHistArray*    hTofinoShower;  //!< hist pointer
    static HMonHist2*     htofinomult_raw;  //!
    static HMonHist2*     htofinomulttemp_raw;  //!
    static HTrendArray*   htofinomulttrend_raw;  //!

    static HMonHistAddon*  hhits_rawA;  //!< hist addon pointer
    static HMonHistAddon*  hprofA;  //!< hist addon pointer
    static HMonHistAddon*  hmulttdcallA;  //!< hist addon pointer
    static HMonHistAddon*  hAdcA;  //!< hist addon pointer
    static HMonHistAddon*  hTdcA;  //!< hist addon pointer
    static HMonHistAddon*  hTofinoShowerA;  //!< hist addon pointer
    static HMonHistAddon*  htofinomult_rawA;  //!
    static HMonHistAddon*  htofinomulttemp_rawA;  //!
    static HMonHistAddon*  htofinomulttrend_rawA;  //!

    // Colors
    static TArrayI colorsSector; ///< colors for hists to be compared in one plane
    static TArrayI colorsModule; ///< colors for hists to be compared in one sector
    static TArrayI colorsCuts;   ///< colors for hists to be compared for cuts

public:
    HTofinoHistFiller(const Char_t* name   ="TofinoHistFiller",
		      const Char_t* title  ="parameters for Tofino hists");
    ~HTofinoHistFiller();
    /// put parameter to list of HParCond
    void   putTofinoParams(HParamList*,TList*);
    /// get parameter to list of HParCond
    Bool_t getTofinoParams(HParamList*,TList*,TList*);
    /// fill raw histograms
    void   fillRaw(HTofinoRaw*);
    /// fill raw cat histograms
    void   fillRawCat(HIterator*);
    /// fill cal histograms
    void   fillCal(HTofinoCal*);
	//Fill ShowerTofino correlation
    void   fillShowerTofino(HIterator*,HIterator*);
    /// fill trend histograms
    void   fillTrend();
    ClassDef(HTofinoHistFiller,1) // Container for the Tofino Hists
};

#endif  /*!HTOFINOHISTFILLER_H*/

