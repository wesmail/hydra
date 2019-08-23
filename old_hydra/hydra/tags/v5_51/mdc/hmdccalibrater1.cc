//*-- AUTHOR : Ilse Koenig
//*-- Modified : 17/01/2002 by Ilse Koenig
//*-- Modified : 17/02/2000 by J. Kempter
//*-- Modified : 24/08/99 by R. Holzmann
//*-- Modified : 29/07/99 by Ilse Koenig

///////////////////////////////////////////////////////////////////////////////
//
// HMdcCalibrater1
//
// This transformation class calculates the drift time information in all fired
// cells using parameters from the parameter containers MdcCalParRaw for the
// calibration parameters and from MdcLookupGeom to map the layer number and
// cell number on Cal1 level to the motherboard number and TDC channel number
// on Raw level.
// The drift times are randomized within one bin of TDC resolution.
//
// The constructor gives different options how to fill HMdcCal1 level:
// HMdcCalibrater1(name,title,option input, otption timecut)
//    option input   =  1 No Start and Cal (default)
//                      2 Start and Cal
//                      3 No Start and no Cal
//    option timecut =  1 time cuts switched on
//                      0 time cuts switched off
//Start means that the startime from the Startdetector will be used to correct.
// Cal means real calibration with parameters from HMdcCalParRaw
// If no Cal (3)is selected no input from HMdcCalParRaw is needed
// and the TDC slope parameter will be always 1.The offset is set to 2048
// which means that the HMdcCal1 level will be a reversed copy of HMdcRaw.
///////////////////////////////////////////////////////////////////////////////

#include "hmdccalibrater1.h"
#include "hmdcdef.h"
#include "hdebug.h"
#include "hades.h"
#include "hiterator.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hevent.h"
#include "hcategory.h"
#include "hlocation.h"
#include "hmdcraw.h"
#include "hmdccal1.h"
#include "hmdccalparraw.h"
#include "hmdctimecut.h"
#include "hmdclookupgeom.h"
#include "hstarthit.h"

#include <iostream.h>
#include <stdlib.h>

ClassImp(HMdcCalibrater1)

HMdcCalibrater1::HMdcCalibrater1(void) {
  // Default constructor calls the function setParContainers().
  rawCat=0;
  calCat=0;
  startHitCat=0;
  iter=0;
  iterstart=0;
  calparraw=0;
  timecut=0;
  lookup=0;
  NoStartandCal=kTRUE;
  setTimeCut=kFALSE;
}

HMdcCalibrater1::HMdcCalibrater1(Text_t* name,Text_t* title,Int_t vers,Int_t cut)
                 :  HReconstructor(name,title) {
  // Constructor calls the constructor of class HReconstructor with the name
  // and the title as arguments. It also calls the function setParContainers().
  rawCat=0;
  calCat=0;
  startHitCat=0;
  iter=0;
  iterstart=0;
  calparraw=0;
  timecut=0;
  lookup=0;
  if(vers==1)NoStartandCal=kTRUE;
  if(vers==2)StartandCal=kTRUE;
  if(vers==3)NoStartandNoCal=kTRUE;
  if(cut==0) setTimeCut=kFALSE;
  if(cut==1) setTimeCut=kTRUE;

}

HMdcCalibrater1::~HMdcCalibrater1(void) {
  // destructor deletes the iterator
  if (iter) delete iter;
  iter=0;
  if (iterstart) delete iterstart;
  iterstart=0;
}


void HMdcCalibrater1::setParContainers() {
  // creates the parameter containers MdcCalParRaw and MdcLookupGeom if they do not
  // exist and adds them to the list of parameter containers in the rumtime
  // database
    lookup=(HMdcLookupGeom*)gHades->getRuntimeDb()->getContainer("MdcLookupGeom");
    if(setTimeCut) {
	timecut=(HMdcTimeCut*)gHades->getRuntimeDb()->getContainer("MdcTimeCut");
	timecut->setIsUsed(kTRUE);
    }

    if(StartandCal||NoStartandCal){
	calparraw=(HMdcCalParRaw*)gHades->getRuntimeDb()
	    ->getContainer("MdcCalParRaw");
    }


}


