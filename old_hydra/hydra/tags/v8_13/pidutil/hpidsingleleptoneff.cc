// $Id: hpidsingleleptoneff.cc,v 1.8 2007-05-24 10:39:58 jacek Exp $
// Author: Laura Fabbietti <L.Fabbietti@physik.tu-muenchen.de>
// Last update by Laura Fabbietti: 05/06/05 21:47:04
// Modified in March 2006 by W. Przygoda / T. Wojcik (Cracow)
// Thomas.Eberl@ph.tum.de, last modified : 2006-11-08 10:42:49
// 
// This task is used to generate ntuples of embedded leptons 
// with white distributions in 1/p, theta, phi.
// There are ntuples with such leptons in the acceptance and 
// additionally identified and accepted leptons.
// These ntuples are later divided by each other (external code)
// to get (p,theta,phi) distributions of reconstruction efficiency
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
#include "hpairfl.h"

ClassImp(HPidSingleLeptonEff)
// -----------------------------------------------------------------------------
HPidSingleLeptonEff::HPidSingleLeptonEff(char * file)
                    : HReconstructor("PidPartFiller",
                                     "Filler of HPidParticle category")
{
    out = new TFile(file,"recreate");
  bookNTuple(); 

  
    
  
    //cout<<" kindOFPair  "<<kindOFPair.Data()<<endl;
}
// -----------------------------------------------------------------------------
HPidSingleLeptonEff::~HPidSingleLeptonEff(){

    /*delete simYieldEle;
    delete simYieldPosi;
    delete effEle;
    delete effPosi;
    delete fakeEle;
    delete fakePosi;
    delete acceEle;
    delete accePosi;*/

    delete out;

}
// -----------------------------------------------------------------------------
HPidSingleLeptonEff::HPidSingleLeptonEff(Text_t name[], Text_t title[],Char_t * file)
                    : HReconstructor(name,title)
{    
    out = new TFile(file,"recreate");
    bookNTuple();
   
}

