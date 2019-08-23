#include "hmdcclustfromseg.h"
#include "hades.h"
#include "hevent.h"
#include "hiterator.h"
#include "hmdctrackddef.h"
#include "hmdctrackgdef.h"
#include "hmdcclus.h"
#include "hmetamatch.h"
#include "hsplinetrack.h"
#include "hmdcseg.h"
#include "hmdctrkcand.h"
#include "hmdchit.h"
#include "hmdcclusinf.h"
#include "hmdcgetcontainers.h"
#include "hmdcsizescells.h"
#include "hparamlist.h"

ClassImp(HMdcClustFromSeg)

HMdcClustFromSeg::HMdcClustFromSeg() {
  clear();
  initCut(NULL);
}

HMdcClustFromSeg::HMdcClustFromSeg(Text_t *name,Text_t *title) : 
    HReconstructor(name,title) {
  clear();
  initCut(NULL);
}

HMdcClustFromSeg::HMdcClustFromSeg(Text_t *name,Text_t *title, HParamList *cut) : 
    HReconstructor(name,title) {
  clear();
  initCut(cut);
}

void HMdcClustFromSeg::initCut(HParamList* cut) {
  if(cut) {
    // use user defined cut parameter
    cut->fill("min_momentum",&fMinMomentum);
    cut->fill("min_beta",    &fMinBeta);
    cut->fill("max_beta",    &fMaxBeta);
    cut->fill("polarity",    &fPolarity);
    // some basic checks - without a strong physical meaning
    if(fMinMomentum<0 || fMinMomentum>2000 ||
       fMinBeta    <0 || fMinBeta>fMaxBeta ||
       fMaxBeta    >2 || fPolarity==0) {
      ERROR_msg( 1, "Invalid cut parameter set!" );
      exit( 1 ); // uhgg ... but there is no "gHades->terminate()"
    }
  } else {
    // by default select pi-
    fMinMomentum=250.0;
    fMinBeta    =0.88;
    fMaxBeta    =1.12;
    fPolarity   =-1;
  }
  cout << "==> Cluster from Segment Filler - Cut Parameter:" << "\n"
       << "==>   min_momentum = " << fMinMomentum << "\n" 
       << "==>   min_beta     = " << fMinBeta     << "\n"
       << "==>   max_beta     = " << fMaxBeta     << "\n"
       << "==>   polarity     = " << fPolarity    << endl;
}

void HMdcClustFromSeg::clear(void) {
  pCatMetaMatch=0;
  iterMetaMatch=0;
  pCatSplineTrack=0;
  pCatMdcSeg=0;
  pCatMdcTrkCand=0;
  pSizesCells=0;
  pCatMdcHit=0;
}

HMdcClustFromSeg::~HMdcClustFromSeg() {
  // destructor 
  if(iterMetaMatch) delete iterMetaMatch;
}


Bool_t HMdcClustFromSeg::init(void) {
  HMdcGetContainers* fGetCont = HMdcGetContainers::getObject();
  pCatMetaMatch   = gHades->getCurrentEvent()->getCategory(catMetaMatch);
  pCatMdcSeg      = gHades->getCurrentEvent()->getCategory(catMdcSeg);
  pCatSplineTrack = gHades->getCurrentEvent()->getCategory(catSplineTrack);
  pCatMdcTrkCand  = fGetCont->getCatMdcTrkCand();
  pCatMdcHit      = gHades->getCurrentEvent()->getCategory(catMdcHit);
  pCatMdcClusInf  = fGetCont->getCatMdcClusInf();
  if(!pCatMetaMatch || !pCatSplineTrack || !pCatMdcSeg || !pCatMdcTrkCand ||
      !pCatMdcHit || !pCatMdcClusInf) return kFALSE;
  iterMetaMatch=(HIterator*)pCatMetaMatch->MakeIterator();
  if(!iterMetaMatch) return kFALSE;
  
  pCatMdcClus = fGetCont->getCatMdcClus(kTRUE);
  if(!pCatMdcClus) return kFALSE;
  pSizesCells=HMdcSizesCells::getObject();
  if(!pSizesCells) return kFALSE;
  return kTRUE;
}

Bool_t HMdcClustFromSeg::reinit(void) {
  if(!pSizesCells->initContainer()) return kFALSE;
  return kTRUE;
}

Bool_t HMdcClustFromSeg::finalize(void) {
  return kTRUE;
}

