//*-- AUTHOR : Vladimir Pechenov
//*-- Modified : 25/02/99

//_HADES_CLASS_DESCRIPTION 
//


using namespace std;
#include "hmdcgeanttrack.h"
#include "hades.h"
#include "hevent.h"
//#include "hcategory.h"
#include "hmatrixcategory.h"
#include "hiterator.h"
#include "hmdcsizescells.h"
#include "hgeantkine.h"
#include "hgeantmdc.h"
#include "hmdccal1sim.h"
#include "hmdcdef.h"
#include "hpidphysicsconstants.h"
#include "hspectrometer.h"
#include "hmdcdetector.h"
#include "hmdclistcells.h"
#include "hmdcclussim.h"
#include "hmdcsegsim.h"
#include "hmdchitsim.h"
#include "hmdcgetcontainers.h"
#include "hgeantmdc.h"
#include <iostream> 
#include <iomanip>
#include <stdlib.h>

ClassImp(HMdcGeantSeg)

ClassImp(HMdcGeantTrack)

ClassImp(HMdcGeantEvent)

HMdcGeantSeg::HMdcGeantSeg(void) {
  clear(-1);
}

void HMdcGeantSeg::clear(Int_t ns) {
  nHitsMdc[0]=nHitsMdc[1]=0;
  segNumber=ns;
  for(Int_t m=0;m<2;m++) for(Int_t l=0;l<7;l++) geantLay[m][l]=0;
  sec = -1;
  mod = -1;
  HMdcList12GroupCells::clear();
}

void HMdcGeantSeg::addFirstHit(HGeantMdc* pGeantMdc,Bool_t* mdcSecSetup,Int_t ns) {
  for(Int_t m=0;m<2;m++) for(Int_t l=0;l<7;l++) geantLay[m][l]=0;
  HMdcList12GroupCells::clear();
  mod        = pGeantMdc->getModule();  // =0 or 1 !
  ioseg      = mod/2;
  Int_t modI = mod%2;
  Int_t lay  = pGeantMdc->getLayer();
  sec        = pGeantMdc->getSector();
  segNumber  = ns;
  nHitsMdc[0]=nHitsMdc[1]=0;
  if(lay!=6) nHitsMdc[modI]++;
  geantLay[modI][lay] = pGeantMdc;
  direction = dirHit(pGeantMdc);
  if(mdcSecSetup[ioseg*2] && mdcSecSetup[ioseg*2+1]) mod=-1;
}

Char_t HMdcGeantSeg::dirHit(HGeantMdc* pGeantMdc) {
  Float_t ath, aph;
  pGeantMdc->getIncidence(ath,aph);
  return dirTheta(ath); 
}

Bool_t HMdcGeantSeg::addHit(HGeantMdc* pGeantMdc) {
  if(pGeantMdc->getSector() != sec) return kFALSE;
  Char_t mod  = pGeantMdc->getModule();
  if(mod/2 != ioseg) return kFALSE;
  if(direction != dirHit(pGeantMdc)) return kFALSE;
  Int_t modI = mod%2;
  Int_t lay  = pGeantMdc->getLayer();
  if(geantLay[modI][lay]) return kFALSE;
  geantLay[modI][lay] = pGeantMdc;
  if(lay!=6) nHitsMdc[modI]++;
  return kTRUE;
}

void HMdcGeantSeg::setAnotherHit(HGeantMdc* pGeantMdc) {
  Int_t modI = pGeantMdc->getModule()%2;  // =0 or 1 !
  Int_t lay  = pGeantMdc->getLayer();
  if(geantLay[modI][lay]) geantLay[modI][lay]=pGeantMdc;
}


Int_t HMdcGeantSeg::getFirstLayer12(void) const {
  if(nHitsMdc[0]) {
    for(Int_t l=0;l<6;l++) if(geantLay[0][l]) return l;
  } else for(Int_t l=0;l<6;l++) if(geantLay[1][l]) return l+6;
  return -1;
}

Int_t HMdcGeantSeg::getLastLayer12(void) const {
  if(nHitsMdc[1]) {
    for(Int_t l=5;l>=0;l--) if(geantLay[1][l]) return l+6;
  } else for(Int_t l=5;l>=0;l--) if(geantLay[0][l]) return l;
  return -1;
}

HGeantMdc* HMdcGeantSeg::getMdcLayerHit(Int_t m,Int_t l) {
  return (nMdcOk(m) && l>=0 && l<7) ? geantLay[m%2][l]:0;
}

void HMdcGeantSeg::print(void) {
  printf("%3i) %i sec. %i seg.   GeantMdc:%4i hits (layers ",
      segNumber,sec+1,ioseg+1,getNGMdcHits());
  if(direction>0) printf("%2i -> %2i)",getFirstLayer12()+1,getLastLayer12()+1);
  else            printf("%2i -> %2i)",getLastLayer12()+1,getFirstLayer12()+1);
  if(areWiresColl) printf("   MdcCal1:%4i hits in %2i layers",
      getNDrTimes(),getNLayers());
  printf("\n");
}

Bool_t HMdcGeantSeg::getLayerHitPos(Int_t m, Int_t l, HGeomVector& hit,
    Bool_t extrFlag) {
  // Return geant "hit" position in mdc layer.
  // If no GeantMdc hit in this layer and extrFlag=kTRUE
  // function does extrapolation to this layer.
  if(!nMdcOk(m) || l<0 || l>5) return kFALSE;
  Int_t modI=m%2;
  if(geantLay[modI][l]) {
    Float_t ax,ay,tof,momLay;
    geantLay[modI][l]->getHit(ax,ay,tof,momLay);
    hit.setXYZ(ax,ay,0.);
    return kTRUE;
  }
  if(!extrFlag) return kFALSE;
  //================= If no GeantMdc hit in layer "l": =================
  if(nHitsMdc[modI]<1 || (nHitsMdc[modI]==1 && geantLay[modI][6]==0))
      return kFALSE;    // must be two hits at list.
  
  Int_t layLFirst,layLSecond,layRFirst,layRSecond;
  layLFirst=layLSecond=layRFirst=layRSecond=-1; 
  for(Int_t lay=l-1;lay>=0; lay--) {
    if(l>2 && lay<3 && geantLay[modI][6]) {
      if(layLFirst<0) layLFirst=6;
      else if(layLSecond<0 && layLFirst!=6) layLSecond=6;
    }
    if(geantLay[modI][lay]) {
      if(layLFirst<0) layLFirst=lay;
      else if(layLSecond<0) layLSecond=lay;
    }
  }
  for(Int_t lay=l+1;lay<6; lay++) {
    if(l<3 && lay>2 && geantLay[modI][6]) {
      if(layRFirst<0) layRFirst=6;
      else if(layRSecond<0 && layRFirst!=6) layRSecond=6;
    }
    if(geantLay[modI][lay]) {
      if(layRFirst<0) layRFirst=lay;
      else if(layRSecond<0) layRSecond=lay;
    }
  }
  if(layLFirst<0)      calcMdcHitPos(modI,layRFirst,  layRSecond, hit,l);
  else if(layRFirst<0) calcMdcHitPos(modI,layLSecond, layLFirst,  hit,l);
  else                 calcMdcHitPos(modI,layLFirst,  layRFirst,  hit,l);
  return kTRUE;
}

Int_t HMdcGeantSeg::getFirstGeantMdcLayer(Int_t m) const {
  // Return first layer with geant hit in mdc "m".
  if(!nMdcOk(m)) return -1;
  Int_t modI=m%2;
  for(Int_t lay=0;lay<6;lay++) if(geantLay[modI][lay]) return lay;
  return -1;
}

Int_t HMdcGeantSeg::getLastGeantMdcLayer(Int_t m) const {
  // Return first layer with geant hit in mdc "m".
  if(!nMdcOk(m)) return -1;
  Int_t modI=m%2;
  for(Int_t lay=5;lay>=0;lay--) if(geantLay[modI][lay]) return lay;
  return -1;
}

