#include "hmdctrackfittera.h"
#include "hmdctrackfitpar.h"
#include "hmdcsizescells.h"
#include "hmdccal2parsim.h"

//*-- Author : A.P.Jerusalimov
//*-- Modified: 27.02.2002
//*-- Modified: 07.05.2002 by V.Pechenov

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
  numOfParam = 5;
  Int_t mdc;
  if(nMDC1Times>0 && nMDC2Times==0) mdc=segment*2;
  else if(nMDC1Times==0 && nMDC2Times>0) mdc=1+segment*2;
  else mdc=-1;
  HMdcTrackFitParSec& fitParSec=(*fitPar)[sector];
  if(mdc>=0) {
    HMdcTrackFitParMod& fitParMod=fitParSec[mdc];
    if(mdc==0) driftVelMDC1 = fitParMod.getDriftVelocityMDC();
    if(mdc==1) driftVelMDC2 = fitParMod.getDriftVelocityMDC();
    if(mdc==2) driftVelMDC3 = fitParMod.getDriftVelocityMDC();
    if(mdc==3) driftVelMDC4 = fitParMod.getDriftVelocityMDC();
    //    fitParMod.getSteps(step);
    tukeyConst = fitParMod.getTukeyConstant();
    //    maxIteration = fitParMod.getMaxIteration();
  } else {
    if(segment==0) {
      driftVelMDC1 = fitParSec[0].getDriftVelocityMDC();
      driftVelMDC2 = fitParSec[1].getDriftVelocityMDC();
      //      fitParSec.getStepsSeg1(step);
      tukeyConst = fitParSec.getTukeyConstantSeg1();
      //      maxIteration = fitParSec.getMaxIterationSeg1();
    } else {
      driftVelMDC3 = fitParSec[2].getDriftVelocityMDC();
      driftVelMDC4 = fitParSec[3].getDriftVelocityMDC();
      //      fitParSec.getStepsSeg2(step);
      tukeyConst = fitParSec.getTukeyConstantSeg2();
      //      maxIteration = fitParSec.getMaxIterationSeg2();
    }
  }
  maxIteration = 500; //!!!!!!!!!!!!!!!!!!!!
  parameter[0] = xVertex;
  parameter[1] = yVertex;
  parameter[2] = xInitPlane;
  parameter[3] = yInitPlane;
  parameter[4] = 3;
  parSc=4;
}

//-----------------------------------------------------
void HMdcTrackFitterA::fillOutput(Double_t* par) {
  for(Int_t n=0; n<numOfParam; n++) parameter[n]=par[n];
  funMin = valueOfFunctional(parameter,3);
  xOutVertex = parameter[0];
  yOutVertex = parameter[1];
  zOutVertex = zVertex;
  xOutPlane = parameter[2];
  yOutPlane = parameter[3];
  zOutPlane = parD-parA*xOutPlane-parB*yOutPlane; //!!! zInitPlane;
  arriveTime = parameter[4];
}

//-----------------------------------------------------
Bool_t HMdcTrackFitterA::finalize() {
  return kTRUE;
}

//-----------------------------------------------------
void HMdcTrackFitterA::printResult(Char_t* status) const{
  printf("%s\n",status);
  printParam("final",parameter);
}

//-----------------------------------------------------
void HMdcTrackFitterA::printParam(Char_t* title, const Double_t *par) const {
  printf("%s %3i Par.=%-6g %-6g %-6g %-6g %-6g  fun.=%-6g->%-6g\n",
      title,iteration,par[0],par[1],par[2],par[3],par[4],
      functional,newFunctional);
}

