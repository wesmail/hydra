// Last update by Thomas Eberl: 04/09/23 14:02:51
//
#ifndef HRICHSELECTOR_H
#define HRICHSELECTOR_H

#include "hreconstructor.h"
#include "TTree.h"
#include "TNtuple.h"
#include "TFile.h"
class HCategory;
class HIterator;
class HHitMatch;
class HDiHitMatch;
class HHitMatchSim;
class HDiHitMatchSim;
class HTrackInfo;
class HRichSelector : public HReconstructor {
public:

	HRichSelector();
        HRichSelector(const Text_t *name,const Text_t *title,const Char_t *swt,const Char_t *kevtType );
	~HRichSelector();

        Bool_t init();
        Bool_t finalize();
        Int_t execute();
    ClassDef(HRichSelector,1) 

private:
    // data members
    HCategory* pHitMatchCat;//!
    HIterator* pHitMatchIter;//!
    HCategory* pDiHitMatchCat;//!
    HIterator* pDiHitMatchIter;//!
    Int_t nEvtsProcessed;//!
    TString *pFileName; //!
    Bool_t evtType; //!
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //
    struct TrackE {
	Float_t recCorr;
	Float_t single;
	Float_t Double;
	Float_t pid;
	Float_t verx;
	Float_t very;
	Float_t verz;
	// reconstructed info
	Float_t goodCorr;
	Float_t goodRing;
	Float_t goodShower;
	Float_t goodBetaMom;
	Float_t recCharge;
	Float_t chi2fit;
	Float_t avgChrg;
	Float_t patmat;
	Float_t nrpads;
	Float_t ampl;
	Float_t centr;
	Float_t mom;
	Float_t tof;
	Float_t beta;
	Float_t sec;
	Float_t ktheta;
	Float_t kphi;
	Float_t rtheta;
	Float_t rphi;
	Float_t metatheta;
	Float_t metaphi;
	Float_t sys;
    };//!
    struct TrackS {
	Float_t truetrack;
	Float_t fake;
	Float_t recCorr;
	Float_t single;
	Float_t Double;
	Float_t nbpart;
	// maxcorrelated particle 
	Float_t pid1;
	Float_t corr1;
	Float_t orig1;
	Float_t cpid1;
	Float_t ctrk1;
	Float_t mech1;
	Float_t med1;
	Float_t gmom1;
	Float_t verx1;
	Float_t very1;
	Float_t verz1;
	// particle 2
	Float_t pid2;
	Float_t corr2;
	Float_t orig2;
	Float_t cpid2;
	Float_t ctrk2;
	Float_t mech2;
	Float_t med2;
	Float_t gmom2;
	Float_t verx2;
	Float_t very2;
	Float_t verz2;
	// reconstructed info
	Float_t goodCorr;
	Float_t goodRing;
	Float_t goodShower;
	Float_t goodBetaMom;
	Float_t recCharge;
	Float_t chi2fit;
	Float_t avgChrg;
	Float_t patmat;
	Float_t nrpads;
	Float_t ampl;
	Float_t centr;
	Float_t mom;
	Float_t tof;
	Float_t beta;
	Float_t sec;
	Float_t ktheta;
	Float_t kphi;
	Float_t metatheta;
	Float_t metaphi;
	Float_t sys;
    };//!
    
    struct PairE {
	Float_t mass;
	Float_t opang;
	Float_t nbDmdc;
	Float_t nbDrich;
	Float_t nbDmeta;
	Float_t goodPair;
	Float_t charge;
	Float_t betadiff;
	Float_t tofdiff;
	Float_t rap;
	Float_t pt;
    };//!
    
    struct PairS {
	Float_t mass;
	Float_t opang;
	Float_t nbDmdc;
	Float_t nbDrich;
	Float_t nbDmeta;
	Float_t goodPair;
	Float_t recCharge;
	Float_t betadiff;
	Float_t tofdiff;
	// sim
	Float_t orig;//0 conv/1 pidal/2 etadal
	Float_t truepair;//y/n
	Float_t fake;//code 0:trueCB, 1:fakeCB
	Float_t faketype;//0 e / 1 pion / 2 proton
	Float_t med;
	Float_t charge;
	Float_t rap;
	Float_t pt;
    };//!
    


    //
    TFile *pFileOut;//!
    TTree *outTreeTracksRings; //!
    TTree *outTreePairsTracksRings; //!
    TTree *outTreeTracks; //!
    TTree *outTreePairs; //!
    TTree *outTreePairsTracks; //!
    TNtuple* outNtupleS; //!
    TNtuple* outNtuplePairsS; //!
    TNtuple* outNtupleE; //!
    TNtuple* outNtuplePairsE; //!
    HHitMatch *track;//!
    HHitMatchSim *tracksim;//!
    HDiHitMatch *pair;//!
    HDiHitMatchSim *pairsim;//!
    // member functions
    void classifyTrackS(HHitMatchSim*,TrackS&);
    void resetTrackS(TrackS&);
    void resetPairS(PairS&);
    void classifyTrackE(HHitMatch*,TrackE&);
    void resetTrackE(TrackE&);
    void resetPairE(PairE&);
    Int_t getSingleParticleIndexFromTrackInfo(HTrackInfo*);
    Float_t getMaxRichWeight(HTrackInfo*);
    Float_t getMaxMdcWeight(HTrackInfo*);
    Bool_t isGoodPair(HDiHitMatch*);
    Bool_t isGoodPairWithGoodTracks(HDiHitMatch*);
    Bool_t isGoodTrack(HHitMatch*);
    Bool_t isGoodRing(HHitMatch*);
    Bool_t isBetaMom3s(HHitMatch*);
    Bool_t isGoodShower(HHitMatch*);
    Bool_t isGoodCorrelation(HHitMatch*);
    Bool_t isSingleTrack(HHitMatchSim*);
    UInt_t getDownscalingFlag();
    UInt_t getDownscaling();
    UInt_t getTriggerDecision();
    UInt_t getSeqNumber();                        
    Int_t getRecCorrCode(HHitMatch*);
    HHitMatch* get1stTrackE(HDiHitMatch*);
    HHitMatch* get2ndTrackE(HDiHitMatch*);
    HHitMatchSim* get1stTrackS(HDiHitMatchSim*);
    HHitMatchSim* get2ndTrackS(HDiHitMatchSim*);
    Bool_t fillTrackNtupleS();
    Bool_t fillNtupleS(HHitMatchSim*);
    Bool_t fillTrackNtupleE();
    Bool_t fillNtupleE(HHitMatch*);
    Bool_t fillPairNtupleS();
    Bool_t fillNtuplePairS(HDiHitMatchSim*);
    Bool_t fillPairNtupleE();
    Bool_t fillNtuplePairE(HDiHitMatch*);
    Bool_t selectTracksE();
    Bool_t selectTracksS();
    Bool_t selectPairsE();
    Bool_t selectPairsS();
    Bool_t selectRingsInTracksE();
    Bool_t selectRingsInTracksS();
    Bool_t selectRingsInTracksInPairsE();
    Bool_t selectRingsInTracksInPairsS();
    Bool_t selectTracksInPairsE();
    Bool_t selectTracksInPairsS();
};

#endif // HRICHSELECTOR_H
