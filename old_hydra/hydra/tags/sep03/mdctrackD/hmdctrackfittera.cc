#include "hmdctrackfittera.h"
#include "hmdctrackfitpar.h"
#include "hmdcsizescells.h"
#include "hmdccal2parsim.h"
#include <stdlib.h>

//*-- Author : A.P.Jerusalimov
//*-- Modified: 15.07.2003 by V.Pechenov
//*-- Modified: 27.02.2002
//*-- Modified: 24.03.2002 by V.Pechenov
//*-- Modified: 17.07.2002 by A.P.Jerusalimov
//*-- Modified: 08.08.2002 by A.P.Jerusalimov
ClassImp(HMdcTrackFitterA)

//-----------------------------------------------------
HMdcTrackFitterA::~HMdcTrackFitterA(void) {
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
    if(mdc==0) driftVelMDC1 = fitInOut->fitPar->getDriftVelocityMDC(sector,mdc);
    if(mdc==1) driftVelMDC2 = fitInOut->fitPar->getDriftVelocityMDC(sector,mdc);
    if(mdc==2) driftVelMDC3 = fitInOut->fitPar->getDriftVelocityMDC(sector,mdc);
    if(mdc==3) driftVelMDC4 = fitInOut->fitPar->getDriftVelocityMDC(sector,mdc);
    //    fitInOut->fitPar->getSteps(sector,mdc,step);
    //    tukeyConst = fitInOut->fitPar->getTukeyConstant(sector,mdc);
    //    maxIteration = fitInOut->fitPar->getMaxIteration(sector,mdc);
  } else {
    if(segment==0) {
      driftVelMDC1 = fitInOut->fitPar->getDriftVelocityMDC(sector,0);
      driftVelMDC2 = fitInOut->fitPar->getDriftVelocityMDC(sector,1);
      //      fitInOut->fitPar->getStepsSeg1(sector,step);
      //      tukeyConst = fitInOut->fitPar->getTukeyConstantSeg1(sector);
      //      maxIteration = fitInOut->fitPar->getMaxIterationSeg1(sector);
    } else {
      driftVelMDC3 = fitInOut->fitPar->getDriftVelocityMDC(sector,2);
      driftVelMDC4 = fitInOut->fitPar->getDriftVelocityMDC(sector,3);
      //      fitInOut->fitPar->getStepsSeg2(sector,step);
      //      tukeyConst = fitInOut->fitPar->getTukeyConstantSeg2(sector);
      //      maxIteration = fitInOut->fitPar->getMaxIterationSeg2(sector);
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
  zOutVertex = firstPl->getZOnPlane(xOutVertex,yOutVertex);
//  zOutVertex = parD1-parA1*xOutVertex-parB1*yOutVertex; //!!! zVertex;
  xOutPlane = parameter[2];
  yOutPlane = parameter[3];
  zOutPlane = secondPl->getZOnPlane(xOutPlane,yOutPlane);
//  zOutPlane = parD2-parA2*xOutPlane-parB2*yOutPlane; //!!! zInitPlane;
  arriveTime = parameter[4];
}

//-----------------------------------------------------
Bool_t HMdcTrackFitterA::finalize() {
  return kTRUE;
}

//-----------------------------------------------------
void HMdcTrackFitterA::printResult(Char_t* status,Double_t* par) {
  printf("%s\n",status);
  printParam("final",par);
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

  for(int time = firstTime; time < lastTime; time++) {
    if(hitStatus[time]==1) qualityWeight[time] = weight[time] = 1;
    else {
      if(fprint) printf("  Warn.!  HIT %i is deleted\n",time);
      qualityWeight[time] = weight[time] = 0.0;
    }
  }

  for(Int_t k = 0; k < numOfParam; k++ )  par0[k] = parameter[k];
  for(int time = firstTime; time < lastTime; time++) {
    errTdcTime[time] = 1.;
    oneDErrTdcTime2[time] = 1.;
  }

  Int_t iteration2 = 0;
  Int_t iexit = 0;

  functional = valueOfFunctional(par0,10);
  for(Int_t k = 0; k <= numOfParam; k++ )  parameter[k] = par0[k]; //new
  calcTdcErrors(par0);
  functional = valueOfFunctional(par0,10);
  filterOfHits();
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
        for(Int_t k = 0; k <= numOfParam; k++ )  parameter[k] = par0[k]; //new
        calcTdcErrors(par0);
        functMin = newFunctional = valueOfFunctional(par0,11);
        for(Int_t k = 0; k <= numOfParam; k++) parMin[k] = par0[k];
        derivativesOfFunctional(newFunctional,par0,2);
      }
        else if(iterAfterFilter == limIter1forFilter) {
        funct1beforeFilter = newFunctional;
        filterOfHits();
        iterAfterFilter = 0;
        for(Int_t k = 0; k <= numOfParam; k++ )  parameter[k] = par0[k]; //new
        calcTdcErrors(par0); //new
        newFunctional = valueOfFunctional(par0);
        derivativesOfFunctional(newFunctional,par0,1);
      }
    } else {
//*** 2-nd method
      for(Int_t k = 0; k < numOfParam; k++) pari[k] = par0[k];
      solutionOfLinearEquationsSystem(par0,numOfParam);
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
        if(fabs(pari[k] - par0[k]) > limStep2[k]) iexit = 3;
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
        for(Int_t k = 0; k < numOfParam; k++) {
          if(fabs(pari[k] - par0[k]) > 1.5*limStep2[k]) {
            iexit = 3;
            break;
          }
        }
      }
      if(iexit == 1 || iexit == 2 ||
         (return2to1 > 0 && iteration2 == limIter2)) {
        for(Int_t k = 0; k <= numOfParam; k++) par0[k] = parMin[k];
        functional = newFunctional;
        newFunctional =  functMin;
        filterOfHits();
        for(int time = firstTime; time < lastTime; time++)
            weight[time]=(chi2[time]<maxChi2 || weight[time]>minWeight) ? 1.:0.;
        functMin = newFunctional = valueOfFunctional(par0);
        if(functMin > 100000.0) {  //?????????
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
      filterOfHits();
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
    if(iexit == 1) printResult(" conversion ok, exit=1 ",par0);
    if(iexit == 2) printResult(" conversion ok, exit=2 ",par0);
    if(iexit == 3) printResult(" no conversion, exit=3 ",par0);
    if(iexit == 4) printResult(" no conversion, exit=4 ",par0);
  }
//   if(iexit > 2) return 0; //Only precise results !!!!
  if(iexit > 3) return 0; //Only precise results !!!!
  Int_t numOfGoodWires = 0;
  for(int time = firstTime; time < lastTime; time++)
      if(weight[time] > 0.1) numOfGoodWires++;
  if(numOfGoodWires < 5) return 0; //Not less than 5 wires !!!
//  if(par0[4] < -15.0 || par0[4] > 30.0) return 0;  // Too large t-off !!!

  calculationOfErrors(par0);  //Errors calculations
  fillOutput(par0);
  if(fprint) printf("    !!! GOOD EXIT !!!\n");

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
      (hitStatus[nt]==1)?'+':'-',lModule[nt]+1,lLayer[nt]+1,lCell[nt]+1);
  printf(" %5.2fmm",distWire[nt]);
  if(flDstTime) printf(" %c",inCell[nt] ? 'I':'O');
  printf(" dev=%5.1f%+4.1f%+6.1f=%5.1fns",drTime[nt],aTm,-tdcTime[nt],dev[nt]);
  printf(" dT=%4.1f chi2=%5.1f WT=%g\n",errTdcTime[nt],chi2[nt],weight[nt]);
}

