//*--- Author : G.Agakichiev
//*--- Modified: 23.01.07 V.Pechenov
//*--- Modified: 16.06.2005 by V.Pechenov
//*--- Modified: 21.07.2003 by V.Pechenov
//*--- Modified: 28.07.2002 by V.Pechenov
//*--- Modified: 07.05.2002 by V.Pechenov

using namespace std;
#include "hmdctrackfitter.h"
#include "hdebug.h"
#include "hades.h"
#include "TStopwatch.h"
#include "hmdctrackfitpar.h"
#include "hruntimedb.h"
#include "hmdcsizescells.h"
#include "hgeomvector.h"
#include "hmdccal2parsim.h"
#include "hmdccal2par.h"
#include "hmdcgetcontainers.h"
#include "hmdclistcells.h"
#include "hmdcwirefitsim.h"
#include "hmdcclusfitsim.h"
#include "hmdcclus.h"
#include "hmdctrackdset.h"
#include "hmdcdigitpar.h"

//_HADES_CLASS_DESCRIPTION 
//////////////////////////////////////////////////////////////////////////////
// 
// HMdcTrackFitInOut
//
// Service class for for Dubna track piece fitters 
//
//
// HMdcTrackFitter
//
// Base class for Dubna track piece fitters
//
//////////////////////////////////////////////////////////////////////////////

ClassImp(HMdcTrackFitInOut)
ClassImp(HMdcTrackFitter)

HMdcTrackFitInOut::HMdcTrackFitInOut(void) {
  fSizesCells=0;
  setDefaultFitParam();
}

Bool_t HMdcTrackFitInOut::init(void) {
  // parameters:
  version=HMdcTrackDSet::getFitVersion();
  if(version==0) {
    Warning("init",
        "track fit version 0 is not supported more, version 1 will used");
    version=1;
  }
  fitPar=(HMdcTrackFitPar*)gHades->getRuntimeDb()->
      getContainer("MdcTrackFitPar");
  fprint=HMdcTrackDSet::fPrint();
  
  fCal2ParSim=(HMdcCal2ParSim*)gHades->getRuntimeDb()->
      getContainer("MdcCal2ParSim");
  fCal2Par=(HMdcCal2Par*)gHades->getRuntimeDb()->getContainer("MdcCal2Par");
  if(!fCal2ParSim || !fCal2Par) return kFALSE;
  HMdcWireData::setCal2Par(fCal2Par);
  HMdcWireData::setDriftTimePar(&driftTimePar);
  wireOffsetFlag=HMdcTrackDSet::getUseWireOffset();
  if(wireOffsetFlag) {
    fDigitPar=(HMdcDigitPar*)gHades->getRuntimeDb()->getContainer("MdcDigitPar");
    if(!fDigitPar) {
      Error("init:","Zero pointer for HMdcDigitPar recieved!");
      return kFALSE;
    }
  } else fDigitPar=0;
  
  fSizesCells=HMdcSizesCells::getObject();
  if (!fSizesCells) {
    Error("init","HMdcSizesCells is absent");
    return kFALSE;
  }
  
  // categoryes:
  geantFlag=HMdcGetContainers::isGeant();
  if(geantFlag && HMdcTrackDSet::fNTuple()) {
    fGeantKineCat=gHades->getCurrentEvent()->getCategory(catGeantKine);
    fGeantMdcCat=gHades->getCurrentEvent()->getCategory(catMdcGeantRaw);
  } else {
    fGeantKineCat=0;
    fGeantMdcCat=0;
  }
  fCalCat=HMdcGetContainers::getCatMdcCal1();
  if (!fCalCat) return kFALSE;
  if(HMdcTrackDSet::fNTuple()) {
    fClusFitCat = HMdcGetContainers::getCatMdcClusFit(kTRUE);
    fWireFitCat = HMdcGetContainers::getCatMdcWireFit(kTRUE);
    if(!fClusFitCat || !fWireFitCat) return kFALSE;
  } else {
    fClusFitCat = 0;
    fWireFitCat = 0;
  }
  locClusFit.set(1,0);
  locWireFit.set(1,0);
  loc.set(4,0,0,0,0);
  return kTRUE;
}