Bool_t HMdcCalibrater1::init(void) {
  // creates the MdcRaw  and MdcCal1 categories and adds them to the current
  // event
  // creates an iterator which loops over all fired cells
  // calls the function setParContainers()
  setParContainers();
  rawCat=gHades->getCurrentEvent()->getCategory(catMdcRaw);
  if (!rawCat) {
    rawCat=gHades->getSetup()->getDetector("Mdc")->buildCategory(catMdcRaw);
    if (!rawCat) return kFALSE;
    else gHades->getCurrentEvent()->addCategory(catMdcRaw,rawCat,"Mdc");
  }
  calCat=gHades->getCurrentEvent()->getCategory(catMdcCal1);
  if (!calCat) {
    calCat=gHades->getSetup()->getDetector("Mdc")->buildCategory(catMdcCal1);
    if (!calCat) return kFALSE;
    else gHades->getCurrentEvent()->addCategory(catMdcCal1,calCat,"Mdc");
  }

  if(StartandCal){
      startHitCat=gHades->getCurrentEvent()->getCategory(catStartHit);
      if (!startHitCat) {
	  startHitCat=gHades->getSetup()->getDetector("Start")->buildCategory(catStartHit);
	  if (!startHitCat) return kFALSE;
	  else {gHades->getCurrentEvent()->addCategory(catStartHit,startHitCat,"Start");
	      cout<<"Start detector and Triggerbox have to be added to your macro!!!!"<<endl;
	  }
      }
  }

  iter=(HIterator *)rawCat->MakeIterator("native");
  loc.set(4,0,0,0,0);

  if(StartandCal){
      iterstart=(HIterator *)startHitCat->MakeIterator("native");
  }

   cout<<"****************************HMdcCalibrater1**************************"<<endl;
   cout<<"*  options input 1 (default) no Start and calibration parameters    *"<<endl;
   cout<<"*                2           Start and calibration parameters       *"<<endl;
   cout<<"*                3           no Start and no calibration parameters *"<<endl;
   cout<<"*  time cut      0 (default) no time cuts used                      *"<<endl;
   cout<<"*                1           time cuts from HMdcTimeCut are used    *"<<endl;

   if(NoStartandCal)
  {cout<<"*  No Start detector time Information will be used                  *"<<endl;
   cout<<"*  Calibrater1 uses parameter from Input                            *"<<endl;
  }
  if(NoStartandNoCal)
  {cout<<"*  No Start detector time Information will be used                  *"<<endl;
   cout<<"*  Calibrater1 uses slope=1 and offset=2048                         *"<<endl;
  }
  if(StartandCal)
  {cout<<"*  Start detector time Information will be used                     *"<<endl;
   cout<<"*  Calibrater1 uses parameter from Input                            *"<<endl;
  }
  if(setTimeCut)
  {cout<<"*  time cuts are used                                               *"<<endl;
  }
  if(!setTimeCut)
  {cout<<"*  no time cuts are used                                            *"<<endl;
  }
   cout<<"*********************************************************************"<<endl;


  fActive=kTRUE;
  return kTRUE;
}

Float_t HMdcCalibrater1::getstarttime(){
  // Need some work for multiple hists in start detector
  // Better select multiplicity 1 in start.
  Int_t i=0;
  Int_t startmod=-1;
  HStartHit* starthit=0;
  iterstart->Reset();
  Float_t starttime=0;
  while ((starthit=(HStartHit *)iterstart->Next())!=0) {
      startmod=starthit->getModule();
      if(startmod==0)
      {
	  i++;
	  if(starthit->getFlag()) starttime=starthit->getTime();
      }
  }
  if (i!=1) Error("getstarttime(int)","Multiplicity in Start > 1 or 0");
  return starttime;
}

