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
#include "kickdef.h"
#include "richdef.h"
#include "hmdcdef.h"
#include "hgeantrich.h"
#include "hruntimedb.h"
#include "TMath.h"
#include "hhitmatchsim.h"
#include "htrackinfo.h"
#include "hrichutilfunc.h"

ClassImp(HPidRichTable)
  HPidRichTable::HPidRichTable(void) : HReconstructor("RichPidTable","table")
{
 
  pRichHitMatchPropertiesSingle = new TNtuple("RichHitMatchPropertiesSingle","Properties of single rings in rich",
					      "pmq:padnr:centroid:locmax:avcharge:theta:phi:sector:openingAngle:dalitz:gamma");
  pRichHitMatchPropertiesDouble = new TNtuple("RichHitMatchPropertiesDouble","Properties of Double_t rings in rich",
					      "pmq:padnr:centroid:locmax:avcharge:theta:phi:sector:dalitz:gamma");
  pRichHitMatchPropertiesFake   = new TNtuple("RichHitMatchPropertiesFake","Properties of fake rings in rich",
					      "pmq:padnr:centroid:locmax:avcharge:theta:phi:sector");


}
HPidRichTable::HPidRichTable(const Text_t name[],const Text_t title[]):
  HReconstructor( name,title){

  pRichHitMatchPropertiesSingle = new TNtuple("RichHitMatchPropertiesSingle","Properties of single rings in rich",
					      "pmq:padnr:centroid:locmax:avcharge:theta:phi:sector:openingAngle:dalitz:gamma");
  pRichHitMatchPropertiesDouble = new TNtuple("RichHitMatchPropertiesDouble","Properties of Double_t rings in rich",
					      "pmq:padnr:centroid:locmax:avcharge:theta:phi:sector:dalitz:gamma");
  pRichHitMatchPropertiesFake   = new TNtuple("RichHitMatchPropertiesFake","Properties of fake rings in rich",
					      "pmq:padnr:centroid:locmax:avcharge:theta:phi:sector");


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
    pHitMatchIterator2 = (HIterator *)pCatHitMatch->MakeIterator();

  evtCount = 0;
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
  Float_t dThetaRM1 = 0;
  Float_t dPhiRM = 0;
  Float_t dPhiRM1 = 0;

  Float_t ringOpAngle[100];
  Int_t counter = 0;

  for(Int_t i=0;i<100;i++){
       ringOpAngle[i] = 0.;
  }

  if(evtCount%1000==0) cout<<" processing event "<<evtCount<<endl;

  HHitMatchSim* pHitMatchObject = NULL;
  HHitMatchSim* pHitMatchObject1 = NULL;
  pHitMatchIterator->Reset();
  pHitMatchIterator2->Reset();

  //loop to get rid of the rings that are included in more than
  // one hhitmatachsim object. Only the tracklet with the minimal
  // Dphi*Dtheta is left.

  while((pHitMatchObject= (HHitMatchSim *)pHitMatchIterator->Next())){
      dPhiRM = (pHitMatchObject->getRichPhi() - pHitMatchObject->getMdcPhi())*
	  TMath::Sin(pHitMatchObject->getMdcTheta()/57.3);
      dThetaRM = pHitMatchObject->getRichTheta() - pHitMatchObject->getMdcTheta();
      pHitMatchIterator2->Reset();
      while((pHitMatchObject1= (HHitMatchSim *)pHitMatchIterator2->Next())){
	  dPhiRM1 = (pHitMatchObject1->getRichPhi() - pHitMatchObject1->getMdcPhi())*
	      TMath::Sin(pHitMatchObject1->getMdcTheta()/57.3);
	  dThetaRM1 = pHitMatchObject1->getRichTheta() - pHitMatchObject1->getMdcTheta();
	  if((pHitMatchObject->getRichInd()==pHitMatchObject1->getRichInd()) &&
	     ((TMath::Abs(dPhiRM))*(TMath::Abs(dThetaRM)))<((TMath::Abs(dPhiRM1))*(TMath::Abs(dThetaRM1)))) {
	      pHitMatchObject1 ->setMatchedRichMdc(0);
	      //	break;
	  }
	  
      }
  }
  
  //Loop over all hitmatch objects which show up in more than one detector (isGCLeptonRing)
  //and see whether zero, one or two tracks on the mirror matched the ring!
  
 pHitMatchIterator->Reset();

  HRichUtilFunc func;
  while((pHitMatchObject = ( HHitMatchSim *)pHitMatchIterator->Next()) != NULL ){
      if( pHitMatchObject->getMatchedRichMdc())
      {
	  //retrieve ring properties from the hitmatch object for filling the appropriate histogram;
	  HTrackInfo * trackInfo = pHitMatchObject->getTrackInfoObj();
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
	  if(pmq>200 && padNr > 5 && centroid < 2.8 && avCharge >4 && avCharge < 40 && 
	     deltaTheta >-1.8 && deltaTheta<1.8 &&
	     deltaPhi >-1.7 &&deltaPhi<1.7){
	      if(nCorrelatedLepOnMirror==1)
	      {
		  //fill the ntuple for ONE ring and ONE track
		  //	  cout << "filling one" << endl;
		  
		  //the following loop checks if the single rings are sorrounded
		  // by additional rings and in this case it calculates the opening angle
		  // If there are more than 1 "neighbours" the minimal opening
		  // angle is stored.
		  counter++;	      
		  pHitMatchIterator2->Reset();
		  while((pHitMatchObject1= (HHitMatchSim *)pHitMatchIterator2->Next())){
		      if(pHitMatchObject1->getRichInd()!= 
			 pHitMatchObject->getRichInd()){
			  Float_t tempOpAngle = func.calcOpeningAngleT(theta,phi,pHitMatchObject1->getRichTheta(),pHitMatchObject1->getRichPhi());
			  if(tempOpAngle<ringOpAngle[counter]|| ringOpAngle[counter]==0) ringOpAngle[counter] = tempOpAngle; 
		      }
		  }
		  pRichHitMatchPropertiesSingle->Fill(pmq,padNr,centroid,locMax,avCharge,theta,phi,sector,ringOpAngle[counter],checkDalitz(trackInfo),checkConv(trackInfo));	  
	      }
	      if(nCorrelatedLepOnMirror==2)
	      {
		  //fill the ntuple for ONE ring and TWO tracks
		  //cout << "filling two" << endl;
		  pRichHitMatchPropertiesDouble->Fill(pmq,padNr,centroid,locMax,avCharge,theta,phi,sector,checkDalitz(trackInfo),checkConv(trackInfo));
	      }
	      if(nCorrelatedLepOnMirror==0)
	  {
	      //fill the ntuple for fake rings
	      //cout << "filling zero" << endl;
	      pRichHitMatchPropertiesFake->Fill(pmq,padNr,centroid,locMax,avCharge,theta,phi,sector);
	  }
      }
    }
  }
  return 0;
}
Float_t HPidRichTable::checkDalitz(HTrackInfo *tOb){
    for(Int_t i = 0;i<5;i++){
	if(tOb->getPi0Dalitz(i)==1) return tOb->getRichWeight(i);
    }
    return 0;
}
Float_t HPidRichTable::checkConv(HTrackInfo *tOb){
    for(Int_t i = 0;i<5;i++){
	if(tOb->getConvLep(i)==1) return tOb->getRichWeight(i);
    }
    return 0;
}
Bool_t HPidRichTable::finalize(void){
      TFile* out= new TFile("pidNtuples.root","recreate");
  out->cd();
  //cout << "writing ntuples" << endl;
  pRichHitMatchPropertiesSingle->Write();
  pRichHitMatchPropertiesDouble->Write();
  pRichHitMatchPropertiesFake->Write();
  out->Close();
  return kTRUE;
};