Bool_t HMdcTrackFitInOut::reinit(void) {
  if(!fSizesCells->initContainer()) return kFALSE;
  if(!driftTimePar.initContainer()) return kFALSE;
  for(Int_t sec=0; sec<6; sec++) {
    HMdcSizesCellsSec& fSCSec=(*fSizesCells)[sec];
    for(Int_t mod=0; mod<4; mod++)
        fSCModAr[sec][mod]=(fSizesCells->modStatus(sec,mod)) ? &fSCSec[mod] : 0;
  }
  if( !HMdcGetContainers::isInited(fCal2ParSim) ||
      !HMdcGetContainers::isInited(fCal2Par) ) return kFALSE;
  if( !HMdcGetContainers::isInited(fitPar) ) return kFALSE;
  if(wireOffsetFlag) {
    if( !HMdcGetContainers::isInited(fDigitPar) ) return kFALSE;
    signalSpeed=fDigitPar->getSignalSpeed();
  } else signalSpeed=0.;
  return kTRUE;
}

void HMdcTrackFitInOut::setDefaultFitParam(void) {
  // setting default parameters of track fitter
  fprint         = kFALSE;     
  printStartEv   = 0;
  nEventPrint    = 1000000000; 

  version        = 1;
  geantFlag      = kFALSE;
  wireOffsetFlag = 0;
  signalSpeed    = 0.005;
  useTukeyFlag   = kTRUE;
  cutWeight      = 0.01;
  tofFlag        = 3;
  doTargScan     = kTRUE;
  
  minTimeOffset  = -30.;       // Time offset cut
  maxTimeOffset  = 60.;        // -/-
  minCellsNum    = 5;
  
  chi2CutFlag    = kTRUE;      // kTRUE - do cut for funct., else for chi2/ndf
  totalChi2Cut   = 300.;       // default value for funct. cut
  chi2PerNdfCut  = 50.;        // default value for chi2/ndf cut
  
  // Tukey weight constants:
  useTukeyFlag   = kTRUE;
  cnWs           = 6.4516;    //2.54*2.54;
  cn2s           = 17.5561;   //4.19*4.19;
  cn4s           = 10.6276;   //3.26*3.26;
  minSig2        = 2.5*2.5;
  maxNFilterIter = 4;
  minWeight      = 0.5;              // wt[time]=(wt[time]<minWeight) ? 0.:1.;
  maxChi2        = 25.; /*36.;6.0*/; // wt[time]=(chi2[time]>maxChi2) ? 0.:1.;
  
  minTOffsetIter = -50.; // if(timeOffset<minTOffsetIter) timeOffset=minTOffsetIter
  
  // Fit parameters for derivatives calc.:
  funCt1 = 500.;        // if(fun0 < funCt1)
  stepD1 = 0.0001;      //               stepD = stepD1;
  funCt2 = 10000.0;     // else if(fun0 < funCt2)
  stepD2 = 0.001;       //                stepD = stepD2;
  stepD3 = 0.01;        // else stepD = stepD3;
}

void HMdcTrackFitInOut::setNEventsPrint(Int_t start,Int_t nev) {
  printStartEv = start;
  nEventPrint  = nev;
}

void HMdcTrackFitInOut::setTukeyConstants(Double_t cw,Double_t c2,Double_t c4) {
  // Setting of tukey weights constants:
  // if     (chi2<cnWs*sig2) weight=(1-(chi2/(cn4s*sig2))^2)^2
  // else if(chi2<cn2s*sig2) weight=(1- chi2/(cn2s*sig2)   )^2
  // else weight=0.                                        
  cnWs = cw*cw;
  cn2s = c2*c2;
  cn4s = c4*c4;
}

void HMdcTrackFitInOut::getTukeyConstants(Double_t& cw,Double_t& c2,
    Double_t& c4) const {
  cw = sqrt(cnWs);
  c2 = sqrt(cn2s);
  c4 = sqrt(cn4s);
}
   
HMdcWireFit* HMdcTrackFitInOut::getNewWireFitSlot(Int_t* indWireFit) {
  TObject* fWireFit=fWireFitCat->getNewSlot(locWireFit,indWireFit);
  if(!fWireFit) {
    Warning("getNewWireFitSlot","No slot HMdcWireFit available");
    return 0;
  }
  if(geantFlag) return new(fWireFit) HMdcWireFitSim;
  else return new(fWireFit) HMdcWireFit;
}

HMdcClusFit* HMdcTrackFitInOut::getNewClusFitSlot(Int_t* indClusFit) {
  TObject* fClusFit=fClusFitCat->getNewSlot(locClusFit,indClusFit);
  if(!fClusFit) {
    Warning("getNewClusFitSlot","No slot HMdcClusFit available");
    return 0;
  }
  if(geantFlag) return new(fClusFit) HMdcClusFitSim;
  else return new(fClusFit) HMdcClusFit;
}

Double_t HMdcTrackFitInOut::getStepDer(Double_t funct) const {
  return (funct<funCt1) ? stepD1:((funct<funCt2) ? stepD2:stepD3);
}

