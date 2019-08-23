#include "hmdctrackfitter.h"
#include "hmdcdef.h"
#include "hdebug.h"
#include "hades.h"
#include <iostream.h>
#include "TStopwatch.h"
#include "TMinuit.h"
#include "TMath.h"
#include "hmdctrackfitpar.h"
#include "hruntimedb.h"

//*-- Author : G.Agakichiev
//*-- Modified: 7.5.99

HMdcTrackFitter::HMdcTrackFitter(void) {  
  init();
  maxNMdcTimes = fitPar->getMaxNMdcTimes(); 
  xLeft = 0;
  xRight = 0;
  yLeft = 0;
  yRight = 0;
  zLeft = 0;
  zRight = 0;
  TDCTime = 0;
  dist = new Float_t [maxNMdcTimes];
  distWire = new Float_t [maxNMdcTimes];
  weight = new Float_t [maxNMdcTimes];
  parameter = new Double_t [6];
//  fprint = kFALSE;
    fprint = kTRUE;
  
}

HMdcTrackFitter::~HMdcTrackFitter(void) {
  delete [] parameter;
  delete [] weight;
  delete [] distWire;
  delete [] dist;
}

void HMdcTrackFitter::setParContainers() {
  fitPar=(HMdcTrackFitPar*)gHades->getRuntimeDb()->getContainer("MdcTrackFitPar");
  if (!fitPar) {
    fitPar=new HMdcTrackFitPar;
//    fitPar->init();
    gHades->getRuntimeDb()->addContainer(fitPar);
  }  
}

Bool_t HMdcTrackFitter::init(void) {
  setParContainers();
  myEvtNum=0;
  driftVelocityMDC1 = fitPar->getDriftVelocityMDC1();
  driftVelocityMDC2 = fitPar->getDriftVelocityMDC2();
  driftVelocityMDC3 = fitPar->getDriftVelocityMDC3();
  driftVelocityMDC4 = fitPar->getDriftVelocityMDC4();
  maxIteration = fitPar->getMaxIteration();
  step = fitPar->getStep();
  weightPar = fitPar->getWeight();
  stepNorm = fitPar->getStepNorm();  
  particleVelocity = fitPar->getParticleVelocity();
  cnst = fitPar->getTukeyConstant(); //cnst dec!!!!
  return kTRUE;
}

