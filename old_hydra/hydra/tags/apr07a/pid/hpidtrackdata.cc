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
//*-- Modified: TassiloChrst 05/02/2005
//           - Upgraded code documentation

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
#include "hmdcsizescells.h"

#include <TError.h>
#include <TTree.h>
#include "hpidhitdata.h"
#include "hbasetrack.h"
#include "hkicktrack.h"
#include "hkicktrackB.h"
#include "hsplinetrack.h"
#include "hrktrackB.h"
#include "hshowerhittof.h"
#include "hshowerhittoftrack.h"
#include "htofhit.h"
#include "htofcluster.h"
// -----------------------------------------------------------------------------

ClassImp(HPidTrackData)
  
  // -----------------------------------------------------------------------------
  
HPidTrackData::HPidTrackData(void)
{
  // Default HPidTrackData ctor.
  //cout << "Calling Clear in track data constructor" << endl;
  reset();
}

HPidTrackData::HPidTrackData(const HPidTrackData& source):TObject(source)
{
  reset();
  nBestMomAlg = source.getBestMomAlg();

  nRKTrackInd = source.getRKTrackInd();
  nKickTrackInd = source.getKickTrackInd();
  nKickTrack123Ind = source.getKickTrack123Ind();
  nRefTrackInd = source.getRefTrackInd();
  nSplineTrackInd = source.getSplineTrackInd();

  fMetaMatchingQuality = source.getMetaMatchingQuality();

  nCloseTracklets=0;

  //cout << "Source claims to have: " << source.getNCloseTracklets() << " neighbours " << endl;
  //Int_t a;
  //cin >> a;
  for(Int_t i=0; i< source.getNCloseTracklets(); i++)
    {
      //cout << "Source claims to have: " << nCloseTracklets << " neighbours " << endl;
      //cout << "Searching for " << i << endl;
      appendCloseTracklet(source.getTrackletDistance(i));
    }
  
  fRKPhi = source.getRKPhi();
  fRKTheta = source.getRKTheta(); 

  dxRkMeta = source.getRkMetadx();
  dyRkMeta = source.getRkMetady();
  dzRkMeta = source.getRkMetadz();

  dxMdcMeta = source.getMdcMetadx();
  dyMdcMeta = source.getMdcMetady();
  dzMdcMeta = source.getMdcMetadz();

  xMeta = source.getxMeta();
  yMeta = source.getyMeta();
  zMeta = source.getzMeta();
  
  errXMeta = source.geterrXMeta();
  errYMeta = source.geterrYMeta();
  errZMeta = source.geterrZMeta();

  //unInnerMdcNumber = source.getInnerMdcNumber();

  fPull = source.getPull();
  fSplineChiSquare = source.getSplineChiSquare();
  fRKChiSquare = source.getRKChiSquare();


  for(Int_t i = 0; i < MAX_MOM_ALGS; i++)
    {
      qIOMatching[i] = source.getIOMatching(i);
      
      nPolarity[i] = source.getPolarity(i);
      
      fMomenta[i] = source.getMomenta(i);
      fMomError[i] = source.getMomError(i);

      fTrackR[i] = source.getTrackR(i);
      fTrackZ[i] = source.getTrackZ(i);
      
      fCorrectedEloss[i] = source.getCorrectedEloss(i);
      fCorrectedBeta[i] = source.getCorrectedBeta(i); 
      
      fPathLength[i] = source.getPathLength(i);

      fMassSquared[i] = source.getMassSquared(i);
 
      bIsAccepted[i] = source.getIsAccepted(i);
 
      nTofRecFlag[i] = source.getTofRecFlag(i);
      fTofRecTof[i] = source.getTofRecTof(i);
      fTofRecBeta[i] = source.getTofRecBeta(i);
      fTofRecMassSquared[i] = source.getTofRecMassSquared(i);
      cov[i].setCov((HSymMat&)source.cov[i]);

    }
  
}

