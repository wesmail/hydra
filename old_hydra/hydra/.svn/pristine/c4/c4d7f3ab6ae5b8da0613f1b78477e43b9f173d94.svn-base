#include "hpidlepfilter.h"
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
#include "richdef.h"
#include "hrichhit.h"
#include "htofhit.h"
#include "kickdef.h"
#include "hkicktrack.h"
#include "TH1F.h"
#include "TFile.h"
#include "TClonesArray.h"
#include "hlinearcategory.h"
#include <iostream.h>
#include "hpidmdcfunc.h"
#include "hpidparticlesim.h"
#include "hrichhitsim.h"
#include "hkicktracksim.h"
#include "hshowerhitfpar.h"
#include "hparset.h"

#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hshowerdetector.h"
#include "hshowerdetector.h"


//*-- Author : J.Bielcik
//*-- Modified :29.11.2002
////////////////////////////////////////////////////////////////////////////////////////////////
// HPidLeptonFilter                                                                           //  
// Should filter HPidParticle puting lepton cuts defined                                      //
// by set functions:                                                                          // 
// setRichQRingCuts(4,5,200,2.8);                                                             //
// setShowerLepCond(kTRUE);                                                                   //
// setBetaCutsMomDep(arr_mean_beta_s0,arr_sigma_beta_s0,arr_mean_beta_s1,arr_sigma_beta_s1,3) // 
////////////////////////////////////////////////////////////////////////////////////////////////



HPidLepFilter::HPidLepFilter(void):HReconstructor (){
  setDefaults();
 
}

HPidLepFilter::HPidLepFilter(Text_t *name,Text_t *title,Int_t run_mod) :HReconstructor (name,title) {
  /////////////////////////////////////////////////////////////////////////////////////////////////
  //run_mod=0 means that no Lepton cuts defined in set functions is applied                      //        
  //run_mod=1 means that all Leptons cuts defined in set functions are applied                   //     
  //run_mod=2 means that run_mod=1+ few histograms are filled to tof_histo.root                  //
  /////////////////////////////////////////////////////////////////////////////////////////////////
  mode=run_mod;
  setDefaults();
  
}

HPidLepFilter::~HPidLepFilter(void){;}

