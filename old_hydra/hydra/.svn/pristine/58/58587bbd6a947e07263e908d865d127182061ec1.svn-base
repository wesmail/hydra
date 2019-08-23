#include "hpiddilepfilter.h"
#include "hreconstructor.h"
#include "hcategory.h"
#include "hiterator.h"
#include "hspectrometer.h"
#include "hades.h"
#include "hevent.h"
#include "piddef.h"
#include "hlinearcategory.h"
#include "hpiddilepton.h"
#include "TArrayI.h"
#include "TString.h"
#include "hmdcdef.h"
#include "hmdctrackddef.h"
#include "hmdcfunc1.h"
#include "hpidparticle.h"
#include <iostream.h>
//*-- Author : J.Bielcik
//*-- Modified :05.08.2003
////////////////////////////////////////////////////////////
// HPidDiLeptonFilter                                     //  
// Should filter HPidDileptons puting some clever cuts    //
////////////////////////////////////////////////////////////



HPidDiLepFilter::HPidDiLepFilter(void):HReconstructor (){
  
  check_common_hits=kTRUE; 
  check_cpr_rej=kTRUE; 
  check_open_angle=kTRUE;
  skip_no_pairs=kTRUE;
  mode_data=0;
  singleSelectCond="OR";
  open_angle_cut=0.0;
  prob_single_cut=0.0;
  fCpPidParName="cp_pid_param_histos.root";

}

HPidDiLepFilter::HPidDiLepFilter(Text_t *name,Text_t *title) :HReconstructor (name,title) {
  
  check_common_hits=kTRUE; 
  check_cpr_rej=kTRUE; 
  check_open_angle=kTRUE;
  skip_no_pairs=kTRUE; 
  mode_data=0;
  singleSelectCond="OR";

  open_angle_cut=0.0;
  prob_single_cut=0.0;
  fCpPidParName="cp_pid_param_histos.root";
  
}

HPidDiLepFilter::~HPidDiLepFilter(void){;}

Bool_t HPidDiLepFilter::init(void) {
  
  fPidDilepton=gHades->getCurrentEvent()->getCategory(catPidDilepton);
  if (!fPidDilepton) {
    Error("init()","no HPidParticle hit category in input");
  }
  
  fPidDileptonIter =(HIterator*) fPidDilepton->MakeIterator();
  

  fMdcHitCat=gHades->getCurrentEvent()->getCategory(catMdcHit);
  if (!fMdcHitCat) {
    cout<<"there is no catMdcHit !"<<endl;
    return kFALSE;
  }
 
  fClusInfCat=gHades->getCurrentEvent()->getCategory(catMdcClusInf);
  if(!fClusInfCat) {
    cout<<"there is no catMdcClusInf !"<<endl;
    return kFALSE;
  }


  double_ids=new TArrayI(3);
  

  diLepFilter.setCpPidParamName(fCpPidParName);
  diLepFilter.setOpeningAngleCut(open_angle_cut);
  diLepFilter.setProbSingleCut(prob_single_cut);
  diLepFilter.setPidDileptonCatIter(fPidDileptonIter);
  diLepFilter.setProbSingleCondition(singleSelectCond);
  diLepFilter.setOptions(cutParams);

  

 
return kTRUE;
}



Bool_t HPidDiLepFilter::finalize(void){

  return kTRUE;
}




Int_t HPidDiLepFilter::execute(void) { 
  Int_t debug=0;
  if(debug) cout<<"in HPidDiLepFilter::execute"<<endl;
  if(fPidDilepton->getEntries()<1&&skip_no_pairs==kTRUE) return kSkipEvent;

  fillRM_array();
  diLepFilter.setRMArray(double_ids);
  
  if(debug) printf("passing following array\n");
  if(debug) for(Int_t i=0;i<double_ids->GetSize();i++) {printf("%i : %i\n",i,double_ids->At(i));}
  fPidDilepton->filter(diLepFilter);
  //cout<<"out HPidDiLepFilter::execute"<<endl; 
  return 0;
}




