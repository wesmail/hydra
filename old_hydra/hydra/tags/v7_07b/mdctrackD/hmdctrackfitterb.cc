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

//-----------------------------------------------------
HMdcTrackFitterB::HMdcTrackFitterB(HMdcTrackFitInOut* fIO)
    : HMdcTrackFitter(fIO) {
  setDefaultParam();
}

void HMdcTrackFitterB::setDefaultParam(void) {
  maxIteration = 250;  // maximal numer of iterations
  // Fit parameters for 1-st minim. method:
  initStepFit       = 50.;     // init value of stepFit
  limIter1forFilter = 3;       // max. number of iter. at the filtering
  limFunct1to2      = 10.0;    // limit for functional
  limDeltaF1to2     = 0.001;   // limit for functional
  limGrad1to2       = 1.0;     //
  // DownhillOnGradient parameters:
  stepCh[0] = 2.0;
  stepCh[1] = 1.5;
  stepCh[2] = 1.5;
  stepCh[3] = 2.0;
  stepCh[4] = 1.2;
  stepCh[5] = 1.1;
  dFunctMax = 1.5;
  dFunctMin = 0.7;

  // Fit parameters for 2-nd minim. method:
  limGrad2    = 0.5;              //
  limStep2[0] = 0.25;             // dX1: limit for diff. old-new param.
  limStep2[1] = 0.15;             // dX2
  limStep2[2] = 0.25;             // dY1
  limStep2[3] = 0.15;             // dY2
  limIter2    = 6; /*3;*/         // max. number of iter.
}

//-----------------------------------------------------
HMdcTrackFitterB::~HMdcTrackFitterB(void) {
}

//-----------------------------------------------------
Double_t HMdcTrackFitterB::getFunctional(void) {
  // not used yet ???
  return wires.valueOfFunctional(finalParam);
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
  Bool_t noWightsFor3Iter=kTRUE; //kTRUE;
  if(doTargScan) targetScan();


  //********* Initialisation of fit ************************
  stepFit=initStepFit;      // init value of step
  return2to1 = 0;           // counter

  wires.valueOfFunctional(finalParam);  //???
  wires.calcTdcErrorsAndFunct(finalParam);
  iterAfterFilter=0;
  if(!noWightsFor3Iter && fitInOut->useTukey()) wires.filterOfHits(finalParam,1); //!!!!!!!!!!!!!!???
//iterAfterFilter=-9; //!!!!!!!!!!!!!!!!!!!

  //******************* BEGIN of FIT PROCEDURE ******************
  Int_t minimizationMethod = 1;
  for (iteration = 0; iteration < maxIteration; iteration++ ) {
    if(minimizationMethod==1) {
      minimizationMethod=firstMethod();
      if(minimizationMethod<1) break;
    } else if(minimizationMethod==2) {
      minimizationMethod=secondMethod();
      if(minimizationMethod<1) break;
    }
  } //end of iter loop
  exitFlag=(minimizationMethod<0) ? -minimizationMethod : 4;
  if(fprint) printResult();

  //******************* TEST of RESULT ***************************
//!???????? Duzhe esli ochen' bolshoy funck. no est' minimum delat' viykid???
  //   if(exitFlag > 2) return 0; //Only precise results !!!!
  if(exitFlag <= 3 && wires.calcNGoodWiresAndChi2(finalParam) &&
      wires.valueOfFunctional(finalParam,2) < threshold) {
    wires.calculateErrors(finalParam); //Errors calculations
    if(fprint) printf("    !!! GOOD EXIT !!!\n");
    return 1;
  }
  return 0;
}

