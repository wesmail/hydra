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


HMdcTrackFinder::HMdcTrackFinder() {
  fCalCat=0;
  fLookUpTb=0;
  iter=0; 
  level4=10;
  level5=50;
  setParContainers();
}

HMdcTrackFinder::HMdcTrackFinder(Text_t *name,Text_t *title) 
                  : HReconstructor(name,title) {

  fCalCat=0;
  fLookUpTb=0;
  iter=0;    
  level4=10;
  level5=50;
  setParContainers();
}

void HMdcTrackFinder::setParContainers() {
  fLookUpTb=(HMdcLookUpTb*)gHades->getRuntimeDb()->getContainer("MdcLookUpTb");
  if(!fLookUpTb) {
   fLookUpTb= new HMdcLookUpTb(319); //!!!!!!!!!!!!319!!!!!!!!!
   gHades->getRuntimeDb()->addContainer(fLookUpTb);
   printf("HMdcLookUpTb added\n"); 
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
    nModules[sec] = ( fmdc->getModule(sec,0) ) ? 1 : 0;
    nModules[sec] += ( fmdc->getModule(sec,1) ) ? 2 : 0;
  }
  
  return kTRUE;
}


Int_t HMdcTrackFinder::execute(void) {

//TStopwatch timer;
//timer.Start();
  Int_t numHits[2][6];
  Int_t s,m,l,c;
  for(Int_t i=0; i<6; i++) numHits[0][i]=numHits[1][i]=0; 
  iter->Reset();
  
  (*fLookUpTb).clearwk();
  
  // Geant data ===========  
  if(isGeant) {
   HMdcCal1Sim* cal=0;
   while ((cal=(HMdcCal1Sim *)iter->Next())!=0) {
    if(cal->getStatus()==-1) continue;
    cal->getAddress(s,m,l,c);
    if( m > 1 || fmdc->getModule(s,m) == 0 ) continue;
    HMdcLookUpTbSec& lookutsec = (*fLookUpTb)[s]; 
    if( &lookutsec ) { 
     lookutsec.setCell(m,l,c,cal->getNHits());
     numHits[m][s]++;
    }
   }
  }
  // Real data ===========
  else {
   HMdcCal1* cal=0;
   while ((cal=(HMdcCal1 *)iter->Next())!=0) {
    cal->getAddress(s,m,l,c);
    if( m > 1 || fmdc->getModule(s,m) == 0 ) continue;
//==Cut.  Beam-time !!!!!!!!!!!!!!!!! 
    if(fcut && fcut->cut(cal)) continue;
    HMdcLookUpTbSec& lookutsec = (*fLookUpTb)[s]; 
    if( &lookutsec ) { 
//    lookutsec.setCell(m,l,c,cal->getNHits());
     lookutsec.setCell(m,l,c,1); // For beam time 09.99 only !!!
     numHits[m][s]++;
    }
   }
  }
   
//timer.Stop();  
//printf(" Cpu time for setCell=%f\n",timer.CpuTime());
//timer.Start();

  Int_t numcl=0;
  for(Int_t ns=0; ns<6; ns++) {
    if( &(*fLookUpTb)[ns]==0 || nModules[ns]==0 ) continue;
      Int_t minbin= (nModules[ns] == 3) ? 12 : 6;
      if( nModules[ns]&1 ) {
        if( numHits[0][ns] <= level5 ) minbin--;
        if( numHits[0][ns] <= level4 ) minbin--;
      }
      if( nModules[ns]&2 ) {
        if( numHits[1][ns] <= level5 ) minbin--;
        if( numHits[1][ns] <= level4 ) minbin--;
      }
    numcl+=(*fLookUpTb)[ns].findClusters(minbin);
  } 
   
//timer.Stop();  
//printf(" Cpu time for track finder=%f\n",timer.CpuTime());

  
//printf("*************Total %i clasters found:\n",numcl);
//  (*fLookUpTb).printClusters();

  return 0;
} 

ClassImp(HMdcTrackFinder)
