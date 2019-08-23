// File: hpidleptoneff.cc
//
// Author: Laura Fabbietti <L.Fabbietti@physik.tu-muenchen.de>
// Last update by Laura Fabbietti: 05/06/05 21:47:04
//

#include "hpidsingleleptoneff.h"
#include "hades.h"
#include "hevent.h"
#include "piddef.h"
#include "hrichhit.h"
#include "iostream"
#include "hmdcseg.h"
#include "hgeantkine.h"
#include <TH1.h>
#include <TH3.h>
#include <TMath.h>
#include "hgeomvector2.h"
#include "hgeomvector.h"
#include "hpidfl.h"
#include <hruntimedb.h>
#include "hpairsim.h"
#include "hpairdata.h"
#include "hpairgeantdata.h"
#include "hpaircutpar.h"
#include "hpidparticlesim.h"

ClassImp(HPidSingleLeptonEff)
// -----------------------------------------------------------------------------
HPidSingleLeptonEff::HPidSingleLeptonEff(char * file)
                    : HReconstructor("PidPartFiller",
                                     "Filler of HPidParticle category")
{
    simYieldEle = new TNtuple("simYieldEle","simYieldEle","p:theta:phi:pt");
    simYieldPosi = new TNtuple("simYieldPosi","simYieldPosi","p:theta:phi:pt");
    effEle = new TNtuple("effEle","effEle","pRecon:pGeant:thetaRecon:thetaGeant:phiRecon:phiGeant:ptGeant:isCut:isCP");
    effPosi = new TNtuple("effPosi","effPosi","pRecon:pGeant:thetaRecon:thetaGeant:phiRecon:phiGeant:ptGeant:isCut:isCP");
    
    acceEle = new TNtuple("acceEle","acceEle","p:theta:phi:pt");
    accePosi = new TNtuple("accePosi","accePosi","p:theta:phi:pt");
   
    out = new TFile(file,"recreate");
  
    
  
    //cout<<" kindOFPair  "<<kindOFPair.Data()<<endl;
}
// -----------------------------------------------------------------------------
HPidSingleLeptonEff::~HPidSingleLeptonEff(){

    delete simYieldEle;
    delete simYieldPosi;
    delete effEle;
    delete effPosi;
   
    delete acceEle;
    delete accePosi;


}
// -----------------------------------------------------------------------------
HPidSingleLeptonEff::HPidSingleLeptonEff(Text_t name[], Text_t title[],Char_t * file)
                    : HReconstructor(name,title)
{    
    simYieldEle = new TNtuple("simYieldEle","simYieldEle","p:theta:phi:pt");
    simYieldPosi = new TNtuple("simYieldPosi","simYieldPosi","p:theta:phi:pt");    
    effEle = new TNtuple("effEle","effEle","pRecon:pGeant:thetaRecon:thetaGeant:phiRecon:phiGeant:ptGeant:isCut:isCP");
    effPosi = new TNtuple("effPosi","effPosi","pRecon:pGeant:thetaRecon:thetaGeant:phiRecon:phiGeant:ptGeant:isCut:isCP");
    acceEle = new TNtuple("acceEle","acceEle","p:theta:phi:pt");
    accePosi = new TNtuple("accePosi","accePosi","p:theta:phi:pt");
    
    out = new TFile(file,"recreate");
   
}
// -----------------------------------------------------------------------------
Bool_t HPidSingleLeptonEff::init(void){

  if((pInputCat1 = gHades->getCurrentEvent()->getCategory(catPidPart))
                == NULL)
    {
        Error("init", "No category catPidPart");
        return kFALSE;
    }

  pidPartInput = (HIterator *)pInputCat1->MakeIterator();

    if((pInputCat = gHades->getCurrentEvent()->getCategory(catPairs))
                == NULL)
    {
        Error("init", "No category catPidPart");
        return kFALSE;
    }

    if((pitInput = (HIterator *)pInputCat->MakeIterator()) == NULL)
    {
        Error("init", "Cannot make an iterator for HPidparticle category");

        return kFALSE;
    }

    pGeantCat = gHades->getCurrentEvent()->getCategory(catGeantKine);
    if(!pGeantCat){
        Error("init", "No category catGeantKine ");
        return kFALSE;
    }
    pitGeant = (HIterator *)pGeantCat->MakeIterator();
    pitGeant1 = (HIterator *)pGeantCat->MakeIterator();
    

    HRuntimeDb* rtdb = gHades->getRuntimeDb();
    if (rtdb)   pC  = (HPairCutPar*)rtdb->findContainer("PairCutParameters");  
    
  return kTRUE;
}
Int_t HPidSingleLeptonEff::execute(void){

    // All the lepton pairs contained in the GeantKine container are formed
    // The yield is stored in a ntuple (simYieldN) as a function of the pair mass
    // rapidity and transverse momentum. Moreover the lepton pairs in the acceptance
    // are stored in a ntuple (accept). 
    // The HPidDilepton class contains all the pairs that have been reconstructed.
    // For each of the GeantKine pair in the acceptance we checked if this pair 
    // has been properly
    // recontructed and is contained in the HPidDilepton Container.
    // For the first correctly recontructed pair a second ntuple (eff) will
    // be filled with weight 1, for each additional reconstructed pair
    // that dont correpond to any Geant pair, a third ntuple (fake) will be filled
    // with weight 1.
    // All these informations will be stored in Ntuples  to allow
    // later on the user to create of efficiency matrixes with different bining.
    //cout<<" executing EFF **********************************************"<<endl;
    HGeantKine *pKine;
   
    Int_t aParLepton, aMedLepton, aMechLepton;
   
    Int_t aTrackLepton, aIDLepton;
   


    Int_t * partIndex = new Int_t[8];
    for(int i = 0;i<8;i++) partIndex[i] = 0;
    Int_t countLep = 0;
    removecloseTracks(partIndex);

    pitGeant->Reset();

    while((pKine = (HGeantKine*) pitGeant->Next()) != NULL){


	pKine->getParticle(aTrackLepton,aIDLepton);
	 pKine->getCreator(aParLepton,aMedLepton,aMechLepton);
	 if(aParLepton==0&&(aIDLepton==2||aIDLepton==3)){
	     Float_t xMom,yMom,zMom;
	     TVector3 pD;
	     pKine->getMomentum(xMom,yMom,zMom);
	     pD.SetXYZ(xMom,yMom,zMom);
	     if(aIDLepton==3) simYieldEle->Fill(pD.Mag(),pD.Theta(),pD.Phi(),pD.Pt()); 
	     if(aIDLepton==2) simYieldPosi->Fill(pD.Mag(),pD.Theta(),pD.Phi(),pD.Pt()); 
	     // cout<<"  partIndex[countLep]  "<<partIndex[countLep]<<"  countLep  "<<countLep<<endl;
	     if (partIndex[countLep]==0) checkEff(pKine);
	     countLep++;
	
	 }
    }
    

    // all the lepton pairs that dont matches to a geant pair are counted as
    // fakes.
  
    delete [] partIndex;
    return 0;
}
void  HPidSingleLeptonEff::removecloseTracks(Int_t *p){

    // all the tracks coming fomr the white generator that has a neighbour
    // also coming from the white generator that is closer thatn 9 deg are tagged
    HGeantKine *pKine=NULL;
    HGeantKine *pKine1=NULL;
    pitGeant->Reset();
    
    Int_t aTrackLepton,aIDLepton,aTrackLepton1,aIDLepton1;
    Int_t  aParLepton,aMedLepton,aMechLepton,aParLepton1,aMedLepton1,aMechLepton1;
    Int_t countLep = -1;

    while((pKine = (HGeantKine*) pitGeant->Next()) != NULL){

	pKine->getParticle(aTrackLepton,aIDLepton);
	 pKine->getCreator(aParLepton,aMedLepton,aMechLepton);

	 if(aParLepton==0&&(aIDLepton==2||aIDLepton==3)){

	     countLep++;
	     ///	 cout<<" looping on the first electron "<<countLep<<endl;
	     pitGeant1->Reset();
	     while((pKine1 = (HGeantKine*) pitGeant1->Next()) != NULL){
		pKine1->getParticle(aTrackLepton1,aIDLepton1);
		pKine1->getCreator(aParLepton1,aMedLepton1,aMechLepton1);
		if(aParLepton1==0&&(aIDLepton1==2||aIDLepton1==3) &&
		       aTrackLepton!=aTrackLepton1) {
		    // cout<<"  lepton 1 Track "<<aTrackLepton<<" lepton 2 Track "<<
		    //aTrackLepton1<<" opening angle  "<<calcOpeningAngleKine(pKine,pKine1)<<endl; 
		    if (calcOpeningAngleKine(pKine,pKine1)<9.5){
			p[countLep] =1;
			//	cout<<"  small opening angle betweeb track "<<aTrackLepton<<"  and track  "<<aTrackLepton1<<endl;
		    }
		}
	     }
	 }
    }
    
}
Float_t HPidSingleLeptonEff::calcOpeningAngleKine(HGeantKine *kine1,HGeantKine *kine2)
{
    //input 2 kine objects
    //output opening angle of trajectories

    Double_t rad2deg = TMath::RadToDeg();

    HGeomVector vec1;
    if (kine1){
    Float_t xMom1,yMom1,zMom1;
    kine1->getMomentum(xMom1,yMom1,zMom1);
    vec1.setX(xMom1);
    vec1.setY(yMom1);
    vec1.setZ(zMom1); 
    
    vec1/=vec1.length();//norm
    }
    HGeomVector vec2;
    if (kine2){
    Float_t xMom2,yMom2,zMom2;
    kine2->getMomentum(xMom2,yMom2,zMom2);
    vec2.setX(xMom2);
    vec2.setY(yMom2);
    vec2.setZ(zMom2);
    
    vec2/=vec2.length();//norm
    }
    
    Float_t cosfOpeningAngle = vec1.scalarProduct(vec2);
    //cout<<cosfOpeningAngle<<endl;
    Float_t fOpeningAngle=0.;
    if (TMath::Abs(cosfOpeningAngle) <= 1) 
	fOpeningAngle=TMath::ACos(cosfOpeningAngle) * rad2deg;
    
    return fOpeningAngle;
}

