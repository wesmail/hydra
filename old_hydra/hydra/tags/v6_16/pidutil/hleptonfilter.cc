#include "hleptonfilter.h"

#include "hreconstructor.h"
#include "hcategory.h"
#include "hiterator.h"
#include "hspectrometer.h"
#include "hades.h"
#include "hevent.h"
#include "piddef.h"
#include "hpidparticle.h"
#include "hpidcandidate.h"
#include "hpidtrackcand.h"
#include "hshowerhittof.h"
#include "hshowertofpid.h"
#include "showertofinodef.h"
#include "hkicktrack.h"
#include "hrichhit.h"
#include "htofhit.h"
#include "hkicktrack.h"
#include <TH1F.h>
#include <TFile.h>
#include <TClonesArray.h>
#include "hlinearcategory.h"
#include "hrichhit.h"
#include <iostream.h>
#include "hshowerhitfpar.h"
//*-- Author : J.Bielcik
//*-- Modified :29.11.2002
////////////////////////////////////////////////////////////
// HLeptonFilter                                          //  
// Should filter HPidParticle puting some cuts            //
////////////////////////////////////////////////////////////



HLeptonFilter::HLeptonFilter(void){
  setDefaults();
}


HLeptonFilter::~HLeptonFilter(void){;}



void HLeptonFilter::setDefaults(void){
  
  
  tof_tof_mn=0.0;
  tof_tof_mx=10000.0;
  tofino_tof_mn=0.0;
  tofino_tof_mx=10000.0;
  tof_beta_mn=0.0;
  tof_beta_mx=5000.0;
  tofino_beta_mn=0.0;
  tofino_beta_mx=5000.0;
  thetadiff=180.0;
  phidiff=180.0;
  mult_tofino_cut=kFALSE;
  filter_only_singles=kFALSE;
  check_showlep_cond=kFALSE;
  check_mom=kFALSE;
  mom_cut=0.0;
  avcharge=0.0;
  npads=0;
  pattenqual=0;
  centroid=100.0;

  beta_sig_cut=3;
  pull_sig_cut=3;

  check_beta=kFALSE;
  check_rings=kFALSE;
  check_pull=kFALSE;
  check_mom_beta=kFALSE;
  check_mom_pull=kFALSE;
}

void HLeptonFilter::setBetaCutsMomDep(Float_t * array_mean_s0,Float_t * array_sigma_s0,Float_t * array_mean_s1,Float_t * array_sigma_s1,Float_t how_many_sigma){
  
  for(Int_t i=0;i<10;i++){
    array_beta_mean_s0[i]=array_mean_s0[i];
    array_beta_sigma_s0[i]=array_sigma_s0[i];
    array_beta_mean_s1[i]=array_mean_s1[i];
    array_beta_sigma_s1[i]=array_sigma_s1[i];
  }
  beta_sig_cut=how_many_sigma;
  check_mom_beta=kTRUE;
}

void HLeptonFilter::setPullCutsMomDep(Float_t * array_mean_s0,Float_t * array_sigma_s0,Float_t * array_mean_s1,Float_t * array_sigma_s1,Float_t how_many_sigma){
  for(Int_t i=0;i<10;i++){
    array_pull_mean_s0[i]=array_mean_s0[i];
    array_pull_sigma_s0[i]=array_sigma_s0[i];
    array_pull_mean_s1[i]=array_mean_s1[i];
    array_pull_sigma_s1[i]=array_sigma_s1[i];
  }
  pull_sig_cut=how_many_sigma;
  check_mom_pull=kTRUE;
}

Bool_t HLeptonFilter::check(TObject* pidPart){
  // returns kTRUE if the pad has not to be deleted. 
  // otherwise, returns kFALSE

   // printf("inside  HLeptonFilter\n");
  
  if (checkConditions((HPidParticle*)pidPart)) {
    return kTRUE;
  }
  //cout<<"HLeptonFilter revoming HPidParticle"<<endl; 
  return kFALSE;
}


