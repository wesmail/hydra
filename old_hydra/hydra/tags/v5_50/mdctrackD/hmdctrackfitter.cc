#include "hmdctrackfitter.h"
#include "hdebug.h"
#include "hades.h"
//#include <iostream.h>
#include "TStopwatch.h"
#include "TMath.h"
#include "hmdctrackfitpar.h"
#include "hruntimedb.h"
#include "hmdcsizescells.h"
#include "hgeomvector.h"
#include "hmdccal2parsim.h"
#include "hmdcgetcontainers.h"

//*-- Author : G.Agakichiev
//*-- Modified: 7.5.99

ClassImp(HMdcTrackFitter)

HMdcTrackFitter::HMdcTrackFitter(Bool_t prnt) {
  xLeft = 0;
  xRight = 0;
  yLeft = 0;
  yRight = 0;
  zLeft = 0;
  zRight = 0;
  TDCTime = 0;
  fprint = prnt;
  if(fprint) timer=new TStopwatch();
  else timer=0;
  version=0;
}

HMdcTrackFitter::~HMdcTrackFitter(void) {
  if(timer) delete timer;
}

Bool_t HMdcTrackFitter::init() {
  fitPar=(HMdcTrackFitPar*)gHades->getRuntimeDb()->getContainer("MdcTrackFitPar");
  if(version != 0)
    fCal2ParSim=(HMdcCal2ParSim*)gHades->getRuntimeDb()->
        getContainer("MdcCal2ParSim");
  else fCal2ParSim=0;
  return kTRUE;
}

Bool_t HMdcTrackFitter::reinit(void) {
  if(version!=0 && !HMdcGetContainers::isInited(fCal2ParSim)) return kFALSE;
  if( !HMdcGetContainers::isInited(fitPar) ) return kFALSE;
  return kTRUE;
}

Double_t HMdcTrackFitter::getFunctional(void) {
  setParam();
  calculationOfFunctional(4);
  return calculationOfFunctional(5);
}

void HMdcTrackFitter::setParam(void) {
  numberOfParameter = 5;
  Int_t mdc;
  if(nMDC1Times>0 && nMDC2Times==0) mdc=segment*2;
  else if(nMDC1Times==0 && nMDC2Times>0) mdc=1+segment*2;
  else mdc=-1;
  HMdcTrackFitParSec& fitParSec=(*fitPar)[sector];
  if(mdc>=0) {
    HMdcTrackFitParMod& fitParMod=fitParSec[mdc];
    if(mdc==0) driftVelocityMDC1 = fitParMod.getDriftVelocityMDC();
    if(mdc==1) driftVelocityMDC2 = fitParMod.getDriftVelocityMDC();
    if(mdc==2) driftVelocityMDC3 = fitParMod.getDriftVelocityMDC();
    if(mdc==3) driftVelocityMDC4 = fitParMod.getDriftVelocityMDC();
    fitParMod.getSteps(step);
    fitParMod.getWeights(weightPar);
    stepNorm =  fitParMod.getStepNorm();
    cnst = fitParMod.getTukeyConstant();
    maxIteration = fitParMod.getMaxIteration();
  } else {
    if(segment==0) {
      driftVelocityMDC1 = fitParSec[0].getDriftVelocityMDC();
      driftVelocityMDC2 = fitParSec[1].getDriftVelocityMDC();
      fitParSec.getStepsSeg1(step);
      fitParSec.getWeightsSeg1(weightPar);
      stepNorm =  fitParSec.getStepNormSeg1();
      cnst = fitParSec.getTukeyConstantSeg1();
      maxIteration = fitParSec.getMaxIterationSeg1();
    } else {
      driftVelocityMDC3 = fitParSec[2].getDriftVelocityMDC();
      driftVelocityMDC4 = fitParSec[3].getDriftVelocityMDC();
      fitParSec.getStepsSeg2(step);
      fitParSec.getWeightsSeg2(weightPar);
      stepNorm =  fitParSec.getStepNormSeg2();
      cnst = fitParSec.getTukeyConstantSeg2();
      maxIteration = fitParSec.getMaxIterationSeg2();
    }
  }
  parameter[0] = xVertex;
  parameter[1] = yVertex;
  parameter[2] = xInitPlane;
  parameter[3] = yInitPlane;
  parameter[4] = 3;
}

