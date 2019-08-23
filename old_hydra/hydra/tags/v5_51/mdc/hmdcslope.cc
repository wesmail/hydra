//*-- AUTHOR           : P.Zumbruch, J.Kempter, T.Bretz
//*-- Modified         : 26-06-2000 P.Zumbruch 
//*-- Last Modification: 15-05-2001 P.Zumbruch 

///////////////////////////////////////////////////////////////////////////////
//
// HMdcSlope
//
// Calculates the Slope parameter for MDC calibration. Uses cal1 container
// as an input data
//
///////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <fstream.h>
#include <iomanip.h>
#include <iostream.h>

#include <TH1.h>
#include <TH2.h>
#include <TF1.h>
#include <TNtuple.h>
#include <TGraphErrors.h>
#include <TStopwatch.h>

#include "heventheader.h"
#include "hmdcslope.h"
#include "hmdcdef.h"
#include "hmdcraw.h"
#include "hdebug.h"
#include "hades.h"
#include "hiterator.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hevent.h"
#include "hcategory.h"
#include "hlocation.h"
#include "hmdccal1.h"
#include "hmdclookupgeom.h"
#include "hmdccalpar.h"
#include "hmdccalparraw.h"
#define HMDC_SLOPE_NALGORITHM 3
#define HMDC_SLOPE_NFITHISTOGRAMMMETHODS 2

ClassImp(HMdcSlope)

HMdcSlope::HMdcSlope()
{
    rawCat = NULL;
    iter   = NULL;
    file   = NULL;

    avgSlope               = (MyFloatField*)new MyFloatField;
    avgSlopeErr            = (MyFloatField*)new MyFloatField;
    nEvt                   = (MyIntField*)  new MyIntField;

    linRegActive           = kTRUE;
    histFitActive          = kFALSE;
    graphFitActive         = kFALSE;

    selectorActive         = kFALSE;

    createNtupleActive     = kFALSE;

    debugActive            = 0;
    trendCalculationActive = kFALSE;
    calibrationAlgorithm   = 1;
    nTimeValues            = 6;
}

HMdcSlope::HMdcSlope(Text_t* name,Text_t* title) : HReconstructor(name,title)
{
    rawCat = NULL;
    iter   = NULL;
    file   = NULL;

    avgSlope               = (MyFloatField*)new MyFloatField;
    avgSlopeErr            = (MyFloatField*)new MyFloatField;
    nEvt                   = (MyIntField*)  new MyIntField;

    linRegActive           = kTRUE;
    histFitActive          = kFALSE;
    graphFitActive         = kFALSE;

    selectorActive         = kFALSE;

    createNtupleActive     = kFALSE;

    debugActive            = 0;
    trendCalculationActive = kFALSE;
    calibrationAlgorithm   = 1;
    nTimeValues            = 6;
}

HMdcSlope::~HMdcSlope()
{
    // destructor deletes the iterator
    if (iter)       delete iter;
    if (fNameRoot)  delete fNameRoot;
    if (avgSlope)   delete[] avgSlope;
    if (avgSlopeErr)delete[] avgSlopeErr;
    if (nEvt)       delete[] nEvt;
    iter = NULL;
}

void HMdcSlope::setOutputRoot(Char_t *c)
{
  if (fNameRoot) delete fNameRoot;
  fNameRoot = new Char_t[strlen(c)+1];
  strcpy(fNameRoot, c);
  return;
}

void HMdcSlope::setMbSelector(Int_t i)
{ 
  mbSelector = 0;
  if (i >= 0 && i <=16)
    {
      mbSelector = i;
    }
  return;
}

void HMdcSlope::selectCalibrationAlgorithm(Int_t i)
{
  calibrationAlgorithm=0;
  if (i >= 0 && i <= HMDC_SLOPE_NALGORITHM) 
      {
	calibrationAlgorithm=i;
      }
  return;
}

void HMdcSlope::selectHistFitMethod(Int_t i)
{
  calibrationAlgorithm=0;
  if (i >= 0 && i <= HMDC_SLOPE_NFITHISTOGRAMMMETHODS) 
      {
	fitHistMethod=i;
      }
  return;
}

void HMdcSlope::setnTimeValues(Int_t i)
{
  nTimeValues=6;
  if (i >= 2 && i <= 6) 
      {
	nTimeValues=i;
      }
}

void HMdcSlope::setTrendCalculationActive(Bool_t b)
{
  trendCalculationActive = b;
  return;
}

