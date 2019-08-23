//*-- Modified: Stefano Spataro 21/06/2005
//           - bIsAccepted included
//           - TofRec data members included
//*-- Modified: Stefano Spataro 05/08/2005
//           - fPathLength included
//*-- Modified: Stefano Spataro 22/08/2005
//           - covariance matrix included
//*-- Modified: Stefano Spataro 02/09/2005
//           - fPathLength from the new HBaseTrack getTofDist() function
//*-- Modified: AnarRustamov 08/11/2005
//           - Matching quality is added

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
#include "hrktrackB.h"
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
  nRKTrackInd = pMetaMatch->getRungeKuttaInd();
  (pMetaMatch->getSystem()==0) ? fMetaMatchingQuality =pMetaMatch->getQualitySHOWER() : fMetaMatchingQuality =pMetaMatch->getQualityTOF();
  if(pMetaMatch->getKickInd()>-1&& pMetaMatch->isKickAccepted())
    {
      HBaseTrack* pKickTrack = (HBaseTrack*)HPidFL::getKickTrackB(pMetaMatch->getKickInd());
      fillFromBaseTrack(pKickTrack, ALG_KICK);
      nBestMomAlg = ALG_KICK; 
      trackingSuccessfull=kTRUE;
      fPull=HPidFL::getKickTrackB(pMetaMatch->getKickInd())->getPull();
      bIsAccepted[ALG_KICK] = pMetaMatch->isKickAccepted();
    }
  
  if(pMetaMatch->getSplineInd()>-1 && pMetaMatch->isSplineAccepted())
    {
      HBaseTrack* pSplineTrack = (HBaseTrack*)HPidFL::getSplineTrack(pMetaMatch->getSplineInd());
      fillFromBaseTrack(pSplineTrack, ALG_SPLINE);
      nBestMomAlg = ALG_SPLINE;
      trackingSuccessfull=kTRUE;
      fSplineChiSquare=HPidFL::getSplineTrack(pMetaMatch->getSplineInd())->getQSpline();
      bIsAccepted[ALG_SPLINE] = pMetaMatch->isSplineAccepted();
    }
    if( nRKTrackInd >-1 && pMetaMatch->isRungeKuttaAccepted()){
      HBaseTrack* pRKTrack = (HBaseTrack*)HPidFL::getRKTrack(nRKTrackInd);
      fillFromBaseTrack(pRKTrack, ALG_RUNGEKUTTA);
      nBestMomAlg = ALG_RUNGEKUTTA;
      trackingSuccessfull = kTRUE;
      fRKChiSquare = HPidFL::getRKTrack(nRKTrackInd)->getChiq();
      bIsAccepted[ALG_RUNGEKUTTA] = pMetaMatch->isRungeKuttaAccepted();
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
    if ( momAlgIndex==ALG_RUNGEKUTTA ){
        if( nRKTrackInd<0 ){
            ::Error("HPidTrackData::getBaseTrack","No r-k index to base track for this index");
      return NULL;
        }
         else {
            return (HBaseTrack*) HPidFL::getRKTrack(nRKTrackInd);
        }
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
  if(momalg>ALG_RUNGEKUTTA)
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
      fTofRecTof[momalg] = pBaseTrack->getTof();
      fTofRecBeta[momalg] = pBaseTrack->getBeta();
      fTofRecMassSquared[momalg] = pBaseTrack->getMass2();
      fPathLength[momalg] = pBaseTrack->getTofDist();
      cov[momalg].setCov(pBaseTrack->getCovariance());
      qIOMatching[momalg]=pBaseTrack->getIOMatch();
    }
}

// -----------------------------------------------------------------------------

void HPidTrackData::reset(Option_t *)
{
  // Clear all members
  setFiltered(kFALSE);
  setBad(kFALSE);
  
  fMetaMatchingQuality      = -1.0;
  nBestMomAlg               = -1;
  nKickTrackInd             = -1;
  nSplineTrackInd           = -1;
  nKickTrack123Ind          = -1;
  nRefTrackInd              = -1;
  nRKTrackInd               = -1;

  fPull                     = -1.0; //KickPlane-quality parameter
  fSplineChiSquare          = -1.0; //Spline tracking qp
  fRKChiSquare              = -1.0; //R-K tracking quality
  unInnerMdcNumber          = 0;

  for(Int_t i = 0; i < MAX_MDC_SEG; i++)
    {
      aInnerMdcIds[i]       = -1;
      aInnerMdcCounts[i]    = -1;
    }
  
  for(Int_t i = 0; i < MAX_MOM_ALGS; i++)
    {
      nPolarity[i]          = 0;
      fTrackR[i]            = -1.0;
      fTrackZ[i]            = -1.0;
      fMomenta[i]           = -1;
      fMomError[i]          = -1; 
      
      fCorrectedEloss[i]    = -1;
      fCorrectedBeta[i]     = -1; 
      
      fMassSquared[i]       = -1; 
      
      fPathLength[i]        = -1;
 
      bIsAccepted[i]        = kFALSE;
 
      nTofRecFlag[i]        = 0;
      fTofRecTof[i]         = -100.0;
      fTofRecBeta[i]        = -1.0;
      fTofRecMassSquared[i] = -1.0;
      cov[i].Clear();
      qIOMatching[i]=-1.;
    }
  
  setIsCPCandidate(kFALSE);
  fAngleWithClosePairCandidate = -1.0;
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
  
  printf("Kick mass square:     %f\n", fMassSquared[ALG_KICK]);
  printf("Spline mass square:   %f\n", fMassSquared[ALG_SPLINE]);
  printf("Reftrack mass square: %f\n", fMassSquared[ALG_REFT]);

}

