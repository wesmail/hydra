//*-- Authors: Leszek Kidon & Jacek Otwinowski
//*-- Last Modified: 08/07/2001 (Marcin Jaskula)
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
#include "hshowerwire.h"
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
#define TABLE_SIZE 10

ClassImp(HShowerPadDigitizer)

HShowerPadDigitizer::HShowerPadDigitizer(Text_t *name,Text_t *title) :
  HShowerDigitizer(name,title)
{
  fTrackIter = NULL;
  fChannelCoeff = 256.0 / 60.0; // QDC calibration factor
}

HShowerPadDigitizer::HShowerPadDigitizer()
{
  fTrackIter = NULL;
}


HShowerPadDigitizer::~HShowerPadDigitizer(void) {
  if (fTrackIter) delete fTrackIter;

}

Bool_t HShowerPadDigitizer::init() {
 // creates ShowerGeantWire(input), ShowerRawMatr(output) and ShowerTrack
 // categories and adds them to current event
 // creates an iterator for ShowerTrack category
    printf("initialization of shower pad digitizer \n");
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

    fTrackIter = (HIterator*)getTrackCat()->MakeIterator("native");

    return res;
}

HShowerPadDigitizer& HShowerPadDigitizer::operator=(HShowerPadDigitizer &c) {
// It should have been done
  return c;
}

Bool_t HShowerPadDigitizer::digitize(TObject *pHit) {
  HLocation fLoc;

#if DEBUG_LEVEL>2
  gDebuger->enterFunc("HShowerPadDigitizer::execute");
  gDebuger->message("Hit  cat points to %p",pHit);
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
      pShowerTrack->setAddress(pShowerRaw->getAddress());
      pShowerTrack->setTrack(nTrack);
   }
}

void HShowerPadDigitizer::updatePad(HShowerPad *pPad, Float_t fIndQ, Int_t nSect, Int_t nMod, Int_t nTrack)
{
   Int_t nRow, nCol;
   HShowerRaw *pShowerRaw=NULL;
   HLocation loc;
   Float_t fUpdateThreshold = ((HShowerDigiDetPar*)getDigiParSet())
                                                    ->getUpdatePadThreshold();

   pPad->getPadPos(&nRow, &nCol);
   loc.set(4, nSect, nMod, nRow, nCol);

   pShowerRaw = (HShowerRaw*)((HMatrixCategory*)getOutCat())->getObject(loc);
   if (!pShowerRaw)
   {
      pShowerRaw = (HShowerRaw*)((HMatrixCategory*)getOutCat())->getSlot(loc);
      if (pShowerRaw != NULL)
      {
        pShowerRaw = new(pShowerRaw) HShowerRaw;
        pShowerRaw->setSector(loc[0]);
        pShowerRaw->setModule(loc[1]);
        pShowerRaw->setRow(loc[2]);
        pShowerRaw->setCol(loc[3]);
      }
   }

    if(pShowerRaw)
    {
        pShowerRaw->addCharge(fIndQ);

        if(pShowerRaw->getCharge() >= fUpdateThreshold)
            updateTrack(pShowerRaw, loc, nTrack);
    }
}

Float_t HShowerPadDigitizer::calcCharge(Float_t charge, Float_t dist,
                Float_t Xd, Float_t Yd, Float_t Xu, Float_t Yu) {
//charge density matrix is calculated

const float twoPI = 6.28318530718;

return ((charge/twoPI)*(
                      atan(Xd*Yd/(dist*sqrt(dist*dist+Xd*Xd+Yd*Yd))) -
                      atan(Xd*Yu/(dist*sqrt(dist*dist+Xd*Xd+Yu*Yu))) +
                      atan(Xu*Yu/(dist*sqrt(dist*dist+Xu*Xu+Yu*Yu))) -
                      atan(Xu*Yd/(dist*sqrt(dist*dist+Xu*Xu+Yd*Yd)))));
}

/* ************************************************************************** */

