#include "hrichhitsim.h"
#include "hmdcsegsim.h"
#include "hmdchitsim.h"
#include "hmdcdef.h"
#include "richdef.h"
#include "hgeantkine.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hcategory.h"
#include "hiterator.h"
#include "hspectrometer.h"
#include "hdebug.h"
#include "tofdef.h"
#include "hevent.h"
#include "heventheader.h"
#include "hdetector.h"
#include <iostream.h>
#include <TVector3.h>
#include "hrichgeometrypar.h"
#include "hrichraw.h"
#include "hgeantrich.h"
#include <TClonesArray.h>
#include <TArrayF.h>
#include "hgeantmdc.h"
#include "hmdctrackddef.h"
#include "hmdcclusinf.h"
#include "hmdctrackddef.h"
#include "hmdcclustertohit.h"
#include "hmdcclus.h"
#include "hmdccpselector.h"

//*-- Author : J.Bielcik
//*-- Modified :23.1.2002
////////////////////////////////////////////////////////////
// HMdcCPSelector                                         //  
// This class analyses MDC clusters and                   //
// Rich rings properties and fills                        // 
// file with TNtuples of singles and doubles in mdc and   //
// common singles and doubles in mdc-rich from simulatios //  
////////////////////////////////////////////////////////////

HMdcCPSelector::HMdcCPSelector(void) { 
  nameHistoFile="histograms_cpselection.root";
  
  singles=NULL;
  doubles=NULL;
  conv_mdcgeant=NULL;
  conv_richmirrgeant=NULL;
  conv_mdcseg=NULL;
  conv_richhit=NULL;
  mdc_singles=NULL;
  mdc_doubles=NULL;
  
  fGeantCat= NULL;
  fMdcSegCat= NULL;
  fClusInf=NULL;
  iterator = 0;
  index = 0;
  iterator_mdcseg = 0;
  iterator_kine = 0;
  iterator_richmirror =0;
  iterator_mdcgeant =0;
  iterator_mdcgeant_2=0;
  nThreshold = 10000;
  nEvCounter=0;
  version_flag=kFALSE;
  all_flag=kFALSE;
}

HMdcCPSelector::HMdcCPSelector(Text_t *name,Text_t *title,char *histoname) :HReconstructor (name,title) {
  
  nameHistoFile=histoname;
  
  fGeantCat= NULL;
  fMdcSegCat= NULL;
  iterator = 0; 
  iterator_kine = 0;
  iterator_mdcseg = 0;
  iterator_richmirror =0;
  iterator_clusinf =0;
  nThreshold = 100000;
  index = 0;
  nEvCounter=0;
  version_flag=kFALSE;
  all_flag=kFALSE;
}

HMdcCPSelector::~HMdcCPSelector(void){
  
}


Bool_t HMdcCPSelector::init(void) {
  // cout<<"in HMdcCPSelector::init"<<endl;
  fGeantCat=gHades->getCurrentEvent()->getCategory(catGeantKine);
  if (!fGeantCat) cout<<"there is no HGeantKine !"<<endl;
  iterator_kine= (HIterator*)fGeantCat->MakeIterator();
  
  fGeantMdc=gHades->getCurrentEvent()->getCategory(catMdcGeantRaw);
  if (!fGeantMdc) cout<<"there is no HGeantMdc !"<<endl;
  iterator_mdcgeant= (HIterator*)fGeantMdc->MakeIterator();
  iterator_mdcgeant_2= (HIterator*)fGeantMdc->MakeIterator();
  
  
  fMdcSegCat=gHades->getCurrentEvent()->getCategory(catMdcSeg);
  if (!fMdcSegCat) {
    fMdcSegCat=gHades->getSetup()->getDetector("Mdc")->buildCategory(catMdcSeg);
    if (!fMdcSegCat) return kFALSE;
    else gHades->getCurrentEvent()->addCategory(catMdcSeg,fMdcSegCat,"Mdc");
  }
  
  iterator_mdcseg = (HIterator*)fMdcSegCat->MakeIterator();
  
  fMdcHitCat=gHades->getCurrentEvent()->getCategory(catMdcHit);
  if (!fMdcHitCat) {
    fMdcHitCat=gHades->getSetup()->getDetector("Mdc")->buildCategory(catMdcHit);
    if (!fMdcHitCat) return kFALSE;
    else gHades->getCurrentEvent()->addCategory(catMdcHit,fMdcHitCat,"Mdc");
  }
  
  iterator_mdchit = (HIterator*)fMdcHitCat->MakeIterator();
  iterator_mdchit_2 = (HIterator*)fMdcHitCat->MakeIterator();

  fRichHitCat=gHades->getCurrentEvent()->getCategory(catRichHit);
  if (!fRichHitCat) {
    fRichHitCat=gHades->getSetup()->getDetector("Rich")->buildCategory(catRichHitSim);
    if (!fRichHitCat) return kFALSE;
    else gHades->getCurrentEvent()->addCategory(catRichHitSim,fRichHitCat,"Rich");
  }
  
  iterator_richhit = (HIterator*)fRichHitCat->MakeIterator("native");
  
  
  fGeantRichMirrorCat=gHades->getCurrentEvent()->getCategory(catRichGeantRaw+2);
  if (!fGeantRichMirrorCat) {
    fGeantRichMirrorCat=gHades->getSetup()->getDetector("Rich")->buildCategory(catRichGeantRaw+2);
    if (!fGeantRichMirrorCat) return kFALSE;
    else gHades->getCurrentEvent()->addCategory(catRichGeantRaw+2,fGeantRichMirrorCat,"Rich");
  }
  
  
  iterator_richmirror = (HIterator*)fGeantRichMirrorCat->MakeIterator();
  
  fRichGeoPar = (HRichGeometryPar*)gHades->getRuntimeDb()
    ->getContainer("RichGeometryParameters");
  if (!fRichGeoPar) {
    fRichGeoPar=new HRichGeometryPar;
    gHades->getRuntimeDb()->addContainer(fRichGeoPar);
  }
  
 
  
  fClusInf=gHades->getCurrentEvent()->getCategory(catMdcClusInf);
  if(!fClusInf) {
    cout<<"there is no catMdcClusInf !"<<endl;
    return kFALSE;
  }
  iterator_clusinf = (HIterator*)fClusInf->MakeIterator();
  
  
  
  pPadsPar = ((HRichGeometryPar*)fRichGeoPar)->getPadsPar();
  pPad = NULL; 
  
  
  singles=new TNtuple("singles","singles","m_the:m_phi:r_the:r_phi:clussize:nwires:charge:mergecls:sig1:sig2:alpha:level");
  doubles=new TNtuple("doubles","doubles","m_the:m_phi:r_the:r_phi:clussize:nwires:charge:mergecls:sig1:sig2:alpha:level");
  
  conv_mdcgeant=new TNtuple("convinmdcgeant","conv in mdc geant","geant_the:geant_phi:geant_mom:medium:module:inmod01");
  conv_richmirrgeant=new TNtuple("convinrichgeant","conv in rich mirr geant","geant_the:geant_phi:geant_mom:medium:inmdc0:inmdc01");
  conv_mdcseg=new TNtuple("conv_mdcseg","conv in mdc seg","theta:phi:theta_bin:phi_bin:cls:nwires:nmerged:single_double_0_1");
  conv_richhit=new TNtuple("conv_richhit","conv in rich hit ","theta:phi:ncharge:npads:clssizeoverth:single_double_0_1");
  
  mdc_singles = new TNtuple("mdc_singles","mdc_singles","theta:phi:cls:nwires:nmerged:sig1:sig2:alpha:level");
  mdc_doubles = new TNtuple("mdc_doubles","mdc_doubles","theta:phi:cls:nwires:nmerged:sig1:sig2:alpha:level");
 
  hsystem=new TH1F("info","various informations",100,0,10);
  rich_help=new TNtuple("richhelp","richhelp","xdiff:ydiff:weight:match");
  
  theta_bin_size=10;
  phi_bin_size=10;
 
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
  //  printf("EVENTTN=%i\n",nEvCounter);
#if DEBUG_LEVEL>2
  gDebuger->enterFunc("HMdcCPSelector::execute");
#endif
  
  findConvInRichHit(); //find singles and doubles in RichHit
  findConvInMdcSeg();  //find common singles and doubles in MdcSeg&RichHit and in MdcSeg
  findConvPairsInGeantMdc();
  findConvPairsInGeantRichMirr();
  // cout<<"out HMdcCPSelector::execute"<<endl;
  return 0;
}