void HMdcTrackFitInOut::setPrintFlag(Int_t currentEvent) {
  fprint=(currentEvent<printStartEv || 
          currentEvent>=printStartEv+nEventPrint) ? kFALSE : kTRUE;
}

void HMdcTrackFitInOut::setChi2PerNdfCut(Double_t cut) {
  chi2CutFlag   = kFALSE;
  chi2PerNdfCut = cut;
}

void HMdcTrackFitInOut::setTotalChi2Cut(Double_t cut) {
  chi2CutFlag  = kTRUE;
  totalChi2Cut = cut;
}

//============================================================================

HMdcTrackFitter::HMdcTrackFitter(HMdcTrackFitInOut* fIO) {
  fitInOut=fIO;
  init();
}

void HMdcTrackFitter::init(void) {
  fCal2ParSim=fitInOut->fCal2ParSim;
  fCal2Par=fitInOut->fCal2Par;
  fprint=fitInOut->getPrintFlag();
  wires.setPrintFlag(fprint);
  wires.setTrackFitInOut(fitInOut);
  tofFlag=fitInOut->tofFlag;
}

void HMdcTrackFitter::setPrintFlag(Bool_t prnt)        {
  fprint=prnt;
  wires.setPrintFlag(fprint);
}

Bool_t HMdcTrackFitter::fillListHits(HMdcClus* clus1,HMdcClus* clus2) {
  segIndex=-1;
  indClusFit=-1; //??? mozhet v drugoe mesto?
  if(!wires.fillListHits(clus1,clus2)) return kFALSE;
  setPlanes();
  return kTRUE;
}

Bool_t HMdcTrackFitter::fillListHits(HMdcEvntListCellsAndTimes* store,
				     HMdcClus* clus1,HMdcClus* clus2) {
  segIndex=-1;
  indClusFit=-1; //??? mozhet v drugoe mesto?
  if(!wires.fillListHits(store,clus1,clus2)) return kFALSE;
  setPlanes();  
  return kTRUE;
}

Bool_t HMdcTrackFitter::fillListHits(HMdcEvntListCellsAndTimes* store) {
  segIndex=-1;
  indClusFit=-1; //??? mozhet v drugoe mesto?
  if(!wires.fillListHits(store)) return kFALSE;
  setPlanes();  
  return kTRUE;
}

void HMdcTrackFitter::setPlanes(void) {
  HMdcSizesCellsMod** fSCModAr=fitInOut->fSCModAr[wires.getSector()];
  Int_t nClTimes=wires.getNDriftTimes();
  initParam.setFirstPlane(&((*(fSCModAr[wires[0].getModule()]))[0]));
  initParam.setSecondPlane(&((*(fSCModAr[wires[nClTimes-1].getModule()]))[5]));
  wires.fillLookupTableForDer(initParam);
}

Bool_t HMdcTrackFitter::fillClusFitCont(void) {
  HMdcClusFit* fClusFit=fitInOut->getNewClusFitSlot(&indClusFit);
  if(!fClusFit) return kFALSE;
  fClusFit->setFitStatus(fitStatus);
  finalParam.fillClusFit(fClusFit);
  fClusFit->setExitFlag(exitFlag);
  wires.fillClusFitSim(fClusFit,finalParam);
  wires.fillClusFitAndWireFit(fClusFit);       // must be after fillClusFitSim
  return kTRUE;
}

Bool_t HMdcTrackFitter::fitCluster(Int_t fittingMod) {
  //??? calcMinFunctional();  // Utochnenie nachal'nogo priblizheniya
  fitStatus=kFALSE;
  if(wires.getNCellsInInput(fittingMod) < 5) return kFALSE;
  Int_t iter=0;
  wires.setHitStatM1toP1();  // if(hitStatus==-1) hitStatus=1
  Int_t nCells=0;
  while(kTRUE) {
    Int_t exit=minimize(iter++);
    Double_t delW=wires.testFitResult();
    nCells=wires.getNCellsInOutput(fittingMod);
    if( delW<0.5 || nCells<6 ) {
      if(exit==0) break;
      if(delW>0.) {
        if(finalParam.calcChi2PerDF(wires.getNumOfGoodWires()) < 0.) break;
        if(!testChi2Cut()) break;
      }
      if(finalParam.testParameters(fitInOut->minTimeOffset,
          fitInOut->maxTimeOffset) && nCells >= fitInOut->minCellsNum) 
         fitStatus=kTRUE;
      break;
    }
    if(fprint) printf("TestFit: num.of deleted cells=%.1f, refit this!\n",delW);
  }
  if(fitInOut->fClusFitCat && HMdcTrackDSet::fNTuple()) fillClusFitCont();
  return fitStatus;
}

