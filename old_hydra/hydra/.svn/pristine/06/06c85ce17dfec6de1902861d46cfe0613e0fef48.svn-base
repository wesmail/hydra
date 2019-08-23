//*-- Author : Tomasz  Wojcik 24/08/2003

////////////////////////////////////////////////////////////////////////////////
//
// HPidAlgShower
//
// PreShower algorithms for PID.
//
// algShowerTofino - algorithm based on TOFino information (default)
// algShower - main shower algorithm
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
HPidAlgShower::HPidAlgShower(EnumPidAlgorithm_t algType,Int_t nMult)
                        : HPidAlgorithm("AlgShower", algType) {
    // Default constructor
    //  constructor with algorithm type:
    // algShower - main shower algorithm
    // algShowerTofino - algorithm based on TOFino+Shower (default)
    // nMult - Tofino multiplicity (=1 or >1)
    m_algType=algType;
    m_nMult = nMult;
}


// -----------------------------------------------------------------------------

HPidAlgShower::HPidAlgShower(Float_t fWeight)
                        : HPidAlgorithm("AlgShower", algShowerTofino, fWeight) {
// Default constructor with weight
    m_algType=algShowerTofino;
    m_nMult=1;
}

// -----------------------------------------------------------------------------
HPidAlgShower::HPidAlgShower(EnumPidAlgorithm_t algType, Float_t fWeight,Int_t nMult)
                        : HPidAlgorithm("AlgShower", algType, fWeight){
    // constructor with weight and algorithm type
    m_algType=algType;
    m_nMult = nMult;
}

// -----------------------------------------------------------------------------


Bool_t HPidAlgShower::init(void) {
// Get pointer to the parameter's container

    if((m_pParams = (HPidAlgShowerPar *)gHades->getRuntimeDb()
                        ->getContainer(PIDALGSHOWERPAR_NAME)) == NULL) {
        Error("init", "Cannot get parameters: %s", PIDALGSHOWERPAR_NAME);
        return kFALSE;
    }
    else{
        return kTRUE;
    }
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
    Int_t   nTofMult=0;
    Float_t fMom=0.0;
    Float_t fFactor=0.0;
    Float_t fSum0=0.0;
    Float_t fSum1=0.0;
    Float_t fSum2=0.0;
    Float_t fTheta=0.0;
    Float_t fTof=0.0;
    Float_t fCharge = 0.0;
    Float_t aRichLeptonFraction[10] =
                {0.46,0.76,0.73,0.69,0.65,0.59,0.53,0.4,0.23,0.04};
    Float_t aRichHadronFraction[10] =
                {0.0065,0.0031,0.0018,0.0016,0.0016,0.0016,0.0016,0.0018,0.0018,0.0003};

    if(pTrack->getKickTrackId() < 0) return kFALSE; // not a full track
    if(pTrack->isSim()){
        pShowerHitTof=(HShowerHitTof*)pTrack->getShowerHitTofTrack();
    }
    else{
        pShowerHitTof=pTrack->getShowerHitTof();
    }
    if( (pTrack->getSystem()!=0 ) || pShowerHitTof==NULL){
        return kFALSE;  //No track in ShowerTof
    }
    else{
        if(pTrack->getRingId()>=0){ //if was ring in RICH
            if((pKickTrack = pTrack->getKickTrack()) == NULL){
                Error("calculatePDF", "Cannot get HKickTrack for the track: %d",
                    pTrack->getKickTrackId());
                return kFALSE;
            }
            nTofMult    = pShowerHitTof->getTofinoMult();
            //if required m_nMult=1 skip all other calculation
            if( m_algType == algShowerTofino && m_nMult==1 && nTofMult!=1){
                return kFALSE;
            }
            fCharge     = pKickTrack->getCharge();
            fTof        = pShowerHitTof->getTof();
            fMom        = pKickTrack->getP();
            fSum0       = pShowerHitTof->getSum(0);
            fSum1       = pShowerHitTof->getSum(1);
            fSum2       = pShowerHitTof->getSum(2);
            fFactor     = TMath::Max( fSum1/fSum0, fSum2/fSum0);
            fTheta      = TMath::RadToDeg() * pKickTrack->getTheta();

            if( fSum1 == 0 ){ //particle didn't pass  the shower detectors
						      //can not provide information
                return kFALSE;
            }
            #if DEBUG_LEVEL>4
                printf("\n*****************************************************\n");
                printf("Mom: %f, Theta: %f, Mult: %d\n",fMom,fTheta,nTofMult);
                printf("Sum0: %f, Factor: %f, Tof: %f\n",fSum0,fFactor,fTof);
            #endif
            //Loop over all particles
            for(Int_t i = 0; i < pRec->particlesNumber(); i++){

                nId = pRec->getParticleId(i);
                if(fCharge != HPidPhysicsConstants::charge(nId)) {
                    //Charge from kicktrackdiffers from correct charge
                    afReturn[i] = 0.0f;
                    #if DEBUG_LEVEL>6
                        printf("Wrong charge \n");
                    #endif
                    continue;
                }
                #if DEBUG_LEVEL>4
                    Float_t fVal[4]={-1,-1,-1,-1};
                #endif
                //First check sum0, histogram offset is 0
                nOffset=0;
                afReturn[i] = 100*m_pParams->getValue(nId,nOffset,fSum0,fMom,0);
                #if DEBUG_LEVEL>4
                    fVal[0]=afReturn[i];
                #endif
                //First check sum0, histogram offset is 0

                //Next is max factor
                nOffset=1;
                afReturn[i]*=100*m_pParams->getValue(nId,nOffset,fFactor,fMom,0);
                #if DEBUG_LEVEL>4
                    fVal[1]=100*m_pParams->getValue(nId,nOffset,fFactor,fMom,0);
                #endif
                //If required TOFino help
                //Mult=1
                afReturn[i]=1;
                if(m_algType == algShowerTofino && nTofMult==1){
                    nOffset=2;
                    afReturn[i]*=100*m_pParams->getValue(nId,nOffset,fTof,fMom,0);
                    #if DEBUG_LEVEL>4
                        fVal[2]=100*m_pParams->getValue(nId,nOffset,fTof,fMom,0);
                    #endif
                }
                //Mult > 1
                else if ( m_algType == algShowerTofino && nTofMult>1 ){
                    nOffset=3;
                    afReturn[i]*=100*m_pParams->getValue(nId,nOffset,fTof,fMom,0);
                    #if DEBUG_LEVEL>4
                        fVal[2]=100*m_pParams->getValue(nId,nOffset,fTof,fMom,0);
                    #endif
                }
                //Fraction Correction:
                Int_t nIndx=0;
                if( fMom<2000){
                    nIndx = (Int_t)fMom/200;
                }
                else{
                    nIndx = 9;
                }
                if( nId==2 || nId==3 )
                    afReturn[i] *= aRichLeptonFraction[nIndx];
                else{
                 afReturn[i] *= aRichHadronFraction[nIndx];
                }
            }
        }
        else{
        //
        }
        #if DEBUG_LEVEL>4
            fVal[3]=afReturn[i];
            printf("Pid: %2d: pSum: %9.7f, pFact: %9.7f, pTof: %9.7f, Merged: %9.7f\n", nId,fVal[0],fVal[1],fVal[2],fVal[3]);
        #endif
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
