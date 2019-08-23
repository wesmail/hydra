
// File: hpseudotracking.cc
//
// Author: Jaro Bielcik <bielcik@gsi.de>
// Last update by Laura Fabbietti 06/02/01 19:30
// changed by: T. Eberl (Thomas.Eberl@ph.tum.de), 
// Thu Jul 11 13:35:28 CEST 2001
using namespace std;
#include "hpseudotracking.h"
#include "hdebug.h"
#include "hades.h"
#include "tofdef.h"
#include "richdef.h"
#include "hrichdetector.h"
#include "hiterator.h"
#include "hspectrometer.h"
#include "htofdetector.h"
#include "hevent.h"
#include "hcategory.h"
#include "hlinearcategory.h"
#include <iostream> 
#include <iomanip>
#include "htofhitsim.h"
#include "hgeantkine.h"
#include "TRandom.h"
#include "haddef.h"
#include "hmdccal1.h"
#include "hmdccal1sim.h"
#include "hrichhitsim.h"
#include "hgeantrich.h"
#include "hparticlesim.h"
#include "hgeantkine.h"
#include "hlocation.h"
#include "hshowerhittrack.h"
#include "hmdcdef.h"
#include "showerdef.h"
#include "TIterator.h"
#include <TArrayI.h>
#include "hkinesim.h"
#include "hmdcsegsim.h"
#include "hphysicsconstants.h"
#include <TLorentzVector.h>
#include "hevent.h"
HPseudoTracking::HPseudoTracking(void) {
 
    rich_tracknF.Set(100);
    rich_trackn.Set(100);
    rich_Theta.Set(100);
    rich_Phi.Set(100);
    mdc_trackn=new TArrayI(100);
    tof_trackn=new TArrayI(100);
    shower_trackn=new TArrayI(100);
    iter_rich = NULL;
    iter_tof= NULL;
    iter_mdc= NULL;
    iter_shower= NULL;
    iter_tofino= NULL;
    iter= NULL;
    evtCount= 0;

}

HPseudoTracking::HPseudoTracking(Text_t *name,Text_t *title,Int_t closeRej) : 
    HReconstructor(name,title) {
   i_RJ = closeRej;
   rich_tracknF.Set(100);
   rich_trackn.Set(100);
   rich_Theta.Set(100);
   rich_Phi.Set(100);
   mdc_trackn=new TArrayI(100);
   tof_trackn=new TArrayI(100);
   shower_trackn=new TArrayI(100);
   iter_rich = NULL;
   iter_tof= NULL;
   iter_mdc= NULL;
   iter_shower= NULL;
   iter_tofino= NULL;
   iter= NULL;
   evtCount= 0; 
}

HPseudoTracking::~HPseudoTracking(void) {
    // why no deletion of heap objects ??

  //if (rich_trackn) delete [] rich_trackn;
  //if ( mdc_trackn) delete [] mdc_trackn;
  //if (tof_trackn) delete [] tof_trackn;
  //if (shower_trackn) delete [] shower_trackn;

  }

