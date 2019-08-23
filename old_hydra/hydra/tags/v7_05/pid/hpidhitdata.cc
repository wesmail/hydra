// @(#)$Id: hpidhitdata.cc,v 1.13 2005-02-16 10:23:55 christ Exp $
//*-- Author : Marcin Jaskula 02/03/2003
//  Modified : Marcin Jaskula 14/03/2003
//              tof and mdc variables added
//  Modified : Marcin Jaskula 16/04/2003
//              flags added
//  Refactored: Tassilo Christ 16/10/03
//              redesigned to conform with new tracking scheme

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// HPidHitData                                                                //
//                                                                            //
// Class for storing all intersting data from hit objects matched in          //
// HMetaMatch. Dedicated to store data and serve as a base class of           //
// HPidTrackCandidate.                                                        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "hpidhitdata.h"
#include "hshowerhittof.h"
#include "htofhit.h"
#include "htofcluster.h"
#include "hshowerhittoftrack.h"
#include "hcategory.h"
#include "hmetamatch.h"
#include "hmdctrkcand.h"
#include "hmdctrackgdef.h"
#include "hrichhit.h"
#include "hkicktrack.h"
#include "hades.h"
#include "hpidmdcfunc.h"
#include "hpidfl.h"
#include "hevent.h"
#include "hcategory.h"
#include <TObject.h>
#include <TError.h>
#include "piddef.h"
#include "richdef.h"
#include "showertofinodef.h"
#include "hmdcdef.h"
#include "hmdcseg.h"
#include "hrichhitIPU.h"
#include "triggerinfodef.h"
// -----------------------------------------------------------------------------

ClassImp(HPidHitData)

// -----------------------------------------------------------------------------


void HPidHitData::reset(void)
{
// Set all memebers to -1

  nSector=-1;
  iSystem=-1;
  nRingPadNr=-1;		
  fRingCentroid=-1.0;	
  fRichTheta=-1.0;		
  fRichPhi=-1.0;		
  nRingPatMat=-1;		
  nRingHouTra=-1;		
  nRingAmplitude=-1;	
  nRingLocalMax4=-1;	

				
  fDistanceToVertex=-1.0;

  iMdcClsSize0=-1;		
  iMdcNWires0=-1;		  
  iMdcLevelCls0=-1;	
  iMdcClsSize1=-1;		
  iMdcNWires1=-1;		
  iMdcLevelCls1=-1;	
  fInnerMdcChiSquare=-1.0;	
  fMdcRCoord=-1.0; 
  fMdcZCoord=-1.0; 
  fMdcTheta=-1.0;		
  fMdcPhi=-1.0;		

  iMdcClsSize2=-1;		
  iMdcNWires2=-1;		
  iMdcLevelCls2=-1;	
  iMdcClsSize3=-1;		
  iMdcNWires3=-1;		
  iMdcLevelCls3=-1;	
  fOuterMdcChiSquare=-1.0;	

  
  iIPURingQuality=-1;	
  iIPUVetoQuality=-1;	

  for (Int_t i=0;i<3;i++)
    {
      fShowerSum[i]=-1.0;	
      nShowerClS[i]=-1;
    }

  fShowerTimeOfFlight=-1.0;	
  fTOFTimeOfFlight=-1.0;	
  iTofinoMult=-1;		
				
				
  iIndRICH=-1;		
  iIndRICHIPU=-1;		
  iIndInnerSeg=-1;		
  iIndOuterSeg=-1;		
  iIndTOF=-1;		
  iIndShower=-1;		
  iIndMatch=-1;		
  nTofClsSize=-1;
 
  clearFlags();

  for(Int_t i=0;i<nMomAlgs;i++)
    {
      hasRingCorrelation[i]=kFALSE;
    }

}



// -----------------------------------------------------------------------------

void HPidHitData::fillFromHitData(HPidHitData* pSource)
{
  memcpy(this,pSource,sizeof(HPidHitData));
}

