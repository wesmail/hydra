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
  //  hdistvertexmdc->Draw();
  //  hcalfit_2dim->Draw();
  file = new TFile("cal_fit.root","RECREATE");
  hCalFit2_44->Write();
  hCalFit2_45->Write();
  hCalFit2_46->Write();
  hCalFit2_47->Write();
  hCalFit2_48->Write();
  hCalFit2_49->Write();
  hCalFit2_50->Write();
  hCalFit2_51->Write();
  hCalFit2_52->Write();
  hCalFit2_53->Write();
  hCalFit5_44->Write();
  hCalFit5_45->Write();
  hCalFit5_46->Write();
  hCalFit5_47->Write();
  hCalFit5_48->Write();
  hCalFit5_49->Write();
  hCalFit5_50->Write();
  hCalFit5_51->Write();
  hCalFit5_52->Write();
  hCalFit5_53->Write();
  file->Close();
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
  hdistvertexmdc = new TH1F("hdistvertexmdc","hdistvertexmdc",6000,500.,1500.);
  hCalFit2_44 = new TH1F("hCalFit2_44","hCalFit2_44",2000,-100.,100.);
  hCalFit2_45 = new TH1F("hCalFit2_45","hCalFit2_45",2000,-100.,100.);
  hCalFit2_46 = new TH1F("hCalFit2_46","hCalFit2_46",2000,-100.,100.);
  hCalFit2_47 = new TH1F("hCalFit2_47","hCalFit2_47",2000,-100.,100.);
  hCalFit2_48 = new TH1F("hCalFit2_48","hCalFit2_48",2000,-100.,100.);
  hCalFit2_49 = new TH1F("hCalFit2_49","hCalFit2_49",2000,-100.,100.);
  hCalFit2_50 = new TH1F("hCalFit2_50","hCalFit2_50",2000,-100.,100.);
  hCalFit2_51 = new TH1F("hCalFit2_51","hCalFit2_51",2000,-100.,100.);
  hCalFit2_52 = new TH1F("hCalFit2_52","hCalFit2_52",2000,-100.,100.);
  hCalFit2_53 = new TH1F("hCalFit2_53","hCalFit2_53",2000,-100.,100.);
  hCalFit5_44 = new TH1F("hCalFit5_44","hCalFit5_44",2000,-100.,100.);
  hCalFit5_45 = new TH1F("hCalFit5_45","hCalFit5_45",2000,-100.,100.);
  hCalFit5_46 = new TH1F("hCalFit5_46","hCalFit5_46",2000,-100.,100.);
  hCalFit5_47 = new TH1F("hCalFit5_47","hCalFit5_47",2000,-100.,100.);
  hCalFit5_48 = new TH1F("hCalFit5_48","hCalFit5_48",2000,-100.,100.);
  hCalFit5_49 = new TH1F("hCalFit5_49","hCalFit5_49",2000,-100.,100.);
  hCalFit5_50 = new TH1F("hCalFit5_50","hCalFit5_50",2000,-100.,100.);
  hCalFit5_51 = new TH1F("hCalFit5_51","hCalFit5_51",2000,-100.,100.);
  hCalFit5_52 = new TH1F("hCalFit5_52","hCalFit5_52",2000,-100.,100.);
  hCalFit5_53 = new TH1F("hCalFit5_53","hCalFit5_53",2000,-100.,100.);
  hcalfit_2dim = new TH2F("hcalfit_2dim","hcalfit_2dim",2000,0.,200.,2000,0.,200.);
  myEvtNum=0;
  //  c1 = new TCanvas("c1","c1",0,0,600,800);
  //  c1->Divide(2,5);
  //  c2 = new TCanvas("c2","c2",0,0,600,800);
  //  c2->Divide(2,5);
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

