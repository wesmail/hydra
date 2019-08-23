//*-- Authors: Leszek Kidon & Jacek Otwinowski
//*-- Last Modified: 07/06/2000
///////////////////////////////////////////////////////////////////////////// 
//
//  HShowerPadDigitizer
//
// This class digitizes the shower pads. For each fired wire it calculates all
// the pads which the charge couples to. For each pad the track numbers of the
// particles that fired the pad are stored in a linear category (HShowerTrack).
// This category is sortable, in particular the track numbers are sorted by
// the respective pad address.
// All the fired pads are stored in a matrix category (catShowerRawMatr,
// this matrix category is used only for the simulated data).  
//
//////////////////////////////////////////////////////////////////////////////
#include "hshowerpaddigitizer.h"

#include <stdio.h>                                 
#include <stdlib.h>                                 
#include <math.h>
#include "hruntimedb.h"
#include "hevent.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hshowerdetector.h"
#include "hcategory.h"
#include "hmatrixcategory.h"
#include "hlinearcategory.h"
#include "hlocation.h"
#include "hshowergeantwire.h"
#include "hshowerraw.h"
#include "hshowercal.h"
#include "hshowertrack.h"
#include "hshowerpad.h"
#include "hshowerframe.h"
#include "hshowerdigidetpar.h"
#include "hshowergeometry.h"
#include "hdebug.h"
#include "hades.h"
#include "showerdef.h"


#define MIN(A,B) (((A) <= (B)) ? (A) : (B))
#define MAX(A,B) (((A) >= (B)) ? (A) : (B))

ClassImp(HShowerPadDigitizer)

HShowerPadDigitizer::HShowerPadDigitizer(Text_t *name,Text_t *title) :
  HShowerDigitizer(name,title)
{
  fFactorInd = 0.0;
  fTrackIter = NULL;
  fChannelCoeff = 256.0 / 60.0; // QDC calibration factor
}

HShowerPadDigitizer::HShowerPadDigitizer()
{
  fFactorInd = 0.0;
  fTrackIter = NULL;
}


HShowerPadDigitizer::~HShowerPadDigitizer(void) {
  if (fTrackIter) delete fTrackIter;

}

Bool_t HShowerPadDigitizer::init() {
 // creates ShowerGeantWire(input), ShowerRawMatr(output) and ShowerTrack
 // categories and adds them to current event
 // creates an iterator for ShowerTrack category   

    HCategory *pCat;
    HShowerDetector *pShowerDet = (HShowerDetector*)gHades->getSetup()
                                                  ->getDetector("Shower");

    pCat=gHades->getCurrentEvent()->getCategory(catShowerGeantWire);
    if (!pCat) {
      pCat=pShowerDet->buildCategory(catShowerGeantWire);

      if (!pCat) return kFALSE;
      else gHades->getCurrentEvent()
                         ->addCategory(catShowerGeantWire, pCat, "Shower");
    }
    setInCat(pCat);

    pCat=gHades->getCurrentEvent()->getCategory(catShowerRawMatr);
    if (!pCat) {
      pCat=pShowerDet->buildCategory(catShowerRawMatr);

      if (!pCat) return kFALSE;
      else gHades->getCurrentEvent()
                        ->addCategory(catShowerRawMatr, pCat, "Shower");
    }

    setOutCat(pCat);

    pCat=gHades->getCurrentEvent()->getCategory(catShowerTrack);
    if (!pCat) {
      pCat=pShowerDet->buildCategory(catShowerTrack);

      if (!pCat) return kFALSE;
      else gHades->getCurrentEvent()
                        ->addCategory(catShowerTrack, pCat, "Shower");
    }

    setTrackCat(pCat);

    int res = HShowerDigitizer::init();
    initPar();
  
    fTrackIter = (HIterator*)getTrackCat()->MakeIterator("native");
  
    return res;
}

HShowerPadDigitizer& HShowerPadDigitizer::operator=(HShowerPadDigitizer &c) {
// It should have been done
  return c;
}

void HShowerPadDigitizer::initPar() {
   fFactorInd = ((HShowerDigiDetPar*)getDigiParSet())->getFactorInd(); 
}

Bool_t HShowerPadDigitizer::digitize(HDataObject *pHit) {
  HLocation fLoc;

#if DEBUG_LEVEL>2
  gDebuger->enterFunc("HShowerPadDigitizer::execute");
  gDebuger->message("Cal cat points to %p",fCalCat);
#endif

  HShowerGeantWire *pGeantWire = (HShowerGeantWire*) pHit;
  HShowerDigiDetPar *pDetPar = (HShowerDigiDetPar*) getDigiParSet();

  if (!pDetPar)
  {
    Error("HShowerPadDigitizer::digitize",
                            "Digitization parameters not initialised!");
    return 0;
}

  if (pGeantWire) {
//////////////////////////////////////////////////////////
    
    digiPads(pGeantWire);

/////////////////////////////////////////////////////////
  }

#if DEBUG_LEVEL>2
  gDebuger->leaveFunc("HShowerPadDigitizer::execute");
#endif

  return (pGeantWire) ? kTRUE : kFALSE;
}