Bool_t HPidLepFilter::init(void) {
  
  fPidPart=gHades->getCurrentEvent()->getCategory(catPidPart);
  if (!fPidPart) {
    Error("init()","no HPidParticle hit category in input");
  }
  
  fPidPartIter =(HIterator*) fPidPart->MakeIterator();


  fShowerTofPID=gHades->getCurrentEvent()->getCategory(catShowerTofPID);
  if (!fShowerTofPID) {
    Error("init()","no HShowerTofPID hit category in input");
  }
  
  fShowerTofPIDIter =(HIterator*) fShowerTofPID->MakeIterator();

  
  fRichHitCat=gHades->getCurrentEvent()->getCategory(catRichHit);
  if (!fRichHitCat) {
   Error("init", "No catRichHit");
        return kFALSE; 
  }
  
  //iterator_richhit = (HIterator*)fRichHitCat->MakeIterator("native");

  if((fKickTrackCat = gHades->getCurrentEvent()->getCategory(catKickTrack)) == NULL)
    {
      Error("init", "No catKickTrack");
      return kFALSE;
    }


  //=======



HShowerDetector *pShowerDet = (HShowerDetector*)gHades->getSetup()->getDetector("Shower");




  pHitFPar = (HShowerHitFPar*)gHades->getRuntimeDb()->getContainer("ShowerHitFinderParams");
  if(!pHitFPar){
      Error("HPidLepFilter::init","Cannot get ShowerHitFinderParams container");
      return kFALSE;
    }

  // HShowerDetector *pShowerDet = (HShowerDetector*)gHades->getSetup()->getDetector("Shower");
//   if(pShowerDet == NULL) 
//     {
//       Error("HPidLepFilter::init","cannot get Shower detector");
//       return kFALSE;
//     }
  
//   HRuntimeDb* rtdb=gHades->getRuntimeDb();
  
//   if(	(pHitFPar = (HShowerHitFPar*)rtdb->
// 	 getContainer("ShowerHitFinderParams")) == NULL)
//     {
//       Error("HPidLepFilter::init","Cannot get ShowerHitFinderParams container");
//       return kFALSE;
//     }
  
  //================




  // lepFilter=new HMdcPidLepFilter();
 if(mode==2) initHisto();
  
 // lepFilter.setTofCuts(6.46,8.66,6.78,9.1); //conditions for REAL NOV01 data
 // lepFilter.setTofCuts(tof_tof_mn,tof_tof_mx,tofino_tof_mn,tofino_tof_mx);
 if (check_beta) lepFilter.setBetaCuts(tof_beta_mn,tof_beta_mx,tofino_beta_mn,tofino_beta_mx);
 if (check_rings) lepFilter.setRichQRingCuts(avcharge,npads,pattenqual,centroid);
 if (check_showlep_cond) lepFilter.setShowParamCont(pHitFPar);
 if (check_pull) lepFilter.setCutOnPull(cut_on_pull);
 
 if (check_mom_beta) lepFilter.setBetaCutsMomDep(array_beta_mean_s0,array_beta_sigma_s0,array_beta_mean_s1,array_beta_sigma_s1,beta_sig_cut);
 if (check_mom_pull) lepFilter.setPullCutsMomDep(array_pull_mean_s0,array_pull_sigma_s0,array_pull_mean_s1,array_pull_sigma_s1,pull_sig_cut);

 if (check_mom) lepFilter.setMinMomCut(mom_cut);

 //lepFilter.setShowerLepCond(check_showlep_cond);
 
 cout<<"#############LEPTON FILTER SETTINGS################"<<endl;
 //cout<<"tof time cuts="<<tof_tof_mn<<" "<<tof_tof_mx<<endl;
 //cout<<"tofino time cuts="<<tofino_tof_mn<<" "<<tofino_tof_mx<<endl;
 if (check_beta) printf("tof beta cut: betamin=%f betamax=%f\n",tof_beta_mn,tof_beta_mx);
 if (check_beta) printf("tofino beta cut: betamin=%f betamax=%f\n",tofino_beta_mn,tofino_beta_mx);
 if (check_rings)printf("RICH rings cut: Avcharge=%f Npads=%i PatternM=%i Centroid=%f\n",avcharge,npads,pattenqual,centroid); 
 if (check_showlep_cond)  printf("SHOW PARAM conected\n");
 if (check_pull)  printf("PULL cut:%f\n", cut_on_pull);

 if (check_mom_beta) {
   printf("beta cut is momentum dependent within %f sigma\n",beta_sig_cut);
 }
 if (check_mom_pull) {
   printf("pull cut is momentum dependent within %f sigma\n",pull_sig_cut);
 }

 if (check_mom) {
   printf("mom cut ON mom_min=%f\n",mom_cut);
 }




 //lepFilter.setMultTofinoCut(mult_tofino_cut); //select only tracks in shower part with mult in tofino=1
 //lepFilter.setMatchingCuts(thetadiff,phidiff); // do more precize matching
 //printf("more precize matching cuts %f %f \n",thetadiff,phidiff); 
 //printf("checking mult=1 in tofino in show tracks=%i\n",mult_tofino_cut); 
 //printf("applying shower lep cond=%i\n",check_showlep_cond);
 cout<<"###################################################"<<endl;
 //lepFilter.setTofCuts(6.77,7.89,6.24,8.48); //conditions for SIM NOV01 data
  return kTRUE;
}

Bool_t HPidLepFilter::reinit(void)
{
// Dummy method
    return kTRUE;
}

void HPidLepFilter::setDefaults(void){
  fPidPart=NULL;
  fPidPartIter=NULL;
  fShowerTofPID=NULL;
  fShowerTofPIDIter=NULL;
  htof_sh=NULL;
  htof_sh_lep=NULL;
  htof_tof=NULL;

  tof_tof_mn=0.0;
  tof_tof_mx=10000.0;
  tofino_tof_mn=0.0;
  tofino_tof_mx=10000.0;
  tof_beta_mn=0.0;
  tof_beta_mx=5000.0;
  tofino_beta_mn=0.0;
  tofino_beta_mx=5000.0;

  mom_cut=0.0;

  thetadiff=10.0;
  phidiff=10.0;
  mult_tofino_cut=kFALSE;
  check_showlep_cond=kFALSE;
  select_best_match_part=kFALSE;
  avcharge=0.0;
  npads=0;
  pattenqual=0;
  centroid=100;
 
  check_mom=kFALSE;
  check_beta=kFALSE;
  check_mom_beta=kFALSE;
  

  check_rings=kFALSE;
  check_pull=kFALSE;
  check_mom_pull=kFALSE;
}
void HPidLepFilter::initHisto(void){
  htof_sh=new TH1F("tof_sh","time of flight from tofino",500,0,50);
  htof_sh_lep=new TH1F("tof_sh_lep","time of flight from tofino for show lep",500,0,50);
  htof_tof=new TH1F("tof_tof","time of flight from tof",500,0,50);

}

