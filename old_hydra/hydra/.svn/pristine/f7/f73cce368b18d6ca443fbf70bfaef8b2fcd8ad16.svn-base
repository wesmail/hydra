//*-- Author : Jochen Markert  18.07.2007

#ifndef  HPidTrackSorter_H
#define  HPidTrackSorter_H

#include "hcategory.h"
#include "hiterator.h"

#include <vector>
#include "TString.h"
#include "TFile.h"
#include "TNtuple.h"


using  std::vector;
using  std::iostream;

# define NUMBER_OF_INDEX 7
# define NUMBER_OF_QUALITY_CRITERIA 12

class HPidTrackCand;

class HPidTrackSorter : public TNamed {

public:

    enum ERichMatch {
	kUseRICHIndex         = 0,   // require RICH valid index
	kUseRKRICHCorrelation = 1,   // require RK-RICH correlation (from HPidTrackCand)
	kUseRKRICHWindow      = 2    // require RK-RICH matching inside +- window
    };

    enum ESelect {
	kIsLepton = 0,
	kIsHadron = 1
    };

    enum ESwitch {
	kIsIndexRICH         =  0,    //
	kIsIndexInnerMDC     =  1,    //
	kIsIndexOuterMDC     =  2,    //
	kIsIndexTOF          =  3,    //
	kIsIndexSHOWER       =  4,    //
        kIsIndexRPC          =  5,    //
        kIsIndexMETA         =  6,    //
	kIsBestHitRICH       =  7,    //  (by number of pads)
	kIsBestHitInnerMDC   =  8,    //  (by chi2)
	kIsBestHitOuterMDC   =  9,    //  (by chi2)
	kIsBestHitMETA       = 10,    //  (by META match quality)
	kIsBestRKMETA        = 11,    //  (by RK META match quality)
	kIsBestRKRICH        = 12,    //  (by RK RICH match quality)
	kIsBestRK            = 13,    //  (by RK chi2, none fitted outer segments with lower priority)
	kIsBestSPLINE        = 14,    //  (by chi2)
	kIsBestKICK          = 15,    //  (by pull)
        kIsBestRKRKMETA      = 16,    //  (by RK chi2 * RK META match quality)
        kIsBestRKRKRICH      = 17,    //  (by RK chi2 * RK RICH match quality)
        kIsBestRKMETAQA      = 18     //  (by RK chi2 * META match quality)

    };


protected:


   typedef struct {
     Short_t ind_PidCand;
     Short_t ind_RICH   ;
     Short_t ind_innerMDC;
     Short_t ind_outerMDC;
     Short_t ind_TOF;
     Short_t ind_SHOWER;
     Short_t ind_RPC;
     Short_t ring_PadNr;
     Double_t innerMDCChi2;
     Double_t outerMDCChi2;
     Double_t metamatch_Quality;
     Double_t RK_META_match_Quality;
     Double_t RK_RICH_match_Quality;
     Double_t RK_Chi2;
     Double_t SPLINE_Chi2;
     Double_t KICK_Pull;
     Bool_t  RICH_RK_Corr;
   } candidate;

    static Bool_t  kDebug;
    static Int_t   printLevel;             //! higher -> more prints (1,2,3)
    static Int_t   kSwitchIndex;           //! switch to select sort by index
    static Int_t   kSwitchQuality;         //! switch to select sort by quality algorithms
    static Int_t   kSwitchParticle;        //! switch to select leptons/hadrons
    static Int_t   kSwitchRICHMatching;    //! switch to select RICH/MDC matching in lepton selection
    static Float_t fRICHMDCWindow  ;       //! matching window RICH/MDC in phi/theta (symmetric, degree)

    static Bool_t kIgnoreRICH;             //! switch to ignore RICH hits for Double_t hit counting
    static Bool_t kIgnoreInnerMDC;         //! switch to ignore InnerMDC hits for Double_t hit counting
    static Bool_t kIgnoreOuterMDC;         //! switch to ignore OuterMDC hits for Double_t hit counting
    static Bool_t kIgnoreMETA;             //! switch to ignore META hits for Double_t hit counting
    static Bool_t kIgnorePreviousIndex;    //! switch to ignore indices from previoius marked ued objects

    Int_t   kSwitchRICHMatchingBackup;    //! switch to select RICH/MDC matching in lepton selection
    Float_t fRICHMDCWindowBackup  ;       //! matching window RICH/MDC in phi/theta (symmetric, degree)