Bool_t HMdcGeantSeg::getFirstAndLastGMdcLayers(Int_t m,Int_t& lFisrt,Int_t& lLast) const {
  // return kFALSE if "m" is not valid or one mdc geant hit exist only.
  if(!nMdcOk(m)) return kFALSE;
  Int_t modI=m%2;
  if(nHitsMdc[modI]<2) return kFALSE;
  for(lFisrt=0;lFisrt<6;lFisrt++) if(geantLay[modI][lFisrt]) break;
  for(lLast=5;lLast>=0;lLast--) if(geantLay[modI][lLast]) break;
  return kTRUE;
}


Bool_t HMdcGeantSeg::getMdcHitPos(Int_t m, HGeomVector& hit) {
  // Return geant "hit" position in mdc mid-plane.
  if(!nMdcOk(m)) return kFALSE;
  Int_t modI=m%2;
  if(geantLay[modI][6]) {
    Float_t ax,ay,tof,momLay;
    geantLay[modI][6]->getHit(ax,ay,tof,momLay);
    hit.setXYZ(ax,ay,0.);
    return kTRUE;
  }
  //=============== If no GeantMdc hit in mdc mid-plane: ===============
  if(nHitsMdc[modI]<2) return kFALSE;
  Int_t layLFirst,layLSecond,layRFirst,layRSecond;
  layLFirst=layLSecond=layRFirst=layRSecond=-1; 
  for(Int_t lay=2;lay>=0; lay--) if(geantLay[modI][lay]) {
    if(layLFirst<0) layLFirst=lay;
    else if(layLSecond<0) layLSecond=lay;
  }
  for(Int_t lay=3;lay<6; lay++) if(geantLay[modI][lay]) {
    if(layRFirst<0) layRFirst=lay;
    else if(layRSecond<0) layRSecond=lay;
  }
  if(layLFirst<0)      calcMdcHitPos(modI,layRFirst,  layRSecond, hit);
  else if(layRFirst<0) calcMdcHitPos(modI,layLSecond, layLFirst,  hit);
  else                 calcMdcHitPos(modI,layLFirst,  layRFirst,  hit); 
  return kTRUE;  
}

void HMdcGeantSeg::calcMdcHitPos(Int_t modI,Int_t lay1, Int_t lay2, 
    HGeomVector& hit, Int_t lay) {
  // Linear extrapolation of two geant mdc hits to the mdc mid-plane.
  Float_t x1,y1,x2,y2,tof,momLay;
  geantLay[modI][lay1]->getHit(x1,y1,tof,momLay);
  geantLay[modI][lay2]->getHit(x2,y2,tof,momLay);
  Float_t l0 = (lay==6)  ? 2.5:lay;
  Float_t l1 = (lay1==6) ? 2.5:lay1;
  Float_t l2 = (lay2==6) ? 2.5:lay2;
  Double_t norm=(l0-(l1))/(l2-l1);
  hit.setXYZ(x1+(x2-x1)*norm,y1+(y2-y1)*norm,0.);
}

Bool_t HMdcGeantSeg::getMdcHitPosSec(Int_t m, HGeomVector& hit) {
  // Return geant "hit" position in mid-plane mdc in sector coor.sys.
  HMdcSizesCells* pMdcSizesCells=HMdcSizesCells::getExObject();
  if(pMdcSizesCells==0) return kFALSE;
  if(!getMdcHitPos(m,hit)) return kFALSE;
  HMdcSizesCellsSec& pSCSec=(*pMdcSizesCells)[sec];
  HMdcSizesCellsMod& pSCMod=pSCSec[m];
  hit=pSCMod.getSecTrans()->transFrom(hit);
  return kTRUE;
}

Bool_t HMdcGeantSeg::getMdcHitPosLab(Int_t m, HGeomVector& hit) {
  // Return geant "hit" position in mid-plane mdc in lab.coor.sys..
  HMdcSizesCells* pMdcSizesCells=HMdcSizesCells::getExObject();
  if(pMdcSizesCells==0) return kFALSE;
  if(!getMdcHitPos(m,hit)) return kFALSE;
  HMdcSizesCellsSec& pSCSec=(*pMdcSizesCells)[sec];
  HMdcSizesCellsMod& pSCMod=pSCSec[m];
  hit=pSCMod.getSecTrans()->transFrom(hit);
  hit=pSCSec.getLabTrans()->transFrom(hit);
  return kTRUE;
}

Bool_t HMdcGeantSeg::getLayerHitPosSec(Int_t m, Int_t l,HGeomVector& hit,
    Bool_t extrFlag) {
  // Return geant "hit" position in mdc layer in sector coor.sys..
  HMdcSizesCells* pMdcSizesCells=HMdcSizesCells::getExObject();
  if(pMdcSizesCells==0) return kFALSE;
  if(!getLayerHitPos(m,l,hit,extrFlag)) return kFALSE;
  HMdcSizesCellsSec& pSCSec=(*pMdcSizesCells)[sec];
  HMdcSizesCellsLayer& pSCLay=pSCSec[m][l];
  hit=pSCLay.getSecTrans()->transFrom(hit);
  return kTRUE;
}

Bool_t HMdcGeantSeg::getLayerHitPosLab(Int_t m, Int_t l, HGeomVector& hit,
    Bool_t extrFlag) {
  // Return geant "hit" position in mdc layer in lab.coor.sys.
  // If no GeantMdc hit in this layer and extrFlag=kTRUE
  // function does extrapolation to this layer.
  HMdcSizesCells* pMdcSizesCells=HMdcSizesCells::getExObject();
  if(pMdcSizesCells==0) return kFALSE;
  if(!getLayerHitPos(m,l,hit,extrFlag)) return kFALSE;
  HMdcSizesCellsSec& pSCSec=(*pMdcSizesCells)[sec];
  HMdcSizesCellsLayer& pSCLay=pSCSec[m][l];
  hit=pSCLay.getSecTrans()->transFrom(hit);
  hit=pSCSec.getLabTrans()->transFrom(hit);
  return kFALSE;
}

