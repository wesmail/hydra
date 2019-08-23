#pragma implementation
using namespace std;
#include <stdlib.h>
#include <iostream> 
#include <iomanip>
#include "hades.h"
#include "hspectrometer.h"
#include "hrichframe.h"
#include "hrichdetector.h"
#include "hruntimedb.h"
#include "hrichgeometrypar.h"
#include "hadascii.h"

ClassImp(HRichFrameCorner)
ClassImp(HRichFrame)
ClassImp(HRichFrameTab)

HRichFrameCorner::HRichFrameCorner() {
  fXcor = 0.0;
  fYcor = 0.0;
  nFlagArea = 0;
}


HRichFrame::HRichFrame() {
  reset();
} // eof constructor


HRichFrame::~HRichFrame() {
  reset();
} // eof destructor

void HRichFrame::reset()
{
  nCornerNr = 0;
  m_FrameArr.Delete();
  fPadsWiresDistance = 0.;
}


//----------------------------------------------------------------------------
Bool_t HRichFrame::initAscii(HParHadAsciiFileIo* pHadAsciiFile) {

 using namespace HadAsciiAuxiliaryFunctions;
  if (!pHadAsciiFile) return kFALSE;

  Int_t i,j,k, fArea0, fArea1, nrcor1, nrcor2;
  Float_t a, b, xcor1, xcor2, ycor1, ycor2;

  char buf[80];

  HAsciiKey &mydata = *pHadAsciiFile->GetKeyAscii();

  sprintf(buf, "Rich Sector Frame parameters");

  mydata.SetActiveSection(buf);

    k = 0;
    HRichFrameCorner *pFrameCorner;
    Int_t cornerNr = mydata.ReadInt("Frame corners");
    mydata.ReadFloat("Frame coordinates");
    for (int i = 0; i < cornerNr; i++) {
       pFrameCorner = new HRichFrameCorner;
       pFrameCorner->setX(mydata.ReadFloat(k++));
       pFrameCorner->setY(mydata.ReadFloat(k++));
       pFrameCorner->setCornerNr(i);
       addCorner(pFrameCorner);
    }

 // calculation of flag area

 for (i = 0; i < getCornerNr(); i++) {

  getCorner(i)->getXY(&xcor1, &ycor1);
  nrcor1 = getCorner(i)->getCornerNr();
  if (i+1 < getCornerNr()) {
   getCorner(i+1)->getXY(&xcor2, &ycor2);
   nrcor2 = getCorner(i+1)->getCornerNr();
  } else {
   getCorner(0)->getXY(&xcor2, &ycor2);
   nrcor2 = getCorner(0)->getCornerNr();
  }

  fArea0 = fArea1 = 0;
  if (xcor1 == xcor2) {

    for (j = 0; j < getCornerNr(); j++) {
     if (getCorner(j)->getCornerNr() != nrcor1 &&
         getCorner(j)->getCornerNr() != nrcor2) {
      if (getCorner(j)->getX() > xcor1) fArea1++;
      if (getCorner(j)->getX() < xcor1) fArea0++;
     }
    }
    if (fArea1+2 == getCornerNr()) getCorner(i)->setFlagArea(1);
    else if (fArea0+2 == getCornerNr()) getCorner(i)->setFlagArea(0);
    else {
     ErrorMsg(2,"HRichFrame::initAscii",
      1,"Inconsistency in frame corners coordinates.");
     throw (Bool_t) kFALSE;
    }

  } else if (ycor1 == ycor2) {

    for (j = 0; j < getCornerNr(); j++) {
     if (getCorner(j)->getCornerNr() != nrcor1 &&
         getCorner(j)->getCornerNr() != nrcor2) {
      if (getCorner(j)->getY() > ycor1) fArea1++;
      if (getCorner(j)->getY() < ycor1) fArea0++;
     }
    }
    if (fArea1+2 == getCornerNr()) getCorner(i)->setFlagArea(1);
    else if (fArea0+2 == getCornerNr()) getCorner(i)->setFlagArea(0);
    else {
     ErrorMsg(2,"HRichFrame::initAscii",
      1,"Inconsistency in frame corners coordinates.");
     throw (Bool_t) kFALSE;
    }

  } else {

    a = (ycor2 - ycor1)/(xcor2 - xcor1);
    b = (xcor2*ycor1 - xcor1*ycor2)/(xcor2 - xcor1);

    for (j = 0; j < getCornerNr(); j++) {
     if (getCorner(j)->getCornerNr() != nrcor1 &&
         getCorner(j)->getCornerNr() != nrcor2) {
      if (getCorner(j)->getY() > a * getCorner(j)->getX() + b) fArea1++;
      if (getCorner(j)->getY() < a * getCorner(j)->getX() + b) fArea0++;
     }
    }
    if (fArea1+2 == getCornerNr()) getCorner(i)->setFlagArea(1);
    else if (fArea0+2 == getCornerNr()) getCorner(i)->setFlagArea(0);
    else {
     ErrorMsg(2,"HRichFrame::initAscii",
      1,"Inconsistency in frame corners coordinates.");
     throw (Bool_t) kFALSE;
    }

  }

 } // end of loop over all corners


  return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HRichFrame::writeAscii(HParHadAsciiFileIo* pHadAsciiFile) {

  if (!pHadAsciiFile) return kFALSE;

  HAsciiKey &mydata = *pHadAsciiFile->GetKeyAscii();

  mydata.WriteSection("Rich Sector Frame parameters");

  Float_t fPadX, fPadY;
  Int_t cornerNr = getCornerNr();
  mydata.WriteInt("Frame corners", cornerNr);
  mydata.SetNumInLine(2);
  mydata.WriteKeyword("Frame coordinates");
  for (int i = 0; i < cornerNr; i++) {
   fPadX = getCorner(i)->getX();
   fPadY = getCorner(i)->getY();
   mydata.WriteLine("\n\\ ");
   mydata.WriteNFloat(2,fPadX,fPadY);
  }

 return kTRUE;
}
//============================================================================


HRichFrameCorner* HRichFrame::getCorner(Int_t n)
{
   if ((n < 0) || (n >= nCornerNr)) return NULL;

   return (HRichFrameCorner*)m_FrameArr.At(n);
}


Int_t HRichFrame::setCorner(HRichFrameCorner* pCorner, Int_t n)
{
   if ((n < 0) || (n >= nCornerNr))
      return 0;

   delete m_FrameArr.At(n);
   m_FrameArr.AddAt(pCorner, n);

   return 1;
}

Int_t HRichFrame::addCorner(HRichFrameCorner* pCorner)
{
  m_FrameArr.Add(pCorner);
  return nCornerNr++;
}

int HRichFrame::isOut(float x, float y) {

float a,b;
int nCornerNr = getCornerNr();

if (nCornerNr<=2) {
// msglog(1,LOG_ERR,"%s\a\n","Frame must have at least 3 corners!");
 exit(1);
}

#ifdef RICHDIGI_DEBUG3
 cout << "RICHDIGI: HRichFrame _isOut_ is now calculated \n";
#endif

Float_t X0, X1, Y0, Y1;
Int_t nFlag;

for (int i=0; i < nCornerNr; i++) {
 getCorner(i)->getXY(&X0, &Y0);
 nFlag = getCorner(i)->getFlagArea(); 
 if (i+1 < nCornerNr) getCorner(i+1)->getXY(&X1, &Y1);
  else getCorner(0)->getXY(&X1, &Y1);
 
 if (X0 == X1) {
  if (nFlag == 0 && x > X0) return 1;
  else if (nFlag == 1 && x < X0) return 1;
 }
 else if (Y0 == Y1) {
  // b = (X1 * Y0 - X0 * Y1)/(X1 - X0);
  if (nFlag == 0 && y > Y0) return 1;
  else if (nFlag == 1 && y < Y0) return 1;
 }
 else {
  a = (Y1 - Y0)/(X1 - X0);
  b = (X1*Y0 - X0*Y1)/(X1 - X0);
  if (nFlag == 0 && y > (a*x+b)) return 1;
   else if (nFlag == 1 && y < (a*x+b)) return 1;
 }
}

return 0;
}


HRichFrameTab::HRichFrameTab()
{
  phFrame = NULL;
  nrFrames = 0;
  m_pReadParam = NULL;
}

HRichFrameTab::~HRichFrameTab()
{
  deleteFrames();
}

void HRichFrameTab::deleteFrames()
{
  if (phFrame)
    delete[] phFrame;
  phFrame = NULL;
  nrFrames = 0;
}

Bool_t HRichFrameTab::init() {

  if (!m_pReadParam) return kFALSE;

  createFrames(1);

  HRichFrame *phFrame = getFrame(0);
  phFrame->initAscii(NULL);


  return kTRUE;
}

void HRichFrameTab::createFrames(Int_t n)
{
  deleteFrames();
  nrFrames = n;
  phFrame = new HRichFrame[nrFrames];
}

Int_t HRichFrameTab::getNFrames() {
  return nrFrames;
}

HRichFrame* HRichFrameTab::getFrame(Int_t nFrame)
{ 
    if (nFrame < 0 || nFrame >= nrFrames) return NULL;

    return &phFrame[nFrame];
}

Int_t HRichFrameTab::isOut(Int_t nFrame, Float_t x, Float_t y)
{
  if (nFrame < 0 || nFrame >= nrFrames) return 0;

  return phFrame[nFrame].isOut(x,y);
}
