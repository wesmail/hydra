#include <iostream.h>

#include "hshowerpad.h"

ClassImp(HShowerPad)
ClassImp(HShowerPadTab)

/////////////////////////////////////////////////////////////
// HShowerPad
// Class describe coordinates one pad
//
// HShowerPadTab
// Table of local coordinates of pads.  
// Thera are pads informations in one dimensional table
// Two-dimensional structure of pads in shower detector
// is calculated to one index of table of pads
//
/////////////////////////////////////////////////////////////


HShowerPad::HShowerPad() {
//creating default pad
     nPadNr = 0;
     nPadFlag = 0;
     fXld = fYld = fXlu = fYlu = 0.;
     fXrd = fYrd = fXru = fYru = 0.;
     qpad = 0.;
     nLinkedNr = 0;
     linkPad = NULL;
     nPadsX = 0;
}

HShowerPad::HShowerPad(const HShowerPad& srcPad)
{
//copy ctor
     reset();
     nPadNr = srcPad.nPadNr;
     nPadsX = srcPad.nPadsX;
     nPadFlag =  srcPad.nPadFlag;
     fXld = srcPad.fXld;
     fYld = srcPad.fYld;
     fXlu = srcPad.fXlu;
     fYlu = srcPad.fYlu;
     fXrd = srcPad.fXrd;
     fYrd = srcPad.fYrd;
     fXru = srcPad.fXru;
     fYru = srcPad.fYru;
     qpad =  srcPad.qpad;
     nLinkedNr = srcPad.nLinkedNr;
     if (srcPad.nLinkedNr) {
        linkPad  = new (HShowerPad*)[nLinkedNr];
        for(int i = 0; i < nLinkedNr; i++)
           linkPad[i] = srcPad.linkPad[i];
     }
     else
       linkPad = NULL;
}




HShowerPad::~HShowerPad() {
  reset();
} // eof destructor

HShowerPad& HShowerPad::operator=(const HShowerPad& srcPad)
{
     reset();
     nPadNr = srcPad.nPadNr;
     nPadsX = srcPad.nPadsX;
     nPadFlag =  srcPad.nPadFlag;
     fXld = srcPad.fXld;
     fYld = srcPad.fYld;
     fXlu = srcPad.fXlu;
     fYlu = srcPad.fYlu;
     fXrd = srcPad.fXrd;
     fYrd = srcPad.fYrd;
     fXru = srcPad.fXru;
     fYru = srcPad.fYru;
     qpad =  srcPad.qpad;
     nLinkedNr = srcPad.nLinkedNr;
     linkPad  = new (HShowerPad*)[nLinkedNr];
     for(int i = 0; i < nLinkedNr; i++)
       linkPad[i] = srcPad.linkPad[i];
 
     return *this;
}

void HShowerPad::reset()
{
//clearing data
   nPadNr = 0;
   nPadsX = 0;
   nPadFlag = 0;
   fXld = fYld = fXlu = fYlu = 0.;
   fXrd = fYrd = fXru = fYru = 0.;
   qpad = 0.;
   nLinkedNr = 0;
   if (linkPad) {
     delete [] linkPad;
     linkPad = NULL;
   }
}

void HShowerPad::getPadCenter(Float_t *pfX, Float_t *pfY) {
//calculate position of geometrical center of pad
  *pfX = 0.25 * (fXld + fXlu + fXrd + fXru); 
  *pfY = 0.25 * (fYld + fYlu + fYrd + fYru); 
}

void HShowerPad::getPadPos(Int_t *pnRow, Int_t *pnCol) {
//return number of row and col 
  *pnCol = nPadNr % nPadsX;
  *pnRow = nPadNr / nPadsX;
}

int HShowerPad::isOut(HShowerGeantWire *qhit) {
//?????????????????
 float fX, fY;
 qhit->getXY(&fX, &fY);

 return isOut(fX, fY);  
}

int HShowerPad::isOut(float x, float y) {
//return TRUE if point (x,y) is outside of pad
  return (isOutY(x, y) || isOutX(x, y));
}