void HMdcSlope::setTdcSelector(Int_t i)
{
  tdcSelector = 0;
  if (i >= 0 && i <= 95)
    {
      tdcSelector = i;
    }
  return;
}

void HMdcSlope::setBinError(Int_t i)
{
  binError=0;
  if (i >= 0)
    {
      binError = i;
    }
  return;
}

void HMdcSlope::setSelectorActive(Bool_t b)
{
  selectorActive = b;
}

void HMdcSlope::setCreateNtupleActive(Bool_t b)
{
  createNtupleActive = b;
  if (createNtupleActive) cout << "setting NtupleActive" << endl;
}

void HMdcSlope::setOutputNtuple(Char_t *c)
{
  if (fNameNtuple) delete fNameNtuple;
  fNameNtuple = new Char_t[strlen(c)+1];
  strcpy(fNameNtuple, c);
  return;
}

void HMdcSlope::setLinRegActive(Bool_t b)
{
  linRegActive = b;
}

void HMdcSlope::setHistFitActive(Bool_t b)
{
  histFitActive = b;
}

void HMdcSlope::setGraphFitActive(Bool_t b)
{
  graphFitActive = b;
}

void HMdcSlope::setDebugActive(Int_t i)
{
  debugActive = 0;
  if (i >=0)
    {
      debugActive = i;
    }
  return;
}

inline void HMdcSlope::fitHistogramm(Float_t* slope, Float_t* sigma,
				     Int_t sector, Int_t module, 
				     Int_t motherboard, Int_t tdc, Int_t method)
{
  switch(method)
    {
    case 1: 
      fitHistogrammMaxCondition (slope, sigma, sector, module, motherboard, tdc);
      break;
    case 2:
      fitHistogrammMeanCondition(slope, sigma, sector, module, motherboard, tdc);
      break;
    default:
      cout << "wrong histogramm fitting method" << endl;
      return;
      break;
    }
  return;
}

inline void HMdcSlope::fitHistogrammMaxCondition(Float_t* slope, Float_t* sigma,
				     Int_t sector, Int_t module, Int_t motherboard, Int_t tdc)
{
  Char_t tmp[80];
  sprintf(tmp,"fithist_sec%i_mod%i_mb%i_tdc%i",sector,module,motherboard,tdc);

  if (fNameRoot) file->cd();

  TH1F *proj = new TH1F(tmp,tmp,5000,0.3,0.8);

  for (Int_t myiter=0;myiter<5000;myiter++)
    { 
      proj->SetBinContent(myiter,slopeTrend[sector][module][motherboard][tdc][myiter]);
    }

  Float_t xminfitbin=(proj->GetMaximum())-0.001;
  Float_t xmaxfitbin=(proj->GetMaximum())+0.001;
  TF1 *f = new TF1("f1","gaus", xminfitbin, xmaxfitbin); 
  proj->Fit("f1","RNQ");
  
  *slope = (Float_t)f->GetParameter(1);
  *sigma = (Float_t)f->GetParameter(2);
 
  if ((debugActive & 0x20)) cout << endl << "Slope " << *slope << endl;

if (fNameRoot)
  {
    file->cd();  
    proj->SetFillColor(2);
    proj->Write();
    if ((debugActive & 0x20)) cout << "writing file" << endl;
  }

  delete proj;
  delete f;
  return;
}

inline void HMdcSlope::fitHistogrammMeanCondition(Float_t* slope, Float_t* sigma,
				     Int_t sector, Int_t module, Int_t motherboard, Int_t tdc)
{
  Char_t tmp[80];
  sprintf(tmp,"fithist_sec%i_mod%i_mb%i_tdc%i",sector,module,motherboard,tdc);

  if (fNameRoot) file->cd();

  TH1F *proj = new TH1F(tmp,tmp,5000,0.3,0.8);

  for (Int_t myiter=0;myiter<5000;myiter++)
    { 
      proj->SetBinContent(myiter,slopeTrend[sector][module][motherboard][tdc][myiter]);
    }

  Float_t xminfitbin=(proj->GetMean())-10*0.0001;
  Float_t xmaxfitbin=(proj->GetMean())+10*0.0001; 
  TF1 *f = new TF1("f1","gaus", xminfitbin, xmaxfitbin); 
  proj->Fit("f1","RNQ");
  
  *slope = (Float_t)f->GetParameter(1);
  *sigma = (Float_t)f->GetParameter(2);
 
  if ((debugActive & 0x20)) cout << endl << "Slope " << *slope << endl;

if (fNameRoot)
  {
    file->cd();  
    proj->SetFillColor(2);
    proj->Write();
    if ((debugActive & 0x20)) cout << "writing file" << endl;
  }

  delete proj;
  delete f;
  return;
}

