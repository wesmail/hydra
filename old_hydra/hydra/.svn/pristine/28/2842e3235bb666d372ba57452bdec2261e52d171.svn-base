//*-- Author : V.Pechenov
//*-- Modified : 12.05.2001 by V.Pechenov

//////////////////////////////////////////////////////////////////////
// HMdcTrackFinder
//
// The program for candidates to tracks finding.
//
// Using:
// Put in task list task:
// HMdcTrackFinder* trackFinder;
// ...
// tasks->connect((trackFinder=new HMdcTrackFinder("MdcTrackD","MdcTrackD",          
//                                     isCoilOff,typeClustFinder)),"...");
//
// isCoilOff=kTRUE(kFULSE) - The current in coil is turn off(on) for this run(s).
// typeClustFinder=1(0) - type of the work of cluster finder
//
// The parameters isCoilOff and typeClustFinder define the type
// of the work of cluster finder (default values of isCoilOff 
// and typeClustFinder are kFALSE and 0):
//
//    isCoilOff=kTRUE - the tracks in all MDC will finded to respect of target.
//    isCoilOff=kFALSE - the tracks in MDC1 & MDC2 will finded to respect 
//      of target. In MDC3 & MDC4 - to respect of the the point on kick plane.
//
//    typeClustFinder=0 - the tracks will finded in segment or sector
//      (needs in a alignment of MDC's).
//  
// 1. isCoilOff=kTRUE, typeClustFinder=1:
//      The tracks will be searched in each MDC (1-4) independently
// 2. isCoilOff=kTRUE, typeClustFinder=0:
//      The tracks will be searched in all MDC of sector together to
//    respect of target (needs an alignment of MDC's).
// 3. isCoilOff=kFALSE, typeClustFinder=1:
//      The tracks will be searched in MDC1, MDC2 (segment 1) independently,
//      in MDC3, MDC4 (segment 2) together.
// 4. isCoilOff=kFALSE, typeClustFinder=0:
//      The tracks will be searched in MDC1, MDC2 (segment 1) together
//      (needs an alignment of MDC's).
// For isCoilOff=kFALSE the program needs HKickPlane parameters
//
// The member function
// trackFinder->setNLayers(nLayers);
// set the number of woring planes in MDC's
// Int_t nLayers[6][4]={ {3,6,6,6}, {6,6,6,6}, {6,6,6,6},                             
//                       {4,6,6,6}, {6,6,6,6}, {5,6,6,6} }; 
//
//
// The output of the program is container HMdcClus.
// For writing of HMdcClus to ROOT file use function:
// trackFinder->setPersitency(kTRUE); 
//                           (put this line in macros befor makeTree()!)
//
// HMdcClusterToHit task copy the HMdcClus data to HMdcHit and HMdcSeg
// Attention! In the case isCoilOff=kFALSE the HMdcClusterToHit doesn't
// copy tracks in MDC3&4 (it will fixed soon).
//
// The HMdcHit and HMdcSeg containers filled by HMdc12Fit task.
// The new ver. of HMdc12Fit will be commited to CVS soon.
//
//////////////////////////////////////////////////////////////////////

#include "hmdctrackfinder.h"
#include "hmdctrackddef.h"
//#include "hdebug.h"
#include "hiterator.h"
#include "hcategory.h"
#include "hmdcgetcontainers.h"
#include "hmdcclussim.h"
#include "hmdccal1sim.h"
#include "hmdctimecut.h"
#include "hmdclookuptb.h"
#include "hmdc34clfinder.h"
//#include "TStopwatch.h"

HMdcTrackFinder::HMdcTrackFinder(Bool_t isCOff, Int_t typeClFn) {
  isCoilOff=isCOff;
  typeClFinder=typeClFn;
  setParContainers();
}

HMdcTrackFinder::HMdcTrackFinder(Text_t *name, Text_t *title,
     Bool_t isCOff, Int_t typeClFn) : HReconstructor(name,title) {
  isCoilOff=isCOff;
  typeClFinder=typeClFn;
  setParContainers();
}

HMdcTrackFinder::~HMdcTrackFinder() {
  HMdcLookUpTb::deleteCont();
  HMdc34ClFinder::deleteCont();
}

void HMdcTrackFinder::setParContainers() {
  isInitialised=kFALSE;
  fLookUpTb=0;
  f34ClFinder=0;
  level4[0]=10;
  level5[0]=50;
  level4[1]=10;
  level5[1]=30;
  locClus.set(2,0,0);
  locNewCl.set(3,0,0,0);
  fClusCat=0;
  perMdcClus=kFALSE;
  for(Int_t sec=0; sec<6; sec++) {
    nModules[sec]=0;
    for(Int_t mod=0; mod<4; mod++) listModules[sec][mod]=6;
  }
}