Bool_t HMdcGeantSeg::geantClustPos(HMdcClusSim* pMdcClusSim, 
    HMdcList12GroupCells* listIden, Int_t indtr, //indtr ???
    Float_t& x, Float_t& y) {
  if(pMdcClusSim->getSec() != sec) return kFALSE;
  if(pMdcClusSim->getIOSeg() != ioseg) return kFALSE;
  HMdcSizesCells* pMdcSizesCells=HMdcSizesCells::getExObject();
  if(pMdcSizesCells==0) return kFALSE;
//?  Short_t nWrSeg = pMdcClusSim->getNTimesInTrack(indtr);
//?  Int_t nLayMod1 = pMdcClusSim->getNLayersInTrack(indtr,0);
//?  Int_t nLayMod2 = pMdcClusSim->getNLayersInTrack(indtr,1);
  
//???  HMdcList12GroupCells listIden;
//???  Int_t nLaysSeg = compare(pMdcClusSim,0,11,&listIden);
  Int_t nLaysM1 = listIden->getNLayersMod(0);
  Int_t nLaysM2 = listIden->getNLayersMod(1);
  if(nLaysM1+nLaysM2 < 2) return kFALSE; // ??????????????????

  Short_t modCl = pMdcClusSim->getMod();
  if(pMdcClusSim->getTypeClFinder()==1 && modCl>=0) {
    if((modCl%2) == 0) nLaysM2=0;
    else nLaysM1=0;
  }
  
  HGeomVector hit1;
  HGeomVector hit2;
Float_t dX1,dY1,dX2,dY2,dX3,dY3,dX4,dY4,dX,dY;
dX1=dY1=dX2=dY2=dX3=dY3=dX=dY=-100000.;
  if(mod>=0) {
    if(!getMdcHitPosSec(mod,hit1)) return kFALSE;
    pMdcClusSim->calcIntersection(hit1,x,y);
pMdcClusSim->print();
printf("+++ dX3=%.1f dY3=%.1f \n",pMdcClusSim->getX()-x,pMdcClusSim->getY()-y);
    return kTRUE;
  }
  
  if(nLaysM1==0 || nLaysM2==0) {
pMdcClusSim->print();
printf("+++=0! nLaysM1=%i nLaysM2=%i\n",nLaysM1,nLaysM2);
    Int_t m = (nLaysM1>0) ? ioseg*2 : ioseg*2+1;
    Int_t l1,l2;
    if(!getFirstAndLastGMdcLayers(m,l1,l2)) return kFALSE;
printf("+++ m=%i l1=%i l2=%i\n",m,l1,l2);
    if(!getLayerHitPosSec(m,l1,hit1,kFALSE)) return kFALSE;
    if(!getLayerHitPosSec(m,l2,hit2,kFALSE)) return kFALSE;
Float_t x1,y1,x2,y2;
pMdcClusSim->calcIntersection(hit1,x1,y1);
pMdcClusSim->calcIntersection(hit2,x2,y2);
dX1=pMdcClusSim->getX()-x1;
dX2=pMdcClusSim->getX()-x2;
dY1=pMdcClusSim->getY()-y1;
dY2=pMdcClusSim->getY()-y2;

HGeomVector hit;
if(nLaysM2==0) {if(!getMdcHitPosSec(ioseg*2,hit)) return kFALSE;}
else if(!getMdcHitPosSec(ioseg*2+1,hit)) return kFALSE;
pMdcClusSim->calcIntersection(hit,x1,y1);
dX4=pMdcClusSim->getX()-x1;
dY4=pMdcClusSim->getY()-y1;
printf("+++ dX4=%.1f dY4=%.1f \n",pMdcClusSim->getX()-x1,pMdcClusSim->getY()-y1);

  } else {
pMdcClusSim->print();
printf("+++ nLaysM1=%i nLaysM2=%i\n",nLaysM1,nLaysM2);
    if(!getMdcHitPosSec(ioseg*2,  hit1)) return kFALSE;//???????????????
    if(!getMdcHitPosSec(ioseg*2+1,hit2)) return kFALSE;//???????????????
Float_t x1,y1,x2,y2;
pMdcClusSim->calcIntersection(hit1,x1,y1);
pMdcClusSim->calcIntersection(hit2,x2,y2);
dX1=pMdcClusSim->getX()-x1;
dX2=pMdcClusSim->getX()-x2;
dY1=pMdcClusSim->getY()-y1;
dY2=pMdcClusSim->getY()-y2;
  }
  pMdcClusSim->calcIntersection(hit1,hit2,x,y);
dX=pMdcClusSim->getX()-x;
dY=pMdcClusSim->getY()-y;
printf("***  dX=%.1f  dY=%.1f\n",dX,dY);
if(dX1>-90000.) printf("+++ dX1=%.1f dY1=%.1f \n+++ dX2=%.1f dY2=%.1f\n",dX1,dY1,dX2,dY2);
  return kTRUE;
}

//===========================================================================

Bool_t  HMdcGeantTrack::mdcSetup[6][4]=
    {{1,1,1,1},{1,1,1,1},{1,1,1,1},{1,1,1,1},{1,1,1,1},{1,1,1,1}};

HMdcGeantTrack::HMdcGeantTrack(void) {
  // constructor for the case when geantSegment array created in HMdcGeantTrack
  arrSizeLoc    = 0;
  arrSize       = &arrSizeLoc;
  arrOffset     = 0;
  arrGlobOffset = 0;
  iterMdcCal1   = 0;
  segments      = new TObjArray();
  clear();
}

HMdcGeantTrack::HMdcGeantTrack(TObjArray* arr,Int_t* size,Int_t* offst) {
  // constructor for the case when geantSegment array created out of HMdcGeantTrack
  // parameters:
  // arr - pointer to TObjArray for HMdcGeantSeg objects
  // size - pointer to Int_t value which keep num. of not empty slots 
  //        in TObjArray (should not has empty slots between not empty slots)
  // offset - pointer to Int_t value which keep num. of HMdcGeantSeg objects
  //          already used in TObjArray for previous tracks
  segments      = arr;
  arrSize       = size;
  arrOffset     = 0;
  arrGlobOffset = offst;
  pMdcCal1Cat   = 0;
  iterMdcCal1   = 0;
  clear();
}

HMdcGeantTrack::~HMdcGeantTrack(void) {
  if(iterMdcCal1) {
    delete iterMdcCal1;
    iterMdcCal1 = 0;
  }
  if(segments && arrGlobOffset==0) {
    segments->Delete();
    delete segments;
    segments=0;
  }
}

void HMdcGeantTrack::clear(void) {
  pGeantKine     = 0;
  debugPrintFlag = kFALSE;
  setDefault();
}

void HMdcGeantTrack::setDefault(void) {
  nGeantMdcHits = 0;
  nSegments     = 0;
  nWSegments    = 0;
  mdcWSector    = -1;
  mdcSector     = -1;
  nIOSeg        = 0;
  nWIOSeg       = 0;
  nWires        = -1;
  directionFlag = kTRUE;
  geantBugFlag  = kFALSE;
}

void HMdcGeantTrack::testMdcSetup(void) {
  if(gHades==0) return;
  HMdcDetector* pMdcDet=(HMdcDetector*)(gHades->getSetup()->getDetector("Mdc"));
  if(pMdcDet==0) return;
  for(Int_t s=0;s<6;s++) for(Int_t m=0;m<4;m++)
      if(pMdcDet->getModule(s,m)==0) mdcSetup[s][m]=kFALSE;
}

// void HMdcGeantTrack::setMdcEvntListCells(HMdcEvntListCells* pt) {
//   if(HMdcEvntListCells::getExObject()) return;
//   if(pt) pMdcListCells = pt;
//   else   pMdcListCells = HMdcEvntListCells::getExObject;
//   if(pMdcListCells) HMdcGeantSeg::setWiresAreColl(); //???
// }

Bool_t HMdcGeantTrack::setMdcCal1Cat(void) {
  HCategory* cat = HMdcGetContainers::getCatMdcCal1();
  if(pMdcCal1Cat && pMdcCal1Cat==cat) return kTRUE;
  pMdcCal1Cat=cat;
  if(pMdcCal1Cat==0) return kFALSE;
  if(iterMdcCal1) delete iterMdcCal1;
  iterMdcCal1 = (HIterator*)pMdcCal1Cat->MakeIterator(); 
   
//   if(pMdcListCells) return kTRUE;
//   if(cat) pMdcCal1Cat=cat;
//   if(pMdcCal1Cat) return kTRUE;
//   if(gHades) pMdcCal1Cat = gHades->getCurrentEvent()->getCategory(catMdcCal1);
//   if(pMdcCal1Cat==0) return kFALSE;
//   if(iterMdcCal1) delete iterMdcCal1;
//   iterMdcCal1 = (HIterator*)pMdcCal1Cat->MakeIterator();
//   HMdcGeantSeg::setWiresAreColl();
  return kTRUE;
}

Int_t HMdcGeantTrack::testGeantTrack(Int_t trNum) {
  // return number of mdc segments
  HCategory* pGeantKineCat = HMdcGetContainers::getCatGeantKine();
  if(pGeantKineCat) return 
      testGeantTrack((HGeantKine*)pGeantKineCat->getObject(trNum-1));
  Error("testGeantTrack"," Can't get catGeantKine category!");
  setDefault();
  return 0;
}

Int_t HMdcGeantTrack::testGeantTrack(HGeantKine* pGK) {
  // return number of mdc segments
  pGeantKine = pGK;
  if(pGeantKine==0) {
    setDefault();
    Error("testGeantTrack"," pointer of HGeantKine object = 0!");
    return 0;
  }
//  setMdcEvntListCells();
  testHitsInDetectors();
  return testMdcHits();
}

void HMdcGeantTrack::testHitsInDetectors(void) {
  trackNumber  = pGeantKine->getTrack(); 
  mom          = pGeantKine->getTotalMomentum(); 
  isInMdcFlag  = ( pGeantKine->getFirstMdcHit()>=0  )      ? kTRUE:kFALSE;
  isInRichFlag = ( pGeantKine->getFirstRichHit()>=0 )      ? kTRUE:kFALSE;
  isInMetaFlag = ( pGeantKine->getFirstTofHit()>=0    ||
                   pGeantKine->getFirstShowerHit()>=0 ||
                   pGeantKine->getFirstRpcHit()>=0       ) ? kTRUE:kFALSE;
}