Bool_t HMdcSlope::init(void)
{
 
  // creates the parameter containers MdcCalPar and MdcLookup if they do not
  // exist and adds them to the list of parameter containers in the runtime
  // database
  
  calparraw=(HMdcCalParRaw*)gHades->getRuntimeDb()->getContainer("MdcCalParRaw");
  
  // creates the MdcRaw category and adds them to the current
  // event
  
  rawCat=gHades->getCurrentEvent()->getCategory(catMdcRaw);
  if (!rawCat) 
    {
      rawCat=gHades->getSetup()->getDetector("Mdc")->buildCategory(catMdcRaw);
      if (!rawCat) return kFALSE;
      else gHades->getCurrentEvent()->addCategory(catMdcRaw,rawCat,"Mdc");
    }

  // creates an iterator which loops over all fired cells
  
  iter=(HIterator *)rawCat->MakeIterator();

  if (trendCalculationActive)
    {
      fitHistMethod=2;
    }

  if (fNameRoot) 
    {
      file = new TFile(fNameRoot,"RECREATE");
      file->cd();
      if ((debugActive & 0x02)) printf("File %s aktiv \n",fNameRoot);
      hSlope = new TH1F("Slope", "Slope", 800, 0,0.8);
      hTime  = new TH1F("Time" , "Time", 4096, 0,4096);
    } 

  if (createNtupleActive)
    {
      if ((debugActive & 0x02)) printf("generating Ntuple \n");
       if (fNameNtuple) ntupleOutputFile = new TFile(fNameNtuple,"RECREATE");
       else ntupleOutputFile = new TFile("Slope-ntuple.root","RECREATE");
       ntupleOutputFile->cd();
       ntuple = new TNtuple("ntuple",
			    "ntuple",
			    "index:time:evtSeqNr:sector:module:mbo:tdc:slope:error:t1:t2:t3:t4:t5:t6");
    }

  
 fActive=kTRUE;
 return kTRUE;
}

Bool_t HMdcSlope::finalize(void)
{
  HDetector *mdcDet = gHades->getSetup()->getDetector("Mdc");
  
  if (!mdcDet)
    {
      cout << "Error in HMdcSlope::finalize: Mdc-Detector setup (gHades->getSetup()->getDetector(\"Mdc\")) missing." << endl;
    }
  else
    {
      Float_t slope, sigma;
      //loop over sectors
      for(Int_t s=0; s<6; s++) 
	{  
	  //loop over modules
	  for(Int_t mo=0; mo<4; mo++) 
	    {  
	      if (!mdcDet->getModule(s, mo)) continue;
	      Int_t nMb=(*calparraw)[s][mo].getSize();
	      //loop over motherboards
	      for(Int_t mb=0; mb<nMb; mb++) 
		{ 
		  Int_t nTdc=(*calparraw)[s][mo][mb].getSize();
		  for(Int_t t=0; t<nTdc; t++) 
		    {
		      Int_t n=(*nEvt)[s][mo][mb][t];
		 
		      // 
		      // Trend Calculation 
		      // 
		      if (trendCalculationActive)
			{
			  if (n)
			    {
			      fitHistogramm(&slope, &sigma, s, mo, mb, t, 
					    fitHistMethod);
			      if (slope<1)
				{
				  (*calparraw)[s][mo][mb][t].setSlope(slope);
				  (*calparraw)[s][mo][mb][t].setSlopeErr(sigma);
				  (*calparraw)[s][mo][mb][t].setSlopeMethod(2);
				}
			      else
				{
				  (*calparraw)[s][mo][mb][t].setSlope(0);
				  (*calparraw)[s][mo][mb][t].setSlopeErr(0);
				  (*calparraw)[s][mo][mb][t].setSlopeMethod(3);
				} 
			    }
			  else
			    {
			      (*calparraw)[s][mo][mb][t].setSlope(0.5);
			      (*calparraw)[s][mo][mb][t].setSlopeErr(0);
			      (*calparraw)[s][mo][mb][t].setSlopeMethod(4);
			    }
			}
		      // 
		      // Online Calculation !!!!!!!!!!! 
		      // 
		      else 
			{
			  (*calparraw)[s][mo][mb][t].setSlope(n?((*avgSlope)[s][mo][mb][t]/n):0);
			  (*calparraw)[s][mo][mb][t].setSlopeErr(n?((*avgSlopeErr)[s][mo][mb][t]/n):0);
			}
		    }
		}
	    }
	}
    }
  // writing to file 


  if (file)
    {
      file->cd();
      hSlope->Write();
      hTime-> Write();

      file->Save();
      file->Close();
      delete file;
    }

  if (createNtupleActive)
    {
      ntupleOutputFile->cd();
      ntuple->Write();
      ntupleOutputFile->Save();
      ntupleOutputFile->Close();
      delete ntupleOutputFile;
    }

  // free memory space;//??

  if (trendCalculationActive)
    {
    }
  
  return kTRUE;
}

