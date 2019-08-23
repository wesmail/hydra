#include "hmdcsimeventinf.h"
#include "hmdcdef.h"
#include "hdebug.h"
#include "hades.h"
//#include "hmdccal1.h"
#include "hiterator.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hevent.h"
#include "hcategory.h"
#include "hlocation.h"
#include <iostream.h>
#include "hgeomvector.h"
#include "hgeantmdc.h"
#include "hgeantkine.h"
#include "hruntimedb.h"
#include "hmatrixcatiter.h"
#include "hlinearcatiter.h"
#include "hrecevent.h"
#include "hpartialevent.h"


//*-- Author : V.Pechenov
//*-- Modified: 14.04.99

ClassImp(HMdcSimTrackInf)
ClassImp(HMdcSimEventInf)

HMdcSimEventInf::HMdcSimEventInf() {
  fCalCat=0;
  fGeantMdcCat=0;
  fGeantKineCat=0;
  iter=0;
  iterin=0;
  array=new TObjArray(20000);
  isSimEvent=-1;
  
  nTracks=0;
  eventId=0;
}

HMdcSimEventInf::~HMdcSimEventInf() {
 if(iterin) delete iterin;
 array->Delete();
 if (!iter) delete iter; 
}


void HMdcSimEventInf::setParContainers() {
 isSimEvent=1;
 fCalCat=gHades->getCurrentEvent()->getCategory(catMdcCal1);
 if (!fCalCat) { isSimEvent=0; return; }
 iter=(HIterator *)fCalCat->MakeIterator();
 loc.set(0,0,0,0,0);
 
 fGeantMdcCat = (HMatrixCategory*)(gHades->getCurrentEvent()
                   ->getCategory(catMdcGeantRaw));
 if (!fGeantMdcCat){ isSimEvent=0;  return; }
 iterin = (HIterator*)fGeantMdcCat->MakeIterator("native");
 
 fGeantKineCat = (HLinearCategory*)(gHades->getCurrentEvent()
                   ->getCategory(catGeantKine));
  if (!fGeantKineCat) { isSimEvent=0; return; }
}

void HMdcSimEventInf::collectInf(void) {
 eventId++;
 if( isSimEvent == -1 ) setParContainers();
 else array->Delete();

  for(Int_t i=0; i<12; i++) nTracksSeg[i/2][i%2]=0;
  for(Int_t i=0; i<6; i++) nTotHitsSec[i]=0;
  //Collecting information about Ca1Sim
  Int_t nTr=0;
  Int_t nTime;
  HMdcCal1Sim* cal=0;
  iter->Reset();
  nTracks=0;
  xVertex=yVertex=zVertex=0.0;
  while ((cal=(HMdcCal1Sim *)iter->Next())!=0) {
   Int_t nHits=cal->getNHits();
   if(cal->getModule()<2) nTotHitsSec[cal->getSector()]++; //!!!!!!!!!!!!!!!!
   for(Int_t i=0; i<nHits; i++) {
    if(i==0) { nTr=cal->getNTrack1(); nTime=1; }
        else { nTr=cal->getNTrack2(); nTime=2; }
    if( !(*array)[nTr] ) {
     (*array)[nTr] = new HMdcSimTrackInf(nTr);
     listTracks[nTracks]=nTr;
     nTracksSeg[cal->getSector()][cal->getModule()/2]++;
     nTracks++;
    }
    (*this)[nTr].setSimHit(cal,nTime);
   }
  }
  //Collecting information about GeantMdc
 HGeantMdc* geant;
//HMatrixCatIter nextM(fGeantMdcCat);
 iterin->Reset();
// while((geant=(HGeantMdc*)nextM()) != NULL) {
 while((geant=(HGeantMdc*)iterin->Next()) != NULL) {
  Int_t nTr=(Int_t)(geant->getTrack());
  if( (*array)[nTr] ) {
   (*this)[nTr].setGeantMdc(geant);
  }
 }
  //Collecting information about GeantKine 
 HGeantKine* kine;
 HLinearCatIter nextL(fGeantKineCat);
 xVertex=yVertex=zVertex=0.0;
 while((kine=(HGeantKine*)nextL()) != NULL) {
  Int_t nTrK,id;
  kine->getParticle(nTrK,id);
  if( (*array)[nTrK] ) {
   (*this)[nTrK].setGeantKine(kine);
   Int_t parTr, med, mech;
   kine->getCreator(parTr, med, mech);
   if(parTr == 0) {
    kine->getVertex(xVertex,yVertex,zVertex);
   }
  }
 }
 for(nTr=0; nTr<nTracks; nTr++) {
  (*this)[listTracks[nTr]].sort();
//  (*this)[listTracks[nTr]].printTrack();
 }
// HGeantHeader* header;
// header=(HGeantHeader*)
// (((HRecEvent*)(gHades->getCurrentEvent()))->
// getPartialEvent(catSimul)->getSubHeader());
// if(header) Printf("-------Event %i --------",header->getEventId());

}