void HMdcCPSelector::findConvInMdcSeg() {
 //  cout<<"in HMdcCPSelector::findConvInMdcSeg()"<<endl;
  //this function finds singles and doubles in MdcSeg and if there are also in HRichHit that it writes it in ntuples
  Int_t track_mul;
  HMdcSegSim * pObj_mdc_hit;
  iterator_mdcseg->Reset(); 
  while ( (pObj_mdc_hit=(HMdcSegSim *)iterator_mdcseg->Next())!=NULL) {
    // if(pObj_mdc_hit->getModule()!=1) continue; 
    
    //look only at inner segments :very important
    if(pObj_mdc_hit->getIOSeg()!=0) continue; 
    hsystem->Fill(1); //inner segment
    
    //cout<<"module="<<pObj_mdc_hit->getModule()<<endl;
    track_mul=pObj_mdc_hit->getNTracks();
    if(pObj_mdc_hit->getNTimes(0)<4){
      // printf("mdchit multip of 1.=%i is less than 4 \n",pObj_mdc_hit->getNTimes(0));
      // return 0;
      hsystem->Fill(2); //inner segment that has low multiplicity of the most frequent track 
      continue ;
    }else{
      //multiplicity of the first track in track list is 4 or more => (this track is significant)
      if( track_mul>1&& pObj_mdc_hit->getNTimes(1)>3) {
	//CANDIDATE FOR DOUBLE: first two tracks in track list has contribution in cell 4 and more 
	// printf("candidate for double %i %i %i \n",track_mul,pObj_mdc_hit->getNTimes(0),pObj_mdc_hit->getNTimes(1));
	//NOW we check if the seg contain e+e- from conversion 
	analyzeDouble(pObj_mdc_hit);
        hsystem->Fill(3); //candidate for double  
      } else {
	// printf("candidate for single \n");
	//CANDIDATE FOR SINGLE
	//if cluster contain only single track or multiplicity of second is less than 4  
	if((track_mul>1&&pObj_mdc_hit->getNTimes(0)>3&&pObj_mdc_hit->getNTimes(1)<4)||track_mul==1) {
	  analyzeSingle(pObj_mdc_hit);
	  hsystem->Fill(4);
	} else {
	  hsystem->Fill(5);
	}
      }
    }
  }
  // cout<<"out HMdcCPSelector::findConvInMdcSeg()"<<endl;
} 

