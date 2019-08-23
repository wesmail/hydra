//*-- Author : Tomasz  Wojcik 24/08/2003

////////////////////////////////////////////////////////////////////////////////
//
// HPidAlgShower
//
// PreShower algorithms for PID.
// 
// algShower - main shower algorithm (default)
// algShowerTofino - algorithm based on TOFino information  
//     
////////////////////////////////////////////////////////////////////////////////

#pragma implementation

#include "TH2.h"
#include "hpidalgshower.h"
#include "hpidtrackcand.h"
#include "hpidreconstructor.h"

#include "hades.h"
#include "hruntimedb.h"

#include "hpidalgshowerpar.h"
#include "hpidphysicsconstants.h"

#include "hpidgaussconf.h"

#include <hkicktrack.h>

#include "hshowerhittof.h"
// -----------------------------------------------------------------------------

ClassImp(HPidAlgShower)

// -----------------------------------------------------------------------------

HPidAlgShower::HPidAlgShower(void)
                        : HPidAlgorithm("AlgShower", algShower) {
	m_algType=algShower;
    m_nMult=1;
// Default constructor
}
// -----------------------------------------------------------------------------
HPidAlgShower::HPidAlgShower(EnumPidAlgorithm_t algType,Int_t nMult)
                        : HPidAlgorithm("AlgShower", algType) {
	//  constructor with algorithm type:
	// algShower - main shower algorithm (default)
	// algShowerTofino - algorithm based on TOF
    // nMult - Tofino multiplicity (=1 or >1) 
	m_algType=algType;
    m_nMult = nMult;
}


// -----------------------------------------------------------------------------

HPidAlgShower::HPidAlgShower(Float_t fWeight)
                        : HPidAlgorithm("AlgShower", algShower, fWeight) {
// Default constructor with weight
	m_algType=algShower;
    m_nMult=1;
}

// -----------------------------------------------------------------------------
HPidAlgShower::HPidAlgShower(EnumPidAlgorithm_t algType, Float_t fWeight,Int_t nMult)
                        : HPidAlgorithm("AlgShower", algType, fWeight){
	// constructor with weight and algorithm type
	m_algType=algShower;
    m_nMult = nMult;
}

// -----------------------------------------------------------------------------


Bool_t HPidAlgShower::init(void) {
// Get pointer to the parameter's container

    if((pParams = (HPidAlgShowerPar *)gHades->getRuntimeDb()
                        ->getContainer(PIDALGSHOWERPAR_NAME)) == NULL)
    {
        Error("init", "Cannot get parameters: %s", PIDALGSHOWERPAR_NAME);
        return kFALSE;
    }

    return kTRUE;
}

// -----------------------------------------------------------------------------

Bool_t HPidAlgShower::reinit(void) {
// Dummy method

    return kTRUE;
}

// -----------------------------------------------------------------------------

Bool_t HPidAlgShower::finalize(void) {
// Dummy method

    return kTRUE;
}

// -----------------------------------------------------------------------------

Bool_t HPidAlgShower::calculatePDF(HPidTrackCand *pTrack,
            Float_t afReturn[], Short_t &nCatIndex) {
	// Calculate PDF from data stored in histograms from  HPidAlgShowerPar
	HKickTrack *pKickTrack=NULL;
	HShowerHitTof *pShowerHitTof=NULL;
	Int_t   nId=-1;
	Int_t   nOffset=0;
	Int_t 	nTofMult=0;
	Float_t fMom=0.0;
	Float_t fFactor=0.0;
	Float_t fSum0=0.0;
    Float_t fTheta=0.0;	
    Float_t fTof=0.0;	

	if(pTrack->getKickTrackId() < 0) return kFALSE; // not a full track
	if((pTrack->getSystem()!=0 ) || 
			                (pShowerHitTof=pTrack->getShowerHitTof())==NULL){
		return kFALSE;	//No track in ShowerTof
	}
	else{
		if((pKickTrack = pTrack->getKickTrack()) == NULL){
      	    Error("calculatePDF", "Cannot get HKickTrack for the track: %d",
                    pTrack->getKickTrackId());
            return kFALSE;
		}
		nTofMult    = pShowerHitTof->getTofinoMult();
        //if required m_nMult=1 skip all other events
        if( m_algType == algShowerTofino && m_nMult==1 && nTofMult!=1){
            return kFALSE;
        }
		fTof		= pShowerHitTof->getTof();
		fMom		= pKickTrack->getP();
		fSum0		= pShowerHitTof->getSum(0);
		fFactor     = TMath::Max( pShowerHitTof->getSum(1)/fSum0,
								  pShowerHitTof->getSum(2)/fSum0);
        if( fFactor == 0. ){ //particle did pass  shower detectors
            return kFALSE;
        }
        fTheta      = TMath::RadToDeg() * pKickTrack->getTheta();
        //Loop over all particles
		for(Int_t i = 0; i < pRec->particlesNumber(); i++){
			nId = pRec->getParticleId(i);
			//First check sum0, histogram offset is 0
			nOffset=0;
			afReturn[i] = pParams->getValue(nId,nOffset,fSum0,fMom,fTheta);
#if DEBUG_LEVEL>4
            cout<<"Pid "<<nId<<endl;
            cout<<"Sum0 Value :"<<afReturn[i];
#endif
			//Next is max factor
			nOffset=1;
			afReturn[i]*=pParams->getValue(nId,nOffset,fFactor,fMom,fTheta);
#if DEBUG_LEVEL>4
            cout<<" Max Value :"<<afReturn[i];
#endif
			//If required TOFino help
			if(m_algType == algShowerTofino && nTofMult==1){
				nOffset=2;
				afReturn[i]*=pParams->getValue(nId,nOffset,fTof,fMom,fTheta);
#if DEBUG_LEVEL>4
            cout<<" Tof Value :"<<afReturn[i];
#endif
			}
			else if ( m_algType == algShowerTofino && nTofMult>1 ){
				nOffset=3;
				afReturn[i]*=pParams->getValue(nId,nOffset,fTof,fMom,fTheta);
#if DEBUG_LEVEL>4
            cout<<" Tof Mult Value :"<<afReturn[i];
#endif
			}
#if DEBUG_LEVEL>4
            cout<<"\nMom: "<<fMom<<" Theta: "<<fTheta<<" Value: "<<afReturn[i]<<endl;
#endif
		}	
	}	
	 	return kTRUE;
}

// -----------------------------------------------------------------------------

Bool_t HPidAlgShower::calculateCL(HPidTrackCand *pTrack,
            Float_t afReturn[], Short_t &nCatIndex) {
	// No CL in this case
    return kTRUE;
}

// -----------------------------------------------------------------------------

void HPidAlgShower::print(void) const {
    cout<<"Shower PID algorithm"<<endl;
    cout<<"Type: ";
    if ( m_algType == algShowerTofino ){
        cout<<"ShowerTofino";
        if (m_nMult==1)
            cout<<", Multiplicity = 1";
        else
            cout<<", Multiplicity > 1";
    }
    else 
        cout<<"Shower";
    cout<<endl;
}