//-----------------------------------------------------
void HMdcTrackFitterA::derivativesOfFunctional( Double_t fun0, Double_t* par,
    Int_t iflag) {
  // iflag==3 -errors calculation
  Double_t stepD;

  if(fun0 < 500.0) stepD = 0.0001;
  else if(fun0 < 10000.0) stepD = 0.001;
  else stepD = 0.01;
  Double_t oneDv2StepD=1./(2.*stepD);
  Double_t oneDvStepD2=1./(stepD*stepD);

  agrad = 0.0;
  for(Int_t k = 0; k < numOfParam; k++) {
    Double_t park = par[k];
    
    par[k] = park + stepD;
    Double_t funk = valueOfFunctional(par);
    if(iflag==3) for(int time=firstTime; time<lastTime; time++) 
      dTdA[k][time]=drTime[time];
    
    par[k] = park - stepD;
    Double_t funk1 = valueOfFunctional(par);
    if(iflag==3) {
      for(int time=firstTime; time<lastTime; time++) {
        dTdA[k][time] -= drTime[time];
        dTdA[k][time] *= oneDv2StepD; // == dTdA[k][time]/(2.0*stepD);
      }
    }
    
    grad2[k][k] = (funk + funk1 - 2.0*fun0)*oneDvStepD2;
    grad[k] = (funk - funk1)*oneDv2StepD;  // ! scaling !    
    if(iflag == 1 && grad2[k][k]>0.001) {
      scalePar[k] = 1./sqrt(grad2[k][k]);
      grad[k] *=scalePar[k];
    } else scalePar[k] = 1.0;
    
    agrad += grad[k]*grad[k];
    par[k] = park;

    if(iflag < 2) continue;
    for(Int_t l = k+1; l < numOfParam; l++) {
      par[k] = park + stepD;
      Double_t parl = par[l];
      par[l] = parl + stepD;
      Double_t funkl = valueOfFunctional(par);
      par[k] = park;
      Double_t funl = valueOfFunctional(par);
      par[l] = parl;
      grad2[k][l] = grad2[l][k] = (funkl + fun0 - funk - funl)*oneDvStepD2;
    }   
  }
  agrad = sqrt(agrad);
  for(Int_t k = 0; k < numOfParam; k++) cosgrad[k] = grad[k]/agrad;
  return;
}

