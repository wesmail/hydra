#include "hshowercalibrater.h"

#include "hruntimedb.h"
#include "hevent.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hshowerdetector.h"
#include "hcategory.h"
#include "hmatrixcatiter.h"
#include "hlocation.h"
#include "hshowerraw.h"
#include "hshowercal.h"
#include "hshowercalpar.h"
#include "hdebug.h"
#include "hades.h"
#include "showerdef.h"


ClassImp(HShowerCalibrater)

HShowerCalibrater::HShowerCalibrater(Text_t *name,Text_t *title) :
  HReconstructor(name,title)
{
   m_pCalPar = NULL;
}

HShowerCalibrater::HShowerCalibrater()
{
   m_pCalPar = NULL;
}

void HShowerCalibrater::initCalPar() {
    HShowerDetector *pShowerDet = (HShowerDetector*)gHades->getSetup()
                                                  ->getDetector("Shower");
    HRuntimeDb* rtdb=gHades->getRuntimeDb();
    m_pCalPar = rtdb->getContainer("ShowerCalPar");

    if (!m_pCalPar) {
          m_pCalPar= new HShowerCalPar;
          m_pCalPar->SetName("ShowerCalPar");

          if (m_pCalPar) {
            ((HShowerCalPar*)m_pCalPar)->
                   setSetup(pShowerDet->getShowerSectors(),
                            pShowerDet->getShowerModules(),
                            pShowerDet->getRows(),
                            pShowerDet->getColumns());

             rtdb->addContainer(m_pCalPar);
          }
     
   }

}

HShowerCalibrater::~HShowerCalibrater(void) {
   if (m_pCalPar) delete m_pCalPar;
}

Bool_t HShowerCalibrater::init() {
    nn = 0;
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
    return kTRUE;
}

void HShowerCalibrater::setCalPar(HParSet *pCalPar) {
    if (m_pCalPar) delete m_pCalPar;

    m_pCalPar = pCalPar;
}

Bool_t HShowerCalibrater::finalize(void) {
    return kTRUE;
}

HShowerCalibrater& HShowerCalibrater::operator=(HShowerCalibrater &c) {

  return c;
}

Int_t HShowerCalibrater::execute()
{
  HIterator *iter;
  HShowerRaw *pRaw;
  HLocation loc;

//  printf("Event nr=%d\n", ++nn);
  iter = (HIterator*)getRawCat()->MakeIterator(); 
  Int_t n = 0;

  while((pRaw = (HShowerRaw *)iter->Next()))
  {
     loc = pRaw->getLocation();
     calibrate(loc);
     n++;
  }

  return 1;
}

Bool_t HShowerCalibrater::calibrate(HLocation& fLoc) {
#if DEBUG_LEVEL>2
  gDebuger->enterFunc("HShowerCalibrater::execute");
  gDebuger->message("Raw cat points to %p",fRawCat);
#endif
  HShowerRaw *raw=NULL;
  HShowerCal *cal=NULL;
  HShowerCalParCell *calparcell=NULL;

  raw=(HShowerRaw *)m_pRawCat->getObject(fLoc);

//  printf("loc: %d %d %d %d\n", fLoc[0], fLoc[1], fLoc[2], fLoc[3]);
  calparcell = (HShowerCalParCell*) ((HShowerCalPar*)getCalPar())
                                         ->getObject(fLoc);

  if (raw) {
    Float_t fCharge;

    cal=(HShowerCal *)m_pCalCat->getSlot(fLoc);
    if (cal!=NULL) {
      cal=new(cal) HShowerCal;

      fCharge = raw->getCharge() * calparcell->getSlope()
                                        + calparcell->getOffset();

      if (fCharge < 0.0) 
            fCharge = 0.0;

      cal->setCharge(fCharge);
      cal->setSector(raw->getSector());
      cal->setModule(raw->getModule());
      cal->setRow(raw->getRow());
      cal->setCol(raw->getCol());
      cal->setEventNr(raw->getEventNr());
   }
  }
#if DEBUG_LEVEL>2
  gDebuger->leaveFunc("HShowerCalibrater::execute");
#endif
  return kTRUE;
}

