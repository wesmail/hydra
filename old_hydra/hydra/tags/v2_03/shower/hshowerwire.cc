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

Bool_t HShowerWireTab::init()
{
  if (!m_pReadParam)
     return kFALSE;

  HadIoAscii &mydata = *m_pReadParam;

  mydata.open();

  int nrWires = mydata.getIntData("nrWires");
  HShowerWire* pWire;

  setVoltage(mydata.getFloatData("voltage"));
  setDistWire(mydata.getFloatData("distWire"));
  for (int i=0; i<nrWires; i++) {
    pWire = new HShowerWire;
    pWire->setNrWire(i);
    pWire->setYWire(mydata.getFloatData("Ywir",i));
    addWire(pWire);
  }
  mydata.close();

  return kTRUE;
}

void HShowerWireTab::reset()
{
   fVoltage = 0.0;
   fDistWire = 0.0;
   nTotalNr = 0;
   fNewX = fNewY = fQ = 0.0;
   nFiredWire = -1;

   nWires = 0;
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