void HMdcCPSelector::findConvInRichHit() {
  // cout<<"in HMdcCPSelector::findConvInRichHit()"<<endl;
  // This function fills singles and doubles in RichHitSim
  HRichHitSim *pObj_rich;
  
  Int_t pID2=-2,pID1=-1,pID3=-3,pTrack_par1=-1,pTrack_par2=-2,pTrack_par3=-3;
  TVector3 vec1;
  TVector3 vec2;
  TVector3 vec3;
  Int_t firedpad,maxthrclustsize,intcharge;
  Float_t rtheta,rphi;

  iterator_richhit->Reset();
  while ((pObj_rich=(HRichHitSim*)iterator_richhit->Next())!= 0) {
    // printf("RingAlgorithm=%i RingHouTra=%i \n",pObj_rich->iRingAlgorithmIndex,pObj_rich->iRingHouTra);
     if((pObj_rich->iRingAlgorithmIndex%2)==1&&(pObj_rich->iRingHouTra>28)){
      
       if(checkRing(pObj_rich)) {
	
 	hsystem->Fill(6);//this ring is also in Mirror
	//	printf("tracks %i %i %i \n",pObj_rich->track1,pObj_rich->track2,pObj_rich->track3);
        if(pObj_rich->track1>0){
	  if(checkConv(getObj(pObj_rich->track1))) {
	    //vec,pID,pTrack_parent are global and filed by checkConv funcion
	    vec1=vec;
	    pID1=pID;
	    pTrack_par1=pTrack_parent;
	  }
	}  
	if(pObj_rich->track2>0){
	  if(checkConv(getObj(pObj_rich->track2))) {
	    vec2=vec;
	    pID2=pID;
	    pTrack_par2=pTrack_parent;
	  }  
	}
	if(pObj_rich->track3>0){
	  if(checkConv(getObj(pObj_rich->track3))) {
	    vec3=vec;
	    pID3=pID;
	    pTrack_par3=pTrack_parent;
	  }  
	}
	firedpad=pObj_rich->iRingPadNr; 
	maxthrclustsize=pObj_rich->fMaxThrClusterSize;
	intcharge=pObj_rich->iRingAmplitude;
	rtheta=pObj_rich->getTheta();
	rphi=pObj_rich->getPhi();
	
	if(((pID1==2&&pID2==3)||(pID2==2&&pID1==3))&&(pTrack_par2==pTrack_par1)){
	  //cout<<"!!!CONVERSION PAIR!!!!!"<<endl;
	  //first two tracks are e+e- pair from conversion 
	  //tr1 tr2
	  conv_richhit->Fill(rtheta,rphi,intcharge,firedpad,maxthrclustsize,1);
	   
	} else {
	  if(((pID1==2&&pID3==3)||(pID3==2&&pID1==3))&&(pTrack_par3==pTrack_par1)){
	    //cout<<"!!!CONVERSION PAIR!!!!!"<<endl;
	    //tr1 tr3
	    conv_richhit->Fill(rtheta,rphi,intcharge,firedpad,maxthrclustsize,1);
	    
	    
	    
	  } else {
	    
	    if(((pID2==2&&pID3==3)||(pID3==2&&pID2==3))&&(pTrack_par3==pTrack_par2)){
	      //cout<<"!!!CONVERSION PAIR!!!!!"<<endl;
	      //tr2 tr3
	      conv_richhit->Fill(rtheta,rphi,intcharge,firedpad,maxthrclustsize,1);
	      
	      
	      
	    }else {
	      
	      //test single
	      if(pID1==2||pID1==3) {
		//tr1 is single
		conv_richhit->Fill(rtheta,rphi,intcharge,firedpad,maxthrclustsize,0);
		continue ;
	      }
	      
	      if(pID2==2||pID2==3) {
		//tr2 is single
		conv_richhit->Fill(rtheta,rphi,intcharge,firedpad,maxthrclustsize,0);
		continue ;
	      }
	      
	      if(pID3==2||pID3==3) {
		//tr3 is single
		conv_richhit->Fill(rtheta,rphi,intcharge,firedpad,maxthrclustsize,0);
		continue;
	      }
	    }
	  }
	}
       }else{
	 hsystem->Fill(7);
       }
     }
  }

  //cout<<"out HMdcCPSelector::findConvInRichHit()"<<endl;

}

HGeantKine* HMdcCPSelector::getObj(Int_t nTrack) {
   return (HGeantKine*)(fGeantCat->getObject(nTrack-1));
}

Bool_t HMdcCPSelector::checkConv(HGeantKine *kine) {
  
  
  //cout<<"in checkConv PID="<<dummy<<endl;
  Int_t pTrack_par=-1,pMed=-1,pMech=-1,pID_par=0,pTrack_par_par=-1,pMed_par=-1,pMech_par=-1,pID_par_par=-1;
  Float_t mom_x=0,mom_y=0,mom_z=0;
  Int_t pTrack_particle=-1,pID_particle=0;


  kine->getParticle(pTrack_particle,pID_particle);
  kine->getCreator(pTrack_par,pMed,pMech);


  if(pTrack_par!=0){
    getObj(pTrack_par)->getParticle(pTrack_par,pID_par);
    //cout<<"in checkConv rodic="<<pID_par<<endl;
    if(pID_par==1){
      getObj(pTrack_par)->getCreator(pTrack_par_par,pMed_par,pMech_par);
      if(pTrack_par_par!=0){
	getObj(pTrack_par_par)->getParticle(pTrack_par_par,pID_par_par);
	//	cout<<"in checkConv prarodic ="<<pID_par_par<<endl;
	if(pID_par_par==7){
          //  printf("===========in checkConv=====================\n");
// 	   printf("particle track=%i id=%i\n",pTrack_particle,pID_particle);
// 	   printf("parent   track=%i id=%i\n",pTrack_par,pID_par);
// 	   printf("grandparent track=%i id=%i\n",pTrack_par_par,pID_par_par);
	   
	   
	   vec.SetXYZ(mom_x,mom_y,mom_z);
	   // cout<<"this is pi0->gama->e+e-"<<endl;
// 	   printf("===========out checkConv=====================\n");
	   getObj(pTrack_par_par)->getMomentum(mom_x,mom_y,mom_z);
	   vec.SetXYZ(mom_x,mom_y,mom_z);
	   pID=pID_particle;
	   pTrack_parent=pTrack_par;
	   return kTRUE ;
	}
      }
    }
  }
 //  printf("===========in checkConv=====================\n");
//   printf("particle track=%i id=%i\n",pTrack_particle,pID_particle);
//   printf("parent   track=%i id=%i\n",pTrack_par,pID_par);
//   printf("grandparent track=%i id=%i\n",pTrack_par_par,pID_par_par);
  
 
  vec.SetXYZ(mom_x,mom_y,mom_z);
  // cout<<"this is not pi0->gama->e+e-"<<endl;
//   printf("===========out checkConv=====================\n");
  return  kFALSE;
}

Bool_t  HMdcCPSelector::checkIfLept(HGeantKine *kine){
  Int_t pTrack_particle=-1,pID_particle=0;
  
  kine->getParticle(pTrack_particle,pID_particle);
  
  if(pID_particle==2||pID_particle==3) return  kTRUE;
  return  kFALSE;
}


void HMdcCPSelector::analyzeDouble(HMdcSegSim *mdchit) {
  //cout<<"in HMdcCPSelector::analyzeDouble"<<endl; 
  Int_t pID2=0,pID1=-1,pTrack_par1=-1,pTrack_par2=-2;
  TVector3 vec2;
  TVector3 vec1;
  
  //  printf("****************ANALYZE DOUBLE*************IN****************\n");
  //now check if this is electron positron pair from one gama and pi0
  
  
  if(checkConv(getObj(mdchit->getTrack(0)))) {
    //vec,pID,pTrack_parent are global and filed by checkConv funcion
    vec1=vec;
    pID1=pID;
    pTrack_par1=pTrack_parent;
  }  
  
  if(checkConv(getObj(mdchit->getTrack(1)))) {
    vec2=vec;
    pID2=pID;
    pTrack_par2=pTrack_parent;
  }  
  // printf("ID of the pair ID1=%i ID2=%i\n",pID1,pID2);
  // printf(" pTrack_parent of the pair pTrack_parent1=%i pTrack_parent2=%i\n",pTrack_par1,pTrack_par2);
  
  if(((pID1==2&&pID2==3)||(pID2==2&&pID1==3))&&(pTrack_par2==pTrack_par1)){
    //cout<<"!!!CONVERSION PAIR!!!!!"<<endl;
    //first two tracks are e+e- pair from conversion 
    fillHistoDouble(mdchit);
    checkRich(mdchit,2);
  }
  //printf("****************ANALYZE DOUBLE*******OUT**********************\n");
}


