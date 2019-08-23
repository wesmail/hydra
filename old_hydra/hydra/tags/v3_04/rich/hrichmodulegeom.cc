#pragma implementation
#include <iostream.h>
#include "hrichmodulegeom.h"

ClassImp(HRichModuleGeom)


HRichModuleGeom::HRichModuleGeom() {
  reset();
} // eof constructor


HRichModuleGeom::~HRichModuleGeom() {
  reset();
} // eof destructor

void HRichModuleGeom::reset()
{
  m_nModuleID = -1;

  m_pFrame = NULL;
  m_pWires = NULL;
  m_pPads = NULL;

  m_pReadParam = NULL;

  m_fCenterX =  m_fCenterY = m_fCenterZ = 0.0;
}

Bool_t HRichModuleGeom::initAscii(HParHadAsciiFileIo* pHadAsciiFile) {
  if (!pHadAsciiFile)
     return kFALSE;

  char buf[80];

  sprintf(buf, "Center Module %d", m_nModuleID);
  HAsciiKey* pHAscii = pHadAsciiFile->GetKeyAscii();
  HAsciiKey &mydata = *pHAscii;

  m_fCenterX = mydata.ReadFloat(buf, 0);
  m_fCenterY = mydata.ReadFloat(buf, 1);
  m_fCenterZ = mydata.ReadFloat(buf, 2);
  
  return kTRUE;
}

//______________________________________________________________________________
void HRichModuleGeom::Streamer(TBuffer &R__b)
{
   // Stream an object of class HRichModuleGeom.

   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(); if (R__v) { }
      HParSet::Streamer(R__b);
      R__b >> m_fCenterX;
      R__b >> m_fCenterY;
      R__b >> m_fCenterZ;
      R__b >> m_nModuleID;
/*
      R__b >> m_pFrame;
      R__b >> m_pWires;
      R__b >> m_pPads;
*/
   } else {
      R__b.WriteVersion(HRichModuleGeom::IsA());
      HParSet::Streamer(R__b);
      R__b << m_fCenterX;
      R__b << m_fCenterY;
      R__b << m_fCenterZ;
      R__b << m_nModuleID;
      m_pFrame = NULL;
      m_pWires = NULL;
      m_pPads = NULL;
/*
      R__b << m_pFrame;
      R__b << m_pWires;
      R__b << m_pPads;
*/
   }
}

