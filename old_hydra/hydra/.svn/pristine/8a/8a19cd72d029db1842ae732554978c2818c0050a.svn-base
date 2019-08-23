#ifndef HMDCHISTFILLER_H
#define HMDCHISTFILLER_H
#include "TNamed.h"
#include "hmdcconfig.h"

class HParamList;
class HMonHistAddon;
class HMonHist;
class HMonHist2;
class HTrendHist;
class HHistArray;
class HHistArray2;
class HMdcConfig;
class HMdcRaw;
class HMdcRawEventHeader;
class HMdcCal1;
class HMdcCutStat;
class HMdcCutStat;
class TList;
class TArrayI;
class TString;

class HMdcHistFiller : public TNamed {

protected:
    static HMdcCutStat*    cutstat;//!
    static HMdcConfig*     mdcconfig; //!

    // Histpointers

    static HMonHist*    htime1Raw;  //!
    static HMonHist*    htime1Cal1; //!
    static HTrendHist*  htime1Cal1MeanTrend; //!
    static HMonHist*    htime1Cal1MeanTrendtemp; //!
    static HHistArray*  hcutstat; //!
    static HHistArray*  hcutstatM; //!
    static HMonHist2*   hCal1Hits; //!
    static HMonHist2*   hrawRoc_Subev; //!
    static HMonHist2*   hrawError_Stat; //!

    static HMonHistAddon*  htime1RawA;  //!
    static HMonHistAddon*  htime1Cal1A; //!
    static HMonHistAddon*  htime1Cal1MeanTrendA; //!
    static HMonHistAddon*  htime1Cal1MeanTrendtempA; //!
    static HMonHistAddon*  hcutstatA; //!
    static HMonHistAddon*  hcutstatMA; //!
    static HMonHistAddon*  hCal1HitsA; //!
    static HMonHistAddon*  hrawRoc_SubevA; //!
    static HMonHistAddon*  hrawError_StatA; //!
    // Colors
    static TArrayI colorsSector; // colors for hists to be compared in one plane
    static TArrayI colorsModule; // colors for hists to be compared in one sector
    static TArrayI colorsCuts;   // colors for hists to be compared for cuts

public:
    HMdcHistFiller(const char* name   ="MdcHistFiller",
		   const char* title  ="parameters for Mdc hists");
    ~HMdcHistFiller();
    void   putMdcParams(HParamList*,TList*);
    Bool_t getMdcParams(HParamList*,TList*,TList*);
    void   fillRaw(HMdcRaw*);
    void   fillRawHeader(HMdcRawEventHeader*);
    void   fillCal1(HMdcCal1*);
    void   fillCutStat();
    void   fillTrend();
    void   fillMdcConfig(Int_t);
    static HMdcConfig* getMdcConfig(void);
    ClassDef(HMdcHistFiller,1) // Container for the MDC Hists
};

#endif  /*!HMDCHISTFILLER_H*/