Bool_t HMdcTrackFinder::init(void) {
  fcut=HMdcTimeCut::getExObject();
  if(fcut && fcut->getIsUsed()) fcut=0; 
  fCalCat=HMdcGetContainers::getCatMdcCal1();
  if( !fCalCat ) return kFALSE;
  isGeant=HMdcGetContainers::isGeant();  
  iter=(HIterator *)fCalCat->MakeIterator("native");
    
  fClusCat=HMdcGetContainers::getCatMdcClus(kTRUE);
  if( !fClusCat ) return kFALSE;
  fClusCat->setPersistency(perMdcClus);
  iterClus=(HIterator *)fClusCat->MakeIterator(); 
  
  fLookUpTb=HMdcLookUpTb::getObject();
  fLookUpTb->setPar(319,isCoilOff); //!!!!!!!!!!!!319!
  
  if( !isCoilOff ) f34ClFinder=HMdc34ClFinder::getObject();
  else f34ClFinder=0;
  return kTRUE;
}

Bool_t HMdcTrackFinder::reinit(void) {
  if(!fLookUpTb->initContainer()) return kFALSE;
  if( f34ClFinder && !f34ClFinder->initContainer() ) return kFALSE;
  if( !isInitialised ) {
    if(typeClFinder==1) {
      for(Int_t sec=0; sec<6; sec++)
        if(&((*fLookUpTb)[sec])) (*fLookUpTb)[sec].setTypeClFinder(typeClFinder);
    }
    Int_t nModsF=(isCoilOff) ? 4:2;
    for(Int_t sec=0; sec<6; sec++) {
      nModules[sec]=0;
      HMdcLookUpTbSec* fLookUpTbSec=&((*fLookUpTb)[sec]);
      for(Int_t mod=0; mod<nModsF; mod++) {
        HMdcLookUpTbMod* fLookUpTbMod=(fLookUpTbSec) ? 
            &((*fLookUpTbSec)[mod]):0;
        if( fLookUpTbMod ) {
          nModules[sec]++;
          if(listModules[sec][mod] != 6) 
              fLookUpTbMod->setNLayers(listModules[sec][mod]);
        } else listModules[sec][mod]=0;
      }
      HMdc34ClFinderSec* f34ClFinderSec=(f34ClFinder)?&((*f34ClFinder)[sec]):0;
      for(Int_t mod=nModsF; mod<4; mod++) {
        HMdc34ClFinderMod* f34ClFinderMod=(f34ClFinderSec) ? 
            &((*f34ClFinderSec)[mod]) : 0;
        if( f34ClFinderMod ) nModules[sec]++;
        else listModules[sec][mod]=0;
      }
    }
    isInitialised=kTRUE;
  }
  return kTRUE;
}

void HMdcTrackFinder::setPersistency(Bool_t perCl) {
  perMdcClus=perCl;
  if(fClusCat) fClusCat->setPersistency(perMdcClus);
}


