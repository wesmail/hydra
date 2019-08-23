#pragma implementation
#include <iostream.h>
#include "hades.h"
#include "hspectrometer.h"
#include "hrichframe.h"
#include "hrichdetector.h"
#include "hruntimedb.h"
#include "hrichgeometry.h"

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
}


Bool_t HRichFrame::initAscii(HParHadAsciiFileIo* pHadAsciiFile) {

  if (!pHadAsciiFile) return kFALSE;

  Int_t i,j;

  char buf[80];

  HAsciiKey* pHAscii = pHadAsciiFile->GetKeyAscii();
  HAsciiKey &mydata = *pHAscii;

    sprintf(buf, "Rich Sector Frame parameters");
    printf("%s initializating ...", buf);

    if (!mydata.SetActiveSection(buf))
       return kFALSE;

    HRichFrameCorner *pFrameCorner;
    int cornerNr = mydata.ReadInt("Frame corners");
    mydata.ReadFloat("Frame coordinates");
    for (int i = 0; i < cornerNr; i++) {
       pFrameCorner = new HRichFrameCorner;
       pFrameCorner->setX(mydata.ReadFloat(i*3));
       pFrameCorner->setY(mydata.ReadFloat(i*3+1));
       pFrameCorner->setFlagArea((Int_t)mydata.ReadFloat(i*3+2));
       addCorner(pFrameCorner);
    }
   
  printf("\tdone\n");

  return kTRUE;
}



HRichFrameCorner* HRichFrame::getCorner(Int_t n)
{
   if ((n < 0) || (n >= nCornerNr))
      return NULL;

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

for (int i=0; i < nCornerNr - 1; i++) {
 X0 = getCorner(i)->getX();
 X1 = getCorner(i + 1)->getX();
 Y0 = getCorner(i)->getY();
 Y1 = getCorner(i + 1)->getY();
 nFlag = getCorner(i)->getFlagArea(); 
 
 if (X0 == X1) {
  if (nFlag == 0 && x > X0) return 1;
  else if (nFlag == 1 && x < X0) return 1;
 }
 else if (Y0 == Y1) {
  b = (X1 * Y0 - X0 * Y1)/(X1 - X0);
  if (nFlag == 0 && y > Y0) return 1;
  else if (nFlag == 1 && y < Y0) return 1;
 }
 else {
  a = (Y1 - Y0)/(X1 - X0);
  b = (X1*Y0 - X0*Y1)/(X1 - X0);
  if (a<0 && 
      ((nFlag == 0 && y>(a*x+b)) || 
       (nFlag == 1 && y<(a*x+b)))) return 1;
  else if (a>0 &&
           ((nFlag==0 && y<(a*x+b)) ||
            (nFlag==1 && y>(a*x+b)))) return 1;
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

  if (!m_pReadParam)
     return kFALSE;

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
    if (nFrame < 0 || nFrame >= nrFrames)
     return NULL;

    return &phFrame[nFrame];
}

Int_t HRichFrameTab::isOut(Int_t nFrame, Float_t x, Float_t y)
{
  if (nFrame < 0 || nFrame >= nrFrames)
     return 0;

  return phFrame[nFrame].isOut(x,y);
}