Bool_t HPidLepFilter::finalize(void){
 if(mode==2){ 
   TFile *file=new TFile("tof_histo.root","RECREATE");
   file->cd();
   htof_sh->Write();
   htof_sh_lep->Write();
   htof_tof->Write();
   file->Close();
 }
  return kTRUE;
}




Int_t HPidLepFilter::execute(void) { 
  
  if(fPidPart->getEntries()<1) return kSkipEvent;
  
 
  if(mode==1||mode==2) {
    if(mode==2) fillHisto(); 
    fPidPart->filter(lepFilter);
  } 
  
  return 0;
}


void HPidLepFilter::setBetaCutsMomDep(Float_t * array_mean_s0,Float_t * array_sigma_s0,Float_t * array_mean_s1,Float_t * array_sigma_s1,Float_t how_many_sigma){
  //////////////////////////////////////////////////////////////////////////////////////////////////////////
  //Momentum dependent cut in 100 MeV bin of momentum of beta in sys=0 and sys=1 separately               //
  //array_mean_s0 is array of 10 with means of beta in mombin for sys=0                                   //
  //array_sigma_s0 is array of 10 with sigma of beta in mombin for sys=0                                  //
  //array_mean_s1 is array of 10 with means of beta in mombin for sys=1                                   //
  //array_sigma_s1 is array of 10 with sigma of beta in mombin for sys=1                                  //
  //how_many_sigma determines how many sigma cut from array should be applied                             //
  //for sys=0 only one side cut is done ... because of multiple hits in TOFINO                            //
  //for each mom bin i=0-100 ... only PidParticles with beta:                                             //
  // (array_mean_s0[i]-how_many_sigma*array_sigma_s0[i],array_mean_s1[i]+how_many_sigma*array_sigma_s1[i])//
  //////////////////////////////////////////////////////////////////////////////////////////////////////////
  for(Int_t i=0;i<10;i++){
    array_beta_mean_s0[i]=array_mean_s0[i];
    array_beta_sigma_s0[i]=array_sigma_s0[i];
    array_beta_mean_s1[i]=array_mean_s1[i];
    array_beta_sigma_s1[i]=array_sigma_s1[i];
  }
  beta_sig_cut=how_many_sigma;
  check_mom_beta=kTRUE;
}


void HPidLepFilter::setPullCutsMomDep(Float_t * array_mean_s0,Float_t * array_sigma_s0,Float_t * array_mean_s1,Float_t * array_sigma_s1,Float_t how_many_sigma){
  ///////////////////////////////////////////////////////////////////////////////////////////////////////
  // Similar like beta cut only for Pull                                                               //
  ///////////////////////////////////////////////////////////////////////////////////////////////////////
  for(Int_t i=0;i<10;i++){
    array_pull_mean_s0[i]=array_mean_s0[i];
    array_pull_sigma_s0[i]=array_sigma_s0[i];
    array_pull_mean_s1[i]=array_mean_s1[i];
    array_pull_sigma_s1[i]=array_sigma_s1[i];
  }
  pull_sig_cut=how_many_sigma;
  check_mom_pull=kTRUE;
}



void HPidLepFilter:: fillHisto(void){
  HPidParticle *pPidPart =NULL;
  fPidPartIter->Reset(); 
  while ( (pPidPart=(HPidParticle *)fPidPartIter->Next())!=NULL) {
    
    // printf("PID=%i sys=%i  tof=%f\n",pPidPart->getPid(),pPidPart->getSystem(),getTof(pPidPart));
    
    if(pPidPart->getSystem()==0) {
      htof_sh->Fill(getTof(pPidPart));
      // printf("PID=%i sys=%i  tof=%f waslepton=%i  \n",pPidPart->getPid(),pPidPart->getSystem(),getTof(pPidPart),isLeptonInShower(pPidPart));
      if(isLeptonInShower(pPidPart)==kTRUE) htof_sh_lep->Fill(getTof(pPidPart));
      
    }       
    if(pPidPart->getSystem()==1) htof_tof->Fill(getTof(pPidPart)); 
    
    
  }
}