//-----------------------------------------------------
void HMdcTrackFitterA::solutionOfLinearEquationsSystem(Double_t* par,
     Int_t nmOfPar) {
  // input:  matrix grad2[i][j] (i-string, j-column),  vector grad[i]
  //output;  new param. in vector par[i]
  Double_t a[10][11];
  Double_t invM[10][10];    //+++
  Int_t ieq[10];

  detMatrix = 1.0;    //+++
  
  for(int i = 0; i < nmOfPar; i++) {
    for(int j = 0; j < nmOfPar; j++) {
      a[i][j] = grad2[i][j];
      invM[i][j] = 0.0;    //+++
    }
    a[i][nmOfPar] = -grad[i];
    invM[i][i] = 1.0;    //+++
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
    detMatrix *= corr;    //+++
    for(int j = 0; j <= nmOfPar; j++) {
      a[iMax][j] /= corr;
      invM[iMax][j] /= corr;    //+++
    }
    for(int i = 0; i < nmOfPar; i++) {
      if(i == iMax) continue;
      corr = a[i][jMax];
      for(int j = 0; j <= nmOfPar; j++) {
        a[i][j] -= a[iMax][j]*corr;
        invM[i][j] -= invM[iMax][j]*corr;    //+++
      }
    }
  }

  for(int i = 0; i < nmOfPar; i++) {
    Int_t iout = ieq[i];
    if(iout>=0) {
      par[iout] += a[i][nmOfPar]; //!!!
      for(int j = 0; j < nmOfPar; j++) invMatrix[iout][j] = invM[i][j];//+++
    }
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
      pari[k] = parIn[k] - cosgrad[k]*0.001*scalePar[k];
    Double_t dFdR0 = (valueOfFunctional(pari)/functional - 1.0)/(0.001);

  while(kTRUE) {

    for(Int_t k = 0; k < numOfParam; k++)
      pari[k] = parIn[k] - cosgrad[k]*stepFit*scalePar[k];
    functi = valueOfFunctional(pari)/functional;
    for(Int_t k = 0; k < numOfParam; k++)
      pari[k] = parIn[k] - cosgrad[k]*stepFit*1.001*scalePar[k];
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
      parOut[k] = parIn[k] -cosgrad[k]*stepFit1*scalePar[k];
  newFunctional = valueOfFunctional(parOut);
  if(newFunctional > functional) {
    for(Int_t k = 0; k < numOfParam; k++)
        parOut[k] = parIn[k] -cosgrad[k]*0.3*stepFit1*scalePar[k];
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
  Double_t zV = firstPl->getZOnPlane(xV,yV);
//  Double_t zV = parD1-parA1*xV-parB1*yV; //!!! zVertex;
  Double_t xPlane = par[2];
  Double_t yPlane = par[3];
  Double_t zPlane = secondPl->getZOnPlane(xPlane,yPlane);
//  Double_t zPlane = parD2-parA2*xPlane-parB2*yPlane; //!!! zInitPlane;

  Double_t sumdev = 0;
  Double_t sumweight = 0;

  Double_t functional = 0.0;

  for(int time = firstTime; time < lastTime; time++) {
    if(hitStatus[time]==1  || (hitStatus[time]<0 && flDstTime)) {
      drTime[time] = calcMdcTime(time,xV,yV,zV,xPlane,yPlane,zPlane);
      dev[time] = drTime[time] - tdcTime[time];
      if(hitStatus[time]==1) {
        Double_t wt=(useTukey) ? weight[time] : qualityWeight[time];
        wt *= oneDErrTdcTime2[time]; //wt/errTdcTime^2
        sumdev += dev[time]*wt;
        sumweight += wt;
      }
    }
  }

  par[4] = -sumdev/sumweight;
  if(iflag >=10 && par[4]<-5.0) par[4] = -5.0;  //was 3.0

  for(int time = firstTime; time < lastTime; time++) {
    if(hitStatus[time]==1  || (hitStatus[time]<0 && flDstTime)) {
      dev[time] += par[4];
      Double_t chi=dev[time]/errTdcTime[time];
      chi2[time] = chi*chi;
      if(hitStatus[time]==1) {
        if(useTukey) functional += chi2[time]*weight[time];
        else functional += chi2[time]*qualityWeight[time];
      }
    }
    if(fprint && (iflag==1 || iflag==3 || iflag==11)) printTime("Wt=T",time,iflag,par[4]);
  }
  return functional;
}
//------------------------------------------------------------------------------
void HMdcTrackFitterA::filterOfHits(void) {
  // Filtering of bad hits using Tukey weights

  Double_t tukeWeight;
  Double_t sigma;
  Double_t cw = 6.4516;  //2.54*2.54;
  Double_t c2 = 17.5561; //4.19*4.19;
  Double_t c4 = 10.6276; //3.26*3.26;

  for(Int_t j = 0; j < 4; j++) {
    Double_t sumDist2 = 0.0;
    Double_t sumWeight = 0.0;
    for(int time = firstTime; time < lastTime; time++) {
      if(hitStatus[time]==1 && qualityWeight[time] > 0.001) {
        sumDist2 += chi2[time]*weight[time];
        sumWeight += weight[time];
      }
    }
    sigma = sumDist2/sumWeight;

    if(sqrt(sigma) < 2.5) {
      if(fprint) printf("   Not filtering!-->  sigma=%f\n",sqrt(sigma));
      return;
    }

    for(int time = firstTime; time < lastTime; time++) {
      if(hitStatus[time]==1 && qualityWeight[time] > 0.001) {
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
  if(fprint) printf("   FILTER ! -->  sigma=%f\n",sqrt(sigma));
}

void HMdcTrackFitterA::calculationOfErrors(Double_t* par) {
//Calculate Matrix of errors

  Double_t pari[10];
  Double_t G[10][10];
  Double_t H[10][10];
  Double_t gradi[10];
//  Double_t deti;
//  Double_t errInvG[10];
//  int nend;

  int numOfParG =  numOfParam + 1;

  for(int i=0; i<numOfParG; i++) {
    gradi[i] = 0.0;
    for(int j=0; j<numOfParG; j++) G[i][j] = 0.0;
  }
  for(int k = 0; k<numOfParam; k++) pari[k] = par[k];
  
  Double_t functi = valueOfFunctional(pari);
  derivativesOfFunctional(functi,pari,3);

  for(int i=0; i<numOfParam; i++) {
    for(int j = i; j<numOfParam; j++) {
      G[i][j] = G[j][i] = grad2[i][j];
      H[i][j] = 0.0;
      for(int time = firstTime; time<lastTime; time++) H[i][j] += 
        4.0*(dTdA[i][time]*dTdA[j][time])*oneDErrTdcTime2[time]*weight[time];
      H[j][i] = H[i][j];
    }
  }

  for(int k=0; k<numOfParam; k++) {
    for(int m=4; m<numOfParG; m++) G[k][m] = 0.0; //!!!
    for(int time=firstTime; time<lastTime; time++) G[k][4] +=  //[lModule[time]+4] !!!
         2.0*weight[time]*oneDErrTdcTime2[time]*dTdA[k][time];
    for(int m=4; m<numOfParG; m++) G[m][k] = G[k][m];
  }

  for(int time = firstTime; time<lastTime; time++)
    G[4][4] +=  2.0*oneDErrTdcTime2[time]*weight[time];//[lModule[time]+4][lModule[time]+4] !!!
  for(int j = numOfParam; j<numOfParG; j++) {
    H[j][j] = 2.0*G[j][j];
    for(int l = 0; l<j; l++) H[j][l] = H[l][j] = 2.0*G[j][l];
  }

//   for(int i = numOfParam; i <= 7; i++) if(G[i][i] < 0.1) numOfParG--;
    
//   for(int i = numOfParam; i<7; i++) {
//     nend = 7 - i;
//     while(G[i][i]<0.1 && nend>0) {
//       for(int j=0; j<numOfParam; j++) {
//         for(int n = 0; n<nend; n++) {
//           G[i+n][j] = G[j][i+n] = G[i+n+1][j];
//           H[i+n][j] = H[j][i+n] = H[i+n+1][j];
//         }
//         G[i+nend][j] = G[j][i+nend] = 0.0;
//         H[i+nend][j] = H[j][i+nend] = 0.0;
//       }
//       for(int n = 0; n<nend; n++) {
//         G[i+n][i+n] = G[i+n+1][i+n+1];
//         H[i+n][i+n] = H[i+n+1][i+n+1];
//       }
//       G[i+nend][i+nend] = 0.0;
//       H[i+nend][i+nend] = 0.0;
//       nend--;
//     }
//   }

  for(int i=0; i<numOfParG; i++) {
    for(int j=0; j<numOfParG; j++) grad2[i][j] = G[i][j];
  }
  solutionOfLinearEquationsSystem(pari,numOfParG /*-1 ???*/);
  for(int i=0; i < numOfParG; i++) {
    for(int k = 0; k < numOfParG; k++) {
      errMatrix[i][k] = 0.0;
      for(int j=0; j < numOfParG; j++) {
        for(int l = 0; l < numOfParG; l++)
          errMatrix[i][k] += invMatrix[i][j]*H[j][l]*invMatrix[l][k];
      }
    }
  }
  
// //---- Conv. into another coord. sys.   Proverka tol'ko !!!!
//   Double_t b[10];
//   Double_t dbda[10][10];
//   Double_t errMb[10][10];
// 
//   Double_t dX=par[2]-par[0];
//   Double_t dY=par[3]-par[1];
//   Double_t dZ=secondPl->getZOnPlane(par[2],par[3]) -
//               firstPl ->getZOnPlane(par[0],par[1]);   // 300.0;
//   if(numOfParG < 6) dZ = 30.0;
// 
//   Double_t r = sqrt(dX*dX + dY*dY + dZ*dZ);
//   b[0] = par[0];
//   b[1] = par[1];
//   b[2] = acos(dZ/r);
//   b[3] = atan2(dY,dX);
// 
//   dbda[0][0] = 1.0;
//   dbda[0][1] = dbda[0][2] = dbda[0][3] = dbda[0][4] = dbda[0][5] = 0.0;
// 
//   dbda[1][1] = 1.0;
//   dbda[1][0] = dbda[1][2] = dbda[1][3] = dbda[1][4] = dbda[1][5] = 0.0;
// 
//   dbda[2][0] = -dZ*dX/(r*r*sqrt(r*r - dZ*dZ));
//   dbda[2][1] = -dZ*dY/(r*r*sqrt(r*r - dZ*dZ));
//   dbda[2][2] =  dZ*dX/(r*r*sqrt(r*r - dZ*dZ));
//   dbda[2][3] =  dZ*dY/(r*r*sqrt(r*r - dZ*dZ));
//   dbda[2][4] = dbda[2][5] = 0.0;
// 
//   dbda[3][0] =  dY/(r*r - dZ*dZ);
//   dbda[3][1] =  dX/(r*r - dZ*dZ);
//   dbda[3][2] = -dY/(r*r - dZ*dZ);
//   dbda[3][3] = -dX/(r*r - dZ*dZ);
//   dbda[3][4] = dbda[3][5] = 0.0;
// 
//   dbda[4][4] = 1.0;
//   dbda[4][0] = dbda[4][1] = dbda[4][2] = dbda[4][3] = dbda[4][5] = 0.0;
// 
// //  dbda[5][5] = 1.0;
// //  dbda[5][0] = dbda[5][1] = dbda[5][2] = dbda[5][3] = dbda[5][4] = 0.0;
// 
//   for(int i=0; i<5; i++) {
//     for(int j=0; j<5; j++) {
//       errMb[i][j] = 0.0;
//       for(int k = 0; k<5; k++) {
//         for(int l = 0; l<5; l++)
//         errMb[i][j] += dbda[i][k]*dbda[j][l]*errMatrix[k][l];
//       }
//     }
//   }
// 
//   if(fprint) {
//     printf("\n   Input parameters:");
//     for(int i=0; i<5; i++) printf("  %9.3f",par[i]);
//     
//     printf("\n\n  Matrix of Errors:\n");
//     for(int i=0; i<5; i++) {
//       if(errMatrix[i][i] < 0.0) {
//         printf("      Caution !!!\n");
//         break;
//       }
//     }
//     printf("\n");
//     for(int i=0; i<5; i++) {
//       for(int j=0; j<5; j++) printf(" %10.3e",errMatrix[i][j]);
//       printf("\n\n");
//     }
//     printf("   -> Errors from Matrix of errors (in mkm and ps):\n");
//     Double_t errErrM[10];
//     for(int i=0; i<5; i++) {
//       errErrM[i] = 1000.0*sqrt(fabs(errMatrix[i][i]));
//       if(errMatrix[i][i] < 0.0) errErrM[i] = -errErrM[i];
//       printf(" %9.3f",errErrM[i]);
//     }
//     printf("\n\n");
// 
//     printf("\n\n  Transform Matrix:\n");
//     for(int i=0; i<5; i++) {
//       for(int j=0; j<5; j++) printf(" %10.3e",dbda[i][j]);
//       printf("\n\n");
//     }
// 
//     printf("\n   Another Input parameters:");
//     for(int i=0; i<5; i++) printf("  %9.3f",b[i]);
//     printf("\n\n");
// 
//     printf("\n\n  Another Matrix of Errors:\n");
//     for(int i=0; i<5; i++) {
//       if(errMb[i][i] < 0.0) {
//         printf("      Caution !!!\n");
//         break;
//       }
//     }
//     printf("\n");
//     for(int i=0; i<5; i++) {
//       for(int j=0; j<5; j++) printf(" %10.3e",errMb[i][j]);
//       printf("\n\n");
//     }
//     printf("   -> Errors from Matrix of errors (in mkm and mrad):\n\n");
//     for(int i=0; i<5; i++) {
//       errErrM[i] = 1000.0*sqrt(fabs(errMb[i][i]));
//       if(errMb[i][i] < 0.0) errErrM[i] = -errErrM[i];
//       printf(" %9.3f",errErrM[i]);
//     }
//     printf("\n\n");
// 
//   }
// 
// /*???*/  for(int i=0; i<5; i++) {    // Trans. of errors
//     for(int j=0; j<5; j++) errMatrix[i][j] = errMb[i][j];
//   }
// //----------------------------------------------------------
  
  return;
}
