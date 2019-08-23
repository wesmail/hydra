//*-- Author : Tomasz  Wojcik 24/08/2003

//_HADES_CLASS_DESCRIPTION
////////////////////////////////////////////////////////////////////////////////
//
// HPidAlgShower
//
// PreShower algorithms for PID.
// For each track correlated with ring in given window (see hpidtrackfiller)
// probability for given particle species is calculated as a merged value of
// probability for maximum of charge multiplication factors
// max(sum1/sum0,sum2/sum0) and probability for sum of charge (sum0 from
// Pre-chamber)
// for track without correlated ring -1 values are returned
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

#include "hpidhitdata.h"
#include "hpidtrackdata.h"

#include "hshowerhittof.h"
// -----------------------------------------------------------------------------
#define MAX_SUM_0 55
#define MAX_FACTOR 135
#define MIN_FACTOR -40
#define MAX_MOM  1100
//#define  DEBUG_LEVEL 0
ClassImp(HPidAlgShower)

// -----------------------------------------------------------------------------
HPidAlgShower::HPidAlgShower()
                        : HPidAlgorithm("AlgShower", algShower) {
    // Default constructor
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
    m_nMomAlgInd    = -1;
    m_nSector       = -1;
    m_fFactor       = 0.0;
    m_fCharge       = 0.0;
    m_fMom          = 0.0;
    m_pKickTrack    = NULL;
    m_pShowerHitTof = NULL;
    m_pTrackData    = NULL;
    m_pHitData      = NULL;
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
      m_pParams->setContext(pRec->iSelectedMomAlg);
      for(Int_t i = 0; i < pRec->particlesNumber(); i++)
	{
	  Int_t iId = pRec->getParticleId(i);
	  if(iId<0)
	    {
	      ::Error("HPidAlgShower::init()","The Shower cannot provide a PDF for fake particles!");
	      exit(-1);
	    }
	  
	}
      return kTRUE;
    }
}

// -----------------------------------------------------------------------------

Bool_t HPidAlgShower::reinit(void) {
    //Check if parameter context corresponds to the
    //appropriate momentum algorithm index
    //If not -> return kFALSE (analysis can not be done in such case)
    // and print error message
    #warning FIX_ME
  cout << "reinit shower with context number: " << pRec->iSelectedMomAlg;
    return m_pParams->checkContext(pRec->iSelectedMomAlg);
    //return kTRUE;
}

// -----------------------------------------------------------------------------

Bool_t HPidAlgShower::finalize(void) {
// Dummy method

    return kTRUE;
}

// -----------------------------------------------------------------------------