void HPidDiLepFilter::setOptions(Option_t *pParams){
  //function that allows to set the dilepton cuts
  //cuts are set by default and should be remove by special worlds
  // NOMULTHITSCUT ... there will not be cut checking common hits in lepton tracks in pair
  // NOCPCUT       ... there will not be cut checking that leptons are not doubles
  // NOOPENCUT     ... there will not be cut checking open angle of dilepton  
  
  // SIM ... data are treated to be simulated
  // REAL ...data are treated to be real
  
 

  
  cutParams=pParams;
  TString s = pParams;
  s.ToUpper();
  
  check_common_hits  = (strstr(s.Data(),"NOMULTHITSCUT") == NULL);
  check_cpr_rej = (strstr(s.Data(),"NOCPCUT") == NULL);
  check_open_angle=(strstr(s.Data(),"NOOPENCUT") == NULL);
  skip_no_pairs=(strstr(s.Data(),"NOSKIPNOPAIRS") == NULL);

  if(strstr(s.Data(),"REAL") !=NULL)  mode_data=0;
  if(strstr(s.Data(),"SIM") !=NULL)   mode_data=1;
  


}


void HPidDiLepFilter::fillRM_array(){
  //cout<<"in HPidDiLepFilter::fillRM_array"<<endl;
  HPidDilepton *pDil =NULL; 
  HMdcFunc1 fun;

  fun.setMdcHitCat(fMdcHitCat);
  fun.setClusInfCat(fClusInfCat);

  HPidParticle *pPart1=NULL;
  HPidParticle *pPart2=NULL;
  Int_t nId1=-1;
  Int_t nId2=-1;
  Int_t inx=0;
  
  Int_t debug=0; 

  //cout<<" double_ids"<< double_ids<<endl;
  double_ids->Set(3);
  fun.resetArray(double_ids);
  //first I go over all dileptons that I have and make list of all that have partner closer that opening angle
  fPidDileptonIter->Reset(); 
  while ( (pDil=(HPidDilepton *)fPidDileptonIter->Next())!=NULL) {
    //if(pDil->getCharge()==0){
    pPart1=(HPidParticle *)pDil->getParticle1();
    pPart2=(HPidParticle *)pDil->getParticle2();
   
    if(check_cpr_rej==kTRUE){
      if (fun.passCprPdfCuts(pPart1,prob_single_cut,fCpPidParName,mode_data,singleSelectCond)){
	if (fun.passCprPdfCuts(pPart2,prob_single_cut,fCpPidParName,mode_data,singleSelectCond)){
	  //this means that both are singles and if they are e+e- I remove them if they are close to each other 
	  if(pDil->getCharge()==0&&pDil->getOpeningAngleDeg()<open_angle_cut&&check_open_angle==kTRUE){
	    nId1 = ((HPidParticle *)pPart1)->getIndex();
	    nId2 = ((HPidParticle *)pPart2)->getIndex();
	    if(debug) printf("pairs has small op angle and go to array %i %i\n", nId1, nId2);
	    fun.putInArray(nId1,double_ids,inx);
	    inx++;
	    fun.putInArray(nId2,double_ids,inx);
	    inx++;
	  }
	} else {
	  //this mean that pPart2 id double and I rm it
	  nId2 = ((HPidParticle *)pPart2)->getIndex();
	  if(debug) printf("P2 is double go to array %i\n",nId2);
	  fun.putInArray(nId2,double_ids,inx);
	  inx++;
	}
      } else {
	//this mean that pPart1 id double and I rm it
	nId1 = ((HPidParticle *)pPart1)->getIndex();
	fun.putInArray(nId1,double_ids,inx);
	if(debug) printf("P1 is double go to array %i\n",nId1);
	inx++;
	if(!fun.passCprPdfCuts(pPart2,prob_single_cut,fCpPidParName,mode_data,singleSelectCond)){
	  //this mean that pPart2 id is also double and I rm it
	  nId2 = ((HPidParticle *)pPart2)->getIndex();
	  if(debug) printf("P2 is double go to array %i\n",nId2);
	  fun.putInArray(nId2,double_ids,inx);
	  inx++;
	}
      }
    } else {
      //if there is no CB on one puts to array only those e+e- with small op angle
      if(pDil->getCharge()==0&&pDil->getOpeningAngleDeg()<open_angle_cut&&check_open_angle==kTRUE){
	nId1 = ((HPidParticle *)pPart1)->getIndex();
	nId2 = ((HPidParticle *)pPart2)->getIndex();
	
	fun.putInArray(nId1,double_ids,inx);
	inx++;
	fun.putInArray(nId2,double_ids,inx);
	inx++;
      }
    }
  }
}







ClassImp(HPidDiLepFilter)
