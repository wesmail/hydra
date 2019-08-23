//*-- Author : V.Pechenov
//*-- Modified : 07.05.2002 by V.Pechenov
//*-- Modified : 17.01.2002 by V.Pechenov
//*-- Modified : 12.05.2001 by V.Pechenov

//_HADES_CLASS_DESCRIPTION 
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
// 5. isCoilOff=kFALSE, typeClustFinder=2:
//      The tracks will be searched in MDC1, MDC2 (segment 1) together
//      and after it in MDC1, MDC2 (segment 1) independently.
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
#include "hiterator.h"
#include "hcategory.h"
#include "hmdcgetcontainers.h"
#include "hmdcclus.h"
#include "hmdctimecut.h"
#include "hmdclookuptb.h"
#include "hmdc34clfinder.h"
#include "hmdctrackdset.h"
#include "hmdclistcells.h"
#include "hmdcgeanttrack.h"

HMdcTrackFinder::HMdcTrackFinder(Bool_t isCOff, Int_t typeClFn) {
  HMdcTrackDSet::setMdcTrackFinder(isCOff,typeClFn);
  setParContainers();
}

HMdcTrackFinder::HMdcTrackFinder(Text_t *name, Text_t *title,
     Bool_t isCOff, Int_t typeClFn) : HReconstructor(name,title) {
  HMdcTrackDSet::setMdcTrackFinder(isCOff,typeClFn);
  setParContainers();
}

HMdcTrackFinder::HMdcTrackFinder(void) {
  setParContainers();
}

HMdcTrackFinder::HMdcTrackFinder(Text_t *name, Text_t *title) : 
    HReconstructor(name,title) {
  setParContainers();
}

HMdcTrackFinder::~HMdcTrackFinder() {
  HMdcEvntListCells::deleteCont();
  HMdcGeantEvent::deleteCont();
  HMdcLookUpTb::deleteCont();
  HMdc34ClFinder::deleteCont();
  HMdcTrackDSet::pTrackFinder=0;
  if(iterClus){delete iterClus; iterClus=0;}
}

void HMdcTrackFinder::setParContainers() {
  HMdcTrackDSet::setTrackFinder(this);
  event         = 0;
  geantEvent    = 0;
  prntSt        = kFALSE;
  isInitialised = kFALSE;
  fLookUpTb     = 0;
  f34ClFinder   = 0;
  level4[0]     = HMdcTrackDSet::level4[0];
  level5[0]     = HMdcTrackDSet::level5[0];
  level4[1]     = HMdcTrackDSet::level4[1];
  level5[1]     = HMdcTrackDSet::level5[1];
  fClusCat      = 0;
  iterClus      = 0;
  perMdcClus    = kFALSE;
  
  locClus.set(2,0,0);
  locNewCl.set(3,0,0,0);
  for(Int_t sec=0; sec<6; sec++) {
    nModules[sec]=0;
    for(Int_t mod=0; mod<4; mod++) numLayers[sec][mod]=
        HMdcTrackDSet::nLayersInMod[sec][mod];
  }
}

Bool_t HMdcTrackFinder::init(void) {
  if(event == 0)      event      = HMdcEvntListCells::getObject();
  if(geantEvent == 0) geantEvent = HMdcGeantEvent::getObject();
  
  fClusCat=HMdcGetContainers::getCatMdcClus(kTRUE);
  if( !fClusCat ) return kFALSE;
  fClusCat->setPersistency(perMdcClus);
  iterClus=(HIterator *)fClusCat->MakeIterator();

  fLookUpTb=HMdcLookUpTb::getObject();
  fLookUpTb->setPar(319,HMdcTrackDSet::isCoiloff); //!!!!!!!!!!!!319!

  f34ClFinder = (HMdcTrackDSet::isCoiloff) ? 0 : HMdc34ClFinder::getObject();
  if(!prntSt) printStatus();
  return kTRUE;
}

Bool_t HMdcTrackFinder::reinit(void) {
  if(!fLookUpTb->initContainer()) return kFALSE;
  if( f34ClFinder && !f34ClFinder->initContainer(*event) ) return kFALSE;
  Int_t typeClFinder=HMdcTrackDSet::clFinderType;
  if( isInitialised ) return kTRUE;
  fLookUpTb->setTypeClFinder(typeClFinder);
  Int_t nModsF=(HMdcTrackDSet::isCoiloff) ? 4:2;
  for(Int_t sec=0; sec<6; sec++) {
    nModules[sec]=0;
    HMdcLookUpTbSec* fLookUpTbSec=&((*fLookUpTb)[sec]);
    for(Int_t mod=0; mod<nModsF; mod++) {
      HMdcLookUpTbMod* fLookUpTbMod=(fLookUpTbSec) ? &((*fLookUpTbSec)[mod]):0;
      if( fLookUpTbMod ) {
        nModules[sec]++;
        fLookUpTbMod->setNLayers(numLayers[sec][mod]);
      } else numLayers[sec][mod]=0;
    }
    HMdc34ClFinderSec* f34ClFinderSec=(f34ClFinder) ? &((*f34ClFinder)[sec]):0;
    if(f34ClFinderSec) f34ClFinderSec->setTypeClFinder(typeClFinder);
    for(Int_t mod=nModsF; mod<4; mod++) {
      if(f34ClFinderSec!=0 && (&((*f34ClFinderSec)[mod]))!=0) nModules[sec]++;
      else numLayers[sec][mod]=0;
    }
  }
  isInitialised=kTRUE;
  return kTRUE;
}