Bool_t HPidAlgShower::calculatePDF(HPidTrackCand *pTrack,
            Float_t afReturn[], Short_t &nCatIndex) {
    // Calculate PDF from data stored in histograms in HPidAlgShowerPar cont.
    // Only track with correlated ring are tested by this algorithm.
    // Thus, shower algorithm is not suitable for hadron identification
    clearMembers();
    m_pTrackData = pTrack->getTrackData();
    m_pHitData   = pTrack->getHitData();
    if( m_pHitData->iSystem!=0 ){
        return kFALSE;  //No track in ShowerTof
    }
    else{
         /*//Loop over all particle species
            for(Int_t i = 0; i < pRec->particlesNumber(); i++){
                    afReturn[i] = 1;
        }*/
        m_nMomAlgInd = pRec->iSelectedMomAlg;
        m_nSector    = m_pHitData->getSector();
        m_fCharge    = m_pTrackData->nPolarity[m_nMomAlgInd];
        m_fMom       = m_pTrackData->fMomenta[m_nMomAlgInd];
        m_fTheta     = m_pHitData->fMdcTheta;
        for(int i=0;i<3;i++){
            m_aSum[i]    = m_pHitData->fShowerSum[i];
        }
       // return kTRUE;
        //Multiplication factor
        //m_fFactor     = TMath::Max(m_aSum[1]/m_aSum[0],m_aSum[2]/m_aSum[0]);
#warning FIX_ME!!!!!!!!!!!
        m_fFactor     = m_aSum[1]+m_aSum[2]-m_aSum[0];
        if(m_fFactor>MAX_FACTOR){
            m_fFactor=MAX_FACTOR;
        }
        if(m_fFactor<MIN_FACTOR){
            m_fFactor=MIN_FACTOR;
        }

        if(m_aSum[0]>MAX_SUM_0){
            m_aSum[0]=MAX_SUM_0;
        }
        if(m_fMom>MAX_MOM){
            m_fMom=MAX_MOM;
        }
        if(m_pHitData->getFlagRICH()==kTRUE &&          //if was ring in RICH
                    m_pHitData->hasRingCorrelation[m_nMomAlgInd]==kTRUE){ 
            if(m_fFactor<0.00001){ //particle didn't pass  the shower detectors
                //can not provide information
                #if DEBUG_LEVEL>1
                    printf("\vSum1==0! \n\n");
                #endif
                //return kFALSE;
            }
            #if DEBUG_LEVEL>1
                printf("\n*************************************************\n");
                printf("Mom: %f,Theta: %f\n",m_fMom,m_fTheta);
                printf("Sum0:%f,Factor:%f\n",m_aSum[0],m_fFactor);
            #endif
            //Loop over all particle species
            for(Int_t i = 0; i < pRec->particlesNumber(); i++){
                m_nId = pRec->getParticleId(i);
                if(m_fCharge != HPidPhysicsConstants::charge(m_nId)) {
                    //Charge from track  differs from correct charge
                    afReturn[i] = 0.0f;
                    #if DEBUG_LEVEL>20
                        printf("Wrong charge. Particle id=%d\n",m_nId);
                    #endif
                    continue;
                }
               //Fakes
                if(HPidPhysicsConstants::isFake(m_nId)){
		  Error("HPidAlgShower::calculatePDF()", "Shower does not provide PDFs for fake particle species!");
		  exit(-1);

/*                  //Negative fake
                    m_nOffset=SUM0_OFFSET;
                    if(m_fCharge<0){
                        afReturn[i] =
                            m_pParams->getValue(-1,
                                        m_nOffset,m_nSector,m_fMom,m_aSum[0]);
                        m_nOffset=MAXF_OFFSET;
                        afReturn[i] *=
                            m_pParams->getValue(-1,
                                        m_nOffset,m_nSector,m_fMom,m_fFactor);
                        continue;
                    }
                    //Positive fake
                    else{
                        afReturn[i]=
                            m_pParams->getValue(-2,
                                        m_nOffset,m_nSector,m_fMom,m_aSum[0]);
                        m_nOffset=MAXF_OFFSET;
                        afReturn[i]*=
                            m_pParams->getValue(-2,
                                        m_nOffset,m_nSector,m_fMom,m_fFactor);
                        continue;
                    }
*/
                }
                #if DEBUG_LEVEL>0
                    Float_t fVal[4]={-1,-1,-1,-1};
                #endif
                //First check sum0, histogram offset is defined in hpidalgshowerpar.h
                m_nOffset=SUM0_OFFSET;
                afReturn[i] =
                   m_pParams->getValue(m_nId,m_nOffset,m_nSector,m_fMom,m_aSum[0]);
                #if DEBUG_LEVEL>0
                    fVal[0]=afReturn[i];
                #endif
                //Next is max factor
                m_nOffset=MAXF_OFFSET;
                afReturn[i] *=
                    m_pParams->getValue(m_nId,m_nOffset,m_nSector,m_fMom,m_fFactor);
                #if DEBUG_LEVEL>0
                    fVal[1] =
                        m_pParams->getValue(m_nId,m_nOffset,m_nSector,m_fMom,m_fFactor);
                #endif
                #if DEBUG_LEVEL>0
                    fVal[2]=afReturn[i];
                    if(afReturn[i]==0){
                        printf("Zero!!! Pid: %2d: pSum: %9.7f, pFact: %9.7f,Merged: %9.7f\n", m_nId,fVal[0],fVal[1],fVal[2]);
                    }
                #endif
                #if DEBUG_LEVEL>0
                    if(afReturn[i]==-1){
                        printf("Negative !!! Pid: %2d: pSum: %9.7f, pFact: %9.7f,Merged: %9.7f\n", m_nId,fVal[0],fVal[1],fVal[2]);
                    }
                #endif
             
             }
       
        }
        else{
            //"No ring, no question
            for(Int_t i = 0; i < pRec->particlesNumber(); i++){
                m_nId = pRec->getParticleId(i);
                afReturn[i]=-1;
            }
        }
    }
    
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
