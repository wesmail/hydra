#include "hrichdigitizer.h"
#include <stdlib.h>
#include "hevent.h"
#include "hmatrixcatiter.h"
#include "hgeantrich.h"
#include "hcategory.h"
#include "hlinearcatiter.h"
#include "hlocation.h"
#include "hrichdigitisationpar.h"
#include "hrichgeometrypar.h"
#include "hrichanalysispar.h"
#include "hdebug.h"
#include "hades.h"
#include "hrichpad.h"
#include "hrichwiresignal.h"
#include "hspectrometer.h"
#include "hrichdetector.h"
#include "hruntimedb.h"
#include "hrichcal.h"
#include "richdef.h"


ClassImp(HRichDigitizer)



//----------------------------------------------------------------------------
HRichDigitizer::HRichDigitizer(Text_t *name, Text_t *title) : HReconstructor(name,title)
{
  fCerIter = NULL;
  fDirIter = NULL;

  fCerInputCat = NULL;
  fDirInputCat = NULL;
  fOutputCat = NULL;

  fQmean = 0.;
  fpChargePatternMatrix = NULL;
  for (Int_t i = 0; i < 9; i++) fDigitPadMatrix[i] = 0.;

  fQlower = 0.;
  fQupper = 0.;
  fChargePerChannel = 0.;
  fSigmaValue = 0.;

  fFactor1 = 0.;
  fFactor2 = 0.;
  fFactor3 = 0.;
  fFactor4 = 0.;
  fFactor5 = 0.;
  fFactor6 = 0.;
  fFactor7 = 0.;
  fFactor8 = 0.;
  fParam1 = 0.;
  fParam2 = 0.;


}
//============================================================================

//----------------------------------------------------------------------------
HRichDigitizer::HRichDigitizer()
{
  fCerIter = NULL;
  fDirIter = NULL;

  fCerInputCat = NULL;
  fDirInputCat = NULL;
  fOutputCat = NULL;

  fQmean = 0.;
  fpChargePatternMatrix = NULL;
  for (Int_t i = 0; i < 9; i++) fDigitPadMatrix[i] = 0.;

  fQlower = 0.;
  fQupper = 0.;
  fChargePerChannel = 0.;
  fSigmaValue = 0.;

  fFactor1 = 0.;
  fFactor2 = 0.;
  fFactor3 = 0.;
  fFactor4 = 0.;
  fFactor5 = 0.;
  fFactor6 = 0.;
  fFactor7 = 0.;
  fFactor8 = 0.;
  fParam1 = 0.;
  fParam2 = 0.;


}
//============================================================================