Int_t HMdcTrackFitter::execute(Float_t threshold) {
   myEvtNum++;
    numberOfParameter = 5;
  stepNorm = fitPar->getStepNorm();    
  Double_t functional, maxDelta, delta[numberOfParameter];
  Double_t newFunctional=0;
  TStopwatch timer;
  
  timer.Start();
  parameter[0] = xVertex;
  parameter[1] = yVertex;
  parameter[2] = xInitPlane;
  parameter[3] = yInitPlane;
  parameter[4] = 3;
  
  functional = calculationOfFunctional(1);
  
  for (iteration = 0; iteration < maxIteration; iteration++ ) {
    
    maxDelta = 0;
    for(i = 0; i < numberOfParameter; i++) {
      parameter[i] += stepNorm*step[i];
      delta[i] = (calculationOfFunctional(2) - functional)/(stepNorm*step[i]);
      parameter[i] -= stepNorm*step[i];
    }
    
    for(i = 0; i < numberOfParameter; i++)
      if (maxDelta < fabs(delta[i])) maxDelta = fabs(delta[i]);
    
    for(i = 0; i < numberOfParameter; i++){

      parameter[i] -= stepNorm*step[i]*delta[i]*weightPar[i]/maxDelta;
    }    
    newFunctional = calculationOfFunctional(2);
    
    if(fprint) {
       cout << "out1 "<< iteration << " "
 	   << parameter[0] << " " << parameter[1] << " " << parameter[2] << " "
 	   << parameter[3] << " " << parameter[4] << " "
 	   << functional <<" "<< newFunctional << endl;
    }
    if(newFunctional < functional) functional = newFunctional;
    else {
      for(i = 0; i < numberOfParameter; i++)
	parameter[i] += 0.5*stepNorm*step[i]*delta[i]*weightPar[i]/maxDelta;
      
      newFunctional = calculationOfFunctional(2);
      if( newFunctional < functional ) functional = newFunctional;
      else {
	for(i = 0; i < numberOfParameter; i++)
	  parameter[i] += 0.5*stepNorm*step[i]*delta[i]*weightPar[i]/maxDelta;
	
	stepNorm /= 2;
      }
    }
      if(stepNorm < 0.05) {
      if(fprint) {
	cout<<" conversion ok "<<endl;
	cout<<"Time at the end of job = "<<timer.CpuTime()<<" seconds"<<endl;
	cout << "final "<< iteration << " "
	     <<parameter[0] << " " << parameter[1] << " " << parameter[2] << " "
	     << parameter[3] << " " << parameter[4] << " "
	     << functional<<" "<<newFunctional<<endl;
      }
      
      funMin = calculationOfFunctional(3);
      
      xOutVertex = parameter[0];
      yOutVertex = parameter[1];
      zOutVertex = zVertex;
      xOutPlane = parameter[2];
      yOutPlane = parameter[3];
      zOutPlane = zInitPlane;
      pVelocity = parameter[4];
      
	  
      return ((funMin < threshold) ? 1 : 0);
    }
  }
  funMin = calculationOfFunctional(3);
  xOutVertex = parameter[0];
  yOutVertex = parameter[1];
  zOutVertex = zVertex;
  xOutPlane = parameter[2];
  yOutPlane = parameter[3];
  zOutPlane = zInitPlane;
  pVelocity = parameter[4];
  if(fprint) {
    cout<<" no conversion "<<endl;
    cout<<"Time at the end of job = "<<timer.CpuTime()<<" seconds"<<endl;
    cout << "final "<< iteration << " "
	 <<parameter[0] << " " << parameter[1] << " " << parameter[2] << " "
	 << parameter[3] << " " << parameter[4] << " "
	 << functional<<" "<<newFunctional<<endl;
  }
  return 0;
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
      
   return deviation(xV, yV, zV, \
			  xPlane, yPlane, zPlane, pV, iflag);
}
Double_t HMdcTrackFitter::deviation(Double_t xVertex, Double_t yVertex, Double_t zVertex,\
				    Double_t xPlane, Double_t yPlane, Double_t zPlane, \
				    Double_t particleVelocity, Int_t iflag) { 

   Double_t sumDeviation, sumDeviation0;
   Double_t distance;
   Double_t distVertexMDC;
   Double_t driftVelocityMDC;
   Double_t driftTime, fullTime;
   Double_t ap, bp, cp;
   Double_t dp, ep, fp;
   Double_t sumWeight;
   static Double_t width;

   if (iflag == 1) {
   }
   
   sumDeviation = 0;
   sumDeviation0 = 0;
   sumWeight = 0;
   for(int time = 0; time < NMDC1Times + NMDC2Times; time++) {

     ap = xRight[time] - xLeft[time];
     bp = yRight[time] - yLeft[time];
     cp = zRight[time] - zLeft[time];

     dp = xPlane - xVertex;
     ep = yPlane - yVertex;
     fp = zPlane - zVertex;

     distance = (ep*cp*xLeft[time] - ep*cp*xVertex - fp*bp*xLeft[time] + fp*bp*xVertex -
		 dp*cp*yLeft[time] + dp*cp*yVertex + dp*bp*zLeft[time] - dp*bp*zVertex +
		 ap*fp*yLeft[time] - ap*fp*yVertex - ap*ep*zLeft[time] + ap*ep*zVertex)/
	        sqrt(ep*ep*cp*cp - 2*ep*cp*fp*bp + fp*fp*bp*bp+fp*fp*ap*ap -
		     2*fp*ap*dp*cp+dp*dp*cp*cp + dp*dp*bp*bp - 2*dp*bp*ep*ap+ep*ep*ap*ap);
     distWire[time]=abs(distance);

     if(nSegment == 0) {
       if( time < NMDC1Times) {
	 distVertexMDC = distVertexMDC1;
	 driftTime = ( abs(distance) / driftVelocityMDC1 )*1000.;
       }

       else {
	 distVertexMDC = distVertexMDC2;
	 //        driftVelocityMDC = driftVelocityMDC2;
	 driftTime = 5.85 + 4.30*abs(distance) + 18.88*distance*distance
	   - 8.54*distance*distance*abs(distance)
	   + 1.48*distance*distance*distance*distance;
// 	 driftTime = ( abs(distance) / driftVelocityMDC2 )*1000.;
// 	 driftTime = driftTime*.9;    // for Dubna chambers B != 0
       }
     }
     else {
       if( time < NMDC1Times) {
// 	 distVertexMDC = distVertexMDC3;
// 	 driftVelocityMDC = driftVelocityMDC3;
// 	 driftTime = ( abs(distance) / driftVelocityMDC3 )*1000.;
	 driftTime =
	   - 0.03542
	   + 78.7210 * abs(distance)
	   - 287.847 * abs(pow(distance,2))
	   + 512.043 * abs(pow(distance,3))
	   - 454.673 * abs(pow(distance,4))
	   + 228.805 * abs(pow(distance,5))
	   - 68.1692 * abs(pow(distance,6))
	   + 11.9125 * abs(pow(distance,7))
	   - 1.12813 * abs(pow(distance,8))
	   + 0.04468 * abs(pow(distance,9));
	 
//    1  p0          -3.54201e-02   1.48897e+00   6.61675e-04   1.67073e-06
//    2  p1           7.87210e+01   1.25067e+00   2.56053e-04   2.88928e-06
//    3  p2          -2.87847e+02   5.06960e-01   1.37256e-04   1.12583e-05
//    4  p3           5.12043e+02   1.25888e-01   2.44161e-04   4.00551e-05
//    5  p4          -4.54673e+02   2.81771e-02   2.16805e-04   9.80737e-05
//    6  p5           2.28805e+02   5.78644e-03   1.09103e-04  -2.33797e-05
//    7  p6          -6.81692e+01   1.12835e-03   3.25056e-05  -2.61630e-03
//    8  p7           1.19125e+01   2.10236e-04   5.68031e-06  -2.54293e-02
//    9  p8          -1.12813e+00   3.64331e-05   5.37935e-07  -1.89697e-01
//   10  p9           4.46826e-02   5.50300e-06   2.13063e-08  -1.27341e+00
 	 
       }

       else {
	 distVertexMDC = distVertexMDC4;
	 driftVelocityMDC = driftVelocityMDC4;
	 driftTime = ( abs(distance) / driftVelocityMDC4 )*1000.;
       }
     }
     
    arriveTime = particleVelocity;
//     arriveTime =  abs(distVertexMDC) / particleVelocity;
//      arriveTime = 0;
     
     fullTime = arriveTime + driftTime;

     dist[time] = fullTime - TDCTime[time];

//      if (iflag == 1) weight = qualityWeight[time];
     if (iflag == 1) {
       weight[time] = 1;
       qualityWeight[time] = 1;
       if(nSegment == 0 && fullTime > 200) qualityWeight[time] = 0;
       if(nSegment == 1 && fullTime > 1000) qualityWeight[time] = 0;
//        if(nSegment == 0 && fullTime > 200) weight[time] = 0;
//        if(nSegment == 1 && fullTime > 300) weight[time] = 0;
     }
     else {
       if (abs(dist[time]) < width) weight[time] = (1 - ((dist[time]/width)*(dist[time]/width))) * (1 - ((dist[time]/width)*(dist[time]/width)));
       else weight[time] = 0.;   

     }
     
     weight[time] *= qualityWeight[time];

     sumWeight += weight[time];
     sumDeviation += weight[time]*dist[time]*dist[time] ;
     
     if(fprint) {
       
       if((iflag == 1 || iflag == 3)) {
	 printf(" iflag=%i", iflag) ;
	 printf(" time=%i", time) ;
	 printf(" arriveTime=%f", arriveTime) ;
	 printf(" driftTime=%f",driftTime ) ;
	 printf(" fullTime=%f",fullTime ) ;
	 printf(" TDCTime=%f", TDCTime[time]) ;
	 printf(" dist=%f",dist[time] ) ;
	 printf(" weight=%f\n",weight[time] ) ;
       }
     }

   }

   width = cnst*sqrt(sumDeviation/sumWeight);
   if(fprint && iflag == 3) printf("========================== width=%f\n",width); 
       
   return sumDeviation;
 }

//ClassImp(HMdcTrackFitter)

Bool_t HMdcTrackFitter::finalize() {
  return kTRUE;
}