//---------------------------------------------------------------------
Int_t HMdcTrackFitterB::firstMethod(void) {
  //*** 1-st method
  // function return minimizationMethod flag
  Double_t funct1beforeFilter=finalParam.functional();
  wires.calcDerivatives(finalParam,1);
  for(; iteration<maxIteration; iteration++) {
    downhillOnGradient(finalParam);
    if(fprint) finalParam.printParam("out1");

    iterAfterFilter++;
    //---------*** Test for method of minimization ***---------
    if(return2to1 > 0 && iterAfterFilter < 2) {
      wires.calcDerivatives(finalParam,1);
      continue;
    }

    if((iterAfterFilter>=2 && ((finalParam>funct1beforeFilter && iteration>2) ||
        finalParam.isFunctRelChangLess(limDeltaF1to2))) ||
        finalParam<limFunct1to2) {
// nuzhna li fil'traciya
      wires.calcTdcErrorsAndFunct(finalParam,1);
      wires.calcDerivatives(finalParam,2);
      return 2; // mini. method
    }

    if(iterAfterFilter == limIter1forFilter) { // ??? == Pochemu???
      funct1beforeFilter = finalParam.functional();
      if(fitInOut->useTukey() && wires.filterOfHits(finalParam))
          iterAfterFilter=0;
      wires.calcTdcErrorsAndFunct(finalParam);
    }

    wires.calcDerivatives(finalParam,1);
    // Scaled gradiend calculation // ??? Pochemu proveryaetsya sc.gr.???
    if(calcScaledAGrad(finalParam)<limGrad1to2) {
// nuzhna li fil'traciya
      wires.calcTdcErrorsAndFunct(finalParam,1);
      return 2; // mini. method
    }
  }
  return -4;
}

Int_t HMdcTrackFitterB::secondMethod(void) {
  //*** 2-nd method
  Int_t iteration2=0;
  Bool_t final = kFALSE;
  parMin.copyAllParam(finalParam);
  wires.calcDerivatives(parMin,2);
  for(; iteration<maxIteration; iteration++) {
    parMin.saveFunct();
    pari.copyParam(parMin);
    solutionOfLinearEquationsSystem(parMin);
    if(fprint) parMin.printParam((final) ? "outF":"out2");
//??? esli vesa pereschitaniy to kak sravnivat' s finalParam ???
    Bool_t parMinEqFinalParam=(parMin < finalParam) ? kTRUE:kFALSE;
    if(parMinEqFinalParam) finalParam.copyAllParam(parMin);
    iteration2++;

    //***  Test for exit from minimization
    if(!final) {
      if(/*agrad < 1.5*limGrad2 ||*/ !pari.compare(parMin,limStep2,1.5) ||
            (return2to1>0 && iteration2>=limIter2)) {
//!??? /*agrad < 1.5*limGrad2 ||*/ Proverit'. I dlya agrad nado schita' proizv.!!!
        // For filtering take the best parameters:
        if(!parMinEqFinalParam) wires.valueOfFunctional(finalParam);
//!???  if(iterAfterFilter>=limIter1forFilter) //++++++++++++++!!!!!!!!!!!!!!!!!!!
        if(fitInOut->useTukey() && wires.filterOfHits(finalParam))
            iterAfterFilter=0;
//!??? Pochemu net calcTdcErrorsAndFunct ???
        wires.setWeightsTo1or0(finalParam);  // for finalParam!
        iterAfterFilter = -1000000;
        if(finalParam > 100000.0) return -5; // Too large functional !
        parMin.copyAllParam(finalParam);
        final = kTRUE;
        iteration2 = 0;
      } else if(iteration2 >= limIter2) {
        //  For filtering take the best parameters:
        if(!parMinEqFinalParam) wires.valueOfFunctional(finalParam);
//!??? if(iterAfterFilter>=limIter1forFilter) //++++++++++++++!!!!!!!!!!!!!!!!!!!
        if(fitInOut->useTukey() && wires.filterOfHits(finalParam))
            iterAfterFilter=0;
        wires.calcTdcErrorsAndFunct(finalParam);
        iteration2 = 0;
        return2to1++;
        return 1;   // go back to the first method
      }
      wires.calcDerivatives(parMin,2);
    } else {
      wires.calcDerivatives(parMin,2);
      if(wires.getAGrad() < limGrad2) return -1;               // return exit flag 1
      if(iteration2 < 2) continue;
      if(!pari.compare(parMin,limStep2)) return -2; // return exit flag 2
      if(iteration2 >= limIter2) return -3;         // return exit flag 3
    }
  }
  return -4;
}