Int_t HMdcClustFromSeg::execute(void) {
  iterMetaMatch->Reset();
  HMetaMatch* pMetaMatch;

  // clear cluster category if there
  // was running the rackfinder before
  pCatMdcClus->Clear();

  while((pMetaMatch=(HMetaMatch*)iterMetaMatch->Next()) !=0) {
    if(!pMetaMatch->isSplineAccepted()) continue;
    if(pMetaMatch->getNCandForRich() > 0) continue;
    if(pMetaMatch->getNCandForIPU() > 0) continue;
    Int_t system = pMetaMatch->getSystem();
    if(system<0) continue;
    Short_t spInd = pMetaMatch->getSplineInd();
    if(spInd<0) continue;
    HSplineTrack* pSplineTrack=(HSplineTrack*)pCatSplineTrack->getObject(spInd);
    Char_t pol=pSplineTrack->getPolarity();
    Float_t beta = pSplineTrack->getBeta();
    Float_t mom  = pSplineTrack->getP();

    // apply the cuts - for defaults, see constructor
    if(!fPolarity == 2){
	if (fPolarity > 0 && pol <  0) continue;
	if (fPolarity < 0 && pol >= 0) continue;
    }
    if (beta<fMinBeta || beta>fMaxBeta) continue;
    if (mom<fMinMomentum) continue;
      
    Short_t trkCndInd = pMetaMatch->getTrkCandInd();
    HMdcTrkCand* pMdcTrkCand=(HMdcTrkCand*)pCatMdcTrkCand->getObject(trkCndInd);
    fillCluster(pMdcTrkCand->getSeg1Ind());
    fillCluster(pMdcTrkCand->getSeg2Ind());
  }
  return 0;
}

void HMdcClustFromSeg::fillCluster(Short_t segInd) {
  if(segInd<0) return;
  HMdcSeg* pSeg=(HMdcSeg*)pCatMdcSeg->getObject(segInd);
  if(!pSeg) return;
  Int_t nLay[2]={0,0};
  for(Int_t l=0;l<12;l++) if(pSeg->getNCells(l)>0) nLay[l/6]++;
  if(nLay[0]<5 && nLay[1]<5) return;
  
  for(Int_t m=0;m<2;m++) {
    if(nLay[m]<5) continue;
    Int_t hitInd = pSeg->getHitInd(m);
    if(hitInd<0) {nLay[m]=0; continue;}
    HMdcHit* pHit=(HMdcHit*)pCatMdcHit->getObject(hitInd);
    if(pHit==0) {nLay[m]=0; continue;}
    Short_t clusInfIndex = pHit->getClusInfIndex();
    if(clusInfIndex<0) {nLay[m]=0; continue;}
    HMdcClusInf* pClusInf=(HMdcClusInf*)pCatMdcClusInf->getObject(clusInfIndex);
    if(pClusInf==0) {nLay[m]=0; continue;}
    Char_t levelClFinding = pClusInf->getLevelClFinding();
if(levelClFinding>5) nLay[m]=0; //!!!!!!!!!!!!!!
  }
  if(nLay[0]<5 && nLay[1]<5) return;
  
  HMdcClus* pClus=getClusterSlot(pSeg);
  Int_t l1=(nLay[0]>=5) ? 0:6;
  Int_t l2=(nLay[1]>=5) ? 12:6;
  pClus->setMinCl(l1==0 ? 5:0, l2==12 ? 5:0);
  if(l1!=0 || l2!=12) {
    pClus->setTypeClFinder(1);
    Int_t mod = pSeg->getIOSeg()*2;
    pClus->setMod((l1==0) ? mod:mod+1);
  }
  for(Int_t layer=l1;layer<l2;layer++) {
    Int_t nCells=pSeg->getNCells(layer);
    if(nCells==0) continue;
    for(Int_t n=0; n<nCells; n++) {
      UChar_t nCTimes=pSeg->getSignId(layer,n);
      Int_t cell=pSeg->getCell(layer,n);
      pClus->setTime(layer,cell,nCTimes);
    }
  } 
}

HMdcClus* HMdcClustFromSeg::getClusterSlot(HMdcSeg* pSeg) {
  Int_t sector = pSeg->getSec();
  HMdcSizesCellsSec& fSCSec=(*pSizesCells)[sector];
  if(&fSCSec==0) return 0;
  Int_t seg    = pSeg->getIOSeg();
  HMdcSizesCellsMod* pSCMod1=&(fSCSec[seg*2]);
  HMdcSizesCellsMod* pSCMod2=&(fSCSec[seg*2+1]);
  if(pSCMod1==0 && pSCMod2==0) return 0;
  if(pSCMod2==0) pSCMod2=pSCMod1;
  else if(pSCMod1==0) pSCMod1=pSCMod2;
  locClus.set(2,sector,seg);
  Int_t index;
  HMdcClus* pClus = (HMdcClus*)pCatMdcClus->getNewSlot(locClus,&index);
  if(!pClus) {
    Warning("getClusterSlot","Sec.%i Segment %i No slot HMdcClus available",
        sector+1,seg+1);
    return 0;
  }
  pClus=new(pClus) HMdcClus();
  
  pClus->setAddress(sector,seg,-1); //!!!!!!!!!!!!!!!!!!!!!!!!!!
  
  HMdcSizesCellsLayer& fSCLay1=(*pSCMod1)[0];
  HMdcSizesCellsLayer& fSCLay2=(*pSCMod2)[5];
  trackLine.setFirstPlane(&fSCLay1);
  trackLine.setSecondPlane(&fSCLay2);
  trackLine.setSegmentLine(pSeg->getZ(),pSeg->getR(),
      pSeg->getTheta(),pSeg->getPhi());
  
  pClus->setTarget(trackLine.x1(),0.,trackLine.y1(),0.,trackLine.z1(),0.);
  pClus->setPrPlane(fSCLay2.A(),fSCLay2.B(),fSCLay2.D());
  pClus->setXY(trackLine.x2(),0.,trackLine.y2(),0.);
  return pClus;
}