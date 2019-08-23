#include <iostream.h>
#include <TClonesArray.h>
#include "hades.h"
#include "hrichdetector.h"
#include "hspectrometer.h"
#include "hrichpad.h"


ClassImp(HRichPadCorner)
ClassImp(HRichPad)
ClassImp(HRichPadTab)


//----------------------------------------------------------------------------
HRichPadCorner::HRichPadCorner() {
 fXcor = 0.0;
 fYcor = 0.0;
 fCornerNr = 0;
 fAreaFlag = 0;
}
//============================================================================

//----------------------------------------------------------------------------
HRichPadCorner::~HRichPadCorner() {}
//============================================================================

//----------------------------------------------------------------------------
HRichPadCorner::HRichPadCorner(const HRichPadCorner& source) {
 fXcor = source.fXcor;
 fYcor = source.fYcor;
 fCornerNr = source.fCornerNr;
 fAreaFlag = source.fAreaFlag;
}
//============================================================================

//----------------------------------------------------------------------------
HRichPadCorner& HRichPadCorner::operator=(const HRichPadCorner& source) {
 if (this != &source) {
  fXcor = source.fXcor;
  fYcor = source.fYcor;
  fCornerNr = source.fCornerNr;
  fAreaFlag = source.fAreaFlag;
 }
return *this;
}
//============================================================================



//----------------------------------------------------------------------------
HRichPad::HRichPad() {
 fCornersNr = 0;
 fRealCornersNr = 0;
 fPadX = 0;
 fPadY = 0;
 fPadNr = 0;
 fPadFlag = 0;
 fPadActive = kFALSE;
 fPadAmplitude = 0;
 fPadLabel = 0;
 fPadLock = 0;
 fLinkedPadsNr = 0;

 fLinkedPads = NULL;
}
//============================================================================

//----------------------------------------------------------------------------
HRichPad::~HRichPad() {
 reset();
} 
//============================================================================

//----------------------------------------------------------------------------
HRichPad::HRichPad(const HRichPad& source)
{
 // ============= NOT READY
 Int_t i;
 //for (i = 0; i < source.fCornersNr; i++) 
 // addCorner(new HRichPadCorner(*source.getCorner(i)));
 //for (i = 0; i < source.fRealCornersNr; i++) 
 // addRealCorner(new HRichPadCorner(*source.getRealCorner(i)));
 
 fPadX = source.fPadX;
 fPadY = source.fPadY;
 fPadNr = source.fPadNr;
 fPadFlag = source.fPadFlag;
 fPadActive = source.fPadActive;
 fPadLabel = source.fPadLabel;
 fPadLock = source.fPadLock;
 fLinkedPadsNr = source.fLinkedPadsNr;
 if (fLinkedPadsNr > 0) {
  fLinkedPads  = new (HRichPad*)[fLinkedPadsNr];
  for (i = 0; i < fLinkedPadsNr; i++)
   fLinkedPads[i] = source.fLinkedPads[i];
 } else fLinkedPads = NULL;
}

//============================================================================

//----------------------------------------------------------------------------
HRichPad& HRichPad::operator=(const HRichPad& source)
{
 if (this != &source) {
  Int_t i;
  // ============= NOT READY
  if (fLinkedPadsNr > 0 && fLinkedPads) delete [] fLinkedPads;
  //for (i = 0; i < source.fCornersNr; i++) 
  // addCorner(new HRichPadCorner(*source.getCorner(i)));
  //for (i = 0; i < source.fRealCornersNr; i++)
  // addRealCorner(new HRichPadCorner(*source.getRealCorner(i)));
  
  fPadX = source.fPadX;
  fPadY = source.fPadY;
  fPadNr = source.fPadNr;
  fPadFlag = source.fPadFlag;
  fPadActive = source.fPadActive;
  fPadLabel = source.fPadLabel;
  fPadLock = source.fPadLock;
  fLinkedPadsNr = source.fLinkedPadsNr;
  if (fLinkedPadsNr > 0) {
   fLinkedPads  = new (HRichPad*)[fLinkedPadsNr];
   for (i = 0; i < fLinkedPadsNr; i++)
    fLinkedPads[i] = source.fLinkedPads[i];
  } else fLinkedPads = NULL;
 }
return *this;
}
//============================================================================

