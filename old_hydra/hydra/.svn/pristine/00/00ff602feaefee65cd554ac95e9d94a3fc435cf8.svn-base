#include "hmdctrackfittera.h"
#include "hmdctrackfitpar.h"
#include "hmdcsizescells.h"
#include "hmdccal2parsim.h"
#include <stdlib.h>

//*-- Author : A.P.Jerusalimov
//*-- Modified: 27.02.2002
//*-- Modified: 24.03.2002 by V.Pechenov
//*-- Modified: 17.07.2002 by A.P.Jerusalimov
//*-- Modified: 08.08.2002 by A.P.Jerusalimov
ClassImp(HMdcTrackFitterA)

//-----------------------------------------------------
HMdcTrackFitterA::HMdcTrackFitterA(void) {
}

//-----------------------------------------------------
HMdcTrackFitterA::~HMdcTrackFitterA(void) {
}

//-----------------------------------------------------
Bool_t HMdcTrackFitterA::init(void) {
  Bool_t flag=HMdcTrackFitter::init();
  return flag;
}

//-----------------------------------------------------
Bool_t HMdcTrackFitterA::reinit(void) {
  Bool_t flag=HMdcTrackFitter::reinit();
  return flag;
}

//-----------------------------------------------------
Double_t HMdcTrackFitterA::getFunctional(void) {
  // not used yet ???
  setParam();
  valueOfFunctional(parameter,4);
  return valueOfFunctional(parameter,5);
}

//-----------------------------------------------------
void HMdcTrackFitterA::setParam(void) {
  numOfParam = 4; //was 5
  Int_t mdc;
  if(nMDC1Times>0 && nMDC2Times==0) mdc=segment*2;
  else if(nMDC1Times==0 && nMDC2Times>0) mdc=1+segment*2;
  else mdc=-1;
  if(mdc>=0) {
    if(mdc==0) driftVelMDC1 = fitPar->getDriftVelocityMDC(sector,mdc);
    if(mdc==1) driftVelMDC2 = fitPar->getDriftVelocityMDC(sector,mdc);
    if(mdc==2) driftVelMDC3 = fitPar->getDriftVelocityMDC(sector,mdc);
    if(mdc==3) driftVelMDC4 = fitPar->getDriftVelocityMDC(sector,mdc);
    //    fitPar->getSteps(sector,mdc,step);
    tukeyConst = fitPar->getTukeyConstant(sector,mdc);
    //    maxIteration = fitPar->getMaxIteration(sector,mdc);
  } else {
    if(segment==0) {
      driftVelMDC1 = fitPar->getDriftVelocityMDC(sector,0);
      driftVelMDC2 = fitPar->getDriftVelocityMDC(sector,1);
      //      fitPar->getStepsSeg1(sector,step);
      tukeyConst = fitPar->getTukeyConstantSeg1(sector);
      //      maxIteration = fitPar->getMaxIterationSeg1(sector);
    } else {
      driftVelMDC3 = fitPar->getDriftVelocityMDC(sector,2);
      driftVelMDC4 = fitPar->getDriftVelocityMDC(sector,3);
      //      fitPar->getStepsSeg2(sector,step);
      tukeyConst = fitPar->getTukeyConstantSeg2(sector);
      //      maxIteration = fitPar->getMaxIterationSeg2(sector);
    }
  }
  tukeyConst = 4.5;
  maxIteration = 500; //!!!!!!!!!!!!!!!!!!!!
  parameter[0] = xVertex;
  parameter[1] = yVertex;
  parameter[2] = xInitPlane;
  parameter[3] = yInitPlane;
  parameter[4] = 3;
}

//-----------------------------------------------------
void HMdcTrackFitterA::fillOutput(Double_t* par) {
  for(Int_t n=0; n<numOfParam; n++) parameter[n]=par[n];
  funMin = valueOfFunctional(parameter,3);
  xOutVertex = parameter[0];
  yOutVertex = parameter[1];
  zOutVertex = parD1-parA1*xOutVertex-parB1*yOutVertex; //!!! zVertex;
  xOutPlane = parameter[2];
  yOutPlane = parameter[3];
  zOutPlane = parD2-parA2*xOutPlane-parB2*yOutPlane; //!!! zInitPlane;
  arriveTime = parameter[4];
}

//-----------------------------------------------------
Bool_t HMdcTrackFitterA::finalize() {
  return kTRUE;
}

//-----------------------------------------------------
void HMdcTrackFitterA::printResult(Char_t* status) {
  printf("%s\n",status);
  printParam("final",parameter);
  printf("**************************************************************\n\n");
}