inline void HMdcSlope::calcSlope(Int_t t0, Int_t t1, Int_t t2, Int_t t3, Int_t t4, Int_t t5, 
				 Float_t delta, 
				 Float_t* myslope, 
				 Float_t* myerror, 
				 Float_t* chiSquare, 
				 Int_t nvals, 
				 Int_t myAlgorithm)
{
  if  (t5<0 || t4<0 || t3<0 || t2<0 || t1<0 || t0<0)  
    {
      *myslope  =  0;
      *myerror  = -99;
      *chiSquare= -99;
      return;
    }

 switch(myAlgorithm)
   {
   case 1: 
     calc     (t0,t1,t2,t3,t4,t5, delta, myslope, myerror);
     break;   
   case 2: 
     calc5    (t0,t1,t2,t3,t4,t5, delta, myslope, myerror);
     break;   
   case 3:
     calcCHI  (t0,t1,t2,t3,t4,t5, delta, myslope, myerror);
     break;   
   case 4:
     calcGraph(t0,t1,t2,t3,t4,t5, delta, myslope, myerror);
     break;   
   default:
     *myslope  =0;
     *myerror  =-99;
     *chiSquare=-99;
     break;
   }

return;
}

inline void HMdcSlope::calc(Int_t t0, Int_t t1, Int_t t2, Int_t t3, Int_t t4, Int_t t5,
			    Float_t delta, Float_t *myslope, Float_t *myerror)
{

  // Linear regression
  // with 6 equal distanced base points, (t[i] = delta * i, i = 1...6) 
  // f(t) = a0 + a1 * t
  // 
  // s2a1: (standard deviation)^2 of mya1
  // slope = -1/mya1;
  //                                sqrt(s2a1) 
  // standard deviation of slope = --------------
  //                                  (mya1)^2
  //
  
  if  (t5<0 || t4<0 || t3<0 || t2<0 || t1<0 || t0<0)  
    {
      *myslope = 0;
      *myerror = 0;
      return;
    }
  
  // correct error calculation has to be improved
  
  Double_t sumt ;     // t0   + ... + t5
  Double_t sumkt;     // t0*1 + ... + t5*6
  Double_t s2y  ;     // variance of y
  Double_t sa1  ;     // standard deviation of a1
  Double_t mya0 ;     // a0
  Double_t mya1 ;     // a1
  Double_t difference[6]; 
  Int_t ybin[6];
  
  ybin[0]=t0;
  ybin[1]=t1;
  ybin[2]=t2;
  ybin[3]=t3;
  ybin[4]=t4;
  ybin[5]=t5;
  
  sumt = ybin[0] +ybin[1]  +ybin[2]  +ybin[3]  +ybin[4]  +ybin[5];

  //         n          
  //        ---         
  //        \           
  // sumt  = > ybin[k-1]
  //        /           
  //        ---         
  //        k=1         

  sumkt= ybin[0] +ybin[1]*2+ybin[2]*3+ybin[3]*4+ybin[4]*5+ybin[5]*6;

  //         n  
  //        --- 
  //        \   
  // sumkt = > k * ybin[k-1]
  //        /  
  //        ---
  //        k=1
  
  mya0=(13/15)*sumt-0.2*sumkt;
  
  //                      ---          n                n              ---
  //                      |           ---              ---               |
  //              1       |           \                \                 |
  // mya0 = ------------- |  2(2n + 1) > ybin[k-1] - 6  > k * ybin[k-1]  |
  //         n * (n - 1)  |           /                /                 |
  //                      |           ---              ---               |
  //                      ---         k=1              k=1             ---
  //
  //              1                  
  //      = ------------- (  2(2n + 1) sumt - 6 * sumkt) 
  //         n * (n - 1)             
  //                                 
  //                                                    , n=number of Points (=6)
  //
  //    (n=6)     1                                         13
  //      = ------------- ( 2(2*6 + 1) sumt - 6 * sumkt) = ---- sumt - 0.2 sumkt 
  //         6 * (6 - 1)                                    15
  //                                 

 
  mya1=(2*sumkt-7*sumt)/(35*delta);

  //                               ---   n                        n            ---   
  //                               |    ---                      ---             |
  //                   1           |    \                        \               |
  // mya1 = ---------------------- | 12  > k * ybin[k-1] - 6(n+1) > ybin[k-1]    |
  //         delta * n * (n^2 - 1) |    /                        /               |
  //                               |    ---                      ---             |
  //                               ---  k=1                      k=1           ---
  //
  //                   1               
  //      = ---------------------- ( 12 * sumkt - 6(n+1) * sumt )
  //         delta * n * (n^2 - 1)     
  //
  //                                                    , n=number of Points (=6)
  //
  //    (n=6)          1               
  //      = ---------------------- (12 * sumkt - 6(6+1) * sumt)  
  //         delta * 6 * (6^2 - 1)     
  //
  //              1                
  //      = ----------- (2 * sumkt - 7 * sumt)  
  //         delta * 35     
  //

  
  for (Int_t k=1;k<=6;k++)
    { 
      difference[k-1]=(ybin[k-1]-mya0-mya1*delta*k)*(ybin[k-1]-mya0-mya1*delta*k);
    }

  s2y=(difference[0]+difference[1]+difference[2]+difference[3]+difference[4]+difference[5])/4;

  // s2y:s^2y: variance in y:
  //
  //                    n
  //                   ---  
  //           1       \                                    2
  //  s^2y = ----- *    >  (ybin[k-1] - a0 - a1 * k * delta) 
  //         n - 2     /
  //                   ---
  //                   k=1
  //
  //                    n
  //                   ---  
  //           1       \                  2
  //       = ----- *    >  difference[k-1]                     , n=number of Points (=6)
  //         n - 2     /
  //                   ---
  //                   k=1
  //

  
  sa1=(sqrt(s2y/10)/delta); 

  // sa1: standard deviation of a1: 
  //
  //            ------------------------------
  //           /                  12        
  // sa1 =    / (sy^2)*  ---------------------  , n=number of Points (=6)
  //       \ /           delta * n * (n^2 - 1)
  //        V
  //
  //
  //            ------------------------------
  //           /                  12        
  //     =    / (sy^2)*  ---------------------  
  //       \ /           delta * 6 * (6^2 - 1)
  //        V
  //
      
  *myslope=(Float_t) (-1/mya1);

  //            -1
  // myslope = ----
  //           mya1

  *myerror=(Float_t) ((sa1)/(mya1*mya1));
 
  //
  //             sa1
  // myerror = -------
  //                2
  //            mya1

  return; 
}

