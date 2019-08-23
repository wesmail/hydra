#include <iostream.h>
#include <stdlib.h>
#include <math.h>
#include "hshowerwire.h"
#include "hshowergeantwire.h"

ClassImp(HShowerWire)
ClassImp(HShowerWireTab)

HShowerWire::HShowerWire() {
   nWireNr = 0;
   fYwir = 0.;
} // eof constructor

HShowerWire::~HShowerWire() {
/* this is dummy destructor. It will take care of
   freeing memory if there are any hits on wire */

} // eof destructor

HShowerWireTab::HShowerWireTab()
{
   reset();
}

HShowerWireTab::~HShowerWireTab()
{
   reset();
}

Bool_t HShowerWireTab::initAscii(HParHadAsciiFileIo* pHadAsciiFile) {

  if (!pHadAsciiFile)
     return kFALSE;

  char buf[80];

  sprintf(buf, "Shower Module %d - Wires Coordinates", m_nModuleID);
  printf("%s initializating ...", buf);

  HAsciiKey* pHAscii = pHadAsciiFile->GetKeyAscii();
  HAsciiKey &mydata = *pHAscii;


  if(!mydata.SetActiveSection(buf))
     return kFALSE;

  int nrWires = mydata.ReadInt("Wires");
  HShowerWire* pWire;

  setDistWire(mydata.ReadFloat("Distance"));

  for (int i=0; i<nrWires; i++) {
    pWire = new HShowerWire;
    pWire->setNrWire(i);
    pWire->setYWire(mydata.ReadFloat("Wires Coordinates",i));
    addWire(pWire);
  }

  printf("\tdone\n");
  return kTRUE;
}

Bool_t HShowerWireTab::writeAscii(HParHadAsciiFileIo* pHadAsciiFile) {
  return kTRUE;
}


void HShowerWireTab::reset()
{
   fDistWire = 0.0;
   nTotalNr = 0;
   fNewX = fNewY = fQ = 0.0;
   nFiredWire = -1;

   nWires = 0;
   m_nModuleID = -1;
   m_WiresArr.Delete();

}


HShowerWire* HShowerWireTab::getWire(Int_t n) {
   if ((n < 0) || (n >= nWires))
      return NULL;

   return (HShowerWire*)m_WiresArr.At(n);
}

Int_t HShowerWireTab::addWire(HShowerWire* pWire) {
  m_WiresArr.Add(pWire);
  return nWires++;
}

Int_t HShowerWireTab::setWire(HShowerWire* pWire, Int_t n) {
   if ((n < 0) || (n >= nWires))
      return 0;

   delete m_WiresArr.At(n);
   m_WiresArr.AddAt(pWire, n);

   return 1;

}

Int_t HShowerWireTab::lookupWire(float y)
{
  int nWireNr = -1;;
  float Ywir;

  for(int j = 0; j < nWires; j++)
  {
     Ywir = getWire(j)->getYWire();
     if ((y >= Ywir - fDistWire) && (y < Ywir + fDistWire))
     {
        nWireNr = j;
        break;
     }
  }

  return nWireNr;
}