// The hit data object collects all relevant hit information from a set of matched 
// hit objects from different detectors
void HPidHitData::fillFromMetaMatch(HMetaMatch *pMatch, Int_t iMatchIndex)
{

  //Global properties of the match-object
  HMdcSeg* pInnerMdcSeg=NULL;
  HMdcSeg* pOuterMdcSeg=NULL;

  HRichHit* pRichHit=NULL;
  HRichHitIPU* pRichHitIPU=NULL;

  HShowerHitTof* pShowerHitTof = NULL;
  HTofHit* pTofHit=NULL;

  reset();
  nSector = pMatch->getSector();
  if(nSector>5 || nSector<0)
    cout << "wrong sector from metamatch!" << nSector <<endl;
  iIndMatch = iMatchIndex;//pInputCategory->getIndex(pMatch);

    HMdcTrkCand* pTrackCand = HPidFL::getMdcTrkCand(pMatch->getTrkCandInd());

    if(pTrackCand!=NULL)
      {
	if(pTrackCand->getSeg1Ind()>-1)
	  pInnerMdcSeg = HPidFL::getMdcSeg(pTrackCand->getSeg1Ind());
	
	if (pInnerMdcSeg != NULL)
	  {
		#warning FIXME
	    fDistanceToVertex  = HPidFL::computeDistanceToVertex(pInnerMdcSeg);
	    
	    iMdcClsSize0       = HPidMdcFunc::getMdcClsSize(pInnerMdcSeg,  0);
	    iMdcNWires0        = HPidMdcFunc::getMdcNWires(pInnerMdcSeg,   0);
	    iMdcLevelCls0      = HPidMdcFunc::getMdcLevelCls(pInnerMdcSeg, 0);
	    iMdcClsSize1       = HPidMdcFunc::getMdcClsSize(pInnerMdcSeg,  1);
	    iMdcNWires1        = HPidMdcFunc::getMdcNWires(pInnerMdcSeg,   1);
	    iMdcLevelCls1      = HPidMdcFunc::getMdcLevelCls(pInnerMdcSeg, 1);
	    fInnerMdcChiSquare = HPidMdcFunc::getMdcChiSquare(pInnerMdcSeg  );
	    fMdcRCoord = pInnerMdcSeg->getR();
	    fMdcZCoord = pInnerMdcSeg->getZ();
	    fMdcTheta = HPidFL::getMdcTheta(pInnerMdcSeg->getTheta());
	    fMdcPhi = HPidFL::getMdcPhi(pInnerMdcSeg->getSec(),pInnerMdcSeg->getPhi());
	    setFlagInnerMDC();
	    iIndInnerSeg = pTrackCand->getSeg1Ind();
	  }
	if(pTrackCand->getSeg2Ind()>-1)
	  pOuterMdcSeg = HPidFL::getMdcSeg(pTrackCand->getSeg2Ind());
	
	if(pOuterMdcSeg != NULL)
	  {
	    iMdcClsSize2       = HPidMdcFunc::getMdcClsSize(pOuterMdcSeg,  0);
	    iMdcNWires2        = HPidMdcFunc::getMdcNWires(pOuterMdcSeg,   0);
	    iMdcLevelCls2      = HPidMdcFunc::getMdcLevelCls(pOuterMdcSeg, 0);
	    iMdcClsSize3       = HPidMdcFunc::getMdcClsSize(pOuterMdcSeg,  1);
	    iMdcNWires3        = HPidMdcFunc::getMdcNWires(pOuterMdcSeg,   1);
	    iMdcLevelCls3      = HPidMdcFunc::getMdcLevelCls(pOuterMdcSeg, 1);
	    fOuterMdcChiSquare = HPidMdcFunc::getMdcChiSquare(pOuterMdcSeg);
	    setFlagOuterMDC();
	    iIndOuterSeg = pTrackCand->getSeg2Ind();
	  }
      }


    
    //Check whether multiple rings match the track
    Int_t bestRingIndex=-1;
    
    if(pMatch->getARichInd(0)>-1)
      {
	if(pMatch->getARichInd(1)>-1)
	  {
	    bestRingIndex= HPidFL::pickBestRing(pMatch, fMdcPhi, fMdcTheta);
	    pRichHit = HPidFL::getRichHit(pMatch->getARichInd(bestRingIndex));
	  }
	else 
	  {
	    pRichHit = HPidFL::getRichHit(pMatch->getARichInd(0));
	  }
      }
    
    if(pRichHit != NULL)
      {
	//cout << "Rich hit found" << endl;
	nRingPadNr     = pRichHit->getRingPadNr();
	fRingCentroid  = pRichHit->getCentroid();
	fRichTheta     = pRichHit->getTheta();
	fRichPhi       = pRichHit->getPhi();
	nRingPatMat    = pRichHit->getRingPatMat();
	nRingHouTra    = pRichHit->getRingHouTra();
	nRingAmplitude = pRichHit->getRingAmplitude();
	nRingLocalMax4 = pRichHit->getRingLocalMax4();
	iIndRICH       = pMatch->getARichInd(0);
	setFlagRICH();
      }
    
    
    
    
    if(pMatch->getARichIPUInd(0)>-1)
      pRichHitIPU = HPidFL::getRichHitIPU(pMatch->getARichIPUInd(0));

    if(pRichHitIPU != NULL)
      {
	iIPURingQuality = pRichHitIPU->getQuality();
	iIPUVetoQuality = pRichHitIPU->getVetoQuality();
	setFlagRICHIPU();
	iIndRICHIPU = pMatch->getARichIPUInd(0);
      }
    //cout << "beyond hit ipu" << endl;
    iSystem=pMatch->getSystem();

    if(pMatch->getSystem() == 0) //Meta hit was found in shower/tofino
      {

	//Check for REAL category
	pShowerHitTof = HPidFL::getShowerHitTof(pMatch->getMetaHitInd());
	if(!pShowerHitTof) {

	  //No hit tof category was present  - see whether we have simulation category
	  pShowerHitTof = HPidFL::getShowerHitTofTrack(pMatch->getMetaHitInd());
	}
	if(pShowerHitTof!=NULL)
	  {
	    setFlagSHOWER();
	    fShowerTimeOfFlight = pShowerHitTof->getTof();
	    fShowerSum[0] = pShowerHitTof->getSum(0);
	    fShowerSum[1] = pShowerHitTof->getSum(1);
	    fShowerSum[2] = pShowerHitTof->getSum(2);
	    nShowerClS[0] = pShowerHitTof->getClusterSize(0);
	    nShowerClS[1] = pShowerHitTof->getClusterSize(1);
	    nShowerClS[2] = pShowerHitTof->getClusterSize(2);
	    iIndShower    = pMatch->getShowerHitInd();
        iTofinoMult   = pShowerHitTof->getTofinoMult();
	  }
	else Error("HPidHitData::fillFromMeta","Shower hit indexed but not found");
	
      }
    //cout << "beyond sys0" << endl;

    if(pMatch->getSystem() == 1) //Tof hit
      {
		Int_t nClsSize = (Int_t)pMatch->getTofClusterSize();
		//printf("nClsSize=%d\n",nClsSize);
		//First check what we have from metamach - tofhit or tofcluster
		if ( nClsSize == 0 ){
			//No tofcluster, take htofhit category
			pTofHit = HPidFL::getTofHit(pMatch->getMetaHitInd());
				}
		else if ( nClsSize >= 1 ){
			//tofcluster
			pTofHit = HPidFL::getTofCluster(pMatch->getMetaHitInd());
		}
		else {
			Warning("fillFromMetaMatch", 
			"What should I do with cluster size=%d?", nClsSize);
		}

		if(pTofHit!=NULL){
			setFlagTOF();
			fTOFTimeOfFlight = pTofHit->getTof();
			iIndTOF = pMatch->getTofHitInd();
			nTofClsSize = nClsSize;
			if(iIndTOF<0){
				Error("fillFromMetaMatch","Negative tof hit index!");
			}
		}
	}//end system==1
}