inline void HMdcSlope::calc5(Int_t t0, Int_t t1, Int_t t2, Int_t t3, Int_t t4, Int_t t5,
			    Float_t delta, Float_t *myslope, Float_t *myerror)
{
  if (debugActive &0x10) cout << "." << flush;
  // Linear regression
  // with 5 equal distanced base points, (t[i] = delta * i, i = 1...5) 
  // f(t) = a0 + a1 * t
  // 
  // s2a1: (standard deviation)^2 of mya1
  // slope = -1/mya1;
  //                                sqrt(s2a1) 
  // standard deviation of slope = --------------
  //                                  (mya1)^2
  //
  
  if  (t5<0 || t4<0 || t3<0 || t2<0 || t1<0 || t0<0)  
    {
      *myslope = 0;
      *myerror = 0;
      return;
    }
  
  // correct error calculation has to be improved
  
  Double_t sumt ;     // t0   + ... + t4
  Double_t sumkt;     // t0*1 + ... + t4*5
  Double_t s2y  ;     // variance of y
  Double_t sa1  ;     // standard deviation of a1
  Double_t mya0 ;     // a0
  Double_t mya1 ;     // a1
  Double_t difference[5]; 
  Int_t ybin[5];
  
  ybin[0]=t0;
  ybin[1]=t1;
  ybin[2]=t2;
  ybin[3]=t3;
  ybin[4]=t4;
  
  sumt =ybin[0]+ybin[1]  +ybin[2]  +ybin[3]  +ybin[4];
  sumkt=ybin[0]+ybin[1]*2+ybin[2]*3+ybin[3]*4+ybin[4]*5;
  
  mya0=+1.1*sumt-0.3*sumkt;
  
  //                      ---          n                n              ---
  //                      |           ---              ---               |
  //              1       |           \                \                 |
  // mya0 = ------------- |  2(2n + 1) > ybin[k-1] - 6  > k * ybin[k-1]  |
  //         n * (n - 1)  |           /                /                 |
  //                      |           ---              ---               |
  //                      ---         k=1              k=1             ---
  //
  //              1                  
  //      = ------------- (  2(2n + 1) sumt - 6 * sumkt) 
  //         n * (n - 1)             
  //                                 
  //                                                    , n=number of Points (=5)
  //
  //    (n=5)     1                  
  //      = ------------- ( 2(2*5 + 1) sumt - 6 * sumkt) = 1.1 sumt - 0.3 sumkt 
  //         5 * (5 - 1)             
  //                                 

 
  mya1=(sumkt-3*sumt)/(10*delta);

  //                               ---   n                        n            ---   
  //                               |    ---                      ---             |
  //                   1           |    \                        \               |
  // mya1 = ---------------------- | 12  > k * ybin[k-1] - 6(n+1) > ybin[k-1]    |
  //         delta * n * (n^2 - 1) |    /                        /               |
  //                               |    ---                      ---             |
  //                               ---  k=1                      k=1           ---
  //
  //                   1               
  //      = ---------------------- ( 12 * sumkt - 6(n+1) * sumt )
  //         delta * n * (n^2 - 1)     
  //
  //                                                    , n=number of Points (=5)
  //
  //    (n=5)          1               
  //      = ---------------------- (12 * sumkt - 6(5+1) * sumt)  
  //         delta * 5 * (5^2 - 1)     
  //
  //              1                
  //      = ----------- (sumkt - 3 sumt)  
  //         delta * 10     
  //
  
  for (Int_t k=1;k<=5;k++)
    { 
      difference[k-1]=(ybin[k-1]-mya0-mya1*delta*k)*(ybin[k-1]-mya0-mya1*delta*k);
    }

  s2y=(difference[0]+difference[1]+difference[2]+difference[3]+difference[4])/3;

  // s2y:s^2y: variance in y:
  //
  //                    n
  //                   ---  
  //           1       \                                    2
  //  s^2y = ----- *    >  (ybin[k-1] - a0 - a1 * k * delta) 
  //         n - 2     /
  //                   ---
  //                   k=1
  //                    n
  //                   ---  
  //           1       \                  2
  //       = ----- *    >  difference[k-1]                     , n=number of Points (=5)
  //         n - 2     /
  //                   ---
  //                   k=1
  

  sa1=(sqrt(s2y/10)/delta); 

  // sa1: standard deviation of a1: 
  //
  //            ------------------------------
  //           /                  12        
  // sa1 =    / (sy^2)*  ---------------------  , n=number of Points (=5)
  //       \ /           delta * n * (n^2 - 1)
  //        V
  //
      
  *myslope=(Float_t) (-1/mya1);

  //            -1
  // myslope = ----
  //           mya1

  *myerror=(Float_t) ((sa1)/(mya1*mya1));
 
  //
  //             sa1
  // myerror = -------
  //                2
  //            mya1

if ((debugActive & 0x10))
  {
    cout << endl;
    cout << "LS" << endl;
    cout <<  (-1/mya1) <<" " << ((sa1)/(mya1*mya1)) << endl;
  }

  return; 
}

