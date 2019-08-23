#pragma implementation
using namespace std;
#include <iostream> 
#include <iomanip>
#include "hades.h"
#include "hspectrometer.h"
#include "hparhadasciifileio.h"
#include "hlocation.h"
#include "hspectrometer.h"
#include "hshowerparset.h"
#include "hshowercalpar.h"
#include "hshowerdetector.h"
#include "hshowersubidpar.h"

ClassImp(HShowerSubIdPar)

HShowerSubIdPar::HShowerSubIdPar() {
  SetName("ShowerSubIdPar");
 
  m_nSubId = 0;
  m_nSector = 0;
  m_nPartOfSector = 0;
  m_nSwap = 0;
}

HShowerSubIdPar::HShowerSubIdPar(Int_t nSubId) {
  SetName("ShowerSubIdPar");
 
  defaultInit(nSubId);
}


Bool_t HShowerSubIdPar::defaultInit(Int_t nSubId) {
  m_nSubId = nSubId;
  m_nSector = (m_nSubId - 300) / 2;
  m_nPartOfSector = (m_nSubId - 300) % 2;
  m_nSwap = 0;

  return kTRUE;
}
 

Bool_t HShowerSubIdPar::initAscii(HParHadAsciiFileIo* pHadAsciiFileIo) {
  Bool_t status = kTRUE;
  char buf[60];

  try {
     HAsciiKey* pHAscii = pHadAsciiFileIo->GetKeyAscii();
     HAsciiKey &mydata = *pHAscii;

     sprintf(buf, "Shower subevent %d", m_nSubId);
     mydata.SetActiveSection(buf);

     m_nSubId = mydata.ReadInt("Id");
     m_nSector = mydata.ReadInt("Sector");
     m_nPartOfSector = mydata.ReadInt("Part of sector");
     m_nSwap = mydata.ReadInt("Swap");
  }
  catch (Bool_t ret) {
    status = ret;
  }

  return status;
}

Bool_t HShowerSubIdPar::writeAscii(HParHadAsciiFileIo* pHadAsciiFileIo) {
  Bool_t status = kTRUE;
  char buf[60];

  try {

     HAsciiKey* pHAscii = pHadAsciiFileIo->GetKeyAscii();
     HAsciiKey &mydata = *pHAscii;

     sprintf(buf, "Shower subevent %d", m_nSubId);
     mydata.WriteSection(buf);

     mydata.WriteInt("Id", m_nSubId);
     mydata.WriteInt("Sector", m_nSector);
     mydata.WriteInt("Part of sector", m_nPartOfSector);
     mydata.WriteInt("Swap", m_nSwap);
  }
  catch (Bool_t ret) {
    status = ret;
  }

  return status;
}
 