//-----------------------------------------------------
Int_t HMdcTrackFitterA::execute(Double_t threshold, Int_t iter) {
  if(iter==0 || numOfParam==0) setParam();

  Int_t maxIter2 = 5;
  Int_t ittest = 2;
  Double_t functionalmin = 25.0;
  Double_t deltaFuncMin = 0.001;
  Double_t gradfunctionalmin = 0.1;
  Double_t gradFuncMeth2In = 3.0;
  Double_t par0[10], parNew[10];
  Double_t par1[10], pari[10];

  if(fprint) {
    printf("\n  ********************************\n");
    printf("  *** PROGRAM OF TRACK FITTING ***\n");
    printf("  ********************************\n");
  }

  Int_t numberOfGoodHits=nClTimes-firstTime;
  for(int time = firstTime; time < nClTimes; time++) {
    hitSt[time]=hitStatus[time];
    if(hitStatus[time]==1) qualityWeight[time] = weight[time] = 1;
    else {
      numberOfGoodHits--;
      qualityWeight[time] = weight[time] = 0;
    }
  }

  //**************   Filtering of bad hits   ****************************
  useTukey = kTRUE;
  filterOfHits(parNew,numberOfGoodHits);
  if(numberOfGoodHits < 5) return 0;

  //**************  End of filtering of bad hits  ***************
  if(fprint) {
    printf("\n  ----------------------------------");
    printf("\n  *** Begin of the FIT procedure ***\n");
    printf("  ----------------------------------\n");
  }
  //********* Initialisation of fit ****************************************

  useTukey = kFALSE; //????????????????????????????

  for(int time = firstTime; time < nClTimes; time++)
    if(qualityWeight[time] >= 0.001) weight[time] = 1.;


  stepFit=10.0;
  for(Int_t k = 0; k < numOfParam; k++ ) {
    scalePar[k] = 1.0;
    par0[k] = parameter[k];
  }

  functional = valueOfFunctional(par0,1);
  newFunctional = functional;
  derivativesOfFunctional(functional,par0);
  Int_t minimizationMethod = 1;
  Int_t iexit2 = 0;
  Int_t iteration2 = 0;
  if(fprint) printf("\n  *** Minimization by the 1-st method ***\n");
//********* End of initialization ****************************************

//******************* BEGIN of FIT PROCEDURE *****************************
  Double_t funct1=0;
  for (iteration = 0; iteration < maxIteration; iteration++ ) {
    functional = newFunctional;

    if(minimizationMethod == 1) {
      //**** 1 method of minimization (use mainly 1-st derivatives) ****

      downhillOnGradient(par0,parNew);

      if(fprint) printParam("out1",parNew);  //print of out data for each iter.
      derivativesOfFunctional(newFunctional,parNew);
      for(Int_t k = 0; k < numOfParam; k++) {
        par1[k] = par0[k];
        par0[k] = parNew[k];
        scalePar[k] = (k==parSc||grad2[k][k]<0.001||grad2[parSc][parSc]<0.001) ?
            1.0 : sqrt(grad2[k][k]/grad2[parSc][parSc]);
      }
      //*** Test for method of minimization ************************************
      if(iteration >= ittest) {
        if(newFunctional > functional) {
          minimizationMethod = 2;
          for(Int_t k = 0; k < numOfParam; k++) {
            scalePar[k] = 1.0;
            par0[k] = par1[k];
          }
          funct1 = functional;
        } else {
          Double_t agradTest = 0.0;
          for(Int_t k = 0; k < numOfParam; k++)
              agradTest += (cosgrad[k]*scalePar[k])*(cosgrad[k]*scalePar[k]);
          agradTest = agrad*sqrt(agradTest);

          for(Int_t k = 0; k < numOfParam; k++) par0[k] = parNew[k];
          Double_t deltaFunc = (functional - newFunctional)/functional;
          if( newFunctional <= functionalmin || 
              agrad < gradFuncMeth2In ||
              (newFunctional >= 1000 && deltaFunc < deltaFuncMin) ||
              (newFunctional <= 400 && deltaFunc < 10*deltaFuncMin) ||
              (newFunctional > 400 && newFunctional < 1000 && 
              deltaFunc < 2.5*deltaFuncMin)) {
            minimizationMethod = 2;
            funct1 = newFunctional;
            for(Int_t k = 0; k < numOfParam; k++) {
              par1[k] = par0[k];
              scalePar[k] = 1.0;
            }
          } else functional = newFunctional;
        }
      }
    } else if(minimizationMethod == 2) {
      //**** 2 method of minimization (use 2-nd derivatives) **********
      iexit2 = 0;
      for(Int_t k = 0; k < numOfParam; k++) pari[k] = par0[k];
      derivativesOfFunctional(functional,par0,1);
      solutionOfLinearEquationsSystem(par0);
      newFunctional = valueOfFunctional(par0);
      iteration2 = iteration2 + 1;
      if(fprint) printParam("out2",par0); //print of out data for each iter.
    }

  //**** Test for exit from iterations *************************************
    if(agrad <= gradfunctionalmin || newFunctional < 0.1) {
      fillOutput(par0);
      if(fprint) printResult(" conversion ok !!!");
      return ((newFunctional < threshold) ? 1 : 0);
    } //* Normal exit

    if(iteration2 >= 1 && newFunctional > (1000.0*numberOfGoodHits)) {
      iexit2 = 3;
      newFunctional = functional = funct1;
      for(Int_t k = 0; k < numOfParam; k++) par0[k] = par1[k];
      fillOutput(par0);
      if(fprint)  printResult(" conversion almost ok, exit2=3 ");
      return ((newFunctional < threshold) ? 1 : 0);
    }

    if(iteration2 == maxIter2) {
      if(newFunctional > funct1) {
        if(funct1 > functional) {  // functional < funct1 < newFunct.
        iexit2 = 1;
        newFunctional = functional;
        for(Int_t k = 0; k < numOfParam; k++) par0[k] = pari[k];
        functional = funct1;
        }
        else {  // funct1 < functional < newFunctional
        iexit2 = 2;
        newFunctional = functional = funct1;
        for(Int_t k = 0; k < numOfParam; k++) par0[k] = par1[k];
        }
      }
      else {  // newFunctional < funct1
        iexit2 = 1;
        if(newFunctional > functional) {  // functional < newFunct.< funct1
          funct1 = newFunctional;
          newFunctional = functional;
          functional = funct1;
          for(Int_t k = 0; k < numOfParam; k++) par0[k] = pari[k];
        }
      }
    }
    if(iexit2 > 0) {
      fillOutput(par0);
      if(fprint) {
        if(iexit2 == 1) printResult(" conversion almost ok, exit2=1 ");
        if(iexit2 == 2) printResult(" conversion almost ok, exit2=2 ");
      }
      return ((newFunctional < threshold) ? 1 : 0);
    }

  }  // end of iterations loop
  fillOutput(par0);
  if(fprint)  printResult(" no conversion ");
  return 0;
}