//----------------------------------------------------------------------------
void HRichPad::reset()
{
 fCornersNr = 0;
 fRealCornersNr = 0;
 fPadX = 0;
 fPadY = 0;
 fPadNr = 0;
 fPadFlag = 0;
 fPadActive = kFALSE;
 fPadAmplitude = 0;
 fPadLabel = 0;
 fPadLock = 0;
 fLinkedPadsNr = 0;
 fPadCornersArray.Delete();
 fPadRealCornersArray.Delete();
 if (fLinkedPads) {
  delete [] fLinkedPads;
  // only array of connections should be deleted, not the objects!
  fLinkedPads = NULL;
 }
}
//============================================================================

//----------------------------------------------------------------------------
void HRichPad::clear() {
 fPadAmplitude = 0;
 fPadLabel = 0;
 fPadLock = 0;
}
//============================================================================

//----------------------------------------------------------------------------
void HRichPad::CalcNrtoXY(Int_t padsx) {
 fPadX = fPadNr % padsx;
 fPadY = fPadNr / padsx;
}
//============================================================================

//----------------------------------------------------------------------------
void HRichPad::CalcXYtoNr(Int_t padsx) {
 fPadNr = fPadX + padsx*fPadY;
}
//============================================================================

//----------------------------------------------------------------------------
void HRichPad::getPadXY(Int_t *pX, Int_t *pY) {
 *pX = fPadX;
 *pY = fPadY;
}
//============================================================================

//----------------------------------------------------------------------------
HRichPadCorner* HRichPad::getCorner(Int_t n) {
 if (n < 0 || n >= fCornersNr) return NULL;
 return (HRichPadCorner*)fPadCornersArray.At(n);
}
//============================================================================

//----------------------------------------------------------------------------
Int_t HRichPad::addCorner(HRichPadCorner* corner) {
 fPadCornersArray.Add(corner);
 return fCornersNr++;
}
//============================================================================

//----------------------------------------------------------------------------
Int_t HRichPad::setCorner(HRichPadCorner* corner, Int_t n) {
 if (n < 0 || n >= fCornersNr) return 0;
 delete fPadCornersArray.At(n);
 fPadCornersArray.AddAt(corner, n);
 return 1;
}
//============================================================================

//----------------------------------------------------------------------------
HRichPadCorner* HRichPad::getRealCorner(Int_t n) {
 if (n < 0 || n >= fRealCornersNr) return NULL;
 return (HRichPadCorner*)fPadRealCornersArray.At(n);
}
//============================================================================

//----------------------------------------------------------------------------
Int_t HRichPad::addRealCorner(HRichPadCorner* corner) {
 fPadRealCornersArray.Add(corner);
 return fRealCornersNr++;
}
//============================================================================

//----------------------------------------------------------------------------
Int_t HRichPad::setRealCorner(HRichPadCorner* corner, Int_t n) {
 if (n < 0 || n >= fRealCornersNr) return 0;
 delete fPadRealCornersArray.At(n);
 fPadRealCornersArray.AddAt(corner, n);
 return 1;
}
//============================================================================