    Bool_t kIgnoreRICHBackup;             //! switch to ignore RICH hits for Double_t hit counting
    Bool_t kIgnoreInnerMDCBackup;         //! switch to ignore InnerMDC hits for Double_t hit counting
    Bool_t kIgnoreOuterMDCBackup;         //! switch to ignore OuterMDC hits for Double_t hit counting
    Bool_t kIgnoreMETABackup;             //! switch to ignore META hits for Double_t hit counting
    Bool_t kIgnorePreviousIndexBackup;    //! switch to ignore indices from previoius marked ued objects

    Int_t  fill_Iteration;                 //! remember the number of fill() calls
    Int_t  selectBest_Iteration;           //! remember the number of selectBest() calls
    Int_t  currentEvent;                   //! find new event

    vector < candidate *> all_candidates;  //!

    vector < Int_t > index_RICH;           //!
    vector < Int_t > index_InnerMDC;       //!
    vector < Int_t > index_OuterMDC;       //!
    vector < Int_t > index_SHOWER;         //!
    vector < Int_t > index_TOF;            //!
    vector < Int_t > index_RPC;            //!
    vector < Int_t > old_flags;            //! remember the old flags of HPidTrackCand for restoring

    TString*   nameIndex  ;                //!
    TString*   nameQuality;                //!

    Bool_t     isSimulation;               //!
    TFile*     fout;                       //!
    TNtuple*   nt;                         //!
    HCategory* pPidTrackCandCat;           //! HPidTrackCand category
    HIterator* iterPidTrackCandCat;        //! iterator on HPidTrackCand

    void   printCand        (candidate*, Int_t, TString spacer = "    ");
    static Bool_t cmpIndex  (candidate*, candidate*);
    static Bool_t cmpQuality(candidate*, candidate*);
    Bool_t rejectIndex      (candidate*, HPidTrackSorter::ESwitch, Int_t&);
    Bool_t rejectQuality    (candidate*, HPidTrackSorter::ESwitch);
    Int_t  flagAccepted     (vector<candidate*>&, HPidTrackSorter::ESwitch);
    Int_t  flagDouble       (vector<candidate*>&, HPidTrackSorter::ESwitch);
    Int_t  setFlagsDouble   (vector<candidate*>&, HPidTrackSorter::ESwitch);
    Int_t  clearVector      (vector<candidate*>&);
    Int_t  fillInput        (vector<candidate*>&);
    void   selection(Bool_t (*function)(HPidTrackCand* ));
    Int_t  fillAndSetFlags  ();
    void   clear(void);
public:
    HPidTrackSorter(void);
    HPidTrackSorter(TString name,TString title);
    ~HPidTrackSorter(void);
    Bool_t init     (void);
    Bool_t finalize (void);
    // setup selections
    static void   setDebug              (Bool_t debug = kTRUE)  { kDebug               = debug;}
    static void   setIgnoreRICH         (Bool_t ignore = kTRUE) { kIgnoreRICH          = ignore;}
    static void   setIgnoreInnerMDC     (Bool_t ignore = kTRUE) { kIgnoreInnerMDC      = ignore;}
    static void   setIgnoreOuterMDC     (Bool_t ignore = kTRUE) { kIgnoreOuterMDC      = ignore;}
    static void   setIgnoreMETA         (Bool_t ignore = kTRUE) { kIgnoreMETA          = ignore;}
    static void   setIgnorePreviousIndex(Bool_t ignore = kTRUE) { kIgnorePreviousIndex = ignore;}
    static void   setPrintLevel         (Int_t level)           { printLevel           = level;}
    void          setOutputFile(TString filename = "");

    // control selections
    void          resetFlags(Bool_t flag = kTRUE,Bool_t reject = kTRUE,Bool_t used = kTRUE,Bool_t lepton = kTRUE);
    void          cleanUp(Bool_t final = kTRUE);
    Int_t         fill(Bool_t (*function)(HPidTrackCand* ));
    void          backupFlags();
    Bool_t        restoreFlags();
    Int_t         selectBest(HPidTrackSorter::ESwitch byQuality, Int_t byParticle);
    static void   setRICHMatching(HPidTrackSorter::ERichMatch match, Float_t window = 4.);
    static Bool_t selectLeptons(HPidTrackCand* pcand = 0);
    static Bool_t selectHadrons(HPidTrackCand* pcand = 0);
    void  fillNtuple();
    void  printEvent(TString comment);
    ClassDef(HPidTrackSorter,0);
};
#endif /* !HPidTrackSorter_H */







