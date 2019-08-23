#include "hmdctrackfinder.h"
#include "hmdcdef.h"
#include "hdebug.h"
#include "hades.h"
#include "hmdcxy.h"
#include "hiterator.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hevent.h"
#include "hcategory.h"
//#include "TStopwatch.h"
#include "TRandom.h"
#include "hmdcsizescells.h"
#include "hmdc12cellprojec.h"
#include "hgeomcompositevolume.h" 

//*-- Author : V.Pechenov
//*-- Modified : 22.05.2000 by V.Pechenov
//*-- Modified : 02.12.99 by V.Pechenov
//*-- Modified : 14.04.99


HMdcTrackFinder::HMdcTrackFinder(Int_t mbv=12) {
  fCalCat=0;
  fLookUpTb=0;
  iter=0; 
  minbin=mbv;
  level4=10;
  level5=50;
  setParContainers();
}

HMdcTrackFinder::HMdcTrackFinder(Text_t *name,Text_t *title, 
                  Int_t mbv=12) : HReconstructor(name,title) {

  fCalCat=0;
  fLookUpTb=0;
  iter=0;    
  minbin=mbv;
  if(minbin< 4) minbin=4;
  if(minbin>12) minbin=12;
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
  
  fHitCat = gHades->getCurrentEvent()->getCategory(catMdcHit);
  if (!fHitCat) {
     fHitCat = gHades->getSetup()->getDetector("Mdc")
                                 ->buildCategory(catMdcHit);
     if (!fHitCat) return kFALSE;
     else gHades->getCurrentEvent()->addCategory(catMdcHit,fHitCat,"Mdc");
  }
  
  // For filling HMdcXY !
  fGeomPar=(HMdcGeomPar*)gHades->getRuntimeDb()->getContainer("MdcGeomPar");
  for(Int_t ns=0; ns<6; ns++) {
    HMdcLookUpTbSec* plSec=&(*fLookUpTb)[ns];
    if( !plSec ) continue;
    targetM[ns]=plSec->getTargetF()+plSec->getTargetL();
    targetM[ns]/=2.;
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
    if( m > 1 ) continue;
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
    if( m > 1 ) continue;
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
    if(&(*fLookUpTb)[ns]==0) continue;
 //  numHits[ns]=numHits[ns]/12.; //!!!!!!!!!!!!!
 //+++++++++++in++++++++++++++++++++++++++++++
 /*      if(numHits[ns]<3.) minbin=8;
       else if(numHits[ns]<6.) minbin=9;
       else if(numHits[ns]<10.) minbin=10;
       else if(numHits[ns]<15.) minbin=11;
       else minbin=12; */
       minbin=6;
       Int_t nMod=1;
       if(numHits[0][ns]>0 && numHits[1][ns]>0) nMod=2;
       minbin=6*nMod;
       Int_t nHits=(numHits[0][ns]+numHits[1][ns])/nMod;

       if(nHits<=level4) minbin-=2*nMod;
       else if(nHits<=level5) minbin-=nMod;

 //+++++++++++++++++++++++++++++++++++++++++++
    numcl+=(*fLookUpTb)[ns].findClusters(minbin);
  } 
 
 
  fillHitCont();

   
//timer.Stop();  
//printf(" Cpu time for track finder=%f\n",timer.CpuTime());

  
//printf("*************Total %i clasters found:\n",numcl);
//  (*fLookUpTb).printClusters();

  return 0;
}

