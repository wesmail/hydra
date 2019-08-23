#include "hshowercalparcalculator.h"

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
#include "hshowercalpar.h"
#include "hshowerhist.h"
#include "hshowercalasic.h"
#include "hshowerlookupasic.h"
#include "hdebug.h"
#include "hades.h"
#include "hiterator.h"
#include "showerdef.h"


ClassImp(HShowerCalParCalculator)

HShowerCalParCalculator::HShowerCalParCalculator(Text_t *name,Text_t *title, 
          Int_t nPeaksNumber) : HReconstructor(name,title)
{
   m_pCalPar = NULL;
   m_pCalParHist = NULL;
   fIter=NULL;
   m_loc.set(4,0,0,0,0);
   m_zeroLoc.set(0);
   m_nPeaksNumber = nPeaksNumber;
}

HShowerCalParCalculator::HShowerCalParCalculator(Int_t nPeaksNumber)
{
  m_pCalPar = NULL;
  m_pCalParHist = NULL;
  fIter=NULL;
   m_loc.set(4,0,0,0,0);
   m_zeroLoc.set(0);
   m_nPeaksNumber = nPeaksNumber;
}

void HShowerCalParCalculator::initCalPar() {
    HShowerDetector *pShowerDet = (HShowerDetector*)gHades->getSetup()
                                                  ->getDetector("Shower");
    HRuntimeDb* rtdb=gHades->getRuntimeDb();

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

    m_pCalParHist = rtdb->getContainer("ShowerHist");
    if (!m_pCalParHist) {
          m_pCalParHist= new HShowerHist;

          if (m_pCalParHist) {
            ((HShowerHist*)m_pCalParHist)->
                   setSetup(pShowerDet->getShowerSectors(),
                            pShowerDet->getShowerModules(),
                            pShowerDet->getRows(),
                            pShowerDet->getColumns());

             rtdb->addContainer(m_pCalParHist);
          }
   }

}

HShowerCalParCalculator::~HShowerCalParCalculator(void) {
     if (fIter) delete fIter;
}

Bool_t HShowerCalParCalculator::init() {
    printf("initialization of shower calpar calculator\n");
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

    initCalPar();
    fIter=(HIterator*)m_pRawCat->MakeIterator();

    return kTRUE;
}

Bool_t HShowerCalParCalculator::reinit() {
    HRuntimeDb* rtdb=gHades->getRuntimeDb();

    HShowerCalAsic *pCalAsic = (HShowerCalAsic*)rtdb->
                                               getContainer("ShowerCalAsic");

    HShowerLookupAsic *pLookupAsic = (HShowerLookupAsic*)rtdb->
                                               getContainer("ShowerLookupAsic");


    if (pCalAsic && pLookupAsic) {
       ((HShowerCalPar*)getCalPar())->setAsicTables(pLookupAsic, pCalAsic);
    }
    return kTRUE;
}

void HShowerCalParCalculator::setCalPar(HParSet *pCalPar) {
    if (m_pCalPar) delete m_pCalPar;

    m_pCalPar = pCalPar;
}

void HShowerCalParCalculator::setCalParHist(HParSet *pCalParHist) {
    if (m_pCalParHist) delete m_pCalParHist;

    m_pCalParHist = pCalParHist;
}

Bool_t HShowerCalParCalculator::finalize(void) {
   ((HShowerHist*)getCalParHist())
                ->calculate((HShowerCalPar*)getCalPar(), m_nPeaksNumber);
   ((HShowerCalPar*)getCalPar())->propagateDataToAsic();
   
   return kTRUE;
}

Int_t HShowerCalParCalculator::execute()
{
  HShowerRaw *pRaw;

  Int_t n = 0;
 
  fIter->Reset();
  while((pRaw = (HShowerRaw*)fIter->Next()))
  {
    m_loc[0]=pRaw->getSector();
    m_loc[1]=pRaw->getModule();
    m_loc[2]=pRaw->getRow();
    m_loc[3]=pRaw->getCol();

    ((HShowerHist*)getCalParHist())->fill(m_loc, pRaw->getCharge());
    n++;
  }

  nn++;
  return 0;
}

