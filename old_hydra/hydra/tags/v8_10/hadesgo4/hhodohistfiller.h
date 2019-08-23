#ifndef HHODOHISTFILLER_H
#define HHODOHISTFILLER_H
#include "TNamed.h"

class HParamList;
class HMonHistAddon;
class HMonHist;
class HMonHist2;
class HTrendHist;
class HHistArray;
class HTrendArray;
class HHistArray2;
class HHodoRaw;
class TList;
class TArrayI;
class TString;
class HIterator;

/** Class to fill Hodo histograms.
 * This class is responible of booking, creating
 * and filling of the Hodo histograms. The fill
 * functions are called by HHodoGo4Mon or TGo4Hades.
 * All actions on the histograms have to go through
 * HHodoHistBooker and HHodoHistFiller.
*/

class HHodoHistFiller : public TNamed {

protected:
    // Histpointers

//    static HHistArray*    hhits_raw;  //!< hist pointer
    static HMonHist*      hProf1;  //!< hist pointer
    static HMonHist*      hProf2;  //!< hist pointer
//    static HMonHist*      hmulttdcall;  //!< hist pointer
    static HMonHist2*     hRawTime;  //!< hist pointer
    static HMonHist2*     hRawWidth;  //!< hist pointer
    static HMonHist2*     hSpot;  //!< hist pointer
    static HMonHist2*     hCrT1;  //!< hist pointer
    static HMonHist2*     hCrT2;  //!< hist pointer
    static HMonHist2*     hTDCMult;  //!< hist pointer
    static HMonHist2*     hFib1Mult;  //!< hist pointer
    static HMonHist2*     hFib2Mult;  //!< hist pointer
//    static HMonHist2*     hhodomult_raw;  //!
//    static HMonHist2*     hhodomulttemp_raw;  //!
//    static HTrendArray*   hhodomulttrend_raw;  //!

//    static HMonHistAddon*  hhits_rawA;  //!< hist addon pointer
    static HMonHistAddon*  hProf1A;  //!< hist addon pointer
    static HMonHistAddon*  hProf2A;  //!< hist addon pointer
//    static HMonHistAddon*  hmulttdcallA;  //!< hist addon pointer
    static HMonHistAddon*  hRawTimeA;  //!< hist addon pointer
    static HMonHistAddon*  hRawWidthA;  //!< hist addon pointer
    static HMonHistAddon*  hSpotA;  //!< hist addon pointer
    static HMonHistAddon*  hCrT1A;  //!< hist addon pointer
    static HMonHistAddon*  hCrT2A;  //!< hist addon pointer
    static HMonHistAddon*  hTDCMultA;  //!< hist addon pointer
    static HMonHistAddon*  hFib1MultA;  //!< hist addon pointer
    static HMonHistAddon*  hFib2MultA;  //!< hist addon pointer
//    static HMonHistAddon*  hhodomult_rawA;  //!
//    static HMonHistAddon*  hhodomulttemp_rawA;  //!
//    static HMonHistAddon*  hhodomulttrend_rawA;  //!

    // Colors
    static TArrayI colorsSector; ///< colors for hists to be compared in one plane
    static TArrayI colorsModule; ///< colors for hists to be compared in one sector
    static TArrayI colorsCuts;   ///< colors for hists to be compared for cuts

public:
    HHodoHistFiller(const char* name   ="HodoHistFiller",
		      const char* title  ="parameters for Hodo hists");
    ~HHodoHistFiller();
    /// put parameter to list of HParCond
    void   putHodoParams(HParamList*,TList*);
    /// get parameter to list of HParCond
    Bool_t getHodoParams(HParamList*,TList*,TList*);
    /// fill raw histograms
    void   fillRaw(HHodoRaw*);
    /// fill raw cat histograms
    void   fillRawCat(HIterator*);
    /// fill trend histograms
    void   fillTrend();
    ClassDef(HHodoHistFiller,1) // Container for the Hodo Hists
};

#endif  /*!HHODOHISTFILLER_H*/