void HMdcTrackFinder::fillHitCont(void) {
  HLocation loc;   // dummy for getNewSlot()
  loc.set(0);
  HMdcDetector* fmdc=(HMdcDetector*)gHades->getSetup()->getDetector("Mdc");
  HGeomVector cross;
  if( !fmdc ) return;
  for(Int_t ns=0; ns<6; ns++) {
    HMdcLookUpTbSec* plSec=&(*fLookUpTb)[ns];
    if( !plSec ) continue;
    HMdc12Clusters& clusters = (*fLookUpTb)[ns].getClusters();
    Int_t nclst = clusters.getNClusters();
    for(Int_t nc=0; nc<nclst; nc++) {
      HMdcCluster& clst = clusters[nc];
      for(Int_t mod=0; mod<2; mod++) {
        if(!fmdc->getModule(ns,mod)) continue; 
        HGeomVector vClust(clst.getX(),clst.getY(),clst.getZ());
        if( !calcCross(&(targetM[ns]),&vClust,mod,&cross) ) continue;
        HMdcXY *hit = (HMdcXY*)fHitCat->getNewSlot(loc);
        if(!hit) continue;
        hit = new(hit) HMdcXY;
        hit->setSector(ns);
        hit->setModule(mod);
        hit->setX(cross.getX());
        hit->setY(cross.getY());
        hit->setZ(cross.getZ());
        HGeomVector dir=vClust-targetM[ns];
        hit->setXdir(dir.getX()/dir.length());
        hit->setYdir(dir.getY()/dir.length());
        //Calculating of errors
//        HGeomVector vCluSh;
        HGeomVector crossSh;
        HGeomVector vCluSh(clst.getX()+clst.getErrX(),
                         clst.getY()+clst.getErrY(),
                         clst.getZ()-clst.getErrZ());
        calcCross(&plSec->getTargetF(),&vCluSh,mod,&crossSh);
        Float_t errX=TMath::Abs(crossSh.getX()-cross.getX());
        Float_t errY=TMath::Abs(crossSh.getY()-cross.getY());
        Float_t errXdir=TMath::Abs(crossSh.getX()/crossSh.length()-hit->getXdir());
        Float_t errYdir=TMath::Abs(crossSh.getY()/crossSh.length()-hit->getYdir());
        calcCross(&plSec->getTargetL(),&vCluSh,mod,&crossSh);
        errX=TMath::Max(errX,TMath::Abs(crossSh.getX()-cross.getX()));
        errY=TMath::Max(errY,TMath::Abs(crossSh.getY()-cross.getY()));
        errXdir=TMath::Max(
            errXdir,TMath::Abs(crossSh.getX()/crossSh.length()-hit->getXdir()));
        errYdir=TMath::Max(
            errYdir,TMath::Abs(crossSh.getY()/crossSh.length()-hit->getYdir()));
        hit->setErrX(errX);
        hit->setErrY(errY);
        hit->setErrXdir(errXdir);
        hit->setErrYdir(errYdir);
        hit->setNClust(nc);
      }
    }
  }  
}


Int_t HMdcTrackFinder::calcCross( const HGeomVector *p1, const HGeomVector *p2,
                                  Int_t mod, HGeomVector *cross) {
// Calculeting a cross of line (point1-point2) with layer
  HGeomCompositeVolume* pComVol=fGeomPar->getRefVolume(mod);
  if(!pComVol) return 0; 
  const HGeomVector* vMdc=&((pComVol->getTransform()).getTransVector());
  if(!vMdc) return 0;
  Double_t x1=p1->getX();   Double_t x2=p2->getX();
  Double_t y1=p1->getY();   Double_t y2=p2->getY();
  Double_t z1=p1->getZ();   Double_t z2=p2->getZ();
  Double_t A=vMdc->getX();
  Double_t B=vMdc->getY();
  Double_t C=vMdc->getZ();
  Double_t D=-(A*A+B*B+C*C);
  Double_t point[3];
  point[0]=(x1*(B*(y1-y2)+C*(z1-z2))-(B*y1+C*z1+D)*(x1-x2))/
           (A*(x1-x2)+B*(y1-y2)+C*(z1-z2));
  point[1]=(point[0]-x1)*(y1-y2)/(x1-x2)+y1;
  point[2]=(-A*point[0]-B*point[1]-D)/C;
  cross->setVector(point);
  return 1;
} 

ClassImp(HMdcTrackFinder)