Bool_t HMdcTrackFitter::distVertex(void) {                    
    
  /*  Double_t a, b, c;
  Double_t xVertex = 0;
  Double_t yVertex = 0;
  Double_t zVertex = 0;
  arriveTime = 0.;
  
  a = xRight[0] - xLeft[0];
  b = yRight[0] - yLeft[0];
  c = zRight[0] - zLeft[0];
  


  distVertexMDC1 = sqrt(( -2*a*xVertex*b*yVertex + 2*a*xVertex*c*zLeft[0] -
			  2*a*xVertex*c*zVertex - 2*a*xLeft[0]*c*zLeft[0] -
			  2*a*xLeft[0]*b*yLeft[0] + 2*a*xLeft[0]*b*yVertex -
			  2*xLeft[0]*xVertex*c*c - 2*xLeft[0]*xVertex*b*b -
			  2*zLeft[0]*zVertex*b*b - 2*zLeft[0]*zVertex*a*a -
			  2*yLeft[0]*yVertex*c*c - 2*yLeft[0]*yVertex*a*a -
			  2*b*yVertex*c*zVertex - 2*b*yLeft[0]*c*zLeft[0] +
			  2*b*yLeft[0]*c*zVertex + 2*b*yVertex*c*zLeft[0] +
			  2*a*xLeft[0]*c*zVertex + 2*a*xVertex*b*yLeft[0] +
			  yLeft[0]*yLeft[0]*a*a + zVertex*zVertex*b*b +
			  zVertex*zVertex*a*a + yLeft[0]*yLeft[0]*c*c +
			  zLeft[0]*zLeft[0]*a*a + zLeft[0]*zLeft[0]*b*b +
			  yVertex*yVertex*c*c + xLeft[0]*xLeft[0]*b*b +
			  xLeft[0]*xLeft[0]*c*c + yVertex*yVertex*a*a +
			  xVertex*xVertex*b*b + xVertex*xVertex*c*c) /
			(a*a + b*b + c*c));

   a = xRight[NMDC1Times] - xLeft[NMDC1Times];
   b = yRight[NMDC1Times] - yLeft[NMDC1Times];
   c = zRight[NMDC1Times] - zLeft[NMDC1Times];

   distVertexMDC2 = sqrt(( -2*a*xVertex*b*yVertex + 2*a*xVertex*c*zLeft[NMDC1Times] -
			  2*a*xVertex*c*zVertex - 2*a*xLeft[NMDC1Times]*c*zLeft[NMDC1Times] -
			  2*a*xLeft[NMDC1Times]*b*yLeft[NMDC1Times] +
			  2*a*xLeft[NMDC1Times]*b*yVertex -
			  2*xLeft[NMDC1Times]*xVertex*c*c - 2*xLeft[NMDC1Times]*xVertex*b*b -
			  2*zLeft[NMDC1Times]*zVertex*b*b - 2*zLeft[NMDC1Times]*zVertex*a*a -
			  2*yLeft[NMDC1Times]*yVertex*c*c - 2*yLeft[NMDC1Times]*yVertex*a*a -
			  2*b*yVertex*c*zVertex - 2*b*yLeft[NMDC1Times]*c*zLeft[NMDC1Times] +
			  2*b*yLeft[NMDC1Times]*c*zVertex + 2*b*yVertex*c*zLeft[NMDC1Times] +
			  2*a*xLeft[NMDC1Times]*c*zVertex + 2*a*xVertex*b*yLeft[NMDC1Times] +
			  yLeft[NMDC1Times]*yLeft[NMDC1Times]*a*a + zVertex*zVertex*b*b +
			  zVertex*zVertex*a*a + yLeft[NMDC1Times]*yLeft[NMDC1Times]*c*c +
			  zLeft[NMDC1Times]*zLeft[NMDC1Times]*a*a +
			  zLeft[NMDC1Times]*zLeft[NMDC1Times]*b*b +
			  yVertex*yVertex*c*c + xLeft[NMDC1Times]*xLeft[NMDC1Times]*b*b +
			  xLeft[NMDC1Times]*xLeft[NMDC1Times]*c*c + yVertex*yVertex*a*a +
			  xVertex*xVertex*b*b + xVertex*xVertex*c*c) /
			  (a*a + b*b + c*c));
   
 */   
   hdistvertexmdc->Fill(distVertexMDC2);

   return kTRUE;
}