int HShowerPad::isOutX(float x, float y) {
//return TRUE if  x coordinate of point is outside of pad
float a,b;

if (fXld==fXlu && x<fXld) return 1;
if (fYld==fYlu && y<fYld) return 1;

 a = (fYlu-fYld)/(fXlu-fXld);
 b = (fXlu*fYld-fXld*fYlu)/(fXlu-fXld);
 if(a>0.){
    if (y>(a*x+b)) return 1;
 }
 else {
    if (y<(a*x+b)) return 1;
 }

if (fXru==fXrd && x>fXru) return 1;
if (fYru==fYrd && y>fYru) return 1;

 a = (fYrd-fYru)/(fXrd-fXru);
 b = (fXrd*fYru-fXru*fYrd)/(fXrd-fXru);
 if(a>0.){
    if (y<(a*x+b)) return 1;
 } else {
    if (y>(a*x+b)) return 1;
 }

return 0;
}


int HShowerPad::isOutY(float x, float y) {
//return TRUE if  y coordinate of point is outside of pad

float a,b;

if (fYlu==fYru && y>fYlu) return 1;
if (fYrd==fYld && y<fYrd) return 1;

if (fXrd==fXld && x<fXrd) return 1;
if (fXlu==fXru && x>fXlu) return 1;


 a = (fYru-fYlu)/(fXru-fXlu);
 b = (fXru*fYlu-fXlu*fYru)/(fXru-fXlu);
 if (y>(a*x+b)) return 1;

 a = (fYld-fYrd)/(fXld-fXrd);
 b = (fXld*fYrd-fXrd*fYld)/(fXld-fXrd);
 if (y<(a*x+b)) return 1;


return 0;
}

HShowerPadTab::HShowerPadTab() {
//default ctor
  reset();
  m_pPadArr = new TClonesArray("HShowerPad");
}

HShowerPadTab::~HShowerPadTab() {
  deletePads();
}

void HShowerPadTab::reset() {
//clearing data
  m_pPadArr = NULL;
  nPads = 0;
  nPadsX = 0;
  nPadsY = 0;
  m_nModuleID = -1;
}

Bool_t HShowerPadTab::initAscii(HParHadAsciiFileIo* pHadAsciiFile) {
//reading and initializing pads table from ascii file (HAsciiKey format)
  Bool_t status = kTRUE;

  if (!pHadAsciiFile)
     return kFALSE;

  char buf[80];

  try {

     sprintf(buf, "Shower Module %d - Pads Coordinates", m_nModuleID);
     printf("%s initializating ...\n", buf);

     HAsciiKey* pHAscii = pHadAsciiFile->GetKeyAscii();
     HAsciiKey &mydata = *pHAscii;
     int i,j,k,l;

     mydata.SetActiveSection(buf);

     HShowerFrame* phFrame = getFrame();
     int nrPads = mydata.ReadInt("Pads");
     int nrPadX = mydata.ReadInt("PadsX");
     int nrPadY = mydata.ReadInt("PadsY");
     int nr;
 
     createPads(nrPadX, nrPadY);
     for(i=0; i <nrPads; i++) {
        HShowerPad* pad = new HShowerPad;
        setPad(pad, i);
        delete pad;
     }

     j=0;
     for (i=0; i<nrPads; i++) {
       nr  = (int)mydata.ReadFloat("Pads Data", j++);   
       HShowerPad* pad = getPad(nr);
       pad->setPadNr(nr);
   
       pad->fXld = mydata.ReadFloat("Pads Data", j++);
       pad->fYld = mydata.ReadFloat("Pads Data", j++);
       pad->fXlu = mydata.ReadFloat("Pads Data", j++);
       pad->fYlu = mydata.ReadFloat("Pads Data", j++);
       pad->fXrd = mydata.ReadFloat("Pads Data", j++);
       pad->fYrd = mydata.ReadFloat("Pads Data", j++);
       pad->fXru = mydata.ReadFloat("Pads Data", j++);
       pad->fYru = mydata.ReadFloat("Pads Data", j++);
       if (!phFrame->isOut(pad->fXld, pad->fYld) &&
          !phFrame->isOut(pad->fXlu, pad->fYlu) &&
          !phFrame->isOut(pad->fXru, pad->fYru) &&
          !phFrame->isOut(pad->fXrd, pad->fYrd)) {
        pad->setPadFlag(1);
       } else if (!phFrame->isOut(pad->fXld, pad->fYld) ||
           !phFrame->isOut(pad->fXlu, pad->fYlu) ||
           !phFrame->isOut(pad->fXru, pad->fYru) ||
           !phFrame->isOut(pad->fXrd, pad->fYrd)) {
        pad->setPadFlag(2);
       } else {
        pad->setPadFlag(0);
       }
       pad->setLinkedNr((int)mydata.ReadFloat("Pads Data",j++));
       if (pad->getLinkedNr()) {
          if (pad->linkPad) delete [] pad->linkPad;
          pad->linkPad = new (HShowerPad*)[pad->getLinkedNr()];
          for (k=0; k<pad->getLinkedNr(); k++) {
            l = (int)mydata.ReadFloat("Pads Data",j++);
            pad->linkPad[k] = getPad(l);
          }
       }
     }
 
     printf("\tdone\n");
  }
  catch (Bool_t ret) {
    status = ret;
  }

  return status;
}

