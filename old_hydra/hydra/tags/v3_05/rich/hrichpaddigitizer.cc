#include <stdlib.h>                                 
#include <math.h>
#include "hrichpaddigitizer.h"
#include "hevent.h"
#include "hcategory.h"
#include "hmatrixcategory.h"
#include "hlocation.h"
#include "hrichgeantwire.h"
#include "hrichraw.h"
#include "hrichpad.h"
#include "hrichframe.h"
#include "hrichdigidetpar.h"
#include "hdebug.h"
#include "hades.h"
#include "richdef.h"


ClassImp(HRichPadDigitizer)

HRichPadDigitizer::HRichPadDigitizer(Text_t *name,Text_t *title) :
  HRichDigitizer(name,title)
{
  init();
  fFactorInd = 0.0;
}

HRichPadDigitizer::HRichPadDigitizer()
{
  init();
  fFactorInd = 0.0;
}


HRichPadDigitizer::~HRichPadDigitizer(void) {
}

Bool_t HRichPadDigitizer::init() {
    setInCat(gHades->getCurrentEvent()->getCategory(catRichGeantWire));
    setOutCat(gHades->getCurrentEvent()->getCategory(catRichRaw));
    return HRichDigitizer::init();
}

HRichPadDigitizer& HRichPadDigitizer::operator=(HRichPadDigitizer &c) {
// It should have been done
  return c;
}

void HRichPadDigitizer::initPar() {
   fFactorInd = ((HRichDigiDetPar*)getDigiParSet())->getFactorInd(); 
}

Bool_t HRichPadDigitizer::digitize(HDataObject *pHit) {
  HLocation fLoc;

#if DEBUG_LEVEL>2
  gDebuger->enterFunc("HRichPadDigitizer::execute");
  gDebuger->message("Raw cat points to %p",fRawCat);
#endif

  HRichGeantWire *pGeantWire = (HRichGeantWire*) pHit;
  HRichDigiDetPar *pDetPar = (HRichDigiDetPar*) getDigiParSet();

  if (!pDetPar)
  {
    Error("HRichPadDigitizer::digitize", "Digitazation parameters not initialised !!");
    return 0;
  }

  if (pGeantWire) {
//////////////////////////////////////////////////////////
    
    digiPads(pGeantWire);

/////////////////////////////////////////////////////////
  }

#if DEBUG_LEVEL>2
  gDebuger->leaveFunc("HRichPadDigitizer::execute");
#endif

  return (pGeantWire) ? kTRUE : kFALSE;
}

void HRichPadDigitizer::updateCharge(HRichPad *pPad, Float_t fIndQ, Int_t nSect)
{
   Int_t nRow, nCol;
   HRichRaw *pRichRaw=NULL;
   HLocation loc;

   pPad->getPadXY(&nRow, &nCol);
   loc.set(3, nSect, nRow, nCol);

   pRichRaw = (HRichRaw*)((HMatrixCategory*)getOutCat())->getObject(loc);
   if (!pRichRaw)
   {
      pRichRaw = (HRichRaw*)((HMatrixCategory*)getOutCat())->getSlot(loc);   
      if (pRichRaw != NULL)
      { 
        pRichRaw = new(pRichRaw) HRichRaw;
        pRichRaw->setEventNr(lNrEvent);
        pRichRaw->setSector(loc[0]);
        pRichRaw->setRow(loc[1]);
        pRichRaw->setCol(loc[2]);
      }
   }

   if(pRichRaw)
      pRichRaw->addCharge(fIndQ/3500);  

}


Float_t HRichPadDigitizer::calcCharge(Float_t charge, Float_t dist,
                Float_t Xd, Float_t Yd, Float_t Xu, Float_t Yu) {

const float twoPI = 6.28318530718;

return ((fFactorInd*charge/twoPI)*(
                      atan(Xd*Yd/(dist*sqrt(dist*dist+Xd*Xd+Yd*Yd))) - 
                      atan(Xd*Yu/(dist*sqrt(dist*dist+Xd*Xd+Yu*Yu))) +
                      atan(Xu*Yu/(dist*sqrt(dist*dist+Xu*Xu+Yu*Yu))) -
                      atan(Xu*Yd/(dist*sqrt(dist*dist+Xu*Xu+Yd*Yd)))));
} // eof