Bool_t HPseudoTracking::init(void) {
    HEvent *event=gHades->getCurrentEvent();
    fGeantKineCat =(HLinearCategory*) gHades->getCurrentEvent()->getCategory(catGeantKine);
    if (!fGeantKineCat) { cout<<"no GEANT Kine category available\n"<<endl; }                
    iter_kine = (HIterator *)fGeantKineCat ->MakeIterator("native");
    
    // set HGeantKine category pointer
    fHGeantKine=gHades->getCurrentEvent()->getCategory(catGeantKine);
    if (!fHGeantKine){ fHGeantKine = new HLinearCategory("HGeantKine");
    gHades->getCurrentEvent()->addCategory(catGeantKine,fHGeantKine,"HGeantKine"); 
    }
    iter = (HIterator *)fHGeantKine->MakeIterator("native"); 
    
    cout<<" after hkine "<<endl;
    //  fParticle = gHades->getCurrentEvent()->getCategory(catParticle);   
//      if (!fParticle){
//  	fParticle = new HLinearCategory("HParticleSim");
//  	gHades->getCurrentEvent()->addCategory(catParticle,fParticle,"Particle");
	
//      }
    // set particle category pointer
    // =========================================================
    // new category creation for HParticle
    fParticle=gHades->getCurrentEvent()->getCategory(catParticle);
  if (!fParticle) {
    fParticle = new HLinearCategory("HParticleSim",1000);
    if (fParticle) gHades->getCurrentEvent()->addCategory(catParticle,fParticle,"PhyAna");
    else {
      Error("init","Unable to create HParticle container");
      return kFALSE;
    }
  }
  // ===========================================================
    iter_part  = (HIterator *)fParticle ->MakeIterator("native");
    
    cout<<" after particle "<<endl;    
    //set tof category pointer and create iterator  
    fTofCat=gHades->getCurrentEvent()->getCategory(catTofHit);
    if (!fTofCat) {
	fTofCat=gHades->getSetup()->getDetector("Tof")->buildCategory(catTofHit);
	if (!fTofCat) return kFALSE;
	else gHades->getCurrentEvent()->
		 addCategory(catTofHit,fTofCat,"Tof");
    }
    
    iter_tof=(HIterator *)fTofCat->MakeIterator("native");
    
    
    cout<<"after tof"<<endl;
    //set mdc category pointer and create iterator  
    fMdcCat=event->getCategory(catMdcSeg);
    if (!fMdcCat) {
	Error("init","No MDC segment category defined");
	return kFALSE;
    }
    else iter_mdc=(HIterator *)fMdcCat->MakeIterator(); 
    
    //  fMdcCat=gHades->getCurrentEvent()->getCategory(catMdcCal1);
    
    //     if (!fMdcCat) {
// 	fMdcCat=gHades->getSetup()->getDetector("Mdc")->buildCategory(catMdcCal1);
    // 	if (!fMdcCat) return kFALSE;
    // 	else gHades->getCurrentEvent()->
    // 		 addCategory(catMdcCal1,fMdcCat,"Mdc");
    //     }
    
    //     iter_mdc=(HIterator *)fMdcCat->MakeIterator(); 
    
    cout<<" after mdc "<<endl;    
    // set shower  category pointer and create iterator  
    
    fShowerCat=gHades->getCurrentEvent()->getCategory(catShowerHitTrack);
    if (!fShowerCat) {
	fShowerCat=gHades->getSetup()->getDetector("Shower")->buildCategory(catShowerHitTrack);
	if (!fShowerCat) return kFALSE;
	else gHades->getCurrentEvent()->
		 addCategory(catShowerHitTrack,fShowerCat,"Shower");
    }
    
    iter_shower=(HIterator *)fShowerCat->MakeIterator("native"); 
    
    

    // set rich category pointer and create iterator

    fRichCat=gHades->getCurrentEvent()->getCategory(catRichHit);
    if (!fRichCat) {
	fRichCat =gHades->getSetup()->getDetector("Rich")->buildCategory(catRichHit);
	if (!fRichCat) return kFALSE;
	else gHades->getCurrentEvent()->addCategory(catRichHit,fRichCat,"Rich");
    }

    iter_rich = (HIterator *)fRichCat->MakeIterator("native");
    

    return kTRUE;
}


