#ifndef HMDCHISTFILLER_H
#define HMDCHISTFILLER_H
#include "TNamed.h"
#include "hmdcconfig.h"
#include "hmdcconnectedtdc.h"

class HParamList;
class HMonHistAddon;
class HMonHist;
class HMonHist2;
class HTrendHist;
class HHistArray;
class HHistArray2;
class HTrendArray;
class HMdcConfig;
class HMdcRaw;
class HMdcRawEventHeader;
class HIterator;
class HMdcCal1;
class HMdcCutStat;
class HMdcCutStat;
class TList;
class TArrayI;
class TString;

/** Class to fill Mdc histograms.
 * This class is responible of booking, creating
 * and filling of the Mdc histograms. The fill
 * functions are called by HMdcGo4Mon or TGo4Hades.
 * All actions on the histograms have to go through
 * HMdcHistBooker and HMdcHistFiller.
 */
class HMdcHistFiller : public TNamed {

protected:
    static HMdcCutStat*    cutstat;//!< pointer to HMdcCutSat container
    static HMdcConfig*     mdcconfig; //!< pointer to internal HMdcConfig
    static HMdcConnectedTdc* mdclookup; //!< pointer to internal HMdcConnectedTdc

    // Histpointers

    static HHistArray*  htime1Cal1; //!<hist pointer
    static HTrendHist*  htime1Cal1MeanTrend; //!<hist pointer
    static HMonHist*    htime1Cal1MeanTrendtemp; //!<hist pointer
    static HHistArray*  hcutstat; //!<hist pointer
    static HMonHist2*   hCal1Hits; //!<hist pointer
    static HMonHist2*   hCal1Hitstemp; //!<hist pointer
    static HTrendArray* hCal1HitsTrend; //!<hist pointer
    static HMonHist2*   hrawRoc_Subev; //!<hist pointer
    static HMonHist2*   hrawError_Stat; //!<hist pointer
    static HHistArray*  hlayer; //!<hist pointer
    static HHistArray2* hmbo_tdc; //!<hist pointer
    static HHistArray2* hmbo_tdc_calib; //!<hist pointer
    static HHistArray*  htime1Mod0; //!<hist pointer
    static HHistArray*  htime1Mod1; //!<hist pointer
    static HHistArray*  htime1Mod2; //!<hist pointer
    static HHistArray*  htime1Mod3; //!<hist pointer
    static HHistArray*  htime2Mod0; //!<hist pointer
    static HHistArray*  htime2Mod1; //!<hist pointer
    static HHistArray*  htime2Mod2; //!<hist pointer
    static HHistArray*  htime2Mod3; //!<hist pointer
    static HHistArray*  htime12Mod0; //!<hist pointer
    static HHistArray*  htime12Mod1; //!<hist pointer
    static HHistArray*  htime12Mod2; //!<hist pointer
    static HHistArray*  htime12Mod3; //!<hist pointer
    static HHistArray2* htime1_12; //!<hist pointer
    static HHistArray*  htime1rawMod0; //!<hist pointer
    static HHistArray*  htime1rawMod1; //!<hist pointer
    static HHistArray*  htime1rawMod2; //!<hist pointer
    static HHistArray*  htime1rawMod3; //!<hist pointer
    static HHistArray*  htime2rawMod0; //!<hist pointer
    static HHistArray*  htime2rawMod1; //!<hist pointer
    static HHistArray*  htime2rawMod2; //!<hist pointer
    static HHistArray*  htime2rawMod3; //!<hist pointer
    static HHistArray*  htime12rawMod0; //!<hist pointer
    static HHistArray*  htime12rawMod1; //!<hist pointer
    static HHistArray*  htime12rawMod2; //!<hist pointer
    static HHistArray*  htime12rawMod3; //!<hist pointer
    static HHistArray2* htime1_12raw; //!<hist pointer
    static HHistArray2* hmbo; //!<hist pointer
    static HTrendArray* hmbotrendMod0; //!<hist pointer
    static HTrendArray* hmbotrendMod1; //!<hist pointer
    static HTrendArray* hmbotrendMod2; //!<hist pointer
    static HTrendArray* hmbotrendMod3; //!<hist pointer
    static HHistArray*  hmod_timestat_calib; //!<hist pointer

