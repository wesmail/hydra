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
class HHodoCal;
class HHodoHit;
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

// do not change ... has to be equal to 10 or major code changes have to be done
#define MAX_HODO_MODULES  10
// do not change, because color is valid up to 8 only
#define ANZ_EFFIS         8

class HHodoHistFiller : public TNamed {

protected:
    // Histpointers

//    static HHistArray*    hhits_raw;  //!< hist pointer
//    static HMonHist*      hmulttdcall;  //!< hist pointer
    static HHistArray*    hProf[MAX_HODO_MODULES];  //!< hist pointer
    static HMonHist2*     hRawTime[MAX_HODO_MODULES];  //!< hist pointer
    static HMonHist2*     hRawWidth[MAX_HODO_MODULES];  //!< hist pointer
    static HMonHist2*     hCrT[MAX_HODO_MODULES];  //!< hist pointer
    static HMonHist2*     hFibMult[MAX_HODO_MODULES];  //!< hist pointer

    static HMonHist2*     hTDCMult;  //!< hist pointer
    static HMonHist2*     hSpot;  //!< hist pointer
    static HMonHist2*     hNe5;  //!< hist pointer
    static HTrendArray* hEfficiency;  //!< hist pointer
//    static HMonHist2*     hhodomult_raw;  //!
//    static HMonHist2*     hhodomulttemp_raw;  //!
//    static HTrendArray*   hhodomulttrend_raw;  //!

//    static HMonHistAddon*  hhits_rawA;  //!< hist addon pointer
//    static HMonHistAddon*  hmulttdcallA;  //!< hist addon pointer
    static HMonHistAddon*  hProfA[MAX_HODO_MODULES];  //!< hist addon pointer
    static HMonHistAddon*  hRawTimeA[MAX_HODO_MODULES];  //!< hist addon pointer
    static HMonHistAddon*  hRawWidthA[MAX_HODO_MODULES];  //!< hist addon pointer
    static HMonHistAddon*  hCrTA[MAX_HODO_MODULES];  //!< hist addon pointer
    static HMonHistAddon*  hFibMultA[MAX_HODO_MODULES];  //!< hist addon pointer

    static HMonHistAddon*  hSpotA;  //!< hist addon pointer
    static HMonHistAddon*  hNe5A;  //!< hist addon pointer
    static HMonHistAddon*  hTDCMultA;  //!< hist addon pointer
//    static HMonHistAddon*  hhodomult_rawA;  //!
//    static HMonHistAddon*  hhodomulttemp_rawA;  //!
//    static HMonHistAddon*  hhodomulttrend_rawA;  //!
    static HMonHistAddon*  hEfficiencyA;  //!

    // Colors
    static TArrayI colorsModule; ///< colors for hists not onyl for module
//    static TArrayI colorsCuts;   ///< colors for hists to be compared for cuts

    Int_t effi_count_all[ANZ_EFFIS], effi_count[ANZ_EFFIS];
public:
    HHodoHistFiller(const Char_t* name   ="HodoHistFiller",
		      const Char_t* title  ="parameters for Hodo hists");
    ~HHodoHistFiller();
    /// put parameter to list of HParCond
    void   putHodoParams(HParamList*,TList*);
    /// get parameter to list of HParCond
    Bool_t getHodoParams(HParamList*,TList*,TList*);
    /// fill raw histograms
    void   fillRaw(HHodoRaw*);
    /// fill cal histograms
    void   fillCal(HHodoCal*);
    /// fill hit histograms
    void   fillHit(HHodoHit*);
    /// fill hit histograms
    void   fillHitCat(HIterator*);
    /// fill raw cat histograms
    void   fillRawCat(HIterator*);
    /// fill trend histograms
    void   fillTrend();
    ClassDef(HHodoHistFiller,1) // Container for the Hodo Hists
};

#endif  /*!HHODOHISTFILLER_H*/

