using namespace std;
#include "hmdcsegsim.h"
#include "hmdchitsim.h"
#include "hmdcdef.h"
#include "hgeantkine.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hcategory.h"
#include "hiterator.h"
#include "hspectrometer.h"
#include "hdebug.h"

#include "hevent.h"
#include "heventheader.h"
#include "hdetector.h"
#include <iostream>
#include <iomanip>
#include <TClonesArray.h>
#include <TArrayF.h>
#include "hgeantmdc.h"
#include "hmdctrackddef.h"
#include "hmdcclusinf.h"
#include "hmdctrackddef.h"
#include "hmdcclustertohit.h"
#include "hmdcclus.h"
#include "hmdccpselector.h"
#include "piddef.h"
#include "hpidparticle.h"
#include "hpidparticlesim.h"
#include "hpidcandidate.h"
#include "hpidtrackcand.h"

#include "hmdcfunc.h"
#include "TNtuple.h"
#include "TH1F.h"
//*-- Author : J.Bielcik
//*-- Modified :04.04.2003
/////////////////////////////////////////////////////////////
// HMdcCPSelector                                          //  
// This class analyses MDC clusters in HPidParticles or    // 
// MdcSeg  and fill the  file with TNtuples of singles and //
// doubles                                                 //
/////////////////////////////////////////////////////////////

HMdcCPSelector::HMdcCPSelector(void) { 
  nameHistoFile="histograms_cpselection.root";
  
  mdc_singles=NULL;
  mdc_doubles=NULL;
  
  fGeantCat= NULL;
  fMdcSegCat= NULL;
  fClusInf=NULL;
  
  iterator_mdcseg = NULL;
  iterator_kine = NULL;
  iterator_mdchit= NULL;
  iterator_clusinf= NULL;
  
  nThreshold = 10000;
  nEvCounter=0;
  special_mode=0;
  mod_mdcseg=0;
  arr_doub_mdcseg=NULL;
  arr_sing_mdcseg=NULL;
}

HMdcCPSelector::HMdcCPSelector(Text_t *name,Text_t *title,char *histoname,Int_t mode) :HReconstructor (name,title) {
  //histoname name of the output file where singles and doubles will be written
  //mode ..0 select from HMdcSegSim
  //mode ..1 select from HPidParticleSim
  // notation of info counting  histogram:
  // 0 test the segment
  // 1 it is a single
  // 2 it is a double
  // 3 it is not a double not a single
  
  special_mode=0;
  mod_mdcseg=mode;
  nameHistoFile=histoname;
  
  fGeantCat= NULL;
  fMdcSegCat= NULL;
  
  iterator_kine = 0;
  iterator_mdcseg = 0;
  iterator_clusinf =0;
  
  nThreshold = 100000;
  index = 0;
  nEvCounter=0;
  arr_doub_mdcseg=NULL;
  arr_sing_mdcseg=NULL;
}

HMdcCPSelector::~HMdcCPSelector(void){
}

void HMdcCPSelector::resetArray(TArrayI *ar){
  //setting the content of the array to -1
  for(Int_t i=0;i<ar->GetSize();i++){
    ar->AddAt(-1,i); 
  }
}

Bool_t  HMdcCPSelector::checkArray(Int_t member,TArrayI *ar){
  //check wheather member is already inside of array
  for(Int_t i=0;i<ar->GetSize();i++){
    if(ar->At(i)==member) return kTRUE; 
  }
  return kFALSE;
}