Int_t HMdcTrackFitter::execute(Float_t threshold, Int_t iter) {
  if(fprint) printf("========= Iteration %i\n",iter);
  if(iter==0 || numberOfParameter==0) setParam();
  Double_t stepNormW=stepNorm;
  Double_t maxDelta, delta[numberOfParameter];
  newFunctional=0;
  if(fprint && timer) timer->Start();
//printParam();

  functional = calculationOfFunctional(1);

  for (iteration = 0; iteration < maxIteration; iteration++ ) {

    maxDelta = 0;
    for(Int_t i = 0; i < numberOfParameter; i++) {
      parameter[i] += stepNormW*step[i];
      delta[i] = (calculationOfFunctional(2) - functional)/(stepNormW*step[i]);
      parameter[i] -= stepNormW*step[i];
    }

    for(Int_t i = 0; i < numberOfParameter; i++)
      if (maxDelta < fabs(delta[i])) maxDelta = fabs(delta[i]);

    for(Int_t i = 0; i < numberOfParameter; i++) {
      parameter[i] -= stepNormW*step[i]*delta[i]*weightPar[i]/maxDelta;
    }
    newFunctional = calculationOfFunctional(2);

    if(fprint) printParam("out1");
    if(newFunctional < functional) functional = newFunctional;
    else {
      for(Int_t i = 0; i < numberOfParameter; i++)
	parameter[i] += 0.5*stepNormW*step[i]*delta[i]*weightPar[i]/maxDelta;

      newFunctional = calculationOfFunctional(2);
      if( newFunctional < functional ) functional = newFunctional;
      else {
	for(Int_t i = 0; i < numberOfParameter; i++)
	  parameter[i] += 0.5*stepNormW*step[i]*delta[i]*weightPar[i]/maxDelta;
	
	stepNormW /= 2;
      }
    }
    if(stepNormW >= 0.05) continue;
    if(fprint) printResult(" conversion ok ");
    fillOutput();
    return ((funMin < threshold) ? 1 : 0);
  }
  fillOutput();
  if(fprint)  printResult(" no conversion ");
  return 0;
}

void HMdcTrackFitter::fillOutput(void) {
  funMin = calculationOfFunctional(3);
  xOutVertex = parameter[0];
  yOutVertex = parameter[1];
  zOutVertex = zVertex;
  xOutPlane = parameter[2];
  yOutPlane = parameter[3];
  zOutPlane = zInitPlane;
  pVelocity = parameter[4];
}

Double_t HMdcTrackFitter::calculationOfFunctional(Int_t iflag) {

  Double_t xV, yV, zV;
  Double_t xPlane, yPlane, zPlane;
  Double_t pV;

//  fixed zPlane
  xV = parameter[0];
  yV = parameter[1];
  zV = zVertex;

  xPlane = parameter[2];
  yPlane = parameter[3];
  zPlane = zInitPlane;

  pV = parameter[4];

  return deviation(xV, yV, zV, xPlane, yPlane, zPlane, pV, iflag);
}

