using namespace std;
#include "hpidrichtable.h"
#include "hades.h"
#include "hevent.h"
#include "hcategory.h"
#include "hiterator.h"
#include "hdetector.h"
#include "hspectrometer.h"
#include "hkicktrack.h"
#include "hkicktracksim.h"
#include <iostream>
#include <iomanip>
#include "hrichhit.h"
#include "hrichpad.h"
#include "hrichgeometrypar.h"
#include "hruntimedb.h"
#include "hrichanalysispar.h"
#include "kickdef.h"
#include "richdef.h"
#include "hmdcdef.h"
#include "hgeantrich.h"
#include "hruntimedb.h"
#include <TMath.h>
#include "hhitmatchsim.h"
#include "htrackinfo.h"
#include "hrichutilfunc.h"
#include <stdlib.h>

ClassImp(HPidRichTable)
  HPidRichTable::HPidRichTable(void) : HReconstructor("RichPidTable","table")
{
  pRichHitMatchPropertiesSingle = new TNtuple("RichHitMatchPropertiesSingle","Properties of single rings in rich",
					      "dPhi:dTheta:pmq:padnr:centroid:locmax:avcharge:theta:phi:sector:neighbour:openingAngle:dalitz:gamma");
  pRichHitMatchPropertiesDouble = new TNtuple("RichHitMatchPropertiesDouble","Properties of double rings in rich",
					      "dPhi:dTheta:pmq:padnr:centroid:locmax:avcharge:theta:phi:sector:neighbour:dalitz:gamma");
  pRichHitMatchPropertiesFake   = new TNtuple("RichHitMatchPropertiesFake","Properties of fake rings in rich",
					      "dPhi:dTheta:pmq:padnr:centroid:locmax:avcharge:theta:phi:sector:neighbour");


}
HPidRichTable::HPidRichTable(Text_t name[], Text_t title[]):
  HReconstructor( name,title){

  pRichHitMatchPropertiesSingle = new TNtuple("RichHitMatchPropertiesSingle","Properties of single rings in rich",
					      "dPhi:dTheta:pmq:padnr:centroid:locmax:avcharge:theta:phi:sector:neighbour:openingAngle:dalitz:gamma");
  pRichHitMatchPropertiesDouble = new TNtuple("RichHitMatchPropertiesDouble","Properties of double rings in rich",
					      "dPhi:dTheta:pmq:padnr:centroid:locmax:avcharge:theta:phi:sector:neighbour:dalitz:gamma");
  pRichHitMatchPropertiesFake   = new TNtuple("RichHitMatchPropertiesFake","Properties of fake rings in rich",
					      "dPhi:dTheta:pmq:padnr:centroid:locmax:avcharge:theta:phi:sector:neighbour");


}
HPidRichTable::~HPidRichTable(void){
  delete pRichHitMatchPropertiesSingle;
  delete pRichHitMatchPropertiesDouble;
  delete pRichHitMatchPropertiesFake;

}
Bool_t HPidRichTable::init(void){

  HEvent        *pEvent;
  HSpectrometer *pSpect;
  fGeometryPar = NULL;
  if(gHades == NULL)
    {
      Error("init", "gHades == NULL");
      return kFALSE;
    }
  
  if((pEvent = gHades->getCurrentEvent()) == NULL)
    {
      Error("init", "gHades->getCurrentEvent() == NULL");
      return kFALSE;
    }
  
  if((pSpect = gHades->getSetup()) == NULL)
    {
      Error("init", "gHades->getSetup() == NULL");
      return kFALSE;
    }
  
  if((pCatHitMatch = pEvent->getCategory(catMatchHit)) == NULL)
    {
      Error("init", "No catHitMatch");
      return kFALSE;
    }
  else
    pHitMatchIterator = (HIterator *)pCatHitMatch->MakeIterator();
    pInnerHitMatchIterator = (HIterator *)pCatHitMatch->MakeIterator();

  evtCount = 0;
  
  HRuntimeDb* rtdb=gHades->getRuntimeDb();
  pAnalysisPar = (HRichAnalysisPar*)rtdb->getContainer("RichAnalysisParameters");
  
  return kTRUE;
}

Bool_t  HPidRichTable::reinit(void){
  pPadsPar = ((HRichGeometryPar*)fGeometryPar)->getPadsPar();
  return kTRUE;
}

