// @(#)$Id: hpidhitdata.cc,v 1.23 2006-08-16 16:13:35 halo Exp $
//*-- Author : Marcin Jaskula 02/03/2003
//  Modified : Marcin Jaskula 14/03/2003
//              tof and mdc variables added
//  Modified : Marcin Jaskula 16/04/2003
//              flags added
//  Refactored: Tassilo Christ 16/10/03
//              redesigned to conform with new tracking scheme
//  Modified : Stefano Spataro 26/08/2005
//              TOF rod/TOFINO cell included
//  Modified : Tassilo Christ 05/02/2005
//             Upgraded documentation

//_HADES_CLASS_DESCRIPTION
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// HPidHitData                                                                //
//                                                                            //
// Class for storing all intersting data from hit objects matched in          //
// HMetaMatch. Dedicated to store data and serve as an associated class of    //
// HPidTrackCandidate and HPidParticle                                        //
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
#include "hbasetrack.h"
// -----------------------------------------------------------------------------

ClassImp(HPidHitData)
  
// -----------------------------------------------------------------------------

HPidHitData::HPidHitData(const HPidHitData& source):TObject(source)
{
  //The hit flag datamembers are copied by the base class copy constructor
  nSector=source.getSector();
  iSystem=source.getSystem();
  nRingPadNr=source.getRingPadNr();		
  fRingCentroid=source.getRingCentroid();
  fRichTheta=source.getRichTheta();		
  fRichPhi=source.getRichPhi();
  nRingPatMat=source.getRingPatMat();		
  nRingHouTra=source.getRingHouTra();				
  nRingAmplitude=source.getRingAmplitude();	       	
  nRingLocalMax4=source.getRingLocalMax4();	


  iMdcClsSize0=source.getMdcClsSize0();
  iMdcNWires0=source.getMdcNWires0();		  
  iMdcLevelCls0=source.getMdcLevelCls0();
  iMdcClsSize1=source.getMdcClsSize1();
  iMdcNWires1=source.getMdcNWires1();		
  iMdcLevelCls1=source.getMdcLevelCls1();	
  fInnerMdcdEdx=source.getInnerMdcdEdx();
  fInnerMdcdEdxSigma=source.getInnerMdcdEdxSigma();
  fInnerMdcChiSquare=source.getInnerMdcChiSquare();	
  
  fMdcRCoord=source.getMdcRCoord(); 
  fMdcZCoord=source.getMdcZCoord();
  fMdcTheta=source.getMdcTheta();		
  fMdcPhi=source.getMdcPhi(); 		

  iMdcClsSize2=source.getMdcClsSize2();
  iMdcNWires2=source.getMdcNWires2();		  
  iMdcLevelCls2=source.getMdcLevelCls2();
  iMdcClsSize3=source.getMdcClsSize3();
  iMdcNWires3=source.getMdcNWires3();		
  iMdcLevelCls3=source.getMdcLevelCls3();	
  fOuterMdcdEdx=source.getOuterMdcdEdx();
  fOuterMdcdEdxSigma=source.getOuterMdcdEdxSigma();
  fOuterMdcChiSquare=source.getOuterMdcChiSquare();	
    
  
  iIPURingQuality=source.getIPURingQuality();
  iIPUVetoQuality=source.getIPUVetoQuality();
  
  for (Int_t i=0;i<3;i++)
    {
      fShowerSum[i]=source.getShowerSum(i);
      nShowerClS[i]=source.getShowerClS(i);
    }
  nShowerRow = source.getShowerRow();
  nShowerCol = source.getShowerCol();
  fShowerTimeOfFlight=source.getShowerTimeOfFlight();	
  fTOFTimeOfFlight=source.getTOFTimeOfFlight();
  fTOFLeftAmplitude=source.getTOFLeftAmplitude();
  fTOFRightAmplitude=source.getTOFRightAmplitude();	
  fTofEloss=source.getTofEloss();	
  fMetaLocalX = source.getMetaLocalX();
  fMetaLocalY = source.getMetaLocalY();

  iTofinoMult=source.getTofinoMult();		
  nTofClsSize=source.getTofClsSize();
  nMetaCell=source.getMetaCell();				
  nTofCell=source.getTofCell();				
  nTofModule=source.getTofModule();				
  
  iIndRICH=source.getIndRICH();
  iIndRICHIPU=source.getIndRICHIPU();
  iIndInnerSeg=source.getIndInnerSeg();		
  iIndOuterSeg=source.getIndOuterSeg();		
  iIndTOF=source.getIndTOF();		
  iIndShower=source.getIndShower();		
  iIndMatch=source.getIndMatch();		
  
  
  for(Int_t i=0;i<nMomAlgs;i++)
    {
      fDistanceToVertex[i]=source.getDistanceToVertex(i);
      hasRingCorrelation[i]=source.getRingCorrelation(i);
      hasMetaTrackCorrelation[i]=source.getMetaTrackCorrelation(i);
    }

  
}