inline void HMdcSlope::calcCHI(Int_t t0, Int_t t1, Int_t t2, Int_t t3, Int_t t4, Int_t t5,
			       Float_t delta, Float_t *myslope, Float_t *myerror)
{
  
  // fitting with 5 base points, neglecting 6th time
  
  if  (t5<0 || t4<0 || t3<0 || t2<0 || t1<0 || t0<0)  
    {
      *myslope = 0;
      *myerror = 0;
      return;
    }
  
  TH1F *hFitarea = new TH1F("fit","fit",5,0.5,5.5);
  hFitarea->SetBinContent((Int_t) 1 ,t0);
  hFitarea->SetBinContent((Int_t) 2 ,t1);
  hFitarea->SetBinContent((Int_t) 3 ,t2);
  hFitarea->SetBinContent((Int_t) 4 ,t3);
  hFitarea->SetBinContent((Int_t) 5 ,t4);
  
  for (Int_t myiter=1;myiter <=5; myiter++)
    {
      hFitarea->SetBinError((Int_t) myiter, binError);
    }
  
  // calculation of slope & chisquare
  
  TF1 *f = new TF1("f1","pol1", 0,6); // call fit function poynom 1.order in x-range
  hFitarea->Fit("f1","RNQ");
  
  delete hFitarea;
  
  *myslope = (Float_t) ((-1)/f->GetParameter(1)*delta) ;
  *myerror = (Float_t) (f->GetParError(1)/((*myslope)*(*myslope)*delta));
  //  *chi2= (Float_t) (f->GetChisquare()/f->GetNDF());

  if (debugActive & 0x10)
    {
      cout << endl;
      cout << "chi" << endl;
      cout << *myslope <<" " << *myerror << endl;
    }

  delete f;

  return; 
}