Int_t HPidRichTable::execute(void){
  evtCount++;
  
  //The number of lepton tracks on the mirror in angular correlation with the ring
  Int_t nCorrelatedLepOnMirror;
  Int_t isCorrelatedGeantLepton; //more than one detector!

  Float_t dThetaRM = 0;
  Float_t dThetaRMInner = 0;
  Float_t dPhiRM = 0;
  Float_t dPhiRMInner = 0;

  Float_t minRingOpeningAngle = 0.0;
  Float_t tempRingOpeningAngle = 0.0;

  Float_t RingCenterDistance=0.0;
  Float_t neighbour=0.0;
  Float_t InnerProduct=0.0;
  Float_t OuterProduct=0.0;

  if(evtCount%1000==0) cout<<"processing event "<<evtCount<<endl;
  
  if(evtCount%5000==0) 
    {
      outtmp= new TFile("pidNtuplesRichTmp.root","recreate");
      outtmp->cd();
      cout << "Writing temp data!" << endl;
      pRichHitMatchPropertiesSingle->Write();
      pRichHitMatchPropertiesDouble->Write();
      pRichHitMatchPropertiesFake->Write();
      outtmp->Close();
    }
  

  HHitMatchSim* pHitMatchObject = NULL;
  HHitMatchSim* pInnerHitMatchObject = NULL;
  pHitMatchIterator->Reset();
  pInnerHitMatchIterator->Reset();

  //loop to get rid of the rings that are included in more than
  //one hhitmatachsim object. Only the tracklet with the minimal
  //opening angle is left. The hit correlation flag from the others
  //is removed

  //Outer loop over hitmatch objects
  while((pHitMatchObject=(HHitMatchSim *)pHitMatchIterator->Next())){
    /*
      dPhiRM = (pHitMatchObject->getRichPhi() - pHitMatchObject->getMdcPhi())*
      TMath::Sin(pHitMatchObject->getMdcTheta()/57.3);
      dThetaRM = pHitMatchObject->getRichTheta() - pHitMatchObject->getMdcTheta();
    */
    //Inner loop over hitmatch objects. 
    //We avoid double counting of rings by removing correlations which refer to the same ring
    pInnerHitMatchIterator->Reset();
    while((pInnerHitMatchObject= (HHitMatchSim *)pInnerHitMatchIterator->Next())){
      /*      
       dPhiRMInner = (pInnerHitMatchObject->getRichPhi() - pInnerHitMatchObject->getMdcPhi())*
       TMath::Sin(pInnerHitMatchObject->getMdcTheta()/57.3);
       dThetaRMInner = pInnerHitMatchObject->getRichTheta() - pInnerHitMatchObject->getMdcTheta();
      */

      //If two hitmatches use the same ring, we remove the worse correlation by removing the flag indicating 
      //a correlation of rich and mdc
      if((pHitMatchObject->getRichInd()==pInnerHitMatchObject->getRichInd()))
	{
	  InnerProduct = TMath::Sin(pInnerHitMatchObject->getMdcTheta())*TMath::Sin(pInnerHitMatchObject->getMdcPhi())*
	    TMath::Sin(pInnerHitMatchObject->getRichTheta())*TMath::Sin(pInnerHitMatchObject->getRichPhi())+
	    TMath::Sin(pInnerHitMatchObject->getMdcTheta())*TMath::Cos(pInnerHitMatchObject->getMdcPhi())*
	    TMath::Sin(pInnerHitMatchObject->getRichTheta())*TMath::Cos(pInnerHitMatchObject->getRichPhi());
	  
	  OuterProduct = TMath::Sin(pHitMatchObject->getMdcTheta())*TMath::Sin(pHitMatchObject->getMdcPhi())*
	    TMath::Sin(pHitMatchObject->getRichTheta())*TMath::Sin(pHitMatchObject->getRichPhi())+
	    TMath::Sin(pHitMatchObject->getMdcTheta())*TMath::Cos(pHitMatchObject->getMdcPhi())*
	    TMath::Sin(pHitMatchObject->getRichTheta())*TMath::Cos(pHitMatchObject->getRichPhi());

	  if(InnerProduct<OuterProduct){
	    
	    //As the scalar product is a monotonuously decreasing function of the angle in the 
	    //considered range we can exclude the hitmatch object with the smaller SP from further 
	    //considerations.
	    pInnerHitMatchObject->setMatchedRichMdc(0);
	  }
	}

	  /*
	    if((pHitMatchObject->getRichInd()==pInnerHitMatchObject->getRichInd()) &&
	    ((TMath::Abs(dPhiRM))*(TMath::Abs(dThetaRM)))<((TMath::Abs(dPhiRMInner))*(TMath::Abs(dThetaRMInner)))) {
	    pInnerHitMatchObject->setMatchedRichMdc(0);
	    
	    
	    }
	  */      

    }
  }
  
  //Loop over all hitmatch objects and see whether zero, one or two tracks on the mirror matched the ring!
  pHitMatchIterator->Reset();
  
  while((pHitMatchObject = ( HHitMatchSim *)pHitMatchIterator->Next()) != NULL ){

    //check whether the matching flag has been removed from the hitmatch object 
    if( pHitMatchObject->getMatchedRichMdc())
      {
	//check whether this ring has at least one neighbour centered within a distance of twice the radius + 2 pads
	neighbour = 0.0;
	pInnerHitMatchIterator->Reset();
	while((pInnerHitMatchObject= (HHitMatchSim *)pInnerHitMatchIterator->Next())){
	  if(pInnerHitMatchObject->getRichInd()!=pHitMatchObject->getRichInd()){
	    //We are comparing different rings! OK!
	    RingCenterDistance=TMath::Sqrt(TMath::Power(pInnerHitMatchObject->getRingCenterX()-pHitMatchObject->getRingCenterX(),2.0)+
					   TMath::Power(pInnerHitMatchObject->getRingCenterY()-pHitMatchObject->getRingCenterY(),2.0));
	    if(RingCenterDistance<=(float)(2+2*pAnalysisPar->iRingRadius)) 
	      {
		neighbour = 1.0;
	      }
	  }
	  
	}
	//retrieve ring properties from the hitmatch object for filling the appropriate histogram;
	HTrackInfo * trackInfo = pHitMatchObject->getTrackInfoObj();
	if(!trackInfo){
	  exit(-1);
	}
	sector     = pHitMatchObject->getSector();
	theta      = pHitMatchObject->getRichTheta();
	phi        = pHitMatchObject->getRichPhi();
	deltaPhi   = (phi   - pHitMatchObject->getMdcPhi())* TMath::Sin(pHitMatchObject->getMdcTheta()/57.3);
	deltaTheta = theta - pHitMatchObject->getMdcTheta();
	pmq        = pHitMatchObject->getRingPatMat();
	padNr      = pHitMatchObject->getRingPadNr();
	locMax     = pHitMatchObject->getRingLocalMax4();
	centroid   = pHitMatchObject->getCentroid();
	avCharge   = (float)(pHitMatchObject->getRingAmplitude())/(float)(pHitMatchObject->getRingPadNr());
	
	nCorrelatedLepOnMirror  = pHitMatchObject->getLeptonOnMirror();
	isCorrelatedGeantLepton = pHitMatchObject->isGCLepRing();
	
	if(nCorrelatedLepOnMirror==1)
	  {
	    //fill the ntuple for ONE ring and ONE track
	    //the following loop checks if the single rings are surrounded
	    //by additional rings and in this case it calculates the opening angle
	    //If there are more than 1 "neighbours" the minimal opening
	    //angle is stored.
	    pInnerHitMatchIterator->Reset();
	    while((pInnerHitMatchObject= (HHitMatchSim *)pInnerHitMatchIterator->Next())){
	      if(pInnerHitMatchObject->getRichInd()!= 
		 pHitMatchObject->getRichInd()){
		tempRingOpeningAngle = HRichUtilFunc::calcOpeningAngleT(theta,phi,pInnerHitMatchObject->getRichTheta(),
									pInnerHitMatchObject->getRichPhi());
		if(tempRingOpeningAngle<minRingOpeningAngle|| minRingOpeningAngle==0) minRingOpeningAngle = tempRingOpeningAngle; 
	      }
	    }
	    pRichHitMatchPropertiesSingle->Fill(deltaPhi, deltaTheta,pmq,padNr,centroid,locMax,avCharge,
						theta,phi,sector,neighbour,minRingOpeningAngle,
						checkDalitz(trackInfo),checkConv(trackInfo));	  
	  }
	if(nCorrelatedLepOnMirror==2)
	  {
	    //fill the ntuple for ONE ring and TWO tracks
	    pRichHitMatchPropertiesDouble->Fill(deltaPhi, deltaTheta,pmq,padNr,centroid,locMax,avCharge,
						theta,phi,sector,neighbour,checkDalitz(trackInfo),checkConv(trackInfo));
	  }
	if(nCorrelatedLepOnMirror==0)
	  {
	    //fill the ntuple for fake rings
	    pRichHitMatchPropertiesFake->Fill(deltaPhi, deltaTheta,pmq,padNr,centroid,locMax,avCharge,theta,phi,sector,neighbour);
	  }
      }
  }
  return 0;
}

Float_t HPidRichTable::checkDalitz(HTrackInfo *tOb){
    for(int i = 0;i<5;i++){
	if(tOb->getPi0Dalitz(i)==1) return tOb->getRichWeight(i);
    }
    return 0;
}
Float_t HPidRichTable::checkConv(HTrackInfo *tOb){
    for(int i = 0;i<5;i++){
	if(tOb->getConvLep(i)==1) return tOb->getRichWeight(i);
    }
    return 0;
}
Bool_t HPidRichTable::finalize(void){
  outtmp->Close();
  out= new TFile(".pidNtuplesRich.root","recreate");
  out->cd();
  pRichHitMatchPropertiesSingle->Write();
  pRichHitMatchPropertiesDouble->Write();
  pRichHitMatchPropertiesFake->Write();
  out->Close();

  return kTRUE;
};
