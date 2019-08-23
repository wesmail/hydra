// File: hrichraw.cc
// ***************************************************************************

//*-- Author : Witold Przygoda (przygoda@psja1.if.uj.edu.pl)
//*-- Modified : 1999/12/04 by Witold Przygoda (przygoda@psja1.if.uj.edu.pl)

using namespace std;
#include <iostream> 
#include <iomanip>
#include <stdlib.h>
#include <TClonesArray.h>
#include "hades.h"
#include "hrichdetector.h"
#include "hspectrometer.h"
#include "hrichpad.h"

// ***************************************************************************
//_HADES_CLASS_DESCRIPTION 
//////////////////////////////////////////////////////////////////////////////
//
//  HRichPadCorner
//  HRichPad
//  HRichPadTab
//
//  These classes contain definition of corners, pads and array of pads.
//
//////////////////////////////////////////////////////////////////////////////
// ***************************************************************************



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
HRichPadCorner::HRichPadCorner(Int_t nrcor, 
                               Float_t xcor, Float_t ycor, Int_t flag) {
 fCornerNr = nrcor;
 fXcor = xcor;
 fYcor = ycor;
 fAreaFlag = flag;
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
 fX = 0.;
 fY = 0.;
 fPadX = 0;
 fPadY = 0;
 fPadNr = 0;
 fXmin = 0.;
 fYmin = 0.;
 fXmax = 0.;
 fYmax = 0.;
 fPadFlag = 0;
 fAmplitFraction = 0.;
 fPadActive = kFALSE;

 fTheta = 0.;
 for (Int_t i = 0; i < 6; i++) {
  fXlab[i] = 0.;
  fYlab[i] = 0.;
  fZlab[i] = 0.;
  fPhi[i] = 0.;
 }
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
 fCornersNr = 0; // this will be set to real number afterwards
 Int_t i;
 for (i = 0; i < source.fCornersNr; i++) { 
  HRichPadCorner *pCor = new HRichPadCorner;
  *pCor = *((HRichPadCorner*)source.fPadCornersArray.At(i));
  addCorner(pCor);
 }
 
 fCornersNr = source.fCornersNr;
 fX = source.fX;
 fY = source.fY;
 fPadX = source.fPadX;
 fPadY = source.fPadY;
 fPadNr = source.fPadNr;
 fXmin = source.fXmin;
 fYmin = source.fYmin;
 fXmax = source.fXmax;
 fYmax = source.fYmax;
 fPadFlag = source.fPadFlag;
 fAmplitFraction = source.fAmplitFraction;
 fPadActive = source.fPadActive;
 fTheta = source.fTheta;
 for (Int_t i = 0; i < 6; i++) {
  fXlab[i] = source.fXlab[i];
  fYlab[i] = source.fYlab[i];
  fZlab[i] = source.fZlab[i];
  fPhi[i] = source.fPhi[i];
 }
}

//============================================================================

//----------------------------------------------------------------------------
HRichPad& HRichPad::operator=(const HRichPad& source)
{
 if (this != &source) {
  fX = source.fX;
  fY = source.fY;
  fPadX = source.fPadX;
  fPadY = source.fPadY;
  fPadNr = source.fPadNr;
  fXmin = source.fXmin;
  fYmin = source.fYmin;
  fXmax = source.fXmax;
  fYmax = source.fYmax;
  fPadFlag = source.fPadFlag;
  fAmplitFraction = source.fAmplitFraction;
  fPadActive = source.fPadActive;
  fTheta = source.fTheta;
  for (Int_t i = 0; i < 6; i++) {
   fXlab[i] = source.fXlab[i];
   fYlab[i] = source.fYlab[i];
   fZlab[i] = source.fZlab[i];
   fPhi[i] = source.fPhi[i];
  }
 }
return *this;
}
//============================================================================