//-----------------------------------------------------
void HMdcTrackFitterA::printParam(Char_t* title, Double_t *par) {
  printf("%s %3i Par.=%-6g %-6g %-6g %-6g %-6g  fun.=%-6g->%-6g\n",
      title,iteration,par[0],par[1],par[2],par[3],par[4],
      functional,newFunctional);
}

//-----------------------------------------------------
Int_t HMdcTrackFitterA::execute(Double_t threshold, Int_t iter) {
  if(iter==0 || numOfParam==0) setParam();

  Int_t    limIter1forFilter = 3;
  Double_t funct1beforeFilter;

  Double_t limFunct1to2 = 10.0;
  Double_t limDeltaF1to2 = 0.001;
  Double_t limGrad1to2 = 1.0;

  Double_t limGrad2 = 0.5;
//   Double_t limStep2[10] = {0.025, 0.015, 0.025, 0.015, 0.5};
  Double_t limStep2[10] = {0.25, 0.15, 0.25, 0.15, 0.5};
//   Int_t    limIter2 = 3;
  Int_t    limIter2 = 6;

  Double_t minWeight = 0.5;
  Double_t maxChi2 = 25.; //36.0; // 6.0;

  Double_t par0[10], parNew[10];
  Double_t parMin[10], pari[10];
  Double_t functMin=0.;

  Int_t return2to1 = 0;

  if(fprint) {
    printf("\n  ********************************\n");
    printf("  *** PROGRAM OF TRACK FITTING ***\n");
    printf("  ********************************\n");
  }

//********* Initialisation of fit ************************
  stepFit=50.0;
  maxIteration = 250;

  Int_t numberOfGoodHits=nClTimes-firstTime;
  for(int time = firstTime; time < nClTimes; time++) {
    hitSt[time]=hitStatus[time];
    if(hitStatus[time]==1) qualityWeight[time] = weight[time] = 1;
    else {
      if(fprint) printf("  Warn.!  HIT %i is deleted\n",time);
      numberOfGoodHits--;
      qualityWeight[time] = weight[time] = 0.0;
    }
  }

  for(Int_t k = 0; k < numOfParam; k++ )  par0[k] = parameter[k];
  for(int time = firstTime; time < nClTimes; time++) errTdcTime[time] = 1.0;

  Int_t iteration2 = 0;
  Int_t iexit = 0;

  functional = valueOfFunctional(par0,10);
  for(Int_t k = 0; k < numOfParam; k++ )  parameter[k] = par0[k]; //new
  calcTdcErrors(par0);
  functional = valueOfFunctional(par0,10);
  filterOfHits(par0,numberOfGoodHits);
//******************* BEGIN of FIT PROCEDURE ******************

  Int_t iterAfterFilter = 0;
  Bool_t final = kFALSE;
     useTukey = kTRUE;
  newFunctional = funct1beforeFilter = valueOfFunctional(par0,11);
  derivativesOfFunctional(newFunctional,par0);
  Int_t minimizationMethod = 1;
  for (iteration = 0; iteration < maxIteration; iteration++ ) {
    functional = newFunctional;

//*** 1-st method
    if(minimizationMethod == 1) {
      downhillOnGradient(par0,parNew);
      if(fprint) printParam("out1",parNew);
      derivativesOfFunctional(newFunctional,parNew,1);
      iterAfterFilter++;
      for(Int_t k = 0; k <= numOfParam; k++ ) par0[k] = parNew[k];
//----end of 1-st method

//----*** Test for method of minimization
      if(return2to1 > 0 && iterAfterFilter < 2) continue;
      if((agrad < limGrad1to2) ||
         (newFunctional < limFunct1to2) ||
         (iterAfterFilter >= 2 &&
          ((newFunctional > funct1beforeFilter && iteration >2) ||
           fabs((functional - newFunctional)/functional) < limDeltaF1to2))) {
        minimizationMethod = 2;
        for(Int_t k = 0; k < numOfParam; k++ )  parameter[k] = par0[k]; //new
        calcTdcErrors(par0);
        functMin = newFunctional = valueOfFunctional(par0,11);
        for(Int_t k = 0; k <= numOfParam; k++) parMin[k] = par0[k];
        derivativesOfFunctional(newFunctional,par0,2);
      }
        else if(iterAfterFilter == limIter1forFilter) {
        funct1beforeFilter = newFunctional;
        filterOfHits(par0,numberOfGoodHits);
        iterAfterFilter = 0;
        for(Int_t k = 0; k < numOfParam; k++ )  parameter[k] = par0[k]; //new
        calcTdcErrors(par0); //new
        newFunctional = valueOfFunctional(par0);
        derivativesOfFunctional(newFunctional,par0,1);
      }
    } else {
//*** 2-nd method
      for(Int_t k = 0; k < numOfParam; k++) pari[k] = par0[k];
      solutionOfLinearEquationsSystem(par0);
      newFunctional = valueOfFunctional(par0);
      if(newFunctional < functMin) {
        functMin = newFunctional;
        for(Int_t k = 0; k <= numOfParam; k++) parMin[k] = par0[k];
      }
      derivativesOfFunctional(newFunctional,par0,2);
      iteration2++;
      if(fprint) {
        if(!final) printParam("out2",par0);
        else printParam("outF",par0);
      }

//***  Test for exit from minimization

    if(final) {
      iexit = 1;
      if(agrad < limGrad2) break;

      iexit = 2;
      if(iteration2 < 2) continue;
      for(Int_t k = 0; k < numOfParam; k++)
        if(fabs(pari[k] - par0[k]) > limStep2[k]) {
// 	  cout<< " first iexit = 3 " << k << " " << fabs(pari[k] - par0[k])<< endl;
	  iexit = 3;
	}
      if(iexit == 2) break;

      if(iteration2 >= limIter2) {
        for(Int_t k = 0; k <= numOfParam; k++) par0[k] = parMin[k];
        functional = newFunctional;
        newFunctional =  functMin;
        break;
      }
      continue;
    }
    else {

      if(agrad < 1.5*limGrad2) iexit = 1;
      else {
        iexit = 2;
        for(Int_t k = 0; k < numOfParam; k++)
          if(fabs(pari[k] - par0[k]) > 1.5*limStep2[k]) {
// 	    cout<< " second iexit = 3 " << k << " " << fabs(pari[k] - par0[k])<< endl;
	    iexit = 3;
	  }
      }
      if(iexit == 1 || iexit == 2 ||
         (return2to1 > 0 && iteration2 == limIter2)) {
        for(Int_t k = 0; k <= numOfParam; k++) par0[k] = parMin[k];
        functional = newFunctional;
        newFunctional =  functMin;
        filterOfHits(par0,numberOfGoodHits);
        for(int time = firstTime; time < nClTimes; time++)
            weight[time]=(chi2[time]<maxChi2 || weight[time]>minWeight) ? 1.:0.;
        functMin = newFunctional = valueOfFunctional(par0);
        if(functMin > 1000.0) {
          if(fprint) printf(" ! Too large functional !\n");
          return 0; // bad track
        }
        derivativesOfFunctional(functional,par0,2);
        final = kTRUE;
        iteration2 = 0;
        continue;
      }

      if(iteration2 < limIter2) continue;

      return2to1++;
      minimizationMethod = 1;
      newFunctional = valueOfFunctional(parMin);
//------
      calcTdcErrors(parMin);
//======
//      filterOfHits(par0,numberOfGoodHits);
      filterOfHits(parMin,numberOfGoodHits);
      iterAfterFilter = 0;
      newFunctional = valueOfFunctional(parMin);
      derivativesOfFunctional(newFunctional,parMin,1);
      for(Int_t k = 0; k <= numOfParam; k++) par0[k] = parMin[k];
      iteration2 = 0;
      continue;
    }

    }  //end of 2-nd method
    iexit = 4;
  } //end of iter loop

  if(fprint) {
    if(iexit == 1) printResult(" conversion ok, exit=1 ");
    if(iexit == 2) printResult(" conversion ok, exit=2 ");
    if(iexit == 3) printResult(" no conversion, exit=3 ");
    if(iexit == 4) printResult(" no conversion, exit=4 ");
  }
//   if(iexit > 2) return 0; //Only precise results !!!!
  if(iexit > 3) return 0; //Only precise results !!!!
  Int_t numOfGoodWires = 0;
  for(int time = firstTime; time < nClTimes; time++)
    if(weight[time] > 0.1) numOfGoodWires++;
  if(numOfGoodWires < 5) return 0; //Not less than 5 wires !!!
  if(par0[4] < -15.0 || par0[4] > 30.0) return 0;  // Too large t-off !!!

  fillOutput(par0);
  if(fprint) cout<< "    !!! GOOD EXIT !!!" << endl;

  return ((newFunctional < threshold) ? 1 : 0);
}