void HPidSingleLeptonEff::bookNTuple(){

    simYieldEle = new TNtuple("simYieldEle","simYieldEle","p:theta:phi:pt");
    simYieldPosi = new TNtuple("simYieldPosi","simYieldPosi","p:theta:phi:pt");   
    TString sVarList("pRecon:pGeant:thetaRecon:thetaGeant:phiRecon:phiGeant:ptGeant:isCut:isCP:commonTrack:isDirect:cutNb:nHadron:nLepton");

    effEle = new TNtuple("effEle","effEle",sVarList);
    effPosi = new TNtuple("effPosi","effPosi",sVarList);
    fakeEle = new TNtuple("fakeEle","fakeEle",sVarList);
    fakePosi = new TNtuple("fakePosi","fakePosi",sVarList);
    acceEle = new TNtuple("acceEle","acceEle","p:theta:phi:pt");
    accePosi = new TNtuple("accePosi","accePosi","p:theta:phi:pt");

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

    if((pitInput1 = (HIterator *)pInputCat->MakeIterator()) == NULL)
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
    
// --------- MDC GEANT category
    
    pMdcGeantCat = gHades->getCurrentEvent()->getCategory(catMdcGeantRaw);
    if(!pMdcGeantCat){
        Error("init", "No category catMdcGeantRaw ");
        return kFALSE;
    }
    pitMdcGeant = (HIterator *)pMdcGeantCat->MakeIterator();

// --------- TOF GEANT category

    pTofGeantCat = gHades->getCurrentEvent()->getCategory(catTofGeantRaw);
    if(!pTofGeantCat){
        Error("init", "No category catTofGeantRaw ");
        return kFALSE;
    }
    pitTofGeant = (HIterator *)pTofGeantCat->MakeIterator();

// --------- SHOWER GEANT category

    pShowerGeantCat = gHades->getCurrentEvent()->getCategory(catShowerGeantRaw);
    if(!pShowerGeantCat){
        Error("init", "No category catTofGeantRaw ");
        return kFALSE;
    }
    pitShowerGeant = (HIterator *)pShowerGeantCat->MakeIterator();

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
    //   cout<<" executing EFF *******************************"<<endl;
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
		  //		     cout<<"  lepton 1 Track "<<aTrackLepton<<" lepton 2 Track "<<
 //		    aTrackLepton1<<" opening angle  "<<calcOpeningAngleKine(pKine,pKine1)<<endl; 
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
  //  cout<<"  form a pair  +++++++++++++++++++++++++++++++++++++"<<endl;  
    
     //cout<<" mech  "<<mech<<"  medium "<<medium<<endl;
     
     //fun.calcPairPt(p1,p2),1);
    
     if(isGeantTrackInAcceptance(p1)) {
	 if(id==2) accePosi->Fill(mom1,pD1.Theta(),pD1.Phi(),pD1.Pt());
	 if(id==3) acceEle->Fill(mom1,pD1.Theta(),pD1.Phi(),pD1.Pt());	
	 //  cout<<"  electron in the acceptance *********************"<<endl;	 
     }

     //loop to label the pairs that are recosntructed twice.
     //pairLoopToCompareMom();
     //  checking
     Int_t comDet = 0;
    //
    //Check for direct cuts 
     Int_t nLepCount=0;
     Int_t nHadCount=0;
     Int_t iBit=0;
     HPairSim * pair1=NULL;
     pitInput1->Reset();
    Int_t  bDirectCut=0;
    while(( pair1= (HPairSim  *)pitInput1->Next()) != NULL)
    {
        comDet = checkTrackId(pair1,trackNb);
        if(comDet>=76){
            if(pair1->getPid1()<0 || pair1->getPid2()<0) nHadCount++;
            if(pair1->getPid1()>0 && pair1->getPid2()>0){
                 nLepCount++;
            }
            if(pair1->getPid1()>0 || pair1->getPid2()>0){
                iBit=(Int_t)pair1->getBitFieldPassedCuts();
                //printf("BIT=%d \n",iBit);
//                if( HPairFL::getBit((UChar_t)2,iBit)==1 ||  
  //                  HPairFL::getBit((UChar_t)3,iBit)==1){
                if(pair1->getIsCutFlag()!=0){
                    bDirectCut=1;
                }
            }
        }
    }
   // PRINTF("%d %d\n",nHadCount,nLepCount);
    //Counts number of leptons and hadrons in one event

     // checking
     HPairSim * pair=NULL;
     pitInput->Reset();
     Int_t cpFlag = 0;
     Int_t flagPair = 0;
     Float_t thetaExp, phiExp, momExp;
     thetaExp = phiExp = momExp = 0.;
     
     Int_t isGoodPair = 0;
     
     while(( pair= (HPairSim  *)pitInput->Next()) != NULL)
       {
	 //  HPairGeantData pg(pair);
	 //       cout<<"  cheasing lepTrackNb  "<<trackNb<<endl;
	 //       cout<<" pg.nGParentTrackNumber1  "<< pg.getParentTrackNumber1()<<
	 //    	" pg.sGCommonDetectors1  "<<pg.getCommonDetectors1()<<
	 //        "  pg.nGTrackNumber1  "<<pg.getTrackNumber1()<<endl;
	 
      
	 //        cout<<" pg.nGParentTrackNumber2  "<< pg.getParentTrackNumber2()<<
	 //    	"  pg.sGCommonDetectors2  "<<pg.getCommonDetectors2()<<
	 //   	"  pg.nGTrackNumber2  "<<pg.getTrackNumber2()<<endl;
	 //        cout<<"  cutPair  "<<(int)pair->getIsCutFlag()<<endl;
      //        cout<<"  new pair ********************  "<<endl;
	 
	 comDet = checkTrackId(pair,trackNb); 
	 //if(comDet>0&&checkLeptonId(pair)){//only lepton-lepton pairs are checked
	 if(comDet>0){//only lepton-lepton pairs are checked
	   calculateTrackProperties(pair,thetaExp, phiExp, momExp,cpFlag);
	   flagPair= 1;
	   if (pair->getIsCutFlag()==0){
               Float_t aVar[]={momExp,mom1,thetaExp,pD1.Theta(),phiExp,pD1.Phi(),pD1.Pt(),0,cpFlag,comDet,bDirectCut,0,nHadCount,nLepCount};
	     //    cout<<" comDet   "<<comDet<<endl;
	     if(comDet>=76 && isGoodPair==0 ){
	       if(id==3) effEle->Fill(aVar);
	       if(id==2) effPosi->Fill(aVar);
            if(pair->getIsCutFlag()==0){
    	       isGoodPair=1;
            }
	     }
	     if(comDet<76){
               Float_t aVar[]={momExp,mom1,thetaExp,pD1.Theta(),phiExp,pD1.Phi(),pD1.Pt(),0,cpFlag,comDet,bDirectCut,0,nHadCount,nLepCount};
	       if(id==3) fakeEle->Fill(aVar);
	       if(id==2) fakePosi->Fill(aVar);
	       
	     }
	   }
	   
	   //cout<<" lepton in the pair   "<<endl;
	   //	break; // only the first pair that matches is filled in the eff.
	   // matrxi, the rest is fake even if it matches.
	 }
	 
       }
     // only the electron that form a pair that survives all the cuts (  pairCpFlag[i] ==0) are stores in the ntuple.
     
     if(flagPair==0){
       HPidParticleSim * part=NULL;
       pidPartInput->Reset();
       // if there is no combination between the white lepton and the
       // leptons from the background, they still can be seen as asingle tracks.   
       while(( part= ( HPidParticleSim *)pidPartInput->Next()) != NULL)
	 {
	   //	for(int k = 0; k<part->getGeantTrackSet()->getNCorrelatedTrackIds();k++)
	   //	  cout<<"  corrFlag  "<<part->getGeantTrackSet()->getCorrelationFlag(k)<<"   at k  "<<k<<endl;
	   if(trackNb==part->getGeantTrackSet()->getGeantTrackID()&&
	      part->getGeantTrackSet()->getGeantParentTrackID()==0&&
	      part->getGeantTrackSet()->getMostCommonCorrelation()>=76&&
	      (part->getPid()==2 || part->getPid()==3)){
           Float_t aVar[] = {part->P(),mom1,part->Theta(),pD1.Theta(),part->Phi(),pD1.Phi(),pD1.Pt(),-1,-1,part->getGeantTrackSet()->getMostCommonCorrelation(),-1,0,-1,-1};;
	     if(id==3) effEle->Fill(aVar);
	     if(id==2) effPosi->Fill(aVar);
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

Int_t  HPidSingleLeptonEff::checkTrackId(HPairSim *p,Int_t lepTrackNb){  
  
     HPairGeantData pg(p);
       
   //    cout<<" pg.nGParentTrackNumber1  "<< pg.getParentTrackNumber1()<<
//     	" pg.sGCommonDetectors1  "<<pg.getCommonDetectors1()<<
//         "  pg.nGTrackNumber1  "<<pg.getTrackNumber1()<<"  lepTrackNb  "<<lepTrackNb<<"   pg.getParentTrackNumber2()  "<< pg.getParentTrackNumber2()<<endl;



//         cout<<" pg.nGParentTrackNumber2  "<< pg.getParentTrackNumber2()<<
//     	"  pg.sGCommonDetectors2  "<<pg.getCommonDetectors2()<<
//    	"  pg.nGTrackNumber2  "<<pg.getTrackNumber2()<<"  lepTrackNb  "<<lepTrackNb<<"   pg.getParentTrackNumber1()  "<< pg.getParentTrackNumber1()<<endl;

     if(pg.getParentTrackNumber1()==0 && 
	pg.getTrackNumber1()==lepTrackNb)
     {
	 if(p->getPid1()==2 || p->getPid1()==3) // first particle is lw
	 {
	     //printf("First case %d %d %d %d\n",pg.getParentTrackNumber1(),pg.getParentTrackNumber2(),p->getPid1(),p->getPid2());
	     if((pg.getParentTrackNumber2()==0 &&(p->getPid2()!=2 && p->getPid2()!=3)) || //generator hadron 
		(pg.getParentTrackNumber2()!=0 )) // secondary lepton (decay in GEANT)
	     {
		 return pg.getCommonDetectors1();
	     }
	     else return 0;
        }
       
     }
     else if(pg.getParentTrackNumber2()==0 && 
	     pg.getTrackNumber2()==lepTrackNb){ 
       if (p->getPid2()==2 || p->getPid2()==3){
	 //printf("Second case %d %d %d %d\n",pg.getParentTrackNumber1(),pg.getParentTrackNumber2(),p->getPid1(),p->getPid2());
       if((pg.getParentTrackNumber1()==0 &&(p->getPid1()!=2 && p->getPid1()!=3))||(pg.getParentTrackNumber1()!=0))         {
	    return pg.getCommonDetectors2();
       }
       else return 0;
        }
     }
     return 0;
}
Bool_t HPidSingleLeptonEff::checkLeptonId(HPairSim *pair){
  
  if((pair->getPid1()==2 || pair->getPid1()==3) &&
     (pair->getPid2()==2 || pair->getPid2()==3))
    return kTRUE;
  else return kFALSE;
  
}
void HPidSingleLeptonEff::calculateTrackProperties(HPairSim *pair,Float_t& th,Float_t& ph,Float_t& p,Int_t& cpF){

  cpF = 0;
  HPairGeantData pg(pair);
  HPairData pd(pair);

  if(pg.getParentTrackNumber1()==0){
    th = pd.getThetaDeg1();
    ph= pd.getPhiDeg1();
    p= pd.getMom1();

	if (pd.getAngleToClosestFittedLepton1() < 9 ||
		pd.getAngleToClosestNonFittedLepton1() < 9)
	{
	   cpF = 1;
	}


  }
  else if(pg.getParentTrackNumber2()==0){
    th = pd.getThetaDeg2();
    ph= pd.getPhiDeg2();
    p= pd.getMom2();

	if (pd.getAngleToClosestFittedLepton2() < 9 ||
		pd.getAngleToClosestNonFittedLepton2() < 9)
	{
	   cpF = 1;
	}

  }
  
}
Bool_t HPidSingleLeptonEff::isGeantTrackInAcceptance(HGeantKine *pG){
     pG->getParticle(lTrack,lId);

     nStatMDC1 = nStatMDC2 = nStatMDC3 = nStatMDC4 = 0;
     nStatTof = nStatShower = 0;
     HGeantMdc *pMdc;
     pitMdcGeant->Reset();
     while((pMdc = (HGeantMdc*) pitMdcGeant->Next()) != NULL){

             if (pMdc->getTrack() == lTrack)
             {
                switch (((Int_t)pMdc->getModule()))
                {
                    case 0: nStatMDC1 = 1;
                            break;
                    case 1: nStatMDC2 = 1;
                            break;
                    case 2: nStatMDC3 = 1;
                            break;
                    case 3: nStatMDC4 = 1;
                            break;
                    default: cerr << "WRONG MDC module number!" << endl;
                }
             }
     }

     HGeantTof *pTof;
     pitTofGeant->Reset();
     while((pTof = (HGeantTof*) pitTofGeant->Next()) != NULL){

            if (pTof->getTrack() == lTrack)
            {
               nStatTof = 1;
            }
     }

     HGeantShower *pShower;
     pitShowerGeant->Reset();
     while((pShower = (HGeantShower*) pitShowerGeant->Next()) != NULL){

            if (pShower->getTrack() == lTrack)
            {
               nStatShower = 1;
            }

     }

     if (nStatMDC1 && nStatMDC2 && nStatMDC3 && /* nStatMDC4 && */ (nStatTof || nStatShower))
       return kTRUE;

 return kFALSE;
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
    fakeEle->Write();
    fakePosi->Write();
    out->Close();
    
    return kTRUE;   
}