void HMdcCPSelector::analyzeSingle(HMdcSegSim *mdchit) {
  //candidate for sigle
  // printf("candidate for single tr1=%i mul1=%i tr2=%i mul2=%i  \n",mdchit->getTrack(0),mdchit->getNTimes(0),mdchit->getTrack(1),mdchit->getNTimes(1));
  
  Int_t pID1=-1,pTrack_par1=-1;
  TVector3 vec2;
  TVector3 vec1;
  
  //printf("****************ANALYZE SINGLE*************IN****************\n");
  //now check if this is electron or  positron  from one gama and pi0
  
  if(all_flag) {
    //check only whether this track is lepton
   
    if(checkIfLept(getObj(mdchit->getTrack(0)))) {
     
      fillHistoSingle(mdchit); 
      checkRich(mdchit,1);
    }
  } else {
    if(checkConv(getObj(mdchit->getTrack(0)))) {
      vec1=vec;
      pID1=pID;
      pTrack_par1=pTrack_parent;
      // cout<<"!!!CONVERSION SINGLE!!!!!"<<endl;
      // this lepton is single and comes from conversion
      fillHistoSingle(mdchit);
      checkRich(mdchit,1);
    }
  }  
}


Bool_t HMdcCPSelector::fillHistoDouble(HMdcSegSim *mdchit) {
  //cout<<"in HMdcCPSelector::fillHistoDouble"<<endl;
  Float_t  cls,mdc_phi,mdc_theta;

  Float_t sigma1, sigma2, alpha;
  
  Int_t nwires,mdc_theta_bin,mdc_phi_bin,nmerged,level;
  
  nmerged=getNMergedClsInSeg(mdchit); 
  cls=getClusterSizeInSeg(mdchit);
  

  nwires=getNWiresInSeg(mdchit);

  mdc_phi=mdchit->getPhi()*180.0/3.141592-60.0; 
  mdc_theta=mdchit->getTheta()*180.0/3.141592;
  //Gosia

  
  sigma1=getSigma1InSeg(mdchit);
  sigma2=getSigma2InSeg(mdchit);
  alpha=getAlphaInSeg(mdchit);
  level=getLevelClFindingInSeg(mdchit);
  
  mdc_doubles->Fill(mdc_theta, mdc_phi,cls,nwires,nmerged, sigma1, sigma2, alpha,level);

  if(mdc_phi<30) {
    mdc_phi_bin=(Int_t )mdc_phi/phi_bin_size;
  }else{
    mdc_phi_bin=(Int_t )(60-mdc_phi)/phi_bin_size; 
  }
  mdc_theta_bin=(Int_t )mdc_theta/theta_bin_size-1;

  conv_mdcseg->Fill(mdc_theta,mdc_phi,mdc_theta_bin,mdc_phi_bin,cls,nwires,nmerged,1);
  return kTRUE;
}
Bool_t HMdcCPSelector::fillHistoSingle(HMdcSegSim *mdchit) {

  Float_t  cls,mdc_phi,mdc_theta;
  Int_t nwires,mdc_theta_bin,mdc_phi_bin,nmerged,level;
  Double_t sigma1, sigma2, alpha;
  
  cls=getClusterSizeInSeg(mdchit);
  nwires=getNWiresInSeg(mdchit);
  nmerged=getNMergedClsInSeg(mdchit); 
  mdc_phi=mdchit->getPhi()*180.0/3.141592-60.0; 
  mdc_theta=mdchit->getTheta()*180.0/3.141592;
  sigma1=getSigma1InSeg(mdchit);
  sigma2=getSigma2InSeg(mdchit);
  alpha=getAlphaInSeg(mdchit);
  level=getLevelClFindingInSeg(mdchit);
  
  mdc_singles->Fill(mdc_theta, mdc_phi,cls,nwires,nmerged, sigma1, sigma2, alpha,level);

  if(mdc_phi<30) {
    mdc_phi_bin=(Int_t )mdc_phi/phi_bin_size;
  }else{
    mdc_phi_bin=(Int_t )(60-mdc_phi)/phi_bin_size; 
  }
  mdc_theta_bin=(Int_t )mdc_theta/theta_bin_size-1;
  
  conv_mdcseg->Fill(mdc_theta,mdc_phi,mdc_theta_bin,mdc_phi_bin,cls,nwires,nmerged,0);  
  return kTRUE;
}