Int_t HMdcTrackFitter::execute(Float_t threshold) {
   myEvtNum++;
//   if(!(myEvtNum%200000)) {
//     c1->cd(1);
//     hCalFit2_44->Draw();
//     c1->cd(2);
//     hCalFit2_45->Draw();
//     c1->cd(3);
//     hCalFit2_46->Draw();
//     c1->cd(4);
//     hCalFit2_47->Draw();
//     c1->cd(5);
//     hCalFit2_48->Draw();
//     c1->cd(6);
//     hCalFit2_49->Draw();
//     c1->cd(7);
//     hCalFit2_50->Draw();
//     c1->cd(8);
//     hCalFit2_51->Draw();
//     c1->cd(9);
//     hCalFit2_52->Draw();
//     c1->cd(10);
//     hCalFit2_53->Draw();
//     c1->Modified();
//     c1->Update();
//     c2->cd(1);
//     hCalFit5_44->Draw();
//     c2->cd(2);
//     hCalFit5_45->Draw();
//     c2->cd(3);
//     hCalFit5_46->Draw();
//     c2->cd(4);
//     hCalFit5_47->Draw();
//     c2->cd(5);
//     hCalFit5_48->Draw();
//     c2->cd(6);
//     hCalFit5_49->Draw();
//     c2->cd(7);
//     hCalFit5_50->Draw();
//     c2->cd(8);
//     hCalFit5_51->Draw();
//     c2->cd(9);
//     hCalFit5_52->Draw();
//     c2->cd(10);
//     hCalFit5_53->Draw();
//     c2->Modified();
//     c2->Update();
//  }
  //  numberOfParameter = 5;
    numberOfParameter = 6;
  stepNorm = fitPar->getStepNorm();    
  Double_t functional, newFunctional, maxDelta, delta[numberOfParameter];
  TStopwatch timer;
  //   cout<<"Starting timer"<<endl;
  timer.Start();
  parameter[0] = xVertex;
  parameter[1] = yVertex;
  parameter[2] = zVertex;
  parameter[3] = xInitPlane;
  parameter[4] = yInitPlane;
  //  cout << "  xInitPlane = " <<  xInitPlane << endl;
  //  cout << "  yInitPlane = " <<  yInitPlane << endl;
  //      parameter[5] = 280;
  parameter[5] = particleVelocity;
  
  //    parameter[0] = 0;
  //    parameter[1] = 0;
  //    parameter[2] = xInitPlane;
  //    parameter[3] = yInitPlane;
  //    parameter[4] = 28;
  
  //    parameter[0] = 0;   
  //    parameter[1] = xInitPlane;
  //    parameter[2] = yInitPlane;
  //    parameter[3] = 28;
    
  functional = calculationOfFunctional(1);
  //   cout << "first " << functional << endl;
  
  for (iteration = 0; iteration < maxIteration; iteration++ ) {
    
    maxDelta = 0;
    for(i = 0; i < numberOfParameter; i++) {
      parameter[i] += stepNorm*step[i];
      delta[i] = (calculationOfFunctional(2) - functional)/(stepNorm*step[i]);
      parameter[i] -= stepNorm*step[i];
      //      cout << "delta " <<  delta[i]  <<endl; 
    }
    
    for(i = 0; i < numberOfParameter; i++)
      if (maxDelta < fabs(delta[i])) maxDelta = fabs(delta[i]);
    
    for(i = 0; i < numberOfParameter; i++){

      parameter[i] -= stepNorm*step[i]*delta[i]*weightPar[i]/maxDelta;
      //      cout << i << " par " << stepNorm*step[i]*delta[i]*weightPar[i]/maxDelta << endl;
    }    
    newFunctional = calculationOfFunctional(2);
    
    if(fprint) {
//       cout << "out1 "<< iteration << " "
// 	   << parameter[0] << " " << parameter[1] << " " << parameter[2] << " "
// 	   << parameter[3] << " " << parameter[4] << " " << zOutPlane << " " <<//  parameter[5] << " "
// 	   << " " << functional <<" "<< newFunctional << endl;
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
      
      // 	  hist1[197]->Fill(timer.CpuTime(),1);
      funMin = calculationOfFunctional(3);
      
      xOutVertex = parameter[0];
      yOutVertex = parameter[1];
      zOutVertex = parameter[2];
      xOutPlane = parameter[3];
      yOutPlane = parameter[4];
      zOutPlane = zInitPlane;
      pVelocity = parameter[5];
      
//    	  xOutVertex = parameter[0];
//    	  yOutVertex = parameter[1];
//    	  zOutVertex = 0.;
//    	  xOutPlane = parameter[2];
//    	  yOutPlane = parameter[3];
//    	  zOutPlane = zInitPlane;
	  
//  	  xOutVertex = 0;
//  	  yOutVertex = 0;
//  	  zOutVertex = parameter[0];
//  	  xOutPlane = parameter[1];
//  	  yOutPlane = parameter[2];
//  	  zOutPlane = zInitPlane;
	  
      return ((funMin < threshold) ? 1 : 0);
    }
  }
  if(fprint) {
    cout<<" no conversion "<<endl;
    cout<<"Time at the end of job = "<<timer.CpuTime()<<" seconds"<<endl;
  }
  return 0;
}