Int_t HMdcGeantTrack::testMdcHits(void) {
  // return number of mdc segments
  setDefault();
  if(!isInMdcFlag) return 0;
  if(arrGlobOffset) arrOffset = *arrGlobOffset;
  
  pGeantMdcPrev     = 0;
  Char_t  modPrev   = -1;       
  Char_t  lay14Prev = -1;
  Float_t tofPrev   = -1.;
  Float_t momPrev   = mom;
  geantBugFlag      = kFALSE;

  if(!addSegAtAndExpand(0)) return 0;
  HCategory* pGeantMdcCat = HMdcGetContainers::getCatGeantMdc();
  if(pGeantMdcCat==0) {
    Error("testMdcHits","Can't get catMdcGeantRaw category!");
    return 0;
  }
  pGeantKine->setMdcCategory(pGeantMdcCat);
  pGeantKine->resetMdcIter();
  while((pGeantMdc=(HGeantMdc*)pGeantKine->nextMdcHit()) != NULL) {
    Char_t sec = pGeantMdc->getSector();
    Char_t mod = pGeantMdc->getModule();
    if(!mdcSetup[(Int_t)sec][(Int_t)mod]) continue;
    Char_t lay    = pGeantMdc->getLayer();
    Char_t lay14  = calcLay14(mod,lay);
    Char_t hitDir = HMdcGeantSeg::dirHit(pGeantMdc);
    Float_t ax,ay,tof,momLay;
    pGeantMdc->getHit(ax,ay,tof,momLay);
    Char_t  dMod  = mod-modPrev;
    Char_t  dLay  = lay14-lay14Prev;
    Float_t dMom  = momPrev-momLay;
    Bool_t isDirOk = kTRUE;
    if(modPrev>=0 && (hitDir!=dirDLayer(dLay) || 
        hitDir!=segment->getDirection())) isDirOk = kFALSE;
    
    // Finding bugs in HGeantMdc:
    if(isGeantBug1(momLay)) continue;
    if(isGeantBug2(sec,dMod,dLay,hitDir,tof-tofPrev)) continue;
    mayBeGeantBug(dMom);
    
    if(debugPrintFlag && nGeantMdcHits==0) printf("---- New track: ----\n");
    if(modPrev<0 || !isDirOk || !segment->addHit(pGeantMdc)) {
      // New segment ---------------------------
      if(modPrev>=0 && !addSegment()) break;
      segment->addFirstHit(pGeantMdc,mdcSetup[(Int_t)sec],nSegments);
    }
    if(lay!=6) collectWires(sec,mod,lay,tof);
    if(debugPrintFlag) printDebug(dMom,dMod);
    nGeantMdcHits++;
    pGeantMdcPrev = pGeantMdc;
    modPrev       = mod;
    lay14Prev     = lay14;
    tofPrev       = tof;
    momPrev       = momLay;
  }
  if(nGeantMdcHits>0) {
    if(segment->getNGMdcHits()>0) nSegments++;
    else nGeantMdcHits--;  // remove one mid-planeHit segment
  }
  if(nGeantMdcHits==0) isInMdcFlag=kFALSE;
  calcNSectors();
  return nSegments;
}

Bool_t HMdcGeantTrack::addSegAtAndExpand(Int_t segNum) {
  if(*arrSize <= segNum+arrOffset) {
    segment=new HMdcGeantSeg();
    segment->clear(segNum);
    segments->AddAtAndExpand(segment,segNum+arrOffset);
    (*arrSize)++;
  } else segment = (HMdcGeantSeg*)segments->At(segNum+arrOffset);
  if(segment) return kTRUE;
  return kFALSE;
}

Bool_t HMdcGeantTrack::addSegment(void) {
  if(segment->getNGMdcHits()>0) {
    nSegments++;
    if(!addSegAtAndExpand(nSegments)) return kFALSE;
    if(debugPrintFlag) printf("- New segment:\n");
  } else {
    if(debugPrintFlag) printDebug(pGeantMdcPrev,-1," ! mid-plane segment");
    nGeantMdcHits--;  // remove segment from one mdc mid-plane hit
  }
  return kTRUE;
}

Int_t HMdcGeantTrack::calcLay14(Int_t m, Int_t l) {
  // recalculate module and layer number in layer number from 0 to 13
  // were numb. 3 and 10 correspond mid-planes of mdc's
  if(l<3) return l+(m%2)*7;
  if(l<6) return l+1+(m%2)*7;
  return 3+(m%2)*7;             // mid-plane MDC
}

Bool_t HMdcGeantTrack::isGeantBug1(Float_t momLay) {
  // Finding bug in HGeantMdc. Hit momentum > vertex momentum.
  if(mom<momLay-0.001) {
    if(debugPrintFlag) printDebug(pGeantMdc,-1,"! P>Pver");
    geantBugFlag=kTRUE;
    return kTRUE;
  }
  return kFALSE;
}

Bool_t HMdcGeantTrack::isGeantBug2(Char_t sec,Char_t dMod, Char_t dLay,
    Char_t hitDir, Float_t dTof) {
  // Finding bug in HGeantMdc. Too short t.o.f. between two hits.
  if(dTof>0.006 || segment->getNGMdcHits()<=0) return kFALSE;
  if(sec==segment->getSec() && dLay==0 && dMod==0) {
    if(hitDir != segment->getDirection() || !isGeantBug3()) return kFALSE;
  } else if(abs(dLay) <= 1) return kFALSE;
  if(debugPrintFlag) printDebug(pGeantMdc,-1,"! dTof<0.006");
  geantBugFlag=kTRUE;
  return kTRUE;
}

Bool_t HMdcGeantTrack::mayBeGeantBug(Float_t dMom) {
  // Finding bug in HGeantMdc. Hit momentum > prev.hit momentum.
  if(dMom > -0.01) return kFALSE;
  geantBugFlag=kTRUE;
  return kTRUE;
}

Bool_t HMdcGeantTrack::isGeantBug3(void) {
  // removing fake HGeantMdc hits from list
  Float_t x1,y1,tof1,p1;
  pGeantMdcPrev->getHit(x1,y1,tof1,p1);
  Float_t x2,y2,tof2,p2;
  pGeantMdc->getHit(x2,y2,tof2,p2);
  if(fabs(x1-x2) > 4.) return kFALSE;
  if(fabs(y1-y2) > 4.) return kFALSE;
  Char_t lay=pGeantMdc->getLayer();
  if(lay!=6) {
    Int_t nwires1=segment->getNDrTimes();
    Int_t nwires2=collectWires(pGeantMdc->getSector(),pGeantMdc->getModule(),
        lay,tof2);
    if(nwires2 > nwires1) {
      pGeantMdcPrev=pGeantMdc;
      segment->setAnotherHit(pGeantMdc);
    }
  }
  return kTRUE;
}

Int_t HMdcGeantTrack::collectWires(Char_t sec, Char_t mod, Char_t lay,
    Float_t atof) {
  Int_t nwires=0;
  HMdcEvntListCells* pMdcListCells = HMdcEvntListCells::getExObject();
  if(pMdcListCells) {
    HMdcLayListCells& pLayListCells=(*pMdcListCells)[sec][mod][lay];
    Int_t cell=-1;
    Int_t track1,track2;
    Float_t tof1,tof2;
    UChar_t times;
//printf("-------------------------- trackNumber=%i atof=%f --------------------------\n",trackNumber,atof);
    while( (times=pLayListCells.nextCellSim(cell,track1,track2,tof1,tof2)) ) {
      if(track1!=trackNumber || fabs(atof-tof1)>0.0005) times &= 2;
      if(track2!=trackNumber || fabs(atof-tof2)>0.0005) times &= 1;
//if(track1==trackNumber) printf("track1=%i, tof1=%f, times=%i\n",track1,tof1,times);
      if(times==0) continue;
      segment->setTime(lay+(mod%2)*6,cell,times);
      nwires++;
    }
    segment->setWiresAreColl();
//if(nwires>0) printf("************** nwires=%i\n",nwires);
    return nwires;
  } else if(setMdcCal1Cat()) {
    locMdcCal1.set(3,sec,mod,lay);
    iterMdcCal1->Reset();
    iterMdcCal1->gotoLocation(locMdcCal1);
    HMdcCal1Sim * pMdcCal1Sim;
    while((pMdcCal1Sim=(HMdcCal1Sim*)iterMdcCal1->Next())) {
      Int_t nHits=pMdcCal1Sim->getNHits();
      if(nHits==0) continue;
      UChar_t times=(nHits>0) ? 1:-nHits;
      if(times==2) times=3;
      if((times&1) && (pMdcCal1Sim->getStatus1()<=0 ||
          pMdcCal1Sim->getNTrack1()!=trackNumber ||
          fabs(atof-pMdcCal1Sim->getTof1())>0.0005)) times&=2;
      if((times&2) && (pMdcCal1Sim->getStatus2()<=0 ||
          pMdcCal1Sim->getNTrack2()!=trackNumber ||
          fabs(atof-pMdcCal1Sim->getTof2())>0.0005)) times&=1;
      if(times==0) continue;
      nwires++;
      segment->setTime(lay+(mod%2)*6,pMdcCal1Sim->getCell(),times);
    }
    segment->setWiresAreColl();
  } else segment->setWiresAreNotColl();
  return nwires;
}