//-----------------------------------------------------
void HMdcTrackFitterB::solutionOfLinearEquationsSystem(HMdcTrackParam& par) {
  // input :  matrix grad2[i][j] (i-string, j-column),  vector grad[i]
  // output:  new param. in vector par[i]
  Double_t a[10][11];
  Int_t ieq[10];

  TMatrixD& grad2m=wires.getGrad2Matr();
  Double_t* grad=wires.getGrad();
  Int_t nmOfPar=par.getNumParam();
  for(int i = 0; i < nmOfPar; i++) {
    for(int j = 0; j < nmOfPar; j++) a[i][j] = grad2m(i,j);
    a[i][nmOfPar] = -grad[i];
    ieq[i] = -1;
  }
  Int_t iMax = 0;
  Int_t jMax = 0;
  for(int l = 0; l < nmOfPar; l++) {
    Double_t maxA = 0.0 ;
    for(int i = 0; i < nmOfPar; i++) {
      if(ieq[i] != -1) continue;
      for(int j = 0; j < nmOfPar; j++) {
	if(fabs(a[i][j]) <= maxA) continue;
	maxA = fabs(a[i][j]);
	iMax = i;
	jMax = j;
      }
    }
    ieq[iMax] = jMax;
    Double_t corr = a[iMax][jMax];
    for(int j = 0; j <= nmOfPar; j++) a[iMax][j] /= corr;
    for(int i = 0; i < nmOfPar; i++) {
      if(i == iMax) continue;
      corr = a[i][jMax];
      for(int j = 0; j <= nmOfPar; j++) a[i][j] -= a[iMax][j]*corr;
    }
  }

  for(int i = 0; i < nmOfPar; i++) {
    Int_t iout = ieq[i];
    if(iout>=0) par.addToParam(iout,a[i][nmOfPar]); //!!!
  }

  wires.valueOfFunctional(par);
  par.incIterNumb();
}

