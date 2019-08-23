#include "hrichhitdigitizer.h"
#include "hevent.h"
#include "hcategory.h"
#include "hlinearcategory.h"
#include "hlocation.h"
#include "hrichgeantdirhit.h"
#include "hrichgeantwire.h"
#include "hrichdigidetpar.h"
#include "hdebug.h"
#include "hades.h"
#include "richdef.h"


ClassImp(HRichHitDigitizer)

HRichHitDigitizer::HRichHitDigitizer(Text_t *name,Text_t *title) :
  HRichDigitizer(name,title)
{
  init();
}

HRichHitDigitizer::HRichHitDigitizer()
{
  init();
}


HRichHitDigitizer::~HRichHitDigitizer(void) {
}

Bool_t HRichHitDigitizer::init() {
    setInCat(gHades->getCurrentEvent()->getCategory(catRichGeantDirHit));
    setOutCat(gHades->getCurrentEvent()->getCategory(catRichGeantWire));
    return HRichDigitizer::init();
}

HRichHitDigitizer& HRichHitDigitizer::operator=(HRichHitDigitizer &c) {
// It should have been done
  return c;
}

Bool_t HRichHitDigitizer::digitize(HDataObject *pHit) {
  HLocation fLoc;
  Bool_t nResult = kFALSE;

#if DEBUG_LEVEL>2
  gDebuger->enterFunc("HRichHitDigitizer::execute");
  gDebuger->message("Raw cat points to %p",fRawCat);
#endif

  HRichGeantDirHit *pCherHit = (HRichGeantDirHit*) pHit;
  HRichDigiDetPar *pDetPar = (HRichDigiDetPar*) getDigiParSet();

  if (!pDetPar)
  {
    Error("HRichHitDigitizer::digitize", "Digitazation parameters not initialised !!");
    return 0;
  }

  if (pCherHit) {
//////////////////////////////////////////////////////////
    
     nResult = digiHits(pCherHit);

/////////////////////////////////////////////////////////
  }

#if DEBUG_LEVEL>2
  gDebuger->leaveFunc("HRichHitDigitizer::execute");
#endif

  return nResult;
}

Bool_t HRichHitDigitizer::digiHits(HRichGeantDirHit *simhit) {
 int hit=0;
 HLocation fLoc;
 HRichGeantWire *pGeantWire=NULL;
 Int_t nStep;
 Float_t lStep;

 Float_t numElec = ((HRichDigiDetPar*)getDigiParSet())->getNumElec();
 HRichWireTab *phWires = ((HRichDigiDetPar*)getDigiParSet())->getWireTab();
 HRichFrame *phFrame = ((HRichDigiDetPar*)getDigiParSet())->getFrame(0);

 Float_t fTheta, fPhi;
 Float_t fX, fY, fZ;

 simhit->getXYZ(&fX, &fY, &fZ);
 simhit->getAngles(&fTheta, &fPhi);
 
// statflag==2 means in this simulation detector
  if (simhit->IsMirror() == 0 && simhit->getEnerLos() > 0.0 &&
         !phFrame->isOut(fX, fY)) {

    nStep = (int)(1+numElec*simhit->getEnerLos());
    lStep = simhit->getTLen()/(float)nStep;

    for (int k=0; k < nStep; k++) {
      float fNewX, fNewY;
      fNewX = fX + (k+1) * lStep * sin(fTheta)*cos(fPhi);
      fNewY = fY + (k+1) * lStep * sin(fTheta)*sin(fPhi);

      hit=0;
      if (phWires->calcHit(fNewX, fNewY)) {
         hit=1;
         pGeantWire = (HRichGeantWire*)((HLinearCategory*)getOutCat())
                                                        ->getNewSlot(fLoc);
         if (pGeantWire != NULL) {
            pGeantWire = new(pGeantWire) HRichGeantWire;
            pGeantWire->setSrcQ(qHit);
            phWires->fillGeantWireHit(pGeantWire);
         }
      }
    }
  }

  return hit;
}