// static void HMdcTrackFitter::fcn(Int_t &npar, Double_t *gin, Double_t &f, Double_t *x, Int_t iflag) {
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
   
//  fixed zVertex and zPlane
//    xVertex = parameter[0];
//    yVertex = parameter[1];
//    zVertex = 0.;

//    xPlane = parameter[2];
//    yPlane = parameter[3];
//    zPlane = zInitPlane;

//    particleVelocity = parameter[4];
   
//  fixed xVertex yVertex and zPlane
//    xVertex = 0;
//    yVertex = 0;
//    zVertex = parameter[0];

//    xPlane = parameter[1];
//    yPlane = parameter[2];
//    zPlane = zInitPlane;

//    particleVelocity = parameter[3];
   
//     if(fprint) {
//       printf(" xVertex=%f", xVertex) ;
//       printf(" yVertex=%f", yVertex) ;
//       printf(" zVertex=%f", zVertex) ;
//       printf(" xPlane=%f", xPlane) ;
//       printf(" yPlane=%f", yPlane) ;
//       printf(" zPlane=%f", zPlane) ;
//       printf(" particleVelocity=%f\n", particleVelocity) ;
//     }
   
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
   //   Double_t driftTime, arriveTime, fullTime;
   Double_t driftTime, fullTime;
   Double_t ap, bp, cp;
   Double_t dp, ep, fp;
   Double_t sumWeight;
   static Double_t width;