//-----------------------------------------------------
// Double_t HMdcTrackFitterA::valueOfFunctional(Double_t* par, Int_t iflag) {
//   Double_t xV = par[0];
//   Double_t yV = par[1];
//   Double_t zV = zVertex;
//   Double_t xPlane = par[2];
//   Double_t yPlane = par[3];
//   Double_t zPlane = parD-parA*xPlane-parB*yPlane; //!!! zInitPlane;
// 
//   Double_t sumDist2 = 0.0;
//   Double_t sumWeight = 0.0;
//   for(int time = firstTime; time < nClTimes; time++) {
//     if(qualityWeight[time] < 0.01) continue;
//     fullTime[time] = calcMdcTime(time,xV,yV,zV,xPlane,yPlane,zPlane) + par[4];
//     dev[time] = fullTime[time] - tdcTime[time];
//     if(useTukey) {
//       sumDist2 += dev[time]*dev[time]*weight[time];
//       sumWeight += weight[time];
//     }
//   }
//   Double_t width = 0.0;
//   if(useTukey) width = tukeyConst*sqrt(sumDist2/sumWeight);
//   Double_t funct=0.0;
//   for(int time = firstTime; time < nClTimes; time++) {
//     if(qualityWeight[time] < 0.001) continue;
//     if(useTukey) {
//       Double_t tukeWeight;
//       if(dev[time] < width) {
//         tukeWeight=dev[time]/width;
//         tukeWeight = 1.0 - tukeWeight*tukeWeight;
//         weight[time] = qualityWeight[time] * tukeWeight*tukeWeight;
//       } else weight[time] = 0.;
//     }
//     funct += dev[time]*dev[time]*qualityWeight[time];
//     if(fprint) {
//       if((iflag == 1 || iflag == 3)) {
//         printf(" iflag=%i time=%-3i arriveTime=%-7.4g", iflag,time,par[4]);
//         printf(" driftTime=%-7.4g fullTime=%-8.5g",
//             fullTime[time]-par[4], fullTime[time]);
//         printf(" TDCTime=%-8.5g dev=%-8.5g weight=%g\n",
//                tdcTime[time],dev[time],weight[time]);
//       }
//     }
//   }
//   if(fprint && useTukey && iflag == 3)
//     printf("========================== width=%f\n",width);
//   return funct;
// }

