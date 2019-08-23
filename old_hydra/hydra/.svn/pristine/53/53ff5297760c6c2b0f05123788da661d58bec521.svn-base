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
  xLeft = new Float_t [maxNMdcTimes];
  xRight = new Float_t [maxNMdcTimes];
  yLeft = new Float_t [maxNMdcTimes];
  yRight = new Float_t [maxNMdcTimes];
  zLeft = new Float_t [maxNMdcTimes];
  zRight = new Float_t [maxNMdcTimes];
  TDCTime = new Float_t [maxNMdcTimes];
  dist = new Float_t [maxNMdcTimes];
  weight = new Float_t [maxNMdcTimes];
  parameter = new Double_t [6];
  fprint = kFALSE;
//   fprint = kTRUE;
  
}

HMdcTrackFitter::~HMdcTrackFitter(void) {
}

void HMdcTrackFitter::setParContainers() {
  fitPar=(HMdcTrackFitPar*)gHades->getRuntimeDb()->getContainer("MdcTrackFitPar");
  if (!fitPar) {
    fitPar=new HMdcTrackFitPar;
    gHades->getRuntimeDb()->addContainer(fitPar);
  }  
}

Bool_t HMdcTrackFitter::init(void) {
  setParContainers();
  myEvtNum=0;
  driftVelocityMDC1 = driftVelocityMDC2 = fitPar->getDriftVelocity();
  maxIteration = fitPar->getMaxIteration();
  step = fitPar->getStep();
  weightPar = fitPar->getWeight();
  stepNorm = fitPar->getStepNorm();  
  xVertex = fitPar->getXVertex();
  yVertex = fitPar->getYVertex();
  zVertex = fitPar->getZVertex();
  particleVelocity = fitPar->getParticleVelocity();
  cnst = fitPar->getTukeyConstant(); //cnst dec!!!!
  return kTRUE;
}

Int_t HMdcTrackFitter::execute(Float_t threshold) {
   myEvtNum++;
    numberOfParameter = 6;
  stepNorm = fitPar->getStepNorm();    
  Double_t functional, newFunctional, maxDelta, delta[numberOfParameter];
  TStopwatch timer;
  
  timer.Start();
  parameter[0] = xVertex;
  parameter[1] = yVertex;
  parameter[2] = zVertex;
  parameter[3] = xInitPlane;
  parameter[4] = yInitPlane;
  parameter[5] = particleVelocity;
  
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
    
    if(fprint && iteration%10 == 0) {
       cout << "out1 "<< iteration << " "
 	   << parameter[0] << " " << parameter[1] << " " << parameter[2] << " "
 	   << parameter[3] << " " << parameter[4] << " " << parameter[5] << " "
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
    if(stepNorm < 0.5) {
      // 	if(stepNorm < 0.05) {
      if(fprint) {
	cout<<" conversion ok "<<endl;
	cout<<"Time at the end of job = "<<timer.CpuTime()<<" seconds"<<endl;
	cout << "final "<< iteration << " "
	     <<parameter[0] << " " << parameter[1] << " " << parameter[2] << " "
	     << parameter[3] << " " << parameter[4] << " " << parameter[5] << " "
	     << functional<<" "<<newFunctional<<endl;
      }
      
      funMin = calculationOfFunctional(3);
      
      xOutVertex = parameter[0];
      yOutVertex = parameter[1];
      zOutVertex = parameter[2];
      xOutPlane = parameter[3];
      yOutPlane = parameter[4];
      zOutPlane = zInitPlane;
      pVelocity = parameter[5];
      
	  
      return ((funMin < threshold) ? 1 : 0);
    }
  }
  if(fprint) {
    cout<<" no conversion "<<endl;
    cout<<"Time at the end of job = "<<timer.CpuTime()<<" seconds"<<endl;
  }
  return 0;
}

Double_t HMdcTrackFitter::calculationOfFunctional(Int_t iflag) {

   Double_t xVertex, yVertex, zVertex; 
   Double_t xPlane, yPlane, zPlane; 
   Double_t particleVelocity; 
    
//  fixed zPlane
   xVertex = parameter[0];
   yVertex = parameter[1];
   zVertex = parameter[2];

   xPlane = parameter[3];
   yPlane = parameter[4];
   zPlane = zInitPlane;

   particleVelocity = parameter[5];
      
   return deviation(xVertex, yVertex, zVertex, \
			  xPlane, yPlane, zPlane, particleVelocity, iflag);
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

     if( time < NMDC1Times) {
       distVertexMDC = distVertexMDC1;
       driftVelocityMDC = driftVelocityMDC1;
       driftTime = ( abs(distance) / driftVelocityMDC )*1000.;
     }

     else {
       distVertexMDC = distVertexMDC2;
//        driftVelocityMDC = driftVelocityMDC2;
       driftTime = 5.85 + 4.30*abs(distance) + 18.88*distance*distance
	 - 8.54*distance*distance*abs(distance)
	 + 1.48*distance*distance*distance*distance;
         driftTime = driftTime*.9;    // for Dubna chambers
     }

    arriveTime =  abs(distVertexMDC) / particleVelocity;
//      arriveTime = 0;
     
     fullTime = arriveTime + driftTime;

     dist[time] = fullTime - TDCTime[time];

//      if (iflag == 1) weight = qualityWeight[time];
     if (iflag == 1) {
       weight[time] = 1;
       if(fullTime > 200) weight[time] = 0;
     }
     else {
       if (abs(dist[time]) < width) weight[time] = (1 - ((dist[time]/width)*(dist[time]/width))) * (1 - ((dist[time]/width)*(dist[time]/width)));
       else weight[time] = 0.;   

     }
     
     weight[time] *= qualityWeight[time];
     sumWeight += weight[time];
     sumDeviation += weight[time]*dist[time]*dist[time] ;
     
     if(fprint) {
       
       if((iflag == 1 || iflag == 3) && sumDeviation > 500000000) {
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