void HShowerPadDigitizer::digiPads(HShowerGeantWire* pWireHit)
{
Float_t fX, fY, fQ;
Float_t factor;
Float_t sum;
Int_t   nPadX, nPadY, nPadRange;
Float_t pChargeTable[MAX_PADS_DIST];
int     iMaxInArray, m;
HShowerPad *pTmpPad;
HShowerPad *pCPad;
Float_t fPadThreshold    = ((HShowerDigiDetPar *)getDigiParSet())
                                                    ->getPadThreshold();
Float_t fUpdateThreshold = ((HShowerDigiDetPar *)getDigiParSet())
                                                    ->getUpdatePadThreshold();
HShowerPadTab *pPadParam = ((HShowerGeometry *)getGeometry())
                                    ->getPadParam(pWireHit->getModule());

        // don't calculate if charge belowe threshold
    if((fQ = pWireHit->getCharge()) < fUpdateThreshold)
        return;

    pWireHit->getXY(&fX, &fY);

            //row = Y,col = X
    if((pCPad = pPadParam->getPad(fX, fY)) == NULL)
        return;

    pCPad->getPadPos(&nPadY, &nPadX);  //row = Y; col = X
    iMaxInArray = MAX_PADS_DIST;
    nPadRange   = (MAX_PADS_DIST - 1) / 2;

    if(numericalCalc(pCPad, pWireHit, pChargeTable) != 0)
        return;

        // calculate sum for the first renormalization
    sum = 0.0f;
    for(m = 0; m < iMaxInArray; m++)
        sum += pChargeTable[m];

    if(sum <= 0.0f)
        return;

    factor = fQ / sum;
    for(m = 0; m < iMaxInArray; m++)
    {
        pChargeTable[m] *= factor;
    }

        // calculate sum for a renormalization
    sum = 0.0f;
    for(m = 0; m < iMaxInArray; m++)
    {
        if(pChargeTable[m] >= fPadThreshold)
            sum += pChargeTable[m];
        else
            pChargeTable[m] = 0.0f;
    }

        // if sum is below threshold put all charge to the central pad
    if(sum < fPadThreshold)
    {
        for(m = 0; m < iMaxInArray; m++)
            pChargeTable[m] = 0.0f;

        pChargeTable[nPadRange] = fQ;
    }
    else
    {
            // renormalization: sum of the charge on the pads
            //                  must be equal to the charge on the wire
        factor = fQ / sum;

        for(m = 0; m < iMaxInArray; m++)
        {
            if(pChargeTable[m] > 0.0f)
                pChargeTable[m] *= factor;
        }
    }

    for(m = 0; m < iMaxInArray; m++)
    {
        pTmpPad = pPadParam->getPad(nPadY, nPadX + m - nPadRange);
        if(((m == nPadRange) || (pChargeTable[m] >= fPadThreshold))
                && (pTmpPad != NULL) && (pTmpPad->getPadFlag()))
        {
            updatePad(pTmpPad, pChargeTable[m], pWireHit->getSector(),
                        pWireHit->getModule(), pWireHit->getTrack());
        }
    }
}

/* ************************************************************************** */

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


void HShowerPadDigitizer::moveCoord(HShowerPad *pPad,Float_t distWire, Float_t fDx, Float_t fDy,
          Float_t *corXld, Float_t *corYld, Float_t *corXlu, Float_t *corYlu,
          Float_t *corXrd, Float_t *corYrd, Float_t *corXru, Float_t *corYru) {
// moves charge density matrix to the place where is placed induced charge on the sense wire

  *corXld = pPad->fXld - fDx;
// *corYld = pPad->fYld - fDy;
  *corYld =  fDy - fDy - distWire;
  *corXlu = pPad->fXlu - fDx;
//  *corYlu = pPad->fYlu - fDy;
  *corYlu = fDy - fDy + distWire;
  *corXrd = pPad->fXrd - fDx;
//  *corYrd = pPad->fYrd - fDy;
  *corYrd = fDy - fDy - distWire;
  *corXru = pPad->fXru - fDx;
//  *corYru = pPad->fYru - fDy;
  *corYru = fDy - fDy + distWire;
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

  Int_t nModule = pWireHit->getModule();

  HShowerWireTab *pfWire = ((HShowerGeometry*)getGeometry())->
                                                getWireTab(nModule);
  Float_t distWire  = pfWire->getDistWire();

//  cout <<  " distWire " << distWire << endl;



  moveCoord(pPad, distWire, fX, fY, &corXld, &corYld, &corXlu, &corYlu,
                                &corXrd, &corYrd, &corXru, &corYru);

  fIndQ =  calcCharge(pWireHit->getCharge(),
     ((HShowerDigiDetPar*)getDigiParSet())->getPlaneDist(),
     corXld, corYld, corXru, corYru);

  updatePad(pPad, fIndQ, pWireHit->getSector(),
                         pWireHit->getModule(), pWireHit->getTrack());
}