Bool_t HPseudoTracking::checkThetaPhi(Float_t theta,Float_t phi){
    // this function checks that the recognized lepton is produced in
    // the rich and that its polar azymthal angles agreee with the HGeant one
    // since the Theta RIch is off of 2 degree a correciotn is necessary ( 07/05/2001)
    Int_t nTrackKine=0;
    Int_t nId=0;
    HGeantKine * kine =0;
    Float_t x,y,z;
    x = y = z  =0;
    Float_t pX,pY,pZ,pT,pTot,pTheta,pPhi;
    pX=pY=pZ=pT=pTot=pTheta=pPhi=0;
    Float_t factor = 1.0F/57.2958F;
    Int_t aTRUE =0;
    iter_kine->Reset();

    while((kine=(HGeantKine *)iter_kine->Next())!=0){
	
	kine->getParticle(nTrackKine,nId);
	kine->getVertex(x,y,z);
	// is a lepton and is created inside the rich
	if( (nId ==2 || nId ==3) && (z>-40.0F && z<400.0F && x*x+y*y+(z+500.0F)*(z+500.0F) < 810000.0F)){
	    kine->getMomentum(pX,pY,pZ);
	    pT=pX*pX+pY*pY;
	    pTot=sqrt(pT+pZ*pZ);
	    pT=sqrt(pT);
	    pTheta=asin(pT/pTot)/factor;
	    pPhi=asin(pY/pT)/factor;
	    if(pX<0.0F) pPhi=180.0F-pPhi;
	    else if(pY<0.0F) pPhi+=360.0F;	
	    // cout<<" theta kine "<<pTheta<< "  phi kine "<<pPhi<<endl;
	    if(TMath::Abs(theta+2.-pTheta)<5 && TMath::Abs(phi-pPhi)<5){
		//		cout<<" theta "<< theta<< " phi "<<phi<<endl;
		//	cout<<" theta Geant "<< pTheta<< " phi Geant "<<pPhi<<endl;
		aTRUE = 1;
		break;
	    } // endif
	} // endif particle id
    } // end while loop
    
    return aTRUE;
  
}

void HPseudoTracking::fillRichTrackList(){
    //filling RICH tracks in event to the list
  
    HRichHitSim* pObj_rich = NULL;
    Int_t cont = 0;
    Int_t rich_track1,rich_track2;
    rich_track1 = rich_track2 = 0;
    
    iter_rich->Reset();
    
    
    while ((pObj_rich=(HRichHitSim*)iter_rich->Next())!= 0) {
	
	if ((cont+1)== rich_trackn.GetSize()) { // resize
	    rich_Theta.Set(2*cont);
	    rich_Phi.Set(2*cont);
	    rich_trackn.Set(2*cont);
	}
	rich_track1=pObj_rich->track1;
	
	rich_track2=pObj_rich->track2;
	// check that the hit is a lepton and that it is produced in the RICH volume
	if(checkThetaPhi(pObj_rich->getTheta(),pObj_rich->getPhi()) ){
	    // check for track numbers that show up twice
	    // function adds particle values to arrays
	    if (rich_track1) scanRingTrack(pObj_rich,rich_track1);
	    if (rich_track2) scanRingTrack(pObj_rich,rich_track2);
	}
	
    }
    
    if(rich_tracknF.GetSize()!=rich_trackn.GetSize()) rich_tracknF.Set(rich_trackn.GetSize());
    rich_tracknF = rich_trackn; // copy one array to the other
    
    Int_t k =0;  
    while((rich_tracknF.At(k))!=0){ // what is this doing ???
	
	k++;
    }
}


void HPseudoTracking::scanRingTrack(HRichHitSim *richHit, Int_t track){

    // the track array is scanned to see whether there are track numbers
    // which show up twice ( int this case 1 copy has to be removed)
    // this can happen when 2 rings are very closed to each other
    // so that they have some overlap, 
    // but distinguished by the ring finder; then a mixing of track numbers
    // occours between the 2 rings and they are assigned identical track 
    // numbers: both tracks for each ring.

    Bool_t atrue = 0;
    Int_t i =0;
    while((rich_trackn.At(i))!=0){
	if (track==rich_trackn.At(i)) atrue = 1;
	i++;
    }

    
    if(!atrue) {
	rich_trackn.AddAt(track,i);
	rich_Theta.AddAt(richHit->getTheta(),i);
	rich_Phi.AddAt(richHit->getPhi(),i);
       
    }
    
}