//----------------------------------------------------------------------------
void HRichPad::reset()
{
 fCornersNr = 0;
 fX = 0.;
 fY = 0.;
 fPadX = 0;
 fPadY = 0;
 fPadNr = 0;
 fPadFlag = 0;
 fAmplitFraction = 0.;
 fPadActive = kFALSE;
 fTheta = 0.;
 for (Int_t i = 0; i < 6; i++) {
  fXlab[i] = 0.;
  fYlab[i] = 0.;
  fZlab[i] = 0.;
  fPhi[i] = 0.;
 }
 if(fPadCornersArray.GetSize()>0) {
      fPadCornersArray.Delete();
 }
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
void HRichPad::getXY(Float_t *pX, Float_t *pY) {
 *pX = fX;
 *pY = fY;
}
//============================================================================

//----------------------------------------------------------------------------
void HRichPad::getPadXY(Int_t *pX, Int_t *pY) {
 *pX = fPadX;
 *pY = fPadY;
}
//============================================================================

//----------------------------------------------------------------------------
void HRichPad::getXYZlab(const Int_t sec, Float_t *pX, Float_t *pY, Float_t *pZ) {
 *pX = fXlab[sec];
 *pY = fYlab[sec];
 *pZ = fZlab[sec];
}
//============================================================================

//----------------------------------------------------------------------------
void HRichPad::getAngles(const Int_t sec, Float_t *pTheta, Float_t *pPhi) {
 *pTheta = fTheta;
 *pPhi = fPhi[sec];
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
void HRichPad::calcPadCenter() {

 Int_t i;
 Float_t xcor = 0., ycor = 0., xsum = 0., ysum = 0.;

 for (i = 0; i < fCornersNr; i++) {
  getCorner(i)->getXY(&xcor, &ycor);
  xsum += xcor;
  ysum += ycor;
 }
 fX = xsum / fCornersNr;
 fY = ysum / fCornersNr;
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
     if (y < ycor1) return kTRUE;
    } else {
     if (y > ycor1) return kTRUE;
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
void HRichPadTab::Streamer(TBuffer &R__b)
{
   // Stream an object of class HRichPadTab.

   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(); if (R__v) { }
      HParSet::Streamer(R__b);
      R__b >> fPadsNr;
      R__b >> fActivePadsNr;
      R__b >> fPadsX;
      R__b >> fPadsY;
      R__b >> fPadsLongestRow;
      R__b >> fMiddlePad;
      R__b >> fPadsLongestRowMiddle;

     // this is dummy initialisation of all pads structure
     if (createPads(fPadsX,fPadsY)) {
      for (Int_t i = 0; i < fPadsNr; i++) {
       HRichPad *pad = new HRichPad;
       pad->setPadNr(i);
       pad->CalcNrtoXY(fPadsX);
       pad->setPadActive(0);
       pad->setPadFlag(0);
       pad->setAmplitFraction(0.);
       setPad(pad, i);
       delete pad;
      }
      fPadsArray->Streamer(R__b);
     }

     initParameters();

   } else {
      R__b.WriteVersion(HRichPadTab::IsA());
      HParSet::Streamer(R__b);
      R__b << fPadsNr;
      R__b << fActivePadsNr;
      R__b << fPadsX;
      R__b << fPadsY;
      R__b << fPadsLongestRow;
      R__b << fMiddlePad;
      R__b << fPadsLongestRowMiddle;

      fPadsArray->Streamer(R__b);
   }
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
  fActivePadsNr = 0;
  fPadsX = 0;
  fPadsY = 0;
  fPadsLongestRow = 0;
  fMiddlePad = 0;
  fPadsLongestRowMiddle = 0.;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HRichPadTab::initParameters() {


 Int_t i,j,k;
 Float_t xcor=0., ycor=0., xmin=0., ymin=0., xmax=0., ymax=0.;
 Float_t x1, x2, y1, y2, a, b;
 Int_t n1, n2, fArea0, fArea1;
 Bool_t fAnalyticDigitisation = kFALSE;

 Int_t fRowLen = 0, fRowMax = 0;
 Float_t fMin=0., fMax=0.;
 Int_t fLeft = -1, fRight = -1;
 Bool_t fActivated = kFALSE;

 HRichPad *pPad = NULL;
 HRichPadCorner *pCorner = NULL;


 for (i = 0; i < getPadsNr(); i++) {

  fAnalyticDigitisation = kTRUE;
  pPad = getPad(i);

  // the loop sets minimum and maximum (in x and y) for given pad
  for (j = 0; j < pPad->getCornersNr(); j++) {
   pCorner = pPad->getCorner(j);
   xcor = pCorner->getX();
   ycor = pCorner->getY();
   if (j == 0) {
    xmin = xmax = xcor;
    ymin = ymax = ycor;
   }
   if (xcor < xmin) xmin = xcor;
   if (xcor > xmax) xmax = xcor;
   if (ycor < ymin) ymin = ycor;
   if (ycor > ymax) ymax = ycor;
  }

  pPad->setXmin(xmin);
  pPad->setYmin(ymin);
  pPad->setXmax(xmax);
  pPad->setYmax(ymax);


  // the loop checks if analytical formula can be used during digitisation
  if (pPad->getCornersNr() != 4) {
   fAnalyticDigitisation = kFALSE;
  } else {
   for (j = 0; j < pPad->getCornersNr(); j++) {
    pPad->getCorner(j)->getXY(&x1, &y1);
    if (j+1 < pPad->getCornersNr()) {
     pPad->getCorner(j+1)->getXY(&x2, &y2);
    } else {
     pPad->getCorner(0)->getXY(&x2, &y2);
    }
    if (x1 != x2 && y1 != y2) fAnalyticDigitisation = kFALSE;
   }
  }

  if (fAnalyticDigitisation)
   pPad->setPadFlag(1);
  else pPad->setPadFlag(2);


 // calculation of flag area
 for (j = 0; j < pPad->getCornersNr(); j++) {

  pPad->getCorner(j)->getXY(&x1, &y1);
  n1 = pPad->getCorner(j)->getCornerNr();
  if (j+1 < pPad->getCornersNr()) {
   pPad->getCorner(j+1)->getXY(&x2, &y2);
   n2 = pPad->getCorner(j+1)->getCornerNr();
  } else {
   pPad->getCorner(0)->getXY(&x2, &y2);
   n2 = pPad->getCorner(0)->getCornerNr();
  }

  fArea0 = fArea1 = 0;
  if (x1 == x2) {

    for (k = 0; k < pPad->getCornersNr(); k++) {
     if (pPad->getCorner(k)->getCornerNr() != n1 &&
         pPad->getCorner(k)->getCornerNr() != n2) {
      if (pPad->getCorner(k)->getX() > x1) fArea1++;
      if (pPad->getCorner(k)->getX() < x1) fArea0++;
     }
    }
    if (fArea1+2 == pPad->getCornersNr()) pPad->getCorner(j)->setAreaFlag(1);
    else if (fArea0+2 == pPad->getCornersNr()) pPad->getCorner(j)->setAreaFlag(0);
    else {
     cerr << "Error in <HRichPadTab::initParameters>: "
             "Inconsistency in pads corners coordinates!\n";
     exit(1);
    }

  } else if (y1 == y2) {

    for (k = 0; k < pPad->getCornersNr(); k++) {
     if (pPad->getCorner(k)->getCornerNr() != n1 &&
         pPad->getCorner(k)->getCornerNr() != n2) {
      if (pPad->getCorner(k)->getY() > y1) fArea1++;
      if (pPad->getCorner(k)->getY() < y1) fArea0++;
     }
    }
    if (fArea1+2 == pPad->getCornersNr()) pPad->getCorner(j)->setAreaFlag(1);
    else if (fArea0+2 == pPad->getCornersNr()) pPad->getCorner(j)->setAreaFlag(0);
    else {
     cerr << "Error in <HRichPadTab::initParameters>: "
             "Inconsistency in pads corners coordinates!\n";
     exit(1);
    }

  } else {

    a = (y2 - y1)/(x2 - x1);
    b = (x2*y1 - x1*y2)/(x2 - x1);

    for (k = 0; k < pPad->getCornersNr(); k++) {
     if (pPad->getCorner(k)->getCornerNr() != n1 &&
         pPad->getCorner(k)->getCornerNr() != n2) {
      if (pPad->getCorner(k)->getY() > a * pPad->getCorner(k)->getX() + b) fArea1++;
      if (pPad->getCorner(k)->getY() < a * pPad->getCorner(k)->getX() + b) fArea0++;
     }
    }
    if (fArea1+2 == pPad->getCornersNr()) pPad->getCorner(j)->setAreaFlag(1);
    else if (fArea0+2 == pPad->getCornersNr()) pPad->getCorner(j)->setAreaFlag(0);
    else {
     cerr << "Error in <HRichPadTab::initParameters>: "
             "Inconsistency in pads corners coordinates!\n";
     exit(1);
    }

  }

 } // end of loop over all corners

 pPad->calcPadCenter();

 } // eof loop over all pads


  for (j = 0; j < fPadsY; j++) {
   fRowLen = 0;
   for (i = 0; i < fPadsX; i++)
    if (getPad(i, j)->getPadActive()) {
     fRowLen++;
    }
   if (fRowLen > fRowMax) {
    fRowMax = fRowLen;
    fPadsLongestRow = j;
   }
  }
  for (i = 0; i < fPadsX; i++)
   if (getPad(i, fPadsLongestRow)->getPadActive()) {
    if (fLeft < 0) fLeft = getPad(i, fPadsLongestRow)->getPadNr();
    if (fRight < getPad(i, fPadsLongestRow)->getPadNr())
                  fRight = getPad(i, fPadsLongestRow)->getPadNr();
   }

  fMiddlePad = fLeft + (fRight-fLeft)/2 + 1;

  for (i = 0; i < fPadsX; i++) {
   if (getPad(i, fPadsLongestRow)->getPadActive()) {
    if (fActivated == kFALSE) {
     fMin = getPad(i, fPadsLongestRow)->getXmin();
     fMax = getPad(i, fPadsLongestRow)->getXmax();
     fActivated = kTRUE;
    }
    if (fMin > getPad(i, fPadsLongestRow)->getXmin())
     fMin = getPad(i, fPadsLongestRow)->getXmin();
    if (fMax < getPad(i, fPadsLongestRow)->getXmax())
     fMax = getPad(i, fPadsLongestRow)->getXmax();
   }
  }
  fPadsLongestRowMiddle = (fMin + fMax)/2.;


 return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HRichPadTab::initAscii(HParHadAsciiFileIo* pHadAsciiFile) {

 if (!pHadAsciiFile) return kFALSE;

 HRichDetector *pRichDet = (HRichDetector*)gHades->getSetup()->getDetector("Rich");

 HAsciiKey &mydata = *pHadAsciiFile->GetKeyAscii();

 mydata.SetActiveSection("Rich Sector Pads parameters");

  Int_t i,j,k,l, xpad, ypad;
  Float_t xcor, ycor;

  fPadsX = pRichDet->getColumns();
  fPadsY = pRichDet->getRows();
  fPadsNr = fPadsX*fPadsY;
  createPads(fPadsX, fPadsY);

  // this is dummy initialisation of all pads structure 
  for (i = 0; i < fPadsNr; i++) {
   HRichPad *pad = new HRichPad;
   pad->setPadNr(i);
   pad->CalcNrtoXY(fPadsX);
   pad->setPadActive(0);
   pad->setPadFlag(0); 
   pad->setAmplitFraction(0.); 
   for (j=0; j<6; j++) pad->setPhi(j,0.0);
   pad->setTheta(0.0);
   setPad(pad, i);
   delete pad;
  }

  while (mydata.IsNextKeyword() == kTRUE) {

    xpad = mydata.ReadInt("*", 0);
    ypad = mydata.ReadInt(1);
    HRichPad *pad = getPad(xpad, ypad);
    pad->setPadActive(1);
    fActivePadsNr++;

    k = l = 0;
    pad->setTheta(mydata.ReadFloat("*", 0));
    mydata.ReadFloat("*");
    for (j = 0; j < 4; j++) {
     HRichPadCorner *padcorner = new HRichPadCorner;
     xcor = mydata.ReadFloat(k++);
     ycor = mydata.ReadFloat(k++);
     padcorner->setX(xcor);
     padcorner->setY(ycor);
     padcorner->setCornerNr(l++);
     pad->addCorner(padcorner);
    }

  } // eof reading data 

  initParameters();

 return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HRichPadTab::writeAscii(HParHadAsciiFileIo* pHadAsciiFile) {

 if (!pHadAsciiFile) return kFALSE;

 Char_t buf[128];
 Int_t i,j,fPadNr;

 HAsciiKey &mydata = *pHadAsciiFile->GetKeyAscii();

 mydata.WriteSection("Rich Sector Pads parameters");

 for (i = 0; i < fPadsNr; i++) 
  if (getPad(i)->getCornersNr() > 0) {
   fPadNr = getPad(i)->getPadNr();
   sprintf(buf,"Pad %d x (col) y (row) position",fPadNr);
   mydata.WriteNInt(buf,2,getPad(i)->getPadX(),getPad(i)->getPadY());

    sprintf(buf,"Pad %d theta in LAB",fPadNr);
    mydata.WriteKeyword(buf);
    mydata.WriteFloat(getPad(i)->getTheta());

   sprintf(buf,"Pad %d corners",fPadNr);
   mydata.WriteKeyword(buf);
   for (j = 0; j < getPad(i)->getCornersNr(); j++) {
    mydata.WriteLine("\n\\ ");
    mydata.WriteNFloat(2,getPad(i)->getCorner(j)->getX(),
                         getPad(i)->getCorner(j)->getY());
   }
   mydata.WriteLine("\n");
   mydata.WriteRemark("----------------------------------------------------------------------",'#');
   mydata.WriteLine("\n");
  }

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
         nPadAddr < fMiddlePad; 
         nPadAddr++) {
     if (getPad(nPadAddr)->getPadActive() == kTRUE)
      if (!getPad(nPadAddr)->isOutX(fXpos)) {                    
       nHit = 1;
       break;
      }
    }
   } else {
    for (nPadAddr = fMiddlePad; 
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
TObject*& HRichPadTab::getSlot(Int_t nIndx) {
  return (fPadsArray->operator[](nIndx));
}
//============================================================================

//----------------------------------------------------------------------------
TObject*  HRichPadTab::getObject(Int_t nIndx) {
  return fPadsArray->At(nIndx);
}
//============================================================================