void HPidSingleLeptonEff::checkEff(HGeantKine *p1){
    Float_t mom1;
    mom1 = 0;
    Int_t trackNb, id;
    TVector3 pD1;
    Float_t xMom1,yMom1,zMom1;
    p1->getMomentum(xMom1,yMom1,zMom1);
   
    p1->getParticle(trackNb,id);
    pD1.SetXYZ(xMom1,yMom1,zMom1);
    mom1 = TMath::Sqrt( xMom1*xMom1 + yMom1*yMom1 + zMom1*zMom1);
    //    cout<<"  form a pair  +++++++++++++++++++++++++++++++++++++"<<endl;  
    
    // cout<<" mech  "<<mech<<"  medium "<<medium<<endl;
     
     //fun.calcPairPt(p1,p2),1);
    
     if(isGeantTrackInAcceptance(p1)) {
	 if(id==2) accePosi->Fill(mom1,pD1.Theta(),pD1.Phi(),pD1.Pt());
	 if(id==3) acceEle->Fill(mom1,pD1.Theta(),pD1.Phi(),pD1.Pt());	
	 // cout<<"  electron in the acceptance *********************"<<endl;	 
     }

     //loop to label the pairs that are recosntructed twice.
     //pairLoopToCompareMom();
     //  checking
 

     // checking
     HPairSim * pair=NULL;
     pitInput->Reset();
     Int_t cpFlag = 0;
     Int_t flagPair = 0;
     Float_t thetaExp, phiExp, momExp;
     thetaExp = phiExp = momExp = 0.;
    while(( pair= (HPairSim  *)pitInput->Next()) != NULL)
    {
	if(checkTrackId(pair,trackNb)){
	    calculateTrackProperties(pair,thetaExp, phiExp, momExp,cpFlag);
	    
	   //	   checkMomenta(p1,p2,dil)){
	     if(id==3) effEle->Fill(momExp,mom1,thetaExp,pD1.Theta(),phiExp,pD1.Phi(),pD1.Pt(),pair->getIsCutFlag(),cpFlag);
	     if(id==2) effPosi->Fill(momExp,mom1,thetaExp,pD1.Theta(),phiExp,pD1.Phi(),pD1.Pt(),pair->getIsCutFlag(),cpFlag);
	   //   cout<<" id "<<id<<" momExp  "<<momExp<< "   mom1  "<<mom1<<"  thetaExp "<<
// 	     thetaExp<<"  pD1.Theta()  "<< pD1.Theta()<<"  phiExp "<<phiExp<<
// 		 "  pD1.Phi()  "<<pD1.Phi()<<"  pD1.Pt()  "<<pD1.Pt()<<
// 		 "  pair->getIsCutFlag()  "<<pair->getIsCutFlag()<<
// 		 "  cpFlag)  "<<cpFlag<<endl;
	     flagPair= 1;
	    break; // only the first pair that matches is filled in the eff.
	    // matrxi, the rest is fake even if it matches.
	}
	
    }
    if(flagPair==0){
    HPidParticleSim * part=NULL;
    pidPartInput->Reset();
    // if there is no combination between the white lepton and the
    // leptons from the background, they still can be seen as asingle tracks.   
    while(( part= ( HPidParticleSim *)pidPartInput->Next()) != NULL)
      {
	if(trackNb==part->getGeantTrackSet()->getGeantTrackID()&&
	   part->getGeantTrackSet()->getGeantParentTrackID()==0&&
	   part->getGeantTrackSet()->getMostCommonCorrelation()>=76){
	  if(id==3) effEle->Fill(part->P(),mom1,part->Theta(),pD1.Theta(),part->Phi(),pD1.Phi(),pD1.Pt(),-1,-1);
	  if(id==2) effPosi->Fill(part->P(),mom1,part->Theta(),pD1.Theta(),part->Phi(),pD1.Phi(),pD1.Pt(),-1,-1);
	  break;  
	  // cout<<"  ****************************************   "<<endl;
	  // 	  cout<< part->getGeantTrackSet()->getRichTrack(0)<<"  "<<
	  // 	    part->getGeantTrackSet()->getMostCommonCorrelation()<<"   "<<
	  // 	    part->getGeantTrackSet()->getGeantTrackID()<<"   "<<
	  // 	    part->getGeantTrackSet()->getGeantParentTrackID()<<endl;
	}
      }
    }
}
Bool_t  HPidSingleLeptonEff::checkTrackId(HPairSim *p,Int_t lepTrackNb){
    HPairGeantData pg(p);
    //     cout<< "  checkTrackId   "<<endl;
    //cout<<" pg.nGParentTrackNumber1  "<< pg.nGParentTrackNumber1<<
 // 	" pg.sGCommonDetectors1  "<<pg.sGCommonDetectors1<<
//  	"  pg.nGTrackNumber1  "<<pg.nGTrackNumber1<<"  lepTrackNb  "<<lepTrackNb<<endl;
//      cout<<" pg.nGParentTrackNumber2  "<< pg.nGParentTrackNumber2<<
//  	"  pg.sGCommonDetectors2  "<<pg.sGCommonDetectors2<<
//  	"  pg.nGTrackNumber2  "<<pg.nGTrackNumber2<<"  lepTrackNb  "<<lepTrackNb<<endl;

    
    if(pg.getParentTrackNumber1()==0 && pg.getCommonDetectors1()>=76 &&
      pg.getTrackNumber1()==lepTrackNb ){
	if(pg.getParentTrackNumber2()!=0) {
	    // cout<<" fisrt case  "<<endl;
	    return kTRUE;
	}
	else return kFALSE;

    }
    else if(pg.getParentTrackNumber2()==0 && pg.getCommonDetectors2()>=76 &&
      pg.getTrackNumber2()==lepTrackNb ){
	if(pg.getParentTrackNumber1()!=0) {
	    //cout<<" second case  "<<endl;
	    return kTRUE;
	}
	else return kFALSE;
    }
    return kFALSE;
}
void HPidSingleLeptonEff::calculateTrackProperties(HPairSim *pair,Float_t& th,Float_t& ph,Float_t& p,Int_t cpF){
    HPairGeantData pg(pair);
    HPairData pd(pair);
    if(pg.getParentTrackNumber1()==0){
	th = pd.getThetaDeg1();
	ph= pd.getPhiDeg1();
	p= pd.getMom1();
	cpF= pd.getIsCPCandidate1();
    }
    else if(pg.getParentTrackNumber2()==0){
	th = pd.getThetaDeg2();
	ph= pd.getPhiDeg2();
	p= pd.getMom2();
	cpF= pd.getIsCPCandidate2();
    }

}
Bool_t HPidSingleLeptonEff::isGeantTrackInAcceptance(HGeantKine *pG){
 //getNMdcHits getNTofHits can be used only if MDC/Tof Geant categories
 // are available
    // cout<<"  mdc hits   "<<pG->getNMdcHits()<<"   Tof hits  "<<pG->getNTofHits()<<endl;
    //cout<<"  mdc hits   "<<pG->getFirstMdcHit()<<"   Tof hits  "<<pG->getFirstTofHit()<<endl;

   // if((pG->getNMdcHits())>0 && (pG->getNTofHits())>0)
   if((pG->getFirstMdcHit())>-1 && (pG->getFirstTofHit())>-1)
	 return kTRUE;
     else return kFALSE;

     
 }

Bool_t  HPidSingleLeptonEff::finalize(){
    cout<<" finalize "<<endl;
    out->cd();
    simYieldEle->Write();
    simYieldPosi->Write();
    effEle->Write();
    effPosi->Write();
    
    acceEle->Write();
    accePosi->Write();
    out->Close();
    
    return kTRUE;   
}