Bool_t HLeptonFilter::checkConditions(HPidParticle *part){
 
 
  if(check_rings&&(checkRingQuality(part)==kFALSE)) return kFALSE; 
     
  if(check_beta&&(isInBetaWindow(part)==kFALSE)) return kFALSE; 
  
  if(check_mom_beta&&(isInMomBetaWindow(part)==kFALSE)) return kFALSE; 

  if(part->getSystem()==0){
    //checking only for sys0
    if(check_showlep_cond&&(isLepInSys0(part)==kFALSE)) return kFALSE;
  }

  if(check_pull&&(checkPull(part)==kFALSE)) return kFALSE;

  if(check_mom_pull&&(checkMomPull(part)==kFALSE)) return kFALSE;

  if(check_mom&&(isInMomWindow(part)==kFALSE)) return kFALSE;


  /*  
    if(fullfillMatching(part)&&checkRingQuality(part)){
      if(part->getSystem()==0) {
	//checking of multiplicity of tofino only for shower tracks
	if((mult_tofino_cut==kTRUE&&fullfillMultTofino(part))||mult_tofino_cut==kFALSE){
	  
	  //if((getTof(part)>=tofino_tof_mn&&getTof(part)<tofino_tof_mx)&&(isLeptonInShower(part)||(check_showlep_cond==kFALSE))) return kTRUE;
	  if(isInTofWindow(part)&&isInBetaWindow(part))return kTRUE; 
	} 
      }
      
      if(part->getSystem()==1) {
	//if(getTof(part)>=tof_tof_mn&&getTof(part)<tof_tof_mx) return kTRUE;
	if(isInTofWindow(part)&&isInBetaWindow(part))return kTRUE; 
      } 
    }
  */
  
  return kTRUE;
}


HPidTrackCand* HLeptonFilter::getPidTrackCand(HPidParticle *part){
  HPidCandidate* pidcand=NULL;
  HPidTrackCand* pidtrackcand=NULL;
  
  pidcand=part->getPidCandidate();
  pidtrackcand=pidcand->getCandidate();
  
  return pidtrackcand;
}

Float_t HLeptonFilter::getTof(HPidParticle *part){
  //tof is taking from corresponding kick track it is more easy
  HPidTrackCand* pPidTrackCand=NULL;
  HKickTrack *pKickTrack=NULL;
  Float_t tof=-1.0;
  
  if (part!=NULL) pPidTrackCand=getPidTrackCand(part);
  if (pPidTrackCand!=NULL) pKickTrack=pPidTrackCand->getKickTrack();
  if (pKickTrack!=NULL) tof=pKickTrack->getTof();
  
  return tof;
  }

Bool_t HLeptonFilter::isInTofWindow(HPidParticle *part){
  Float_t tof=getTof(part);
  if(part->getSystem()==0){
    if(tof>=tofino_tof_mn&&tof<tofino_tof_mx) return kTRUE;
  }
  if(part->getSystem()==1){
    if(tof>=tof_tof_mn&&tof<tof_tof_mx) return kTRUE;
  }
  return kFALSE;
}


Bool_t HLeptonFilter::isInBetaWindow(HPidParticle *part){
  // cout<<"in isInBetaWindow"<<endl;
  Float_t beta=part->getBetaExp();
  if(part->getSystem()==0){
    if(beta>=tofino_beta_mn&&beta<tofino_beta_mx) return kTRUE;
  }
  if(part->getSystem()==1){
    if(beta>=tof_beta_mn&&beta<tof_beta_mx) return kTRUE;
  }
  return kFALSE;
}

