using namespace std;
#include "hrichIPUmap.h"
#include "hdebug.h"
#include "hades.h"
#include "hcategory.h"
#include "hiterator.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hevent.h"
#include "hmatrixcatiter.h"
#include "triggerinfodef.h"
#include "htriggerdetector.h"
#include "hrichdetector.h"
#include "hlocation.h"

#include <iostream>          
#include <iomanip>


//----------------------------------------------------------------------------
HRichIPUMap::HRichIPUMap() { 

  pRich= NULL;
  iterRich=NULL;
}
//============================================================================

//----------------------------------------------------------------------------
HRichIPUMap::HRichIPUMap(Text_t *name,Text_t *title)
               : HReconstructor(name, title) {

  pRich= NULL;
  iterRich=NULL;
}
//============================================================================

//----------------------------------------------------------------------------
HRichIPUMap::~HRichIPUMap() { 
  // set everything to 0
  // especially the iterators

  if (pRich) delete pRich;
  pRich= NULL;
  
  if (iterRich) delete iterRich;
  iterRich=NULL;
}

//============================================================================

//----------------------------------------------------------------------------
Bool_t HRichIPUMap::init(){
  cout << "-------------- INIT of HRichIPUMap() ------------------- " << endl;
  HSpectrometer *spec = gHades->getSetup();
  HRuntimeDb *rtdb = gHades->getRuntimeDb();
  HTriggerDetector * pTrigDet = (HTriggerDetector*)gHades->getSetup()->getDetector("Trigger");
  HRichDetector * pRichDet = (HRichDetector*)gHades->getSetup()->getDetector("Rich");
 
  // allocate input categories
  pRich=gHades->getCurrentEvent()->getCategory(catHardRichHit);
  if (!pRich) {
    return kFALSE;
  }
  else gHades->getCurrentEvent()
	 ->addCategory(catHardRichHit, pRich, "Rich");  
  iterRich = (HIterator*)pRich->MakeIterator("native");

  if(rtdb){  
      HRichGeometryPar *pGeomPar = (HRichGeometryPar*)rtdb->getContainer("RichGeometryParameters");
      setGeomPar(pGeomPar);
      if (pGeomPar == NULL) { 
	  cout << " rich geometry parameters not found " << endl;
	  return kFALSE;
      }
  }
  
  return kTRUE;
}

//============================================================================


//----------------------------------------------------------------------------
Int_t HRichIPUMap::execute() { 
  HRichHitIPU   *pMURich=NULL;
  Int_t yRichHitIPU, xRichHitIPU, nSec;

  iterRich->Reset();
  while ((pMURich = (HRichHitIPU *)iterRich->Next()) != 0) {
      //cout << "BEFORE X " << pMURich->getX() << "  Y " << pMURich->getY() << endl; 
      nSec = pMURich->getSector();
      yRichHitIPU = pMURich->getY();
      xRichHitIPU = pMURich->getX();

      pMURich->setX(yRichHitIPU);
      pMURich->setY(xRichHitIPU);
      //cout << "AFTER  X " << pMURich->getX() << "  Y " << pMURich->getY() << endl; 

      // mapping according to RICH 
      HRichPad * pad = ((HRichGeometryPar*)fpGeomPar)->getPadsPar()->
	getPad(pMURich->getX(),pMURich->getY());
      
      pMURich->setPhi(pad->getPhi(nSec));
      pMURich->setTheta(pad->getTheta());
      //cout << "theta " << pMURich->getTheta() << "  phi " << pMURich->getPhi() << endl; 
  }
  return 0;
}
//============================================================================
  

//----------------------------------------------------------------------------
Bool_t HRichIPUMap::finalize(){
 return kTRUE;
}
//============================================================================

ClassImp(HRichIPUMap)





  