Double_t HMdcTrackFitterA::valueOfFunctional(Double_t* par, Int_t iflag) {
  flDstTime=(iflag==3 || (fprint && iflag==1)) ? kTRUE:kFALSE;
  Double_t xV = par[0];
  Double_t yV = par[1];
  Double_t zV = zVertex;
  Double_t xPlane = par[2];
  Double_t yPlane = par[3];
  Double_t zPlane = parD-parA*xPlane-parB*yPlane; //!!! zInitPlane;

  Double_t funct=0.0;
  if(useTukey) {   // * tukeyWeight----------------------------
    Double_t sumDist2 = 0.0;
    Double_t sumWeight = 0.0;
    for(int time = firstTime; time < nClTimes; time++) {
      if(hitSt[time]==1 || (hitSt[time]<0 && flDstTime)) {
        fullTime[time] = calcMdcTime(time,xV,yV,zV,xPlane,yPlane,zPlane) + par[4];
        dev[time] = fullTime[time] - tdcTime[time];
        if(hitSt[time]!=1) continue;
        sumDist2 += dev[time]*dev[time]*weight[time];
        sumWeight += weight[time];
      }
    }
    Double_t width = tukeyConst*sqrt(sumDist2/sumWeight);
    for(int time = firstTime; time < nClTimes; time++) {
      if(hitSt[time]==1 || (hitSt[time]<0 && flDstTime)) {
        if(hitSt[time]==1) {
          if(dev[time] < width) {
            Double_t tukeWeight=dev[time]/width;
            tukeWeight = 1.0 - tukeWeight*tukeWeight;
            weight[time] = qualityWeight[time] * tukeWeight*tukeWeight;
          } else weight[time] = 0.;
    //      funct += dev[time]*dev[time]*weight[time];
          funct += dev[time]*dev[time]*qualityWeight[time]; // ??????
        }
        if(fprint && (iflag==1 || iflag==3)) printTime("Wt=T",time,iflag,par[4]);
      }
    }
    if(fprint && iflag == 3) printf("======================= width=%f\n",width);
  } else {  //  * qualityWeight[time]-----------------------------
    for(int time = firstTime; time < nClTimes; time++) {
      if(hitSt[time]==1 || (hitSt[time]<0 && flDstTime)) {
        fullTime[time] = calcMdcTime(time,xV,yV,zV,xPlane,yPlane,zPlane)+par[4];
        dev[time] = fullTime[time] - tdcTime[time];
        if(hitSt[time]==1) funct += dev[time]*dev[time]*qualityWeight[time];
        if(fprint && (iflag==1||iflag==3)) printTime("Wt=Q",time,iflag,par[4]);
      }
    }
  }
  return funct;
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
    //     Double_t dp = xPlane - xVertex;
    //     Double_t ep = yPlane - yVertex;
    //     Double_t fp = zPlane - zVertex;
    //     Double_t ap = xRight[time] - xLeft[time];
    //     Double_t bp = yRight[time] - yLeft[time];
    //     Double_t cp = zRight[time] - zLeft[time];
    // 
    //     distance = fabs((cp*ep - bp*fp)*(xLeft[time] - xVertex) +
    // 	        (ap*fp - cp*dp)*(yLeft[time] - yVertex) +
    // 		(bp*dp - ap*ep)*(zLeft[time] - zVertex))/
    // 	   sqrt((cp*ep - bp*fp)*(cp*ep - bp*fp) +
    // 		(ap*fp - cp*dp)*(ap*fp - cp*dp) +
    // 		(bp*dp - ap*ep)*(bp*dp - ap*ep) ) ;
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
//new----------------------- // version = 1 ------------------------------------
    driftTime=fCal2ParSim->calcTime(sector,lModule[time],alpha,distance);
//------------------------------------------------------------------------------
  }
  return driftTime;
}

void HMdcTrackFitterA::printTime(char* title, Int_t time, Int_t iflag, 
    Double_t aTm) {
  printf(" %s %s %3i)%c tof=%6.2f", title,(iflag==1) ? "->":"<-" ,time,
      (hitSt[time]==1) ? '+':'-',aTm);
  printf(" %5.2fmm %s",distWire[time],(inCell[time]) ? "   ":"OUT");
  printf(" drTm=%6.2f fTm=%6.2f",fullTime[time]-aTm,fullTime[time]);
  printf(" TDC=%6.2f dev=%7.2f WT=%g\n",tdcTime[time],dev[time],weight[time]);
} 

