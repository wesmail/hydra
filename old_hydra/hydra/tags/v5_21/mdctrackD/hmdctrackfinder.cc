#include "hmdctrackfinder.h"
#include "hmdcdef.h"
//#include "hdebug.h"
#include "hades.h"
#include "hiterator.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hevent.h"
#include "hcategory.h"
//#include "TStopwatch.h"

//*-- Author : V.Pechenov
//*-- Modified : 14.08.2000 by V.Pechenov
//*-- Modified : 22.05.2000 by V.Pechenov
//*-- Modified : 02.12.99 by V.Pechenov
//*-- Modified : 14.04.99


HMdcTrackFinder::HMdcTrackFinder(Bool_t isCOff=kFALSE, Int_t typeClFn=0) {
  fCalCat=0;
  fLookUpTb=0;
  iter=0; 
  level4=10;
  level5=50;
  isCoilOff=isCOff;
  typeClFinder=typeClFn;
  setParContainers(isCoilOff);
}

HMdcTrackFinder::HMdcTrackFinder(Text_t *name,Text_t *title, 
    Bool_t isCOff=kFALSE, Int_t typeClFn=0) : HReconstructor(name,title) {

  fCalCat=0;
  fLookUpTb=0;
  iter=0;    
  level4=10;
  level5=50;
  isCoilOff=isCOff;
  typeClFinder=typeClFn;
  setParContainers(isCoilOff);
}

void HMdcTrackFinder::setParContainers(Bool_t isCoilOff) {
  fLookUpTb=(HMdcLookUpTb*)gHades->getRuntimeDb()->getContainer("MdcLookUpTb");
  if(!fLookUpTb) {
   fLookUpTb= new HMdcLookUpTb(319,isCoilOff); //!!!!!!!!!!!!319!!!!!!!!!
   gHades->getRuntimeDb()->addContainer(fLookUpTb);
   printf("HMdcLookUpTb added\n"); 
  }
  if( typeClFinder!=0 ) {
    for(Int_t sec=0; sec<6; sec++) {
      if(&((*fLookUpTb)[sec])) (*fLookUpTb)[sec].setTypeClFinder(typeClFinder);
    }
  }
  fcut=(HMdcTimeCut*)gHades->getRuntimeDb()->getContainer("MdcTimeCut");
  if( !fcut ) Printf("Cut for drift time is absent!");
}

Bool_t HMdcTrackFinder::init(void) {
  fCalCat=gHades->getCurrentEvent()->getCategory(catMdcCal1);
  if (!fCalCat) return kFALSE;
  isGeant=(fCalCat->getClass()!=HMdcCal1Sim::Class()) ? kFALSE : kTRUE;
  iter=(HIterator *)fCalCat->MakeIterator("native");
  
  fmdc=(HMdcDetector*)gHades->getSetup()->getDetector("Mdc");
  if( !fmdc ) return kFALSE;
  for(Int_t sec=0; sec<6; sec++) {
    nModules[sec]=0;
    for(Int_t mod=0; mod<4; mod++) {
      listModules[sec][mod]=0;
      if( !isCoilOff && mod>1 ) break;
      if( !fmdc->getModule(sec,mod) ) continue;
      nModules[sec]++;
      listModules[sec][mod]=6;
    }
  }
  
  return kTRUE;
}


Int_t HMdcTrackFinder::execute(void) {

//TStopwatch timer;
//timer.Start();
  Int_t numHits[6][4];
  Int_t s,m,l,c;
  for(Int_t i=0; i<6; i++) numHits[i][0]=numHits[i][1]=
                           numHits[i][2]=numHits[i][3]=0; 
  iter->Reset();
  
  (*fLookUpTb).clearwk();
    
  HMdcCal1* cal=0;
  while ((cal=(HMdcCal1 *)iter->Next())!=0) {
    cal->getAddress(s,m,l,c);
    if( (!isCoilOff && m > 1) || fmdc->getModule(s,m) == 0 ) continue;
    HMdcLookUpTbSec& lookutsec = (*fLookUpTb)[s]; 
    if( !(&lookutsec) ) continue; 
    if(isGeant) {
      // Geant data ================================
      if(((HMdcCal1Sim *)cal)->getStatus()==-1) continue;
      lookutsec.setCell(m,l,c,cal->getNHits());
    } else {
      // Real data =================================
      if(fcut && fcut->cut(cal)) continue; //  Cut.  Beam-time !!!                  
      //(*fClFinder)[s][m][l].setCell(c,cal->getNHits());                          
      lookutsec.setCell(m,l,c,1);
    }
    numHits[s][m]++;
  }
     
//timer.Stop();  
//printf(" Cpu time for setCell=%f\n",timer.CpuTime());
//timer.Start();

  Int_t numcl=0;
  Int_t minbin[4];
  for(Int_t ns=0; ns<6; ns++) {
    if( &(*fLookUpTb)[ns]==0 || nModules[ns]==0 ) continue;
    for(Int_t mod=0; mod<4; mod++) { 
      minbin[mod]=listModules[ns][mod];
      if( minbin[mod]>0 ) {
        Int_t occup=(numHits[ns][mod]*6)/minbin[mod];
        if( occup <= level5 ) minbin[mod]--;
        if( occup <= level4 ) minbin[mod]--;
        if( occup < 3 ) minbin[mod]=3;
      }
    }
    numcl+=(*fLookUpTb)[ns].findClusters(minbin);
  } 
   
//timer.Stop();  
//printf(" Cpu time for track finder=%f\n",timer.CpuTime());

  
//printf("*************Total %i clasters found:\n",numcl);
//  (*fLookUpTb).printClusters();

  return 0;
}

void HMdcTrackFinder::setNLayers(Int_t *lst) {
  for(Int_t sec=0; sec<6; sec++) {
    HMdcLookUpTbSec& fLookUpTbSec=(*fLookUpTb)[sec];
    if( !&fLookUpTbSec ) continue;
    for(Int_t mod=0; mod<4; mod++) {
      HMdcLookUpTbMod& fLookUpTbMod=fLookUpTbSec[mod];
      if( !&fLookUpTbMod ) continue;
      fLookUpTbMod.setNLayers(lst[sec*4+mod]);
      if(listModules[sec][mod]>0) listModules[sec][mod]=lst[sec*4+mod];
    }
  }
}

ClassImp(HMdcTrackFinder)