Bool_t HMdcCPSelector::init(void) {
  // cout<<"in HMdcCPSelector::init"<<endl;
  fGeantCat=gHades->getCurrentEvent()->getCategory(catGeantKine);
  if (!fGeantCat) cout<<"there is no HGeantKine !"<<endl;
  iterator_kine= (HIterator*)fGeantCat->MakeIterator();
  
 
  fMdcSegCat=gHades->getCurrentEvent()->getCategory(catMdcSeg);
  if (!fMdcSegCat) {
    fMdcSegCat=gHades->getSetup()->getDetector("Mdc")->buildCategory(catMdcSeg);
    if (!fMdcSegCat) return kFALSE;
    else gHades->getCurrentEvent()->addCategory(catMdcSeg,fMdcSegCat,"Mdc");
  }
  
  iterator_mdcseg = (HIterator*)fMdcSegCat->MakeIterator("native");
  
  fMdcHitCat=gHades->getCurrentEvent()->getCategory(catMdcHit);
  if (!fMdcHitCat) {
    fMdcHitCat=gHades->getSetup()->getDetector("Mdc")->buildCategory(catMdcHit);
    if (!fMdcHitCat) return kFALSE;
    else gHades->getCurrentEvent()->addCategory(catMdcHit,fMdcHitCat,"Mdc");
  }
  iterator_mdchit = (HIterator*)fMdcHitCat->MakeIterator();
  
  fClusInf=gHades->getCurrentEvent()->getCategory(catMdcClusInf);
  if(!fClusInf) {
    cout<<"there is no catMdcClusInf !"<<endl;
    return kFALSE;
  }
  iterator_clusinf = (HIterator*)fClusInf->MakeIterator();
  
  
  fPidPart=gHades->getCurrentEvent()->getCategory(catPidPart);
  if (!fPidPart) {
    Error("init()","no HPidParticleSim hit category in input");
  }
  
  fPidPartIter =(HIterator*) fPidPart->MakeIterator();

  fGeantMdcCat=gHades->getCurrentEvent()->getCategory(catMdcGeantRaw);
  if (!fGeantMdcCat) cout<<"there is no HGeantMdc !"<<endl;


  
  
  mdc_singles = new TNtuple("mdc_singles","mdc_singles","theta:phi:cls0:nwires0:level0:cls1:nwires1:level1");
  mdc_doubles = new TNtuple("mdc_doubles","mdc_doubles","theta:phi:cls0:nwires0:level0:cls1:nwires1:level1");
  
 
  hsystem=new TH1F("info","various informations",100,0,10);
  // notation
  // 0 test the segment
  // 1 it is a single
  // 2 it is a double
  // 3 it is not a  double not a single
  
  theta_bin_size=10;
  phi_bin_size=10;

  if(mod_mdcseg==0) {
    printf("HMdcCPSelector choose close pairs from MdcSegSim cont  \n");
  }

  if(mod_mdcseg==1) {
    printf("HMdcCPSelector choose close pairs from HPidParticleSim cont \n");
  }

  arr_doub_mdcseg=new TArrayI(3);
  arr_sing_mdcseg=new TArrayI(3);
  
  return kTRUE;
}

Bool_t HMdcCPSelector::finalize(void) {
  writeHisto2File();
  return kTRUE;
}

Int_t HMdcCPSelector::execute(void) {
  //cout<<"============================"<<endl;
  // cout<<"in HMdcCPSelector::execute"<<endl;
  nEvCounter++;
  // printf("EVENTTN=%i\n",nEvCounter);
#if DEBUG_LEVEL>2
  gDebuger->enterFunc("HMdcCPSelector::execute");
#endif
  
  if(mod_mdcseg==0) {
    findConvInMdcSeg();
  }
  if(mod_mdcseg==1) {
    findConvPairsInPidParticle();
  }
  
  return 0;
}

void HMdcCPSelector::findConvInMdcSeg() {
  //this function finds singles and doubles in all MdcSeg 
  
  HMdcSegSim * pObj_mdc_seg;
  iterator_mdcseg->Reset(); 
  while ( (pObj_mdc_seg=(HMdcSegSim *)iterator_mdcseg->Next())!=NULL) {
    hsystem->Fill(0);
    findConvInMdcSeg(pObj_mdc_seg); 
  }
} 

void HMdcCPSelector::findConvInMdcSeg(HMdcSegSim * mdcseg) {
  //looking for singles and doubles only in mdcsegsim
  //using help funcions in HMdcFunc
  HMdcFunc fun;
  fun.setGeantKineCat(fGeantCat);
  fun.setMdcSegCat(fMdcSegCat);
  fun.setMdcHitCat(fMdcHitCat);
  fun.setClusInfCat(fClusInf);
  fun.setGeantMdcCat(fGeantMdcCat);

  hsystem->Fill(0);
  
  Int_t type_of_cluster=-1;
    
  if(special_mode==1){

   type_of_cluster=fun.isSingleDouble(mdcseg,1);
  //if  type_of_cluster=0 single  if type_of_cluster=1 or 2 that it is double

  if (type_of_cluster==1||type_of_cluster==2) {
    //count singles
    hsystem->Fill(2);
    fillHistoDouble(mdcseg);
  } else if (type_of_cluster==0) {
    //count doubles
    hsystem->Fill(1);
    fillHistoSingle(mdcseg);
  } else {
    //count no singles no doubles
    hsystem->Fill(3);
  } 
  } else {
    //this part will be valid if track numbers are propagated without problem
    if (fun.isSingle(mdcseg)) {
      //count singles
      hsystem->Fill(1);
      fillHistoSingle(mdcseg);
    } else if (fun.isDouble(mdcseg,1)) {
      //count doubles
      hsystem->Fill(2);
      fillHistoDouble(mdcseg);
    } else {
      //count no singles no doubles
      hsystem->Fill(3);
    } 
  }
}