Int_t HMdcSimEventInf::getListTracksSeg(Int_t sec, Int_t seg, Int_t *list){
  Int_t nTrTot=0;
  Int_t mod=0;
  if(seg == 1) mod=2;
  for(Int_t i=0; i<nTracks; i++) {
   Int_t ntr=listTracks[i];
   HMdcSimTrackInf& trk=(*this)[ntr];
   if(trk.getNLayers(sec,mod) > 0 && trk.getNLayers(sec,mod+1) > 0) {
    list[nTrTot]=ntr;
    nTrTot++;
   }
  }
  return nTrTot;
}


//-------------------------------------------------------

HMdcSimTrackInf::HMdcSimTrackInf(Int_t track) {
 nTr=track;
 nSimHits=0;
 nGeantHits=0;
 kine=0;
 for(Int_t sec=0; sec<6; sec++) {
  nSimHitsSector[sec]=0;
  for(Int_t mod=0; mod<4; mod++) {
   nTimesInMod[sec][mod]=0;
   nSimHitsMod[sec][mod]=0;
   for(Int_t layer=0; layer<6; layer++) {
    nGeantHitsLayer[sec][mod][layer]=0;
    nSimHitsLayer[sec][mod][layer]=0;
   }
  }
 }
}

void HMdcSimTrackInf::setSimHit(HMdcCal1Sim* hit, Int_t nTime) {
 if(nSimHits <500) {
  sim[nSimHits]=hit;
  time[nSimHits]=nTime;
  nSimHits++;
 }
 else Printf("Very mach sim. hits (>500) for track %i.",nTr);
 
 nSimHitsSector[hit->getSector()]++;
 nSimHitsMod[hit->getSector()][hit->getModule()]++;
 nSimHitsLayer[hit->getSector()][hit->getModule()][hit->getLayer()]++;
}

void HMdcSimTrackInf::sort(void) {

 Int_t indSort=1;
 while(indSort != 0) {
  indSort=0;
  for(Int_t i2=1; i2<nSimHits; i2++) {
   Int_t i1=i2-1;
   Int_t pak1,pak2;
   pak1=sim[i1]->getSector()*10000+sim[i1]->getModule()*1000+sim[i1]->getLayer()*100+
        sim[i1]->getCell();
   pak2=sim[i2]->getSector()*10000+sim[i2]->getModule()*1000+sim[i2]->getLayer()*100+
        sim[i2]->getCell();
   if(pak1>pak2) {
    indSort++;
    HMdcCal1Sim* tmp=sim[i1];
    sim[i1]=sim[i2];
    sim[i2]=tmp;
    Short_t ntm=time[i1];
    time[i1]=time[i2];
    time[i2]=ntm;
   }
  }
 }
 nModules=0;
 moduleHits[0][0]=sim[0]->getSector();
 moduleHits[0][1]=sim[0]->getModule();
 moduleHits[0][2]=moduleHits[0][3]=0;
 for(Int_t i=1; i<nSimHits; i++) {
  if(sim[i]->getSector() != moduleHits[nModules][0] ||
     sim[i]->getModule() != moduleHits[nModules][1]) {
    nModules++;
    moduleHits[nModules][0]=sim[i]->getSector();
    moduleHits[nModules][1]=sim[i]->getModule();
    moduleHits[nModules][2]=i;  
  }
  moduleHits[nModules][3]=i;
 }
 nModules++;

 indSort=1;
 while(indSort != 0) {
  indSort=0;
  for(Int_t i2=1; i2<nGeantHits; i2++) {
   Int_t i1=i2-1;
   Float_t x,y,tof1,tof2,ptof;
   geant[i1]->getHit(x,y,tof1,ptof);
   geant[i2]->getHit(x,y,tof2,ptof);
   if(tof1>tof2) {
    indSort++;
    HGeantMdc* tmp=geant[i1];
    geant[i1]=geant[i2];
    geant[i2]=tmp;
   }
  }
 }
 nPiace=0;
 piacesTrack[0][0]=geant[0]->getSector();
 piacesTrack[0][1]=geant[0]->getModule();
 piacesTrack[0][3]=piacesTrack[0][4]=0;
 Float_t theta,phi;
 geant[0]->getIncidence(theta,phi);
 if( theta < 90 ) piacesTrack[0][2]=0;
 else piacesTrack[0][2]=1;
 Short_t direc=piacesTrack[0][2];
 
 for(Int_t i=1; i<nGeantHits; i++) {
  geant[i]->getIncidence(theta,phi);
  if( (direc%2) == 0 && theta > 90 ) {
   direc++; 
  }
  else if( (direc%2) == 1 && theta < 90 ) {
   direc++;
  }
  if( geant[i]->getSector() != piacesTrack[nPiace][0] ||
      geant[i]->getModule() != piacesTrack[nPiace][1] ||
      direc != piacesTrack[nPiace][2] ) {
   if(i<nGeantHits-1)  {
    nPiace++;
    piacesTrack[nPiace][0]=geant[i]->getSector();
    piacesTrack[nPiace][1]=geant[i]->getModule();
    piacesTrack[nPiace][2]=direc;
    piacesTrack[nPiace][3]=i;
   }
  } 
  piacesTrack[nPiace][4]=i; 
 }
 nPiace++;
 
 
 for(Int_t i=0; i<nPiace; i++) {
  nTimesInMod[piacesTrack[i][0]][piacesTrack[i][1]]++;
 }
 // Quality:
 for(Int_t sec=0; sec<6; sec++) {
  for(Int_t mod=0; mod<4; mod++) {
   quality[sec][mod]=0;
   if(getNLayers(sec, mod) >=4) {
    for(Int_t i=0; i<nPiace; i++) {
     if(piacesTrack[i][0] == sec && piacesTrack[i][1] == mod) {
      if(piacesTrack[i][4]-piacesTrack[i][3]+1 >3) quality[sec][mod]++;
     }
    }
   }
  }
 }
}

