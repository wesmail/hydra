#include "hshowercalibraterIPU.h"

#include "hruntimedb.h"
#include "hevent.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hshowerdetector.h"
#include "hcategory.h"
#include "hmatrixcatiter.h"
#include "hlocation.h"
#include "hshowerparset.h"
#include "hshowergeometry.h"
#include "hshowerraw.h"
#include "hshowercal.h"
#include "hshowercalpar.h"
#include "hdebug.h"
#include "hades.h"
#include "hiterator.h"
#include "showerdef.h"


ClassImp(HShowerCalibraterIPU)

///////////////////////////////////////////////////////////////////////
//HShowerCalibraterIPU
//
// HShowerCalibraterIPU class calculates new value of measured charge from RAW
// level using calibrations parameters from the parameter containers 
// HShowerCalPar. Calculated charge is saved to CAL level.
//
//////////////////////////////////////////////////////////////////////

HShowerCalibraterIPU::HShowerCalibraterIPU(Text_t *name,Text_t *title) :
  HReconstructor(name,title)
{
   m_pCalPar = NULL;
   fIter=NULL;
   m_loc.set(4,0,0,0,0);
   m_zeroLoc.set(0);
}

HShowerCalibraterIPU::HShowerCalibraterIPU()
{
  m_pCalPar = NULL;
  m_loc.set(4,0,0,0,0);
  m_zeroLoc.set(0);
  fIter=NULL;
}

void HShowerCalibraterIPU::initCalPar() {
//initializations of calibration parameters
    HShowerDetector *pShowerDet = (HShowerDetector*)gHades->getSetup()
                                                  ->getDetector("Shower");
    HRuntimeDb* rtdb=gHades->getRuntimeDb();

    m_pCalPar = rtdb->getContainer("ShowerCalPar");

          if (m_pCalPar) {
            ((HShowerCalPar*)m_pCalPar)->
                   setSetup(pShowerDet->getShowerSectors(),
                            pShowerDet->getShowerModules(),
                            pShowerDet->getRows(),
                            pShowerDet->getColumns());

             rtdb->addContainer(m_pCalPar);
          }
     

   
}

HShowerCalibraterIPU::~HShowerCalibraterIPU(void) {
//   if (m_pCalPar) delete m_pCalPar;
     if (fIter) delete fIter;
}

Bool_t HShowerCalibraterIPU::init() {
  // creates the ShowerRaw and ShowerCal categories 
  //  and adds them to the current event
  // creates an iterator which loops over all fired cells

    printf("initialization of shower calibrater\n");
    HShowerDetector *pShowerDet = (HShowerDetector*)gHades->getSetup()
                                                  ->getDetector("Shower");

    m_pRawCat=gHades->getCurrentEvent()->getCategory(catShowerRaw);
    if (!m_pRawCat) {
      m_pRawCat=pShowerDet->buildCategory(catShowerRaw);

      if (!m_pRawCat) return kFALSE;
      else gHades->getCurrentEvent()
                         ->addCategory(catShowerRaw, m_pRawCat, "Shower");
    }

    m_pCalCat=gHades->getCurrentEvent()->getCategory(catShowerCal);
    if (!m_pCalCat) {
      m_pCalCat=pShowerDet->buildCategory(catShowerCal);

      if (!m_pCalCat) return kFALSE;
      else gHades->getCurrentEvent()
                        ->addCategory(catShowerCal, m_pCalCat, "Shower");
    }

    initCalPar();
    fIter=(HIterator*)m_pRawCat->MakeIterator("native");

    return kTRUE;
}

Bool_t HShowerCalibraterIPU::reinit() {
/*
    HRuntimeDb* rtdb=gHades->getRuntimeDb();

    HShowerCalAsic *pCalAsic = (HShowerCalAsic*)rtdb->
                                               getContainer("ShowerCalAsic");

    HShowerLookupAsic *pLookupAsic = (HShowerLookupAsic*)rtdb->
                                               getContainer("ShowerLookupAsic");


    if (pCalAsic && pLookupAsic) {
       ((HShowerCalPar*)getCalPar())->setAsicTables(pLookupAsic, pCalAsic);
       ((HShowerCalPar*)getCalPar())->takeDataFromAsic();
    }
*/

    return kTRUE;
}


void HShowerCalibraterIPU::setCalPar(HParSet *pCalPar) {
    if (m_pCalPar) delete m_pCalPar;

    m_pCalPar = pCalPar;
}

Bool_t HShowerCalibraterIPU::finalize(void) {
   return kTRUE;
}

HShowerCalibraterIPU& HShowerCalibraterIPU::operator=(HShowerCalibraterIPU &c) {

  return c;
}

Int_t HShowerCalibraterIPU::execute()
{
 // calibrates all fired cells

  HShowerRaw *pRaw;

  fIter->Reset();
  while((pRaw = (HShowerRaw *)fIter->Next()))
  {
    //loop on all fired pads
    if (pRaw->getEventType()!=1) break;  //calibrates only event type 1

    m_loc[0]=pRaw->getSector();
    m_loc[1]=pRaw->getModule();
    m_loc[2]=pRaw->getRow();
    m_loc[3]= pRaw->getCol();

    calibrate(pRaw);  
  }

  return 0;
}

Bool_t HShowerCalibraterIPU::calibrate(HShowerRaw *raw) {
//calibrates the pad using slope and offset from parameter container 
#if DEBUG_LEVEL>2
  gDebuger->enterFunc("HShowerCalibraterIPU::execute");
  gDebuger->message("Raw cat points to %p",fRawCat);
#endif
  HShowerCal *cal=NULL;
  HShowerCalParCell *calparcell=NULL;

  //pointer for calibration parameters container for pad in m_loc position
  calparcell = (HShowerCalParCell*) ((HShowerCalPar*)getCalPar())
                                         ->getObject(m_loc);

  if (raw) {
//   Float_t fCharge;
    Int_t fCharge;   
//    fCharge = (raw->getCharge() + calparcell->getOffset()) 
//					* calparcell->getSlope() * calparcell->getGain();
      //
	  // fCharge calculation in the same way like in IPU
	  //
    fCharge = (((int)raw->getCharge()) +((int)calparcell->getOffset())); 
    if (fCharge > 1.0) {  //cutoff not interested data
       cal=(HShowerCal *)m_pCalCat->getNewSlot(m_zeroLoc);

       if (cal!=NULL) {
         cal=new(cal) HShowerCal;  //new element of ShowerCal category
         cal->setCharge(fCharge);
         cal->setSector(raw->getSector());
         cal->setModule(raw->getModule());
         cal->setRow(raw->getRow());
         cal->setCol(raw->getCol());
       }
    }
  }
#if DEBUG_LEVEL>2
  gDebuger->leaveFunc("HShowerCalibraterIPU::execute");
#endif
  return kTRUE;
}