Bool_t HShowerPadTab::writeAscii(HParHadAsciiFileIo* pHadAsciiFile) {
// writing pads table from ascii file (HAsciiKey format)
  Bool_t status = kTRUE;

  if (!pHadAsciiFile)
    return kFALSE;

  char buf[80];

  try {

     HAsciiKey* pHAscii = pHadAsciiFile->GetKeyAscii();
     HAsciiKey &mydata = *pHAscii;
   
     sprintf(buf, "Shower Module %d - Pads Coordinates", m_nModuleID);
     mydata.WriteSection(buf);

     mydata.WriteInt("Pads",nPads);
     mydata.WriteInt("PadsX",nPadsX);
     mydata.WriteInt("PadsY",nPadsY);
   

     for(int i=0; i<nPads; i++){
        HShowerPad *pad = getPad(i);

        mydata.WriteInt("Pads Data",pad->getPadNr());
        mydata.WriteFloat(pad->fXld);
        mydata.WriteFloat(pad->fYld);
        mydata.WriteFloat(pad->fXlu);
        mydata.WriteFloat(pad->fYlu);
        mydata.WriteFloat(pad->fXrd);
        mydata.WriteFloat(pad->fYrd);
        mydata.WriteFloat(pad->fXru);
        mydata.WriteFloat(pad->fYru);
    }
  }
  catch (Bool_t ret) {
    status = ret;
  }

  return status;
}



Int_t HShowerPadTab::createPads(Int_t nX, Int_t nY) {
//creating internal table of pads
   deletePads();

   nPadsX = nX;
   nPadsY = nY;
   nPads = nPadsX * nPadsY;

   m_pPadArr = new TClonesArray("HShowerPad", nPads); 

   return (m_pPadArr) ? nPads : 0;
}
    
void HShowerPadTab::deletePads() {
//deleting internal table of pads and clearing data
   delete m_pPadArr;
   reset();
}

void HShowerPadTab::setPad(HShowerPad* pPad, Int_t nRow, Int_t nCol)
{
//set pad information at position defined by nRow and nCol
  Int_t nAddr = calcAddr(nRow, nCol);  //calculating index
  setPad(pPad, nAddr);
}

void HShowerPadTab::setPad(HShowerPad* pPad, Int_t nAddr) 
{
//set pad information at position defined by nAddr
  HShowerPad* pNewPad;

  pNewPad = (HShowerPad*)getSlot(nAddr);
  pNewPad = new(pNewPad) HShowerPad;
  pPad->setPadsX(nPadsX);
  *pNewPad = *pPad;
}

HShowerPad* HShowerPadTab::getPad(Int_t nPadNr) {
//return pad information
  return (HShowerPad*)getObject(nPadNr);
}


HShowerPad* HShowerPadTab::getPad(Int_t nRow, Int_t nCol) {
//return pad information
//nRow and nCol is used to calculate index

  if ((nRow < 0) || (nRow >= nPadsY))
     return NULL;

  if ((nCol < 0) || (nCol >= nPadsX))
     return NULL;

  Int_t nAddr = calcAddr(nRow, nCol);
  return getPad(nAddr);
}

