#pragma implementation
#include <iostream.h>
#include "hshowerframe.h"

ClassImp(HShowerFrameCorner)
ClassImp(HShowerFrame)
ClassImp(HShowerFrameTab)

HShowerFrameCorner::HShowerFrameCorner() {
  fXcor = 0.0;
  fYcor = 0.0;
  nFlagArea = 0;
}


HShowerFrame::HShowerFrame() {
  reset();
} // eof constructor


HShowerFrame::~HShowerFrame() {
  reset();
} // eof destructor

void HShowerFrame::reset()
{
  nCornerNr = 0;
  m_nModuleID = -1;
  m_FrameArr.Delete();
}

Bool_t HShowerFrame::initAscii(HParHadAsciiFileIo* pHadAsciiFile) {
  if (!pHadAsciiFile)
     return kFALSE;

  char buf[80];

   
  sprintf(buf, "Shower Module %d - Frame Coordinates", m_nModuleID);
  printf("%s initializating ...", buf);

  HAsciiKey* pHAscii = pHadAsciiFile->GetKeyAscii();
  HAsciiKey &mydata = *pHAscii;

  if (!mydata.SetActiveSection(buf))
     return kFALSE;

  HShowerFrameCorner *pFC;
  int cornerNr = mydata.ReadInt("Corners");
  for (int i = 0; i < cornerNr; i++) {
     pFC = new HShowerFrameCorner;
     pFC->setX(mydata.ReadFloat("Corners Coordinates",i*3));
     pFC->setY(mydata.ReadFloat("Corners Coordinates",i*3+1));
     pFC->setFlagArea((int)mydata.ReadFloat("Corners Coordinates",i*3+2));
     addCorner(pFC);
  }

  printf("\tdone\n");

  return kTRUE;
}

Bool_t HShowerFrame::writeAscii(HParHadAsciiFileIo* pHadAsciiFile) {
  return kTRUE;
}

HShowerFrameCorner* HShowerFrame::getCorner(Int_t n)
{
   if ((n < 0) || (n >= nCornerNr))
      return NULL;

   return (HShowerFrameCorner*)m_FrameArr.At(n);
}


Int_t HShowerFrame::setCorner(HShowerFrameCorner* pCorner, Int_t n)
{
   if ((n < 0) || (n >= nCornerNr))
      return 0;

   delete m_FrameArr.At(n);
   m_FrameArr.AddAt(pCorner, n);

   return 1;
}

Int_t HShowerFrame::addCorner(HShowerFrameCorner* pCorner)
{
  m_FrameArr.Add(pCorner);
  return nCornerNr++;
}

int HShowerFrame::isOut(float x, float y) {

float a,b;
int nCornerNr = getCornerNr();

if (nCornerNr<=2) {
// msglog(1,LOG_ERR,"%s\a\n","Frame must have at least 3 corners!");
 exit(1);
}

#ifdef RICHDIGI_DEBUG3
 cout << "RICHDIGI: HShowerFrame _isOut_ is now calculated \n";
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


HShowerFrameTab::HShowerFrameTab()
{
  phFrame = NULL;
  nrFrames = 0;
  m_pReadParam = NULL;
}

HShowerFrameTab::~HShowerFrameTab()
{
  deleteFrames();
}

void HShowerFrameTab::deleteFrames()
{
  if (phFrame)
    delete[] phFrame;
  phFrame = NULL;

  nrFrames = 0;
}

Bool_t HShowerFrameTab::init() {
  if (!m_pReadParam)
     return kFALSE;

  createFrames(1);

  HShowerFrame *phFrame = getFrame(0);
  phFrame->initAscii(NULL);

  return kTRUE;
}

void HShowerFrameTab::createFrames(Int_t n)
{
  deleteFrames();
  nrFrames = n;
  phFrame = new HShowerFrame[nrFrames];
}

Int_t HShowerFrameTab::getNFrames() {
  return nrFrames;
}

HShowerFrame* HShowerFrameTab::getFrame(Int_t nFrame)
{ 
    if (nFrame < 0 || nFrame >= nrFrames)
     return NULL;

    return &phFrame[nFrame];
}

Int_t HShowerFrameTab::isOut(Int_t nFrame, Float_t x, Float_t y)
{
  if (nFrame < 0 || nFrame >= nrFrames)
     return 0;

  return phFrame[nFrame].isOut(x,y);
}


