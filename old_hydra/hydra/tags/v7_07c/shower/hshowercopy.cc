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
#include "hshowercal.h"
#include "hshowercopy.h"

ClassImp(HShowerCopy)

HShowerCopy::HShowerCopy(Text_t *name,Text_t *title, Float_t masterOffset) :
  HReconstructor(name,title)
{
  m_zeroLoc.set(0); 
  m_nEvents = 0;
  fIter = NULL;
  mOffset = masterOffset;
}

HShowerCopy::HShowerCopy()
{
  m_zeroLoc.set(0); 
  m_nEvents = 0;
  fIter = NULL;
  mOffset = 0;
}


HShowerCopy::~HShowerCopy(void) {
  if (fIter) delete fIter;
}


Bool_t HShowerCopy::init() {
  printf("initialization shower copy reconstructor\n");
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
    pCatMatr=pShowerDet->buildCategory(catShowerCal);

    if (!pCatMatr) return kFALSE;
    else gHades->getCurrentEvent()->addCategory(catShowerCal,pCatMatr,"Shower");
  }
  setOutCat(pCatMatr);

  return kTRUE;
}

/**************    COPY     **************/

Bool_t HShowerCopy::copy(HShowerRawMatr *pMatr) {


       HShowerCal *pCal = NULL;
       if (pMatr) {
	 Float_t fCharge;
    
	 fCharge = pMatr->getCharge();

	 if (fCharge > mOffset) {

//         cout << " mOffset  " <<  mOffset << endl;

	   pCal=(HShowerCal *)m_pOutCat->getNewSlot(m_zeroLoc);

	   if (pCal != NULL){
    
	     pCal=new(pCal) HShowerCal;
	     pCal->setCharge( pMatr->getCharge() - mOffset);
	     pCal->setSector( pMatr->getSector() );
	     pCal->setModule( pMatr->getModule() );
	     pCal->setRow( pMatr->getRow() );
	     pCal->setCol( pMatr->getCol() ); 
	   }
	 }
       }
return kTRUE;
       
}



Int_t HShowerCopy::execute()
{
  HShowerRawMatr *pMatr;

  Int_t n = 0;

  fIter->Reset();
  while((pMatr = (HShowerRawMatr*)fIter->Next()))
  {
    copy(pMatr);
    n++;
  }

  m_nEvents++;       

  return 0;
}