Double_t HMdcTrackFitter::deviation(
    Double_t xVertex, Double_t yVertex, Double_t zVertex,
    Double_t xPlane, Double_t yPlane, Double_t zPlane,
    Double_t particleVelocity, Int_t iflag) {

  Double_t sumDeviation, sumDeviation0;
  Double_t distance;
  Double_t distVertexMDC;
  Double_t driftVelocityMDC;
  Double_t driftTime, fullTime;
  Double_t sumWeight;
  static Double_t width;

  sumDeviation = 0;
  sumDeviation0 = 0;
  sumWeight = 0;

  Double_t ap, bp, cp;            // version = 0
  Double_t dp={0},ep={0},fp={0};  // version = 0
  Double_t alphaDrDist;  // version = 1
  Bool_t isInCell=kTRUE;      // version = 1
  if(version == 0) {
    dp = xPlane - xVertex;
    ep = yPlane - yVertex;
    fp = zPlane - zVertex;
  }

  for(int time = 0; time < nMDC1Times + nMDC2Times; time++) {
    if(hitStatus[time] != 1) continue;

  if(version == 0) {
    ap = xRight[time] - xLeft[time];
    bp = yRight[time] - yLeft[time];
    cp = zRight[time] - zLeft[time];

    distance = (ep*cp*xLeft[time] - ep*cp*xVertex - fp*bp*xLeft[time] + fp*bp*xVertex -
               dp*cp*yLeft[time] + dp*cp*yVertex + dp*bp*zLeft[time] - dp*bp*zVertex +
               ap*fp*yLeft[time] - ap*fp*yVertex - ap*ep*zLeft[time] + ap*ep*zVertex)/
               sqrt(ep*ep*cp*cp - 2*ep*cp*fp*bp + fp*fp*bp*bp+fp*fp*ap*ap -
                 2*fp*ap*dp*cp+dp*dp*cp*cp + dp*dp*bp*bp - 2*dp*bp*ep*ap+ep*ep*ap*ap);

    distWire[time]=distance=fabs(distance);
    // printf("old dist=%g ",distance);
    if(lModule[time] == 0) {
      distVertexMDC = distVertexMDC1;
      driftTime = ( distance / driftVelocityMDC1 )*1000.;
    } else if(lModule[time] == 1) {
      distVertexMDC = distVertexMDC2;
      //driftVelocityMDC = driftVelocityMDC2;
      driftTime = 5.85 + 4.30*distance + 18.88*distance*distance
	- 8.54*distance*distance*distance
	+ 1.48*distance*distance*distance*distance;
      //driftTime = ( distance / driftVelocityMDC2 )*1000.;
      //driftTime = driftTime*.9;    // for Dubna chambers B != 0
    } else if(lModule[time] == 2) {
      //distVertexMDC = distVertexMDC3;
      //driftVelocityMDC = driftVelocityMDC3;
      //driftTime = ( distance / driftVelocityMDC3 )*1000.;
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

       //    1  p0    -3.54201e-02   1.48897e+00   6.61675e-04   1.67073e-06
       //    2  p1     7.87210e+01   1.25067e+00   2.56053e-04   2.88928e-06
       //    3  p2    -2.87847e+02   5.06960e-01   1.37256e-04   1.12583e-05
       //    4  p3     5.12043e+02   1.25888e-01   2.44161e-04   4.00551e-05
       //    5  p4    -4.54673e+02   2.81771e-02   2.16805e-04   9.80737e-05
       //    6  p5     2.28805e+02   5.78644e-03   1.09103e-04  -2.33797e-05
       //    7  p6    -6.81692e+01   1.12835e-03   3.25056e-05  -2.61630e-03
       //    8  p7     1.19125e+01   2.10236e-04   5.68031e-06  -2.54293e-02
       //    9  p8    -1.12813e+00   3.64331e-05   5.37935e-07  -1.89697e-01
       //   10  p9     4.46826e-02   5.50300e-06   2.13063e-08  -1.27341e+00
    } else if(lModule[time] == 3) {
      distVertexMDC = distVertexMDC4;
      driftVelocityMDC = driftVelocityMDC4;
      driftTime = ( distance / driftVelocityMDC4 )*1000.;
    } else continue;
    //printf("old driftTime=%g TDCTime=%g\n",driftTime,TDCTime[time]);
  } else {
    //new--------------------
    isInCell=fSizeCellsLayer[time]->getDriftDist(
        xVertex, yVertex, zVertex, xPlane, yPlane, zPlane,
        lCell[time], alphaDrDist, distance);
    distWire[time]=distance;
    driftTime=fCal2ParSim->calcTime(sector,lModule[time],alphaDrDist,distance);
    //printf("new dist=%g \n",distance);
    //Printf("d=%f->%f a=%f %i",fabs(distance),driftDist,alphaDrDist,isIn);
    //printf("new driftTime=%g ",driftTime);
    //-----------------------
  }


    arriveTime = particleVelocity;
//     arriveTime =  fabs(distVertexMDC) / particleVelocity;
//      arriveTime = 0;

    fullTime = arriveTime + driftTime;
    if (iflag == 5) {
      driftTime = ( distance / driftVelocityMDC4 )*1000.;
      fullTime = arriveTime + driftTime;
//       if(segment == 0 && fullTime > 200) fullTime = 200;
//       if(segment == 1 && fullTime > 1000) fullTime = 1000;
    }

    dist[time] = fullTime - TDCTime[time];

//      if (iflag == 1) weight = qualityWeight[time];
    if (iflag == 1) {
      weight[time] = 1;
      qualityWeight[time] = 1;
      if(version==0) {
        if(segment == 0 && fullTime > 200) qualityWeight[time] = 0;
        if(segment == 1 && fullTime > 1000) qualityWeight[time] = 0;
      } else {
        if(!isInCell) qualityWeight[time] = 0;
      }
    } else if (iflag == 4) {
      weight[time] = 1;
      qualityWeight[time] = 1;
    } else {
      weight[time]=(fabs(dist[time]) < width) ?
          pow( (1 - pow((dist[time]/width),2)), 2) : 0;
    }

    weight[time] *= qualityWeight[time];

    sumWeight += weight[time];
    sumDeviation += weight[time]*dist[time]*dist[time] ;

    if(fprint) {

      if((iflag == 1 || iflag == 3)) {
	printf(" iflag=%i time=%-3i arriveTime=%-7.4g", iflag,time,arriveTime);
	printf(" driftTime=%-7.4g fullTime=%-8.5g",driftTime,fullTime );
	printf(" TDCTime=%-8.5g dist=%-8.5g weight=%g\n",
            TDCTime[time],dist[time],weight[time]);
      }
    }

  }

  width = cnst*sqrt(sumDeviation/sumWeight);
  if(fprint && iflag == 3) printf("========================== width=%f\n",width);

  return sumDeviation;
}

Bool_t HMdcTrackFitter::finalize() {
  return kTRUE;
}

void HMdcTrackFitter::printResult(Char_t* status) const{
  printf("%s\n",status);
  if(timer) {
    timer->Stop();
    printf("Time at the end of job = %g seconds\n",timer->CpuTime());
  }
  printParam("final");
}

void HMdcTrackFitter::printParam(Char_t* title) const {
  printf(
      "%s %3i Par.=%-6g %-6g %-6g %-6g %-6g  fun.=%-6g->%-6g\n",
      title,iteration,parameter[0],parameter[1],parameter[2],parameter[3],
      parameter[4],functional,newFunctional);
}

void HMdcTrackFitter::setPrintFlag(Bool_t prnt) {
  fprint=prnt;
  if(fprint && !timer) timer=new TStopwatch();
}