void HMdcCPSelector::checkRich(HMdcSegSim *mdchit,Int_t mdccase) {
  //cout<<"in HMdcCPSelector::checkRich"<<endl;
  HRichHitSim *pObj_rich; 
  Float_t mdc_clustersize=getClusterSizeInSeg(mdchit);
 
  //  Int_t nwires=(mdchit->getNwires())/100;
  // Int_t nmergedcls=(mdchit->getNwires())%100;
  // until kick plane overwrite seg nwires we retrieve nwires from hit
  Int_t nwires=getNWiresInSeg(mdchit);
  Int_t nmergedcls=getNMergedClsInSeg(mdchit);
  // printf("wir=%i cls=%i\n",nwires,nmergedcls);
  Float_t sigma1=getSigma1InSeg(mdchit);
  Float_t sigma2=getSigma2InSeg(mdchit);
  Float_t alpha=getAlphaInSeg(mdchit);
  Int_t  level=getLevelClFindingInSeg(mdchit);
  Float_t mdc_phi=mdchit->getPhi()*180.0/3.141592-60.0; 
  Float_t mdc_theta=mdchit->getTheta()*180.0/3.141592;
  
  Int_t mdc_phi_bin;
  
  //e.g. 10-10 and 50-60 is bin 0
  if(mdc_phi<30) {
    mdc_phi_bin=(Int_t) mdc_phi/phi_bin_size;
  }else{
   mdc_phi_bin=(Int_t) (60-mdc_phi)/phi_bin_size; 
  }

  Int_t rich_theta_bin;
  Int_t rich_phi_bin; 
 
  iterator_richhit->Reset();
  if(mdccase==1){
    //case single
    // cout<<"---------------"<<endl;
    while ((pObj_rich=(HRichHitSim*)iterator_richhit->Next())!= 0) {
      //printf("rich weights= %i %i %i\n",pObj_rich->weigTrack1,pObj_rich->weigTrack2,pObj_rich->weigTrack2);
      if((pObj_rich->iRingAlgorithmIndex%2)==1&&(pObj_rich->iRingHouTra>28&&pObj_rich->track2==0)&&checkRing(pObj_rich)){ 
	// if(pObj_rich->track2==0){ 
       	//  printf("rich track= %i %i %i\n",pObj_rich.track1,pObj_rich.track2,pObj_rich.track3);
	//printf("rich weights= %i %i %i\n",pObj_rich.weigTrack1,pObj_rich.weigTrack2,pObj_rich.weigTrack2);
	
	rich_theta_bin=(Int_t) pObj_rich->getTheta()/10-1;
	rich_phi_bin=(((Int_t) pObj_rich->getPhi())%60)/10; //here goes phi 0-360 I want 0-60 in sector
        if(rich_phi_bin>2) rich_phi_bin=5-rich_phi_bin ;//e.g. 10-10 and 50-60 is bin 0 

	if(pObj_rich->track1==mdchit->getTrack(0)){
	  //printf("this mean that I have rich-mdc common single cluster Alg=%i HoughTra=%i PadNr=%i\n",pObj_rich.iRingAlgorithmIndex,pObj_rich.iRingHouTra, pObj_rich.iRingPadNr);
	  
	  //FILL INFO ABOUT THIS RICH_MDC SINGLE CLUSTER
	  
	  // rich and mdc hits should fall in the same theta,phi bin
	  // printf("=============================================\n") ; 
// 	  printf("mdc theta=%f phi=%f  tbin=%i pbin=%i \n",mdc_theta,mdc_phi,mdc_theta_bin,mdc_phi_bin) ;
// 	  printf("rich theta=%f phi=%f  tbin=%i pbin=%i \n",pObj_rich->getTheta(),pObj_rich->getPhi(),rich_theta_bin,rich_phi_bin) ;
//           printf("maxthetabin=%i maxphibin=%i \n",theta_nbin,phi_nbin) ;

	  singles->Fill(mdc_theta,mdc_phi,pObj_rich->getTheta(),pObj_rich->getPhi(),mdc_clustersize,nwires,pObj_rich->iRingAmplitude,nmergedcls,sigma1,sigma2,alpha,level);
         	  
        } 
      }
    }
  } else if (mdccase==2){
    //case double
    //printf("check Rich-Mdc\n");
    while ((pObj_rich=(HRichHitSim*)iterator_richhit->Next())!= 0) {
      // printf("inside richhitsim\n");
      // printf("RingAlgorithm=%i RingHouTra=%i \n",pObj_rich->iRingAlgorithmIndex,pObj_rich->iRingHouTra);
      if((pObj_rich->iRingAlgorithmIndex%2)==1&&(pObj_rich->iRingHouTra>28&&checkRing(pObj_rich))){
	// printf("rich track= %i %i %i\n",pObj_rich->track1,pObj_rich->track2,pObj_rich->track3);

        Bool_t flag1=kFALSE;
	Bool_t flag2=kFALSE;
	Bool_t flag3=kFALSE;

        if(mdchit->getTrack(0)==pObj_rich->track1&&(mdchit->getTrack(1)==pObj_rich->track2||mdchit->getTrack(1)==pObj_rich->track3)) flag1=kTRUE;
        if(mdchit->getTrack(0)==pObj_rich->track2&&(mdchit->getTrack(1)==pObj_rich->track1||mdchit->getTrack(1)==pObj_rich->track3)) flag2=kTRUE;
	if(mdchit->getTrack(0)==pObj_rich->track3&&(mdchit->getTrack(1)==pObj_rich->track1||mdchit->getTrack(1)==pObj_rich->track2)) flag3=kTRUE;

	//	if((pObj_rich->track1==mdchit->getTrack(0)&&pObj_rich->track2==mdchit->getTrack(1))||(pObj_rich->track1==mdchit->getTrack(1)&&pObj_rich->track2==mdchit->getTrack(0)) ){
	if(flag1||(flag2||flag3)){  
        // cout<<"double rcase"<<endl;
	  // printf("this mean that I have rich-mdc common double cluster Alg=%i HoughTra=%i PadNr=%i\n",pObj_rich.iRingAlgorithmIndex,pObj_rich.iRingHouTra, pObj_rich.iRingPadNr);
	  
	  //FILL INFO ABOUT THIS RICH_MDC DOUBLE CLUSTER
	  rich_theta_bin=(Int_t) pObj_rich->getTheta()/theta_bin_size-1;
	  rich_phi_bin=(((Int_t) pObj_rich->getPhi())%60)/10; //here goes phi 0-360 I want 0-60 in sector
	  if(rich_phi_bin>2) rich_phi_bin=5-rich_phi_bin ;//e.g. 10-10 and 50-60 is bin 0   
	  doubles->Fill(mdc_theta,mdc_phi,pObj_rich->getTheta(),pObj_rich->getPhi(),mdc_clustersize,nwires,pObj_rich->iRingAmplitude,nmergedcls,sigma1,sigma2,alpha,level);
	
	} 	
      }
    }    
  }
}

 
void HMdcCPSelector::findConvPairsInGeantMdc(){
  // cout<<"in HMdcCPSelector::findConvPairsInGeantMdc() "<<endl;
  // I look only in Layer 0 because in this cont are all hits in all layers
  Float_t px,py,pz,theta,phi;
  Int_t aPar,  aMed, aMech,module;
  Int_t tracknum;
  HGeantMdc *pObj_geantmdc=NULL; 
  HGeantMdc *pObj_geantmdc2=NULL; 
  
  Int_t imod0_1=0;
 
  iterator_mdcgeant->Reset();
  while ((pObj_geantmdc=(HGeantMdc*)iterator_mdcgeant->Next())!= 0) { 
    if(pObj_geantmdc->getLayer()!=0) continue; 
    tracknum=pObj_geantmdc->getTrack();
    // cout<<"tracknum="<<tracknum<<endl;
    if(checkConv(getObj(tracknum))){
      //this mean that this track in HGeantMdc one conversion is
      getObj(tracknum)->getMomentum(px,py,pz);
      getObj(tracknum)->getCreator(aPar,aMed,aMech);
      module=pObj_geantmdc->getModule();
      theta=getGeantTheta(px,py,pz);
      phi=getGeantPhi(px,py);
    
      if(module==0){
	iterator_mdcgeant_2->Reset();	
	imod0_1=0;
	// cout<<"====trk1 "<<tracknum<<endl;
	while ((pObj_geantmdc2=(HGeantMdc*)iterator_mdcgeant_2->Next())!= 0) { 
	  if(pObj_geantmdc2->getModule()!=1) continue; 
	  if(pObj_geantmdc2->getLayer()!=0) continue;
	  // cout<<"trk2 "<< pObj_geantmdc2->getTrack()<<endl; 
	  if(pObj_geantmdc2->getTrack()==tracknum){
	    imod0_1=1;
	    break;
	  }	
	  // printf("mod0=%i mod1=%i  \n",module,pObj_geantmdc2->getModule());
	}

	//	cout<<"RESULT: imod0 "<<imod0_1<<endl;
      }
     
      conv_mdcgeant->Fill(theta,phi,sqrt(px*px+py*py+pz*pz),aMed,module,imod0_1);
     
   }
  }
  // cout<<"out HMdcCPSelector::findConvPairsInGeantMdc() "<<endl;
}