void HPidTrackData::Streamer(TBuffer &R__b)
{
   // Stream an object of class HPidTrackData.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      int R__i;
      for (R__i = 0; R__i < 10; R__i++)
         cov[R__i].Streamer(R__b);
      if(R__v>6)
	 {
	    R__b.ReadStaticArray((float*)qIOMatching);
	 }
      else
	 {
	    for(Int_t i=0; i<MAX_MOM_ALGS; i++)
	       qIOMatching[i]=-1.; 
	 }
      R__b >> nBestMomAlg;
      R__b >> nRKTrackInd;
      R__b >> nKickTrackInd;
      R__b >> nKickTrack123Ind;
      R__b >> nRefTrackInd;
      R__b >> nSplineTrackInd;
      R__b >> fMetaMatchingQuality;
      R__b >> bIsClosePairCandidate;
      R__b >> fAngleWithClosePairCandidate;
      R__b.ReadStaticArray((short*)aInnerMdcIds);
      R__b.ReadStaticArray((short*)aInnerMdcCounts);
      R__b >> unInnerMdcNumber;
      R__b >> fPull;
      R__b >> fSplineChiSquare;
      R__b >> fRKChiSquare;
      R__b.ReadStaticArray((short*)nPolarity);
      R__b.ReadStaticArray((float*)fMomenta);
      R__b.ReadStaticArray((float*)fMomError);
      R__b.ReadStaticArray((float*)fTrackR);
      R__b.ReadStaticArray((float*)fTrackZ);
      R__b.ReadStaticArray((float*)fCorrectedEloss);
      R__b.ReadStaticArray((float*)fCorrectedBeta);
      R__b.ReadStaticArray((float*)fPathLength);
      R__b.ReadStaticArray((float*)fMassSquared);
      R__b.ReadStaticArray((bool*)bIsAccepted);
      R__b.ReadStaticArray((short*)nTofRecFlag);
      R__b.ReadStaticArray((float*)fTofRecTof);
      R__b.ReadStaticArray((float*)fTofRecBeta);
      R__b.ReadStaticArray((float*)fTofRecMassSquared);
      R__b.CheckByteCount(R__s, R__c, HPidTrackData::IsA());
   } else {
      R__c = R__b.WriteVersion(HPidTrackData::IsA(), kTRUE);
      TObject::Streamer(R__b);
      int R__i;
      for (R__i = 0; R__i < 10; R__i++)
         cov[R__i].Streamer(R__b);
      R__b.WriteArray(qIOMatching, 10);
      R__b << nBestMomAlg;
      R__b << nRKTrackInd;
      R__b << nKickTrackInd;
      R__b << nKickTrack123Ind;
      R__b << nRefTrackInd;
      R__b << nSplineTrackInd;
      R__b << fMetaMatchingQuality;
      R__b << bIsClosePairCandidate;
      R__b << fAngleWithClosePairCandidate;
      R__b.WriteArray(aInnerMdcIds, 10);
      R__b.WriteArray(aInnerMdcCounts, 10);
      R__b << unInnerMdcNumber;
      R__b << fPull;
      R__b << fSplineChiSquare;
      R__b << fRKChiSquare;
      R__b.WriteArray(nPolarity, 10);
      R__b.WriteArray(fMomenta, 10);
      R__b.WriteArray(fMomError, 10);
      R__b.WriteArray(fTrackR, 10);
      R__b.WriteArray(fTrackZ, 10);
      R__b.WriteArray(fCorrectedEloss, 10);
      R__b.WriteArray(fCorrectedBeta, 10);
      R__b.WriteArray(fPathLength, 10);
      R__b.WriteArray(fMassSquared, 10);
      R__b.WriteArray(bIsAccepted, 10);
      R__b.WriteArray(nTofRecFlag, 10);
      R__b.WriteArray(fTofRecTof, 10);
      R__b.WriteArray(fTofRecBeta, 10);
      R__b.WriteArray(fTofRecMassSquared, 10);
      R__b.SetByteCount(R__c, kTRUE);
   }
}


// -----------------------------------------------------------------------------


