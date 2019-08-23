#include "hshowerhitdigitizer.h"

#include "hcategory.h"
#include "hlinearcategory.h"
#include "hlocation.h"
#include "hshowergeanthit.h"
#include "hshowergeantwire.h"
#include "hshowerdigidetpar.h"
#include "hdebug.h"
#include "hades.h"
#include "showerdef.h"
#include <math.h>
#include <stdlib.h>


ClassImp(HShowerHitDigitizer)

HShowerHitDigitizer::HShowerHitDigitizer(Text_t *name,Text_t *title) :
  HShowerDigitizer(name,title)
{
  piconst = 2*acos(0);
}

HShowerHitDigitizer::HShowerHitDigitizer()
{
  piconst = 2*acos(0);
}


HShowerHitDigitizer::~HShowerHitDigitizer(void) {
}

Bool_t HShowerHitDigitizer::init() {
//    setInCat(gHades->getCurrentEvent()->getCategory(catShowerGeantHit));
//    setOutCat(gHades->getCurrentEvent()->getCategory(catShowerGeantWire));
    return HShowerDigitizer::init();
}

HShowerHitDigitizer& HShowerHitDigitizer::operator=(HShowerHitDigitizer &c) {
// It should have been done
  return c;
}

Bool_t HShowerHitDigitizer::digitize(HDataObject *pHit) {
  HLocation fLoc;
  Bool_t nResult;

#if DEBUG_LEVEL>2
  gDebuger->enterFunc("HShowerHitDigitizer::execute");
  gDebuger->message("Raw cat points to %p",fRawCat);
#endif

  HShowerGeantHit *pGHit = (HShowerGeantHit*) pHit;
  HShowerDigiDetPar *pDetPar = (HShowerDigiDetPar*) getDigiParSet();

  if (!pDetPar)
  {
    Error("HShowerHitDigitizer::digitize", "Digitazation parameters not initialised !!");
    return 0;
  }

  if (pGHit) {
//////////////////////////////////////////////////////////
    
     nResult = digiHits(pGHit);

/////////////////////////////////////////////////////////
  }

#if DEBUG_LEVEL>2
  gDebuger->leaveFunc("HShowerHitDigitizer::execute");
#endif

  return nResult;
}

Bool_t HShowerHitDigitizer::digiHits(HShowerGeantHit *simhit) {
 HLocation fLoc;
 HShowerGeantWire *pGeantWire=NULL;
 float x_exit, y_exit;

 Int_t nDet, nSector;
 Int_t nFiredWire, nFiredWireIn, nFiredWireOut;
 Float_t fTheta, fPhi, fBeta, fCorr, fSin2Theta;
 Float_t fX, fY, fDX, fDY;
 Float_t fNewX, fNewY;
 Float_t fQ, fThick, fVoltage;

 nDet = simhit->getDetector();
 nSector = simhit->getSector();
 simhit->getXY(&fX, &fY);
 simhit->getAngles(&fTheta, &fPhi);
 fBeta = simhit->getBeta();
 
 HShowerWireTab *phWires = ((HShowerDigiDetPar*)getDigiParSet())->
                                  getWireTab(nDet);
 fVoltage = phWires->getVoltage();
 fThick = ((HShowerDigiDetPar*)getDigiParSet())->getThickDet();

 fSin2Theta = sin(fTheta)*sin(fTheta);
 fCorr = 0.5*fSin2Theta*sin(2*fPhi) / (fSin2Theta*cos(fPhi)*cos(fPhi) + 1 - fSin2Theta);

 x_exit = fX + fThick*tan(fTheta)*cos(fPhi);
 y_exit = fY + fThick*tan(fTheta)*sin(fPhi);

 nFiredWireIn = phWires->lookupWire(fY);
 if (nFiredWireIn == -1) return 0;

 nFiredWireOut = phWires->lookupWire(y_exit);
 if (nFiredWireOut == -1) return 0;

 fDX = (x_exit - fX) / (nFiredWireOut - nFiredWireIn + 1);
 fDY = (y_exit - fY) / (nFiredWireOut - nFiredWireIn + 1);

 for(nFiredWire = nFiredWireIn; nFiredWire <= nFiredWireOut; nFiredWire++)
 {
// fNewX = fX + fDX * (0.5 + nFiredWire - nFiredWireIn);
   fNewX = fX + fDX * (0.5 + nFiredWire - nFiredWireIn) + fCorr * 0.5 * fDY; 
   fNewY = phWires->getWire(nFiredWire)->getYWire();

   fQ = calcHit(fBeta, fVoltage);
   pGeantWire = (HShowerGeantWire*)((HLinearCategory*)getOutCat())
                                                        ->getNewSlot(fLoc);
   if (pGeantWire != NULL) {
       pGeantWire = new(pGeantWire) HShowerGeantWire;
       pGeantWire->setWireNr(nFiredWire);
       pGeantWire->setXY(fNewX, fNewY);
       pGeantWire->setCharge(fQ);
       pGeantWire->setModule(nDet);
       pGeantWire->setSector(nSector);
   }
 }

 return 1;
}

Float_t HShowerHitDigitizer::calcQMean(float fBeta, float fVoltage)
{
   Float_t fQmean;
   if (fVoltage<2500 || fVoltage>3500) {
    cerr << "Voltage is out of range..... " << fVoltage << endl;
    exit(1);
    return 0;
  }

  fQmean = 4.178*exp(-1.719*fBeta);
  return fQmean;
}


Float_t HShowerHitDigitizer::calcHit(Float_t fBeta, Float_t fVoltage) {
 float tempStack, fQ, Qlower, Qupper;

 Float_t fQmean = calcQMean(fBeta, fVoltage);
 Float_t fFwhmbroad = ((HShowerDigiDetPar*)getDigiParSet())->getFwhmResponse();
 Int_t nNumElec = ((HShowerDigiDetPar*)getDigiParSet())->getNumElec();

 while (!(Qlower = rand()/(double)RAND_MAX));

 Qupper = rand()/(double)RAND_MAX;
 tempStack = (1/(2.35/fFwhmbroad))*sqrt(-2*log(Qlower))*cos(2*piconst*Qupper);
 fQ = fQmean + fQmean*tempStack;

 return fQ * nNumElec; //liczba electronow pierwotnych
}













