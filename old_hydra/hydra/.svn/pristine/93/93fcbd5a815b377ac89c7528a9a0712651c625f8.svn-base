#ifndef HPHYSICSHISTFILLER_H
#define HPHYSICSHISTFILLER_H
#include "TNamed.h"

class HParamList;
class HMonHistAddon;
class HMonHist;
class HMonHist2;
class HTrendHist;
class HTrendArray;
class HHistArray;
class HHistArray2;
class HIterator;
class TList;
class TArrayI;
class TString;
class HSplineTrack;
class HMdcTrkCand;
class HMdcSeg;
class HTofHit;
class HShowerHitTof;
class HRichHit;

/** Class to fill Physics histograms.
 * This class is responible of booking, creating
 * and filling of the Physics histograms. The fill
 * functions are called by HPhysicsGo4Mon or TGo4Hades.
 * All actions on the histograms have to go through
 * HPhysicsHistBooker and HPhysicsHistFiller.
 */

class HPhysicsHistFiller : public TNamed {

protected:
    // Histpointers
    static HHistArray* hphimdctof;
    static HHistArray* hphimdctofino;
    static HHistArray* hphimdcrich;
    static HHistArray2* hdedxmdc;
    static HHistArray2* hdedxtof;
    static HHistArray2* hdedxtofino;

    static HMonHistAddon* hphimdctofA;
    static HMonHistAddon* hphimdctofinoA;
    static HMonHistAddon* hphimdcrichA;
    static HMonHistAddon* hdedxmdcA;
    static HMonHistAddon* hdedxtofA;
    static HMonHistAddon* hdedxtofinoA;

    // Colors
    static TArrayI colorsSector; ///< colors for hists to be compared in modules

public:
    HPhysicsHistFiller(const char* name   ="PhysicsHistFiller",
		       const char* title  ="parameters for Physics hists");
    ~HPhysicsHistFiller();
    /// put parameter to the list of HParCond
    void   putPhysicsParams(HParamList*,TList*);
    /// get parameter from list of HParCond
    Bool_t getPhysicsParams(HParamList*,TList*,TList*);
    /// fills dEdx of Mdc
    void fillMdcDeDx(HSplineTrack* spline,HMdcTrkCand* trk);
    void fillTofDeDx(HSplineTrack* spline,HTofHit* tofhit);
    void fillTofinoDeDx(HSplineTrack* spline,HShowerHitTof* tofhit);
    void fillMdcTofPhi(HMdcSeg* seg,HTofHit* tofhit);
    void fillMdcTofinoPhi(HMdcSeg* seg,HShowerHitTof* tofhit);
    void fillMdcRichPhi(HMdcSeg* seg,HRichHit* richhit);
    ClassDef(HPhysicsHistFiller,1) // Container for the Physics Hists
};

#endif  /*!HPHYSICSHISTFILLER_H*/