void HMdcCPSelector::findConvPairsInGeantRichMirr(){
  // cout<<"in HMdcCPSelector::findConvPairsInGeantRichMirr() "<<endl;
  Float_t px,py,pz,theta,phi;
  HGeantRichMirror *pObj_richmirr=NULL;
   
  Int_t aTrack,aID,aPar,aMed,aMech,rsector;
  Int_t inmdc_0=0,inmdc_01=0;
  iterator_richmirror->Reset();
  while ((pObj_richmirr=(HGeantRichMirror*)iterator_richmirror->Next())!= 0) {
    pObj_richmirr->getTrack(aTrack,aID);
    if(checkConv(getObj(aTrack))){
      getObj(aTrack)->getMomentum(px,py,pz);
      getObj(aTrack)->getCreator(aPar,aMed,aMech);
      rsector=pObj_richmirr->getSector();
      theta=getGeantTheta(px,py,pz);
      phi=getGeantPhi(px,py);

      inmdc_0=isConvPairsInGeantMdc(aTrack,rsector,0);
      inmdc_01=isConvPairsInGeantMdc(aTrack,rsector,5);

      conv_richmirrgeant->Fill(theta,phi,sqrt(px*px+py*py+pz*pz),aMed,inmdc_0,inmdc_01);
      

    }
  }
  //cout<<"out HMdcCPSelector::findConvPairsInGeantRichMirr() "<<endl;
}

Bool_t  HMdcCPSelector::isConvPairsInGeantMdc(Int_t track,Int_t sec,Int_t option){
  //this function count the layers in GeantMdc where the track make in geant hit and if it is at least 4 layers in mod 0 than returns true.
  
  Int_t tracknum;
  HGeantMdc *pObj_geantmdc=NULL; 
  
  //counting layers in var modules
  Int_t count_layers_0=0;
  Int_t count_layers_1=0;
  Int_t count_layers_2=0;
  Int_t count_layers_3=0;
  Int_t minlaymul =4; // minimal multiplicity of layers to have later hit
  Int_t module,layer,sector;
  
  iterator_mdcgeant->Reset();
  while ((pObj_geantmdc=(HGeantMdc*)iterator_mdcgeant->Next())!= 0) { 
    tracknum=pObj_geantmdc->getTrack();
    sector=pObj_geantmdc->getSector();
    if(track==tracknum&&sec==sector){
      module=pObj_geantmdc->getModule();
      layer=pObj_geantmdc->getLayer();
      
      switch (option) {

      case 0:
	//counting in module 0
	if(module==0) count_layers_0++;
        if(count_layers_0>=minlaymul) return kTRUE; 
      case 1:
	//counting in module 1
	if(module==1) count_layers_1++;
        if(count_layers_1>=minlaymul) return kTRUE; 	
      case 2:
	//counting in module 2
	if(module==2) count_layers_2++;
        if(count_layers_2>=minlaymul) return kTRUE; 
      case 3:
	//counting in module 3
	if(module==3) count_layers_3++;
        if(count_layers_3>=minlaymul) return kTRUE; 
      case 5:
	//counting in moduls 0&1
	if(module==0) count_layers_0++;
        if(module==1) count_layers_1++;

        if((count_layers_0+count_layers_1)>=minlaymul) return kTRUE;  
      case 6:
	//counting in moduls 2&3
	if(module==2) count_layers_2++;
        if(module==3) count_layers_3++;
	
        if((count_layers_2+count_layers_3)>=minlaymul) return kTRUE;  
        
      case 7:
       
	//counting in moduls 0&1-2&3
	if(module==0) count_layers_0++;
        if(module==1) count_layers_1++;
	if(module==2) count_layers_2++;
        if(module==3) count_layers_3++;
	if(((count_layers_0+count_layers_1)>=minlaymul)&&((count_layers_2+count_layers_3)>=minlaymul)) return kTRUE;  
      };
    }
  }
  return kFALSE;
}