Bool_t HLeptonFilter::isInMomBetaWindow(HPidParticle *part){
  Float_t beta=part->getBetaExp();
  
  Int_t i=(Int_t ) (part->P()/100); //gives bin of momentum 
  if(i>9) i=9;
  //printf("in HLeptonFilter::isInMomBetaWindow\n");
  //printf("beta_part=%f mom=%f bin=%i sys=%i\n",beta,part->P(),i,part->getSystem());
  
  if(part->getSystem()==0){
    Float_t bmin=array_beta_mean_s0[i]-beta_sig_cut*array_beta_sigma_s0[i];
    Float_t bmax=array_beta_mean_s0[i]+beta_sig_cut*array_beta_sigma_s0[i];
    //printf("min=%f max=%f \n",bmin,bmax);
    //if(beta>=bmin&&beta<bmax) {printf("return kTRUE\n"); return kTRUE;}
    if(beta>=bmin&&beta<bmax) return kTRUE;
 
  }

  if(part->getSystem()==1){
    Float_t bmin=array_beta_mean_s1[i]-beta_sig_cut*array_beta_sigma_s1[i];
    Float_t bmax=array_beta_mean_s1[i]+beta_sig_cut*array_beta_sigma_s1[i];
    //printf("min=%f max=%f \n",bmin,bmax);
    //if(beta>=bmin&&beta<bmax) {printf("return kTRUE\n"); return kTRUE;} 
    if(beta>=bmin&&beta<bmax) return kTRUE; 
  }
  return kFALSE;
}



Bool_t HLeptonFilter::isLeptonInShower(HPidParticle *part){
  //looking if this HPidParticle come from shower as lepton
  // all leptons are in HShowerTofPID
  HShowerHitTof *pShowerHitTof=NULL;
  
  HPidTrackCand* pPidTrackCand=NULL;
  Int_t ad_SHT= -1;
  Bool_t flag=kFALSE;
  if (part!=NULL) {
    pPidTrackCand=getPidTrackCand(part);
    if (pPidTrackCand!=NULL){
      pShowerHitTof=pPidTrackCand->getShowerHitTof();
      if (pShowerHitTof!=NULL) {
	ad_SHT=pShowerHitTof->getAddress();
        flag=isInShowerTofPID(ad_SHT); 
	if(flag) return kTRUE; 
      } 
    }
  }
  
  
  return kFALSE;
}


Bool_t HLeptonFilter::isLepInSys0(HPidParticle *part)
{
// Jacek method from hit alg
// Check if lepton is in PreShower and Tofino detectors by using tof window and
// F(p)-shower algorithm
  HPidTrackCand* pPidTrackCand=NULL;
  HKickTrack *pKick=NULL;
  HShowerHitTof* showtof=NULL;
  
  if (part!=NULL) pPidTrackCand=getPidTrackCand(part);
  if (pPidTrackCand!=NULL) {
    pKick=pPidTrackCand->getKickTrack();
    showtof=(HShowerHitTof* )pPidTrackCand->getShowerHitTof();
  }
  

Float_t fTof   = pKick->getTof();
Float_t fF10   = pKick->getShowerSum10(1.0);
Float_t fF20   = pKick->getShowerSum20(2.0);
Float_t fMom    = pKick->getP();
//cout<<showtof<<"--"<<pKick<<endl;
// if(showtof->getSum(0)){
//    if(showtof->getSum(1)/showtof->getSum(0)!=fF10) printf("SH fF10=%f fF10=%f\n",showtof->getSum(1)/showtof->getSum(0),fF10);
//    if(showtof->getSum(2)/showtof->getSum(0)!=fF20) printf("SH fF20=%f fF20=%f\n",showtof->getSum(1)/showtof->getSum(0),fF20);
//  } else { 
//    printf("null fF10=%f fF20=%f\n",fF10,fF20);
//  }
//Float_t fMinTof = ((HShowerHitFPar*)pHitFPar)->getMinTof();
//Float_t fMaxTof = ((HShowerHitFPar*)pHitFPar)->getMaxTof();

Float_t fScaleFact = ((HShowerHitFPar*)pShowPar)->getScaleFactor();
Float_t fThr10 = getShowThresh(((HShowerHitFPar*)pShowPar)->getFirstFitParam(),fMom,fScaleFact);
Float_t fThr20 = getShowThresh(((HShowerHitFPar*)pShowPar)->getSecondFitParam(),fMom,fScaleFact);

//if( fTof < fMinTof) return kFALSE;
//      if( fTof > fMaxTof) return kFALSE;
 if( fF10 < fThr10 && fF20 < fThr20 ){
   //printf("NOT lept in shower fF10=%f fThr10=%f fF20=%f fThr20=%f \n",fF10,fThr10,fF20,fThr20); 
   return kFALSE;}
 //printf("IS lept in shower fF10=%f fThr10=%f fF20=%f fThr20=%f \n",fF10,fThr10,fF20,fThr20);
 return kTRUE;
}