//-----------------------------------------------------
void HMdcTrackFitterB::downhillOnGradient(HMdcTrackParam& par) {
  Int_t istepinc = 0;
  Int_t istepdec = 0;
  Int_t iprint = 0;

  //--- Scaling:
  Int_t numOfParam=par.getNumParam();
  Double_t scalePar[numOfParam];
  Double_t cosgrad[numOfParam];
  Double_t agrad=wires.getAGrad();
  Double_t* grad=wires.getGrad();
  TMatrixD& grad2=wires.getGrad2Matr();
  if(iteration>0) {
    agrad = 0.0;
    for(Int_t k=0; k<numOfParam; k++) {
      scalePar[k] = (grad2(k,k)>0.001) ? 1./sqrt(grad2(k,k)) : 1.;
      agrad += grad[k]*grad[k]*scalePar[k]*scalePar[k];
    }
    agrad = sqrt(agrad);
    for(Int_t k = 0; k < numOfParam; k++) cosgrad[k]=grad[k]*scalePar[k]/agrad;
  } else {
    for(Int_t k = 0; k < numOfParam; k++) scalePar[k] = 1.0;
    for(Int_t k = 0; k < numOfParam; k++) cosgrad[k] = grad[k]/agrad;
  }

  Double_t functIn=par.functional();
  par.saveFunct();
  tmpPar.setIterNumb(par.getIterNumb());

  tmpPar.setScParam(par,cosgrad,0.001,scalePar);
  Double_t dFdR0 = (wires.valueOfFunctional(tmpPar)/functIn - 1.0)/(0.001);

  Double_t functi;
  Double_t  dFdRi;
  while(kTRUE) {
    tmpPar.setScParam(par,cosgrad,stepFit,scalePar);
    functi = wires.valueOfFunctional(tmpPar)/functIn;
    tmpPar.setScParam(par,cosgrad,stepFit*1.001,scalePar);
    dFdRi = (wires.valueOfFunctional(tmpPar)/functIn-functi)/(stepFit*0.001);

    if(dFdRi < 0) {
      if(functi > 1.0 && istepdec < 2) {
        stepFit = stepFit/5;
        istepdec++;
        if(fprint && iprint == 1) printStep("de",functi,stepFit);
      } else {
        if(istepinc ==2) break;
        stepFit *= stepCh[istepdec + 3*istepinc++];
        if(fprint && iprint == 1) printStep("in",functi,stepFit);
      }
    } else {   //dFdRi >= 0
      if(functi < dFunctMin) {
        if(istepinc ==2) break;
        stepFit *= stepCh[istepdec + 3*istepinc++];
        if(fprint && iprint == 1) printStep("in",functi,stepFit);
      } else if(functi > dFunctMax) {
        if(istepdec ==2) break;
        stepFit /= stepCh[istepinc + 3*istepdec++];
        if(fprint && iprint == 1) printStep("de",functi,stepFit);
      } else break;
    }
  }

  Double_t coeffC = 1.0;
  Double_t coeffB = dFdR0;
  Double_t coeffA = (functi - coeffC - coeffB*stepFit)/(stepFit*stepFit);
  Double_t stepFit1 = -coeffB/(2*coeffA);
  if(coeffA < 0.000001 || (stepFit1/stepFit) > 1.2)
      stepFit1 = (dFdRi > 0) ? stepFit/2.0 : stepFit/1.5;

  tmpPar.setScParam(par,cosgrad,stepFit1,scalePar);
  wires.valueOfFunctional(tmpPar);
  if(tmpPar < par) par.copyNewParam(tmpPar);
  else {
    par.setScParam(cosgrad,0.3*stepFit1,scalePar);  // 0.3 ????
    wires.valueOfFunctional(par);
  }
  par.incIterNumb();
}

Double_t HMdcTrackFitterB::calcScaledAGrad(HMdcTrackParam& par) {
  Double_t agradSc = 0.0;
  Int_t numOfParam=par.getNumParam();
  Double_t* grad=wires.getGrad();
  TMatrixD& grad2=wires.getGrad2Matr();
  for(Int_t k=0; k<numOfParam; k++) agradSc +=(grad2(k,k)>0.001) ?
    grad[k]*grad[k]/grad2(k,k) : grad[k]*grad[k];
  return sqrt(agradSc);
}

//-----------------------------------------------------
void HMdcTrackFitterB::printResult(void) {
  if(exitFlag==1) printResult(" conversion ok, exit=1 ",finalParam);
  else if(exitFlag==2) printResult(" conversion ok, exit=2 ",finalParam);
  else if(exitFlag==3) printResult(" no conversion, exit=3 ",finalParam);
  else if(exitFlag==4) printResult(" no conversion, exit=4 ",finalParam);
  else printResult(" no conversion, Too large functional!",finalParam);
}

void HMdcTrackFitterB::printResult(const char* status,HMdcTrackParam& par) {
  printf("%s\n",status);
  par.printParam("final");
  printf("**************************************************************\n\n");
}

void HMdcTrackFitterB::printStep(char* cond, Double_t fun, Double_t step) {
  printf("  *functi(dFdRi>0)=%g  *!  step is %screased, now stepFit=%g\n",
      fun,cond,step);
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

      wires.valueOfFunctional(parMin);
      wires.calcTdcErrorsAndFunct(parMin,-1);
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

      wires.valueOfFunctional(parMin);
      wires.calcTdcErrorsAndFunct(parMin,-1);
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