Bool_t  HMdcCPSelector::checkRing(HRichHitSim *richhit){
  //cout<<"in checkRing"<<endl;
  //in this method it is checked if the possition of the found ring center corresponds with hit on Mirror 
  Int_t xMirrPad,yMirrPad; //in pad units from richhitsim 
  Float_t mir_ring_x,mir_ring_y;  //in mm from mirror hit   
  HGeantRichMirror *pObj_richmirr; 
  Int_t mir_tr,mir_parID,weight=0;
  Float_t px,py,pz;
  
  iterator_richmirror->Reset();
  while ((pObj_richmirr=(HGeantRichMirror*)iterator_richmirror->Next())!= 0) {
    pObj_richmirr->getTrack(mir_tr,mir_parID);
    if(mir_tr==richhit->track1||(mir_tr==richhit->track2||mir_tr==richhit->track3)) {
      //if it is single second will not be found indeed
      mir_ring_x=pObj_richmirr->getXRing();
      mir_ring_y=pObj_richmirr->getYRing();
      //for newer hydra version than 5_43 1.415 should go out
      pPad = pPadsPar->getPad(mir_ring_y/10,mir_ring_x/10+1.415);
      //pPad = pPadsPar->getPad(mir_ring_y/10,mir_ring_x/10);
      xMirrPad = pPad->getPadX();
      yMirrPad = pPad->getPadY();
      getObj(mir_tr)->getMomentum(px,py,pz);
      //printf("MOM=%f MIRR: %i %i HIT: %i %i\n",sqrt(px*px+py*py+pz*pz),xMirrPad,yMirrPad,richhit->iRingX,richhit->iRingY);
      //printf("xydiff=%i %i\n",abs(richhit->iRingX-xMirrPad),abs(richhit->iRingY-yMirrPad));
      if (abs(richhit->iRingX-xMirrPad)<2&&abs(richhit->iRingY-yMirrPad)<2){
	// printf("checkRing return kTRUE\n"); 
        
	if(mir_tr==richhit->track1) weight=richhit->weigTrack1;
	if(mir_tr==richhit->track2) weight=richhit->weigTrack2;
	if(mir_tr==richhit->track3) weight=richhit->weigTrack3;
	rich_help->Fill((richhit->iRingX-xMirrPad),(richhit->iRingY-yMirrPad),weight,1); 
	
	return kTRUE;
      } else {
	
        if(mir_tr==richhit->track1) weight=richhit->weigTrack1;
	if(mir_tr==richhit->track2) weight=richhit->weigTrack2;
	if(mir_tr==richhit->track3) weight=richhit->weigTrack3;
        
        rich_help->Fill((richhit->iRingX-xMirrPad),(richhit->iRingY-yMirrPad),weight,0);       
 
      }
      //retrieve xy and compare 
    } 
  }
  //printf("checkRing return kFALSE\n"); 
  return kFALSE;
} 

Int_t HMdcCPSelector::getClusterSizeInSeg(HMdcSegSim *mdcseg){
  HMdcHitSim* mdchit=0;
  mdchit =(HMdcHitSim*)getMdcHitFromSeg(mdcseg);
  if(mdchit) return getClusterSizeFromHit(mdchit);
  return -1;
}



Int_t HMdcCPSelector::getNWiresInSeg(HMdcSegSim *mdcseg){
  HMdcHitSim* mdchit=0;
  HMdcHitSim* mdchit2=0;
  Int_t hit_inx0,hit_inx1,hit_inx ;
  hit_inx0=mdcseg->getHitInd(0);
  hit_inx1=mdcseg->getHitInd(1);
  // printf("==================================\n");
  //printf("in getNWiresInSeg:hit_inx0=%i  hit_inx1=%i \n",hit_inx0,hit_inx1);
  if(hit_inx0>-1&&hit_inx1==-1) {
    hit_inx=hit_inx0;
    mdchit=(HMdcHitSim *)getMdcHitCat()->getObject(hit_inx);
    return getNWiresFromHit(mdchit);
  }
  
  if(hit_inx0==-1&&hit_inx1>-1){
    hit_inx=hit_inx1;
    mdchit=(HMdcHitSim *)getMdcHitCat()->getObject(hit_inx);
    return getNWiresFromHit(mdchit);
  }
  
  if(hit_inx0>-1&&hit_inx1>-1) {
    //this should be always case in combine mode
    mdchit=(HMdcHitSim *)getMdcHitCat()->getObject(hit_inx0);
    mdchit2=(HMdcHitSim *)getMdcHitCat()->getObject(hit_inx1);
    // printf("in getNWiresInSeg: nwires_0=%i nwires_1=%i \n",getNWiresFromHit(mdchit),getNWiresFromHit(mdchit2));  
    return (getNWiresFromHit(mdchit)+getNWiresFromHit(mdchit2));
  }
  return -1;              
}

Int_t HMdcCPSelector::getNMergedClsInSeg(HMdcSegSim *mdcseg){
  HMdcHitSim* mdchit=0;
  mdchit =(HMdcHitSim*)getMdcHitFromSeg(mdcseg);
  if(mdchit) return getNMergedClustersFromHit(mdchit);
  return -1;
}


HMdcHitSim * HMdcCPSelector::getMdcHitFromSeg(HMdcSegSim *mdcseg){

  HMdcHitSim* mdchit=0;
  Int_t hit_inx,hit_inx0,hit_inx1 ;
  hit_inx=-1;
  hit_inx0=mdcseg->getHitInd(0);
  hit_inx1=mdcseg->getHitInd(1);
  
  if(hit_inx0>-1&&hit_inx1==-1) hit_inx=hit_inx0;
  if(hit_inx0==-1&&hit_inx1>-1) hit_inx=hit_inx1;
  if(hit_inx0>-1&&hit_inx1>-1) hit_inx=hit_inx0;
  
    // printf("=================================\n");
  //printf("cltsize cluster =%f\n",mdc->getCls());
  if(hit_inx>-1){
    mdchit=(HMdcHitSim *)getMdcHitCat()->getObject(hit_inx);
    return mdchit;
  } 
  else {
    printf("NO hit in this module \n");
  }
  
  return NULL;

}


Float_t HMdcCPSelector::getGeantPhi(Float_t px,Float_t py) {
  //one has to look at axis definition in geant to understand it
  //or Page 35 of my logbook
  Float_t phi=-1.0;
  if(px!=0){
    phi=atan(py/px)*180/3.1415;
    if(px>0&py>0) phi=phi+0.0;
    if(px<0&py>0) phi=phi+180.0;
    if(px<0&py<0) phi=phi+180.0;
    if(px>0&py<0) phi=phi+360.0;
  }
  return phi;
}

Float_t HMdcCPSelector::getGeantTheta(Float_t px,Float_t py,Float_t pz) {
  Float_t theta=-1.0;
  if(pz!=0.0) {
       theta=(180.0/3.1415)*acos(pz/sqrt(px*px+py*py+pz*pz));
     }else{ 
       theta= -1.0;
     }
  return theta;
}

void HMdcCPSelector::writeHisto2File(void) {
TFile* myoutfile = new TFile(nameHistoFile,"RECREATE");
  myoutfile->cd();

  singles->Write();
  doubles->Write();
  conv_mdcgeant->Write();
  conv_richmirrgeant->Write();
  conv_mdcseg->Write();
  conv_richhit->Write();
  mdc_singles->Write();
  mdc_doubles->Write();
  hsystem->Write();
  
  myoutfile->Close(); 
}

