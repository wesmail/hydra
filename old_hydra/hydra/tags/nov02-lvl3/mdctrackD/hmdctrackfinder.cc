//*-- Author : V.Pechenov
//*-- Modified : 07.05.2002 by V.Pechenov
//*-- Modified : 17.01.2002 by V.Pechenov
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
#include "hmdctrackdset.h"
#include "hmdcclussim.h"

HMdcTrackFinder::HMdcTrackFinder(Bool_t isCOff, Int_t typeClFn) {
  HMdcTrackDSet::setMdcTrackFinder(isCOff,typeClFn);
  setParContainers();
}

HMdcTrackFinder::HMdcTrackFinder(Text_t *name, Text_t *title,
     Bool_t isCOff, Int_t typeClFn) : HReconstructor(name,title) {
  HMdcTrackDSet::setMdcTrackFinder(isCOff,typeClFn);
  setParContainers();
}

HMdcTrackFinder::HMdcTrackFinder() {
  setParContainers();
}

HMdcTrackFinder::HMdcTrackFinder(Text_t *name, Text_t *title) : 
    HReconstructor(name,title) {
  setParContainers();
}

HMdcTrackFinder::~HMdcTrackFinder() {
  HMdcLookUpTb::deleteCont();
  HMdc34ClFinder::deleteCont();
  HMdcTrackDSet::pTrackFinder=0;
}

