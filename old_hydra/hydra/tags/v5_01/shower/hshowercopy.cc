#include "hshowerdigitizer.h"
#include "hiterator.h"
#include "hruntimedb.h"
#include "hevent.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hshowerdetector.h"
#include "hcategory.h"
#include "hlinearcatiter.h"
#include "hlocation.h"
#include "hshowerdigidetpar.h"
#include "hshowergeometry.h"
#include "hdebug.h"
#include "hades.h"
#include "showerdef.h"
#include "hshowercalsim.h"
#include "hshowercopy.h"

ClassImp(HShowerCopy)

HShowerCopy::HShowerCopy(Text_t *name,Text_t *title) :
  HReconstructor(name,title)
{
  m_zeroLoc.set(0); 
  m_nEvents = 0;
  fIter = NULL;
}

HShowerCopy::HShowerCopy()
{
  m_zeroLoc.set(0); 
  m_nEvents = 0;
  fIter = NULL;
}


HShowerCopy::~HShowerCopy(void) {
  if (fIter) delete fIter;
}


Bool_t HShowerCopy::init() {
  printf("Initialization of HShowerCopy\n");
  HCategory *pCatMatr;
  HShowerDetector *pShowerDet = (HShowerDetector*)gHades->getSetup()
                                                  ->getDetector("Shower");
  pCatMatr=gHades->getCurrentEvent()->getCategory(catShowerRawMatr);
  
  if (!pCatMatr) {
     pCatMatr=pShowerDet->buildCategory(catShowerRawMatr);

     if (!pCatMatr) return kFALSE;
     else gHades->getCurrentEvent()
	        ->addCategory(catShowerRawMatr,pCatMatr,"Shower");
  }

  setInCat(pCatMatr);
  fIter = (HIterator*)pCatMatr->MakeIterator("native");

  pCatMatr=gHades->getCurrentEvent()->getCategory(catShowerCal);
  
  if (!pCatMatr) {
     pCatMatr=pShowerDet->buildLinearCat("HShowerCalSim");
     if (!pCatMatr) return kFALSE;
     else gHades->getCurrentEvent()->addCategory(catShowerCal,pCatMatr,
                                                 "Shower");
  }
  setOutCat(pCatMatr);

  return kTRUE;
}

/**************    COPY     **************/

Bool_t HShowerCopy::copy(HShowerRawMatrSim *pMatr) {

       HShowerCalSim *pCalSim = NULL;
       if (pMatr) {
	 Float_t fCharge;    

	 fCharge = pMatr->getCharge();

	 if (fCharge >= 1.0) {

	   pCalSim=(HShowerCalSim *)m_pOutCat->getNewSlot(m_zeroLoc);

	   if (pCalSim != NULL){
    
	     pCalSim=new(pCalSim) HShowerCalSim;
	     pCalSim->setEventNr( pMatr->getEventNr() ); 
	     pCalSim->setCharge( pMatr->getCharge() );
	     pCalSim->setSector( pMatr->getSector() );
	     pCalSim->setModule( pMatr->getModule() );
	     pCalSim->setRow( pMatr->getRow() );
	     pCalSim->setCol( pMatr->getCol() );
             pCalSim->setNTrack1( pMatr->getNTrack1() );
             pCalSim->setNTrack2( pMatr->getNTrack2() ); 
	   }
	 }
       }
return kTRUE;
       
}


Int_t HShowerCopy::execute()
{
  HShowerRawMatrSim *pMatr;

  Int_t n = 0;

  fIter->Reset();
  while((pMatr = (HShowerRawMatrSim*)fIter->Next()))
  {
    copy(pMatr);
    n++;
  }

  m_nEvents++;       

  return 0;
}