Int_t HShowerPadDigitizer::sort() {
//    if (((HLinearCategory*)getTrackCat())->IsSortable())
    ((HLinearCategory*)getTrackCat())->sort();
    return 0;
}


void HShowerPadDigitizer::updateTrack(HShowerRaw *pShowerRaw,  HLocation& loc,  Int_t nTrack) {
// this function eliminates double track for the same  address 
// it means that with one hit should be connected no more than one pad address and one track

   HShowerTrack *pShowerTrack=NULL;
   Int_t nAddress = pShowerRaw->getAddress();
   
   fTrackIter->Reset();
   while((pShowerTrack = (HShowerTrack*)fTrackIter->Next())) 
      if ((pShowerTrack->getAddress()==nAddress) && 
                                      (pShowerTrack->getTrack()==nTrack))
         return;

   pShowerTrack = (HShowerTrack*)((HLinearCategory*)getTrackCat())
                                                        ->getNewSlot(loc);

   if (pShowerTrack) {
      pShowerTrack = new(pShowerTrack) HShowerTrack;
      pShowerTrack->setEventNr(pShowerRaw->getEventNr());
      pShowerTrack->setAddress(pShowerRaw->getAddress());
      pShowerTrack->setTrack(nTrack);
   }
}

void HShowerPadDigitizer::updatePad(HShowerPad *pPad, Float_t fIndQ, Int_t nSect, Int_t nMod, Int_t nTrack)
{
   Int_t nRow, nCol;
   HShowerRaw *pShowerRaw=NULL;
   HLocation loc;
   Float_t fTrackThreshold = ((HShowerDigiDetPar*)getDigiParSet())
                                                    ->getTrackThreshold();

   pPad->getPadPos(&nRow, &nCol);
   loc.set(4, nSect, nMod, nRow, nCol);

   pShowerRaw = (HShowerRaw*)((HMatrixCategory*)getOutCat())->getObject(loc);
   if (!pShowerRaw)
   {
      pShowerRaw = (HShowerRaw*)((HMatrixCategory*)getOutCat())->getSlot(loc);   
      if (pShowerRaw != NULL)
      { 
        pShowerRaw = new(pShowerRaw) HShowerRaw;
        pShowerRaw->setEventNr(lNrEvent);
        pShowerRaw->setSector(loc[0]);
        pShowerRaw->setModule(loc[1]);
        pShowerRaw->setRow(loc[2]);
        pShowerRaw->setCol(loc[3]);
      }
   }

   if(pShowerRaw) {
      pShowerRaw->addCharge(fIndQ * fChannelCoeff);  
      if (fIndQ * fChannelCoeff > fTrackThreshold) 
         updateTrack(pShowerRaw, loc, nTrack);
   }

}


Float_t HShowerPadDigitizer::calcCharge(Float_t charge, Float_t dist,
                Float_t Xd, Float_t Yd, Float_t Xu, Float_t Yu) {
//charge density matrix is calculated

const float twoPI = 6.28318530718;

return ((fFactorInd*charge/twoPI)*(
                      atan(Xd*Yd/(dist*sqrt(dist*dist+Xd*Xd+Yd*Yd))) -   
                      atan(Xd*Yu/(dist*sqrt(dist*dist+Xd*Xd+Yu*Yu))) +   
                      atan(Xu*Yu/(dist*sqrt(dist*dist+Xu*Xu+Yu*Yu))) -  
                      atan(Xu*Yd/(dist*sqrt(dist*dist+Xu*Xu+Yd*Yd)))));  
} // eof

void HShowerPadDigitizer::digiPads(HShowerGeantWire* pWireHit) {
   Float_t fX, fY, fQ;
   Int_t nPadX, nPadY, nPadRange;
   Int_t nModule, nSector;

   pWireHit->getXY(&fX, &fY);
   nModule = pWireHit->getModule();
   nSector = pWireHit->getSector();
   
   fQ = pWireHit->getCharge();

   HShowerPadTab* pPadParam = ((HShowerGeometry*)getGeometry())
                                                   ->getPadParam(nModule);

   nPadRange = ((HShowerDigiDetPar*)getDigiParSet())->getPadRange();
   HShowerPad* pCPad = pPadParam->getPad(fX, fY);     //row = Y,col = X
   HShowerPad* pTmpPad; 
   if (pCPad)
     pCPad->getPadPos(&nPadY, &nPadX);  //row = Y; col = X
   else
      return;

   for (int m = 0; m < 2*nPadRange + 1; m++) {
        pTmpPad = pPadParam->getPad(nPadY, nPadX + m - nPadRange);
        if(pTmpPad && pTmpPad->getPadFlag())
            numericalCalc(pTmpPad, pWireHit); //charge is calculated on pad 
    }

} // eof