Int_t HMdcTrackFinder::execute(void) {
//TStopwatch timer;
//timer.Start();
  Int_t numHits[6][4];
  Int_t s,m,l,c;
  for(Int_t i=0; i<6; i++) numHits[i][0]=numHits[i][1]=
                           numHits[i][2]=numHits[i][3]=0;
  iter->Reset();
  
  (*fLookUpTb).clear();
  if(f34ClFinder) (*f34ClFinder).clear(); 
  HMdcCal1* cal=0;
  while ((cal=(HMdcCal1 *)iter->Next())!=0) {
    cal->getAddress(s,m,l,c);
    
    Int_t  nHits=cal->getNHits();
    if(nHits>0) nHits=1;
    else if( nHits<0 ) {
      nHits=-nHits;
      if(nHits==2) nHits=3;
    } else continue;    
    if(!isGeant ) if(fcut && fcut->cut(cal)) continue; // Cut. for real data
    else {
      HMdcCal1Sim* cals=(HMdcCal1Sim *)cal;
      if((nHits&1) == 1 && cals->getStatus1()<0) nHits-=1;
      if((nHits&2) == 2 && cals->getStatus2()<0) nHits-=2;
      if(nHits==0) continue;
    }
    
    if(isCoilOff || m<2)  (*fLookUpTb)[s].setCell(m,l,c,nHits);
    else if(f34ClFinder) (*f34ClFinder)[s][m][l].setTime(c,nHits);
    numHits[s][m]++;
  }
   
//timer.Stop();  
//printf(" Cpu time for setCell=%f\n",timer.CpuTime());
//timer.Start();

  Int_t numcl=0;
  Int_t minbin[4];
  for(Int_t sec=0; sec<6; sec++) {
    if( nModules[sec] == 0 || &(*fLookUpTb)[sec]==0 ) continue;
    for(Int_t mod=0; mod<4; mod++) {
      minbin[mod]=listModules[sec][mod];
      if( minbin[mod]>0 ) {
// !?! mozhno isp. "Int_t nHits=fSec[mod].getNCells();" vmesto numHits[sec][mod]
        Int_t occup=(numHits[sec][mod]*6)/minbin[mod];
        if( occup <= level5[mod/2] ) minbin[mod]--;
        if( occup <= level4[mod/2] ) minbin[mod]--;
        if( occup < 3 && typeClFinder==0) minbin[mod]=3;
      }
    }
       
    numcl+=(*fLookUpTb)[sec].findClusters(minbin);
    fillCatMdcClus(sec); //??! vremennoe reshenie 

    if(f34ClFinder) {
      HMdcClus* clus;
      HMdc34ClFinderSec& fSec=(*f34ClFinder)[sec];
      if( !(&fSec) ) continue;
      if( numHits[sec][2]+numHits[sec][3] <= 0 ) continue;
      iterClus->Reset();
      locClus[0]=sec;
      locClus[1]=0;
      iterClus->gotoLocation(locClus);
      while ((clus=(HMdcClus *)iterClus->Next())!=0) {
        if(clus->getMod()==0 && minbin[1]>0) continue; // !!! at the indep.finding take MDC2 only for seg.2
        if(clus->getStatus()) numcl+=fSec.findClustersSeg2(clus,minbin);
      }
    }
  }
   
//timer.Stop();  
//printf(" Cpu time for track finder mdc1&2 =%g\n",timer.CpuTime());

  
//printf("*************Total %i clasters found:\n",numcl);
//  (*fLookUpTb).printClusters();

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
  const HMdcPrPlane& prPlane=flookup.getPrPlane();

  Int_t nClus=fclus.getNClusters();
  Int_t nClSeg[2];
  nClSeg[0]=nClSeg[1]=0;
  locNewCl[0]=sec;
  for(Int_t ncl=0; ncl<nClus; ncl++) {
    HMdcCluster& clus=fclus[ncl];
    Int_t seg1,seg2;
    Int_t modCl=clus.getMod();
    if(modCl<0) {
      seg1=(clus.getMaximum(0)+clus.getMaximum(1) > 0) ? 0:1;
      seg2=(clus.getMaximum(2)+clus.getMaximum(3) > 0) ? 1:0;
    } else seg1=seg2=modCl/2;
    for(Int_t seg=seg1; seg<=seg2; seg++) {
      locNewCl[1]=seg;
      locNewCl[2]=nClSeg[seg]++;
      HMdcClus* clusNew = (HMdcClus*)fClusCat->getSlot(locNewCl);
      if(!clusNew) {
          Warning("fillCatMdcClus","S.%i No slot HMdcClus available",sec+1);
          continue;  // continue(not break) -> HMdcBArray must be cleaned !
      }
      if(isGeant) clusNew = (HMdcClus *) (new(clusNew) HMdcClusSim);
      else  clusNew = new(clusNew) HMdcClus;
      clusNew->setAddress(locNewCl);
      if(seg1 != seg2) {
        if(seg == seg1) clusNew->setIndexChilds(nClSeg[seg2],nClSeg[seg2]);
        else clusNew->setIndexParent(nClSeg[seg1]-1);
      }
      if(modCl >= 0) {
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
           clusNew->setTime(lay,clus.getHit(lOC,nh),clus.getTimes(lOC,nh));
      }
      clusNew->setPrPlane(prPlane);   //??! luchshe brat' Ap,Bp...
      clusNew->setTarg(tar);
      clusNew->setErrTarg(errTar);
    }
  }
}

void HMdcTrackFinder::setNLayers(Int_t *lst) {
  for(Int_t sec=0; sec<6; sec++) {
    for(Int_t mod=0; mod<4; mod++) {
      if(lst[sec*4+mod]>3 && lst[sec*4+mod]<=6)
        listModules[sec][mod]=lst[sec*4+mod];
    }
  }
}

ClassImp(HMdcTrackFinder)