Int_t HMdcCalibrater1::execute(void) {

    // calibrates all fired cells
    // For time1 or time2 equal -999 in raw the times in cal1 are set to -999.
    // For time1 or time2 equal 0 in raw the times in cal1 are set to -998.
    // This differnt value is used to distinguish between not filled times and times equal 0.
    // The calibration parameters are taken from HMdcCalParRaw container.The
    // Calibration is only performed if Offset and Slope are not 0.
    // Only time1 and time2 are calibrated.
    Bool_t debug = kFALSE;
    if(StartandCal){
	HMdcRaw* raw=0;
	HMdcCal1* cal=0;
	Float_t testTime1=0;
        Float_t testTime2=0;
	Int_t mbo, ch;
	iter->Reset();
	Float_t starttime=getstarttime();
	while ((raw=(HMdcRaw *)iter->Next())!=0) {
	    raw->getAddress(loc[0],loc[1],mbo,ch);
	    if (loc[0]<0 || loc[1]<0) continue;
	    HMdcLookupChan& chan=(*lookup)[loc[0]][loc[1]][mbo][ch];
	    loc[2]=chan.getNLayer();
	    loc[3]=chan.getNCell();
	    if (loc[3]>=0) {
		const Int_t nHits=raw->getNHits();
		HMdcCalParTdc& tdc=(*calparraw)[loc[0]][loc[1]][mbo][ch];
		if (tdc.getSlope()!=0 && tdc.getOffset()!=0 /*&& abs(raw->getNHits())<=2*/ )
		{
		    testTime1=(tdc.getOffset() -
			       (((Float_t)(raw->getTime(1)) + gRandom->Rndm() - 0.5F)
				* tdc.getSlope())+starttime);

		    if (abs(nHits)>=2){
			testTime2=(tdc.getOffset() -
				   (((Float_t)(raw->getTime(2)) + gRandom->Rndm() - 0.5F)
				    * tdc.getSlope())+starttime);

		    }
                    if(setTimeCut)
		    {
                        // if time cuts are used

			if(timecut->cutTime1(loc[0],loc[1],testTime1)&&
			   timecut->cutTime2(loc[0],loc[1],testTime2)&&
			   timecut->cutTimesDif(loc[0],loc[1],testTime1,testTime2))
			{//
			    cal=(HMdcCal1 *)calCat->getSlot(loc);
			    cal=new(cal) HMdcCal1;

			    if(cal)
			    {

				cal->setNHits(nHits);
				cal->setAddress(loc[0],loc[1],loc[2],loc[3]);
				cal->setTime1(testTime1);
				cal->setTime2(testTime2);
			    }
			}//
		    }
		    else
		    {
                        // if no time cuts are used

			cal=(HMdcCal1 *)calCat->getSlot(loc);
			cal=new(cal) HMdcCal1;

			if(cal)
			{

			    cal->setNHits(nHits);
			    cal->setAddress(loc[0],loc[1],loc[2],loc[3]);
			    cal->setTime1(testTime1);
			    cal->setTime2(testTime2);
			}

		    }
		}
	    } else if (debug) {
		printf("lookup s=%i m=%i mbo=%2i chan=%2i not connected\n",
		       loc[0],loc[1],mbo,ch);
	    }
	}
    }
    if(NoStartandCal){
	HMdcRaw* raw=0;
	HMdcCal1* cal=0;
	Float_t testTime1=0;
        Float_t testTime2=0;
	Int_t mbo, ch;
	iter->Reset();
	while ((raw=(HMdcRaw *)iter->Next())!=0) {
	    raw->getAddress(loc[0],loc[1],mbo,ch);
	    if (loc[0]<0 || loc[1]<0) continue;
	    HMdcLookupChan& chan=(*lookup)[loc[0]][loc[1]][mbo][ch];
	    loc[2]=chan.getNLayer();
	    loc[3]=chan.getNCell();
	    if (loc[3]>=0) {
		const Int_t nHits=raw->getNHits();
		HMdcCalParTdc& tdc=(*calparraw)[loc[0]][loc[1]][mbo][ch];
		if (tdc.getSlope()!=0 && tdc.getOffset()!=0 /*&& abs(raw->getNHits())<=2*/ )
		{
		    testTime1=(tdc.getOffset() -
			       (((Float_t)(raw->getTime(1)) + gRandom->Rndm() - 0.5F)
				* tdc.getSlope()));

		    if (abs(nHits)>=2){
			testTime2=(tdc.getOffset() -
				   (((Float_t)(raw->getTime(2)) + gRandom->Rndm() - 0.5F)
				    * tdc.getSlope()));

		    }
                    if(setTimeCut)
		    {
                        // if time cuts are used

			if(timecut->cutTime1(loc[0],loc[1],testTime1)&&
			   timecut->cutTime2(loc[0],loc[1],testTime2)&&
			   timecut->cutTimesDif(loc[0],loc[1],testTime1,testTime2))
			{
			    cal=(HMdcCal1 *)calCat->getSlot(loc);
			    cal=new(cal) HMdcCal1;

			    if(cal)
			    {

				cal->setNHits(nHits);
				cal->setAddress(loc[0],loc[1],loc[2],loc[3]);
				cal->setTime1(testTime1);
				cal->setTime2(testTime2);
			    }
			}
		    }
		    else
		    {
			// if no time cuts are used

			cal=(HMdcCal1 *)calCat->getSlot(loc);
			cal=new(cal) HMdcCal1;

			if(cal)
			{

			    cal->setNHits(nHits);
			    cal->setAddress(loc[0],loc[1],loc[2],loc[3]);
			    cal->setTime1(testTime1);
			    cal->setTime2(testTime2);
			}
		    }
		}
	    } else if (debug) {
		printf("lookup s=%i m=%i mbo=%2i chan=%2i not connected\n",
		       loc[0],loc[1],mbo,ch);
	    }
	}
    }

    if(NoStartandNoCal){
	HMdcRaw* raw=0;
	HMdcCal1* cal=0;
	Float_t testTime1=0;
        Float_t testTime2=0;

	Int_t mbo, ch;
	iter->Reset();
	while ((raw=(HMdcRaw *)iter->Next())!=0) {
	    raw->getAddress(loc[0],loc[1],mbo,ch);
	    if (loc[0]<0 || loc[1]<0) continue;
	    HMdcLookupChan& chan=(*lookup)[loc[0]][loc[1]][mbo][ch];
	    loc[2]=chan.getNLayer();
	    loc[3]=chan.getNCell();
	    if (loc[3]>=0)
	    {
		const Int_t nHits=raw->getNHits();

		if(setTimeCut)
		{
		    // if time cuts are used

		    testTime1=2048 - raw->getTime(1);

		    if (abs(nHits)>=2){
			testTime2=2048 - raw->getTime(2);
		    }


		    if(timecut->cutTime1(loc[0],loc[1],testTime1)&&
		       timecut->cutTime2(loc[0],loc[1],testTime2)&&
		       timecut->cutTimesDif(loc[0],loc[1],testTime1,testTime2))
		    {
			cal=(HMdcCal1 *)calCat->getSlot(loc);
			cal=new(cal) HMdcCal1;

			if(cal)
			{

			    cal->setNHits(nHits);
			    cal->setAddress(loc[0],loc[1],loc[2],loc[3]);
			    cal->setTime1(testTime1);
			    cal->setTime2(testTime2);
			}
		    }
		}
		else
		{
		    // if no time cuts are used

		    cal=(HMdcCal1 *)calCat->getSlot(loc);
		    if (cal) {
			cal=new(cal) HMdcCal1;
			const Int_t nHits=raw->getNHits();
			cal->setNHits(nHits);
			cal->setAddress(loc[0],loc[1],loc[2],loc[3]);
			switch (raw->getTime(1)){

			case 0:    cal->setTime1(-998.);
			break;
			case -999: cal->setTime1(-999.);
			break;
			default :
			    cal->setTime1(2048 -
					  ((Float_t)(raw->getTime(1)) + gRandom->Rndm() - 0.5F));
			    break;
			}
			if (abs(nHits)>=2){
			    switch(raw->getTime(2)){
			    case 0:    cal->setTime2(-998.);
			    break;
			    case -999: cal->setTime2(-999.);
			    break;
			    default:   cal->setTime2(2048 -
						     ((Float_t)(raw->getTime(2)) + gRandom->Rndm() - 0.5F));
			    break;
			    }
			}
		    }
		}
	    } else  if (debug) {
		printf("lookup s=%i m=%i mbo=%2i chan=%2i not connected\n",
		       loc[0],loc[1],mbo,ch);
	    }
	}
    }


    return 0;
}