//-----------------------------------------------------
void HMdcTrackFitterA::derivativesOfFunctional( Double_t fun0, Double_t* par, 
    Int_t iflag) {

  Double_t stepD;
  if(fun0 < 50000.0) stepD = 0.001;
  else if(fun0 < 1000000.0) stepD = 0.01;
  else stepD = 0.1;

  agrad = 0.0;
  for(Int_t k = 0; k < numOfParam; k++) {
    Double_t park = par[k];
    par[k] = park + stepD;
    Double_t funk = valueOfFunctional(par);
    par[k] = park - stepD;
    Double_t funk1 = valueOfFunctional(par);
    grad[k] = (funk - funk1)/(2.0*stepD) / scalePar[k];  // ! scaling !
    grad2[k][k] = (funk + funk1 - 2.0*fun0)/(stepD*stepD);
    par[k] = park;

    for(Int_t l = k+1; l < numOfParam; l++) {
      if(iflag == 0) grad2[k][l] = grad2[l][k] = 0;
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
    // par[iout] += a[i][numOfParam];
    if(iout>=0) par[iout] += a[i][numOfParam]; //!!!!!!!!!!!!!!!!!!!!!!
  }
  return;
}

//-----------------------------------------------------
Int_t HMdcTrackFitterA::filterOfHits( Double_t* parOut, Int_t& numberOfHits ) {
  // Filtering of bad hits
  Int_t ittest = 2;
  if(fprint)
      printf("\n  *** Filtering of bad hits after %i iteration ***\n\n",ittest);

  Double_t par0[10], parNew[10];
  Double_t  fullTimeLim[2] = {200.,1000.};
  while(kTRUE){
    stepFit=10.0;
    for(Int_t k = 0; k < numOfParam; k++ ) {
      scalePar[k] = 1.0 ;
      par0[k] = parameter[k] ;
    }
    functional = valueOfFunctional(par0,1);
    for (iteration = 1; iteration <= ittest; iteration++) {  //Iter. loop
      derivativesOfFunctional(functional,par0);
      if(iteration > 1) {
        for(Int_t k = 0; k < numOfParam; k++) scalePar[k] =
            (k==parSc||grad2[k][k]<0.001||grad2[parSc][parSc]<0.001) ?
            1.0 : sqrt(grad2[k][k]/grad2[parSc][parSc]);
      }

      downhillOnGradient(par0,parNew);
      if(fprint) printParam("outT",parNew); //print of out data for each iter.
      functional = newFunctional;
      for(Int_t k = 0; k < numOfParam; k++) par0[k] = parNew[k];
    }
    //**************    Test for bad Hits   ***************
    Double_t fullTimeMax = -100.0;
    Int_t nFullTimeMax = 0;
    for(int time = firstTime; time < nClTimes; time++) {
      if(qualityWeight[time] > 0.001 && fullTime[time] > fullTimeMax) {
        fullTimeMax = fullTime[time];
        nFullTimeMax = time;
      }
    }
    if(fullTimeMax > fullTimeLim[segment]) {
      qualityWeight[nFullTimeMax] = 0;
      weight[nFullTimeMax] = 0;
      hitSt[nFullTimeMax]=-1;
      if(fprint) Printf(" ! HIT %i is deleted, fullTime=%g",
      nFullTimeMax,fullTime[nFullTimeMax]);
      if(--numberOfHits < 5) {
        if(fprint)  printResult(" no conversion: too few hits ");
        return numberOfHits;
      }
      continue;
    }
    break;
  }
  for(Int_t k = 0; k < numOfParam; k++ ) parOut[k] = parNew[k] ;
  if(fprint) functional = valueOfFunctional(parOut,3);
  return numberOfHits;
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
        if(fprint && iprint == 1) cout << "  *functi(dFdRi<0)=" << functi <<
                " *! step is decreased, now stepFit=" << stepFit << endl;
        continue;
      } else {
        if(istepinc ==2) break;
        nStepCh = 3*istepinc + istepdec;
        stepFit *= stepCh[nStepCh];
        istepinc++;
        if(fprint && iprint == 1) cout << "  *functi(dFdRi<0)=" << functi <<
                " *!  step is increased, now stepFit=" << stepFit << endl;
        continue;
      }
    } else {   //dFdRi > 0
      if(functi < dfunctmin) {
        if(istepinc ==2) break;
        nStepCh = 3*istepinc + istepdec;
        stepFit *= stepCh[nStepCh];
        istepinc++;
        if(fprint && iprint == 1) cout << "  *functi(dFdRi>0)=" << functi <<
                " *!  step is increased, now stepFit=" << stepFit << endl;
        continue;
      } else {
        if(functi > dfunctmax) {
          if(istepdec ==2) break;
          nStepCh = 3*istepdec + istepinc;
          stepFit /= stepCh[nStepCh];
          istepdec++;
          if(fprint && iprint == 1) cout << "  *functi(dFdRi>0)=" << functi <<
                  " *!  step is decreased, now stepFit=" << stepFit << endl;
          continue;
        }
      }
    }
    break;
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
