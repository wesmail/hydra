
#include "hpiddileptonfilter.h"
#include "hpidparticle.h"
#include "hpiddilepton.h"
#include "TArrayI.h"
#include "TString.h"
#include "hcategory.h"
#include "hiterator.h"
#include "hmdcfunc1.h"
#include <iostream.h>
#include "hpidtrackcand.h"
/*
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
*/

//*-- Author : J.Bielcik
//*-- Modified :05.08.2003
////////////////////////////////////////////////////////////
// HLeptonFilter                                          //  
// Should filter HPidDilepton puting some magic cuts      //
////////////////////////////////////////////////////////////


HPidDiLeptonFilter::HPidDiLeptonFilter(void){
  setDefaults();
}


HPidDiLeptonFilter::~HPidDiLeptonFilter(void){;}


void HPidDiLeptonFilter::setDefaults(){
  skip_no_pairs=kTRUE; 
  check_common_hits=kTRUE; 
  check_cpr_rej=kTRUE; 
  check_open_angle=kTRUE;
  mode_data=0; //mean real
  
  singleSelectCond="OR";

  open_angle_cut=0.0;
  prob_single_cut=0.0;
  fCpPidParName="cp_pid_param_histos.root";
} 


void HPidDiLeptonFilter::setOptions(Option_t *pParams){
  //function that allows to set the dilepton cuts
  //cuts are set by default and should be remove by special worlds
  // NOMULTHITSCUT ... there will not be cut checking common hits in lepton tracks in pair
  // NOCPCUT       ... there will not be cut checking that leptons are not doubles
  // NOOPENCUT     ... there will not be cut checking open angle of dilepton  
  
  // SIM ... data are treated to be simulated
  // REAL ...data are treated to be real
  
  TString s = pParams;
  s.ToUpper();
  
  check_common_hits  = (strstr(s.Data(),"NOMULTHITSCUT") == NULL);
  check_cpr_rej = (strstr(s.Data(),"NOCPCUT") == NULL);
  check_open_angle=(strstr(s.Data(),"NOOPENCUT") == NULL);
  skip_no_pairs=(strstr(s.Data(),"NOSKIPNOPAIRS") == NULL);

  if(strstr(s.Data(),"REAL") !=NULL)  mode_data=0;
  if(strstr(s.Data(),"SIM") !=NULL)   mode_data=1;
  cout<<"#############PID DILEP FILTER SETTINGS################"<<endl;

  if(skip_no_pairs){ 
    printf("Skipping events with no pairs ON\n");
  }else {
    printf("Skipping events with no pairs OFF\n");
  }

  if(check_common_hits){ 
    printf("Pairs with MultHits rejection ON\n");
  }else {
    printf("Pairs with MultHits rejection OFF\n");
  }

  if(check_cpr_rej){ 
    printf("CPR on lep ON\n");
    printf("CpPidParamsFile:%s \n",fCpPidParName);
    printf("Rejecting the pairs that contain lepton with prob to single <=%f \n",prob_single_cut);
    printf("SingleProbCondition between mdc modules is %s\n",singleSelectCond);

    
  }else {
    printf("CPR on lep OFF\n");
  }

  if(check_open_angle){ 
    printf("Rejecting of Pairs with small op angle ON\n");
    printf("Rejecting pairs with angle<%f Degree\n", open_angle_cut);
  }else {
    printf("Rejecting of Pairs with small op angle OFF\n");
  }

  if(mode_data==0){ 
    printf("CPR WILL BE DONE FOR REAL DATA\n");
  } else {
    printf("CPR WILL BE DONE FOR SIMULATION\n");
  }
  cout<<"###################################################"<<endl;
  
}


Bool_t HPidDiLeptonFilter::check(TObject* pidDiLep){
  //return kFALSE if this dilepton should be filetered
 if (checkConditions((HPidDilepton*)pidDiLep)) {
   //printf("check=kTRUE\n"); 
   return kTRUE;
  }
 //printf("check=kFALSE\n"); 
 return kFALSE;
}


Bool_t HPidDiLeptonFilter::checkConditions(HPidDilepton *pidDiLep) {
  //this return kFALSE if pidDil should be filtered out
  Bool_t return_value=kTRUE;
  if(check_common_hits)  return_value=checkCommonHits(pidDiLep);
  //if already this tell me that I have to kick it out, I do not check rest
  //printf("common_hits %i\n",return_value);
  if(!return_value) return return_value;


  


  if(check_cpr_rej||check_open_angle) return_value= isParticleIdSingle(pidDiLep,double_ids);
  //printf("return cpr %i\n",return_value);
  if(!return_value) return return_value;

  if(check_open_angle) return_value=checkOpenAngle(pidDiLep);
  
  return return_value;
}



Bool_t HPidDiLeptonFilter::checkCommonHits(HPidDilepton* pidDiLep){
  //return kFALSE if particles in dilepton have common hits
  HPidParticle *pPart1=NULL;
  HPidParticle *pPart2=NULL;
  
  pPart1=(HPidParticle *)pidDiLep->getParticle1();
  pPart2=(HPidParticle *)pidDiLep->getParticle2();
  // pPart1->print();
//   pPart2->print();

  //pidDiLep->print();

  HPidTrackCand* pTrackCand1=pPart1->getTrackCand();
  HPidTrackCand* pTrackCand2=pPart2->getTrackCand();
 
  Bool_t common_RICH=kFALSE;
  Bool_t common_MDC=kFALSE;
  Bool_t common_META=kFALSE;

    //check ring ID
    if(pTrackCand1->getRingId()==pTrackCand2->getRingId()){ 
      common_RICH=kTRUE;
    }


  //check MdcSeg Id
    if((pTrackCand1->getInnerMdcId(0)==pTrackCand2->getInnerMdcId(0))&&(pTrackCand1->getSector()==pTrackCand2->getSector())){ 
      common_MDC=kTRUE;
    }
    //check META Id
    if(pTrackCand1->getMetaId()==pTrackCand2->getMetaId()){
      common_META=kTRUE;
    }
    
    //printf("cR=%i cM=%i cMe=%i\n",common_RICH,common_MDC,common_META);
    if(common_RICH||common_MDC||common_META) return kFALSE;
    
    return kTRUE;
}

Bool_t HPidDiLeptonFilter::isParticleIdSingle(HPidDilepton* pidDiLep,TArrayI *rm_id){
  //return false if dil contain particle with ind from array of Doubles 

  HMdcFunc1 fun;
 
  HPidParticle *pPart1=NULL;
  HPidParticle *pPart2=NULL;
  Int_t nId1=-1;
  Int_t nId2=-1;
  //  Int_t index=0;


  Int_t debug=0;

  pPart1=(HPidParticle *)pidDiLep->getParticle1();
  pPart2=(HPidParticle *)pidDiLep->getParticle2();
  nId1 = ((HPidParticle *)pPart1)->getIndex();
  nId2 = ((HPidParticle *)pPart2)->getIndex();

  if(debug) printf("check pairs %i %i\n",nId1,nId2);

  if (fun.isInArray( nId1,rm_id)||fun.isInArray( nId2,rm_id)) return kFALSE;
 
  if(debug) printf("pair is OK\n"); 

  return kTRUE;
  
}


Bool_t HPidDiLeptonFilter::checkOpenAngle(HPidDilepton* pidDiLep){
  //this is additional checking of the opening angle 
  //because I would like remove also small open angle like sign pairs (but only that pair,not combinations)

  if(pidDiLep->getOpeningAngleDeg()<open_angle_cut&&check_open_angle==kTRUE) return kFALSE;

  return kTRUE;


}
