#ifndef HSTARTHISTFILLER_H
#define HSTARTHISTFILLER_H
#include "TNamed.h"

class HParamList;
class HMonHistAddon;
class HMonHist;
class HMonHist2;
class HTrendHist;
class HTrendArray;
class HHistArray;
class HHistArray2;
class HStartRaw;
class HStartCal;
class HStartHit;
class HIterator;
class TList;
class TArrayI;
class TString;

/** Class to fill Start histograms.
 * This class is responible of booking, creating
 * and filling of the Start histograms. The fill
 * functions are called by HStartGo4Mon or TGo4Hades.
 * All actions on the histograms have to go through
 * HStartHistBooker and HStartHistFiller.
 */

class HStartHistFiller : public TNamed {

protected:
    // Histpointers

    static HHistArray*    hstrips_hodo_tof;  //!< hist pointer
    static HHistArray*    hstrips_hodo_pos;  //!< hist pointer
    static HHistArray*    hstrips_hodo_raw;  //!< hist pointer
    static HHistArray*    hstrips_hodo_mult;  //!< hist pointer
    static HHistArray*    hstrips_hodo_both_raw;  //!< hist pointer
    static HHistArray*    hstrips_hodo_cal;  //!< hist pointer
    static HHistArray*    hstrips_start_veto_raw;  //!< hist pointer
    static HHistArray*    hstrips_start_vetotemp_raw;  //!< hist pointer
    static HTrendArray*   hstartmeanstriptrend_raw; //!< hist pointer
    static HHistArray*    hmult_start_veto_raw;  //!< hist pointer
    static HHistArray*    htime_start_raw;  //!< hist pointer
    static HHistArray*    htime_veto_raw;  //!< hist pointer
    static HTrendArray*   htime_trend_start;  //!< hist pointer
    static HTrendArray*   htime_trend_hodo;  //!< hist pointer
    static HHistArray*    htime_start_temp;  //!< hist pointer
    static HHistArray*    htime_hodo_temp;  //!< hist pointer
    static HMonHist2*     hstart_veto_corr_raw;  //!< hist pointer
    //Scalers
    static HTrendArray*   hscaler[8];   //!
    static HHistArray*    hscaler_tmp;  //!< hist pointer
	//Latch data
    static HHistArray*    hlatchdata;  //!< hist pointer
    //Vertex
    static HMonHist2*     hvertex_xy;  //!< hist pointer
    static HMonHist2*     hvertex_xz;  //!< hist pointer
    static HMonHist2*     hvertex_yz;  //!< hist pointer

    static HMonHistAddon*  hstrips_hodo_tofA;  //!< hist addon pointer
    static HMonHistAddon*  hstrips_hodo_posA;  //!< hist addon pointer
    static HMonHistAddon*  hstrips_hodo_rawA;  //!< hist addon pointer
    static HMonHistAddon*  hstrips_hodo_multA;  //!< hist addon pointer
    static HMonHistAddon*  hstrips_hodo_both_rawA;  //!< hist addon pointer
    static HMonHistAddon*  hstrips_hodo_calA;  //!< hist addon pointer
    static HMonHistAddon*  hstrips_start_veto_rawA;  //!< hist addon pointer
    static HMonHistAddon*  hstrips_start_vetotemp_rawA;  //!< hist addon pointer
    static HMonHistAddon*  hstartmeanstriptrend_rawA;  //!< hist addon pointer
    static HMonHistAddon*  hmult_start_veto_rawA;  //!< hist addon pointer
    static HMonHistAddon*  htime_start_rawA;  //!< hist addon pointer
    static HMonHistAddon*  htime_veto_rawA;  //!< hist addon pointer
    static HMonHistAddon*  htime_trend_startA;  //!< hist addon pointer
    static HMonHistAddon*  htime_trend_hodoA;  //!< hist addon pointer
    static HMonHistAddon*  htime_start_tempA;  //!< hist addon pointer
    static HMonHistAddon*  htime_hodo_tempA;  //!< hist addon pointer
    static HMonHistAddon*  hstart_veto_corr_rawA;  //!< hist addon pointer
    //scaler
    static HMonHistAddon*  hscalerA[8]; //!
    static HMonHistAddon*  hscaler_tmpA;
	//Latch data
	static HMonHistAddon*  hlatchdataA;  //!< hist addon pointer
    //Vertex
    static HMonHistAddon*     hvertex_xyA;  //!< hist pointer
    static HMonHistAddon*     hvertex_xzA;  //!< hist pointer
    static HMonHistAddon*     hvertex_yzA;  //!< hist pointer
    // Colors
    static TArrayI colorsModule; ///< colors for hists to be compared in modules
    static TArrayI colorsStrips; ///< colors for hists to be compared for all strips
    static TArrayI colorsCuts;   ///< colors for hists to be compared for cuts

public:
    HStartHistFiller(const char* name   ="StartHistFiller",
		     const char* title  ="parameters for Start hists");
    ~HStartHistFiller();
    /// put parameter to the list of HParCond
    void   putStartParams(HParamList*,TList*);
    /// get parameter from list of HParCond
    Bool_t getStartParams(HParamList*,TList*,TList*);
    /// fill latch histograms
    void   fillLatch(void);
    /// fill vertex histograms
    void   fillVertex(void);
    /// fill raw histograms
    void   fillRaw(HIterator*);
    /// fill cal histograms
    void   fillCal(HIterator*);
    /// fill hit histograms
    void   fillHit(HIterator*);
	/// fill scaler histograms
    void   fillScaler(HIterator*);
    /// fill trend histograms
    void   fillTrend();
    ClassDef(HStartHistFiller,1) // Container for the Start Hists
};

#endif  /*!HSTARTHISTFILLER_H*/