inline void HMdcSlope::calcGraph(Int_t t0, Int_t t1, Int_t t2, Int_t t3, Int_t t4, Int_t t5,
				 Float_t delta, Float_t *myslope, Float_t *myerror)
{
  Int_t nrOfPts=5;
  Float_t x[5];
  Float_t y[5];
  Float_t ex[5];
  Float_t ey[5];
      
  for (Int_t myiter=0;myiter<5; myiter++)
    {
      x[myiter] =(myiter+1)*delta;
      ex[myiter]=0;
      ey[myiter]=binError;
    }

  y[0]= t0;
  y[1]= t1;
  y[2]= t2;
  y[3]= t3;
  y[4]= t4;
 
  TGraphErrors *gFitGraph = new TGraphErrors(nrOfPts,x,y,ex,ey);
   
  // calculation of slope & chisquare
  
  TF1 *f = new TF1("f1","pol1", 0,6*delta); // call fit function poynom 1.order in x-range
  gFitGraph->Fit("f1","RNQ");
  
  delete gFitGraph;
  
  *myslope = (Float_t) ((-1)/f->GetParameter(1)) ;
  *myerror = (Float_t) (f->GetParError(1)/((*myslope)*(*myslope)));
  //  *chi2= (Float_t) (f->GetChisquare()/f->GetNDF());

if(debugActive & 0x10)
  {
    cout << endl;
    cout << "Graph" << endl;
    cout << *myslope <<" " << *myerror << endl;
  }

  delete f;
  return; 
}

inline Int_t HMdcSlope::calcBinNr(Double_t value, Double_t min, Double_t max, Int_t nbins)
{
  if (min<max && value >= min && value <= max)
    {
      return ((Int_t)((nbins/(max-min))*(value - min)));
    }
  return 0;
}

