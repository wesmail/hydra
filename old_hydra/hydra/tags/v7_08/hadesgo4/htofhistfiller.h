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
class HTrendArray;
class HTofRaw;
class HTofHit;
class TList;
class TArrayI;
class TString;

class HTofHistFiller : public TNamed {

protected:
    // Histpointers

    static HHistArray*    hhits_raw;  //!
    static HMonHist*      hmulttof_raw;  //!
    static HMonHist*      hmulttoftofino_raw;  //!
    static HMonHist*      htofpatternL_raw;  //!
    static HMonHist*      htofpatternR_raw;  //!
    static HMonHist2*     htoftdcL_raw;  //!
    static HMonHist2*     htoftdcR_raw;  //!
    static HMonHist2*     htofadcL_raw;  //!
    static HMonHist2*     htofadcR_raw;  //!
    static HMonHist2*     htoftdcSum_hit;  //!
    static HMonHist2*     htoftdcDiff_hit;  //!
    static HMonHist2*     htofadcSum_hit;  //!
    static HMonHist2*     htofadcPos_hit;  //!
    static HMonHist*      htofsum_hit;  //!
    static HMonHist*      htoftheta_hit;  //!
    static HMonHist*      htofphi_hit;  //!
    static HMonHist2*     htofyvsx_hit;  //!
    static HMonHist2*     htofmult_raw;  //!
    static HMonHist2*     htofmulttemp_raw;  //!
    static HTrendArray*   htofmulttrend_raw;  //!

    static HMonHistAddon*  hhits_rawA;  //!
    static HMonHistAddon*  hmulttof_rawA;  //!
    static HMonHistAddon*  hmulttoftofino_rawA;  //!
    static HMonHistAddon*  htofpatternL_rawA;  //!
    static HMonHistAddon*  htofpatternR_rawA;  //!
    static HMonHistAddon*  htoftdcL_rawA;  //!
    static HMonHistAddon*  htoftdcR_rawA;  //!
    static HMonHistAddon*  htofadcL_rawA;  //!
    static HMonHistAddon*  htofadcR_rawA;  //!
    static HMonHistAddon*  htoftdcSum_hitA;  //!
    static HMonHistAddon*  htoftdcDiff_hitA;  //!
    static HMonHistAddon*  htofadcSum_hitA;  //!
    static HMonHistAddon*  htofadcPos_hitA;  //!
    static HMonHistAddon*  htofsum_hitA;  //!
    static HMonHistAddon*  htoftheta_hitA;  //!
    static HMonHistAddon*  htofphi_hitA;  //!
    static HMonHistAddon*  htofyvsx_hitA;  //!
    static HMonHistAddon*  htofmult_rawA;  //!
    static HMonHistAddon*  htofmulttemp_rawA;  //!
    static HMonHistAddon*  htofmulttrend_rawA;  //!

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
    void   fillRawMult(Int_t c,Int_t d);
    void   fillHit(HTofHit*);
    void   fillTrend();
    ClassDef(HTofHistFiller,1) // Container for the Tof Hists
};

#endif  /*!HTOFHISTFILLER_H*/

