//*-- AUTHOR   : P.Zumbruch, J.Kempter, T.Bretz
//*-- Modified : 02-22-2000 P.Zumbruch 

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

ClassImp(HMdcSlope)

HMdcSlope::HMdcSlope()
{
    rawCat = NULL;
    iter   = NULL;

    file   = NULL;

    avgSlope    = (MyFloatField*)new MyFloatField;
    avgSlopeErr = (MyFloatField*)new MyFloatField;
    nEvt        = (MyIntField*)  new MyIntField;

}

HMdcSlope::HMdcSlope(Text_t* name,Text_t* title) : HReconstructor(name,title)
{
    rawCat = NULL;
    iter   = NULL;

    file   = NULL;

    avgSlope    = (MyFloatField*)new MyFloatField;
    avgSlopeErr = (MyFloatField*)new MyFloatField;
    nEvt        = (MyIntField*)  new MyIntField;

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
}


Bool_t HMdcSlope::init(void)
{
  // creates the parameter containers MdcCalPar and MdcLookup if they do not
  // exist and adds them to the list of parameter containers in the rumtime
  // database

  calparraw=(HMdcCalParRaw*)gHades->getRuntimeDb()->getContainer("MdcCalParRaw");
  if (!calparraw)
    {
      calparraw=new HMdcCalParRaw;
      gHades->getRuntimeDb()->addContainer(calparraw);
    }
  
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

  if (fNameRoot) 
    {
      file = new TFile(fNameRoot,"RECREATE");
      hSlope = new TH1F("Slope", "Slope",  500, 0.3, 0.8);
      hTime  = new TH1F("Time" ,  "Time", 4096,    0, 4096);
    }

  fActive=kTRUE;
  
  return kTRUE;
}

Bool_t HMdcSlope::finalize(void)
{
  cout << "From HMdcSlope::finalize" << endl;
  
  HDetector *mdcDet = gHades->getSetup()->getDetector("Mdc");
  
  if (!mdcDet)
    cout << "Error in HMdcSlope::finalize: Mdc-Detector setup (gHades->getSetup()->getDetector(\"Mdc\")) missing." << endl;
  else
    {
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
		      (*calparraw)[s][mo][mb][t].setSlope(n?((*avgSlope)[s][mo][mb][t]/n):0);
		      (*calparraw)[s][mo][mb][t].setSlopeErr(n?((*avgSlopeErr)[s][mo][mb][t]/n):0);
		    }
		}
	    }
	}
    }

  if (file)
    {
      file->cd();
      hSlope->Write();
      delete hSlope;
      hTime-> Write();
      delete hTime;
      file->Save();
      file->Close();
      delete file;
    }
  
  return kTRUE;
}

inline void HMdcSlope::calc(Int_t t0, Int_t t1, Int_t t2, Int_t t3, Int_t t4, Int_t t5, Float_t delta, Float_t *slope, Float_t *error)
{
  // Linear regression
  // with 5 equal distanced base points, (t[i] = delta * i, i = 1...5) 
  // f(t) = a0 + a1 * t
  // 
  // LSs2a1: (standard deviation)^2 of mya1
  // slope = -1/mya1;
  //                                sqrt(LSs2a1) 
  // standard deviation of slope = --------------
  //                                  (mya1)^2
  //
  
  if  (t5<0 || t4<0 || t3<0 || t2<0 || t1<0 || t0<0)  
    {
      *slope = 0;
      *error = 0;
      return;
    }
  
  // correct error calculation has to be improved
  
  Double_t LSsumt ;     // t0   + ... + t4
  Double_t LSsumkt;     // t0*1 + ... + t4*5
  Double_t LSs2y  ;     // variance of y
  Double_t LSsa1  ;     // standard deviation of a1
  Double_t mya0   ;     // a0
  Double_t mya1   ;     // a1
  Double_t LSdelta2[5]; 
  Int_t ybin[5];
  
  ybin[0]=t0;
  ybin[1]=t1;
  ybin[2]=t2;
  ybin[3]=t3;
  ybin[4]=t4;
  
  LSsumt =ybin[0]+ybin[1]  +ybin[2]  +ybin[3]  +ybin[4];
  LSsumkt=ybin[0]+ybin[1]*2+ybin[2]*3+ybin[3]*4+ybin[4]*5;
  
  mya0=+1.1*LSsumt-0.3*LSsumkt;
  mya1=(LSsumkt-3*LSsumt)/(10*delta);
  
  for (Int_t k=1;k<=5;k++)
    { 
      LSdelta2[k-1]=(ybin[k-1]-mya0-mya1*delta*k)*(ybin[k-1]-mya0-mya1*delta*k);
    }

  LSs2y=(LSdelta2[0]+LSdelta2[1]+LSdelta2[2]+LSdelta2[3]+LSdelta2[4])/3;
  LSsa1=(sqrt(LSs2y/10)/delta);
  
  *slope=(Float_t) (-1/mya1);
  *error=(Float_t) ((LSsa1)/(mya1*mya1));
  
  return; 
}

inline Int_t HMdcSlope::execute(void)
{
  static HMdcRaw* raw=NULL;
  static Int_t s, mo, mb, t;
  static Float_t slope, error;

  iter->Reset();
  
  while ((raw=(HMdcRaw*)iter->Next()))
    {
      raw->getAddress(s, mo, mb, t);
      Float_t delta = 401.5/2; // delta of 401.5/2 should be retrieved from the database !!!!
      calc(raw->getTime(1)+2048,
	   raw->getTime(2)+2048,
	   raw->getTime(3)+((raw->getTime(3)<raw->getTime(4))?2048:0),
	   raw->getTime(4),
	   raw->getTime(5),
	   raw->getTime(6),
	   delta,
	   &slope,
	   &error);
      
      (*avgSlope)   [s][mo][mb][t] += slope;
      (*avgSlopeErr)[s][mo][mb][t] += error;
      (*nEvt)       [s][mo][mb][t] ++;
      
      if (slope)
	{	   
	  if(file) 
	    {
	      hSlope->Fill(slope);
	      for (Int_t i=1; i<7; i++)
		{
		  hTime->Fill(raw->getTime(i)
			      +((((i<3) ||(i==3 && raw->getTime(3)<raw->getTime(4))))?2048:0));
		}
	    }
	  (*calparraw)[s][mo][mb][t].setSlope   (slope);
	  (*calparraw)[s][mo][mb][t].setSlopeErr(error);
	  (*calparraw)[s][mo][mb][t].setSlopeMethod(1);

	}

      if(!slope)
	{
	  // in later versions the values should be set to a default or former value
	  (*calparraw)[s][mo][mb][t].setSlope(0);
	  (*calparraw)[s][mo][mb][t].setSlopeErr(0);
	  (*calparraw)[s][mo][mb][t].setSlopeMethod(0);
	}
      
      if(slope>1)
	{
	  (*calparraw)[s][mo][mb][t].setSlope(0);
	  (*calparraw)[s][mo][mb][t].setSlopeErr(0);
	  (*calparraw)[s][mo][mb][t].setSlopeMethod(3); // manually set
	}
    }
  return 0;
}