void HPidHitData::reset(void)
{
// Set all members to -1

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


  iMdcClsSize0=-1;		
  iMdcNWires0=-1;		  
  iMdcLevelCls0=-1;	
  iMdcClsSize1=-1;		
  iMdcNWires1=-1;		
  iMdcLevelCls1=-1;	
  fInnerMdcdEdx=-1.0;
  fInnerMdcdEdxSigma=-1.0;
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
  fOuterMdcdEdx=-1.0;
  fOuterMdcdEdxSigma=-1.0;


  
  iIPURingQuality=-1;	
  iIPUVetoQuality=-1;	

  for (Int_t i=0;i<3;i++)
    {
      fShowerSum[i]=-1.0;	
      nShowerClS[i]=-1;
    }
  nShowerRow = -1;
  nShowerCol = -1;
  fShowerTimeOfFlight=-1.0;	
  fMetaLocalX = -999.0;
  fMetaLocalY = -999.0;

  fTOFTimeOfFlight=-1.0;	
  fTOFLeftAmplitude=-1.0;	
  fTOFRightAmplitude=-1.0;	


  iTofinoMult=-1;		
  nTofClsSize=-1;
  nMetaCell= -1;				
  nTofCell= -1;				
  nTofModule= -1;				
  fTofEloss = -1.0;
				
  iIndRICH=-1;		
  iIndRICHIPU=-1;		
  iIndInnerSeg=-1;		
  iIndOuterSeg=-1;		
  iIndTOF=-1;		
  iIndShower=-1;		
  iIndMatch=-1;		

 
  clearFlags();

  for(Int_t i=0;i<nMomAlgs;i++)
    {
      fDistanceToVertex[i]=-1.;
      hasRingCorrelation[i]=kFALSE;
      hasMetaTrackCorrelation[i]=kFALSE;
    }

}



// -----------------------------------------------------------------------------