HShowerPad* HShowerPadTab::getPad(Float_t  fXpos, Float_t fYpos) {
//looking for pad which contains point fXpos, fYpos
   Int_t nPadAddr;
   Int_t nHit = 0;
   
   nHit = 0;

   nPadAddr = 0;
   for(int j = 0; j < nPadsY; j++) {
      if (!getPad(nPadAddr)->isOutY(fXpos, fYpos)) {
         nHit=1;
         break;
      }
      else
         nPadAddr += nPadsX;
   }

   if (!nHit) return NULL;

   nHit = 0;
   for(int j = 0; j < nPadsX; j++, nPadAddr++) {
     if (!getPad(nPadAddr)->isOutX(fXpos, fYpos)) 
     {                    
        nHit = 1;
        break;
     }
   }

   if (!nHit) return NULL;

   return getPad(nPadAddr);
}

Bool_t HShowerPadTab::isOut(Int_t nX, Int_t nY) {
//testing if pad in nX, nY position is inside table
  if ((nX < 0) || (nX >= nPadsX))
     return 1;

  if ((nY < 0) || (nY >= nPadsY))
     return 1;

  return 0;
}

TObject*& HShowerPadTab::getSlot(int nIndx) {
//access to object in TClonesArray 
  return (m_pPadArr->operator[](nIndx));
}

TObject* HShowerPadTab::getObject(int nIndx) {
//return TObject from TClonesArray
  return m_pPadArr->At(nIndx);
}

//______________________________________________________________________________
void HShowerPad::Streamer(TBuffer &R__b)
{
   // Stream an object of class HShowerPad.

   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(); if (R__v) { }
      TObject::Streamer(R__b);
      R__b >> fXld;
      R__b >> fYld;
      R__b >> fXlu;
      R__b >> fYlu;
      R__b >> fXrd;
      R__b >> fYrd;
      R__b >> fXru;
      R__b >> fYru;
      //R__b.ReadArray(linkPad);
      R__b >> nPadNr;
      R__b >> nPadsX;
      R__b >> nPadFlag;
      R__b >> qpad;
      R__b >> nLinkedNr;
   } else {
      R__b.WriteVersion(HShowerPad::IsA());
      TObject::Streamer(R__b);
      R__b << fXld;
      R__b << fYld;
      R__b << fXlu;
      R__b << fYlu;
      R__b << fXrd;
      R__b << fYrd;
      R__b << fXru;
      R__b << fYru;
      //R__b.WriteArray(linkPad, __COUNTER__);
      R__b << nPadNr;
      R__b << nPadsX;
      R__b << nPadFlag;
      R__b << qpad;
      R__b << nLinkedNr;
   }
}

//______________________________________________________________________________
void HShowerPadTab::Streamer(TBuffer &R__b)
{
   // Stream an object of class HShowerPadTab.

   if (R__b.IsReading()) {

      Version_t R__v = R__b.ReadVersion(); if (R__v) { }
      HShowerParSet::Streamer(R__b);
      R__b >> nPads;
      R__b >> nPadsX;
      R__b >> nPadsY;
      R__b >> m_nModuleID;

      if(createPads(nPadsX,nPadsY))
           m_pPadArr->Streamer(R__b);
	  else
		   Fatal("Streamer", "Cannot create pads %dx%d!!!", nPadsX, nPadsY);
    
      for(int i=0; i<nPads; i++) {
          HShowerPad* p = getPad(i);
          int nLinkedPad;
          int nLinkNr = p->getLinkedNr();
          if (!nLinkNr) continue;

          if (p->linkPad)
             delete [] p->linkPad;
          p->linkPad = new (HShowerPad*)[nLinkNr];

          for(int j = 0; j < nLinkNr; j++) {
             R__b >> nLinkedPad;
             p->linkPad[j] = getPad(nLinkedPad);
          }           
      }

      m_pFrame = NULL;
   } else {
      R__b.WriteVersion(HShowerPadTab::IsA());
      HShowerParSet::Streamer(R__b);
      R__b << nPads;
      R__b << nPadsX;
      R__b << nPadsY;
      R__b << m_nModuleID;

      m_pPadArr->Streamer(R__b);

      for(int i=0; i<nPads; i++) {
          HShowerPad* p = getPad(i); 
          int nLinkNr = p->getLinkedNr();
          for(int j = 0; j < nLinkNr; j++) {
            R__b << p->linkPad[j]->getPadNr();
          }
      }
   }
}