Bool_t HPidLepFilter::checkConditions(HPidParticle *part){
 
  if(part->getSystem()==0) {
    if((getTof(part)>=tofino_tof_mn&&getTof(part)<tofino_tof_mx)&&(isLeptonInShower(part)||(check_showlep_cond==kFALSE))) return kTRUE;
  }       
  if(part->getSystem()==1) {
    if(getTof(part)>=tof_tof_mn&&getTof(part)<tof_tof_mx) return kTRUE;
  }
  
  return kFALSE;
}


HPidTrackCand* HPidLepFilter::getPidTrackCand(HPidParticle *part){
  HPidCandidate* pidcand=NULL;
  HPidTrackCand* pidtrackcand=NULL;
  
  pidcand=part->getPidCandidate();
  pidtrackcand=pidcand->getCandidate();
  
  return pidtrackcand;
}

Float_t HPidLepFilter::getTof(HPidParticle *part){
  //tof is taking from corresponding kick track it is more easy
  HPidTrackCand* pPidTrackCand=NULL;
  HKickTrack *pKickTrack=NULL;
  Float_t tof=-1.0;
  
  if (part!=NULL) pPidTrackCand=getPidTrackCand(part);
  if (pPidTrackCand!=NULL) pKickTrack=pPidTrackCand->getKickTrack();
  if (pKickTrack!=NULL) tof=pKickTrack->getTof();
  
  return tof;
  }


Bool_t HPidLepFilter::isLeptonInShower(HPidParticle *part){
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

Bool_t HPidLepFilter::isInShowerTofPID(Int_t ad_SHT){
  HShowerTofPID *pShowerTofPID=NULL;
  fShowerTofPIDIter->Reset(); 
 
  while ( (pShowerTofPID=(HShowerTofPID *)fShowerTofPIDIter->Next())!=NULL) {
    if(pShowerTofPID->getAddress()==ad_SHT) return kTRUE;
  }
  return kFALSE;
} 

Int_t HPidLepFilter::findIndexOfBestMatching(){
  // in a case on ring is correlated with several kicktracks, index of best is returned
  //idea is to pass to LepTon filter list of best correlated tracks to filter the rest
  // it will be implemented later 
  /*  

  HKickTrackSim *pKickTrack=NULL;
  HRichHitSim *pRichHit=NULL;
  HPidTrackCand *pPidTrackCand=NULL;
  HPidMdcFunc fun;
  Float_t the_diff=0.0,phi_diff=0.0;
  Float_t ktrack_phi=0.0,ktrack_the=0.0;
  Int_t ind_r=-1,ind_k=-1;
  HPidParticleSim *pPidPart =NULL;
  fPidPartIter->Reset(); 
  
  if(fPidPart->getEntries()>1){
    printf("=================\n");
    while ( (pPidPart=(HPidParticleSim *)fPidPartIter->Next())!=NULL) {
      pPidTrackCand=fun.getPidTrackCand(pPidPart);
      if(pPidTrackCand){
	
	
	pRichHit=(HRichHitSim *) pPidTrackCand->getRichHit();
	pKickTrack=(HKickTrackSim *) pPidTrackCand->getKickTrack();
	ind_r=fRichHitCat->getIndex(pRichHit);
	ind_k=fKickTrackCat->getIndex(pKickTrack);
	
	printf("index Ring=%i Kick=%i \n",ind_r,ind_k);
	
	ktrack_phi=fun.getNormalMdcPhi(pKickTrack->getSector(),pKickTrack->getPhi());   
	ktrack_the=fun.getNormalMdcTheta(pKickTrack->getTheta());
	
	
	the_diff=TMath::Abs(pRichHit->getTheta()-ktrack_the);
	phi_diff=TMath::Abs(pRichHit->getPhi()-ktrack_phi);
	//printf("the_dif=%f phi_dif=%f mdcindex=%i  metaindx=%i pool=%f quality=%i geantID=%i \n",the_diff,phi_diff,pKickTrack->getSegment(),pKickTrack->getOuterHitId(),pKickTrack->getPull(),pKickTrack->getQuality(),fun.getGeantId(pRichHit,pKickTrack));
	
	
      } else {
	printf("HPidFilter::findIndexOfBestMatching() I cannt find pPidTrackCand for pPidPart\n");
      }
    }
  }  
  

 */

  return -1;
}



ClassImp(HPidLepFilter)