void HMdcCPSelector::findConvPairsInPidParticle() {
  //finding singles and doubles from HPidParticles
  //do not repeat the same seg in one event
  HPidParticleSim *pPidPart =NULL;
  HPidTrackCand* pPidTrackCand=NULL;
  
  HMdcSegSim *pMdcSegSim=NULL;
  
  
  HMdcFunc fun;
  fun.setMdcSegCat(fMdcSegCat);
  fun.setMdcHitCat(fMdcHitCat);
  fun.setClusInfCat(fClusInf);
  
  //reset arrays in every event
  resetArray(arr_doub_mdcseg);
  resetArray(arr_sing_mdcseg);
  
  fPidPartIter->Reset(); 
  while ( (pPidPart=(HPidParticleSim *)fPidPartIter->Next())!=NULL) {
    hsystem->Fill(1);
    if (pPidPart!=NULL) {
      pPidTrackCand=fun.getPidTrackCand(pPidPart);
      if (pPidTrackCand!=NULL) {
	pMdcSegSim=(HMdcSegSim*)pPidTrackCand->getMdcSeg();
	findConvInMdcSeg(pMdcSegSim); 
      }
    }
  }
}

Bool_t HMdcCPSelector::fillHistoDouble(HMdcSegSim *mdcseg) {
  // Fill info about this segment to ntuple as double
  
  HMdcFunc fun;
  fun.setMdcSegCat(fMdcSegCat);
  fun.setMdcHitCat(fMdcHitCat);
  fun.setClusInfCat(fClusInf);
  Int_t cls0=fun.getMdcClsSize(mdcseg,0);
  Int_t cls1=fun.getMdcClsSize(mdcseg,1);
  Int_t nwires0=fun.getMdcNWires(mdcseg,0);
  Int_t nwires1=fun.getMdcNWires(mdcseg,1);
  Int_t level0=fun.getMdcLevelCls(mdcseg,0);
  Int_t level1=fun.getMdcLevelCls(mdcseg,1);
  Float_t  mdc_phi=fun.getNormalMdcPhi(mdcseg->getSec(),mdcseg->getPhi());
  Float_t  mdc_theta=fun.getNormalMdcTheta(mdcseg->getTheta());
  
  mdc_doubles->Fill(mdc_theta, mdc_phi,cls0,nwires0,level0,cls1,nwires1,level1);
  return kTRUE;
}

Bool_t HMdcCPSelector::fillHistoSingle(HMdcSegSim *mdcseg) {
  // Fill info about this segment to ntuple as single

  HMdcFunc fun;
  fun.setMdcSegCat(fMdcSegCat);
  fun.setMdcHitCat(fMdcHitCat);
  fun.setClusInfCat(fClusInf);
  Int_t cls0=fun.getMdcClsSize(mdcseg,0);
  Int_t cls1=fun.getMdcClsSize(mdcseg,1);
  Int_t nwires0=fun.getMdcNWires(mdcseg,0);
  Int_t nwires1=fun.getMdcNWires(mdcseg,1);
  Int_t level0=fun.getMdcLevelCls(mdcseg,0);
  Int_t level1=fun.getMdcLevelCls(mdcseg,1);
  Float_t  mdc_phi=fun.getNormalMdcPhi(mdcseg->getSec(),mdcseg->getPhi());
  Float_t  mdc_theta=fun.getNormalMdcTheta(mdcseg->getTheta());

  mdc_singles->Fill(mdc_theta, mdc_phi,cls0,nwires0,level0,cls1,nwires1,level1);
  return kTRUE;
}


void HMdcCPSelector::writeHisto2File(void) {
TFile* myoutfile = new TFile(nameHistoFile,"RECREATE");
  myoutfile->cd();
  mdc_singles->Write();
  mdc_doubles->Write();
  hsystem->Write();
  myoutfile->Close(); 
}


ClassImp(HMdcCPSelector)






