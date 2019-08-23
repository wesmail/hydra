#ifndef HPIDEFFICIENCYCALCULATION_H
#define HPIDEFFICIENCYCALCULATION_H

#include "hsudummyrec.h"
#include "TString.h"
#define MAX_VARS 200 
#define EFF_NTUPLE_VARS_1 "E_track_number:E_parent_track_number:E_geant_id"
#define EFF_NTUPLE_VARS_2 "E_medium:E_mechanism:E_isPartOfATrack:E_isFullyReconstructed"
#define EFF_NTUPLE_VARS_3 "E_system:E_charge:E_rec_mom:E_beta:E_sector:E_theta:E_phi"
#define EFF_NTUPLE_VARS_4 "E_rec_mom_kick:E_rec_mom_spline:E_rec_mom_rk:E_geant_mom"
#define EFF_NTUPLE_VARS_5 "E_geant_flag:E_inner_chi2:E_outer_chi2:E_rich_corr_kick"
#define EFF_NTUPLE_VARS_6 "E_rich_corr_spline:E_rich_corr_rk:E_rich_flag:E_rec_id:E_isIdentified"
#define EFF_NTUPLE_VARS_7 "E_geant_mom_x:E_geant_mom_y:E_geant_mom_z"
#define EFF_NTUPLE_VARS_8 "E_isAccepted:E_isCloseToOtherParticle:E_isEmbeddedTrack:E_geant_theta:E_geant_phi"
#define EFF_NTUPLE_VARS_9 "E_firstShowerIndex:E_firstTofIndex:E_isCloseToEmbeddedParticle:E_isRemovedByActivePairCut"
#define EFF_NTUPLE_VARS_10 "E_isLegOfPair:E_isLegOfSurvivingPair:E_innerSegmentIndex"
#define EFF_NTUPLE_VARS_11 "E_ringIndex:E_metaIndex:E_PidTrackCandIndex:E_RKIndex:E_KickIndex:E_matchIndex:E_isRemovedByCPCandidateCutH:E_isRemovedByCPCandidateCutL:E_outerSegmentIndex"

class TNtuple;
class HIterator;
class HCategory;
class HGeantKine;
class HPidTrackCand;
class HPidCandidate;
class HPidParticleSim;
class HPidParticle;
class HPidGeantTrackSet;
class HPairSim;
class HShowerHitTof;
class HTofHit;
class HKickTrack;
class HPidHitData;
class HPidTrackData;

class HPidEfficiencyCalculation : public HSUDummyRec{
    public:
        HPidEfficiencyCalculation(Int_t SELECTED_MOMENTUM_ALG,				 
				  Int_t o_sourceID, Int_t o_targetmedium,
				  const Char_t *pOutFileName="eff_ntuple_out.root");
        ~HPidEfficiencyCalculation();
        virtual Bool_t init(void);
        virtual Int_t  execute(void);
        virtual Bool_t finalize(void);
	Int_t checkGeantTrackSet(const HPidGeantTrackSet* pGTS);
	static Bool_t isHiddenLepton(const HPidGeantTrackSet* pPart);
    private:
        void clearMembers();
	void resetNtuple();

	Bool_t isInAcceptance(HGeantKine* pKine);
	Bool_t isCloseToOtherParticle(HGeantKine* pKine, Bool_t b_UseEmbeddedParticlesOnly=kFALSE);
	Bool_t isCloseToEmbeddedParticle(HGeantKine* pKine);

	Bool_t isEmbeddedTrack(HGeantKine* pKine);
	Bool_t isEmbeddedTrack(Int_t geantTrackNumber);
	Bool_t isBackgroundTrack(Int_t geantTrackNumber);
	Int_t  findCorrespondingMDCSegment(Int_t geantTrackNumber,Bool_t bInnerSeg);
	Int_t  findCorrespondingMETAHit(Int_t geantTrackNumber);
	Int_t  findCorrespondingRichRing(Int_t geantTrackNumber);
	Bool_t isDileptonPair(HPairSim* pPairSim);
	Bool_t isRemovedByActivePairCut(HGeantKine* pKine);
	Int_t isPairedWithBackgroundTrack(HPairSim* pPair, HGeantKine* pKine);
	Int_t isPairedWithBackgroundLepton(HPairSim* pPair, HGeantKine* pKine);
	Float_t calcOpeningAngleKine(HGeantKine *kine1,HGeantKine *kine2);
	
	Bool_t isSeenInSingleDetector(Int_t correlationFlag);	
	Bool_t isRemovedByCPCandidateCut(HPidParticle* pPart,Bool_t bSelectLepton);

        HIterator *pItPart;            //iterator over HPidParticleSimExtHit
        HIterator *pItGeant;           //iterator over HGeantKine
        HIterator *pItGeant1;          //iterator over HGeantKine
        HIterator *pItPairs;           //iterator over HPairCategory
        HIterator *pItMdcTrkCand;      //iterator over HMdcTrkCandCategory
        HIterator *pItMetaMatch;       //iterator over HMetaMatchCategory
        HIterator *pItPidTrackCand;    //iterator over HPidTrackCandCategory
        HIterator *pItMdcSeg;          //iterator over HMdcSegmentCategory
        HIterator *pItRichHit;         //iterator over HRichHit
	HIterator *pItTOFHit;          //iterator over HTOFHit
        HIterator *pItShowerHitTof;    //iterator over HShowerHitTof
	HIterator *pItMdcGeant;        //iterator over mdc hits in different chambers
	HIterator *pItTOFGeant;        //iterator over hits in tof
	HIterator *pItShowerGeant;     //iterator over hits in shower

        TNtuple *effNtuple;            //output ntuple
        TString sOutFileName;          //ouput file name 

	//Creating an efficiency requires a choice about a specific momentum algorithm, a specific source of simulated particles
	//and possibly about the medium-number of the target if sources without embedded objects are used
	Int_t selectedMomAlg;
	Int_t sourceID;
	Int_t targetmedium;

        //Ntuple to store all information about the particle's history
        Float_t eNtupleVars[MAX_VARS];

        enum enumLeaves{
	  E_track_number,E_parent_track_number,E_geant_id,
	  E_medium,E_mechanism,E_isPartOfATrack,E_isFullyReconstructed,
	  E_system, E_charge, E_rec_mom, E_beta, E_sector, E_theta, E_phi, 
	  E_rec_mom_kick, E_rec_mom_spline, E_rec_mom_rk, E_geant_mom,
	  E_geant_flag, E_inner_chi2, E_outer_chi2, E_rich_corr_kick,
	  E_rich_corr_spline, E_rich_corr_rk, E_rich_flag, E_rec_id, E_isIdentified,
	  E_geant_mom_x,E_geant_mom_y,E_geant_mom_z,
	  E_isAccepted, E_isCloseToOtherParticle, E_isEmbeddedTrack,
	  E_geant_theta, E_geant_phi, E_firstShowerIndex, E_firstTofIndex, E_isCloseToEmbeddedParticle,
	  E_isRemovedByActivePairCut, E_isLegOfPair, E_isLegOfSurvivingPair,E_innerSegmentIndex,
	  E_ringIndex,E_metaIndex,E_PidTrackCandIndex,E_RKIndex,E_KickIndex,E_matchIndex,
	  E_isRemovedByCPCandidateCutH,E_isRemovedByCPCandidateCutL,E_outerSegmentIndex
        };
	
	ClassDef(HPidEfficiencyCalculation,0)
	  };
#endif /*!HPIDEFFICIENCYCALCULATION_H*/

