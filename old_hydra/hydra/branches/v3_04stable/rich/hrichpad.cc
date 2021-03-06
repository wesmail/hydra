// File: hrichraw.cc
// ***************************************************************************

//*-- Author : Witold Przygoda (przygoda@psja1.if.uj.edu.pl)
//*-- Modified : 1999/12/04 by Witold Przygoda (przygoda@psja1.if.uj.edu.pl)


#include <iostream.h>
#include <TClonesArray.h>
#include "hades.h"
#include "hrichdetector.h"
#include "hspectrometer.h"
#include "hrichpad.h"

// ***************************************************************************
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
 fPadX = 0;
 fPadY = 0;
 fPadNr = 0;
 fXmin = 0.;
 fYmin = 0.;
 fXmax = 0.;
 fYmax = 0.;
 fPadFlag = 0;
 fPadActive = kFALSE;
 // fPadAmplitude = 0;
 // fPadLabel = 0;
 // fPadLock = 0;
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
 Int_t i;
 for (i = 0; i < source.fCornersNr; i++) { 
  HRichPadCorner *pCor = new HRichPadCorner;
  *pCor = *source.getCorner(i);
  addCorner(pCor);
 }
 
 fPadX = source.fPadX;
 fPadY = source.fPadY;
 fPadNr = source.fPadNr;
 fXmin = source.fXmin;
 fYmin = source.fYmin;
 fXmax = source.fXmax;
 fYmax = source.fYmax;
 fPadFlag = source.fPadFlag;
 fPadActive = source.fPadActive;
 // fPadLabel = source.fPadLabel;
 // fPadLock = source.fPadLock;
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
  if (fLinkedPadsNr > 0 && fLinkedPads) delete [] fLinkedPads;
  for (i = 0; i < source.fCornersNr; i++) {
   HRichPadCorner *pCor = new HRichPadCorner;
   *pCor = *source.getCorner(i);
   addCorner(pCor);
  }

  fPadX = source.fPadX;
  fPadY = source.fPadY;
  fPadNr = source.fPadNr;
  fXmin = source.fXmin;
  fYmin = source.fYmin;
  fXmax = source.fXmax;
  fYmax = source.fYmax;
  fPadFlag = source.fPadFlag;
  fPadActive = source.fPadActive;
  // fPadLabel = source.fPadLabel;
  // fPadLock = source.fPadLock;
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
 fPadX = 0;
 fPadY = 0;
 fPadNr = 0;
 fPadFlag = 0;
 fPadActive = kFALSE;
 // fPadAmplitude = 0;
 // fPadLabel = 0;
 // fPadLock = 0;
 fLinkedPadsNr = 0;
 fPadCornersArray.Delete();
 if (fLinkedPads) {
  delete [] fLinkedPads;
  // only array of connections should be deleted, not the objects!
  fLinkedPads = NULL;
 }
}
//============================================================================

//----------------------------------------------------------------------------
// void HRichPad::clear() {
 // fPadAmplitude = 0;
 // fPadLabel = 0;
 // fPadLock = 0;