//    printf(" NMDC1Times=%i", NMDC1Times) ;
//    printf(" NMDC2Times=%i", NMDC2Times) ;
//    printf(" xRight=%f", xRight[0]) ;
//    printf(" yRight=%f", yRight[0]) ;
//    printf(" zRight=%f/n", zRight[0]) ;
//     printf(" xVertex=%f", xVertex) ;
//     printf(" yVertex=%f", yVertex) ;
//     printf(" zVertex=%f\n", zVertex) ;
//     printf(" xPlane=%f", xPlane) ;
//     printf(" yPlane=%f", yPlane) ;
//     printf(" zPlane=%f\n", zPlane) ;
//    printf(" particleVelocity=%f\n", particleVelocity) ;

   if (iflag == 1) {
     //     cnst = 5.5;
//      width = 90.;
   }
   //    width =90.;
   
   sumDeviation = 0;
   sumDeviation0 = 0;
   sumWeight = 0;
   for(int time = 0; time < NMDC1Times + NMDC2Times; time++) {

//      cout << "time  " << time << endl;
//      cout << "xRight  " << xRight[time] << endl;
//      cout << "xLeft   " << xLeft[time] << endl;
//      cout << "yRight  " << yRight[time] << endl;
//      cout << "yLeft   " << yLeft[time] << endl;
//      cout << "zRight  " << zRight[time] << endl;
//      cout << "zLeft   " << zLeft[time] << endl;
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

     distVertexMDC = distVertexMDC1;
     driftVelocityMDC = driftVelocityMDC1;

     if( time >= NMDC1Times) {
       distVertexMDC = distVertexMDC2;
       driftVelocityMDC = driftVelocityMDC2;
     }


    // Drift Velocity described by pol5
 
     /*     driftVelocityMDC  = 13.21-30.32*abs(distance)+41.49*abs(distance)*abs(distance)-27.46*abs(distance)*abs(distance)*abs(distance)+8.672*abs(distance)*abs(distance)*abs(distance)*abs(distance)-1.053*abs(distance)*abs(distance)*abs(distance)*abs(distance)*abs(distance);
    driftVelocityMDC*=10.;

//    driftVelocityMDC+=5.;
     

 

    if(abs(distance)>3.1) driftVelocityMDC = 2.;
     */
     


     //        cout << "driftVelocity = " << driftVelocityMDC << endl;
    //     cout << "abs(distance) = " << abs(distance) << endl;

    //    cout << "driftVelocityMDC = " << driftVelocityMDC << endl;
 
    driftTime = ( abs(distance) / driftVelocityMDC )*1000.;

     // Drift Velocity described by pol5

     
     arriveTime =  abs(distVertexMDC) / particleVelocity;
//      arriveTime = 0;
   
     fullTime = arriveTime + driftTime;

     dist[time] = fullTime - TDCTime[time];

//      dist /= 23.8;

//      if (iflag == 1) weight = qualityWeight[time];
     if (iflag == 1) weight[time] = 1;
     else {

//        weight = (1 - ((dist/width)*(dist/width))) * (1 - ((dist/width)*(dist/width)));
       
//        if (abs(dist) < width) weight = 1.;
       if (abs(dist[time]) < width) weight[time] = (1 - ((dist[time]/width)*(dist[time]/width))) * (1 - ((dist[time]/width)*(dist[time]/width)));
       else weight[time] = 0.;   

     }
//      if(improvment > 5 && abs(dist[time]) < width/10.) qualityWeight[time] = 1.;
     weight[time] *= qualityWeight[time];
     sumWeight += weight[time];
     sumDeviation += weight[time]*dist[time]*dist[time] ;
//      sumDeviation0 += dist[time]*dist[time] ;

     
     // ANB Filling the cal. and fit.  time difference
     ///     if(iflag ==1&&flag_cell==1) {               // only in case of single parti     if(iflag ==1&&flag_cell>=1) {               // only in case of single particle in 6-th layer
     if(iflag ==1&&flag_cell>=1) {
       //	 time_cal[time] = driftTime;
     }
       if(iflag == 3&&flag_cell==1+time&&funMin<250.&&weight[time]>0.97) {
	 if(cell==44&&lay==2) hCalFit2_44->Fill(dist[time]);
	 if(cell==45&&lay==2) hCalFit2_45->Fill(dist[time]);
	 if(cell==46&&lay==2) hCalFit2_46->Fill(dist[time]);
	 if(cell==47&&lay==2) hCalFit2_47->Fill(dist[time]);
	 if(cell==48&&lay==2) hCalFit2_48->Fill(dist[time]);
	 if(cell==49&&lay==2) hCalFit2_49->Fill(dist[time]);
	 if(cell==50&&lay==2) hCalFit2_50->Fill(dist[time]);
	 if(cell==51&&lay==2) hCalFit2_51->Fill(dist[time]);
	 if(cell==52&&lay==2) hCalFit2_52->Fill(dist[time]);
	 if(cell==53&&lay==2) hCalFit2_53->Fill(dist[time]);
	 if(cell==44&&lay==5) hCalFit5_44->Fill(dist[time]);
	 if(cell==45&&lay==5) hCalFit5_45->Fill(dist[time]);
	 if(cell==46&&lay==5) hCalFit5_46->Fill(dist[time]);
	 if(cell==47&&lay==5) hCalFit5_47->Fill(dist[time]);
	 if(cell==48&&lay==5) hCalFit5_48->Fill(dist[time]);
	 if(cell==49&&lay==5) hCalFit5_49->Fill(dist[time]);
	 if(cell==50&&lay==5) hCalFit5_50->Fill(dist[time]);
	 if(cell==51&&lay==5) hCalFit5_51->Fill(dist[time]);
	 if(cell==52&&lay==5) hCalFit5_52->Fill(dist[time]);
	 if(cell==53&&lay==5) hCalFit5_53->Fill(dist[time]);
	 //	 hCalFit->Fill(time_cal[time]-driftTime);
	 //	 hcalfit_2dim->Fill(time_cal[time],driftTime,1.);
// 	 cout << "weight = " << weight[time] << endl;  	     
// 	 cout << "dist! = " << dist[time]<< endl;
// 	 cout << "time_fit numb. = " << time << endl;  	     
// 	 cout << " time_cal = " << time_cal[time] << endl;
//  	 cout << " time_fit = " << driftTime << endl;
//   	 cout << " funMin = " << funMin << endl;
// 	 cout << "difference cal-fit = " << time_cal[time]-driftTime << endl;  	     

      }
       // ANE
     
     if(fprint) {
       
       if(iflag == 1 || iflag == 3) {
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

//        if(iflag == 3) {
//   	 if( sumDeviation < 150)
// 	   hist1[196]->Fill(dist[time],1.);
//        }
   }
//    printf(" ------------- width=%f\n",width); 

   width = cnst*sqrt(sumDeviation/sumWeight);
//     if(fprint) {
//       printf(" ------------- width=%f\n",width); 
//       cout << "------------- sumDeviation" << sumDeviation << endl;
//       cout << "------------- sumWeight" << sumWeight << endl;
//     }
      
   if(iflag == 1 || iflag == 3) {
//      cout << "------------- sumDeviation" << sumDeviation << endl;
//      cout << "------------- sumDeviation0" << sumDeviation0 << endl;
//      cout << "------------- sumWeight" << sumWeight << endl;
//      printf(" ------------- width=%f\n",width);
   }
 
   return sumDeviation;
 }

void HMdcTrackFitter::setHists(void) {
   
//   for(Int_t i=0; i<200; i++) hist1[i]=0;
  
//   hist1[196] = new TH1F("h196","Deviation",100,-50.,50.);
//   hist1[197] = new TH1F("h197","time per cluster",100,0.,.05);
//   hist1[  1] = new TH1F("fh1","Delta t1",50,-25.,25.);
//   hist1[  2] = new TH1F("fh2","Delta t2",50,-25.,25.);
//   hist1[  3] = new TH1F("fh3","Delta t3",50,-25.,25.);
//   hist1[  4] = new TH1F("fh4","Delta t4",50,-25.,25.);
//   hist1[  5] = new TH1F("fh5","Delta t5",50,-25.,25.);
//   hist1[  6] = new TH1F("fh6","Delta t6",50,-25.,25.);
//   hist1[  7] = new TH1F("fh7","Delta t7",50,-25.,25.);
//   hist1[  8] = new TH1F("fh8","Delta t8",50,-25.,25.);
//   hist1[  9] = new TH1F("fh9","Delta t9",50,-25.,25.);
//   hist1[ 10] = new TH1F("fh10","Delta t10",50,-25.,25.);
//   hist1[ 11] = new TH1F("fh11","Delta t11",50,-25.,25.);
//   hist1[ 12] = new TH1F("fh12","Delta t12",50,-25.,25.);
//   hist1[ 13] = new TH1F("fh13","Delta t13",50,-25.,25.);
//   hist1[ 14] = new TH1F("fh14","Delta t14",50,-25.,25.);
//   hist1[ 15] = new TH1F("fh15","Delta t15",50,-25.,25.);
//   hist1[ 16] = new TH1F("fh16","Delta t16",50,-25.,25.);
//   hist1[ 17] = new TH1F("fh17","Delta t17",50,-25.,25.);
//   hist1[ 18] = new TH1F("fh18","Delta t18",50,-25.,25.);
//   hist1[ 19] = new TH1F("fh19","Delta t19",50,-25.,25.);
//   hist1[ 20] = new TH1F("fh20","Delta t20",50,-25.,25.);
//   hist1[ 21] = new TH1F("fh21","Delta t21",50,-25.,25.);
//   hist1[ 22] = new TH1F("fh22","Delta t22",50,-25.,25.);
//   hist1[ 23] = new TH1F("fh23","Delta t23",50,-25.,25.);
//   hist1[ 24] = new TH1F("fh24","Delta t24",50,-25.,25.);
}

void HMdcTrackFitter::writeHists(void) {

//Writing hists:
  
//   TFile *fh=new TFile("fitter_hist.root","RECREATE");
   
//   for(Int_t i=0; i<200; i++)
//      if(hist1[i]) hist1[i]->Write();
   
//    fh->Close();
}

//ClassImp(HMdcTrackFitter)

Bool_t HMdcTrackFitter::finalize() {
  return kTRUE;
}







