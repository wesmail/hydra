#include <iostream.h>
#include <stdlib.h>
#include <math.h>
#include "hrichwire.h"

ClassImp(HRichWire)
ClassImp(HRichWireTab)

HRichWire::HRichWire() {
   fNrWire = 0;
   fXwire = 0.;
} // eof constructor

HRichWire::~HRichWire() {
} // eof destructor

HRichWireTab::HRichWireTab()
{
   reset();
}

HRichWireTab::~HRichWireTab()
{
   reset();
}

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
  fDistWire = mydata.ReadFloat("Distance between wires");
  mydata.ReadFloat("Wires coordinates");
  for (i = 0; i < nWires; i++) {
   pWire = new HRichWire;
   pWire->setNrWire(i);
   pWire->setXWire(mydata.ReadFloat(i));
   addWire(pWire);
  }

  return kTRUE;
}


void HRichWireTab::reset()
{
   fDistWire = 0.0;
   nWires = 0;
   m_WiresArr.Delete();

}


HRichWire* HRichWireTab::getWire(Int_t n) {
   if ((n < 0) || (n >= nWires))
      return NULL;

   return (HRichWire*)m_WiresArr.At(n);
}

Int_t HRichWireTab::addWire(HRichWire* pWire) {
  m_WiresArr.Add(pWire);
 return 1;
}


Int_t HRichWireTab::setWire(HRichWire* pWire, Int_t n) {
   if ((n < 0) || (n >= nWires))
      return 0;

   delete m_WiresArr.At(n);
   m_WiresArr.AddAt(pWire, n);

   return 1;
}