Int_t HMdcSimTrackInf::getNSec(void){
 Int_t listSec[6];
 for(Int_t sec=0; sec<6; sec++) listSec[sec]=0;
 for(Int_t i=0; i<nPiace; i++) {
   listSec[piacesTrack[i][0]]=1;
 }
 Int_t nSec=0;
 for(Int_t sec=0; sec<6; sec++) if(listSec[sec]) nSec++;
 return nSec;
}

void HMdcSimTrackInf::printTrack(void) {
 Printf("=======Geant Trek %i, %i Geant Hits, %i piaces:",
         nTr,nGeantHits,nPiace);
 for(Int_t i=0; i<nPiace; i++) {
  printf("%2i) Sec.%i Mod.%i Layers:",
  i,piacesTrack[i][0],piacesTrack[i][1]);
  for(Int_t ind=piacesTrack[i][3]; ind<=piacesTrack[i][4]; ind++) {
   printf(" %i",geant[ind]->getLayer());
  }
  printf("\n");
 }
 Printf("- - - -Digit. hits (%i hits in %i modules):",nSimHits,nModules);
 for(Int_t i=0; i<nModules; i++) {
  printf("%2i) Qual.%i S%i M%i L(C):", i,
  quality[moduleHits[i][0]][moduleHits[i][1]],
  moduleHits[i][0],moduleHits[i][1]);
  for(Int_t ind=moduleHits[i][2]; ind<=moduleHits[i][3]; ind++) {
   printf(" %i(%i)",sim[ind]->getLayer(),sim[ind]->getCell());
  }
  printf("\n");
 }
}

Short_t HMdcSimTrackInf::getNSimHits(Int_t sec=-1, Int_t mod=-1,  Int_t layer=-1) {
 if(sec==-1) return nSimHits;
 if(sec<0 || sec>5) return 0;
 if(mod==-1) return nSimHitsSector[sec];
 if(mod<0 || mod>3) return 0;
 if(layer==-1) return nSimHitsMod[sec][mod];
 if(layer<0 || layer>5) return 0;
 return nSimHitsLayer[sec][mod][layer];
}

Short_t HMdcSimTrackInf::getNLayers(Int_t sec,Int_t mod){
//Calculating num. of layers in module with hits from this track
 if(sec<0 || sec>5) return 0;
 if(mod<0 || mod>3) return 0;
 Short_t nL=0;
 for(Int_t layer=0; layer<6; layer++) if(nSimHitsLayer[sec][mod][layer]>0) nL++;
 return nL;
}


void HMdcSimTrackInf::setGeantMdc(HGeantMdc* hit) {
 if(nGeantHits <500) {
  geant[nGeantHits]=hit;
  nGeantHits++;
 }
 else Printf("Very mach geant hits (>500) for track %i.",nTr);

 Char_t sec=hit->getSector();
 Char_t mod=hit->getModule();
 Char_t layer=hit->getLayer();
 
 nGeantHitsLayer[sec][mod][layer]++;
}

Short_t HMdcSimTrackInf::getNTimes(Int_t sec, Int_t mod,  Int_t layer=-1) {

 if(sec<0 || sec>5) return 0;
 if(mod<0 || mod>3) return 0;
 
 if(layer==-1) return nTimesInMod[sec][mod];
 if(layer<0 || layer>5) return 0;
 return nGeantHitsLayer[sec][mod][layer];
}

void HMdcSimTrackInf::setGeantKine(HGeantKine* hit) {
  kine=hit;
}

HGeantMdc* HMdcSimTrackInf::getGeantHit(Int_t sec,Int_t mod,Int_t layer, 
                                        Int_t ntime) {
 Int_t ntm=0;
 for(Int_t i=0; i<nPiace; i++) {
  if(piacesTrack[i][0] != sec) continue;
  if(piacesTrack[i][1] != mod) continue;
  ntm++;
  if(ntime != ntm) continue;
  for(Int_t j=piacesTrack[i][3]; j<=piacesTrack[i][4]; j++) {
   if(geant[j]->getLayer() != layer) continue;
   return geant[j];
  }
 } 
 return 0;
}