//-----------------------------------------------------
Double_t HMdcTrackFitterA::calcMdcTime(Int_t time,
    Double_t xVertex, Double_t yVertex, Double_t zVertex,
    Double_t xPlane, Double_t yPlane, Double_t zPlane) {
  Double_t distance,alpha;
  if(flDstTime) {
    inCell[time]=fSizeCellsLayer[time]->getImpact(xVertex, yVertex, zVertex,
        xPlane, yPlane, zPlane, lCell[time], alpha, distance);
    alphaDrDist[time]=alpha;
  } else if(version == 1) {
    distance=fSizeCellsLayer[time]->getImpact(xVertex, yVertex, zVertex,
        xPlane, yPlane, zPlane, lCell[time], alpha);
    alphaDrDist[time]=alpha;
  } else {
    distance=fSizeCellsLayer[time]->getDist(xVertex, yVertex, zVertex,
        xPlane, yPlane, zPlane,lCell[time]);
  }
  distWire[time]=distance;

  Double_t driftTime=-1000.;

  if(version == 0) {

    if(lModule[time] == 0) {
      driftTime = ( distance / driftVelMDC1 )*1000.;
    } else if(lModule[time] == 1) {
      if(distance < 5.0 ) {
      driftTime = 5.85 + 4.30*distance + 18.88*distance*distance
	        - 8.54*distance*distance*distance
	        + 1.48*distance*distance*distance*distance;
    } else driftTime = 1305.1928784*sqrt(distance) - 2561.65;
    } else if(lModule[time] == 2) {
      if(distance < 6.5 ) {
      driftTime =
	  - 0.03542
	  + 78.7210 * distance
	  - 287.847 * pow(distance,2)
	  + 512.043 * pow(distance,3)
	  - 454.673 * pow(distance,4)
	  + 228.805 * pow(distance,5)
	  - 68.1692 * pow(distance,6)
	  + 11.9125 * pow(distance,7)
	  - 1.12813 * pow(distance,8)
	  + 0.04468 * pow(distance,9);
      }  else driftTime = 23717.08*sqrt(distance) - 59099.37;
    } else if(lModule[time] == 3) {
      driftTime = ( distance / driftVelMDC4 )*1000.;
    } else return 0.;
  } else {
//new----------------------- // version = 1 ----------------------------
    driftTime=fCal2ParSim->calcTime(sector,lModule[time],alpha,distance);
//----------------------------------------------------------------------
  }
  return driftTime;
}