//----------------------------------------------------------------------------
HRichDigitizer::~HRichDigitizer() {
//  if (fCerIter) delete fCerIter;
//  if (fDirIter) delete fDirIter;
  fChargeOnWireList.Delete();
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HRichDigitizer::init() {
  lNrEvent = 0;
  fDigitisationPar = NULL;
  fGeometryPar = NULL;

  fQmean = 0.0;
  fpChargePatternMatrix = NULL;


  HCategory *pCat;
  HRichDetector *pRichDet = (HRichDetector*)gHades->getSetup()
                                                  ->getDetector("Rich");


  HRuntimeDb* rtdb=gHades->getRuntimeDb();

  HRichGeometryPar *pGeomPar = 
   (HRichGeometryPar*)rtdb->getContainer("RichGeometryParameters");
  if (pGeomPar == NULL) {
   pGeomPar = new HRichGeometryPar;
   rtdb->addContainer(pGeomPar);
  }
  setGeometryPar(pGeomPar);
  if (pGeomPar == NULL) return kFALSE;

  HRichDigitisationPar *pDigitPar = 
   (HRichDigitisationPar*)rtdb->getContainer("RichDigitisationParameters");
  if (pDigitPar == NULL) {
   pDigitPar = new HRichDigitisationPar;
   rtdb->addContainer(pDigitPar);
  }
  setDigitisationPar(pDigitPar);
  if (pDigitPar == NULL) return kFALSE;


  pCat = gHades->getCurrentEvent()->getCategory(catRichGeantRaw);
  if (pCat == NULL) {
   pCat = pRichDet->buildCategory(catRichGeantRaw);
   if (pCat == NULL) return kFALSE;
    else gHades->getCurrentEvent()
               ->addCategory(catRichGeantRaw, pCat, "Rich");
  }
  setCerInputCat(pCat);
  // fCerIter = (HMatrixCatIter*)getCerInputCat()->MakeIterator();

  pCat = gHades->getCurrentEvent()->getCategory(catRichGeantRaw+1);
  if (pCat == NULL) {
   pCat = pRichDet->buildCategory(catRichGeantRaw+1);
   if (pCat == NULL) return kFALSE;
    else gHades->getCurrentEvent()
               ->addCategory(catRichGeantRaw+1, pCat, "Rich");
  }
  setDirInputCat(pCat);
  // fDirIter = (HMatrixCatIter*)getDirInputCat()->MakeIterator();

  pCat = gHades->getCurrentEvent()->getCategory(catRichCal);
  if (pCat == NULL) {
   pCat = pRichDet->buildCategory(catRichCal);
   if (pCat == NULL) return kFALSE;
    else gHades->getCurrentEvent()
               ->addCategory(catRichCal, pCat, "Rich");
  }
  setOutputCat(pCat);


  if (fQmean == 0.) calcQmean();
  if (fpChargePatternMatrix == NULL) initChargePatternMatrix();

  fQlower = ((HRichDigitisationPar*)fDigitisationPar)->getQlower();
  fQupper = ((HRichDigitisationPar*)fDigitisationPar)->getQupper();
  fSigmaValue = ((HRichDigitisationPar*)fDigitisationPar)->getSigmaValue();
  fChargePerChannel =
      ((HRichDigitisationPar*)fDigitisationPar)->getChargePerChannel();


  fFactor1 = ((HRichDigitisationPar*)fDigitisationPar)->getFactor1();
  fFactor2 = ((HRichDigitisationPar*)fDigitisationPar)->getFactor2();
  fFactor3 = ((HRichDigitisationPar*)fDigitisationPar)->getFactor3();
  fFactor4 = ((HRichDigitisationPar*)fDigitisationPar)->getFactor4();
  fFactor5 = ((HRichDigitisationPar*)fDigitisationPar)->getFactor5();
  fFactor6 = ((HRichDigitisationPar*)fDigitisationPar)->getFactor6();
  fFactor7 = ((HRichDigitisationPar*)fDigitisationPar)->getFactor7();
  fFactor8 = ((HRichDigitisationPar*)fDigitisationPar)->getFactor8();
  fParam1 = ((HRichDigitisationPar*)fDigitisationPar)->getParameter1();
  fParam2 = ((HRichDigitisationPar*)fDigitisationPar)->getParameter2();



  return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
HRichDigitizer::HRichDigitizer(const HRichDigitizer& source) {
 // dummy
}
//============================================================================

//----------------------------------------------------------------------------
HRichDigitizer& HRichDigitizer::operator=(HRichDigitizer &source) {
 if (this != &source) {
  // dummy
 }
return *this;
}
//============================================================================

//----------------------------------------------------------------------------
Int_t HRichDigitizer::execute()
{

  HGeantRichPhoton *pCherenkovHit;
  HGeantRichDirect *pDirectHit;

  lNrEvent++;
  Int_t fCerNr = 0, fDirNr = 0;
  fChargeOnWireList.Delete();

  // fCerIter->Reset();
  fCerIter = (HMatrixCatIter*)getCerInputCat()->MakeIterator();
  while((pCherenkovHit = (HGeantRichPhoton*)fCerIter->Next()))
  {
     digitiseCherenkovHits(pCherenkovHit);
     fCerNr++;
  }

  // fDirIter->Reset();
  fDirIter = (HMatrixCatIter*)getDirInputCat()->MakeIterator();
  while((pDirectHit = (HGeantRichDirect*)fDirIter->Next()))
  {
     digitiseDirectHits(pDirectHit);
     fDirNr++;
  }

  digitisePads();

  delete fDirIter;
  delete fCerIter;

  return 0;
}
//============================================================================

//----------------------------------------------------------------------------
void HRichDigitizer::calcQmean() {

  if (fQmean == 0.)
   fQmean = 1.e5 * exp(((((HRichDigitisationPar*)fDigitisationPar)
                            ->getVoltage() - 2550)/75)*log(2));
}
//============================================================================

//----------------------------------------------------------------------------
Float_t HRichDigitizer::GaussFun(Float_t mean, Float_t sigma) {

 Float_t x,y,z,result;
 
 do {
  y = rand()/(double)RAND_MAX;
 } while (!y);
 z = rand()/(double)RAND_MAX;
 x = z*6.2831853;
 result = mean + sigma*sin(x)*sqrt(-2*log(y));
 return result;
}
//============================================================================

//----------------------------------------------------------------------------
Float_t HRichDigitizer::calcCharge(Float_t sourceCharge, Float_t distance, 
                                   Float_t Xd, Float_t Yd, Float_t Xu, Float_t Yu) {

const Float_t twoPI = 6.28318530718;

return ((((HRichDigitisationPar*)fDigitisationPar)->getCouplingFactor() * 
           sourceCharge / twoPI) *
  (atan(Xd*Yd/(distance*sqrt(distance*distance+Xd*Xd+Yd*Yd)))
 - atan(Xd*Yu/(distance*sqrt(distance*distance+Xd*Xd+Yu*Yu)))
 + atan(Xu*Yu/(distance*sqrt(distance*distance+Xu*Xu+Yu*Yu)))
 - atan(Xu*Yd/(distance*sqrt(distance*distance+Xu*Xu+Yd*Yd)))));
}
//============================================================================

//----------------------------------------------------------------------------
void HRichDigitizer::initChargePatternMatrix() {

 Int_t i,j;
 Int_t fMPadRange = ((HRichDigitisationPar*)fDigitisationPar)
                                              ->getMatrixPadRange();
 Float_t fMPadSize = ((HRichDigitisationPar*)fDigitisationPar)
                                             ->getMatrixPadSize();
 Float_t fPWDist = ((HRichGeometryPar*)fGeometryPar)
                                       ->getDistWiresPads();

 Float_t cornerX = -1.0 * (fMPadRange * fMPadSize + 0.5*fMPadSize);
 Float_t cornerY = cornerX;

 fpChargePatternMatrix = new Float_t[(2*fMPadRange+1)*(2*fMPadRange+1)];
 for (j = 0; j < 2*fMPadRange+1; j++)
  for (i = 0; i < 2*fMPadRange+1; i++)
   fpChargePatternMatrix[i+j*(2*fMPadRange+1)] = 
     calcCharge(1., fPWDist,
                cornerX+i*fMPadSize, cornerY+j*fMPadSize,
                cornerX+(i+1)*fMPadSize, cornerY+(j+1)*fMPadSize);
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HRichDigitizer::calcQE(const Float_t photlen) {

 Int_t i;
 Float_t draw;

 Int_t bins = ((HRichDigitisationPar*)fDigitisationPar)->getQEBinsNr();
 Float_t* photlength = ((HRichDigitisationPar*)fDigitisationPar)->getPhotonLenArray();
 Float_t* photeffic = ((HRichDigitisationPar*)fDigitisationPar)->getPhotonEffArray();

 draw = rand()/(double)RAND_MAX;

 for (i = 0; i < bins; i++)
  if (i == 0) {
   if (photlen < ((photlength[i]+photlength[i+1])/2))
    if (draw > photeffic[i]) { return kFALSE; } 
     else { return kTRUE; }
  } else if (i == bins-1) {
   if (photlen >= ((photlength[i-1]+photlength[i])/2))
    if (draw > photeffic[i]) { return kFALSE; } 
     else { return kTRUE; }
  } else if (((photlength[i-1]+photlength[i])/2) <= photlen &&
             ((photlength[i]+photlength[i+1])/2) > photlen) {
   if (draw > photeffic[i]) { return kFALSE; } 
    else { return kTRUE; }
  }
 return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Int_t HRichDigitizer::getWireNr(Float_t xhit) {

 Int_t fWiresNr = ((HRichGeometryPar*)fGeometryPar)->getWiresPar()->getWiresNr();
 Float_t fWiresDist = ((HRichGeometryPar*)fGeometryPar)->getWiresPar()->getDistWire();

 Int_t i, nWireNr = -1;
 Float_t Xwir;

 for (i = 0; i < fWiresNr; i++) {
  Xwir = ((HRichGeometryPar*)fGeometryPar)->getWiresPar()->getWire(i)->getXWire();
  if ((xhit >= Xwir - fWiresDist) && (xhit < Xwir + fWiresDist)) {
   nWireNr = i;
   break;
  }
 }

 return nWireNr;
}
//============================================================================

//----------------------------------------------------------------------------
void HRichDigitizer::calcChargeOnWire(Int_t sector, Float_t xhit, Float_t yhit) {

 Int_t fHitedWireNr;
 Float_t fTempStack, fDraw;
 Float_t fX, fY, fQ;
 HRichFrame *pFrame = ((HRichGeometryPar*)fGeometryPar)->getFramePar();

  fX = fY = fQ = 0.0;
  fHitedWireNr = getWireNr(xhit);
  if (fHitedWireNr == -1) return;


  fX  = ((HRichGeometryPar*)fGeometryPar)->getWiresPar()
             ->getWire(fHitedWireNr)->getXWire();
  fY  = yhit;

  fDraw = rand()/(double)RAND_MAX;
  fTempStack = exp(-fQlower/fQmean)-fDraw*
               (exp(-fQlower/fQmean)-exp(-fQupper/fQmean));

  if (fTempStack < 1.e-12) {
   fQ = fQupper;
  } else {
   fQ = -fQmean*log(fTempStack);
  }

  if (fQ > 0. && pFrame->isOut(fX, fY) == kFALSE) {
   fChargeOnWireList.Add(new HRichWireSignal(sector,fHitedWireNr,fQ,fX,fY));
  }

}
//============================================================================

//----------------------------------------------------------------------------
void HRichDigitizer::digitiseCherenkovHits(HGeantRichPhoton *pCerHit) {

 HRichFrame *pFrame = ((HRichGeometryPar*)fGeometryPar)->getFramePar();

 if (calcQE(pCerHit->getEnergy()) == kTRUE &&
     // one day it will be additional condition if it is mirror or detector
     pFrame->isOut(pCerHit->getX(), pCerHit->getY()) == kFALSE)
      calcChargeOnWire(pCerHit->getSector(), pCerHit->getX(), pCerHit->getY());

}
//============================================================================

//----------------------------------------------------------------------------
void HRichDigitizer::digitiseDirectHits(HGeantRichDirect *pDirHit) {

 Int_t i, fStepsNr;
 Float_t fStepLength, fNewX, fNewY;
 HRichFrame *pFrame = ((HRichGeometryPar*)fGeometryPar)->getFramePar();

 if (pDirHit->getEnergyLoss() > 0. &&
     // one day it will be additional condition if it is mirror or detector
     pFrame->isOut(pDirHit->getX(), pDirHit->getY()) == kFALSE) {

  fStepsNr = (Int_t)(1+((HRichDigitisationPar*)fDigitisationPar)
                         ->getElectronsNr()*pDirHit->getEnergyLoss());
  if (fStepsNr > 5000) {
   fStepsNr = 5000;
   cerr << "Warning in <HRichDigitizer::digitiseDirectHits>: "
        << "Number of maximum steps exceeded!\n";
  }
  fStepLength = pDirHit->getTrackLengthInPhotDet()/((Float_t)fStepsNr);

  for (i = 0; i < fStepsNr; i++) {
   fNewX = pDirHit->getX() + (i+1) * fStepLength * 
           sin(pDirHit->getTheta()) * cos(pDirHit->getPhi());
   fNewY = pDirHit->getY() + (i+1) * fStepLength * 
           sin(pDirHit->getTheta()) * sin(pDirHit->getPhi());
   calcChargeOnWire(pDirHit->getSector(), fNewX, fNewY);
  }

 }

}
//============================================================================

//----------------------------------------------------------------------------
void HRichDigitizer::calcLimits(HRichPad *pPad, Int_t *Xd, Int_t *Yd, Int_t *Xu, Int_t *Yu) {

// remember that pad coordinates must be after translation to hit coordinates

 *Xd = 0;
 *Yd = 0;
 *Xu = 0;
 *Yu = 0;

 Int_t k;
 Float_t fXmin, fYmin, fXmax, fYmax;

 Int_t fMatrixPadRange = 
     ((HRichDigitisationPar*)fDigitisationPar)->getMatrixPadRange();
 Float_t fMatrixPadSize = 
     ((HRichDigitisationPar*)fDigitisationPar)->getMatrixPadSize();

 if (pPad->getCornersNr() > 0) {

  fXmin = pPad->getXmin();
  fYmin = pPad->getYmin();
  fXmax = pPad->getXmax();
  fYmax = pPad->getYmax();

  k = (Int_t)((fabs(fXmin) + 0.5*fMatrixPadSize)/fMatrixPadSize);
  if (fXmin < 0) 
   *Xd = (k > fMatrixPadRange) ? 0 : fMatrixPadRange - k;
  else
   *Xd = (k > fMatrixPadRange) ? 2*fMatrixPadRange : fMatrixPadRange + k;

  k = (Int_t)((fabs(fYmin) + 0.5*fMatrixPadSize)/fMatrixPadSize);
  if (fYmin < 0)
   *Yd = (k > fMatrixPadRange) ? 0 : fMatrixPadRange - k;
  else
   *Yd = (k > fMatrixPadRange) ? 2*fMatrixPadRange : fMatrixPadRange + k;

  k = (Int_t)((fabs(fXmax) + 0.5*fMatrixPadSize)/fMatrixPadSize);
  if (fXmax < 0)
   *Xu = (k > fMatrixPadRange) ? 0 : fMatrixPadRange - k;
  else 
   *Xu = (k > fMatrixPadRange) ? 2*fMatrixPadRange : fMatrixPadRange + k;

  k = (Int_t)((fabs(fYmax) + 0.5*fMatrixPadSize)/fMatrixPadSize);
  if (fYmax < 0)
   *Yu = (k > fMatrixPadRange) ? 0 : fMatrixPadRange - k;
  else 
   *Yu = (k > fMatrixPadRange) ? 2*fMatrixPadRange : fMatrixPadRange + k;
 }

}
//============================================================================

//----------------------------------------------------------------------------
HRichPad* HRichDigitizer::translateCorners(HRichPad *pPad, Float_t dx, Float_t dy) {

// check what you want to translate - probably real corners

 Int_t i;
 HRichPad *fpTranslatedPad = new HRichPad(*pPad);

 for (i = 0; i < fpTranslatedPad->getCornersNr(); i++) {
  fpTranslatedPad->getCorner(i)->setX(fpTranslatedPad->getCorner(i)->getX() - dx);
  fpTranslatedPad->getCorner(i)->setY(fpTranslatedPad->getCorner(i)->getY() - dy);
 }
 fpTranslatedPad->setXmin(fpTranslatedPad->getXmin() - dx);
 fpTranslatedPad->setYmin(fpTranslatedPad->getYmin() - dy);
 fpTranslatedPad->setXmax(fpTranslatedPad->getXmax() - dx);
 fpTranslatedPad->setYmax(fpTranslatedPad->getYmax() - dy);

 // the ownership of this pointer is passed to outer variable-pointer
 return fpTranslatedPad;
}
//============================================================================

//----------------------------------------------------------------------------
void HRichDigitizer::translateCorners(HRichPad *pPad, Float_t dx, Float_t dy, 
                     Float_t *Xd, Float_t *Yd, Float_t *Xu, Float_t *Yu) {

 *Xd = pPad->getXmin() - dx;
 *Yd = pPad->getYmin() - dy;
 *Xu = pPad->getXmax() - dx;
 *Yu = pPad->getYmax() - dy;
}
//============================================================================

//----------------------------------------------------------------------------
void HRichDigitizer::makeNoiseOnPads() {

 Int_t i, j;
 Float_t fCharge;
 HRichCal *pCal = NULL;


 for (i = 0; i < 6; i++)
  if (((HRichGeometryPar*)getGeometryPar())->getSector(i) > 0) 
   for (j = 0; j < ((HRichGeometryPar*)getGeometryPar())->getPadsNr(); j++)
    if (((HRichGeometryPar*)getGeometryPar())->getPadStatus(j)) {

     fCharge = GaussFun(0.0, fSigmaValue);    
     if (fCharge > fSigmaValue) {
      fCharge /= fChargePerChannel;
      loc.set(3, i, ((HRichGeometryPar*)getGeometryPar())->calcRow(j), 
                    ((HRichGeometryPar*)getGeometryPar())->calcCol(j));
      pCal = (HRichCal*)((HMatrixCategory*)getOutputCat())->getObject(loc);
      if (pCal == NULL) {
       pCal = (HRichCal*)((HMatrixCategory*)getOutputCat())->getSlot(loc);
       if (pCal != NULL) {
        pCal = new(pCal) HRichCal;
        pCal->setEventNr(lNrEvent);
        pCal->setSector(loc[0]);
        pCal->setCol(loc[2]);
        pCal->setRow(loc[1]);
       }
      }
      if (pCal != NULL) pCal->addCharge(fCharge);
     }
    } // eof loop over all pads
}
//============================================================================

//----------------------------------------------------------------------------
void HRichDigitizer::digitisePads() {

 Int_t i, j, xcenter, ycenter;
 Int_t sector;
 HRichPadTab *pPadsPar = ((HRichGeometryPar*)fGeometryPar)->getPadsPar();
 //HRichFrame *pFrame = ((HRichGeometryPar*)fGeometryPar)->getFramePar();
 HRichPad *pPad = NULL;
 HRichPad *pTmpPad = NULL;
 HRichWireSignal *pCharge = NULL;
 Int_t padSide; // 1 for right side, 0 for left side
 Int_t padHalf; // 1 for upper half, 0 for lower half

 Float_t fFactor = 0.;
 
 //makeNoiseOnPads();

 for (i = 0; i < fChargeOnWireList.GetSize(); i++) {
  for (j = 0; j < 9; j++) fDigitPadMatrix[j] = 0.;
  pCharge = (HRichWireSignal*)fChargeOnWireList.At(i);
  sector = pCharge->getSector();
  pPad = pPadsPar->getPad(pCharge->getX(), pCharge->getY());
  xcenter = pPad->getPadX();
  ycenter = pPad->getPadY();

  if ((pPad->getXmax()-pCharge->getX()) < (pCharge->getX()-pPad->getXmin())) {
   padSide = 1;
  } else {
   padSide = 0;
  }

  if ((pPad->getYmax()-pCharge->getY()) < (pCharge->getY()-pPad->getYmin())) {
   padHalf = 1;
  } else {
   padHalf = 0;
  }

  pTmpPad = translateCorners(pPad, pCharge->getX(), pCharge->getY());
  // do not forget to delete it at the end of loop

  // ------------------------ central pad (0,0)
  if (pTmpPad->getPadFlag() == 1) 
   fDigitPadMatrix[4] = analyticalCalc(pTmpPad, fFactor1*pCharge->getCharge());
  else if (pTmpPad->getPadFlag() == 2)
   fDigitPadMatrix[4] = numericalCalc(pTmpPad, fFactor1*pCharge->getCharge());
  fDigitPadMatrix[4] = GaussFun(fDigitPadMatrix[4], fSigmaValue);
  if (fDigitPadMatrix[4] < 0.) fDigitPadMatrix[4] = 0.;
   else updateCharge(sector, pPad, fDigitPadMatrix[4]);

  // ------------------------ pad (0, -1)
  if (pPadsPar->getPad(xcenter, ycenter-1) &&
      pPadsPar->getPad(xcenter, ycenter-1)->getPadActive()) {
   delete pTmpPad;
   pTmpPad = translateCorners(pPadsPar->getPad(xcenter, ycenter-1),
                              pCharge->getX(), pCharge->getY());
   fFactor = (1./(fParam1 * exp(fabs(fParam2 * 
              (pTmpPad->getYmax() + pTmpPad->getYmin())))));
   if (padHalf) {
    if (pTmpPad->getPadFlag() == 1)
     fDigitPadMatrix[1] = analyticalCalc(pTmpPad, fFactor*pCharge->getCharge());
    else if (pTmpPad->getPadFlag() == 2)
     fDigitPadMatrix[1] = numericalCalc(pTmpPad, fFactor*pCharge->getCharge());
   } else {
    if (pTmpPad->getPadFlag() == 1)
     fDigitPadMatrix[1] = analyticalCalc(pTmpPad, fFactor2*pCharge->getCharge());
    else if (pTmpPad->getPadFlag() == 2)
     fDigitPadMatrix[1] = numericalCalc(pTmpPad, fFactor2*pCharge->getCharge());
   }
   fDigitPadMatrix[1] = GaussFun(fDigitPadMatrix[1], fSigmaValue);
   if (fDigitPadMatrix[1] < 0.) fDigitPadMatrix[1] = 0.;
    else updateCharge(sector, pTmpPad, fDigitPadMatrix[1]);
  }

  // ------------------------ pad (0, 1)
  if (pPadsPar->getPad(xcenter, ycenter+1) &&
      pPadsPar->getPad(xcenter, ycenter+1)->getPadActive()) {
   delete pTmpPad;
   pTmpPad = translateCorners(pPadsPar->getPad(xcenter, ycenter+1),
                              pCharge->getX(), pCharge->getY());
   fFactor = (1./(fParam1 * exp(fabs(fParam2 *
              (pTmpPad->getYmax() + pTmpPad->getYmin())))));
   if (!padHalf) {
    if (pTmpPad->getPadFlag() == 1)
     fDigitPadMatrix[7] = analyticalCalc(pTmpPad, fFactor*pCharge->getCharge());
    else if (pTmpPad->getPadFlag() == 2)
     fDigitPadMatrix[7] = numericalCalc(pTmpPad, fFactor*pCharge->getCharge());
   } else {
    if (pTmpPad->getPadFlag() == 1)
     fDigitPadMatrix[7] = analyticalCalc(pTmpPad, fFactor2*pCharge->getCharge());
    else if (pTmpPad->getPadFlag() == 2)
     fDigitPadMatrix[7] = numericalCalc(pTmpPad, fFactor2*pCharge->getCharge());
   }
   fDigitPadMatrix[7] = GaussFun(fDigitPadMatrix[7], fSigmaValue);
   if (fDigitPadMatrix[7] < 0.) fDigitPadMatrix[7] = 0.;
    else updateCharge(sector, pTmpPad, fDigitPadMatrix[7]);
  }

  // ------------------------ pad (-1, 0)
  if (pPadsPar->getPad(xcenter-1, ycenter) &&
      pPadsPar->getPad(xcenter-1, ycenter)->getPadActive()) {
   delete pTmpPad;
   pTmpPad = translateCorners(pPadsPar->getPad(xcenter-1, ycenter),
                              pCharge->getX(), pCharge->getY());
   if (padSide) {
    fDigitPadMatrix[3] = GaussFun(fFactor3*fDigitPadMatrix[4], fSigmaValue);
   } else {
    fDigitPadMatrix[3] = GaussFun(fFactor4*fDigitPadMatrix[4], fSigmaValue);
   }
   if (fDigitPadMatrix[3] < 0.) fDigitPadMatrix[3] = 0.;
    else updateCharge(sector, pTmpPad, fDigitPadMatrix[3]);
  }

  // ------------------------ pad (1, 0)
  if (pPadsPar->getPad(xcenter+1, ycenter) &&
      pPadsPar->getPad(xcenter+1, ycenter)->getPadActive()) {
   delete pTmpPad;
   pTmpPad = translateCorners(pPadsPar->getPad(xcenter+1, ycenter),
                              pCharge->getX(), pCharge->getY());
   if (padSide) {
    fDigitPadMatrix[5] = GaussFun(fFactor4*fDigitPadMatrix[4], fSigmaValue);
   } else {
    fDigitPadMatrix[5] = GaussFun(fFactor3*fDigitPadMatrix[4], fSigmaValue);
   }
   if (fDigitPadMatrix[5] < 0.) fDigitPadMatrix[5] = 0.;
    else updateCharge(sector, pTmpPad, fDigitPadMatrix[5]);
  }

  // ------------------------ pad (-1, -1)
  if (pPadsPar->getPad(xcenter-1, ycenter-1) &&
      pPadsPar->getPad(xcenter-1, ycenter-1)->getPadActive()) {
   delete pTmpPad;
   pTmpPad = translateCorners(pPadsPar->getPad(xcenter-1, ycenter-1),
                              pCharge->getX(), pCharge->getY());
   if (padSide) {
    fDigitPadMatrix[0] = GaussFun(fFactor5*fDigitPadMatrix[1], fSigmaValue);
    if (padHalf) fDigitPadMatrix[0] += GaussFun(fFactor6*fDigitPadMatrix[3], fSigmaValue);
   } else {
    fDigitPadMatrix[0] = GaussFun(fFactor7*fDigitPadMatrix[1], fSigmaValue);
    if (padHalf) fDigitPadMatrix[0] += GaussFun(fFactor8*fDigitPadMatrix[3], fSigmaValue);
   }
   if (fDigitPadMatrix[0] < 0.) fDigitPadMatrix[0] = 0.;
    else updateCharge(sector, pTmpPad, fDigitPadMatrix[0]);
  }

  // ------------------------ pad (1, -1)
  if (pPadsPar->getPad(xcenter+1, ycenter-1) &&
      pPadsPar->getPad(xcenter+1, ycenter-1)->getPadActive()) {
   delete pTmpPad;
   pTmpPad = translateCorners(pPadsPar->getPad(xcenter+1, ycenter-1),
                              pCharge->getX(), pCharge->getY());
   if (padSide) {
    fDigitPadMatrix[2] = GaussFun(fFactor7*fDigitPadMatrix[1], fSigmaValue);
    if (padHalf) fDigitPadMatrix[2] += GaussFun(fFactor8*fDigitPadMatrix[5], fSigmaValue);   } else {
    fDigitPadMatrix[2] = GaussFun(fFactor5*fDigitPadMatrix[1], fSigmaValue);
    if (padHalf) fDigitPadMatrix[2] += GaussFun(fFactor6*fDigitPadMatrix[5], fSigmaValue);   }
   if (fDigitPadMatrix[2] < 0.) fDigitPadMatrix[2] = 0.;
    else updateCharge(sector, pTmpPad, fDigitPadMatrix[2]);
  }

  // ------------------------ pad (-1, 1)
  if (pPadsPar->getPad(xcenter-1, ycenter+1) &&
      pPadsPar->getPad(xcenter-1, ycenter+1)->getPadActive()) {
   delete pTmpPad;
   pTmpPad = translateCorners(pPadsPar->getPad(xcenter-1, ycenter+1),
                              pCharge->getX(), pCharge->getY());
   if (padSide) {
    fDigitPadMatrix[6] = GaussFun(fFactor5*fDigitPadMatrix[7], fSigmaValue);
    if (!padHalf) fDigitPadMatrix[6] += GaussFun(fFactor6*fDigitPadMatrix[3], fSigmaValue);   } else {
    fDigitPadMatrix[6] = GaussFun(fFactor7*fDigitPadMatrix[7], fSigmaValue);
    if (!padHalf) fDigitPadMatrix[6] += GaussFun(fFactor8*fDigitPadMatrix[3], fSigmaValue);   }
   if (fDigitPadMatrix[6] < 0.) fDigitPadMatrix[6] = 0.;
    else updateCharge(sector, pTmpPad, fDigitPadMatrix[6]);
  }

  // ------------------------ pad (1, 1)
  if (pPadsPar->getPad(xcenter+1, ycenter+1) &&
      pPadsPar->getPad(xcenter+1, ycenter+1)->getPadActive()) {
   delete pTmpPad;
   pTmpPad = translateCorners(pPadsPar->getPad(xcenter+1, ycenter+1),
                              pCharge->getX(), pCharge->getY());
   if (padSide) {
    fDigitPadMatrix[8] = GaussFun(fFactor7*fDigitPadMatrix[7], fSigmaValue);
    if (!padHalf) fDigitPadMatrix[8] += GaussFun(fFactor8*fDigitPadMatrix[5], fSigmaValue);   } else {
    fDigitPadMatrix[8] = GaussFun(fFactor5*fDigitPadMatrix[7], fSigmaValue);
    if (!padHalf) fDigitPadMatrix[8] += GaussFun(fFactor6*fDigitPadMatrix[5], fSigmaValue);   }
   if (fDigitPadMatrix[8] < 0.) fDigitPadMatrix[8] = 0.;
    else updateCharge(sector, pTmpPad, fDigitPadMatrix[8]);
  }


  delete pTmpPad;

 } // eof loop on all charges on wires 
}
//============================================================================

//----------------------------------------------------------------------------
void HRichDigitizer::updateCharge(Int_t sector, HRichPad* pPad, Float_t charge) {

 Int_t fCol, fRow;
 HRichCal *pCal = NULL;
 
 pPad->getPadXY(&fCol, &fRow);
 loc.set(3, sector, fRow, fCol);
 pCal = (HRichCal*)((HMatrixCategory*)getOutputCat())->getObject(loc);
 if (pCal == NULL) {
  pCal = (HRichCal*)((HMatrixCategory*)getOutputCat())->getSlot(loc);
  if (pCal != NULL) {
   pCal = new(pCal) HRichCal;
    pCal->setEventNr(lNrEvent);
    pCal->setSector(loc[0]);
    pCal->setCol(loc[2]);
    pCal->setRow(loc[1]);
  }
 }
 if (pCal != NULL)
  pCal->addCharge(charge / fChargePerChannel);
}
//============================================================================

//----------------------------------------------------------------------------
Float_t HRichDigitizer::analyticalCalc(HRichPad* pPad, Float_t wirecharge) {

// remember that pad must be after translation to hit coordinates 

  if (pPad->getPadFlag() != 1) 
   cerr << "Warning in <HRichDigitizer::analyticalCalc>: "
        << "Analytical calculation forced for not rectangular pad!\n";

  Float_t charge = 0.;
  Float_t fDist = ((HRichGeometryPar*)fGeometryPar)->getDistWiresPads();

  charge =  calcCharge(wirecharge, fDist, 
            pPad->getXmin(), pPad->getYmin(), pPad->getXmax(), pPad->getYmax());

 return charge;
}
//============================================================================

//----------------------------------------------------------------------------
Float_t HRichDigitizer::numericalCalc(HRichPad* pPad, Float_t wirecharge) {

// remember that pad must be after translation to hit coordinates

  Float_t charge = 0.;
  Float_t xpoint, ypoint;
  Int_t i, j;
  Int_t limXd, limYd, limXu, limYu;

  Int_t fMatrixPadRange = 
      ((HRichDigitisationPar*)fDigitisationPar)->getMatrixPadRange();
  Float_t fMatrixPadSize = 
      ((HRichDigitisationPar*)fDigitisationPar)->getMatrixPadSize();

  calcLimits(pPad, &limXd, &limYd, &limXu, &limYu);

  for (j = limYd; j <= limYu; j++)
   for (i = limXd; i <= limXu; i++) {
    xpoint = (i - fMatrixPadRange)*fMatrixPadSize;
    ypoint = (j - fMatrixPadRange)*fMatrixPadSize;
    if (pPad->isOut(xpoint,ypoint) == kFALSE)
     charge += wirecharge * fpChargePatternMatrix[i + j*(2*fMatrixPadRange+1)];
   }

 return charge;
}
//============================================================================