void HPseudoTracking::closeLepRejNew(){

    
    // this method apply the ideal close pairs rejection
    // for leptons identified as rings the opening angle
    // is calculated on the base of the tracks.
    
    Int_t i = 0;
    Int_t rich_list_track = 0;
    TList * momLep = new TList();
    TArrayI parentList(10);	
    Float_t xMom,yMom,zMom;
    xMom = yMom = zMom = 0;  
    Int_t parent, dummy = 0;
    parent = dummy = 0;
    rich_tracknF.Reset();

    
    while ((rich_list_track=rich_trackn.At(i))!=0){
	
	getObj(rich_list_track)->getMomentum(xMom,yMom,zMom);
	getObj(rich_list_track)->getCreator(parent,dummy,dummy);
	
	momLep->Add(new TVector3(xMom,yMom,zMom));
	parentList.AddAt(parent,i);
	
	i++;
    }
    
    for(Int_t i = 0;i<momLep->GetSize();i++){
	for(Int_t j = 0;j<momLep->GetSize();j++){
	    
	    Float_t angle = ( (TVector3*)momLep->At(i) )->Angle( *(TVector3*)momLep->At(j) )*57.3;
	    
	    if (angle<15 && angle>0 && i!=j ){
		
		rich_trackn.AddAt(0,i);
		rich_trackn.AddAt(0,j);
		break;
	    }
	    
	}
	
    }
    
    Int_t cont1 = 0;


    for(Int_t i = 0;i<rich_trackn.GetSize();i++){
	if (rich_trackn.At(i)) {
	    rich_tracknF.AddAt(rich_trackn.At(i),cont1);
	    cont1++;
	}
    }



}


void HPseudoTracking::closeLepRej(){
    // same as closeLepRejNew(), but w/o use of TVector3

    // this method apply the close pairs rejection
    // for leptons identified as rings whose opening angle is closer then
    // 15 degree, the 2 leptons have to come from the same parent particle.

  rich_tracknF.Reset();
  Float_t factor = 57.2958;
  Float_t t1,t2,f1,f2 ;
  t1 = t2 = f1 = f2 =0;
  Int_t i = 0;
  Int_t j = 0;

 
  while ((rich_trackn.At(i))!=0){
      while ((rich_trackn.At(j))!=0){
	  t1 = rich_Theta.At(i)/factor;
	  t2 = rich_Theta.At(j)/factor;
	  f1 = rich_Phi.At(i)/factor;
	  f2 = rich_Phi.At(j)/factor;
	  Float_t angle = acos(sin(t1)*sin(t2)*(cos(f1)*cos(f2)+sin(f1)*sin(f2))+cos(t1)*cos(t2))*factor;

	   if (angle<15 && angle>0 && i!=j ){
	       //cout<<"close pair found &&&&&&&&&&&&&&&&&&&&&&&&& "<<endl;
	       // only real pairs are rejected
	       rich_trackn.AddAt(0,i);
	       rich_trackn.AddAt(0,j);
	       break;
	   }
	   j++;
      }
      i++;
  }

  Int_t cont = 0;
  for(Int_t i = 0;i<rich_trackn.GetSize();i++){
      //printf("old array :%d \n",rich_trackn.At(i));
    
    if (rich_trackn.At(i)) {
      
      rich_tracknF.AddAt(rich_trackn.At(i),cont);
      //printf("new array :%d , at: %d\n",rich_tracknF.At(cont),cont);
      cont++;
     
    }
  }
}


void HPseudoTracking::fillMdcTrackList(){
    //filling MDC tracks in event to the list
    

    HMdcSegSim* pObj_mdc = NULL;	  
    Int_t cont = 0;
    Int_t mdc_track = 0;
    iter_mdc->Reset(); 
    Int_t numtracks = 0;

    while ((pObj_mdc=(HMdcSegSim*)iter_mdc->Next())!= 0) { 

	numtracks = pObj_mdc->getNTracks();
	for (Int_t i=0;i<numtracks;i++){
	    if ((cont +1)== mdc_trackn->GetSize() ) mdc_trackn->Set(2*cont);
	    mdc_track=pObj_mdc->getTrack(i);
	    if(mdc_track>0){  
		mdc_trackn->AddAt(mdc_track, cont);
		
		cont++;
	    }
	}
	
    }// end of while
    
    
}


