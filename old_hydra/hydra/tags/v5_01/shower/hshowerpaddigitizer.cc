//*-- Author : Leszek Kidon
//*-- Modified : 22/02/2000 L.Fabbietti
///////////////////////////////////////////////////////////////////////////
//_________________________________________________________________________
//
// HShowerPadDigitizer
// this class digitizes the shower pads. For each fired wire it calculates all
// the pads which the charge couples to. For each pad the track numbers of the 
// particles that fired the pad are stored in a linear category (HShowerTrack).
// This category is sortable, in particular the track numbers are sorted by
// the respective pad address. 
// All the fired pads are stored in a matrix category (catShowerRawMatr,
// this matrix category is used only for the simulated data) and among their
// data members there are 2 integers: nTrack1, nTrack2.
// For each pad nTrack1 gives the index in the TClonesArray that correponds to
// the track of the first particle that has hit the pad; if any other particle
// hits the pad in the same event, nTrack2 is the index of the last particle.
//
///////////////////////////////////////////////////////////////////////////   
#include <stdio.h>                                 
#include <stdlib.h>                                 
#include <math.h>
#include "hshowerpaddigitizer.h"
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
#include "hshowerrawsim.h"
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
   fRawIter= NULL;

  fChannelCoeff = 256.0 / 60.0;
}

HShowerPadDigitizer::HShowerPadDigitizer()
{
  fFactorInd = 0.0;
  fTrackIter = NULL;
  fRawIter= NULL;
}

HShowerPadDigitizer::~HShowerPadDigitizer(void) {
  if (fTrackIter) delete fTrackIter;
  if (fRawIter) delete fRawIter;

  printf("dtor HShowerPadDigitizer\n");
}

Bool_t HShowerPadDigitizer::init(){
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
    fRawIter = (HIterator*)getOutCat()->MakeIterator("native");

    return res;
}

HShowerPadDigitizer& HShowerPadDigitizer::operator=(HShowerPadDigitizer &c) {
// It should have been done
  return c;
}

void HShowerPadDigitizer::initPar() {
   fFactorInd = ((HShowerDigiDetPar*)getDigiParSet())->getFactorInd(); 
}


Int_t HShowerPadDigitizer::execute()
{ 
    //
    // for each event the hshowertrack TClonesArray is sorted by the 
    // pad addresses, then for each pad the corresponding 
    // track numbers (even  if more than one) 
    // are searched in the TClonesArray in order 
    // to assign the two data members 
    // of the HShowerRawSim class : nTrack1, nTrack2. 
    //
  HDataObject *pHit;
  lNrEvent++;
  fIter->Reset();
  Int_t n = 0;

  while((pHit = (HDataObject*)fIter->Next()))
  {
     digitize(pHit);
     n++;
  }


  // *** added by LF 02/23/00 ***
  ((HLinearCategory*)getTrackCat())->sort();
  
  HShowerRawSim *rawobj;
  HShowerTrack * trackobj;
  fTrackIter->Reset();
  fRawIter->Reset();
  Int_t temp = 0;
  Int_t indTrack1 = 0;
  

  while((rawobj = (HShowerRawSim*)fRawIter->Next())){
   Int_t addPad = rawobj->getAddress();
   fTrackIter->Reset();
     while((trackobj = (HShowerTrack*)fTrackIter->Next())){
        Int_t addTrack = trackobj->getAddress();
	if(addPad == addTrack){   
            indTrack1 = ((HLinearCategory*)getTrackCat())->getIndex(trackobj);
	    if (temp!=addPad){    
               rawobj->setNTrack1(indTrack1);
               rawobj->setNTrack2(indTrack1); 
            } else {
               rawobj->setNTrack2(indTrack1);
            }                
            temp = addTrack;
        }
     }  
  } 
  // *** end of LF addition 
  return 0;
}

