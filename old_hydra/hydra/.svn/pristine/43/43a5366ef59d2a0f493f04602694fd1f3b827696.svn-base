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
#include "hmdcgetcontainers.h"
//#include "TStopwatch.h"

#include "hmdc34clfinder.h"   //---???---

//*-- Author : V.Pechenov
//*-- Modified : 14.08.2000 by V.Pechenov
//*-- Modified : 22.05.2000 by V.Pechenov
//*-- Modified : 02.12.99 by V.Pechenov
//*-- Modified : 14.04.99


HMdcTrackFinder::HMdcTrackFinder(Bool_t per, Bool_t isCOff, Int_t typeClFn) { 
  level4=10;
  level5=50;
  perMdcClus=per;
  isCoilOff=isCOff;
  typeClFinder=typeClFn;
  setParContainers();
}

HMdcTrackFinder::HMdcTrackFinder(Text_t *name, Text_t *title, Bool_t per,
     Bool_t isCOff, Int_t typeClFn) : HReconstructor(name,title) {   
  level4=10;
  level5=50;
  perMdcClus=per;
  isCoilOff=isCOff;
  typeClFinder=typeClFn;
  setParContainers();
}

void HMdcTrackFinder::setParContainers() {
  fLookUpTb=HMdcLookUpTb::getExObject();
  if(!fLookUpTb) fLookUpTb=new HMdcLookUpTb(319,isCoilOff); //!!!!!!!!!!!!319!
  if( typeClFinder!=0 ) {
    for(Int_t sec=0; sec<6; sec++) {
      if(&((*fLookUpTb)[sec])) (*fLookUpTb)[sec].setTypeClFinder(typeClFinder);
    }
  }
  fcut=HMdcTimeCut::getExObject();
  if( !fcut ) Printf("Cut for drift time not seted!");
}

Bool_t HMdcTrackFinder::init(void) {
  fCalCat=HMdcGetContainers::getCatMdcCal1();
  if( !fCalCat ) return kFALSE;
  isGeant=HMdcGetContainers::isGeant();  
  iter=(HIterator *)fCalCat->MakeIterator("native");
    
  fClusCat=HMdcGetContainers::getCatMdcClus(kTRUE);
  if( !fClusCat ) return kFALSE;
  fClusCat->setPersistency(perMdcClus);  // perenesti v HMdcTrackFinder !!!
  
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
  
//Int_t nTr[6];       //----???---
//for(Int_t ns=0;ns<6;ns++) nTr[ns]=-1; //----???---
  
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
//???????      if(((HMdcCal1Sim *)cal)->getStatus()==-1) continue;
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
    locCl.set(3,ns,0,0);
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
    fillCatMdcClus(ns); //??! vremennoe reshenie 
  }
   
//timer.Stop();  
//printf(" Cpu time for track finder mdc1&2 =%g\n",timer.CpuTime());

  
//printf("*************Total %i clasters found:\n",numcl);
//  (*fLookUpTb).printClusters();

//HMdc34ClFinder* fmdc34cf=HMdc34ClFinder::getExObject();   //----???---
//if(fmdc34cf) for(Int_t ns=0;ns<6;ns++) (*fmdc34cf)[ns].setNTrack(nTr[ns]);//----???---

  return 0;
} 

void HMdcTrackFinder::fillCatMdcClus(Int_t sec) {
  HMdcLookUpTbSec &flookup=(*fLookUpTb)[sec];
  HMdc12Clusters& fclus=flookup.getClusters();
  const HGeomVector& tarF=flookup.getTargetF();
  const HGeomVector& tarL=flookup.getTargetL();
  HGeomVector tar(tarF+tarL);
  tar*=0.5;
  HGeomVector errTar(tarF-tarL);
  errTar*=0.5;
  errTar.abs();
  const HGeomTransform& prPlane=flookup.getParPlane();

  Int_t nClus=fclus.getNClusters();
  Int_t nClSeg[2];
  nClSeg[0]=nClSeg[1]=0;
  locCl[0]=sec;
  for(Int_t ncl=0; ncl<nClus; ncl++) {
    HMdcCluster& clus=fclus[ncl];
    Int_t seg1=(clus.getMaximum(0)+clus.getMaximum(1) > 0) ? 0:1;
    Int_t seg2=(clus.getMaximum(2)+clus.getMaximum(3) > 0) ? 1:0;
    for(Int_t seg=seg1; seg<=seg2; seg++) {
      locCl[1]=seg;
      locCl[2]=nClSeg[seg]++;
      HMdcClus* clusNew = (HMdcClus*)fClusCat->getSlot(locCl);
      if(!clusNew) {
          Warning("fillCatMdcClus","S.%i No slot HMdcClus available",sec+1);
          continue;  // continue(not break) -> HMdcBArray must be cleaned !
      }
      if(isGeant) clusNew = (HMdcClus *) (new(clusNew) HMdcClusSim);
      else  clusNew = new(clusNew) HMdcClus;
      clusNew->setAddress(locCl);
      if(seg1 != seg2) {
        if(seg == seg1) clusNew->setIndexChilds(nClSeg[seg2],nClSeg[seg2]);
        else clusNew->setIndexParent(nClSeg[seg1]-1);
      }
      if(clus.getMod() >= 0) {
        clusNew->setTypeClFinder(1);
        clusNew->setMod(clus.getMod());
      }
      clusNew->setXY(clus.getX(),clus.getErrX(),clus.getY(),clus.getErrY());
      clusNew->setSumWt(clus.getBinsWt());
      clusNew->setNBins(clus.getNBinsCl());
      clusNew->setMinCl(clus.getMaximum(seg*2),clus.getMaximum(seg*2+1));
      for(Int_t lay=0; lay<12; lay++) {
        Int_t lOC=lay+seg*12;
        Int_t nHits=clus.getNumHits(lOC);
         for(Int_t nh=0; nh<nHits; nh++)
           clusNew->setTime(lay,clus.getHit(lOC,nh),clus.getTimes(lOC,nh)*2-1);
      }
      clusNew->setPrPlane(prPlane);   //??! luchshe brat' Ap,Bp...
      clusNew->setTarg(tar);
      clusNew->setErrTarg(errTar);
    }
  }
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