//-----------------------------------------------------
void HMdcTrackFitterA::printTime(char* title, Int_t nt, Int_t iflag,
    Double_t aTm) {
  printf(" %s %c%3i)%c %iM%iL%3iC",title,(iflag==3) ? '!':' ',nt,
      (hitSt[nt]==1)?'+':'-',lModule[nt]+1,lLayer[nt]+1,lCell[nt]+1);
  printf(" %5.2fmm %c",distWire[nt],(inCell[nt]) ? 'I':'O');
  printf(" dev=%5.1f%+4.1f%+6.1f=%5.1fns",drTime[nt],aTm,-tdcTime[nt],dev[nt]);
  printf(" dT=%4.1f chi2=%5.1f WT=%g\n",errTdcTime[nt],chi2[nt],weight[nt]);
}

//-----------------------------------------------------
void HMdcTrackFitterA::derivativesOfFunctional( Double_t fun0, Double_t* par,
    Int_t iflag) {

  Double_t stepD;

  if(fun0 < 500.0) stepD = 0.0001;
  else if(fun0 < 10000.0) stepD = 0.001;
  else stepD = 0.01;

  agrad = 0.0;
  for(Int_t k = 0; k < numOfParam; k++) {
    scalePar[k] = 1.0;
    Double_t park = par[k];
    par[k] = park + stepD;
    Double_t funk = valueOfFunctional(par);
    par[k] = park - stepD;
    Double_t funk1 = valueOfFunctional(par);
    grad2[k][k] = (funk + funk1 - 2.0*fun0)/(stepD*stepD);
    if(iflag == 1) scalePar[k] = (grad2[k][k]<0.001) ? 1.0 : sqrt(grad2[k][k]);
    grad[k] = (funk - funk1)/(2.0*stepD) / scalePar[k];  // ! scaling !
    par[k] = park;

    for(Int_t l = k+1; l < numOfParam; l++) {
      if(iflag != 2) grad2[k][l] = grad2[l][k] = 0;
      else {
        par[k] = park + stepD;
        Double_t parl = par[l];
        par[l] = parl + stepD;
        Double_t funkl = valueOfFunctional(par);
        par[k] = park;
        Double_t funl = valueOfFunctional(par);
        par[l] = parl;
        grad2[k][l] = grad2[l][k] = (funkl + fun0 - funk - funl)/(stepD*stepD);
      }
    }
    agrad += grad[k]*grad[k];
  }
  agrad = sqrt(agrad);
  for(Int_t k = 0; k < numOfParam; k++) cosgrad[k] = grad[k]/agrad;
  return;
}