Bool_t HShowerPadDigitizer::digitize(HDataObject *pHit) {
  HLocation fLoc;

#if DEBUG_LEVEL>2
  gDebuger->enterFunc("HShowerPadDigitizer::execute");
  gDebuger->message("Cal cat points to %p",fCalCat);
#endif

  HShowerGeantWire *pGeantWire = (HShowerGeantWire*) pHit;
  HShowerDigiDetPar *pDetPar = (HShowerDigiDetPar*) getDigiParSet();

  if (!pDetPar) {
    Error("HShowerPadDigitizer::digitize",
          "Digitazation parameters not initialised !!");
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

void HShowerPadDigitizer::updateTrack(HShowerRawSim *pShowerRawSim, HLocation& loc, Int_t nTrack) {
    //  In this function the track numbers are stored. 
    // If a particle is hitting the same pad twice 
    // in the same  event, its track number is stored only once.

   HShowerTrack *pShowerTrack=NULL;
   Int_t nAddress = pShowerRawSim->getAddress();
   
   fTrackIter->Reset();
   
   
   while((pShowerTrack = (HShowerTrack*)fTrackIter->Next())) 
      if ((pShowerTrack->getAddress()==nAddress) && 
                                      (pShowerTrack->getTrack()==nTrack))
         return;

   pShowerTrack = (HShowerTrack*)((HLinearCategory*)getTrackCat())
                                                        ->getNewSlot(loc);

   if (pShowerTrack) {
      pShowerTrack = new(pShowerTrack) HShowerTrack;
      pShowerTrack->setEventNr(pShowerRawSim->getEventNr());
      pShowerTrack->setAddress(pShowerRawSim->getAddress());
      pShowerTrack->setTrack(nTrack);
   }
   
  
}

void HShowerPadDigitizer::updatePad(HShowerPad *pPad, Float_t fIndQ, Int_t nSect, Int_t nMod, Int_t nTrack)
{
    // for each raw hits the corresponding track is stored 
    // only if the charge on the 
    // pad is greater then the set threshold.
  
   Int_t nRow, nCol;
   HShowerRawSim *pShowerRawSim=NULL;
   HLocation loc;
   Float_t fTrackThreshold = ((HShowerDigiDetPar*)getDigiParSet())
                                                    ->getTrackThreshold();

   pPad->getPadPos(&nRow, &nCol);
   loc.set(4, nSect, nMod, nRow, nCol);

   pShowerRawSim = (HShowerRawSim*)((HMatrixCategory*)getOutCat())->getObject(loc);
   if (!pShowerRawSim)
   {

      pShowerRawSim = (HShowerRawSim*)((HMatrixCategory*)getOutCat())->getSlot(loc);   
  
      if (pShowerRawSim != NULL)
      { 
        pShowerRawSim = new(pShowerRawSim) HShowerRawSim;
        pShowerRawSim->setEventNr(lNrEvent);
        pShowerRawSim->setSector(loc[0]);
        pShowerRawSim->setModule(loc[1]);
        pShowerRawSim->setRow(loc[2]);
        pShowerRawSim->setCol(loc[3]);

    
      }
      
   }

   if(pShowerRawSim) {
      pShowerRawSim->addCharge(fIndQ * fChannelCoeff);
     
      if (fIndQ * fChannelCoeff > fTrackThreshold)
         updateTrack(pShowerRawSim, loc, nTrack);        
   }
  
}


Float_t HShowerPadDigitizer::calcCharge(Float_t charge, Float_t dist,
                Float_t Xd, Float_t Yd, Float_t Xu, Float_t Yu) {

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
   HShowerPad* pCPad = pPadParam->getPad(fX, fY);
   HShowerPad* pTmpPad; 
   if (pCPad)
     pCPad->getPadPos(&nPadX, &nPadY);
   else
      return;

    for (int n = 0; n < 2*nPadRange + 1; n++)
      for (int m = 0; m < 2*nPadRange + 1; m++) {
        pTmpPad = pPadParam->getPad(nPadX + m - nPadRange, 
                                             nPadY + n - nPadRange); 

        if(pTmpPad && pTmpPad->getPadFlag()) 
            numericalCalc(pTmpPad, pWireHit);
    }
} // eof

Int_t HShowerPadDigitizer::calcLimit(Float_t fCor, Int_t nMatrixRange,
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


void HShowerPadDigitizer::moveCoord(HShowerPad *pPad, Float_t fDx, Float_t fDy,
          Float_t *corXld, Float_t *corYld, Float_t *corXlu, Float_t *corYlu,
          Float_t *corXrd, Float_t *corYrd, Float_t *corXru, Float_t *corYru) {

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
  Int_t nPartPadX, nPartPadY;

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
       nPartPadX = (int) (((a-nMatrixRange)*fBoxSize) + fX);
       nPartPadY = (int) (((b-nMatrixRange)*fBoxSize) + fY);
       if (!pPad->isOut(nPartPadX, nPartPadY) && 
                                !pFrame->isOut(nPartPadX, nPartPadY))
       {
          fIndQ += pfChargeMatrix[a+b*(2*nMatrixRange+1)];

       }
    }  

    fIndQ *= pWireHit->getCharge();
    if (fIndQ > ((HShowerDigiDetPar*)getDigiParSet())->getRawThreshold())
       updatePad(pPad, fIndQ, pWireHit->getSector(), 
                           pWireHit->getModule(), pWireHit->getTrack()); 
}