void HPidTrackData::fillFromMetaMatch(HMetaMatch* pMetaMatch)
{
  //cout << "building up track data" << endl;
  reset();
  HShowerHitTof* pShowerHitTof;
  HTofHit* pTofHit;
  Bool_t trackingSuccessfull=kFALSE; //There was no track information yet that told us that ANY tracking regarded this a good track


  //collect track indices from meta match object
  nKickTrackInd = pMetaMatch->getKickInd();
  nSplineTrackInd = pMetaMatch->getSplineInd();
  nRefTrackInd = pMetaMatch->getRtInd();
  nRKTrackInd = pMetaMatch->getRungeKuttaInd();

  //If this was an HMetaMatch in Shower/TOF we get the shower/tof matching quality
  if(pMetaMatch->getSystem()==0)
    {
      fMetaMatchingQuality =pMetaMatch->getQualitySHOWER();
      dxMdcMeta=pMetaMatch->getdxMeta();
      dyMdcMeta=pMetaMatch->getdyMeta();
    }
  if(pMetaMatch->getSystem()==1)
    {
      fMetaMatchingQuality =pMetaMatch->getQualityTOF();
      dxMdcMeta=pMetaMatch->getdxMeta();
      dyMdcMeta=pMetaMatch->getdyMeta();
    }

  //For each successful tracking we store the relevant track parameters!
  if(pMetaMatch->getKickInd()>-1 && pMetaMatch->isKickAccepted()) //Kickplane likes this candidate
    {
      HBaseTrack* pKickTrack = (HBaseTrack*)HPidFL::getKickTrackB(pMetaMatch->getKickInd());
      fillFromBaseTrack(pKickTrack, ALG_KICK);  //Get all kicktrack information and store it in this object
      nBestMomAlg = ALG_KICK; 
      trackingSuccessfull=kTRUE;
      fPull=HPidFL::getKickTrackB(pMetaMatch->getKickInd())->getPull();
      bIsAccepted[ALG_KICK] = pMetaMatch->isKickAccepted();
    }
  
  if(pMetaMatch->getSplineInd()>-1 && pMetaMatch->isSplineAccepted()) //Spline likes this candidate
    {
      HBaseTrack* pSplineTrack = (HBaseTrack*)HPidFL::getSplineTrack(pMetaMatch->getSplineInd());
      fillFromBaseTrack(pSplineTrack, ALG_SPLINE);
      nBestMomAlg = ALG_SPLINE;
      trackingSuccessfull=kTRUE;
      fSplineChiSquare=HPidFL::getSplineTrack(pMetaMatch->getSplineInd())->getQSpline();
      bIsAccepted[ALG_SPLINE] = pMetaMatch->isSplineAccepted();
    }

  if( nRKTrackInd >-1 && pMetaMatch->isRungeKuttaAccepted() && pMetaMatch->isSplineAccepted())
    { //Both Spline AND RK like this guy
      HRKTrackB* pRKTrack = HPidFL::getRKTrack(nRKTrackInd);
      fillFromBaseTrack(pRKTrack, ALG_RUNGEKUTTA);
      nBestMomAlg = ALG_RUNGEKUTTA;
      trackingSuccessfull = kTRUE;
      fRKChiSquare = HPidFL::getRKTrack(nRKTrackInd)->getChiq();
      
      fRKPhi = pRKTrack->getPhiSeg1RK()*TMath::RadToDeg()+60*pRKTrack->getSector();
      fRKPhi = fmod(fRKPhi,(Float_t)360.);
      fRKTheta = pRKTrack->getThetaSeg1RK()*TMath::RadToDeg();
      fTrackR[ALG_RUNGEKUTTA] = pRKTrack->getRSeg1RK();
      fTrackZ[ALG_RUNGEKUTTA] = pRKTrack->getZSeg1RK();
      
      //Variables storing deviation of RK-changed semgment coordinates and META hit
      
      dxRkMeta = pRKTrack->getMETAdx();
      dyRkMeta = pRKTrack->getMETAdy();
      dzRkMeta = pRKTrack->getMETAdz();
      
      //This is required to normalize the deviation to the error of the META coordinates
      if(pMetaMatch->getSystem()==0) //Shower
	{
	  //Here we get the shower hit object and from it the x/y sigmas appropriate for this hit
	  pShowerHitTof = HPidFL::getShowerHitTof(pMetaMatch->getMetaHitInd());
	  if(!pShowerHitTof) {
	    
	    //No shower hit category was present  - see whether we have simulation category - which has a different name
	    pShowerHitTof = HPidFL::getShowerHitTofTrack(pMetaMatch->getMetaHitInd());
	  }
	  if(pShowerHitTof==0)
	    {
	      cout << "NNEEEEE!!!" << endl;
	    }
	  pShowerHitTof->getLabXYZ(&xMeta,&yMeta,&zMeta);
	  errXMeta = pShowerHitTof->getSigmaX();
	  errYMeta = pShowerHitTof->getSigmaY(); 
	}
      
      if(pMetaMatch->getSystem()==1) //TOF
	{
	  Int_t nClsSize = (Int_t)pMetaMatch->getTofClusterSize();
	  
	  //First check what we have from metamach - tofhit or tofcluster - this needs to be cross-checked with anar!!!
#warning - check this with the MetaMatch expert!
	  if ( nClsSize <= 0 ){
	    //No tofcluster, take htofhit category
	    pTofHit = HPidFL::getTofHit(pMetaMatch->getMetaHitInd());
	  }
	  else if ( nClsSize >= 1 ){
	    //tofcluster
	    pTofHit = HPidFL::getTofCluster(pMetaMatch->getMetaHitInd());
	  }
	  
	  //Hardwired errors dictated by TOF resolution
	  if(pTofHit==0)
	    {
	      Error("HPidTrackData::fillFromMetaMatch","No Tof hit for index %s",pMetaMatch->getMetaHitInd());
	    }
	  pTofHit->getXYZLab(xMeta,yMeta,zMeta);
	  errXMeta = 25;
	  errYMeta = 10.2;
	}
      
      bIsAccepted[ALG_RUNGEKUTTA] = pMetaMatch->isRungeKuttaAccepted();
    }
  else
    {
      nRKTrackInd=-1;
      bIsAccepted[ALG_RUNGEKUTTA]= kFALSE;
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



// -----------------------------------------------------------------------------
//Copy all tracking information from a common denominator (base class) of all tracking methods to this object
void HPidTrackData::fillFromBaseTrack(HBaseTrack* pBaseTrack, Int_t momalg)
{
  //reset();

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
      fMomError[momalg] = pBaseTrack->getErrP();
      fCorrectedEloss[momalg] = pBaseTrack->getMetaEloss();
      fCorrectedBeta[momalg] = pBaseTrack->getBeta();
      fMassSquared[momalg] = pBaseTrack->getMass2();
      fTofRecTof[momalg] = pBaseTrack->getTof();           //initialized with value from detector - to be modified later
      fTofRecBeta[momalg] = pBaseTrack->getBeta();         //initialized with value from detector - to be modified later
      fTofRecMassSquared[momalg] = pBaseTrack->getMass2(); //initialized with value from detector - to be modified later
      fPathLength[momalg] = pBaseTrack->getTofDist();      //Not computed by StartTimeFinder-Task - obtion for the future
      cov[momalg].setCov(pBaseTrack->getCovariance());
      qIOMatching[momalg]=pBaseTrack->getIOMatch();
    }
}

// -----------------------------------------------------------------------------

void HPidTrackData::Clear(Option_t *opt)
{
  //cout << "Clearing track data" << endl;
  aTrackletDistances.Set(0);
  nCloseTracklets=0;
}

void HPidTrackData::appendCloseTracklet(Float_t distance)
{
  //cout <<"Appending a neighbour with distance: " << distance << endl;
  Int_t newsize=aTrackletDistances.GetSize()+1;
  aTrackletDistances.Set(newsize);
  aTrackletDistances[newsize-1]=distance;
  nCloseTracklets++;
}
/*
void HPidTrackData::sortTracklets()
{
  Int_t currentposition=0;
  Float_t currentmin=1180.0;
  Float_t currentminposition=0;
  Float_t tmp=0.0;

  for(Int_t i = 0,i<getNCloseTracklets();i++)
    {
      for(Int_t j=i+1,j<getNCloseTracklets();j++)
	{
	  if(getTrackletDistance(j)<getTrackletDistance(i))
	    {
	      tmp=getTrackletDistance(j);
	      getTrackletDistance(j)=getTrackletDistance(i);
	      getTrackletDistance(i)=tmp;
	    }
	}
      }
      isSorted=kTRUE;
}
*/

Float_t HPidTrackData::getAngleWithClosestCandidate(Int_t selectLeptons=0, Int_t selectFitted=0) const
{
  if(selectLeptons < -1 || selectLeptons > 1)
    {
      Error("HPidTrackData::getAngleWithClosestCandidate(Int_t selectLeptons, Int_t selectFitted)",
	    "particle selection %d invalid",selectLeptons);
    }
  if(selectFitted  < -1 || selectFitted  > 1)
    {
      Error("HPidTrackData::getAngleWithClosestCandidate(Int_t selectLeptons, Int_t selectFitted)",
	    "fit selection %d invalid",selectFitted);
    }

  Float_t retval = 2000.0;
  Float_t currentDist=2000.0;

  Float_t sign = 0;
  Int_t isLeptonCandidate=kFALSE;
  //cout << "Number of close tracklets: " << nCloseTracklets << endl;
  for(Int_t i=0;i<nCloseTracklets;i++)
    {
      sign        = TMath::Sign((Float_t)1.0,(Float_t)getTrackletDistance(i));
      currentDist = TMath::Abs(getTrackletDistance(i));
      if(currentDist>1000)
	{
	  currentDist-=1000.0;
	  isLeptonCandidate=kFALSE;
	}
      else
	{
	  isLeptonCandidate=kTRUE;
	}
      
      if(currentDist<retval)
	{
	  //Here we determine whether the current segment belongs to the interesting species or not
	  if(selectLeptons>0 && !isLeptonCandidate) //We only want leptons but the current one's a hadron
	    {
	      continue;

	    }
	  if(selectLeptons<0 && isLeptonCandidate) //We only want hadrons but the current one's a lepton
	    {
	      continue;

	    }
	  if(selectFitted>0 && sign<0.) //We care only for fitted tracks but the current one's not fitted
	    {
	      continue;

	    }
	  if(selectFitted<0 && sign>0.) //We care only for non-fitted tracks but the current one's fitted
	    {
	      continue;

	    }
	  //if we are here than the current track belongs to our selection!
	  retval=currentDist;
	}
    }
  return retval;
}

Float_t HPidTrackData::getAngleWithClosestHadronCandidate(void) const
{
  return getAngleWithClosestCandidate(-1,0);
}

Float_t HPidTrackData::getAngleWithClosestLeptonCandidate(void) const
{
  return getAngleWithClosestCandidate(1,0);
}

//Deprecated!  
Bool_t HPidTrackData::closestCandidateIsFitted(Int_t selectLeptons=0) const
{

  Float_t closestFittedLepton    = getAngleWithClosestCandidate( 1, 1);    
  Float_t closestNonFittedLepton = getAngleWithClosestCandidate( 1,-1); 
  
  Float_t closestFittedHadron    = getAngleWithClosestCandidate(-1, 1);      
  Float_t closestNonFittedHadron = getAngleWithClosestCandidate(-1,-1);    
  
  if(selectLeptons>0) //We care only for leptons
    {
      if(closestFittedLepton<closestNonFittedLepton)
	{
	  return kTRUE;
	}
      else
	{
	  return kFALSE;
	}
    }

  if(selectLeptons<0) //We care only for hadrons
    {
      if(closestFittedHadron<closestNonFittedHadron)
	{
	  return kTRUE;      
	}
      else 
	{
	  return kFALSE;
	}
    }
  //Once we are here this means that we do not care for leptons or hadrons

  if(
     (closestFittedHadron < closestNonFittedHadron &&
      closestFittedHadron < closestNonFittedLepton) ||
     (closestFittedLepton < closestNonFittedHadron &&
      closestFittedLepton < closestNonFittedLepton)
     )
    {
      return kTRUE;
    }
  else
    {
      return kFALSE;
    }
}

//Deprecated
Bool_t  HPidTrackData::closestHadronCandidateIsFitted(void) const
{
  return closestCandidateIsFitted(-1);
}

//Deprecated
Bool_t  HPidTrackData::closestLeptonCandidateIsFitted(void) const
{
  return closestCandidateIsFitted(1);
}

void HPidTrackData::reset(Option_t *)
{
  // Clear all members
  //setFiltered(kFALSE);
  //setBad(kFALSE);
  //isSorted=kFALSE;
  aTrackletDistances.Reset(-9999);
  nCloseTracklets           =  0;
  Clear();

  nBestMomAlg               = -1;  

  nRKTrackInd               = -1;
  nKickTrackInd             = -1;
  nKickTrack123Ind          = -1;
  nRefTrackInd              = -1;
  nSplineTrackInd           = -1;

  fMetaMatchingQuality      = -1.0;
  
  //unInnerMdcNumber          = 0;

  fPull                     = -1.0; 
  fSplineChiSquare          = -1.0; 
  fRKChiSquare              = -1.0;
  /*
  for(Int_t i = 0; i < MAX_MDC_SEG; i++)
    {
      aInnerMdcIds[i]       = -1;
      aInnerMdcCounts[i]    = -1;
    }
  */
  fRKPhi = -1.0;
  fRKTheta = -1.0; 

  dxRkMeta = -1.0;
  dyRkMeta = -1.0;
  dzRkMeta = -1.0;

  xMeta = -1.0;
  yMeta = -1.0;
  zMeta = -1.0;
  
  errXMeta = -1.0;
  errYMeta = -1.0;
  errZMeta = -1.0;

  for(Int_t i = 0; i < MAX_MOM_ALGS; i++)
    {
      qIOMatching[i]=-1.;

      nPolarity[i]          = 0;

      fMomenta[i]           = -1;
      fMomError[i]          = -1; 

      fTrackR[i]            = -1.0;
      fTrackZ[i]            = -1.0;
      
      fCorrectedEloss[i]    = -1;
      fCorrectedBeta[i]     = -1; 
      
      fPathLength[i]        = -1;

      fMassSquared[i]       = -1; 
 
      bIsAccepted[i]        = kFALSE;
 
      nTofRecFlag[i]        = 0;
      fTofRecTof[i]         = -100.0;
      fTofRecBeta[i]        = -1.0;
      fTofRecMassSquared[i] = -1.0;
      cov[i].Clear();

    }

}

// -----------------------------------------------------------------------------

void HPidTrackData::print(void) const
{
  // Print all fields of the track candidate
  
  Int_t i;
  
  printf("BestMomAlg                %4d\n", nBestMomAlg);
  printf("KickTrackId:              %4d\n", nKickTrackInd);
  printf("SplineTrackId:            %4d\n", nSplineTrackInd);
  printf("RefTrackId:               %4d\n", nRefTrackInd);
  
  
  printf("InnerMdcDistances(%2d):", nCloseTracklets);
  for(i = 0; i < nCloseTracklets; i++)
    printf(" %4f ", aTrackletDistances[i]);
   
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
/*
//Custom streamer defined by Anar Rustamov to allow for new variables - backward compatibility to old instances 
//(Generated before 2005 cannot be guaranteed - Hey this is Hydra if you want professional code - sue me :->)
void HPidTrackData::Streamer(TBuffer &R__b)
{
   // Stream an object of class HPidTrackData.
  //cout << "Streaming" << endl;
  Int_t a;
  cin >> a;
   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      int R__i;
      for (R__i = 0; R__i < MAX_MOM_ALGS; R__i++)
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
      R__b >> nCloseTracklets;
      aTrackletDistances.Streamer(R__b);
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
      for (R__i = 0; R__i < MAX_MOM_ALGS; R__i++)
         cov[R__i].Streamer(R__b);
      R__b.WriteArray(qIOMatching, MAX_MOM_ALGS);
      R__b << nBestMomAlg;
      R__b << nRKTrackInd;
      R__b << nKickTrackInd;
      R__b << nKickTrack123Ind;
      R__b << nRefTrackInd;
      R__b << nSplineTrackInd;
      R__b << fMetaMatchingQuality;
      R__b << nCloseTracklets;
      aTrackletDistances.Streamer(R__b);
      R__b << fPull;
      R__b << fSplineChiSquare;
      R__b << fRKChiSquare;
      R__b.WriteArray(nPolarity, MAX_MOM_ALGS);
      R__b.WriteArray(fMomenta, MAX_MOM_ALGS);
      R__b.WriteArray(fMomError, MAX_MOM_ALGS);
      R__b.WriteArray(fTrackR, MAX_MOM_ALGS);
      R__b.WriteArray(fTrackZ, MAX_MOM_ALGS);
      R__b.WriteArray(fCorrectedEloss, MAX_MOM_ALGS);
      R__b.WriteArray(fCorrectedBeta, MAX_MOM_ALGS);
      R__b.WriteArray(fPathLength, MAX_MOM_ALGS);
      R__b.WriteArray(fMassSquared, MAX_MOM_ALGS);
      R__b.WriteArray(bIsAccepted, MAX_MOM_ALGS);
      R__b.WriteArray(nTofRecFlag, MAX_MOM_ALGS);
      R__b.WriteArray(fTofRecTof, MAX_MOM_ALGS);
      R__b.WriteArray(fTofRecBeta, MAX_MOM_ALGS);
      R__b.WriteArray(fTofRecMassSquared, MAX_MOM_ALGS);
      R__b.SetByteCount(R__c, kTRUE);
   }
}


// -----------------------------------------------------------------------------

*/
