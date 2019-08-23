//*-- AUTHOR Bjoern Spruck
//*-- created : 24.03.06
// Modified for Wall by M.Golubeva 01.11.2006

using namespace std;
#include "hwalldetector.h"
#include "hwallcalibrater.h"
#include "walldef.h"
#include "hwallraw.h"
#include "hwallcal.h"
#include "hwallcalpar.h"
#include "hdebug.h"
#include "hades.h"
#include "hiterator.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hevent.h"
#include "hcategory.h"
#include <iostream>
#include <iomanip>

//_HADES_CLASS_DESCRIPTION 
////////////////////////////////////////////////////////////////
//
//  HWallCalibrater: perform global calibration on the data
//
////////////////////////////////////////////////////////////////


HWallCalibrater::HWallCalibrater(void)
{
  rawCat=NULL;
  calCat=NULL;
  iter=NULL;
  loc.set(1,-1);
  pWallCalPar=NULL;
}

HWallCalibrater::HWallCalibrater(Text_t *name,Text_t *title) :
  HReconstructor(name,title)
{
  rawCat=NULL;
  calCat=NULL;
  iter=NULL;
  loc.set(1,-1);
  pWallCalPar=NULL;
}

HWallCalibrater::~HWallCalibrater(void)
{
  if (iter) delete iter;
  iter=NULL;
}

Bool_t HWallCalibrater::init(void)
{
  HWallDetector *wall;
  wall=(HWallDetector *)gHades->getSetup()->getDetector("Wall");
  
  if(!wall){
    Error("init","No Wall Det. found.");
    return kFALSE;
  }
  
  // Categories
  rawCat=gHades->getCurrentEvent()->getCategory(catWallRaw);  
  if (!rawCat) return kFALSE;
  
  calCat=wall->buildCategory(catWallCal);
  if (!calCat) return kFALSE;
  else
    gHades->getCurrentEvent()->addCategory(catWallCal,calCat,"Wall");
  
  // Parameters
  pWallCalPar=(HWallCalPar*)gHades->getRuntimeDb()->getContainer("WallCalPar");
  if (!pWallCalPar) return kFALSE;
  
  iter=(HIterator *)rawCat->MakeIterator();
  if(!iter) return kFALSE;
  loc.set(1,-1);
  
  fActive=kTRUE;
  
  return kTRUE;
}

Int_t HWallCalibrater::execute(void)
{
  HWallRaw *raw=0;
  HWallCal *cal=0;
  Int_t cell=-1;
  loc.set(1,-1);
  
  Float_t rawTime, rawAdc;
  Float_t adc, time;
  //Float_t t[4]={-1,-1,-1,-1},a[4]={-1,-1,-1,-1}; 
  
  //Fill cal category
  iter->Reset();
  while ((raw=(HWallRaw *)iter->Next())!=0) {
    Int_t m=0;// cell hit multiplicty    
 
    m=raw->getNHits();
    if(m<=0) continue;// No Hits -> forget it
    
    loc[0]=raw->getCell();
    if(loc[0] == -1) continue;
    
    if(m>raw->getMaxMult()) m=raw->getMaxMult();
    if(m<=0) continue;
    
    loc[0]=raw->getCell();
    cell = raw->getCell();
    HWallCalParCell &pPar=(*pWallCalPar)[cell];
    
    cal=(HWallCal*)calCat->getSlot(loc);
    if (cal) {
      cal=new (cal) HWallCal;
      if ( !cal ) return EXIT_FAILURE;
      cal->setCell(cell);
      
      for(Int_t i=0; i<m; i++){
	rawTime = raw->getTime(i+1);
	rawAdc = raw->getADC(i+1);
	if(rawAdc>0 && rawTime>0){
	  time = pPar.getTDC_Slope()*(rawTime-pPar.getTDC_Offset());// First add/substract, then multiply
	  adc = pPar.getADC_Slope()*(rawAdc+pPar.getADC_Offset());
	  
	  cal->setTimeAdc(time,adc);
	  //cal->getTimeAndAdc(i+1,*t,*a);

	}//if(rawAdc>=0.0 && rawTime>0)	  
      }//for(Int_t i=0; i<m; i++)
      
    }//if (cal)
    
  }//while ((raw=(HWallRaw *)iter->Next())!=0)
  
  //return 0;
  return EXIT_SUCCESS;
  
}

ClassImp(HWallCalibrater)
