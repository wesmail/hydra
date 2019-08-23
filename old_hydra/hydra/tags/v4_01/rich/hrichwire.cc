// File: hrichwire.cc
// ***************************************************************************

//*-- Author : Witold Przygoda (przygoda@psja1.if.uj.edu.pl)
//*-- Modified : 1999/12/04 by Witold Przygoda (przygoda@psja1.if.uj.edu.pl)


#include <iostream.h>
#include <stdlib.h>
#include <math.h>
#include "hrichwire.h"

// ***************************************************************************
//////////////////////////////////////////////////////////////////////////////
//
//  HRichWire
//  HRichWireTab
//
//  Wire and set of wires parameters.
//
//////////////////////////////////////////////////////////////////////////////
// ***************************************************************************


ClassImp(HRichWire)
ClassImp(HRichWireTab)

//----------------------------------------------------------------------------
HRichWire::HRichWire() {
   fNrWire = 0;
   fXwire = 0.;
} // eof constructor
//============================================================================

//----------------------------------------------------------------------------
HRichWire::~HRichWire() {
} // eof destructor
//============================================================================

//----------------------------------------------------------------------------
HRichWireTab::HRichWireTab()
{
   reset();
}
//============================================================================

//----------------------------------------------------------------------------
HRichWireTab::~HRichWireTab()
{
   reset();
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HRichWireTab::initAscii(HParHadAsciiFileIo* pHadAsciiFile)
{
  if (!pHadAsciiFile)
     return kFALSE;

  Int_t i;
  HRichWire *pWire;
  printf("Rich Sector Wires parameters initializating ...\n");

  HAsciiKey* pHAscii = pHadAsciiFile->GetKeyAscii();
  HAsciiKey &mydata = *pHAscii;

  if (mydata.SetActiveSection("Rich Sector Wires parameters") == NULL)
   return kFALSE;

  nWires = mydata.ReadInt("Wires number");
  fDistWire = mydata.ReadFloat("Half distance between wires");
  mydata.ReadFloat("Wires coordinates");
  for (i = 0; i < nWires; i++) {
   pWire = new HRichWire;
   pWire->setNrWire(i);
   pWire->setXWire(mydata.ReadFloat(i));
   addWire(pWire);
  }

  return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HRichWireTab::writeAscii(HParHadAsciiFileIo* pHadAsciiFile)
{
  if (!pHadAsciiFile) return kFALSE;

  Int_t i;

  HAsciiKey* pHAscii = pHadAsciiFile->GetKeyAscii();
  HAsciiKey &mydata = *pHAscii;

  if (!mydata.WriteSection("Rich Sector Wires parameters"))
      return kFALSE;

  mydata.WriteInt("Wires number",nWires);
  mydata.WriteFloat("Half distance between wires",fDistWire);
  mydata.WriteKeyword("Wires coordinates*");
  for (i = 0; i < nWires; i++) {
   if (i && !((i+1) % 6)) mydata.WriteLine("\n\\ ");
   mydata.WriteFloat(getWire(i)->getXWire());
  }

 return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
void HRichWireTab::reset()
{
   fDistWire = 0.0;
   nWires = 0;
   m_WiresArr.Delete();

}
//============================================================================

//----------------------------------------------------------------------------
HRichWire* HRichWireTab::getWire(Int_t n) {
   if ((n < 0) || (n >= nWires))
      return NULL;

   return (HRichWire*)m_WiresArr.At(n);
}
//============================================================================

//----------------------------------------------------------------------------
Int_t HRichWireTab::addWire(HRichWire* pWire) {
  m_WiresArr.Add(pWire);
 return 1;
}
//============================================================================

//----------------------------------------------------------------------------
Int_t HRichWireTab::setWire(HRichWire* pWire, Int_t n) {
   if ((n < 0) || (n >= nWires))
      return 0;

   delete m_WiresArr.At(n);
   m_WiresArr.AddAt(pWire, n);

   return 1;
}
//============================================================================