Bool_t HMdcTrackFitter::testChi2Cut(void) {
  if(fitInOut->chi2CutFlag) {
         if(finalParam.functional() < fitInOut->totalChi2Cut ) return kTRUE;
  } else if(finalParam.getChi2()    < fitInOut->chi2PerNdfCut) return kTRUE;
  return kFALSE;
}

void HMdcTrackFitter::calcMinFunctional(void) {
//   Double_t xCl=fClst->getX();
//   Double_t yCl=fClst->getY();
//   Double_t errX=fClst->getErrX()*2.; // +/-dX
//   Double_t errY=fClst->getErrY()*2.; // +/-dX
//   Int_t nBinsX=TMath::Max(Int_t(errX*2./2.5),1);  //  / 2.5 mm
//   Int_t nBinsY=TMath::Max(Int_t(errY*2./1.0),1);  //  / 1.0 mm
//   if(nBinsX%2 == 0) nBinsX++;
//   if(nBinsY%2 == 0) nBinsY++;
//   Double_t stepX=errX/nBinsX;
//   Double_t stepY=errY/nBinsY;
//   Double_t xStart=xCl-nBinsX/2*stepX;
//   Double_t yStart=yCl-nBinsY/2*stepY;
//   Double_t minFunc=1.e+20;
// Double_t xMin=xCl;
// Double_t yMin=yCl;
// Printf("xCl=%f, yCl=%f x:%i %f  y:%i %f",xCl,yCl,nBinsX,stepX,nBinsY,stepY);
//   for(Int_t ny=0; ny<nBinsY; ny++) {
//     Double_t yNew=yStart+stepY*ny;
// printf("Y=%7.6g", yNew);
//     for(Int_t nx=0; nx<nBinsX; nx++) {
//       Double_t xNew=xStart+stepX*nx;
//       zPlane = fClst->getZ();
//       setXInitPlane(xNew);
//       setYInitPlane(yNew);
//       setZInitPlane(fClst->getZOnPlane(xNew,yNew));
//       Double_t func=getFunctional();
// printf(" %7.6g",func);
//       if(func<minFunc) {
//         minFunc=func;
//         xMin=xNew;
//         yMin=yNew;
//       }
//     }
// printf("\n");
//   }
// printf("..... xMin=%f, yMin=%f funMin=%f\n\n",xMin,yMin,minFunc);
// setXInitPlane(xMin);
// setYInitPlane(yMin);
// setZInitPlane(fClst->getZOnPlane(xMin,yMin));
// //   setXInitPlane(xPlane);
// //   setYInitPlane(yPlane);
// //   setZInitPlane(zPlane);
}

void HMdcTrackFitter::setRegionOfWires(Int_t mod) {
  wires.setRegionOfWires(mod);
}

Bool_t HMdcTrackFitter::setClustAndFill(HMdcClus* fClst1, HMdcClus* fClst2) {
  if(fprint != fitInOut->fprint) setPrintFlag(fitInOut->fprint);
  if(fprint) {
    fClst1->print();
    if(fClst2) fClst2->print();
  }
  if(!fillListHits(fClst1,fClst2)) return kFALSE;
  initParam.setParam(fClst1->getXTarg(),fClst1->getYTarg(),fClst1->getZTarg(),
                     fClst1->getX(),    fClst1->getY(),    fClst1->getZ());
  wires.subtractWireOffset(initParam);
  return kTRUE;
}

Bool_t HMdcTrackFitter::setClustAndFill(HMdcEvntListCellsAndTimes* store,
					Double_t x1, Double_t y1, Double_t z1,
					Double_t x2, Double_t y2, Double_t z2) {
  if(fprint != fitInOut->fprint) setPrintFlag(fitInOut->fprint);
  if(fprint)
      printf("x1=%f  y1=%f z1=%f   x2=%f y2=%f z2=%f \n",x1,y1,z1,x2,y2,z2);
  if(!fillListHits(store)) return kFALSE;
  initParam.setParam(x1, y1, z1, x2, y2, z2);
  wires.setXYZClust(x2, y2, z2);
  wires.subtractWireOffset(initParam);
  return kTRUE;
}

void HMdcTrackFitter::fillOutput() {
  finalParam.copyLine(initParam);
  wires.calcNGoodWiresAndChi2(finalParam);
  wires.valueOfFunctional(finalParam,2);
  wires.calculateErrors(finalParam); //Errors calculations
}