inline Int_t HMdcSlope::execute(void)
{
  static HMdcRaw* raw=NULL;
  static Int_t s, mo, mb, t;
  static Int_t index;

  index++;

  if (debugActive& 0x04){if (!(index%10)) cout << index << endl;}

  UInt_t DaqTime     = (gHades->getCurrentEvent()->getHeader()->getTime());
  Int_t DaqHour      =        ((DaqTime & 0x00ff0000) >> 16);
  Int_t DaqMin       =        ((DaqTime & 0x0000ff00) >> 8);
  Int_t DaqSec       =          DaqTime & 0x000000ff;
  Float_t DaqJustSec = 3600*DaqHour + 60*DaqMin + DaqSec;
  
  Float_t delta = 401.5/2; // delta of 401.5/2 should be somewhen retrieved from the database !!!!
  iter->Reset();

  // 
  // TrendCalculation
  // 

  if (trendCalculationActive)
    { 
      // 
      // TrendCalculation
      // 
      // the idea of this offline procedure is to first fill 
      // the calculated slopes of the whole dataset into 
      // a histogramm (SlopeTrend) and to derive the slope value 
      // as the result of a Gaussian Fit.
      // Where the slope corresponds to the first moment
      // and the error to the sigma
      //

      static Float_t slope, error, chiSquare; 
            
      while ((raw=(HMdcRaw*)iter->Next()))
	{
	  raw->getAddress(s, mo, mb, t);

	  calcSlope(raw->getTime(1)+2048,
		    raw->getTime(2)+2048,
		    raw->getTime(3)+((raw->getTime(3)<raw->getTime(4))?2048:0),
		    raw->getTime(4),
		    raw->getTime(5),
		    raw->getTime(6),
		    delta,
		    &slope,
		    &error,
		    &chiSquare,
		    nTimeValues,
		    calibrationAlgorithm);
	
	  slopeTrend[s][mo][mb][t][calcBinNr(slope,0.3,0.8,5000)]++;
	  
	  (*nEvt)[s][mo][mb][t]++;

	  if(file) 
	    {
	      file->cd();
	      hSlope->Fill(slope);
	      for (Int_t i=1; i<7; i++)
		{
		  hTime->Fill(raw->getTime(i)
			      +((((i<3) ||(i==3 && raw->getTime(3)<raw->getTime(4))))?2048:0));
		}
	    }


	  if (createNtupleActive)
	    {
	      ntupleOutputFile->cd();
	      if (!selectorActive || 
		  (selectorActive && 
		   (mbSelector == mb || mbSelector == -1) &&
		   (tdcSelector == t || tdcSelector == -1) ))
		{
		  ntuple->Fill(index,
			       DaqJustSec,
			       gHades->getCurrentEvent()->getHeader()->getEventSeqNumber(),
			       s,mo,mb,t,slope,error,
			       raw->getTime(1)+2048,
			       raw->getTime(2)+2048,
			       raw->getTime(3)+((raw->getTime(3)<raw->getTime(4))?2048:0),
			       raw->getTime(4),
			       raw->getTime(5),
			       raw->getTime(6));
		}
	    }
	}
      return 0;
    }

  //
  // Online Calculation
  //
  else
    {
      //
      // Online Calculation
      //
      // For each event the slope is calculated with the chosen algorithm
      // and its content is written to the calparraw container
      //

      static Float_t slope[3], error[3], chiSquare[3]; 

      Bool_t algorithmActive[3] = {linRegActive, histFitActive, graphFitActive}; 

      while ((raw=(HMdcRaw*)iter->Next()))
	{
	  raw->getAddress(s, mo, mb, t);
	  
	  if (!selectorActive || 
	      (selectorActive && (mbSelector==mb || mbSelector == -1) && (tdcSelector == t || tdcSelector == -1) ))
	    {
	      for (Int_t methodIter=0; methodIter <= HMDC_SLOPE_NALGORITHM; methodIter++)
		{
		  if (algorithmActive[methodIter]==kTRUE)
		    {
		      calcSlope(raw->getTime(1)+2048,
				raw->getTime(2)+2048,
				raw->getTime(3)+((raw->getTime(3)<raw->getTime(4))?2048:0),
				raw->getTime(4),
				raw->getTime(5),
				raw->getTime(6),
				delta,
				&slope[methodIter],
				&error[methodIter],
				&chiSquare[methodIter],
				nTimeValues,
				methodIter+1);
		    }
		}
	      
	      (*avgSlope)   [s][mo][mb][t] += slope[0];
	      (*avgSlopeErr)[s][mo][mb][t] += error[0];
	      (*nEvt)       [s][mo][mb][t] ++;
	      
	      if(file) 
		{
		  hSlope->Fill(slope[0]);
		  for (Int_t i=1; i<7; i++)
		    {
		      hTime->Fill(raw->getTime(i)
				  +((((i<3) ||(i==3 && raw->getTime(3)<raw->getTime(4))))?2048:0));
		    }
		}
	      
	      if (slope[0]!=0 && slope[0] <1)
		{	   
		  (*calparraw)[s][mo][mb][t].setSlope   (slope[0]);
		  (*calparraw)[s][mo][mb][t].setSlopeErr(error[0]);
		  (*calparraw)[s][mo][mb][t].setSlopeMethod(1);
		}
	      else if(slope[0]==0)
		{
		  // in later versions the values should be set to a default or former value
		  (*calparraw)[s][mo][mb][t].setSlope(0.5);
		  (*calparraw)[s][mo][mb][t].setSlopeErr(0.1);
		  (*calparraw)[s][mo][mb][t].setSlopeMethod(4);
		}
	      else if(slope[0]>1)
		{
		  (*calparraw)[s][mo][mb][t].setSlope(0);
		  (*calparraw)[s][mo][mb][t].setSlopeErr(0);
		  (*calparraw)[s][mo][mb][t].setSlopeMethod(3); // manually set
		}
	    }
	}
      return 0;
    }
}