    static HMonHistAddon*  htime1Cal1A; //!<hist addon pointer
    static HMonHistAddon*  htime1Cal1MeanTrendA; //!<hist addon  pointer
    static HMonHistAddon*  htime1Cal1MeanTrendtempA; //!<hist addon pointer
    static HMonHistAddon*  hcutstatA; //!<hist addon  pointer
    static HMonHistAddon*  hCal1HitsA; //!<hist addon  pointer
    static HMonHistAddon*  hCal1HitstempA; //!<hist addon  pointer
    static HMonHistAddon*  hCal1HitsTrendA; //!<hist pointer
    static HMonHistAddon*  hrawRoc_SubevA; //!<hist addon  pointer
    static HMonHistAddon*  hrawError_StatA; //!<hist addon  pointer
    static HMonHistAddon*  hlayerA; //!<hist addon  pointer
    static HMonHistAddon*  hmbo_tdcA; //!<hist addon  pointer
    static HMonHistAddon*  hmbo_tdc_calibA; //!<hist addon  pointer
    static HMonHistAddon*  htime1Mod0A; //!<hist addon  pointer
    static HMonHistAddon*  htime1Mod1A; //!<hist addon  pointer
    static HMonHistAddon*  htime1Mod2A; //!<hist addon  pointer
    static HMonHistAddon*  htime1Mod3A; //!<hist addon  pointer
    static HMonHistAddon*  htime2Mod0A; //!<hist addon  pointer
    static HMonHistAddon*  htime2Mod1A; //!<hist addon  pointer
    static HMonHistAddon*  htime2Mod2A; //!<hist addon  pointer
    static HMonHistAddon*  htime2Mod3A; //!<hist addon  pointer
    static HMonHistAddon*  htime12Mod0A; //!<hist addon  pointer
    static HMonHistAddon*  htime12Mod1A; //!<hist addon  pointer
    static HMonHistAddon*  htime12Mod2A; //!<hist addon  pointer
    static HMonHistAddon*  htime12Mod3A; //!<hist addon  pointer
    static HMonHistAddon*  htime1_12A; //!<hist addon  pointer
    static HMonHistAddon*  htime1rawMod0A; //!<hist addon  pointer
    static HMonHistAddon*  htime1rawMod1A; //!<hist addon  pointer
    static HMonHistAddon*  htime1rawMod2A; //!<hist addon  pointer
    static HMonHistAddon*  htime1rawMod3A; //!<hist addon  pointer
    static HMonHistAddon*  htime2rawMod0A; //!<hist addon  pointer
    static HMonHistAddon*  htime2rawMod1A; //!<hist addon  pointer
    static HMonHistAddon*  htime2rawMod2A; //!<hist addon  pointer
    static HMonHistAddon*  htime2rawMod3A; //!<hist addon  pointer
    static HMonHistAddon*  htime12rawMod0A; //!<hist addon  pointer
    static HMonHistAddon*  htime12rawMod1A; //!<hist addon  pointer
    static HMonHistAddon*  htime12rawMod2A; //!<hist addon  pointer
    static HMonHistAddon*  htime12rawMod3A; //!<hist addon  pointer
    static HMonHistAddon*  htime1_12rawA; //!<hist addon  pointer
    static HMonHistAddon*  hmboA; //!<hist addon  pointer
    static HMonHistAddon*  hmbotrendMod0A; //!<hist addon  pointer
    static HMonHistAddon*  hmbotrendMod1A; //!<hist addon  pointer
    static HMonHistAddon*  hmbotrendMod2A; //!<hist addon  pointer
    static HMonHistAddon*  hmbotrendMod3A; //!<hist addon  pointer
    static HMonHistAddon*  hmod_timestat_calibA; //!<hist addon  pointer
    // Colors
    static TArrayI colorsSector; ///< colors for hists to be compared in one plane
    static TArrayI colorsModule; ///< colors for hists to be compared in one sector
    static TArrayI styleModule;  ///< marker style for hists to be compared in modules
    static TArrayI colorsCuts;   ///< colors for hists to be compared for cuts
    static Int_t layerhists; ///< switch on/off layer hists
    static Int_t mbotdchists; ///< switch on/off mbo hists
    static Int_t mbohists; ///< switch on/off mbo hists
    static Int_t timescalhists; ///< switch on/off time cal hists
    static Int_t timesrawhists; ///< switch on/off time raw hists
    static Int_t normhists; ///< switch on/off  normalization

    Float_t countSecMod[6][4];

public:
    HMdcHistFiller(const char* name   ="MdcHistFiller",
		   const char* title  ="parameters for Mdc hists");
    ~HMdcHistFiller();
    /// put parameter to List of HParCond
    void   putMdcParams(HParamList*,TList*);
    /// get parameter from list of HParCond
    Bool_t getMdcParams(HParamList*,TList*,TList*);
    /// get parameter from list of HParCond
    Bool_t getTimesCalParams(HParamList*,TList*,TList*);
    /// get parameter from list of HParCond
    Bool_t getTimesRawParams(HParamList*,TList*,TList*);
    /// get parameter from list of HParCond
    Bool_t getMboParams(HParamList*,TList*,TList*);
    /// fill histograms of Mdc Raw level
    void   fillRaw(HMdcRaw*);
    /// fill histograms of Mdc Raw Header
    void   fillRawHeader(HMdcRawEventHeader*);
    /// fill histograms of Mdc Calibration
    void   fillCalibration(HIterator*);
    /// fill histograms of Mdc Cal1
    void   fillCal1(HMdcCal1*);
    /// fill htsograms for cut statistic
    void   fillCutStat();
    /// fill trend histograms
    void   fillTrend();
    /// fill HMdcConfig object
    void   fillMdcConfig(Int_t);
    /// get pointer to internal HMdcConfig object
    static HMdcConfig* getMdcConfig(void);
    static HMdcConnectedTdc* getMdcLookup(void);
    void initCounter(){for(Int_t s=0;s<6;s++){for(Int_t m=0;m<4;m++){countSecMod[s][m]=0;}}}
    ClassDef(HMdcHistFiller,1) // Container for the MDC Hists
};

#endif  /*!HMDCHISTFILLER_H*/