// -----------------------------------------------------------------------------

Float_t HPidHitData::getDeltaThetaMDCRICH()
{
  return fRichTheta -fMdcTheta;
}

Float_t HPidHitData::getDeltaPhiMDCRICH()
{
  return ( fRichPhi- fMdcPhi )*TMath::Sin(fMdcTheta);
}

void HPidHitData::print(void) const
{
    printf("HPidHitData\n");
    printf("nRingPadNr: %d\n", nRingPadNr);
    printf("fRingCentroid: %f\n", fRingCentroid);
    printf("fRichTheta: %f\n", fRichTheta);
    printf("fRichPhi: %f\n", fRichPhi);
    printf("nRingPatMat: %d\n", nRingPatMat);
    printf("nRingHouTra: %d\n", nRingHouTra);
    printf("nRingAmplitude: %d\n", nRingAmplitude);
    printf("nRingLocalMax4: %d\n", nRingLocalMax4);

    printf("iMdcClsSize0: %d\n", iMdcClsSize0);
    printf("iMdcNWires0: %d\n", iMdcNWires0);
    printf("iMdcLevelCls0: %d\n", iMdcLevelCls0);
    printf("iMdcClsSize1: %d\n", iMdcClsSize1);
    printf("iMdcNWires1: %d\n", iMdcNWires1);
    printf("iMdcLevelCls1: %d\n", iMdcLevelCls1);
    printf("fInnerMdcChiSquare: %f\n", fInnerMdcChiSquare);
    printf("fMdcRCoord: %f\n",fMdcRCoord);
    printf("fMdcZCoord: %f\n",fMdcZCoord);
    printf("fMdcTheta:  %f\n",fMdcTheta);
    printf("fMdcPhi:  %f\n",fMdcPhi);

    printf("iMdcClsSize2: %d\n", iMdcClsSize2);
    printf("iMdcNWires2: %d\n", iMdcNWires2);
    printf("iMdcLevelCls2: %d\n", iMdcLevelCls2);
    printf("iMdcClsSize3: %d\n", iMdcClsSize3);
    printf("iMdcNWires3: %d\n", iMdcNWires3);
    printf("iMdcLevelCls3: %d\n", iMdcLevelCls3);
    printf("ifOuterMdcChiSquare: %f\n", fOuterMdcChiSquare);

    printf("iIPURingQuality: %d\n", iIPURingQuality);
    printf("iIPUVetoQuality: %d\n", iIPUVetoQuality);

    printf("fShowerSum[0]: %f\n", fShowerSum[0]);
    printf("fShowerSum[1]: %f\n", fShowerSum[1]);
    printf("fShowerSum[2]: %f\n", fShowerSum[2]);
    printf("fShowerTimeOfFlight: %f\n", fShowerTimeOfFlight);
    printf("fTOFTimeOfFlight: %f\n", fTOFTimeOfFlight);
    printf("iTofinoMult: %d\n", iTofinoMult);

    printf("iIndRICH: %d\n",iIndRICH);
    printf("iIndRICHIPU: %d\n",iIndRICHIPU);
    printf("iIndInnerSeg: %d\n",iIndInnerSeg);
    printf("iIndOuterSeg: %d\n",iIndOuterSeg);
    printf("iIndShower: %d\n",iIndShower);
    printf("iIndTOF: %d\n",iIndTOF);
    printf("iIndMatch: %d\n",iIndMatch);
    

}



