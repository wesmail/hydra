#include "hshowercalibrater.h"

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
#include "hshowercalasic.h"
#include "hshowerlookupasic.h"
#include "hdebug.h"
#include "hades.h"
#include "hiterator.h"
#include "showerdef.h"


ClassImp(HShowerCalibrater)

HShowerCalibrater::HShowerCalibrater(Text_t *name,Text_t *title) :
  HReconstructor(name,title)
{
   m_pCalPar = NULL;
   fIter=NULL;
   m_loc.set(4,0,0,0,0);
   m_zeroLoc.set(0);
}

HShowerCalibrater::HShowerCalibrater()
{
  m_pCalPar = NULL;
  m_loc.set(4,0,0,0,0);
  m_zeroLoc.set(0);
  fIter=NULL;
}

void HShowerCalibrater::initCalPar() {
    HShowerDetector *pShowerDet = (HShowerDetector*)gHades->getSetup()
                                                  ->getDetector("Shower");
    HRuntimeDb* rtdb=gHades->getRuntimeDb();

    HShowerGeometry *pGeom = (HShowerGeometry*)rtdb->
                                               getContainer("ShowerGeometry");
    if (!pGeom) {
       pGeom = new HShowerGeometry;
       rtdb->addContainer(pGeom);
    }
    setGeometry(pGeom);
    if (!pGeom) return;

    m_pCalPar = rtdb->getContainer("ShowerCalPar");
    if (!m_pCalPar) {
          m_pCalPar= new HShowerCalPar;

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
//   if (m_pCalPar) delete m_pCalPar;
     if (fIter) delete fIter;
}

Bool_t HShowerCalibrater::init() {
    printf("initialization of shower calibrater\n");
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
    fIter=(HIterator*)m_pRawCat->MakeIterator("native");

    return kTRUE;
}

Bool_t HShowerCalibrater::reinit() {
    HRuntimeDb* rtdb=gHades->getRuntimeDb();

    HShowerCalAsic *pCalAsic = (HShowerCalAsic*)rtdb->
                                               getContainer("ShowerCalAsic");

    HShowerLookupAsic *pLookupAsic = (HShowerLookupAsic*)rtdb->
                                               getContainer("ShowerLookupAsic");


    if (pCalAsic && pLookupAsic) {
       ((HShowerCalPar*)getCalPar())->setAsicTables(pLookupAsic, pCalAsic);
       ((HShowerCalPar*)getCalPar())->takeDataFromAsic();
    }
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
  HShowerRaw *pRaw;

  Int_t n = 0;
 
  fIter->Reset();
  while((pRaw = (HShowerRaw *)fIter->Next()))
  {
    if (pRaw->getEventType()!=1) break;

    m_loc[0]=pRaw->getSector();
    m_loc[1]=pRaw->getModule();
    m_loc[2]=pRaw->getRow();
    m_loc[3]= pRaw->getCol();
//    m_loc[3]= (m_loc[1] ) ?  pRaw->getCol() : 31 -  pRaw->getCol();

    calibrate(pRaw);
    //printf("hit = %d loc=%d:%d:%d:%d\n", n, loc[0], loc[1], loc[2], loc[3]);
    n++;
  }

  nn++;
  return 0;
}

Bool_t HShowerCalibrater::calibrate(HShowerRaw *raw) {
#if DEBUG_LEVEL>2
  gDebuger->enterFunc("HShowerCalibrater::execute");
  gDebuger->message("Raw cat points to %p",fRawCat);
#endif
  HShowerCal *cal=NULL;
  HShowerCalParCell *calparcell=NULL;
  Int_t nMasterOffset = 2;

//  printf("loc: %d %d %d %d\n", m_oc[0], m_oc[1], m_loc[2], m_loc[3]);
  calparcell = (HShowerCalParCell*) ((HShowerCalPar*)getCalPar())
                                         ->getObject(m_loc);

//  printf("row = %d, col = %d, offset = %f\n", m_loc[2], m_loc[3],  
//                                           calparcell->getOffset());
//  float offset;
//  offset = calparcell->getOffset();
  if (raw) {
    Float_t fCharge;
//    fCharge = (raw->getCharge() + calparcell->getOffset()) * calparcell->getSlope();
    fCharge = (raw->getCharge() + calparcell->getOffset() - nMasterOffset) * calparcell->getSlope();
    
    if (fCharge >= 1.0) {
       cal=(HShowerCal *)m_pCalCat->getNewSlot(m_zeroLoc);

       if (cal!=NULL) {
         cal=new(cal) HShowerCal;
         cal->setCharge(fCharge);
         cal->setSector(raw->getSector());
         cal->setModule(raw->getModule());
         cal->setRow(raw->getRow());
         cal->setCol(raw->getCol());
         cal->setEventNr(raw->getEventNr());
       }

    } 
    
  }
#if DEBUG_LEVEL>2
  gDebuger->leaveFunc("HShowerCalibrater::execute");
#endif
  return kTRUE;
}