//----------------------------------------------------------------------------
void HRichPad::getPadCenter(Float_t *pfX, Float_t *pfY) {

 Int_t i;
 Float_t xcor = 0., ycor = 0., xsum = 0., ysum = 0.;

 for (i = 0; i < fCornersNr; i++) {
  getCorner(i)->getXY(&xcor, &ycor);
  xsum += xcor;
  ysum += ycor;
 }
 *pfX = xsum / fCornersNr;
 *pfY = ysum / fCornersNr;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HRichPad::isOut(Float_t x, Float_t y) {
 Int_t i, areaFlag1;
 Float_t xcor1, ycor1, xcor2, ycor2, a , b;

 for (i = 0; i < fCornersNr; i++) {

  getCorner(i)->getXY(&xcor1, &ycor1);
  areaFlag1 = getCorner(i)->getAreaFlag();
  if (i+1 < fCornersNr) getCorner(i+1)->getXY(&xcor2, &ycor2);
   else getCorner(0)->getXY(&xcor2, &ycor2);

  if (xcor1 == xcor2) {

    if (areaFlag1 > 0) {
     if (x < xcor1) return kTRUE;
    } else {
     if (x > xcor1) return kTRUE;
    }

  } else if (ycor1 == ycor2) {

    if (areaFlag1 > 0) {
     if (y > ycor1) return kTRUE;
    } else {
     if (y < ycor1) return kTRUE;
    }

  } else {

    a = (ycor2 - ycor1)/(xcor2 - xcor1);
    b = (xcor2*ycor1 - xcor1*ycor2)/(xcor2 - xcor1);

    if (areaFlag1 > 0) {
     if (y < a*x + b) return kTRUE;
    } else {
     if (y > a*x + b) return kTRUE;
    }

  }

 } // end of loop over all corners

return kFALSE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HRichPad::isOutX(Float_t x) {

 Int_t i, areaFlag1;
 Float_t xcor1, ycor1, xcor2, ycor2;

 for (i = 0; i < fCornersNr; i++) {

  getCorner(i)->getXY(&xcor1, &ycor1);
  if (i+1 < fCornersNr) getCorner(i+1)->getXY(&xcor2, &ycor2);
   else getCorner(0)->getXY(&xcor2, &ycor2);

  if (xcor1 == xcor2) {
   areaFlag1 = getCorner(i)->getAreaFlag();
   if (areaFlag1 > 0) {
    if (x < xcor1) return kTRUE;
   } else {
    if (x > xcor1) return kTRUE;
   }
  }

 } // end of loop over all corners

return kFALSE;
}
//============================================================================

//----------------------------------------------------------------------------
HRichPadTab::HRichPadTab() {
  reset();
}
//============================================================================

//----------------------------------------------------------------------------
HRichPadTab::~HRichPadTab() {
  deletePads();
}
//============================================================================

//----------------------------------------------------------------------------
void HRichPadTab::reset() {
  fPadsArray = NULL;
  fPadsNr = 0;
  fPadsX = 0;
  fPadsY = 0;
  fPadsLongestRow = 0;
  fPadsLongestRowMiddle = 0.;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HRichPadTab::initAscii(HParHadAsciiFileIo* pHadAsciiFile) {

 if (!pHadAsciiFile) return kFALSE;

 HRichDetector *pRichDet = (HRichDetector*)gHades->getSetup()->getDetector("Rich");

 HAsciiKey* pHAscii = pHadAsciiFile->GetKeyAscii();
 HAsciiKey &mydata = *pHAscii;

 cout << "Reading of pads parameters .... start!\n";
 if (mydata.SetActiveSection("Rich Sector Pads parameters") == NULL)
   return kFALSE;

  Int_t i,j,k,l, loopend, xpad, ypad;

  fPadsX = pRichDet->getColumns();
  fPadsY = pRichDet->getRows();
  fPadsNr = fPadsX*fPadsY;
  createPads(fPadsX, fPadsY);

  for (i = 0; i < fPadsNr; i++) {
   HRichPad *pad = new HRichPad;
   pad->setPadNr(i);
   pad->CalcNrtoXY(fPadsX);
   pad->setPadActive(0);
   pad->setPadFlag(2); // zur Zeit immer so
   setPad(pad, i);
   delete pad;
  }

  // for (i = 0; i < fPadsNr; i++) {
  while (mydata.IsNextKeyword() == kTRUE) {
    xpad = mydata.ReadInt("*", 0);
    ypad = mydata.ReadInt(1);
    HRichPad *pad = getPad(xpad, ypad);
    pad->setPadActive(mydata.ReadInt("*"));
    // pad->setPadFlag(2); // zur Zeit immer so
    j = 0;
    pad->setLinkedPadsNr(mydata.ReadInt("*", j++));
    if (pad->getLinkedPadsNr() > 0) {
     pad->fLinkedPads = new (HRichPad*)[pad->getLinkedPadsNr()];
     for (k = 0; k < pad->getLinkedPadsNr(); k++) 
      pad->fLinkedPads[k] = getPad(mydata.ReadInt(j++));
    }
    k = l = 0;
    loopend = (Int_t)mydata.ReadFloat("*", k++);
    for (j = 0; j < loopend; j++) {
     HRichPadCorner *padcorner = new HRichPadCorner;
     padcorner->setX(mydata.ReadFloat(k++));
     padcorner->setY(mydata.ReadFloat(k++));
     padcorner->setAreaFlag((Int_t)mydata.ReadFloat(k++));
     padcorner->setCornerNr(l++);
     pad->addCorner(padcorner);
    }
    k = l = 0;
    loopend = (Int_t)mydata.ReadFloat("*", k++);
    for (j = 0; j < loopend; j++) {
     HRichPadCorner *padcorner = new HRichPadCorner;
     padcorner->setX(mydata.ReadFloat(k++));
     padcorner->setY(mydata.ReadFloat(k++));
     padcorner->setAreaFlag((Int_t)mydata.ReadFloat(k++));
     padcorner->setCornerNr(l++);
     pad->addRealCorner(padcorner);
    }  
  }

 cout << "Reading of pads parameters .... stop!\n";
 return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Int_t HRichPadTab::createPads(Int_t nX, Int_t nY) {
   deletePads();

   fPadsX = nX;
   fPadsY = nY;
   fPadsNr = fPadsX * fPadsY;

   fPadsArray = new TClonesArray("HRichPad", fPadsNr); 

   return (fPadsArray) ? fPadsNr : 0;
}
//============================================================================
    
//----------------------------------------------------------------------------
void HRichPadTab::deletePads() {
   delete fPadsArray;
   reset();
}
//============================================================================

//----------------------------------------------------------------------------
void HRichPadTab::setPad(HRichPad* pPad, Int_t nCol, Int_t nRow)
{
  Int_t nAddr = calcAddr(nCol, nRow);
  setPad(pPad, nAddr);
}
//============================================================================

//----------------------------------------------------------------------------
void HRichPadTab::setPad(HRichPad* pPad, Int_t nAddr) 
{
  HRichPad* pNewPad;

  pNewPad = (HRichPad*)getSlot(nAddr);
  pNewPad = new(pNewPad) HRichPad;
  *pNewPad = *pPad;
}
//============================================================================

//----------------------------------------------------------------------------
HRichPad* HRichPadTab::getPad(Int_t nPadNr) {
  return (HRichPad*)getObject(nPadNr);
}
//============================================================================

//----------------------------------------------------------------------------
HRichPad* HRichPadTab::getPad(Int_t nCol, Int_t nRow) {
  if ((nCol < 0) || (nCol >= fPadsX))
     return NULL;

  if ((nRow < 0) || (nRow >= fPadsY))
     return NULL;

  Int_t nAddr = calcAddr(nCol, nRow);
  return getPad(nAddr);
}
//============================================================================

//----------------------------------------------------------------------------
HRichPad* HRichPadTab::getPad(Float_t  fXpos, Float_t fYpos) {

   Int_t i, j, nPadAddr;
   Int_t nHit = 0;
   
   if (fXpos < fPadsLongestRowMiddle) {
    for (nPadAddr = fPadsLongestRow*fPadsX; 
         nPadAddr < fPadsLongestRow*fPadsX + fPadsX/2; 
         nPadAddr++) {
     if (getPad(nPadAddr)->getPadActive() == kTRUE)
      if (!getPad(nPadAddr)->isOutX(fXpos)) {                    
       nHit = 1;
       break;
      }
    }
   } else {
    for (nPadAddr = fPadsLongestRow*fPadsX + fPadsX/2; 
         nPadAddr < fPadsLongestRow*fPadsX + fPadsX; 
         nPadAddr++) {
     if (getPad(nPadAddr)->getPadActive() == kTRUE)
      if (!getPad(nPadAddr)->isOutX(fXpos)) {
       nHit = 1;
       break;
      }
    }
   }

   if (nHit == 0) return NULL;

   nHit = 0;
   i = nPadAddr % fPadsX;
   for (j = 0; j < fPadsY; j++) {
    if (getPad(i,j)->getPadActive() == kTRUE)
     if (!getPad(i,j)->isOut(fXpos,fYpos)) {
      nHit = 1;
      break;
     }
   }

   if (nHit == 0) return NULL;

 return getPad(i,j);
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HRichPadTab::isOut(Int_t nX, Int_t nY) {
 if ((nX < 0) || (nX >= fPadsX)) return kTRUE;
 if ((nY < 0) || (nY >= fPadsY)) return kTRUE;
return kFALSE;
}
//============================================================================

//----------------------------------------------------------------------------
TObject*& HRichPadTab::getSlot(int nIndx) {
  return (fPadsArray->operator[](nIndx));
}
//============================================================================

//----------------------------------------------------------------------------
TObject* HRichPadTab::getObject(int nIndx) {
  return fPadsArray->At(nIndx);
}
//============================================================================


