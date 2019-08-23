#ifndef HSHOWERHISTFILLER_H
#define HSHOWERHISTFILLER_H
#include "TNamed.h"

class HParamList;
class HMonHistAddon;
class HMonHist;
class HMonHist2;
class HTrendHist;
class HTrendArray;
class HHistArray;
class HHistArray2;
class HShowerConfig;
class HShowerRaw;
class HShowerCal;
class HShowerHit;
class TList;
class TArrayI;
class TString;

class HShowerHistFiller : public TNamed {

protected:
    // Histpointers
    HShowerConfig *showerconfig;//!
    static HMonHist2*   hhitsRaw;  //!
    static HMonHist2*   hhitsCal;  //!
    static HMonHist2*   hhitsHit;  //!
    static HMonHist*    hCalCol;  //!
    static HMonHist*    hCalRow;  //!


    //RAW multiplicity
    static HMonHist	*hRawMultTemp; //!
    static HTrendHist	*hRawMultTrend; //!
    static HMonHist	*hCalMultTemp; //!
    static HTrendHist	*hCalMultTrend; //!
    static HMonHist	*hHitMultTemp; //!
    static HTrendHist	*hHitMultTrend; //!
    static HTrendArray *hMultTrend;	 //!
    static HTrendArray *hSumTrend_0;	 //!
    static HTrendArray *hSumTrend_1;	 //!
    static HTrendArray *hSumTrend_2;	 //!
    static HTrendArray *hSumTrend_3;	 //!
    static HTrendArray *hSumTrend_4;	 //!
    static HTrendArray *hSumTrend_5;	 //!
    static HHistArray *hSumTemp; //!
    static HHistArray  *hSums_0; //!
    static HHistArray  *hSums_1; //!
    static HHistArray  *hSums_2; //!
    static HHistArray  *hSums_3; //!
    static HHistArray  *hSums_4; //!
    static HHistArray  *hSums_5; //!
    static HHistArray  *hCalRows[3]; //!
    static HHistArray  *hCalCols[3]; //!
    static HMonHist2*   hshowermult_raw;  //!
    static HMonHist2*   hshowermulttemp_raw;  //!
    static HTrendArray* hshowermulttrend_raw;  //!


    static HMonHistAddon*  hhitsRawA;  //!
    static HMonHistAddon*  hhitsCalA;  //!
    static HMonHistAddon*  hhitsHitA;  //!
    static HMonHistAddon*  hCalColA;  //!
    static HMonHistAddon*  hCalRowA;  //!
    static HMonHistAddon*  hRawMultTempA;  //!
    static HMonHistAddon*  hRawMultTrendA;  //!
    static HMonHistAddon*  hCalMultTempA;  //!
    static HMonHistAddon*  hCalMultTrendA;  //!
    static HMonHistAddon*  hHitMultTempA;  //!
    static HMonHistAddon*  hHitMultTrendA;  //!
    static HMonHistAddon*  hMultTrendA;  //!
    static HMonHistAddon*  hMultTempA;  //!
    static HMonHistAddon*  hSumTempA;  //!

    static HMonHistAddon *hSumTrendA_0;  //!
    static HMonHistAddon *hSumTrendA_1;  //!
    static HMonHistAddon *hSumTrendA_2;  //!
    static HMonHistAddon *hSumTrendA_3;  //!
    static HMonHistAddon *hSumTrendA_4;  //!
    static HMonHistAddon *hSumTrendA_5;  //!
    static HMonHistAddon *hSumsA_0;  //!
    static HMonHistAddon *hSumsA_1;  //!
    static HMonHistAddon *hSumsA_2;  //!
    static HMonHistAddon *hSumsA_3;  //!
    static HMonHistAddon *hSumsA_4;  //!
    static HMonHistAddon *hSumsA_5;  //!
    static HMonHistAddon *hCalRowsA[3];  //!
    static HMonHistAddon *hCalColsA[3];  //!
    static HMonHistAddon*  hshowermult_rawA;  //!
    static HMonHistAddon*  hshowermulttemp_rawA;  //!
    static HMonHistAddon*  hshowermulttrend_rawA;  //!


    // Colors
    static TArrayI colorsSector; // colors for hists to be compared in one plane
    static TArrayI colorsModule; // colors for hists to be compared in one sector
    static TArrayI colorsCuts;   // colors for hists to be compared for cuts

public:
    HShowerHistFiller(const Char_t* name   ="ShowerHistFiller",
		      const Char_t* title  ="parameters for Shower hists");
    ~HShowerHistFiller();
    void   putShowerParams(HParamList*,TList*);
    Bool_t getShowerParams(HParamList*,TList*,TList*);
    void   fillRaw(HShowerRaw*);
    void   fillCal(HShowerCal*);
    void   fillHit(HShowerHit*);
    void   resetHits();
    void   fillTrend(Int_t n);
    void fillSums(HHistArray* hist,Int_t mod,Float_t *sums);
    void fillShowerConfig(Int_t fCount);
    HShowerConfig* getShowerConfig(void){return showerconfig;}
    ClassDef(HShowerHistFiller,1) // Container for the Shower Hists
};

#endif  /*!HSHOWERHISTFILLER_H*/

