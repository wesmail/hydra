#ifndef HWALLHISTFILLER_H
#define HWALLHISTFILLER_H
#include "TNamed.h"

class HParamList;
class HMonHistAddon;
class HMonHist;
class HMonHist2;
class HTrendHist;
class HHistArray;
class HTrendArray;
class HHistArray2;
class HWallRaw;
class HWallHit;
class TList;
class TArrayI;
class TString;
class HIterator;

/** Class to fill Wall histograms.
 * This class is responible of booking, creating
 * and filling of the Wall histograms. The fill
 * functions are called by HWallGo4Mon or TGo4Hades.
 * All actions on the histograms have to go through
 * HWallHistBooker and HWallHistFiller.
*/

class HWallHistFiller : public TNamed {

protected:
    // Histpointers

//    static HHistArray*    hhits_raw;  //!< hist pointer
//    static HMonHist*      hmulttdcall;  //!< hist pointer
    static HHistArray*      hProf1;  //!< hist pointer
    static HHistArray*      hProf2;  //!< hist pointer
    static HHistArray*      hProf3;  //!< hist pointer
    static HMonHist2*     hRawTime1;  //!< hist pointer
    static HMonHist2*     hRawWidth1;  //!< hist pointer
    static HMonHist2*     hHitTime;  //!< hist pointer
    static HMonHist2*     hHitCharge;  //!< hist pointer
    static HMonHist2*     hHitXY;  //!< hist pointer
    
	 static HMonHist2*     hFib1Mult;  //!< hist pointer
    static HHistArray*    hRawMult;  //!< hist pointer
//    static HMonHist2*     hwallmult_raw;  //!
//    static HMonHist2*     hwallmulttemp_raw;  //!
//    static HTrendArray*   hwallmulttrend_raw;  //!

//    static HMonHistAddon*  hhits_rawA;  //!< hist addon pointer
//    static HMonHistAddon*  hmulttdcallA;  //!< hist addon pointer
    static HMonHistAddon*  hProf1A;  //!< hist addon pointer
    static HMonHistAddon*  hProf2A;  //!< hist addon pointer
    static HMonHistAddon*  hProf3A;  //!< hist addon pointer
    static HMonHistAddon*  hRawTime1A;  //!< hist addon pointer
    static HMonHistAddon*  hRawWidth1A;  //!< hist addon pointer
    static HMonHistAddon*  hHitTimeA;  //!< hist addon pointer
    static HMonHistAddon*  hHitChargeA;  //!< hist addon pointer
    static HMonHistAddon*  hHitXYA;  //!< hist addon pointer
    
	 static HMonHistAddon*  hFib1MultA;  //!< hist addon pointer
    static HMonHistAddon*  hRawMultA;  //!< hist addon pointer
//    static HMonHistAddon*  hwallmult_rawA;  //!
//    static HMonHistAddon*  hwallmulttemp_rawA;  //!
//    static HMonHistAddon*  hwallmulttrend_rawA;  //!

    // Colors
    static TArrayI colorsSector; ///< colors for hists to be compared in one plane
    static TArrayI colorsModule; ///< colors for hists to be compared in one sector
    static TArrayI colorsCuts;   ///< colors for hists to be compared for cuts

public:
    HWallHistFiller(const Char_t* name   ="WallHistFiller",
		      const Char_t* title  ="parameters for Wall hists");
    ~HWallHistFiller();
    /// put parameter to list of HParCond
    void   putWallParams(HParamList*,TList*);
    /// get parameter to list of HParCond
    Bool_t getWallParams(HParamList*,TList*,TList*);
    /// fill raw histograms
    void   fillRaw(HWallRaw*);
    /// fill raw cat histograms
    void   fillRawCat(HIterator*);
    /// fill hit histograms
    void   fillHit(HWallHit*);
    /// fill trend histograms
    void   fillTrend();
    ClassDef(HWallHistFiller,1) // Container for the Wall Hists
};

#endif  /*!HWALLHISTFILLER_H*/

