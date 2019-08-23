#pragma implementation

#include "hpidtrackdata.h"
#include <iostream>
#include "hmetamatch.h"
#include "hbasetrack.h"
#include "hpidfl.h"

#include "hades.h"
#include "hevent.h"
#include "hlinearcategory.h"

#include "hmdctrackgdef.h"
#include "kickdef.h"

#include <TError.h>
#include <TTree.h>
#include "hpidhitdata.h"
#include "hbasetrack.h"
#include "hkicktrack.h"
#include "hkicktrackB.h"
#include "hsplinetrack.h"
// -----------------------------------------------------------------------------

ClassImp(HPidTrackData)
  
  // -----------------------------------------------------------------------------
  
HPidTrackData::HPidTrackData(void)
{
  // Default HPidTrackData ctor.
  
  reset();
}


void HPidTrackData::fillFromMetaMatch(HMetaMatch* pMetaMatch)
{
  reset();
  /*  if(nKickTrackInd>=0 || nRefTrackInd>=0 || nKickTrack123Ind>=0 || nRKTrackInd>=0)
    cout <<"Error 1, we have track indices in the making of the track data set!"<< endl;
  */
  Bool_t trackingSuccessfull=kFALSE;

  //This information does not yet exist
  for (Int_t i =0; i< MAX_MDC_SEG;i++)
    {
      aInnerMdcIds[i]=-1;
      aInnerMdcCounts[i]=-1;
    }
  unInnerMdcNumber=0;

  nKickTrackInd = pMetaMatch->getKickInd();
  nSplineTrackInd = pMetaMatch->getSplineInd();
  nRefTrackInd = pMetaMatch->getRtInd();

  fMetaMatchingQuality = pMetaMatch->getSystem()==0 ? pMetaMatch->getQualitySHOWER() : pMetaMatch->getQualityTOF();
  if(pMetaMatch->getKickInd()>-1)
    {
      HBaseTrack* pKickTrack = (HBaseTrack*)HPidFL::getKickTrackB(pMetaMatch->getKickInd());
      fillFromBaseTrack(pKickTrack, ALG_KICK);
      nBestMomAlg = ALG_KICK; 
      trackingSuccessfull=kTRUE;
      fPull=HPidFL::getKickTrackB(pMetaMatch->getKickInd())->getPull();
    }
  
  if(pMetaMatch->getSplineInd()>-1)
    {
      HBaseTrack* pSplineTrack = (HBaseTrack*)HPidFL::getSplineTrack(pMetaMatch->getSplineInd());
      fillFromBaseTrack(pSplineTrack, ALG_SPLINE);
      nBestMomAlg = ALG_SPLINE;
      trackingSuccessfull=kTRUE;
      fSplineChiSquare=HPidFL::getSplineTrack(pMetaMatch->getSplineInd())->getQSpline();      
    }


}

HBaseTrack* HPidTrackData::getBaseTrack(Int_t momAlgIndex) const
{
  if (momAlgIndex==ALG_KICK)
    {
      if(nKickTrackInd<0)
	{
	  ::Error("HPidTrackData::getBaseTrack","No kick index to base track for this index");
	  return NULL;
	} 
      else return (HBaseTrack*) HPidFL::getKickTrack(nKickTrackInd);
    }

   if (momAlgIndex==ALG_SPLINE)
     {
       if(nSplineTrackInd<0)
	 {
	   ::Error("HPidTrackData::getBaseTrack","No spline index to base track for this index");
	   return NULL;
	 } 
       else return (HBaseTrack*) HPidFL::getSplineTrack(nSplineTrackInd);
     }
   ::Error("HPidTrackData::getBaseTrack","No such algorithm");
   return NULL;
}

void HPidTrackData::fillFromTrackData(HPidTrackData* pSource)
{
  memcpy(this,pSource,sizeof(HPidTrackData));
}

// -----------------------------------------------------------------------------
void HPidTrackData::fillFromBaseTrack(HBaseTrack* pBaseTrack, Int_t momalg)
{
  if(momalg>ALG_REFT)
    {
      ::Error("HPidHitData::fillFromBaseTrack","Algorithm index does not exist");
    }
  else
    {
      fTrackR[momalg] = pBaseTrack->getR();
      fTrackZ[momalg] = pBaseTrack->getZ();
      nPolarity[momalg] = pBaseTrack->getPolarity();
      if(nPolarity[momalg]!=-1 && nPolarity[momalg]!=1) cout << "Warning! undefined polarity!" << endl;
      fMomenta[momalg] = pBaseTrack->getP();
      fCorrectedEloss[momalg] = pBaseTrack->getMetaEloss();
      fCorrectedBeta[momalg] = pBaseTrack->getBeta();
      fMassSquared[momalg] = pBaseTrack->getMass2();

    }
}

// -----------------------------------------------------------------------------

void HPidTrackData::reset(Option_t *)
{
  // Clear all members
  setFiltered(kFALSE);
  setBad(kFALSE);
  
  fMetaMatchingQuality = -1.0;
  nBestMomAlg        = -1;
  nKickTrackInd      = -1;
  nSplineTrackInd    = -1;
  nKickTrack123Ind   = -1;
  nRefTrackInd       = -1;
  nRKTrackInd        = -1;

  fPull              = -1.0; //KickPlane-quality parameter
  fSplineChiSquare   = -1.0; //Spline tracking qp

  unInnerMdcNumber = 0;

  for(Int_t i = 0; i < MAX_MDC_SEG; i++)
    {
      aInnerMdcIds[i]    = -1;
      aInnerMdcCounts[i] = -1;
    }
  
  for(Int_t i = 0; i < MAX_MOM_ALGS; i++)
    {
      nPolarity[i]       = 0;
      fTrackR[i]         = -1.0;
      fTrackZ[i]         = -1.0;
      fMomenta[i]        = -1;
      fMomError[i]       = -1; 
      
      fCorrectedEloss[i] = -1;
      fCorrectedBeta[i]  = -1; 
      
      fMassSquared[i]    = -1;  
    }
  
}

// -----------------------------------------------------------------------------

void HPidTrackData::print(void) const
{
  // Print all fields of the track candidate
  
  UInt_t i;
  
  printf("BestMomAlg                %4d\n", nBestMomAlg);
  printf("KickTrackId:              %4d\n", nKickTrackInd);
  printf("SplineTrackId:            %4d\n", nSplineTrackInd);
  printf("RefTrackId:               %4d\n", nRefTrackInd);
  
  
  printf("InnerMdcIds(%2d):", unInnerMdcNumber);
  for(i = 0; i < unInnerMdcNumber; i++)
    printf(" %4d (%4d)", aInnerMdcIds[i], aInnerMdcCounts[i]);
  
  printf("Kick momentum:     %f\n", fMomenta[ALG_KICK]);
  printf("Spline momentum:   %f\n", fMomenta[ALG_SPLINE]);
  printf("Reftrack momentum: %f\n", fMomenta[ALG_REFT]);
  
  printf("Kick momerror:     %f\n", fMomError[ALG_KICK]);
  printf("Spline momerror:   %f\n", fMomError[ALG_SPLINE]);
  printf("Reftrack momerror: %f\n", fMomError[ALG_REFT]);
  
  printf("Kick mass sqaure:     %f\n", fMassSquared[ALG_KICK]);
  printf("Spline mass square:   %f\n", fMassSquared[ALG_SPLINE]);
  printf("Reftrack mass square: %f\n", fMassSquared[ALG_REFT]);

}

// -----------------------------------------------------------------------------


