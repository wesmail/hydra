//*--- Author : V.Pechenov
//*--- Modified: 16.06.2005 by V.Pechenov

#include "hmdctrackfitterb.h"
#include "hmdctrackfitpar.h"
#include "hmdcsizescells.h"
#include "hmdccal2parsim.h"
#include "hsymmat.h"
#include "hmdccal1sim.h"
#include "hcategory.h"
#include "hmdcclus.h"
#include <stdlib.h>
#include <TMatrixD.h>

//////////////////////////////////////////////////////////////////////////////
// 
// HMdcTrackFitterB
//
// Dubna track straight piece fitter. Version B.
// It is tuned version of HMdcTrackFitterA.
//
//////////////////////////////////////////////////////////////////////////////


ClassImp(HMdcTrackFitterB)

HMdcTrackFitterB::HMdcTrackFitterB(HMdcTrackFitInOut* fIO)
    : HMdcTrackFitterA(fIO) {
}

HMdcTrackFitterB::~HMdcTrackFitterB(void) {
}

//-----------------------------------------------------
Int_t HMdcTrackFitterB::minimize(Double_t threshold, Int_t iter) {
  if(fprint) {
    printf("\n  ********************************\n");
    printf("  *** PROGRAM OF TRACK FITTING ***\n");
    printf("  ********************************\n");
  }
  
  wires.setSizeGrad2Matr(initParam);
  finalParam.copyLine(initParam);
  finalParam.setIterNumb(0);
  parMin.copyPlanes(initParam);
  pari.copyPlanes(initParam);
  tmpPar.copyPlanes(initParam);

  // Target scanning:
  Bool_t doTargScan=kTRUE;  //kTRUE;
  Bool_t noWightsFor3Iter=kFALSE; //kTRUE;
  if(doTargScan) targetScan();


  //********* Initialisation of fit ************************
  stepFit=initStepFit;      // init value of step
  return2to1 = 0;           // counter

  wires.valueOfFunctional(finalParam);  //???
  wires.calcTdcErrorsAndFunct(finalParam);
  iterAfterFilter=0;
  if(!noWightsFor3Iter && fitInOut->useTukey()) wires.filterOfHits(finalParam,1); //!!!!!!!!!!!!!!???
//iterAfterFilter=-9; //!!!!!!!!!!!!!!!!!!!

  return doMinimization(threshold);
}

void HMdcTrackFitterB::targetScan(void) {
  if(wires.getSegment()==1) return;
  HMdcSizesCellsSec& fSCSec=(*fitInOut->getMdcSizesCells())[wires.getSector()];
  Int_t nTargets=fSCSec.getNumOfTargets();
  HMdcClus* fClst=wires.getClust1();
  if(nTargets>1) {
    for(Int_t nTg=0;nTg<nTargets;nTg++) {
      HGeomVector* targ=fSCSec.getTarget(nTg);
      parMin.setParam( targ->getX(),  targ->getY(),  targ->getZ(),
          wires.getXClst(),wires.getYClst(),wires.getZClst());

wires.valueOfFunctAndErr(parMin);
//!!!      wires.valueOfFunctional(parMin);
//!!!      wires.calcTdcErrorsAndFunct(parMin,-1);
      wires.calcNGoodWiresAndChi2(parMin);
      
      if(fprint)printf("Scan: %i target. chi2/NDF=%g\n",nTg,parMin.getChi2());
      if(nTg==0 || parMin.getChi2()<finalParam.getChi2()) {
        finalParam.copyNewParam(parMin);
        if(fClst) fClst->setTarg(targ->getX(), targ->getY(), targ->getZ());
      }
      wires.setUnitWeights();
    }
  } else {
    const HGeomVector& firstTargPoint=fSCSec.getTargetFirstPoint();
    const HGeomVector& lastTargPoint=fSCSec.getTargetLastPoint();
    Double_t z1=firstTargPoint.getZ();
    Double_t z2=lastTargPoint.getZ();
    if(z2-z1 < 5.) return;
    Double_t xPl=wires.getXClst();
    Double_t yPl=wires.getYClst();
    Double_t zPl=wires.getZClst();
    const HGeomVector& targ=fSCSec.getTargetMiddlePoint();
    Double_t xTg=targ.getX();
    Double_t yTg=targ.getY();
    
    Double_t zMin=-5000.;
    for(Double_t z=z1;z<=z2;z+=2.5) { //!!!!!!!!!!!!!!!!!!!!
      parMin.setParam(xTg,yTg,z, xPl,yPl,zPl); //!!!!!!!!!!!!!!!!!!!!

wires.valueOfFunctAndErr(parMin);
//      wires.valueOfFunctional(parMin);
//      wires.calcTdcErrorsAndFunct(parMin,-1);
      wires.calcNGoodWiresAndChi2(parMin);

      if(fprint)printf("Scan: Ztarget=%5.1f chi2/NDF=%g\n",z,parMin.getChi2());
      if(zMin<-4000. || parMin.getChi2()<finalParam.getChi2()) {
        zMin=z;
        finalParam.copyNewParam(parMin);
        if(fClst) fClst->setTarg(xTg,yTg,z);
      }
      wires.setUnitWeights();
    }
  }
  finalParam.setIterNumb(0);
}