void HMdcTrackFinder::setPersistency(Bool_t perCl) {
  perMdcClus=perCl;
  if(fClusCat) fClusCat->setPersistency(perMdcClus);
}


Int_t HMdcTrackFinder::execute(void) {
  event->clear();
  Int_t nFiredWires = event->collectWires();
  if(geantEvent) geantEvent->collectTracks(); // MUST be after collectWires()!
  if(nFiredWires<3) return 0;                 // MUST be after collectTracks()!
  
  calcClFndrLevel();             // Cluster finder levels determination
  
  Int_t numcl=clFndrBeforField(); // Cl.finder in MDCI-II/all(without mag.field)
  
  if(f34ClFinder) {              // Track finder in MDCIII-IV (with mag.field)
    f34ClFinder->clear();
    setMod34Levels();
    if(HMdcTrackDSet::pClusterToHit) numcl+=noFitClFndr34();
  }
  return 0;
}

void HMdcTrackFinder::fillLookUpTb(void) {
  (*fLookUpTb).clear();
  Int_t s,m,l,c;
  s=m=l=c=-1;
  UChar_t tm;
  while(event->nextCell(s,m,l,c,tm)) {
    if(HMdcTrackDSet::isCoiloff || m<2) {
      HMdcLookUpTbSec& fSec=(*fLookUpTb)[s];
      if(&fSec) fSec.setCell(m,l,c,tm);
    }
  }
}

void HMdcTrackFinder::calcClFndrLevel(void) {
  // Cluster finder levels determination:
  for(Int_t sec=0; sec<6; sec++) {
    Int_t* levelSec=clFndrLevel[sec];
    for(Int_t mod=0; mod<4; mod++) {
      levelSec[mod]=numLayers[sec][mod];
      if( levelSec[mod]>0 ) {
        Int_t occup=((*event)[sec][mod].getNCells()*6)/levelSec[mod];
	if( occup <= level5[mod/2] ) levelSec[mod]--;
	if( occup <= level4[mod/2] ) levelSec[mod]--;
	if( occup < 3 && HMdcTrackDSet::getClFinderType()==0) levelSec[mod]=3;
        if( levelSec[mod]<3 ) levelSec[mod]=3;
        
        if( HMdcTrackDSet::getClFinderType()==2 && mod>1) {
          if(numLayers[sec][mod]<4) {
            levelSec[mod]=numLayers[sec][mod]-1;
            if(levelSec[mod]<0) levelSec[mod]=0;
          }
        }
      }
    }
  }
}

Int_t HMdcTrackFinder::clFndrBeforField(void) {
  // Track finder in MDCI-II or all MDC's without mag.field
  fillLookUpTb();                // Filling HMdcLookUpTb by list of cells
  Int_t numcl=0;
  for(Int_t sec=0; sec<6; sec++) {
    if( nModules[sec] == 0 || &(*fLookUpTb)[sec]==0 ) continue;
    numcl+=(*fLookUpTb)[sec].findClusters(clFndrLevel[sec]);
  }
  return numcl;
}

void HMdcTrackFinder::setMod34Levels(void) {  
  for(Int_t sec=0; sec<6; sec++) {
    HMdc34ClFinderSec& fSec=(*f34ClFinder)[sec];
    if( !(&fSec) ) continue;
    fSec.setMinBin(clFndrLevel[sec]);
  }
}

Int_t HMdcTrackFinder::noFitClFndr34(void) {
  Int_t numcl=0;
  iterClus->Reset();
  for(Int_t sec=0; sec<6; sec++) {
    HMdc34ClFinderSec& fSec=(*f34ClFinder)[sec];
    if( !(&fSec) ) continue;
    if(fSec.notEnoughWires()) continue;
    locClus[0]=sec;
    iterClus->gotoLocation(locClus);
    HMdcClus* clus;
    while ((clus=(HMdcClus *)iterClus->Next())!=0) {
      Int_t typeClFn=clus->getTypeClFinder();
      Int_t mod=clus->getMod();
      if(typeClFn==1) {
        if(HMdcTrackDSet::modForSeg1 != mod) continue;
      } else if(typeClFn==2 && mod>=0) {
        //                                           mixFitCut for fit!
        if(clus->getNLayersMod(mod^1)<HMdcTrackDSet::mixClusCut) continue;
      }
      numcl+=fSec.findClustersSeg2(clus);
    }
  }
  return numcl;
}

void HMdcTrackFinder::setNLayers(Int_t *lst) {
  HMdcTrackDSet::setTrFnNLayers(lst);
  for(Int_t sec=0; sec<6; sec++) {
    for(Int_t mod=0; mod<4; mod++) {
      if(lst[sec*4+mod]>3 && lst[sec*4+mod]<=6)
        numLayers[sec][mod]=lst[sec*4+mod];
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
  char* hType ="0 = combined chamber clusters, 1 = single cham. cl., 2 = mixed cl.(0+1)";
  char* hLevS1="max. occupancy of MDC for level of cl.finding 4 & 5 layers";
  char* hLevS2="max. occupancy of MDC for level of cl.finding 4 & 5 layers";
  Int_t* l=&numLayers[0][0];
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