// -----------------------------------------------------------------------------

Float_t	HLeptonFilter::getShowThresh(Float_t *pPar, Float_t fMom, Float_t fScale)
{
// Get Pre-Shower charge thresholds which depends on momentum of passing particles

Float_t fFactor = -1;

fFactor = calcFromFit(pPar,fMom);
return fFactor*fScale;
}

// -----------------------------------------------------------------------------

Float_t HLeptonFilter::calcFromFit(Float_t *pPar, Float_t fMom)
{
	 return (pPar[0]+pPar[1]*fMom+pPar[2]*fMom*fMom+pPar[3]*fMom*fMom*fMom);
}	 

// -----------------------------------------------------------------------------




Bool_t HLeptonFilter::isInShowerTofPID(Int_t ad_SHT){
 
  

  HCategory* fShowerTofPID=gHades->getCurrentEvent()->getCategory(catShowerTofPID);
  if (!fShowerTofPID) {
    Error("init()","no HShowerTofPID hit category in input");
  }
  
  HIterator* fShowerTofPIDIter =(HIterator*) fShowerTofPID->MakeIterator();

  HShowerTofPID *pShowerTofPID=NULL;
  fShowerTofPIDIter->Reset(); 
 
  while ( (pShowerTofPID=(HShowerTofPID *)fShowerTofPIDIter->Next())!=NULL) {
    if(pShowerTofPID->getAddress()==ad_SHT) return kTRUE;
  }
  return kFALSE;
} 

Bool_t HLeptonFilter::fullfillMatching(HPidParticle *part){
  HPidTrackCand* pPidTrackCand=NULL;
  HRichHit* pRichHit=NULL;
  HKickTrack* pKickTrack=NULL;
  Float_t ktrack_phi,ktrack_the,the_dif,phi_dif;
  if (part!=NULL) {
    pPidTrackCand=getPidTrackCand(part);
    if (pPidTrackCand!=NULL){
      pRichHit=pPidTrackCand->getRichHit();
      pKickTrack=pPidTrackCand->getKickTrack();
      if(pRichHit!=NULL&&pKickTrack!=NULL){
	
	ktrack_phi=getMdcPhi(pKickTrack->getSector(),pKickTrack->getPhi());   
	ktrack_the=getMdcTheta(pKickTrack->getTheta());
        the_dif=pRichHit->getTheta()-ktrack_the;
	phi_dif=pRichHit->getPhi()-ktrack_phi;
	//printf("thedif=%f phi=%f cutphi=%f\n",the_dif,phi_dif,phidiff/sin(pKickTrack->getTheta()));
	if(TMath::Abs(the_dif)<thetadiff&&TMath::Abs(phi_dif)<(phidiff/sin(pKickTrack->getTheta()))) {
	  //cout<<"match return kTRUE"<<endl;
	  return kTRUE;
	}
      }
    }
  }


  return kFALSE;
}

Bool_t HLeptonFilter::fullfillMultTofino(HPidParticle *part){
  //looking if this HPidParticle come from shower as lepton
  // all leptons are in HShowerTofPID
  HShowerHitTof *pShowerHitTof=NULL;
  
  HPidTrackCand* pPidTrackCand=NULL;
 
  if (part!=NULL) {
    pPidTrackCand=getPidTrackCand(part);
    if (pPidTrackCand!=NULL){
      pShowerHitTof=pPidTrackCand->getShowerHitTof();
      if (pShowerHitTof!=NULL) {
	//	printf("mult in tofino=%i\n",pShowerHitTof->getTofinoMult());
        if(pShowerHitTof->getTofinoMult()==1) {
	  //  cout<<"return kTRUE"<<endl;
	  return kTRUE;
	} 
      } 
    }
  }
  
return kFALSE;  
}