void HPseudoTracking::fillTofTrackList(){
    //filling TOF tracks in event to the list

    HTofHitSim* pObj_tof = NULL;
    Int_t cont = 0;
    Int_t  tof_track= 0;
    iter_tof->Reset();
    while ((pObj_tof=(HTofHitSim*)iter_tof->Next())!= 0) {

	if ((cont + 1) == tof_trackn->GetSize()) tof_trackn->Set(2*cont);
	tof_track=pObj_tof->getNTrack1();
	tof_trackn->AddAt(tof_track,cont);

	cont++;

	tof_track=pObj_tof->getNTrack2();
	if(tof_track>0){ 
	    tof_trackn->AddAt(tof_track,cont);
	    cont++;
	}
    }
    
}

void HPseudoTracking::fillShowerTrackList(){
    //filling Shower tracks in event to the list
    
    
    HShowerHitTrack* pObj_shower = NULL;
    Int_t cont= 0;
    Int_t shower_track = 0;
    iter_shower->Reset(); 

    while ((pObj_shower=(HShowerHitTrack*)iter_shower->Next())!= 0) {                     

	if ((cont+1) == shower_trackn->GetSize()) shower_trackn->Set(2*cont);

	shower_track=pObj_shower->getTrack();
	shower_trackn->AddAt(shower_track,cont);
	cont++;
    }
    
}

void HPseudoTracking::pseudoTracking() {

  //define the lists to store the track numbers of Hits in various detectors
  
    //create iterators over lists of tracks for detectors


    Int_t shower_list_track; // track number in the list 
    Int_t rich_list_track;
    Int_t tof_list_track;
    Int_t mdc_list_track;
     
    Int_t index=0;
        
    //here I fill info which than go to TRACK class about tracks which has  hit in all detectors
    
    TArrayI* foundtrack = new TArrayI(100);
    foundtrack->Reset();
    index=0;
    
    rich_list_track=0;
    mdc_list_track=0;
    tof_list_track=0;
    shower_list_track=0;
    
    // iterate over the lists with the track numbers and finding the track numbers which appear in 
    // RICH-MDC-(TOF||SHOWER)
    // those one are pseudo-found
    
    
    Int_t i,j,k,m;
    i = j =k =m =0;

    while ( (rich_list_track=rich_tracknF.At(i)) != 0 ){
	
	mdc_list_track=0;
	j = 0;
	while ((mdc_list_track=mdc_trackn->At(j))!= 0){

	    if(rich_list_track==mdc_list_track){
		
		tof_list_track=0;
		k = 0;

		while ((tof_list_track=tof_trackn->At(k))!= 0) {
		    if(rich_list_track==tof_list_track){
			
			
			Int_t already_in=0;
			
			for (Int_t ii=0;ii<foundtrack->GetSize();ii++){
			
			    if(mdc_list_track==foundtrack->At(ii)) already_in=1;
			    
			    // cout<<"flag   already_in  : "<<already_in<<endl;
			    
			} //endof for
			
			if (already_in==0){
			    if (index==foundtrack->GetSize()) foundtrack->Set(index+1);
			    foundtrack->AddAt(mdc_list_track,index);
			    //     printf(" ARRAYrich-mdc-tof track=:%i\n",mdc_list_track);
			    count_leptons++;
			    index++;
			} //end if
			
		    } //endif rich-tof
		    k++;
		} // end tof loop
		
		
		shower_list_track=0;
		m = 0;
		while ((shower_list_track=shower_trackn->At(m))!= 0) {
		    if(rich_list_track==shower_list_track){
			//			cout<<"match rich, mdc, shower "<< rich_list_track<<endl;
			Int_t already_in=0;
			for (Int_t ii=0;ii<foundtrack->GetSize();ii++){
			    if(mdc_list_track==foundtrack->At(ii)) already_in=1;  
			}
			if (already_in==0){
			    if (index==foundtrack->GetSize()) foundtrack->Set(index+1);
			    foundtrack->AddAt(mdc_list_track,index);
			    
			    count_leptons++;
			    
			    index++;
			}
		    } // end if rich-shower
		    m++;
		} // end if shower loop
		
	    } // end if rich-mdc matching 
	    j++;
	} // loop over mdc tracks 
	i++;
    }// loop over rich tracks
    
    fillPart(foundtrack); // fill TArrayI with particles.
    
}