void HMdcTrackFinder::setParContainers() {
  prntSt=kFALSE;
  HMdcTrackDSet::setTrackFinder(this);
  isInitialised=kFALSE;
  fLookUpTb=0;
  f34ClFinder=0;
  level4[0]=HMdcTrackDSet::level4[0];
  level5[0]=HMdcTrackDSet::level5[0];
  level4[1]=HMdcTrackDSet::level4[1];
  level5[1]=HMdcTrackDSet::level5[1];
  locClus.set(2,0,0);
  locNewCl.set(3,0,0,0);
  fClusCat=0;
  perMdcClus=kFALSE;
  for(Int_t sec=0; sec<6; sec++) {
    nModules[sec]=0;
    for(Int_t mod=0; mod<4; mod++) listModules[sec][mod]=
        HMdcTrackDSet::nLayersInMod[sec][mod];
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
  fLookUpTb->setPar(319,HMdcTrackDSet::isCoiloff); //!!!!!!!!!!!!319!

  if( !HMdcTrackDSet::isCoiloff ) f34ClFinder=HMdc34ClFinder::getObject();
  else f34ClFinder=0;
  if(!prntSt) printStatus();
  return kTRUE;
}

Bool_t HMdcTrackFinder::reinit(void) {
  if(!fLookUpTb->initContainer()) return kFALSE;
  if( f34ClFinder && !f34ClFinder->initContainer() ) return kFALSE;
  if( !isInitialised ) {
    if(HMdcTrackDSet::clFinderType==1) {
      for(Int_t sec=0; sec<6; sec++) if(&((*fLookUpTb)[sec])) 
        (*fLookUpTb)[sec].setTypeClFinder(HMdcTrackDSet::clFinderType);
    }
    Int_t nModsF=(HMdcTrackDSet::isCoiloff) ? 4:2;
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
    if(!isGeant ) {
      if(fcut && fcut->cut(cal)) continue; // Cut. for real data
    } else {
      HMdcCal1Sim* cals=(HMdcCal1Sim *)cal;
      if((nHits&1) == 1 && cals->getStatus1()<0) nHits-=1;
      if((nHits&2) == 2 && cals->getStatus2()<0) nHits-=2;
      if(nHits==0) continue;
    }

    if(HMdcTrackDSet::isCoiloff || m<2)  (*fLookUpTb)[s].setCell(m,l,c,nHits);
    else if(f34ClFinder) (*f34ClFinder)[s][m][l].setTime(c,nHits);
    numHits[s][m]++;
  }

  // Cluster finder levels determination:
  Int_t minBin[6][4];
  for(Int_t sec=0; sec<6; sec++) {
    Int_t* minbin=minBin[sec];
    for(Int_t mod=0; mod<4; mod++) {
      minbin[mod]=listModules[sec][mod];
      if( minbin[mod]>0 ) {
// !?! mozhno isp. "Int_t nHits=fSec[mod].getNCells();" vmesto numHits[sec][mod]
        Int_t occup=(numHits[sec][mod]*6)/minbin[mod];
        if( occup <= level5[mod/2] ) minbin[mod]--;
        if( occup <= level4[mod/2] ) minbin[mod]--;
        if( occup < 3 && HMdcTrackDSet::clFinderType==0) minbin[mod]=3;
      }
    }
  }
  
  // Track finder in MDCI-II or all MDC's without mag.field
  Int_t numcl=0;
  for(Int_t sec=0; sec<6; sec++) {
    if( nModules[sec] == 0 || &(*fLookUpTb)[sec]==0 ) continue;
    numcl+=(*fLookUpTb)[sec].findClusters(minBin[sec]);
  }
  
  // Track finder in MDCIII-IV (with mag.field)
  if(f34ClFinder) {
    for(Int_t sec=0; sec<6; sec++) {
      HMdc34ClFinderSec& fSec=(*f34ClFinder)[sec];
      if( !(&fSec) ) continue;
      if( numHits[sec][2]+numHits[sec][3] <= 0 ) continue;
      Int_t* minbin=minBin[sec];
      iterClus->Reset();
      locClus[0]=sec;
      locClus[1]=0;
      iterClus->gotoLocation(locClus);
      HMdcClus* clus;
      while ((clus=(HMdcClus *)iterClus->Next())!=0) {
        if(clus->getMod()==0 && minbin[1]>0) continue; // !!! at the indep.finding take MDC2 only for seg.2
        numcl+=fSec.findClustersSeg2(clus,minbin);
      }
    }
  }
  return 0;
}

void HMdcTrackFinder::setNLayers(Int_t *lst) {
  HMdcTrackDSet::setTrFnNLayers(lst);
  for(Int_t sec=0; sec<6; sec++) {
    for(Int_t mod=0; mod<4; mod++) {
      if(lst[sec*4+mod]>3 && lst[sec*4+mod]<=6)
        listModules[sec][mod]=lst[sec*4+mod];
    }
  }
}

void HMdcTrackFinder::setLevel(Int_t l4s1, Int_t l5s1, Int_t l4s2, Int_t l5s2) {
  HMdcTrackDSet::setTrFnLevel(l4s1,l5s1,l4s2,l5s2);
  level4[0]=l4s1; 
  level5[0]=l5s1; 
  level4[1]=l4s2; 
  level5[1]=l5s2;
}

void HMdcTrackFinder::setLevelSeg1(Int_t l4, Int_t l5) {
  HMdcTrackDSet::setTrFnSegLevel(0,l4,l5);
  level4[0]=l4; 
  level5[0]=l5;
}

void HMdcTrackFinder::setLevelSeg2(Int_t l4, Int_t l5) {
  HMdcTrackDSet::setTrFnSegLevel(1,l4,l5);
  level4[1]=l4; 
  level5[1]=l5;
}

void HMdcTrackFinder::printStatus(void) {
  // prints the parameters to the screen
  char* hCoil ="0 = MagnetOn,  1 = MagnetOff";
  char* hType ="0 = combined chamber clusters, 1 = single chamber clusters";
  char* hLevS1="max. occupancy of MDC for level of cl.finding 4 & 5 layers";
  char* hLevS2="max. occupancy of MDC for level of cl.finding 4 & 5 layers";
  Int_t* l=&listModules[0][0];
  printf("--------------------------------------------------------------------------------------------\n");
  printf("HMdcTrackFinderSetup:\n");
  printf("MagnetStatus    =  %i :  %s\n",HMdcTrackDSet::isCoiloff,hCoil);
  printf("ClustFinderType =  %i :  %s\n",HMdcTrackDSet::clFinderType,hType);  
  printf("LevelsSegment1  =  %i,%i  : %s\n",level4[0],level5[0],hLevS1);
  printf("LevelsSegment2  =  %i,%i  : %s\n",level4[1],level5[1],hLevS2);
  printf("NumLayersPerMod = {{%i,%i,%i,%i},\n",l[ 0],l[ 1],l[ 2],l[ 3]);
  printf("                   {%i,%i,%i,%i},\n",l[ 4],l[ 5],l[ 6],l[ 7]);
  printf("                   {%i,%i,%i,%i},\n",l[ 8],l[ 9],l[10],l[11]);
  printf("                   {%i,%i,%i,%i},\n",l[12],l[13],l[14],l[15]);
  printf("                   {%i,%i,%i,%i},\n",l[16],l[17],l[18],l[19]);
  printf("                   {%i,%i,%i,%i}}\n",l[20],l[21],l[22],l[23]);
  printf ("--------------------------------------------------------------------------------------------\n");
  prntSt=kTRUE;
}

ClassImp(HMdcTrackFinder)