void HMdcGeantTrack::unsetMdc(Int_t s, Int_t m) {
  if(s>=0 && s<6 && m>=0 && m<4) mdcSetup[s][m]=kFALSE;
}
  
void HMdcGeantTrack::print(void) {
  // nseg<0 - print all segments
  Float_t ax,ay,az;
  pGeantKine->getVertex(ax,ay,az);
  printf("Tr.%i  %s p=%.3f MeV. Vertex=%.1f/%.1f/%.1f ",trackNumber,
      HPidPhysicsConstants::pid(pGeantKine->getID()),
      pGeantKine->getTotalMomentum(),ax,ay,az);
  if(pGeantKine->isPrimary()) printf("Primary. ");
  else printf("Secondary. ");
  if(isInMdcFlag||isInRichFlag||isInMetaFlag) {
    printf("Present in");
    if(isInRichFlag) printf(" RICH");
    if(isInMdcFlag)  printf(" MDC");
    if(isInMetaFlag) printf(" META");
    printf("\n");
  } else printf("No hits.\n");
  if(nSegments>0) {
    printf(" MDC                 GeantMdc:%3i segments ",nSegments);
    if(mdcSector>=0) printf("in sector %i ",mdcSector+1);
    else  printf("in %i sectors",-mdcSector-1);
    printf("     MdcCal1:");
    if(HMdcEvntListCells::getExObject() || pMdcCal1Cat) {
      if(nWSegments==0) printf(" no hits");
      else {
        printf("%3i segments ",nWSegments);
        if(mdcWSector>=0) printf("in sector %i",mdcWSector+1);
        else printf("in %i sectors",-mdcWSector-1);
      }
    } else printf(" hits not collected");
    printf("\n");
    for(Int_t ns=0;ns<nSegments;ns++)
      ((HMdcGeantSeg*)segments->At(ns+arrOffset))->print();
  }
  printf("\n");
}

void HMdcGeantTrack::printDebug(Float_t dMom,Char_t dMod) {
  if(dMom < -0.01) printDebug(pGeantMdc,nGeantMdcHits,"! P>Pprev.");
  else if(dMom>50. && dMod==0 && segment->getNGMdcHits()>1)
      printDebug(pGeantMdc,nGeantMdcHits,"! Pprev.-P>50");
  else printDebug(pGeantMdc,nGeantMdcHits);
}

void HMdcGeantTrack::printDebug(HGeantMdc* pGeantMdc,Int_t i,Char_t* st) {
  Int_t sec = pGeantMdc->getSector();
  Int_t mod = pGeantMdc->getModule();
  Int_t lay = pGeantMdc->getLayer();
  Int_t lay14=calcLay14(mod,lay);
  Float_t ath, aph;
  pGeantMdc->getIncidence(ath,aph);
  Float_t ax,ay,tof,momLay;
  pGeantMdc->getHit(ax,ay,tof,momLay);
  if(i>=0) printf("%3i)",i);
  else printf("Del.");
  printf(" %3itr. %is %im %2il  l14=%2i t=%7.4f th=%7.3f ph=%7.3f",
   pGeantMdc->getTrack(),sec,mod,lay,lay14,tof,ath,aph);
  printf(" dr=%2i x=%7.1f y=%7.1f p=%.3f",HMdcGeantSeg::dirTheta(ath),
      ax,ay,momLay);
  if(st) printf(" %s",st);
  printf("\n");
}

Int_t HMdcGeantTrack::getNSegsInMdc(Int_t m,Int_t sec) {
  // return number of segments where mdc "m" present in sector "sec"
  // if "sec"<0 - return number of segments where mdc "m" present
  if(!nMdcOk(m) || nSegments==0) return 0;
  Int_t segmdc=m/2;
  Int_t nseg=0;
  for(Int_t nseg=0; nseg<nSegments; nseg++) {
    HMdcGeantSeg * geantSeg =(HMdcGeantSeg*)segments->At(nseg+arrOffset);
    if(geantSeg->getIOSeg()  != segmdc) continue;
    if(sec>=0 && geantSeg->getSec() != sec) continue;
    if(geantSeg->getNGMdcHits(m)) nseg++;
  }
  return nseg;
}

Int_t HMdcGeantTrack::getNSegments(Int_t seg,Int_t sec) {
  // return number of segments where mdc "m" present in sector "sec"
  // if "sec"<0 - return number of segments where mdc "m" present
  if(seg<0 || seg>1 || nSegments==0) return 0;
  Int_t nseg=0;
  for(Int_t nseg=0; nseg<nSegments; nseg++) {
    HMdcGeantSeg * geantSeg =(HMdcGeantSeg*)segments->At(nseg+arrOffset);
    if(geantSeg->getIOSeg()  != seg) continue;
    if(sec>=0 && geantSeg->getSec() != sec)    continue;
    nseg++;
  }
  return nseg;
}

HGeantMdc* HMdcGeantTrack::getMdcMidPlaneHit(Int_t nseg,Int_t mod) {
  if(nSegOk(nseg)) return 
    ((HMdcGeantSeg*)segments->At(nseg+arrOffset))->getMdcMidPlaneHit(mod);
  return 0;
}

HGeantMdc* HMdcGeantTrack::getMdcLayerHit(Int_t nseg,Int_t mod,Int_t lay) {
  if(nSegOk(nseg)) return 
    ((HMdcGeantSeg*)segments->At(nseg+arrOffset))->getMdcLayerHit(mod,lay);
  return 0;
}

Bool_t HMdcGeantTrack::getMdcHitPos(Int_t ns, Int_t m, HGeomVector& hit) {
  // Return geant "hit" position in mid-plane mdc.
  if(nSegOk(ns)) return
    ((HMdcGeantSeg*)segments->At(ns+arrOffset))->getMdcHitPos(m,hit);
  return kFALSE;
}

Bool_t HMdcGeantTrack::getLayerHitPos(Int_t ns, Int_t m, Int_t l, 
    HGeomVector& hit, Bool_t extrFlag) {
  // Return geant "hit" position in mdc layer.
  if(nSegOk(ns)) return ((HMdcGeantSeg*)segments->At(ns+arrOffset))->
      getLayerHitPos(m,l,hit,extrFlag);
  return kFALSE;
}

Bool_t HMdcGeantTrack::getMdcHitPosSec(Int_t ns, Int_t m, HGeomVector& hit) {
  // Return geant "hit" position in mid-plane mdc in sector coor.sys.
  if(nSegOk(ns)) return 
      ((HMdcGeantSeg*)segments->At(ns+arrOffset))->getMdcHitPosSec(m,hit);
  return kFALSE;
}

Bool_t HMdcGeantTrack::getMdcHitPosLab(Int_t ns, Int_t m, HGeomVector& hit) {
  // Return geant "hit" position in mid-plane mdc in lab.coor.sys..
  if(nSegOk(ns)) return 
      ((HMdcGeantSeg*)segments->At(ns+arrOffset))->getMdcHitPosLab(m,hit);
  return kFALSE;
}