//------------------------------------------------------------------------------

int HShowerPadDigitizer::numericalCalc(HShowerPad *pPad,
            HShowerGeantWire* pWireHit, Float_t pfValues[MAX_PADS_DIST])
{
Int_t         iModule;
HShowerFrame *pFrame;
Int_t         iMatrixRange;
Float_t       fBoxSize;
const Float_t *pfChargeMatrix;
Float_t       fDistWire;
Float_t       fAl, fAr, fBl, fBr;
Float_t       fXd, fXu, fYd, fYu;
Float_t       fXmin, fXmax;
Float_t       fYmin, fYmax;
Float_t       fWireX, fWireY;
Float_t       fY, fX, fQ;
Int_t         i;
HShowerFrameCorner *pCorner;

    if((pPad == NULL) || (pPad->getPadFlag() == 0))
        return -1;

    iModule        = pWireHit->getModule();
    pFrame         = ((HShowerGeometry *)getGeometry())->getFrame(iModule);
    fDistWire      = (((HShowerGeometry *)getGeometry())->getWireTab(iModule))
                                            ->getDistWire();
    iMatrixRange   = ((HShowerDigiDetPar *)getDigiParSet())->getMatrixRange();
    fBoxSize       = ((HShowerDigiDetPar *)getDigiParSet())->getBoxSize();
    pfChargeMatrix = ((HShowerDigiDetPar *)getDigiParSet())->getChargeMatrix();

        // left border
    if((pPad->getPadFlag() == 2) && (pPad->fXld < 0.0))
    {
        pCorner = pFrame->getCorner(0);
        fXd = pCorner->getX();
        fYd = pCorner->getY();

        pCorner = pFrame->getCorner(1);
        fXu = pCorner->getX();
        fYu = pCorner->getY();
    }
    else
    {
        fXd = pPad->fXld;
        fYd = pPad->fYld;
        fXu = pPad->fXlu;
        fYu = pPad->fYlu;
    }

    fAl = (fXu - fXd) / (fYu - fYd);
    fBl = fXu - fAl * fYu;

        // right border
    if((pPad->getPadFlag() == 2) && (pPad->fXld >= 0.0))
    {
        pCorner = pFrame->getCorner(3);
        fXd = pCorner->getX();
        fYd = pCorner->getY();

        pCorner = pFrame->getCorner(2);
        fXu = pCorner->getX();
        fYu = pCorner->getY();
    }
    else
    {
        fXd = pPad->fXrd;
        fYd = pPad->fYrd;
        fXu = pPad->fXru;
        fYu = pPad->fYru;
    }

    fAr = (fXu - fXd) / (fYu - fYd);
    fBr = fXu - fAr * fYu;

        // range of the charge matrix
    pWireHit->getXY(&fWireX, &fWireY);
    fXmin = fWireX - iMatrixRange * fBoxSize;
    fXmax = fWireX + iMatrixRange * fBoxSize;
    fYmin = fWireY - fDistWire + 0.5 * fBoxSize;
    fYmax = fWireY + fDistWire - 0.5 * fBoxSize;

    memset(pfValues, 0, MAX_PADS_DIST * sizeof(Float_t));

    for(fY = fYmin; fY <= fYmax; fY += fBoxSize)
    {
            // left border
        if((fX = fAl * fY + fBl) >= fXmin)
        {
            i = (int)((fX - fXmin) / fBoxSize);
            pfValues[0] += pfChargeMatrix[i];
            fQ = pfChargeMatrix[i];
        }
        else
            fQ = 0.0f;

            // right border
        if((fX = fAr * fY + fBr) <= fXmax)
        {
            i = (int)((fX - fXmin) / fBoxSize);
            pfValues[1] += pfChargeMatrix[i] - fQ;
            pfValues[2] += pfChargeMatrix[2 * iMatrixRange] - pfChargeMatrix[i];
        }
        else
            pfValues[1] += pfChargeMatrix[2 * iMatrixRange] - fQ;
    }

    return 0;
}