//-----------------------------------------------------
void HMdcTrackFitterA::solutionOfLinearEquationsSystem(Double_t* par) {
  // input:  matrix grad2[i][j] (i-string, j-column),  vector grad[i]
  //output;  new param. in vector par[i]
  Double_t a[10][11];
  Int_t ieq[10];

  for(int i = 0; i < numOfParam; i++) {
    for(int j = 0; j < numOfParam; j++) a[i][j] = grad2[i][j];
    a[i][numOfParam] = -grad[i];
    ieq[i] = -1;
  }

  Int_t iMax = 0;
  Int_t jMax = 0;
  for(int l = 0; l < numOfParam; l++) {
    Double_t maxA = 0.0 ;
    for(int i = 0; i < numOfParam; i++) {
      if(ieq[i] != -1) continue;
      for(int j = 0; j < numOfParam; j++) {
	if(fabs(a[i][j]) <= maxA) continue;
	maxA = fabs(a[i][j]);
	iMax = i;
	jMax = j;
      }
    }
    ieq[iMax] = jMax;
    Double_t corr = a[iMax] [jMax];
    for(int j = 0; j <= numOfParam; j++) a[iMax][j] /= corr;
    for(int i = 0; i < numOfParam; i++) {
      if(i == iMax) continue;
      corr = a[i][jMax];
      for(int j = 0; j <= numOfParam; j++) a[i][j] -= a[iMax][j]*corr;
    }
  }

  for(int i = 0; i < numOfParam; i++) {
    Int_t iout = ieq[i];
    if(iout>=0) par[iout] += a[i][numOfParam]; //!!!
  }

  return;
}