Bool_t HMdcGeantTrack::getLayerHitPosSec(Int_t ns, Int_t m, Int_t l, 
    HGeomVector& hit, Bool_t extrFlag) {
  // Return geant "hit" position in mdc layer in sector coor.sys..
  if(nSegOk(ns)) return ((HMdcGeantSeg*)segments->At(ns+arrOffset))->
      getLayerHitPosSec(m,l,hit,extrFlag);
  return kFALSE;
}

Bool_t HMdcGeantTrack::getLayerHitPosLab(Int_t ns, Int_t m, Int_t l, 
    HGeomVector& hit, Bool_t extrFlag) {
  // Return geant "hit" position in mdc layer in lab.coor.sys.
  // If no GeantMdc hit in this layer and extrFlag=kTRUE
  // function does extrapolation to this layer.
  if(nSegOk(ns)) return ((HMdcGeantSeg*)segments->At(ns+arrOffset))->
      getLayerHitPosLab(m,l,hit,extrFlag);
  return kFALSE;
}

// ?????????????????????????????????
// Bool_t HMdcGeantTrack::geantClustPos(Int_t ns, HMdcClusSim* pMdcClusSim, Int_t indtr,
//     Float_t& x, Float_t& y) {
//   if(nSegOk(ns)) return ((HMdcGeantSeg*)segments->At(ns+arrOffset))->
//       geantClustPos(pMdcClusSim,indtr,x,y);
//   return kFALSE;
// }

Char_t HMdcGeantTrack::getSegDirection(Int_t ns) {
  if(nSegOk(ns)) return ((HMdcGeantSeg*)segments->At(ns+arrOffset))->
      getDirection();
  return 0;
}
 
Char_t HMdcGeantTrack::getSector(Int_t ns)       {
  if(nSegOk(ns)) return ((HMdcGeantSeg*)segments->At(ns+arrOffset))->getSec();
  return -1;
}

Char_t HMdcGeantTrack::getIOSeg(Int_t ns)        {
  if(nSegOk(ns)) return ((HMdcGeantSeg*)segments->At(ns+arrOffset))->getIOSeg();
  return -1;
}

Char_t HMdcGeantTrack::getFirstLayer12(Int_t ns)   {
  if(nSegOk(ns)) return ((HMdcGeantSeg*)segments->At(ns+arrOffset))->
      getFirstLayer12();
  return -1;
}

Char_t HMdcGeantTrack::getLastLayer12(Int_t ns)    {
  if(nSegOk(ns)) return ((HMdcGeantSeg*)segments->At(ns+arrOffset))->
      getLastLayer12();
  return -1;
}

Char_t HMdcGeantTrack::getNGMdcHits(Int_t ns)    {
  if(nSegOk(ns)) return ((HMdcGeantSeg*)segments->At(ns+arrOffset))->
      getNGMdcHits();
  return 0;
}

void HMdcGeantTrack::calcNSectors(void) {
  if(nSegments==0) return;
  Short_t secList = 0;
  Int_t sec       = -1;
  for(Int_t ns=0;ns<nSegments;ns++) {
    HMdcGeantSeg* geantSeg =(HMdcGeantSeg*)segments->At(ns+arrOffset);
    if(geantSeg->getDirection()<1) directionFlag=kFALSE;
    nIOSeg |= (1<<geantSeg->getIOSeg());
    sec=geantSeg->getSec();
    Short_t tst=1<<sec;
    if(secList&tst) continue;
    secList |= tst;
    mdcSector--;
  }
  if(mdcSector==-2) mdcSector=sec;
  
  if(HMdcEvntListCells::getExObject()==0 && pMdcCal1Cat==0) return;
  secList = 0;
  sec     = -1;
  nWires  = 0;
  for(Int_t ns=0;ns<nSegments;ns++) {
    HMdcGeantSeg* geantSeg =(HMdcGeantSeg*)segments->At(ns+arrOffset);
    Int_t nDrTm=geantSeg->getNDrTimes();
    if(nDrTm==0) continue;
    nWires += nDrTm;
    nWIOSeg |= (1<<geantSeg->getIOSeg());
    sec=geantSeg->getSec();
    Short_t tst=1<<sec;
    nWSegments++;
    if(secList&tst) continue;
    secList |= tst;
    mdcWSector--;
  }
  if(mdcWSector==-2) mdcWSector=sec;
}

HMdcGeantTrack* HMdcGeantEvent::next(Int_t& i) {
  i=(i<0) ? 0:i+1;
  if(i>=nTracks || nTracks==0) return 0;
  return (HMdcGeantTrack*)At(i);
}

HMdcGeantTrack* HMdcGeantEvent::getGeantTrack(Int_t trackNum) {
  if(nTracks <= 0 || trackNum<=0) return 0;
  Int_t nabove = nTracks+1;
  Int_t nbelow = 0;
  while(nabove-nbelow > 1) {
     Int_t middle = (nabove+nbelow)/2;
     HMdcGeantTrack* pTrack=(HMdcGeantTrack*)At(middle-1);
     Int_t track = pTrack->getTrack();
     if(trackNum == track) return pTrack;
     if(trackNum  < track) nabove = middle;
     else                  nbelow = middle;
  }
  return 0; 
//   for(Int_t i=0;i<nTracks;i++) {
//     HMdcGeantTrack* pTrack=(HMdcGeantTrack*)At(i);
//     if(trackNum == pTrack->getTrack()) return pTrack;
//   }
//   return 0;
}

//===========================================================================

HMdcGeantEvent* HMdcGeantEvent::pGlobalGEvent=0;

HMdcGeantEvent* HMdcGeantEvent::getObject(void) {
  // In task call this function not earlier then in your init function.
  // In macro first time call this function after 
  // HMdcGetContainers::setCatGeantKine(HCategory* cat);
  // HMdcGetContainers::setCatGeantMdc(HCategory* cat);
  if(pGlobalGEvent==0) {
    if(HMdcGetContainers::getCatGeantKine() == 0) return 0;
    if(HMdcGetContainers::getCatGeantMdc()  == 0) return 0;
    pGlobalGEvent=new HMdcGeantEvent();
  }
  return pGlobalGEvent;
}

void HMdcGeantEvent::deleteCont(void) {
  if(pGlobalGEvent) delete pGlobalGEvent;
  pGlobalGEvent = 0;
}
    
HMdcGeantEvent::HMdcGeantEvent(void) {
  size           = 0;
  nTracks        = 0;
  pGeantKineCat  = 0;
  pGeantMdcCat   = 0;
//  pMdcCal1Cat    = 0;
  pMdcListCells  = 0;
  debugPrintFlag = kFALSE;
  nGSegments     = 0;
  sizeGSegArr    = 0;
  isMdcLCellsOwn = kFALSE;
  HMdcGeantTrack::testMdcSetup();
}

HMdcGeantEvent::~HMdcGeantEvent(void) {
  // destructor
  Delete();
  HMdcEvntListCells::deleteCont();
  geantSegments.Delete();
}

Bool_t HMdcGeantEvent::setGeantKineCat(void) {
  HCategory* cat = HMdcGetContainers::getCatGeantKine();
  if(pGeantKineCat && cat==pGeantKineCat) return kTRUE;
  pGeantKineCat=cat;
  if(pGeantKineCat==0) {
    Error("setGeantKineCat"," Can't get catGeantKine category!");
    return kFALSE;
  }
  return kTRUE;
}

// Bool_t HMdcGeantEvent::setGeantMdcCat(HCategory* cat)  {
//   
//   if(cat) pGeantMdcCat=cat;
//   if(pGeantMdcCat) return kTRUE;
//   if(gHades)
//       pGeantMdcCat = gHades->getCurrentEvent()->getCategory(catMdcGeantRaw);
//   if(pGeantMdcCat) return kTRUE;
//   Error("setGeantMdcCat","Can't get catMdcGeantRaw category!");
//   return kFALSE;
// }