void HRichPadDigitizer::digiPads(HRichGeantWire* pWireHit) {
   Float_t fX, fY, fQ;
   Int_t nPadX, nPadY, nPadRange;

   pWireHit->getXY(&fX, &fY);
   fQ = pWireHit->getCharge();

   HRichPadTab* pPadParam = ((HRichDigiDetPar*)getDigiParSet())->getPadParam();
   nPadRange = ((HRichDigiDetPar*)getDigiParSet())->getPadRange();
   HRichPad* pCPad = pPadParam->getPad(fX, fY);
   HRichPad* pTmpPad; 
   if (pCPad)
     pCPad->getPadXY(&nPadX, &nPadY);
   else
      return;

    for (int n = 0; n < 2*nPadRange + 1; n++)
      for (int m = 0; m < 2*nPadRange + 1; m++) {
        pTmpPad = pPadParam->getPad(nPadX + m - nPadRange, 
                                             nPadY + n - nPadRange); 
        if (pTmpPad) {
          switch(pTmpPad->getPadFlag()) {
            case 1: analyticCalc(pTmpPad, pWireHit);
                    break;

            case 2: numericalCalc(pTmpPad, pWireHit);
                    break;          

          } // eof switch
        }
    }
} // eof

Int_t HRichPadDigitizer::calcLimit(Float_t fCor, Int_t nMatrixRange,
                                                            Float_t fBoxSize)
{
  Int_t k, nLimit;
 
  k = (Int_t)((fabs(fCor) + 0.5*fBoxSize)/fBoxSize);

  if (fCor < 0 ) 
     nLimit = (k>nMatrixRange) ? 0 : nMatrixRange - k; 
  else
     nLimit = (k>nMatrixRange) ? 2*nMatrixRange : nMatrixRange + k;

  return nLimit;
}


void HRichPadDigitizer::moveCoord(HRichPad *pPad, Float_t fDx, Float_t fDy,
              Float_t *corXd, Float_t *corYd, Float_t *corXu, Float_t *corYu) {
  /* =============== this MUST be changed ====
  *corXd = pPad->fXd - fDx;
  *corYd = pPad->fYd - fDy;
  *corXu = pPad->fXu - fDx;
  *corYu = pPad->fYu - fDy;
  ==========================================*/
}


void HRichPadDigitizer::analyticCalc(HRichPad *pPad, HRichGeantWire* pWireHit)
{
  Float_t fX, fY;
  Float_t fIndQ;

  Float_t corXd, corYd, corXu, corYu;
  pWireHit->getXY(&fX, &fY);

  moveCoord(pPad, fX, fY, &corXd, &corYd, &corXu, &corYu);

  fIndQ =  calcCharge(pWireHit->getCharge(), 
     ((HRichDigiDetPar*)getDigiParSet())->getPlaneDist(),
     corXd, corYd, corXu, corYu);

  updateCharge(pPad, fIndQ, 0); 
}

void HRichPadDigitizer::numericalCalc(HRichPad *pPad, HRichGeantWire* pWireHit)
{
  HRichFrame *pFrame = ((HRichDigiDetPar*)getDigiParSet())->getFrame(0);
  Float_t* pfChargeMatrix = ((HRichDigiDetPar*)getDigiParSet())
                                                   ->getChargeMatrix();
  Int_t nMatrixRange = ((HRichDigiDetPar*)getDigiParSet())->getMatrixRange();
  Float_t fBoxSize = ((HRichDigiDetPar*)getDigiParSet())->getBoxSize();

  Float_t fX, fY;
  Float_t fIndQ;

  Float_t corXd, corYd, corXu, corYu;
  Int_t limXd, limYd, limXu, limYu;
  Int_t nPartPadX, nPartPadY;

  pWireHit->getXY(&fX, &fY);
  moveCoord(pPad, fX, fY, &corXd, &corYd, &corXu, &corYu);
 
  limXd = calcLimit(corXd, nMatrixRange, fBoxSize); 
  limYd = calcLimit(corYd, nMatrixRange, fBoxSize); 
  limXu = calcLimit(corXu, nMatrixRange, fBoxSize); 
  limYu = calcLimit(corYu, nMatrixRange, fBoxSize);

  for (int b=limYd; b<=limYu; b++)
    for (int a=limXd; a<=limXu; a++)
    {    
       nPartPadX = (int) ((a-nMatrixRange)*fBoxSize);
       nPartPadY = (int) ((b-nMatrixRange)*fBoxSize);
       if (!pPad->isOut(nPartPadX, nPartPadY) && 
                                !pFrame->isOut(nPartPadX, nPartPadY))
       {
          fIndQ = pWireHit->getCharge()
                        * pfChargeMatrix[a+b*(2*nMatrixRange+1)];

          updateCharge(pPad, fIndQ, 0); 
       }
    }   

}

