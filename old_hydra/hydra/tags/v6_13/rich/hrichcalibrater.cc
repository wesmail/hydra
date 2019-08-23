
#include "hrichcalibrater.h"
#include "hrichutilfunc.h"
#include "hruntimedb.h"
#include "hevent.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hrichdetector.h"
#include "hcategory.h"
#include "hiterator.h"
#include "hmatrixcatiter.h"
#include "hlocation.h"
#include "hrichraw.h"
#include "hrichcalpar.h"
#include "hrichmappingpar.h"
#include "hrichcal.h"
#include "hdebug.h"
#include "hades.h"
#include "richdef.h"


ClassImp(HRichCalibrater)

HRichCalibrater::HRichCalibrater(Text_t *name,Text_t *title, Bool_t swap_enabled) :
  HReconstructor(name,title)
{
  perform_cable_swapping=swap_enabled;
  fCalPar = NULL;
  fIter = NULL;
}

HRichCalibrater::HRichCalibrater()
{
  perform_cable_swapping=kFALSE;
  fCalPar = NULL;
  fIter = NULL;
}


HRichCalibrater::~HRichCalibrater(void) {
   if (fIter) delete fIter;
}

void HRichCalibrater::initCalPar() {

    HRichDetector *pRichDet = (HRichDetector*)gHades->getSetup()
                                                  ->getDetector("Rich");
    HRuntimeDb* rtdb=gHades->getRuntimeDb();
    fCalPar = rtdb->getContainer("RichCalPar");
    if (fCalPar) {
      ((HRichCalPar*)fCalPar)->
             setSetup(pRichDet->getSectors(),
                      pRichDet->getRows(),
                      pRichDet->getColumns());
    }
    fMapPar = (HRichMappingPar*)rtdb->getContainer("RichMappingParameters");
    
}


Bool_t HRichCalibrater::init() {
    printf("initialization of rich calibrater\n");

    HRichDetector *pRichDet = (HRichDetector*)gHades->getSetup()
                                                  ->getDetector("Rich");

    fRawCat=gHades->getCurrentEvent()->getCategory(catRichRaw);
    if (!fRawCat) {
      fRawCat=pRichDet->buildCategory(catRichRaw);

      if (!fRawCat) return kFALSE;
      else gHades->getCurrentEvent()
                         ->addCategory(catRichRaw, fRawCat, "Rich");
    }

    fCalCat=gHades->getCurrentEvent()->getCategory(catRichCal);
    if (!fCalCat) {
      fCalCat=pRichDet->buildCategory(catRichCal);

      if (!fCalCat) return kFALSE;
      else gHades->getCurrentEvent()
                        ->addCategory(catRichCal, fCalCat, "Rich");
    }

    initCalPar();
    fIter = (HMatrixCatIter*)getRawCat()->MakeIterator(); 
    
    return kTRUE;
}

Bool_t HRichCalibrater::finalize() {
   ((HRichCalPar*)fCalPar)->PrintCalPar();
    return kTRUE;
}


HRichCalibrater& HRichCalibrater::operator=(HRichCalibrater &c) {

  return c;
}

Int_t HRichCalibrater::execute()
{
  HRichRaw* pRaw;
  HLocation loc;
  
  Int_t n = 0;

  fIter->Reset(); 
  while((pRaw = (HRichRaw *)fIter->Next()))
  {
     loc = fIter->getLocation();
     calibrate(loc);
     n++;
  }
//  printf("nn = %d\n", nn++);
  return 0;
}

Bool_t HRichCalibrater::calibrate(HLocation& fLoc) {
#if DEBUG_LEVEL>2
  gDebuger->enterFunc("HRichCalibrater::execute");
  gDebuger->message("Raw cat points to %p",fRawCat);
#endif
  HRichRaw *raw = NULL;
  HRichCal *cal = NULL;
  HRichCalParCell *calparcell = NULL;

  raw = (HRichRaw *)fRawCat->getObject(fLoc);

  Int_t upi;
  HLocation fLocSwap = fLoc;

  //We perform swapping if data come from the critical portion of sector 3 and if the user desires to do so
  if(perform_cable_swapping && raw->getSector()==3)
    {
      switch(fMapPar->getPort(raw->getCol(), raw->getRow())){
      case 4:
	if(fMapPar->getRC(raw->getCol(),raw->getRow())==0)
	  {
	    upi=fMapPar->getRC(raw->getCol(),raw->getRow())*10000+
	      6000+
	      fMapPar->getMod(raw->getCol(),raw->getRow())*100+
	      fMapPar->getCh(raw->getCol(),raw->getRow());
	  }
	break;
      case 6:
	if(fMapPar->getRC(raw->getCol(),raw->getRow())==0)
	  {
	    upi=
	      fMapPar->getRC(raw->getCol(),raw->getRow())*10000+
	      4000+
	      fMapPar->getMod(raw->getCol(),raw->getRow())*100+
	      fMapPar->getCh(raw->getCol(),raw->getRow());
	  }
	break;
      default:
	upi=
	  fMapPar->getRC(raw->getCol(),raw->getRow())*10000+
	  fMapPar->getPort(raw->getCol(),raw->getRow())*1000+
	  fMapPar->getMod(raw->getCol(),raw->getRow())*100+
	  fMapPar->getCh(raw->getCol(),raw->getRow());
	break;
      }
      fLocSwap.setOffset(fMapPar->getCol(upi));
      fLocSwap.setIndex(1,fMapPar->getRow(upi));
      calparcell = (HRichCalParCell*) ((HRichCalPar*)getCalPar())
	->getObject(fLocSwap);
    }
  else {
    //No swaping neccessary - take the unmodified copy of the location
    calparcell = (HRichCalParCell*) ((HRichCalPar*)getCalPar())
      ->getObject(fLocSwap);
  }
	     

  //calparcell = (HRichCalParCell*) ((HRichCalPar*)getCalPar())
  //->getObject(fLoc);

  if (raw) {
    Float_t fCharge;
    fCharge = raw->getCharge() * calparcell->getSlope()
                                        - calparcell->getOffset();
    
    if (fCharge > 0.0) { 
       cal = (HRichCal *)fCalCat->getSlot(fLoc);
       if (cal != NULL) {
         cal = new(cal) HRichCal;
   
         cal->setCharge(fCharge);
         cal->setSector(raw->getSector());
         cal->setRow(raw->getRow());
         cal->setCol(raw->getCol());
         cal->setEventNr(raw->getEventNr());
       }
    }
  }
#if DEBUG_LEVEL>2
  gDebuger->leaveFunc("HRichCalibrater::execute");
#endif
  return kTRUE;
}