// Bool_t HMdcGeantEvent::setMdcCal1Cat(HCategory* cat) {
//   
//   if(pMdcListCells) {
//     if(cat && isMdcLCellsOwn) pMdcListCells->setMdcCal1Cat(cat);
//     return kTRUE;
//   }
//   HCategory* pMdcCal1Cat=cat;
//   if(pMdcCal1Cat==0 && gHades==0) return kFALSE;
//   pMdcCal1Cat = gHades->getCurrentEvent()->getCategory(catMdcCal1);
//   if(pMdcCal1Cat==0) return kFALSE;
//   pMdcListCells = HMdcEvntListCells::getExObject();
//   if(pMdcListCells==0) {
//     pMdcListCells  = getObject();
//     isMdcLCellsOwn = kTRUE;
//     if(cat) pMdcListCells->setMdcCal1Cat(pMdcCal1Cat);
//   }
//   
//   
//   
//       
//   if(cat) pMdcCal1Cat=cat;
//   else if(pMdcCal1Cat==0 && gHades)
//       pMdcCal1Cat = gHades->getCurrentEvent()->getCategory(catMdcCal1);
//   if(pMdcCal1Cat==0) return kFALSE;
//   
//   if(pMdcListCells==0) pMdcListCells = HMdcEvntListCells::getExObject();
//   if(pMdcListCells==0) {
//     pMdcListCells  = getObject();
//     isMdcLCellsOwn = kTRUE;
//     if(cat) pMdcListCells->setMdcCal1Cat(pMdcCal1Cat);
//   }
//   return kTRUE;
// }

Bool_t HMdcGeantEvent::collectTracks(void) {
  nTracks      = 0;
  nGSegments   = 0;
  geantBugFlag = kFALSE;
  if(pMdcListCells==0) {
    pMdcListCells = HMdcEvntListCells::getExObject();
    if(pMdcListCells==0) {
      pMdcListCells  = HMdcEvntListCells::getObject();
      isMdcLCellsOwn = kTRUE;
    }
  }
  if(isMdcLCellsOwn) pMdcListCells->collectWires();
  if(!setGeantKineCat()) return kFALSE;
  HMdcGeantTrack* pGeantTrack;
  Int_t nTrks = pGeantKineCat->getEntries();
  for(Int_t trk=0;trk<nTrks;trk++) {
    HGeantKine* pGeantKine = (HGeantKine*)pGeantKineCat->getObject(trk);
    if(pGeantKine == 0) continue;
    if(nTracks<size) pGeantTrack=(HMdcGeantTrack*)At(nTracks);
    else {
      pGeantTrack=new HMdcGeantTrack(&geantSegments,&sizeGSegArr,&nGSegments);
      AddAtAndExpand(pGeantTrack, nTracks);
      size++;
//!      if(!pGeantTrack->setGeantMdcCat(pGeantMdcCat)) return kFALSE;
//!      pGeantTrack->setGeantKineCat(pGeantKineCat);
//!      pGeantTrack->setMdcEvntListCells(pMdcListCells); //????????????
    }
    pGeantTrack->setDebugPrintFlag(debugPrintFlag);
    nGSegments += pGeantTrack->testGeantTrack(pGeantKine);
    if(pGeantTrack->isGeantBug()) geantBugFlag=kTRUE;
    if(debugPrintFlag) pGeantTrack->print();
    if(!pGeantTrack->isInMdc()) continue;
    if(pGeantTrack->getNWires()==0) continue;
    nTracks++;
  }
  return kTRUE;
}

void HMdcGeantEvent::print(void) {
  for(Int_t i=0;i<nTracks;i++) ((HMdcGeantTrack*)At(i))->print();
}

//===========================================================================

void HMdcTrackInfSim::fillClusTrackInf(HMdcClusSim* pClusSim) {
  sector        = pClusSim->getSec();
  segment       = pClusSim->getIOSeg();
  listTmp       = *((HMdcList12GroupCells*)pClusSim);
  modInd        = -1;
  isWrCollected = kFALSE;
  
  collectTracksInf();
  
  if( !isWrCollected ) pClusSim->calcTrList(); // old calculation
  else {
    Int_t maxNTrcks = pClusSim->getArrsSize();
    Int_t nTrcks = (numTracks < maxNTrcks) ? numTracks : maxNTrcks;
    Int_t nSettedTrcks = 0;
    for(Int_t n=0; n<nTrcks; n++) {
      Int_t ind = sortedInd[n];
      Int_t bin = (ind == noiseInd) ? maxNTrcks-1 : nSettedTrcks++;
      pClusSim->setTrack(bin,tracksNum[ind],numWires[ind],
          wiresList[ind].getListLayers(0),
          wiresList[ind].getListLayers(1));
    }
    pClusSim->setNTracks(nSettedTrcks);
  }
}

void HMdcTrackInfSim::addClusModTrackInf(HMdcClusSim* pClusSim) {
  // This function can be used ONLY after 
  // fillClusTrackInf(HMdcClusSim* pClusSim) for the same pClusSim!!!
  if( !isWrCollected) {
    pClusSim->calcTrListMod(*pClusSim,0); // old calculation
    pClusSim->calcTrListMod(*pClusSim,1); // old calculation
  } else {
    Short_t nWires[numTracks];
    Int_t sInd[numTracks];
    for(Int_t m=0;m<2;m++) {
      for(Int_t tr=0;tr<numTracks;tr++) nWires[tr] = 
                                        wiresList[tr].getNDrTimesMod(m);
      TMath::Sort(numTracks, nWires, sInd);
      Int_t maxTrcks     = pClusSim->getArrsSize();
      Int_t nSettedTrcks = 0;
      for(Int_t n=0; n<numTracks && nSettedTrcks<maxTrcks; n++) {
        Int_t ind = sInd[n];
        if(nWires[ind] == 0) break;
        pClusSim->setTrackM(m,nSettedTrcks,tracksNum[ind],nWires[ind]);
        nSettedTrcks++;
      }
      pClusSim->setNTracksM(m,nSettedTrcks);
    }
  }
}

void HMdcTrackInfSim::fillClusModTrackInf(HMdcClusSim* pClusSim,
    HMdcList12GroupCells* wrLst, Int_t modi) {
  // sector and segment in pClusSim must be setted already!
  isWrCollected = kFALSE;
  if(modi<0 || modi>1) return;
  sector    = pClusSim->getSec();
  segment   = pClusSim->getIOSeg();
  listTmp   = *wrLst;
  modInd    = modi;
  
  collectTracksInf();
  
  if( !isWrCollected ) pClusSim->calcTrListMod(*wrLst,modInd); // old calc.
  else {
    Int_t maxNTrcks    = pClusSim->getArrsSize();
    Int_t nSettedTrcks = (numTracks<maxNTrcks) ? numTracks : maxNTrcks;
    for(Int_t n=0; n<nSettedTrcks; n++) {
      Int_t ind = sortedInd[n];
      pClusSim->setTrackM(modInd,n,tracksNum[ind],numWires[ind]);
    }
    pClusSim->setNTracksM(modInd,nSettedTrcks);
  }
}

void HMdcTrackInfSim::fillSegTrackInf(HMdcSegSim* pSegSim,
    HMdcList24GroupCells* wrLst) {
  isWrCollected = kFALSE;
  sector        = pSegSim->getSec();
  segment       = pSegSim->getIOSeg();
  if(!wrLst->getSeg(listTmp,segment)) return;
  modInd        = -1;

  collectTracksInf();

  if( !isWrCollected ) pSegSim->calcNTracks(); // old calculation
  else {
    Int_t   segListTr[numTracks];
    UChar_t segNTimes[numTracks];
    for(Int_t n=0; n<numTracks; n++) {
      Int_t ind    = sortedInd[n];
      segListTr[n] = tracksNum[ind];
      segNTimes[n] = (numWires[ind] < 256) ? numWires[ind] : 255;
    }
    pSegSim->setNTracks(numTracks,segListTr,segNTimes);
  }
}

void HMdcTrackInfSim::fillHitTrackInf(HMdcHitSim* pHitSim,
    HMdcList24GroupCells* wrLst) {
  isWrCollected = kFALSE;
  sector        = pHitSim->getSector();
  modInd        = pHitSim->getModule()%2;
  segment       = pHitSim->getModule()/2;
  if(!wrLst->getSeg(listTmp,segment)) return;

  collectTracksInf();

  if( !isWrCollected ) pHitSim->calcNTracks(); // old calculation
  else {
    Int_t   segListTr[numTracks];
    UChar_t segNTimes[numTracks];
    for(Int_t n=0; n<numTracks; n++) {
      Int_t ind    = sortedInd[n];
      segListTr[n] = tracksNum[ind];
      segNTimes[n] = (numWires[ind] < 256) ? numWires[ind] : 255;
    }
    pHitSim->setNTracks(numTracks,segListTr,segNTimes);
  }
}

