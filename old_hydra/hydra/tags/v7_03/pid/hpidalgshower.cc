//*-- Author : Tomasz  Wojcik 24/08/2003

////////////////////////////////////////////////////////////////////////////////
//
// HPidAlgShower
//
// PreShower algorithms for PID.
//
// algShowerTofino - algorithm based on TOFino information 
// algShower - main shower algorithm (default)
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
HPidAlgShower::HPidAlgShower()
                        : HPidAlgorithm("AlgShower", algShower) {
    // Default constructor
    // algShower - main shower algorithm (default)
    clearMembers();
    m_pParams       = NULL;
}


// -----------------------------------------------------------------------------

HPidAlgShower::HPidAlgShower(Float_t fWeight)
                        : HPidAlgorithm("AlgShower", algShower, fWeight) {
// Default constructor with weight
    clearMembers();
    m_pParams       = NULL;
}

// -----------------------------------------------------------------------------

void HPidAlgShower::clearMembers(){
    //Clear variables
    m_nId           = -1;
    m_nOffset       = -1;
    m_fFactor       = 0.0;
    m_fCharge       = 0.0;
    m_fMom          = 0.0;
    m_pKickTrack    = NULL;
    m_pShowerHitTof = NULL;
    m_sDesc         = NULL;
    for(Int_t i=0;i<3;i++){
        m_aSum[i]=-1;
    }
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
    
    clearMembers();

    if(pTrack->getKickTrackId() < 0) {
        return kFALSE; // not a full track
    }
    //Sim or real data
    if(pTrack->isSim()){
        m_pShowerHitTof=(HShowerHitTof*)pTrack->getShowerHitTofTrack();
    }
    else{
        m_pShowerHitTof=pTrack->getShowerHitTof();
    }

    if( (pTrack->getSystem()!=0 ) || m_pShowerHitTof==NULL){
        return kFALSE;  //No track in ShowerTof
    }
    else{
        if(pTrack->getRingId()>=0){ //if was ring in RICH
            if((m_pKickTrack = pTrack->getKickTrack()) == NULL){
                Error("calculatePDF", "Cannot get HKickTrack for the track: %d",
                    pTrack->getKickTrackId());
                return kFALSE;
            }
            m_fCharge     = m_pKickTrack->getCharge();
            m_fMom        = m_pKickTrack->getP();
            m_aSum[0]     = m_pShowerHitTof->getSum(0);
            m_aSum[1]     = m_pShowerHitTof->getSum(1);
            m_aSum[2]     = m_pShowerHitTof->getSum(2);
            m_fFactor     = TMath::Max(m_aSum[1]/m_aSum[0],m_aSum[2]/m_aSum[0]);
            m_fTheta      = TMath::RadToDeg() * m_pKickTrack->getTheta();

            if( m_aSum[1] == 0 ){ //particle didn't pass  the shower detectors
						          //can not provide information
                return kFALSE;
            }
            #if DEBUG_LEVEL>7
                printf("\n*************************************************\n");
                printf("Mom: %f,Theta: %f\n",m_fMom,m_fTheta);
                printf("Sum0:%f,Factor:%f\n",m_aSum[0],m_fFactor);
            #endif
            //Loop over all particle species
            for(Int_t i = 0; i < pRec->particlesNumber(); i++){
                m_nId = pRec->getParticleId(i);
                if(m_fCharge != HPidPhysicsConstants::charge(m_nId)) {
                    //Charge from kicktrack differs from correct charge
                    afReturn[i] = 0.0f;
                    #if DEBUG_LEVEL>6
                        printf("Wrong charge \n");
                    #endif
                    continue;
                }
               //Fakes 
                if(HPidPhysicsConstants::isFake(m_nId)){
                    //Negative fake
                    m_nOffset=SUM0_OFFSET;
                    if(m_fCharge<0){
                        afReturn[i] = 
                            m_pParams->getValue(-1,m_nOffset,m_fMom,m_aSum[0]);
                        m_nOffset=MAXF_OFFSET;
                        afReturn[i] *= 
                            m_pParams->getValue(-1,m_nOffset,m_fMom,m_fFactor);
                        continue;
                    }
                    //Positive fake
                    else{
                        afReturn[i]= 
                            m_pParams->getValue(-2,m_nOffset,m_fMom,m_aSum[0]);
                        m_nOffset=MAXF_OFFSET;
                        afReturn[i]*= 
                            m_pParams->getValue(-2,m_nOffset,m_fMom,m_fFactor);
                        continue;
                    }
                
                }
                #if DEBUG_LEVEL>4
                    Float_t fVal[4]={-1,-1,-1,-1};
                #endif
                //First check sum0, histogram offset is defined in hpidalgshowerpar.h   
                m_nOffset=SUM0_OFFSET;
                afReturn[i] = 
                    m_pParams->getValue(m_nId,m_nOffset,m_fMom,m_aSum[0]);
                #if DEBUG_LEVEL>4
                    fVal[0]=afReturn[i];
                #endif
                //Next is max factor
                m_nOffset=MAXF_OFFSET;
                afReturn[i] *= 
                    m_pParams->getValue(m_nId,m_nOffset,m_fMom,m_fFactor);
                #if DEBUG_LEVEL>4
                    fVal[1] = 
                        m_pParams->getValue(m_nId,m_nOffset,m_fMom,m_fFactor);
                #endif
                #if DEBUG_LEVEL>4
                    fVal[2]=afReturn[i];
                    if(afReturn[i]==0){
                        printf("Pid: %2d: pSum: %9.7f, pFact: %9.7f,Merged: %9.7f\n", m_nId,fVal[0],fVal[1],fVal[2]);
                        printf("Mom: %f,Theta: %f\n",m_fMom,m_fTheta);
                        printf("Sum0:%f,Factor:%f\n",m_aSum[0],m_fFactor);
                        printf("\n*************************************************\n");
                    }
                #endif
            }
        }
        else{
            /*for(Int_t i = 0; i < pRec->particlesNumber(); i++){
                m_nId = pRec->getParticleId(i);
                afReturn[i]=0.0f;
            }*/
        //no rich assiociated with track
        }
    }
    /*
    Bool_t bAllZeros=kTRUE;
    Int_t i;
    Int_t iId;
    for(i = 0; i < pRec->particlesNumber(); i++){
        iId = pRec->getParticleId(i);
        if(afReturn[i]!=0.0) {
            bAllZeros=kFALSE;
        }
    }
    if(bAllZeros){
        for(i = 0; i < pRec->particlesNumber(); i++){
            iId = pRec->getParticleId(i);
            afReturn[i] =-1;
        }
    }
    */
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
    cout<<endl;
}