Bool_t HLeptonFilter::checkRingQuality(HPidParticle *part){
  HRichHit *pRichHit=NULL;
  HPidTrackCand* pPidTrackCand=NULL;
  Float_t av=0.0;
  if (part!=NULL) {
    pPidTrackCand=getPidTrackCand(part);
    if (pPidTrackCand!=NULL){
      pRichHit=(HRichHit *) pPidTrackCand->getRichHit();
      if(pRichHit->getRingPadNr()>0) {
	//av=pRichHit->getRingAmplitude()/pRichHit->getRingPadNr();
        av = ((Float_t)pRichHit->getRingAmplitude())/((Float_t)pRichHit->getRingPadNr());
      } else {
	av=0.0;
      }
      //printf("RING: Avcharge=%f Npads=%i PatternM=%i Centroid=%f\n",av,pRichHit->getRingPadNr(),pRichHit->getRingPatMat(),pRichHit->getCentroid()); 
      if(av>avcharge&&pRichHit->getRingPadNr()>npads&&pRichHit->getRingPatMat()>pattenqual&&pRichHit->getCentroid()<centroid){
        //cout<<"checkRingQuality kTRUE"<<endl;  
	return kTRUE;
      }

    }
  }
  //cout<<"checkRingQuality kFALSE"<<endl;
  return kFALSE;

}


Bool_t HLeptonFilter::checkPull(HPidParticle *part){
  HKickTrack *pKickTrack=NULL;
  HPidTrackCand* pPidTrackCand=NULL;
  if (part!=NULL) {
    pPidTrackCand=getPidTrackCand(part);
    if (pPidTrackCand!=NULL){
    pKickTrack=(HKickTrack *) pPidTrackCand->getKickTrack();  
    if(pKickTrack->getPull()<=cut_on_pull&&pKickTrack->getPull()>=-1.0*cut_on_pull) return kTRUE;
    }
  }
  return kFALSE;
}

Bool_t HLeptonFilter::checkMomPull(HPidParticle *part){
  HKickTrack *pKickTrack=NULL;
  HPidTrackCand* pPidTrackCand=NULL;
  
  if (part!=NULL) {

    Int_t i=(Int_t ) (part->P()/100); //gives bin of momentum 
    if(i>9) i=9;
    //printf("in HLeptonFilter::isInMomPullWindow mom=%f %i \n",part->P(),i);
   

    pPidTrackCand=getPidTrackCand(part);
    if (pPidTrackCand!=NULL){
      pKickTrack=(HKickTrack *) pPidTrackCand->getKickTrack();  
      // pKickTrack->getPull() here make momentum cut
      Float_t pull=pKickTrack->getPull();


      if(part->getSystem()==0){
	//in sys0 I use only one side cut because mul>1 hits
	Float_t pumin=array_pull_mean_s0[i]-pull_sig_cut*array_pull_sigma_s0[i];
	//Float_t pumax=array_pull_mean_s0[i]+pull_sig_cut*array_pull_sigma_s0[i];
        //printf("pull min=%f max=%f \n",pumin,pumax);
	//if(pull>=pumin&&pull<pumax) {printf("s0 pull=%f return kTRUE\n",pull); return kTRUE;}
        if(pull>=pumin) return kTRUE;
 
  }
      if(part->getSystem()==1){
	Float_t pumin=array_pull_mean_s1[i]-pull_sig_cut*array_pull_sigma_s1[i];
	Float_t pumax=array_pull_mean_s1[i]+pull_sig_cut*array_pull_sigma_s1[i];
        //printf("min=%f max=%f \n",pumin,pumax);
	//if(pull>=pumin&&pull<pumax) {printf("s1 pull=%f return kTRUE\n",pull); return kTRUE;}
	if(pull>=pumin&&pull<pumax) return kTRUE;
      }
    }
  }
  return kFALSE;
}

Bool_t HLeptonFilter::isInMomWindow(HPidParticle *part){
  if (part!=NULL) {
    
    if(part->P()>mom_cut) return kTRUE;
    
  }
  return kFALSE;
}


ClassImp(HLeptonFilter)