//-----------------------------------------------------
void HMdcTrackFitterA::downhillOnGradient(Double_t* parIn,  Double_t* parOut) {
  Double_t functi,pari[10];
  Int_t istepinc = 0;
  Int_t istepdec = 0;
  Int_t nStepCh = 0;
  Double_t  stepCh[6] = {2.0, 1.5, 1.5, 2.0, 1.2, 1.1};
  Double_t  dFdRi;
  Double_t  dfunctmax = 1.5;
  Double_t  dfunctmin = 0.7;
  Int_t iprint = 0;

  for(Int_t k = 0; k < numOfParam; k++)
      pari[k] = parIn[k] - cosgrad[k]*0.001/scalePar[k];
    Double_t dFdR0 = (valueOfFunctional(pari)/functional - 1.0)/(0.001);

  while(kTRUE) {

    for(Int_t k = 0; k < numOfParam; k++)
      pari[k] = parIn[k] - cosgrad[k]*stepFit/scalePar[k];
    functi = valueOfFunctional(pari)/functional;
    for(Int_t k = 0; k < numOfParam; k++)
      pari[k] = parIn[k] - cosgrad[k]*stepFit*1.001/scalePar[k];
    dFdRi = (valueOfFunctional(pari)/functional-functi)/(stepFit*0.001);

    if(dFdRi < 0) {
      if(functi > 1.0 && istepdec < 2) {
        stepFit = stepFit/5;
        istepdec++;
        if(fprint && iprint == 1) printStep("de",functi,stepFit);
      } else {
        if(istepinc ==2) break;
        nStepCh = 3*istepinc + istepdec;
        stepFit *= stepCh[nStepCh];
        istepinc++;
        if(fprint && iprint == 1) printStep("in",functi,stepFit);
      }
    } else {   //dFdRi >= 0
      if(functi < dfunctmin) {
        if(istepinc ==2) break;
        nStepCh = 3*istepinc + istepdec;
        stepFit *= stepCh[nStepCh];
        istepinc++;
        if(fprint && iprint == 1) printStep("in",functi,stepFit);
      } else if(functi > dfunctmax) {
        if(istepdec ==2) break;
        nStepCh = 3*istepdec + istepinc;
        stepFit /= stepCh[nStepCh];
        istepdec++;
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

  for(Int_t k = 0; k < numOfParam; k++)
      parOut[k] = parIn[k] -cosgrad[k]*stepFit1/scalePar[k];
  newFunctional = valueOfFunctional(parOut);
  if(newFunctional > functional) {
    for(Int_t k = 0; k < numOfParam; k++)
        parOut[k] = parIn[k] -cosgrad[k]*0.3*stepFit1/scalePar[k];
    newFunctional = valueOfFunctional(parOut);
  }
  return;
}

void HMdcTrackFitterA::printStep(char* cond, Double_t fun, Double_t step) {
  printf("  *functi(dFdRi>0)=%g  *!  step is %screased, now stepFit=%g\n",
      fun,cond,step);
}

//New!!!!!!!!-----------------------------------------------------
Double_t HMdcTrackFitterA::valueOfFunctional(Double_t* par, Int_t iflag) {
  flDstTime=(iflag==3 || (fprint && iflag==1)) ? kTRUE:kFALSE;
  Double_t xV = par[0];
  Double_t yV = par[1];
  Double_t zV = parD1-parA1*xV-parB1*yV; //!!! zVertex;
  Double_t xPlane = par[2];
  Double_t yPlane = par[3];
  Double_t zPlane = parD2-parA2*xPlane-parB2*yPlane; //!!! zInitPlane;

  Double_t sumdev = 0;
  Double_t sumweight = 0;

  Double_t functional = 0.0;

  for(int time = firstTime; time < nClTimes; time++) {
    if(hitSt[time]==1  || (hitSt[time]<0 && flDstTime)) {
      drTime[time] = calcMdcTime(time,xV,yV,zV,xPlane,yPlane,zPlane);
      dev[time] = drTime[time] - tdcTime[time];
      if(hitSt[time]==1) {
        Double_t wt=(useTukey) ? weight[time] : qualityWeight[time];
        wt /= errTdcTime[time]*errTdcTime[time];
        sumdev += dev[time]*wt;
        sumweight += wt;
      }
    }
  }

  par[4] = -sumdev/sumweight;
  if(iflag >=10 && par[4]<-5.0) par[4] = -5.0;  //was 3.0

  for(int time = firstTime; time < nClTimes; time++) {
    if(hitSt[time]==1  || (hitSt[time]<0 && flDstTime)) {
      dev[time] += par[4];
      Double_t chi=dev[time]/errTdcTime[time];
      chi2[time] = chi*chi;
      if(hitSt[time]==1) {
        if(useTukey) functional += chi2[time]*weight[time];
        else functional += chi2[time]*qualityWeight[time];
      }
    }
    if(fprint && (iflag==1 || iflag==3 || iflag==11)) printTime("Wt=T",time,iflag,par[4]);
  }
  return functional;
}
//------------------------------------------------------------------------------
Int_t HMdcTrackFitterA::filterOfHits( Double_t* par, Int_t& numberOfHits ) {
// Filtering of bad hits using Tukey weights

  Double_t tukeWeight;
  Double_t sigma;
  Double_t cw = 6.4516;  //2.54*2.54;
  Double_t c2 = 17.5561; //4.19*4.19;
  Double_t c4 = 10.6276; //3.26*3.26;

  for(Int_t j = 0; j < 4; j++) {
    Double_t sumDist2 = 0.0;
    Double_t sumWeight = 0.0;
    for(int time = firstTime; time < nClTimes; time++) {
      if(hitSt[time]==1 && qualityWeight[time] > 0.001) {
        sumDist2 += chi2[time]*weight[time];
        sumWeight += weight[time];
      }
    }
    sigma = sumDist2/sumWeight;

    if(sqrt(sigma) < 2.5) {
      if(fprint) cout<< "   Not filtering!-->  sigma=" << sqrt(sigma) << endl;
      return numberOfHits;
    }

    for(int time = firstTime; time < nClTimes; time++) {
      if(hitSt[time]==1 && qualityWeight[time] > 0.001) {
        if(chi2[time] < c2*sigma) {
          if(chi2[time] < cw*sigma) {
            tukeWeight=chi2[time]/(c4*sigma);
            tukeWeight = 1. - tukeWeight*tukeWeight;
          } else tukeWeight = 1. - chi2[time]/(c2*sigma);
        }
        else tukeWeight = 0;
        weight[time] = qualityWeight[time] * tukeWeight*tukeWeight;
      }
    }
  }
  if(fprint) cout<< "   FILTER ! -->  sigma=" << sqrt(sigma) << endl;

  return  numberOfHits;
}