Int_t HShowerPadDigitizer::calcLimit(Float_t fCor, Int_t nMatrixRange,
                                                            Float_t fBoxSize)
{
//charge density matrix is delimited to the sensitive area which covers fired pads 


  Int_t k, nLimit;
 
  k = (Int_t)((fabs(fCor) + 0.5*fBoxSize)/fBoxSize);

  if (fCor < 0 ) 
     nLimit = (k>nMatrixRange) ? 0 : nMatrixRange - k; 
  else
     nLimit = (k>nMatrixRange) ? 2*nMatrixRange : nMatrixRange + k;

  return nLimit; 
}


void HShowerPadDigitizer::moveCoord(HShowerPad *pPad, Float_t fDx, Float_t fDy,
          Float_t *corXld, Float_t *corYld, Float_t *corXlu, Float_t *corYlu,
          Float_t *corXrd, Float_t *corYrd, Float_t *corXru, Float_t *corYru) {
// moves charge density matrix to the place where is placed induced charge on the sense wire

  *corXld = pPad->fXld - fDx;
  *corYld = pPad->fYld - fDy;
  *corXlu = pPad->fXlu - fDx;
  *corYlu = pPad->fYlu - fDy;
  *corXrd = pPad->fXrd - fDx;
  *corYrd = pPad->fYrd - fDy;
  *corXru = pPad->fXru - fDx;
  *corYru = pPad->fYru - fDy;
}


void HShowerPadDigitizer::analyticCalc(HShowerPad *pPad, HShowerGeantWire* pWireHit)
{
// this function calculates charge induced on pad analytically 
// it is useful only for square or rectangular pads
  Float_t fX, fY;
  Float_t fIndQ;

  Float_t corXld, corYld, corXlu, corYlu;
  Float_t corXrd, corYrd, corXru, corYru;
  pWireHit->getXY(&fX, &fY);

  moveCoord(pPad, fX, fY, &corXld, &corYld, &corXlu, &corYlu, 
                                &corXrd, &corYrd, &corXru, &corYru);

  fIndQ =  calcCharge(pWireHit->getCharge(), 
     ((HShowerDigiDetPar*)getDigiParSet())->getPlaneDist(),
     corXld, corYld, corXru, corYru);

  updatePad(pPad, fIndQ, pWireHit->getSector(), 
                         pWireHit->getModule(), pWireHit->getTrack()); 
}

void HShowerPadDigitizer::numericalCalc(HShowerPad *pPad, HShowerGeantWire* pWireHit)
{
//calculates charge on pad numerically by integrating elements of charge density matrix

  HShowerFrame *pFrame = ((HShowerGeometry*)getGeometry())->getFrame(0);
  Float_t* pfChargeMatrix = ((HShowerDigiDetPar*)getDigiParSet())
                                                   ->getChargeMatrix();
  Int_t nMatrixRange = ((HShowerDigiDetPar*)getDigiParSet())->getMatrixRange();
  Float_t fBoxSize = ((HShowerDigiDetPar*)getDigiParSet())->getBoxSize();

  Float_t fX, fY;
  Float_t fIndQ;

  Float_t corXld, corYld, corXlu, corYlu;
  Float_t corXrd, corYrd, corXru, corYru;
  Int_t limXd, limYd, limXu, limYu;
  Float_t nPartPadX, nPartPadY;

  pWireHit->getXY(&fX, &fY);
  moveCoord(pPad, fX, fY, &corXld, &corYld, &corXlu, &corYlu,       
                                &corXrd, &corYrd, &corXru, &corYru); 

////////////////////////////////////

  limXd = calcLimit(MIN(corXld, corXlu), nMatrixRange, fBoxSize);    
  limYd = calcLimit(corYld, nMatrixRange, fBoxSize);                
  limXu = calcLimit(MAX(corXru, corXrd), nMatrixRange, fBoxSize);
  limYu = calcLimit(corYru, nMatrixRange, fBoxSize);

  fIndQ = 0.0;
  for (int b=limYd; b<=limYu; b++)
    for (int a=limXd; a<=limXu; a++)
    {

       nPartPadX =  (((a-nMatrixRange)*fBoxSize) + fX);
       nPartPadY =  (((b-nMatrixRange)*fBoxSize) + fY);

       if (!pPad->isOut(nPartPadX, nPartPadY) &&
                                !pFrame->isOut(nPartPadX, nPartPadY))
       {
          fIndQ += pfChargeMatrix[a+b*(2*nMatrixRange+1)];

       }
    }

    fIndQ *= pWireHit->getCharge(); //charge induced on pad 
    if (fIndQ)
           updatePad(pPad, fIndQ, pWireHit->getSector(),
                         pWireHit->getModule(), pWireHit->getTrack());
}