Bool_t HMdcTrackInfSim::collectTracksInf(UChar_t sec,UChar_t seg,
    HMdcList12GroupCells* wrLst) {
  sector        = sec;
  segment       = seg;
  listTmp       = *wrLst;
  modInd        = -1;
  isWrCollected = kFALSE;
  
  if(!collectTracksInf()) return kFALSE;
  return kTRUE;
}

Bool_t HMdcTrackInfSim::collectTracksInf(void) {
  HMdcEvntListCells* pMdcListCells = HMdcEvntListCells::getExObject();
  if(pMdcListCells == 0) return kFALSE;
  HMdcGeantEvent* pGeantEvent = HMdcGeantEvent::getExObject();
  if(pGeantEvent == 0) return kFALSE;
  
  HMdcSecListCells& fSecListCells = (*pMdcListCells)[sector];
  Int_t embRealTrack   = gHades->getEmbeddingRealTrackId();
  numTracks      = 0;
  embedInd = -1;
  noiseInd = -1;

  Int_t layStr = (modInd != 1) ?  0 : 6;
  Int_t layEnd = (modInd != 0) ? 12 : 6;
  for(Int_t lay=layStr; lay<layEnd; lay++) {
    Int_t mod    = lay/6 + segment*2;
    Int_t cell   = -1;
    UChar_t nTms = 0;
    HMdcLayListCells& fLayListCells = fSecListCells[mod][lay%6];
    while((cell=listTmp.next(lay,cell,nTms)) >= 0) {
      for(Int_t tm=1; tm<3; tm++) {
        Int_t track = fLayListCells.getGeantTrack(cell,tm);
        if(track == 0) continue;
        if(track<0 || track == embRealTrack) {
          // Embeded real track and noise --------------------------------------
          if(track != embRealTrack) track = -99;
          listTmp.delTime(lay,cell,tm);
          Int_t& ind = (track == embRealTrack) ? embedInd : noiseInd;
          if(ind < 0) {
            if(numTracks < aSize) ind = numTracks++;
            else {
              for(Int_t i=0;i<aSize;i++) if(i!=noiseInd && i!=embedInd) {
                if(ind<0 || numWires[i]<numWires[ind]) ind = i;
              }
            }
            wiresList[ind].clear();
            tracksNum[ind]  = track;
            gntSegList[ind] = 0;
            numWires[ind]   = 1;
          } else numWires[ind]++;
          wiresList[ind].setTime(lay,cell,tm);
        } else {
          // Geant track -------------------------------------------------------
          HMdcGeantTrack* pGeantTrack = pGeantEvent->getGeantTrack(track);
          if(pGeantTrack == 0) continue; // Geant bug;
          Short_t nGeantSegs = pGeantTrack->getNSegments();
          for(Int_t segn=0;segn<nGeantSegs;segn++) {
            HMdcGeantSeg* pGeantSeg = pGeantTrack->getSegment(segn);
            if( pGeantSeg->getSec()   != sector || 
                pGeantSeg->getIOSeg() != segment ) continue;
            numWires[numTracks] = 
                listTmp.compareAndUnset(pGeantSeg,wiresList+numTracks,modInd);
            if(numWires[numTracks] == 0) continue;
            Int_t ind = numTracks;
            if(numTracks < aSize) numTracks++;
            else {
              for(Int_t i=0;i<aSize;i++) if(i!=noiseInd && i!=embedInd) {
                if(numWires[i] < numWires[ind]) ind = i;
              }
              if(ind == aSize) continue;
              wiresList[ind]  = wiresList[numTracks];
              numWires[ind]   = numWires[numTracks];
            }
            tracksNum[ind]  = track;
            gntSegList[ind] = pGeantSeg;
          }
        }
      }
    }
  }

  isGntBugEvent = pGeantEvent->isGeantBug();
  numWires[numTracks] = listTmp.getNDrTimes(layStr,layEnd-1);
  if(numWires[numTracks] == 0) isGntBugSeg = kFALSE;
  else {
    isGntBugSeg = kTRUE;
    Int_t ind = numTracks;
    if(numTracks < aSize) numTracks++;
    else {
      for(Int_t i=0;i<aSize;i++) if(i!=noiseInd && numWires[i]<numWires[ind])
                                                                     ind = i;
      if(ind == embedInd) embedInd = -1;
      numWires[ind]  = numWires[numTracks];
    }
    tracksNum[ind]  = -9;  // GEANT bug!
    wiresList[ind]  = listTmp;
    gntSegList[ind] = 0;
  }
  
  TMath::Sort(numTracks, numWires, sortedInd);
  
// printf("&&& ");
// for(Int_t j=0;j<numTracks;j++) {
//   Int_t i = sortedInd[j];
//   printf(" %i(%i)[%i:%i]",tracksNum[i],numWires[i],
//   HMdcBArray::getNSet(nLayers[i][0]),HMdcBArray::getNSet(nLayers[i][1]));
// }
// 
// printf("\n");
// if(isGntBugSeg){
// printf("ZZZZZZZZZZZZZZZZZZZZZ isGntBugSeg=%i   isGeantBug=%i\n",isGntBugSeg,isGntBugEvent);
// // // listTmp.print();
// //   for(Int_t lay=0; lay<12; lay++) {
// //     Int_t modInd = lay/6;
// //     Int_t mod    = modInd+segment*2;
// //     Int_t cell   = -1;
// //     Short_t add  = 1<<(lay%6);
// //     UChar_t nTms = 0;
// //     HMdcLayListCells& fLayListCells = fSecListCells[mod][lay%6];
// //     while((cell=listTmp.next(lay,cell,nTms)) >= 0) {
// //       Int_t track = fLayListCells.getGeantTrack(cell,1);
// //       if(track<=0) continue;
// //       HMdcGeantTrack* pGeantTrack = getGeantTrack(track);
// //       Short_t nGeantSegs = pGeantTrack->getNSegments();
// //       for(Int_t segn=0;segn<nGeantSegs;segn++) {
// //         HMdcGeantSeg* pGeantSeg = pGeantTrack->getSegment(segn);
// //         if(pGeantSeg->getSec()   != sector) continue;
// //         if(pGeantSeg->getIOSeg() != segment) continue;
// //         pGeantSeg->print();
// //         ((HMdcList12GroupCells*)pGeantSeg)->print();
// //       }
// //       
// //     }
// //   }
// // printf("ZZZZZZZZZZZZZZZZZZZZZ\n");
// }
  
  isWrCollected = kTRUE;
  return kTRUE;
}
    
Int_t HMdcTrackInfSim::getTrack(Int_t trInd) const {
  if(trInd>=0 && trInd<numTracks) return 0;
  return tracksNum[sortedInd[trInd]];
}

Short_t HMdcTrackInfSim::getNumWires(Int_t trInd) const {
  if(trInd>=0 && trInd<numTracks) return 0;
  return numWires[sortedInd[trInd]];
}

UChar_t HMdcTrackInfSim::getNumLayers(Int_t trInd, Int_t modi) const {
  if(trInd>=0 && trInd<numTracks) return 0;
  Int_t indx = sortedInd[trInd];
  if(modi==0 || modi==1) 
      return HMdcBArray::getNSet(wiresList[indx].getListLayers(modi));
  return HMdcBArray::getNSet(wiresList[indx].getListLayers(0))+
         HMdcBArray::getNSet(wiresList[indx].getListLayers(1));
}

HMdcList12GroupCells* HMdcTrackInfSim::getListCells(Int_t trInd) {
  if(trInd>=0 && trInd<numTracks) return 0;
  return &(wiresList[sortedInd[trInd]]);
}

HMdcGeantSeg* HMdcTrackInfSim::getMdcGeantSeg(Int_t trInd) {
  if(trInd>=0 && trInd<numTracks) return 0;
  return gntSegList[sortedInd[trInd]];
}