void HPseudoTracking::fillPart(TArrayI * pTrackArray){
 
    // read all found track numbers in pTrackArray and look in HGeantKine for inforamtion
    // about this particle and fill the HParticleSim container
 
    // changes needed due to the migration to the new official HParticle class
    HGeantKine* pKine = NULL;
    HParticleSim *pParticle = NULL;
    
    Int_t dummy;
    dummy = 0;
    Int_t trknb = 0;
    Int_t paridx,parentidx,aPar,aMech,aMed;
    paridx = parentidx = aPar = aMech= aMed= 0;
    Float_t weight = 0;
    Float_t gen = 0;
    Float_t xx,yy,zz;
    xx = yy = zz =0;

    Int_t medium,mechanism;
    medium = mechanism = 0;
    Float_t px,py,pz,en;
    TLorentzVector mom4;
    px = py = pz = en = 0;
    
    for (Int_t ii=0;ii<pTrackArray->GetSize();ii++){
	
	if(pTrackArray->At(ii)>0){
	    px = py = pz = en = 0;
	    pKine = (HGeantKine*) fHGeantKine->getObject(pTrackArray->At(ii)-1);          
             
	    pKine->getParticle(trknb,paridx);
    	    pKine->getVertex(xx,yy,zz);
	    pKine->getGenerator(gen,weight);
	    pKine->getMomentum(px,py,pz);
	    pKine->getCreator(dummy,medium,mechanism);
	    
	    en= calcEnergy(px,py,pz,paridx);
	    mom4.SetPxPyPzE(px,py,pz,en);
	    HLocation loc1;  
	    pParticle = (HParticleSim*)((HLinearCategory*)getPartCat())->getNewSlot(loc1);

	    if(pParticle){
		pParticle = new (pParticle) HParticleSim;
		pParticle->setPid(paridx);
		pParticle->setVect4(mom4);
		pParticle->setWeight(weight);
		pParticle->setMedium(medium);
		pParticle->setMechanism(mechanism);
		//pParticle->SetTrackNumber(trknb);
		// later an HVertex object can be added
	    }
	}
    }
}


Float_t  HPseudoTracking::calcEnergy(Float_t px, Float_t py,Float_t pz,int id){
  
    //Float_t mass = PData::Mass(id)*1000;
    Float_t mass =HPhysicsConstants::mass(id);

    return sqrt(px*px+py*py+pz*pz+(mass*mass));
  
}


HGeantKine* HPseudoTracking::getObj(Int_t nTrack){


    Int_t nTrackKine=0;
    Int_t nId=0;
    HGeantKine * kine =0;
    HGeantKine * kine1 =0;
    iter->Reset();

    while((kine=(HGeantKine *)iter->Next())!=0){
	kine->getParticle(nTrackKine,nId);
	
	if (nTrackKine==nTrack){
	    kine1 = kine;
	    break;
	}
	
    }
    
    return kine1;
    
}

Int_t HPseudoTracking::execute(void) {

  evtCount ++;

  fillRichTrackList();

  if(i_RJ ) closeLepRej();

  fillMdcTrackList();
  fillTofTrackList();
  fillShowerTrackList();

  pseudoTracking();

  //assignEvtNum(evtCount);

  rich_trackn.Reset();
  rich_Theta.Reset();
  rich_Phi.Reset();
  rich_tracknF.Reset(); 
  mdc_trackn->Reset();
  tof_trackn->Reset();
  shower_trackn->Reset();

  return 0;
}

//  void HPseudoTracking::assignEvtNum(Int_t evt){

//      HParticleSim * partSim =NULL;
//      iter_part->Reset();

//      while((partSim = (HParticleSim*)iter_part->Next())!=0){
//  	partSim->setEvtNum(evt);
//      }
//  }


Bool_t HPseudoTracking::finalize(void){

  return kTRUE;
  
} 
ClassImp(HPseudoTracking)