// The hit data object collects all relevant hit information from a set of matched 
// hit objects from different detectors
void HPidHitData::fillFromMetaMatch(HMetaMatch *pMatch, Int_t iMatchIndex)
{

  //Initialize everything to default values
  HMdcSeg* pInnerMdcSeg=NULL;
  HMdcSeg* pOuterMdcSeg=NULL;

  HRichHit* pRichHit=NULL;
  HRichHitIPU* pRichHitIPU=NULL;

  HShowerHitTof* pShowerHitTof = NULL;
  HTofHit* pTofHit=NULL;

  reset();


  //Global properties of the match-object
  nSector = pMatch->getSector();
  if(nSector>5 || nSector<0)
    cout << "wrong sector from metamatch!" << nSector <<endl;

  iIndMatch = iMatchIndex;

  //Basic tracking object composed of inner (inner/outer) segment
  HMdcTrkCand* pTrackCand = HPidFL::getMdcTrkCand(pMatch->getTrkCandInd());


  if(pTrackCand!=NULL) //object exists
    {
      if(pTrackCand->getSeg1Ind()>-1) //inner segment exists - store its properties
	{
	  pInnerMdcSeg = HPidFL::getMdcSeg(pTrackCand->getSeg1Ind());
	}
      
      if (pInnerMdcSeg != NULL) 
	{
	  //We store all tracking information not asking whether or not a track was accepted!!
	  //This information is handled by HPidTrackData
	  if(pMatch->getKickInd()>-1){
            fDistanceToVertex[ALG_KICK] = ((HBaseTrack*)HPidFL::getKickTrackB(pMatch->getKickInd()))->getTarDist();
	  }
	  if(pMatch->getSplineInd()>-1){
            fDistanceToVertex[ALG_SPLINE] = ((HBaseTrack*)HPidFL::getSplineTrack(pMatch->getSplineInd()))->getTarDist();
	  }
	  if(pMatch->getRungeKuttaInd()>-1){
            fDistanceToVertex[ALG_RUNGEKUTTA] = ((HBaseTrack*)HPidFL::getRKTrack(pMatch->getRungeKuttaInd()))->getTarDist();
	    
	  }
	  
	  iMdcClsSize0       = HPidMdcFunc::getMdcClsSize(pInnerMdcSeg,  0);
	  iMdcNWires0        = HPidMdcFunc::getMdcNWires(pInnerMdcSeg,   0);
	  iMdcLevelCls0      = HPidMdcFunc::getMdcLevelCls(pInnerMdcSeg, 0);
	  iMdcClsSize1       = HPidMdcFunc::getMdcClsSize(pInnerMdcSeg,  1);
	  iMdcNWires1        = HPidMdcFunc::getMdcNWires(pInnerMdcSeg,   1);
	  iMdcLevelCls1      = HPidMdcFunc::getMdcLevelCls(pInnerMdcSeg, 1);
	  fInnerMdcChiSquare = HPidMdcFunc::getMdcChiSquare(pInnerMdcSeg  );
	  fInnerMdcdEdx = pTrackCand->getdedxInnerSeg();
	  fInnerMdcdEdxSigma=pTrackCand->getSigmadedxInnerSeg();
	  fMdcRCoord = pInnerMdcSeg->getR();
	  fMdcZCoord = pInnerMdcSeg->getZ();
	  fMdcTheta = HPidFL::getMdcTheta(pInnerMdcSeg->getTheta());
	  fMdcPhi = HPidFL::getMdcPhi(pInnerMdcSeg->getSec(),pInnerMdcSeg->getPhi());
	  setFlagInnerMDC();
	  iIndInnerSeg = pTrackCand->getSeg1Ind();



	  
	}
      else
	{
	  Error("HPidHitData::fillFromMetMatch()","Inner MDC segment has invalid index");
	  exit(-1);
	}
      
      if(pTrackCand->getSeg2Ind()>-1) //outer segment exists - store its properties
	{
	  pOuterMdcSeg = HPidFL::getMdcSeg(pTrackCand->getSeg2Ind());
	}
	
      if(pOuterMdcSeg != NULL)
	{
	  iMdcClsSize2       = HPidMdcFunc::getMdcClsSize(pOuterMdcSeg,  0);
	  iMdcNWires2        = HPidMdcFunc::getMdcNWires(pOuterMdcSeg,   0);
	  iMdcLevelCls2      = HPidMdcFunc::getMdcLevelCls(pOuterMdcSeg, 0);
	  iMdcClsSize3       = HPidMdcFunc::getMdcClsSize(pOuterMdcSeg,  1);
	  iMdcNWires3        = HPidMdcFunc::getMdcNWires(pOuterMdcSeg,   1);
	  iMdcLevelCls3      = HPidMdcFunc::getMdcLevelCls(pOuterMdcSeg, 1);
	  fOuterMdcChiSquare = HPidMdcFunc::getMdcChiSquare(pOuterMdcSeg);
	  fOuterMdcdEdx = pTrackCand->getdedxOuterSeg();
	  fOuterMdcdEdxSigma=pTrackCand->getSigmadedxOuterSeg();
	  setFlagOuterMDC();
	  iIndOuterSeg = pTrackCand->getSeg2Ind();



	}
    }
  else
    {
      Error("HPidHitData::fillFromMetMatch()","MDC track candidate has invalid index");
      exit(-1);
    }



    
  //Check whether multiple rings match the track
  Int_t bestRingIndex=-1;
  
  if(pMatch->getARichInd(0)>-1)
    {
      bestRingIndex= HPidFL::pickBestRing(pMatch, fMdcPhi, fMdcTheta); //double check this function!
      pRichHit = HPidFL::getRichHit(pMatch->getARichInd(bestRingIndex));
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
      iIndRICH       = pMatch->getARichInd(bestRingIndex); 
      setFlagRICH();
    }
  
  
  
  //At some point someone - whoever cares for it - can implement a search for the best matching IPU ring
  //if there is any
  if(pMatch->getARichIPUInd(0)>-1)
    pRichHitIPU = HPidFL::getRichHitIPU(pMatch->getARichIPUInd(0));
  
  if(pRichHitIPU != NULL)
    {
      iIPURingQuality = pRichHitIPU->getQuality();
      iIPUVetoQuality = pRichHitIPU->getVetoQuality();
      setFlagRICHIPU();
      iIndRICHIPU = pMatch->getARichIPUInd(0);
    }

  
  iSystem=pMatch->getSystem();
  
  if(pMatch->getSystem() == 0) //Meta hit was found in shower
    {
      
      //Check for REAL shower category
      pShowerHitTof = HPidFL::getShowerHitTof(pMatch->getMetaHitInd());
      if(!pShowerHitTof) {
	
	//No shower hit category was present  - see whether we have simulation category - which has a different name
	pShowerHitTof = HPidFL::getShowerHitTofTrack(pMatch->getMetaHitInd());
      }
 
      if(pShowerHitTof!=NULL)
	{
	  setFlagSHOWER();
	  fShowerSum[0] = pShowerHitTof->getSum(0);
	  fShowerSum[1] = pShowerHitTof->getSum(1);
	  fShowerSum[2] = pShowerHitTof->getSum(2);
	  nShowerClS[0] = pShowerHitTof->getClusterSize(0);
	  nShowerClS[1] = pShowerHitTof->getClusterSize(1);
	  nShowerClS[2] = pShowerHitTof->getClusterSize(2);
	  nShowerRow    = pShowerHitTof->getRow();
	  nShowerCol    = pShowerHitTof->getCol();
	  fShowerTimeOfFlight = pShowerHitTof->getTof();
	  iIndShower    = pMatch->getShowerHitInd();
	  iTofinoMult   = pShowerHitTof->getTofinoMult();
	  nMetaCell     = pShowerHitTof->getTofinoCell();
	  pShowerHitTof->getXY(&fMetaLocalX, &fMetaLocalY);
	}
      else 
	{
	  Error("HPidHitData::fillFromMeta","Shower hit indexed but not found");
	}
      
    }
  
  if(pMatch->getSystem() == 1) //Tof hit
    {
      Int_t nClsSize = (Int_t)pMatch->getTofClusterSize();

      //First check what we have from metamach - tofhit or tofcluster - this needs to be cross-checked with anar!!!
      #warning - check this with the MetaMatch expert!
      if ( nClsSize <= 0 ){
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
      
      if(pTofHit!=NULL)
	{
	  setFlagTOF();
	  fTOFTimeOfFlight = pTofHit->getTof();
	  fTOFLeftAmplitude = pTofHit->getLeftAmp();
	  fTOFRightAmplitude = pTofHit->getRightAmp();
	  fTofEloss = pTofHit->getEdep();
	  iIndTOF = pMatch->getTofHitInd();
	  nTofClsSize = nClsSize;
	  nMetaCell = pTofHit->getModule()*8+pTofHit->getCell(); //Implemented by META experts - ask Jurek or Pavel
	  nTofCell = pTofHit->getCell();
	  nTofModule = pTofHit->getModule();
	  fMetaLocalX=pTofHit->getXpos();

	  if(iIndTOF<0)
	    {
	      Error("HPidHitData::fillFromMetaMatch","Negative tof hit index!");
	    }
	}
      else
	{
	  Error("HPidHitData::fillFromMetaMatch","Tof hit indexed but not found!");
	}
      
    }//end system==1
}

// -----------------------------------------------------------------------------

Float_t HPidHitData::getDeltaThetaMDCRICH() const
{
  return fRichTheta -fMdcTheta;
}

Float_t HPidHitData::getDeltaPhiMDCRICH() const
{
  return ( fRichPhi- fMdcPhi )*TMath::Sin(TMath::DegToRad()*fMdcTheta);
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
    printf("fInnerMdcdEdx: %f\n", fInnerMdcdEdx);
    printf("fInnerMdcdEdxSigma: %f\n", fInnerMdcdEdxSigma);
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
    printf("fOuterMdcdEdx: %f\n", fOuterMdcdEdx);
    printf("fOuterMdcdEdxSigma: %f\n", fOuterMdcdEdxSigma);

    printf("iIPURingQuality: %d\n", iIPURingQuality);
    printf("iIPUVetoQuality: %d\n", iIPUVetoQuality);

    printf("fShowerSum[0]: %f\n", fShowerSum[0]);
    printf("fShowerSum[1]: %f\n", fShowerSum[1]);
    printf("fShowerSum[2]: %f\n", fShowerSum[2]);
    printf("fShowerTimeOfFlight: %f\n", fShowerTimeOfFlight);
    printf("fMetaLocalX: %f\n", fMetaLocalX);
    printf("fMetaLocalY: %f\n", fMetaLocalY);
    
    printf("fTOFTimeOfFlight: %f\n", fTOFTimeOfFlight);
    printf("fTOFLeftAmplitude: %f\n", fTOFLeftAmplitude);
    printf("fTOFRightAmplitude: %f\n", fTOFRightAmplitude);
    printf("fTofEloss: %f\n", fTofEloss);
    printf("iTofinoMult: %d\n", iTofinoMult);
    printf("nMetaCell: %d\n", nMetaCell);
    printf("nTofCell: %d\n", nTofCell);
    printf("nTofModule: %d\n", nTofModule);

    printf("iIndRICH: %d\n",iIndRICH);
    printf("iIndRICHIPU: %d\n",iIndRICHIPU);
    printf("iIndInnerSeg: %d\n",iIndInnerSeg);
    printf("iIndOuterSeg: %d\n",iIndOuterSeg);
    printf("iIndShower: %d\n",iIndShower);
    printf("iIndTOF: %d\n",iIndTOF);
    printf("iIndMatch: %d\n",iIndMatch);
    

}