// }
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
void HRichPad::Streamer(TBuffer &R__b)
{
   // Stream an object of class HRichPad.

   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(); if (R__v) { }
      TObject::Streamer(R__b);
      //R__b.ReadArray(fLinkedPads);
      R__b >> fCornersNr;
      fPadCornersArray.Streamer(R__b);
      R__b >> fPadX;
      R__b >> fPadY;
      R__b >> fPadNr;
      R__b >> fPadFlag;
      R__b >> fXmin;
      R__b >> fYmin;
      R__b >> fXmax;
      R__b >> fYmax;
      R__b >> fPadActive;
      R__b >> fLinkedPadsNr;
   } else {
      R__b.WriteVersion(HRichPad::IsA());
      TObject::Streamer(R__b);
      //R__b.WriteArray(fLinkedPads, __COUNTER__);
      R__b << fCornersNr;
      fPadCornersArray.Streamer(R__b);
      R__b << fPadX;
      R__b << fPadY;
      R__b << fPadNr;
      R__b << fPadFlag;
      R__b << fXmin;
      R__b << fYmin;
      R__b << fXmax;
      R__b << fYmax;
      R__b << fPadActive;
      R__b << fLinkedPadsNr;
   }
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

      if(createPads(fPadsX,fPadsY))
       fPadsArray->Streamer(R__b);

      for(int i=0; i<fPadsNr; i++) {
          HRichPad* p = getPad(i);
          int nLinkedPad;
          int nLinkNr = p->getLinkedPadsNr();
          if (!nLinkNr) continue;

          if (p->fLinkedPads)
             delete [] p->fLinkedPads;
          p->fLinkedPads = new (HRichPad*)[nLinkNr];

          for(int j = 0; j < nLinkNr; j++) {
             R__b >> nLinkedPad;
             p->fLinkedPads[j] = getPad(nLinkedPad);
          }
      }                           

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

      for(int i=0; i<fPadsNr; i++) {
          HRichPad* p = getPad(i);
          int nLinkNr = p->getLinkedPadsNr();
          for(int j = 0; j < nLinkNr; j++) {
            R__b << p->fLinkedPads[j]->getPadNr();
          }
      }           

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
Bool_t HRichPadTab::initAscii(HParHadAsciiFileIo* pHadAsciiFile) {

 if (!pHadAsciiFile) return kFALSE;

 HRichDetector *pRichDet = (HRichDetector*)gHades->getSetup()->getDetector("Rich");

 HAsciiKey* pHAscii = pHadAsciiFile->GetKeyAscii();
 HAsciiKey &mydata = *pHAscii;

 cout << "Reading of pads parameters .... start!\n";
 if (mydata.SetActiveSection("Rich Sector Pads parameters") == NULL)
   return kFALSE;

  Int_t i,j,k,l,n, loopend, xpad, ypad;
  Float_t xcor, ycor, xmin, ymin, xmax, ymax;
  Float_t xcor1, ycor1, xcor2, ycor2;
  Int_t fArea0, fArea1, nrcor1, nrcor2;
  Float_t x1,y1,x2,y2, a, b;
  Bool_t fAnalyticDigitisation;

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
   setPad(pad, i);
   delete pad;
  }

  while (mydata.IsNextKeyword() == kTRUE) {

    xpad = mydata.ReadInt("*", 0);
    ypad = mydata.ReadInt(1);
    HRichPad *pad = getPad(xpad, ypad);
    pad->setPadActive(1);
    fActivePadsNr++;
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
     xcor = mydata.ReadFloat(k++);
     ycor = mydata.ReadFloat(k++);
     if (j == 0) {
      xmin = xmax = xcor;
      ymin = ymax = ycor;
     }
     if (xcor < xmin) xmin = xcor;
     if (xcor > xmax) xmax = xcor;
     if (ycor < ymin) ymin = ycor;
     if (ycor > ymax) ymax = ycor;
     padcorner->setX(xcor);
     padcorner->setY(ycor);
     // padcorner->setAreaFlag((Int_t)mydata.ReadFloat(k++));
     padcorner->setCornerNr(l++);
     pad->addCorner(padcorner);
    }
    pad->setXmin(xmin);
    pad->setYmin(ymin);
    pad->setXmax(xmax);
    pad->setYmax(ymax);

    fAnalyticDigitisation = kTRUE;
    if (pad->getCornersNr() != 4) {
     fAnalyticDigitisation = kFALSE;
    } else {
     for (n = 0; n < pad->getCornersNr(); n++) {
      pad->getCorner(n)->getXY(&x1, &y1);
      if (n+1 < pad->getCornersNr()) {
       pad->getCorner(n+1)->getXY(&x2, &y2);
      } else {
       pad->getCorner(0)->getXY(&x2, &y2);
      }
      if (x1 != x2 && y1 != y2) fAnalyticDigitisation = kFALSE;
     }
    }
    if (fAnalyticDigitisation)
     pad->setPadFlag(1);
    else pad->setPadFlag(2);


 // calculation of flag area

 for (i = 0; i < pad->getCornersNr(); i++) {

  pad->getCorner(i)->getXY(&xcor1, &ycor1);
  nrcor1 = pad->getCorner(i)->getCornerNr();
  if (i+1 < pad->getCornersNr()) { 
   pad->getCorner(i+1)->getXY(&xcor2, &ycor2);
   nrcor2 = pad->getCorner(i+1)->getCornerNr();
  } else {
   pad->getCorner(0)->getXY(&xcor2, &ycor2);
   nrcor2 = pad->getCorner(0)->getCornerNr();
  }

  fArea0 = fArea1 = 0;
  if (xcor1 == xcor2) {

    for (j = 0; j < pad->getCornersNr(); j++) {
     if (pad->getCorner(j)->getCornerNr() != nrcor1 &&
         pad->getCorner(j)->getCornerNr() != nrcor2) {
      if (pad->getCorner(j)->getX() > xcor1) fArea1++;
      if (pad->getCorner(j)->getX() < xcor1) fArea0++;
     }
    }
    if (fArea1+2 == pad->getCornersNr()) pad->getCorner(i)->setAreaFlag(1);
    else if (fArea0+2 == pad->getCornersNr()) pad->getCorner(i)->setAreaFlag(0);
    else {
     cerr << "Error in <HRichPadTab::initAscii>: "
             "Inconsistency in pads corners coordinates!\n";
     exit(1);
    }

  } else if (ycor1 == ycor2) {

    for (j = 0; j < pad->getCornersNr(); j++) { 
     if (pad->getCorner(j)->getCornerNr() != nrcor1 &&
         pad->getCorner(j)->getCornerNr() != nrcor2) {
      if (pad->getCorner(j)->getY() > ycor1) fArea1++;
      if (pad->getCorner(j)->getY() < ycor1) fArea0++;
     }
    }
    if (fArea1+2 == pad->getCornersNr()) pad->getCorner(i)->setAreaFlag(1);
    else if (fArea0+2 == pad->getCornersNr()) pad->getCorner(i)->setAreaFlag(0);
    else {
     cerr << "Error in <HRichPadTab::initAscii>: "
             "Inconsistency in pads corners coordinates!\n";
     exit(1);
    }

  } else {

    a = (ycor2 - ycor1)/(xcor2 - xcor1);
    b = (xcor2*ycor1 - xcor1*ycor2)/(xcor2 - xcor1);

    for (j = 0; j < pad->getCornersNr(); j++) { 
     if (pad->getCorner(j)->getCornerNr() != nrcor1 &&
         pad->getCorner(j)->getCornerNr() != nrcor2) {
      if (pad->getCorner(j)->getY() > a * pad->getCorner(j)->getX() + b) fArea1++;
      if (pad->getCorner(j)->getY() < a * pad->getCorner(j)->getX() + b) fArea0++;
     }
    }
    if (fArea1+2 == pad->getCornersNr()) pad->getCorner(i)->setAreaFlag(1);
    else if (fArea0+2 == pad->getCornersNr()) pad->getCorner(i)->setAreaFlag(0);
    else {
     cerr << "Error in <HRichPadTab::initAscii>: "
             "Inconsistency in pads corners coordinates!\n";
     exit(1);
    }

  }

 } // end of loop over all corners


  } // eof reading data 



  // now initialisation of some auxiliary variables

  Int_t fRowLen = 0, fRowMax = 0;
  Float_t fMin, fMax;
  Int_t fLeft = -1, fRight = -1;
  Bool_t fActivated = kFALSE;
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


 cout << "Reading of pads parameters .... stop!\n";
 return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HRichPadTab::writeAscii(HParHadAsciiFileIo* pHadAsciiFile) {

 if (!pHadAsciiFile) return kFALSE;

 Char_t buf[128];
 Int_t i,j,fPadNr;

 HAsciiKey* pHAscii = pHadAsciiFile->GetKeyAscii();
 HAsciiKey &mydata = *pHAscii;

 if (!mydata.WriteSection("Rich Sector Pads parameters"))
     return kFALSE;

 for (i = 0; i < fPadsNr; i++) 
  if (getPad(i)->getCornersNr() > 0) {
   fPadNr = getPad(i)->getPadNr();
   sprintf(buf,"Pad %d x (col) y (row) position",fPadNr);
   mydata.WriteNInt(buf,2,getPad(i)->getPadX(),getPad(i)->getPadY());
   sprintf(buf,"Pad %d linked pads nr",fPadNr);
   mydata.WriteKeyword(buf);
   mydata.WriteInt(getPad(i)->getLinkedPadsNr());
   for (j = 0; j < getPad(i)->getLinkedPadsNr(); j++)
    mydata.WriteInt(getPad(i)->fLinkedPads[j]->getPadNr());
   sprintf(buf,"Pad %d corners",fPadNr);
   mydata.WriteKeyword(buf);
   mydata.WriteInt(getPad(i)->getCornersNr());
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
TObject*& HRichPadTab::getSlot(int nIndx) {
  return (fPadsArray->operator[](nIndx));
}
//============================================================================

//----------------------------------------------------------------------------
TObject* HRichPadTab::getObject(int nIndx) {
  return fPadsArray->At(nIndx);
}
//============================================================================


