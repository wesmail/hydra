#include <stdlib.h>
#include "hrichcerdigitizer.h"
#include "hevent.h"
#include "hcategory.h"
#include "hlinearcategory.h"
#include "hlocation.h"
#include "hrichgeantcherhit.h"
#include "hrichgeantwire.h"
#include "hrichdigidetpar.h"
#include "hdebug.h"
#include "hades.h"
#include "richdef.h"

#include "hrichpad.h"
#include "hrichwire.h"
#include "hrichframe.h"

ClassImp(HRichCerDigitizer)

HRichCerDigitizer::HRichCerDigitizer(Text_t *name,Text_t *title) :
  HRichDigitizer(name,title)
{
  init();
}

HRichCerDigitizer::HRichCerDigitizer()
{
  init();
}


HRichCerDigitizer::~HRichCerDigitizer(void) {
}

Bool_t HRichCerDigitizer::init() {
    setInCat(gHades->getCurrentEvent()->getCategory(catRichGeantCherHit));
    setOutCat(gHades->getCurrentEvent()->getCategory(catRichGeantWire));
    return HRichDigitizer::init();
}

HRichCerDigitizer& HRichCerDigitizer::operator=(HRichCerDigitizer &c) {
  return c;
}

Bool_t HRichCerDigitizer::digitize(HDataObject *pHit) {
  HLocation fLoc;
  Bool_t nResult = kFALSE;

#if DEBUG_LEVEL>2
  gDebuger->enterFunc("HRichCerDigitizer::execute");
  gDebuger->message("Raw cat points to %p",fRawCat);
#endif

  HRichGeantCherHit *pCherHit = (HRichGeantCherHit*) pHit;
  HRichDigiDetPar *pDetPar = (HRichDigiDetPar*) getDigiParSet();

  if (!pDetPar)
  {
    Error("HRichCerDigitizer::digitize", "Digitazation parametes not initialised !!");
    return 0;
  }

  if (pCherHit) {
//////////////////////////////////////////////////////////

    nResult = digiCherenkov(pCherHit);
     
/////////////////////////////////////////////////////////
  }

#if DEBUG_LEVEL>2
  gDebuger->leaveFunc("HRichCerDigitizer::execute");
#endif

  return nResult;
}

Bool_t HRichCerDigitizer::digiCherenkov(HRichGeantCherHit *simhit) {
 int hit=0;
 HLocation fLoc;
 HRichGeantWire *pGeantWire=NULL;
 HRichWireTab *phWires = ((HRichDigiDetPar*)getDigiParSet())->getWireTab();
 HRichFrame *phFrame = ((HRichDigiDetPar*)getDigiParSet())->getFrame(0);

 Float_t fX, fY;
 
 simhit->getXY(&fX, &fY); 
// statflag==2 means in this simulation detector
  if (photonQE(simhit->getEner()) && simhit->getStatFlag() == 2 && 
         !phFrame->isOut(fX, fY)) {

      hit=0;
      if (phWires->calcHit(fX, fY)) {
         hit=1;
         pGeantWire = (HRichGeantWire*)((HLinearCategory*)getOutCat())
                                                        ->getNewSlot(fLoc);
         if (pGeantWire != NULL) {
            pGeantWire = new(pGeantWire) HRichGeantWire;
            pGeantWire->setSrcQ(qCher);
            phWires->fillGeantWireHit(pGeantWire);
         }
      }
  }

  return hit;
}

int HRichCerDigitizer::photonQE(const float len) {

int a;
float fate;

float* photlen = ((HRichDigiDetPar*)getDigiParSet())->photlen;
float* photeff = ((HRichDigiDetPar*)getDigiParSet())->photeff;

fate = rand()/(double)RAND_MAX;

for (a=0;a<18;a++)
 if (a==0) {                                   
  if (len<((photlen[a]+photlen[a+1])/2))   
   if (fate>photeff[a]) {
    return 0;
   } else {
    return 1;
   }
 } else if (a==17) {
  if (len>=((photlen[a-1]+photlen[a])/2))
   if (fate>photeff[a]) {
    return 0;
   } else {
    return 1;
   }
 } else if (((photlen[a-1]+photlen[a])/2)<=len &&
            ((photlen[a]+photlen[a+1])/2)>len) {
  if (fate>photeff[a]) {
   return 0;
  } else {
   return 1;
  }
 }

return 1;
} // eof

