#include "hmdctrackfinder.h"
#include "hmdcdef.h"
#include "hdebug.h"
#include "hades.h"
#include "hmdcxy.h"
#include "hmdccal1.h"
#include "hiterator.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hevent.h"
#include "hcategory.h"
#include "hlocation.h"
//#include <iostream.h>
//#include "TStopwatch.h"
//#include "TRandom.h"
#include "hmdcsizescells.h"
#include "hmdc12cellprojec.h"

//*-- Author : V.Pechenov
//*-- Modified : 07.03.2000 by R. Holzmann
//*-- Modified : 02.12.99 by V. Pechenov
//*-- Modified : 14.04.99

HMdcTrackFinder::HMdcTrackFinder(Int_t mbv=12) {
  fCalCat=0;
  fLookUpTb=0;
  iter=0; 
  minbin=mbv;
  cutLeft=cutRight=0.;   
  level4=10;
  level5=50;
}

HMdcTrackFinder::HMdcTrackFinder(Text_t *name,Text_t *title, 
                  Int_t mbv=12) : HReconstructor(name,title) {

  fCalCat=0;
  fLookUpTb=0;
  iter=0;    
  minbin=mbv;
  if(minbin< 4) minbin=4;
  if(minbin>12) minbin=12;
  cutLeft=cutRight=0.;
  level4=10;
  level5=50;
}

void HMdcTrackFinder::setParContainers() {
  fLookUpTb=(HMdcLookUpTb*)gHades->getRuntimeDb()->getContainer("MdcLookUpTb");
  if(!fLookUpTb) {
   fLookUpTb = new HMdcLookUpTb(319); //!!!!!!!!!!!!319!!!!!!!!!
   gHades->getRuntimeDb()->addContainer(fLookUpTb); 
   printf("HMdcLookUpTb added\n");
  }
}

Bool_t HMdcTrackFinder::init(void) {
  setParContainers();
  fCalCat = gHades->getCurrentEvent()->getCategory(catMdcCal1);
  if (!fCalCat) return kFALSE;
  iter = (HIterator *)fCalCat->MakeIterator("native");

  fHitCat = gHades->getCurrentEvent()->getCategory(catMdcHit);
  if (!fHitCat) {
     fHitCat = gHades->getSetup()->getDetector("Mdc")
                                 ->buildCategory(catMdcHit);
     if (!fHitCat) return kFALSE;
     else gHades->getCurrentEvent()->addCategory(catMdcHit,fHitCat,"Mdc");
  }

  return kTRUE;
}


Int_t HMdcTrackFinder::execute(void) {

//TStopwatch timer;
//timer.Start();
  Int_t fnumOfHits[6];
  Int_t s,m,l,c;
  for(Int_t i=0; i<6; i++) fnumOfHits[i]=0; 
  iter->Reset();
  HMdcCal1* cal=0;
  (*fLookUpTb).clearwk();
  while ((cal=(HMdcCal1 *)iter->Next())!=0) {
    cal->getAddress(s,m,l,c);
    if(m<2) {
//      cal->getAddress(s,m,l,c);
// ==Cut.  Beam-time !!!!!!!!!!!!!!!!! 
      if( cutLeft < cutRight ) {   
        if(cal->getNHits() == 2) {
          if(cal->getTime1()-cal->getTime2() < cutLeft ||
            cal->getTime1()-cal->getTime2() > cutRight) continue; 
        } else continue;
      }
     
      HMdcLookUpTbSec& lookutsec = (*fLookUpTb)[s]; 
      if( &lookutsec ) { 
//      lookutsec.setCell(m,l,c,cal->getNHits());
        lookutsec.setCell(m,l,c,1); // For beam time 09.99 only !!!
        fnumOfHits[s]+=1;
      }
    }
  }
   
//timer.Stop();  
//printf(" Cpu time for setCell=%f\n",timer.CpuTime());
//timer.Start();


  Int_t numcl=0;
  Int_t sumfnum=0;
  for(Int_t ns=0; ns<6; ns++) {
    if(&(*fLookUpTb)[ns]==0) continue;
//  fnumOfHits[ns]=fnumOfHits[ns]/12; //!!!!!!!!!!!!!
    sumfnum+=fnumOfHits[ns];
//+++++++++++in++++++++++++++++++++++++++++++
//      if(fnumOfHits[ns]<3.) minbin=8;
//      else if(fnumOfHits[ns]<6.) minbin=9;
//      else if(fnumOfHits[ns]<10.) minbin=10;
//      else if(fnumOfHits[ns]<15.) minbin=11;
//      else minbin=12;
    minbin=6;
    if(fnumOfHits[ns]<=level4) minbin=4;
    else if(fnumOfHits[ns]<=level5) minbin=5;

//+++++++++++++++++++++++++++++++++++++++++++
    numcl+=(*fLookUpTb)[ns].findClusters(minbin);
  } 
 
  HLocation loc;   // dummy for getNewSlot()
  loc.set(0);
  for(Int_t ns=0; ns<6; ns++) {
    if(&(*fLookUpTb)[ns]==0) continue;
    HMdc12Clusters& clusters = (*fLookUpTb)[ns].getClusters();
    Int_t nclst = clusters.getNClusters();
    for(Int_t nc=0; nc<nclst; nc++) {
      HMdcCluster& clst = clusters[nc];
      HMdcXY *hit = (HMdcXY*)fHitCat->getNewSlot(loc);
      if(!hit) continue;
      hit = new(hit) HMdcXY;
      hit->setSector(ns);
      hit->setModule(1);    // November 99 beam time (only 1 Dubna chamber in)
      hit->setX(clst.getXmid());
      hit->setY(clst.getYmid());
      hit->setZ(clst.getZmid());
      hit->setNBins(clst.getNBins());
    }
  }


//timer.Stop();  
//printf(" Cpu time for track finder=%f\n",timer.CpuTime());

  
//printf("*************Total %i clusters found:\n",numcl);
//  (*fLookUpTb).printClusters();

  return 0;
}

ClassImp(HMdcTrackFinder)