Int_t  HMdcCPSelector::getClusterSizeFromHit(HMdcHitSim * mdchit) {
  
  if(version_flag) return   mdchit->getFlag()>>16; //version up to v6_08
 
  Int_t clssize=-1;
  Int_t clsindex=-1;
  HMdcClusInf *pObj_clusinf=NULL;
  
  clsindex=mdchit->getClusInfIndex();
  pObj_clusinf=getClusInfObj(clsindex);
  if(pObj_clusinf){
   clssize=pObj_clusinf->getClusSize();
  } else {
    return -1;
  }
  return clssize;
}


Int_t  HMdcCPSelector::getNWiresFromHit(HMdcHitSim * mdchit) {
  
  if(version_flag) return  (mdchit->getFlag()&0xFFFF)/100;//version up to v6_08

  
  Int_t ndrtimes=-1;
  Int_t clsindex=-1;
  HMdcClusInf *pObj_clusinf=NULL;
  
  clsindex=mdchit->getClusInfIndex();
  //printf("in getNWiresFromHit():: coresp clsinfoindex=%i \n",clsindex);
  pObj_clusinf=getClusInfObj(clsindex);
  
  if(pObj_clusinf){
    ndrtimes=pObj_clusinf->getNDrTimes();
  } else {
    return -1;
  }
  return ndrtimes;
} 

Int_t  HMdcCPSelector::getNMergedClustersFromHit(HMdcHitSim * mdchit) {
  
  
  if(version_flag) return  (mdchit->getFlag()&0xFFFF)%100;//version up to v6_08
  
  Int_t nmergcls=-1;
  Int_t clsindex=-1;
  HMdcClusInf *pObj_clusinf=NULL;
  
  clsindex=mdchit->getClusInfIndex();
  pObj_clusinf=getClusInfObj(clsindex);
  if(pObj_clusinf){
    nmergcls=pObj_clusinf->getNMergClust();
  } else {
    return -1;
  }
  return nmergcls;
}

Float_t HMdcCPSelector::getSigma1InSeg(HMdcSegSim *segsim) {

  Float_t sigma1;
  
  HMdcHitSim *mdchit =0;
  mdchit = (HMdcHitSim*)getMdcHitFromSeg(segsim);
  sigma1 = getSigma1InHit(mdchit);
  return sigma1;
}

Float_t HMdcCPSelector::getSigma1InHit(HMdcHitSim *mdchit){
  
  Float_t sigma1;
  Int_t clsindex;
  HMdcClusInf *pObj_clusinf;
  
  clsindex=mdchit->getClusInfIndex();
  pObj_clusinf=getClusInfObj(clsindex);
  if(pObj_clusinf){
    sigma1=pObj_clusinf->getSigma1();}
  else{return -1;}
  
  return sigma1;
  
}

Float_t HMdcCPSelector::getSigma2InSeg(HMdcSegSim *segsim) {
  
  Float_t sigma2;
  
  HMdcHitSim *mdchit =0;
  mdchit = (HMdcHitSim*)getMdcHitFromSeg(segsim);
  sigma2 = getSigma2InHit(mdchit);
  return sigma2;
}

Float_t HMdcCPSelector::getSigma2InHit(HMdcHitSim *mdchit){
  
  Float_t sigma2;
  Int_t clsindex;
  HMdcClusInf *pObj_clusinf;

  clsindex=mdchit->getClusInfIndex();
  pObj_clusinf=getClusInfObj(clsindex);
  if(pObj_clusinf){
    sigma2=pObj_clusinf->getSigma2();}
  else{return -1;}
  
  return sigma2;
  
}

Float_t HMdcCPSelector::getAlphaInSeg(HMdcSegSim *segsim) {
  
  Float_t alpha;
  
  HMdcHitSim *mdchit =0;
  mdchit = (HMdcHitSim*)getMdcHitFromSeg(segsim);
  alpha = getAlphaInHit(mdchit);
  return alpha;
}

Float_t HMdcCPSelector::getAlphaInHit(HMdcHitSim *mdchit){
  Float_t alpha;
  Int_t clsindex;
  HMdcClusInf *pObj_clusinf;
  
  clsindex=mdchit->getClusInfIndex();
  pObj_clusinf=getClusInfObj(clsindex);
  if(pObj_clusinf){
    alpha=pObj_clusinf->getAlpha();
  }
  else{
    return -1;
  }
  return alpha;
}



Int_t  HMdcCPSelector::getLevelClFindingInSeg(HMdcSegSim *segsim){
  
  Int_t clslevel=-1;
  HMdcHitSim *mdchit =0;
  mdchit = (HMdcHitSim*)getMdcHitFromSeg(segsim);
  clslevel= getLevelClFindingInHit(mdchit);
  return clslevel;
}

Int_t  HMdcCPSelector::getLevelClFindingInHit(HMdcHitSim *mdchit){
  HMdcHitSim * pObj_mdc_hit=NULL;
  Int_t count_hits=0; 
  Int_t clslevel=-1;
  Int_t hit_module=mdchit->getModule();
  if( version_flag) {
    //here I have to count hits in the same module to get clslevel
    iterator_mdchit->Reset(); 
    while ( (pObj_mdc_hit=(HMdcHitSim *)iterator_mdchit->Next())!=NULL) {
      if(pObj_mdc_hit->getModule()==hit_module) count_hits++  ;
    }
    clslevel=4;
    if(count_hits>10) clslevel=5;
    if(count_hits>50) clslevel=6;  
    return clslevel;
  }

  
 
  Int_t clsindex=-1;
  HMdcClusInf *pObj_clusinf;
  
  clsindex=mdchit->getClusInfIndex();
  pObj_clusinf=getClusInfObj(clsindex);
  if(pObj_clusinf){
    clslevel=pObj_clusinf->getLevelClFinding();
  }
  else{
    return -1;
  }
  return clslevel;
}



HMdcClusInf* HMdcCPSelector::getClusInfObj(Int_t clsindex){
  //get ClusInfObj from category using index
 
  HMdcClusInf *pObj_clusinf=NULL; 
  pObj_clusinf=(HMdcClusInf* )getClusInfCat()->getObject(clsindex);
  return pObj_clusinf;
}





ClassImp(HMdcCPSelector)






