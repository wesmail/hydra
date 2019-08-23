#define HPidHistograms_cxx

#include "TROOT.h"
#include "projectPidNtuple.h"

#define KICK 0    // number of momentum reconstruction algorithm (PID convension) 
#define SPLINE 2
#define RUNGEKUTTA 4

//---------------------------------------------------------------

HPidHistograms::HPidHistograms(const char *pFileName): HSUDummyRec(pFileName) {;}

//---------------------------------------------------------------
Bool_t HPidHistograms::init(void)
{
    setInitOk(kFALSE);

	if(openOutFile() == NULL)
          return kFALSE;

    // getting categories
	
    if((pPidTrackCand = getCategory(catPidTrackCand)) == NULL)
    {
        ::Error("init", "Cannot get catPidTrackCand cat");
        goto lab_End;
    }

    if((pitPidTrackCand = getIterator(catPidTrackCand)) == NULL)
    {
        ::Error("init", "Cannot get catPidTrackCand iterator");
        goto lab_End;
    }

    // Ntupla definitions
	
	pTrackCandidate = new TNtuple("pTrackCandidate","Lepton track candidate properties",
				      "pmat:acharge:npad:rcent:beta_k:mom_k:pull:pol_k:mass_k:beta_s:mom_s:chi2_s:pol_s:mass_s:beta_r:mom_r:chi2_r:pol_r:mass_r:maxF:F10:F20:sum0:sum1:sum2:tof:theta:phi:sec:sys:chi2_inmdc:chi2_outmdc:showerId:tofId:richId:kickId:splineId:rungeId:isRichKick:isRichSpline:isRichRungeKutta");
	
	setInitOk();

lab_End:

    return getInitOk();
}

//---------------------------------------------------------------

Bool_t HPidHistograms::finalize(void)
{
	// output file 
	return writeAndCloseOutFile();
}

//---------------------------------------------------------------

Int_t HPidHistograms::execute(void)
{
HPidTrackCandSim *pTrack     = NULL;   // track candidate
HPidHitData *pHitData     = NULL;   // hit data
HPidTrackData *pTrackData = NULL;   // track candidate data

Float_t avrCharge = 0;
Float_t maxF=-1.0, sum0 = -1.0, sum1=-1.0, sum2=-1.0, F10=-1.0, F20=-1.0;
Int_t  isRichKick = 0, isRichSpline = 0, isRichRungeKutta = 0;

pitPidTrackCand->Reset();
while((pTrack = (HPidTrackCandSim*)pitPidTrackCand->Next()) != NULL)
{
	// reset variables
    maxF=-1.0; sum0 = -1.0; sum1=-1.0; sum2=-1.0; F10=-1.0; F20=-1.0;
	isRichKick = 0; isRichSpline = 0; isRichRungeKutta = 0;
	 
    pHitData   = pTrack->getHitData();
    pTrackData = pTrack->getTrackData();

	// only leptons
	if(pHitData->iIndRICH < 0) continue; // broad window from matching unit

	// leptons momentum dependent window flags
    isRichKick = pHitData->hasRingCorrelation[KICK];
    isRichSpline = pHitData->hasRingCorrelation[SPLINE];
    isRichRungeKutta = pHitData->hasRingCorrelation[RUNGEKUTTA];

	// calculate shower observables
    if( pHitData->fShowerSum[0]>0)
    {
       F10 = pHitData->fShowerSum[1]/pHitData->fShowerSum[0];
       F20 = pHitData->fShowerSum[2]/pHitData->fShowerSum[0];
       maxF = TMath::Max(F10,F20);
    }

	// calculate rich observables
    if(pHitData->nRingPadNr > -1)
	avrCharge = ((Float_t)pHitData->nRingAmplitude)/((Float_t)pHitData->nRingPadNr);

	// set table for ntuple
    Float_t varTable[] =
    {
		// rich

		pHitData->nRingPatMat            // pattern matrix
		,avrCharge                       // avarage charge
		,pHitData->nRingPadNr            // pad number
		,pHitData->fRingCentroid         // ring centroid
		
		// kicktrack
		
		,pTrackData->fCorrectedBeta[KICK]   // beta kick
		,pTrackData->fMomenta[KICK]         // momentum kick
		,pTrackData->fPull               // quality parameters (pull) kick
		,pTrackData->nPolarity[KICK]        // charge kick
		,pTrackData->fMassSquared[KICK]     // squered mass kick
		
		//spline
		
		,pTrackData->fCorrectedBeta[SPLINE]   // beta spline
		,pTrackData->fMomenta[SPLINE]         // momentum spline
		,pTrackData->fSplineChiSquare         // quality parameter (chi2) spline
		,pTrackData->nPolarity[SPLINE]        // charge spline
		,pTrackData->fMassSquared[SPLINE]     // squered mass spline
		
		//rungekutta
		
		,pTrackData->fCorrectedBeta[RUNGEKUTTA]   // beta rungekutta
		,pTrackData->fMomenta[RUNGEKUTTA]         // momentum rungekutta
		,pTrackData->fRKChiSquare                 // quality parameter (chi2) runge kutta
		,pTrackData->nPolarity[RUNGEKUTTA]        // charge rungekutta
		,pTrackData->fMassSquared[RUNGEKUTTA]     // squered mass rungekutta
		
		// shower
		
		,maxF                            // maximum multiplication factor
		,F10							 // multiplication factor post1/pre
		,F20                             // multiplication factor post2/pre
		,pHitData->fShowerSum[0]         // sum of charge in pre
		,pHitData->fShowerSum[1] 		 // sum of charge in post1
		,pHitData->fShowerSum[2]         // sum of charge in post2

		// common
		
		,pHitData->getTof()              // time of flight
		,pHitData->fMdcTheta             // theta
		,pHitData->fMdcPhi               // phi
		,pHitData->nSector               // sector
		,pHitData->iSystem               // system
		,pHitData->fInnerMdcChiSquare    // chi2 from inner MDC
		,pHitData->fOuterMdcChiSquare    // chi2 from outer MDC
		
		,pHitData->iIndShower // index of HShowerHitTof object (-1 no object)
		,pHitData->iIndTOF    // index of HTofHit object (-1 no object)
		,pHitData->iIndRICH   // index of HRichHit object (-1 no object) 
		,pTrackData->nKickTrackInd // index of HKickTrack object (-1 no object) 
		,pTrackData->nSplineTrackInd // index of HSplineTrack object (-1 no object) 
		,pTrackData->nRKTrackInd // index of HRKTack  object (-1 no object) 
		,isRichKick // is Rich ring and Kick track correlated in the momentum dependent window (1 yes)
		,isRichSpline // is Rich ring and Spline track correlated in the momentum dependent window (1 yes)
		,isRichRungeKutta //is Rich ring and RK track correlated in the momentum dependent window (1 yes)
    };
    
    pTrackCandidate->Fill(varTable);
}
 return 0;
}
