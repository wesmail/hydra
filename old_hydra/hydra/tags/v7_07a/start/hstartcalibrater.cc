//*-- Modified : 18/12/2001 by I.Koenig
//*-- Modified : 01/12/2000 by I.Koenig
using namespace std;
#include "hstartcalibrater.h"
#include "hstartdef.h"
#include "hstartraw.h"
#include "hstartcal.h"
#include "hstartcalpar.h"
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

////////////////////////////////////////////////////////////////
//
//  HStartCalibrater: perform global correction on the data
//  
////////////////////////////////////////////////////////////////


HStartCalibrater::HStartCalibrater(void) {
  rawCat=0;
  calCat=0;
  iter=0;
  calpar=0;
}

HStartCalibrater::HStartCalibrater(Text_t *name,Text_t *title) : 
               HReconstructor(name,title) {
  rawCat=0;
  calCat=0;
  iter=0;
  calpar=0;
}

HStartCalibrater::~HStartCalibrater(void) {
  if (iter) delete iter;
  iter=0;
}

Bool_t HStartCalibrater::init(void) {
  calpar=(HStartCalPar*)gHades->getRuntimeDb()->getContainer("StartCalPar");
  if (!calpar) return kFALSE;
  rawCat=gHades->getCurrentEvent()->getCategory(catStartRaw);
  if (!rawCat) {
    rawCat=gHades->getSetup()->getDetector("Start")->buildCategory(catStartRaw);
    if (!rawCat) return kFALSE;
    else gHades->getCurrentEvent()->addCategory(catStartRaw,rawCat,"Start");
  }
  calCat=gHades->getCurrentEvent()->getCategory(catStartCal);
  if (!calCat) {
    calCat=gHades->getSetup()->getDetector("Start")->buildCategory(catStartCal);
    if (!calCat) return kFALSE;
    else gHades->getCurrentEvent()->addCategory(catStartCal,calCat,"Start");
  }
  iter=(HIterator *)rawCat->MakeIterator();
  loc.set(2,0,0);
  fActive=kTRUE;

  return kTRUE;
}

Int_t HStartCalibrater::execute(void) {
	HStartRaw *raw=0;
	HStartCal *cal=0;
	Int_t nType=-1;
	Int_t mod=0, strip=0;
	Int_t nMult[6];
	Float_t val=0.F;
	Float_t timeR=-1,timeL=-1,timeM=-1;
	Float_t rawTime,rawTimeR,rawTimeL,rawTimeM;
	iter->Reset();
	for(Int_t i=0;i<6;i++){
		nMult[i]=0;
	}
	//Fill multiplicity
	while ((raw=(HStartRaw *)iter->Next())!=0) {
		nType = raw->getType();
		raw->getAddress(mod,strip);
		if(nType==TYPE_LEFT || nType==TYPE_RIGHT){
			//Only if both sides fired	
			if(raw->getTimeR() > 0.0 && raw->getTimeL() > 0.0 ){
				nMult[mod]++;
			}	
		}
		else if(nType==TYPE_MEANTIMER){
			if(raw->getTimeM()>0.0)
				nMult[mod]++;
		}
		else{
			//Old diamond detector
			if(raw->getTime()>0.0)
				nMult[mod]++;
		}
	}	
	//Fill cal category
	iter->Reset();
	while ((raw=(HStartRaw *)iter->Next())!=0) {
		raw->getAddress(mod,strip);
		loc[0] = mod; loc[1] = strip;
	    cal=(HStartCal*)calCat->getSlot(loc);
    	if (cal) {
      		cal=new (cal) HStartCal;
      		if ( !cal ) return -1;
			rawTime  = raw->getTime();
			rawTimeR = raw->getTimeR();
			rawTimeL = raw->getTimeL();
			rawTimeM = raw->getTimeM();
			nType = raw->getType();
      		HStartCalParChan &pPar=(*calpar)[mod][strip];

			if( nType==TYPE_LEFT || nType==TYPE_RIGHT ){
				if(rawTimeR>0.0){
					timeR = pPar.getSlopeR()*rawTimeR;
					cal->fill(timeR,mod,strip,TYPE_RIGHT);
				}
				if(rawTimeL>0.0){
					timeL = pPar.getSlopeL()*rawTimeL;
					cal->fill(timeL,mod,strip,TYPE_LEFT);
      			}
				cal->setType(nType);
				//Only if both sides fired
				if( rawTimeR>0.0 && rawTimeL>0.0 ){
					cal->setTof((timeR+timeL)/2. + pPar.getOffset());
					cal->setPos((timeR-timeL)/2. - pPar.getPosOffset());
				}
			}
			else if( nType==TYPE_MEANTIMER ){
				if(rawTimeM>0.){
					timeM = pPar.getSlopeM();
					cal->fill(timeM,mod,strip,nType);
					cal->setTof(timeM + pPar.getOffset());
				}
			}
			else {
				if(rawTime>0.){
					val=pPar.getSlope()*rawTime+pPar.getOffset();
					cal->fill(val,mod,strip);
					cal->setTof(val + pPar.getOffset());
				}
			}//fill multiplicity information
			for(Int_t i=0;i<6;i++)	
				cal->setMult(i,nMult[i]);
		}
	}
 
	return 0;
}

ClassImp(HStartCalibrater)















