//#pragma implementation
#include "hades.h"
#include "hspectrometer.h"
#include "hshowerparset.h"
#include "hshowerdetector.h"
#include "hshowercalasic.h"
ClassImp(HShowerCalAsic)

////////// **********ShowerCalAsic defintion ****************/////////////77
HShowerCalAsic::HShowerCalAsic() {
  SetName("ShowerCalAsic");

  m_pList = new TList;
}

HShowerCalAsic::~HShowerCalAsic()
{ 
  if (m_pList) {
    m_pList->Delete();
    delete m_pList;
    m_pList = NULL;
  }
}


HShowerAsic* HShowerCalAsic::getAsic(Int_t nAsicNr)
{
  return (HShowerAsic*) m_pList->At(nAsicNr);
}

Bool_t  HShowerCalAsic::addAsic(Int_t nAsicNr, HShowerAsic* pAsic) {
  if (nAsicNr != pAsic->getAsicNr())
     pAsic->setAsicNr(nAsicNr);   
  
  if (m_pList->At(nAsicNr)) {
      printf("Asic nr = %d exist\n", nAsicNr);
      return kFALSE;
  }
     
  m_pList->AddAt(pAsic, nAsicNr);
  return kTRUE;
}

Bool_t HShowerCalAsic::addAsic(HShowerAsic* pAsic) {
  return addAsic(pAsic->getAsicNr(), pAsic);
}

Float_t HShowerCalAsic::getSlope(Int_t nAsicNr, Int_t nIdx)
{
  return getAsic(nAsicNr)->getSlope(nIdx);
}

Float_t HShowerCalAsic::getOffset(Int_t nAsicNr, Int_t nIdx)
{
  return getAsic(nAsicNr)->getOffset(nIdx);
}

void HShowerCalAsic::setSlope(Int_t nAsicNr, Int_t nIdx, Float_t fSlope)
{
  getAsic(nAsicNr)->setSlope(nIdx, fSlope);
}

void HShowerCalAsic::setOffset(Int_t nAsicNr, Int_t nIdx, Float_t fOffset)
{
  getAsic(nAsicNr)->setOffset(nIdx, fOffset);
}

Bool_t HShowerCalAsic::initAscii(HParHadAsciiFileIo* pHadAsciiFileIo)
{
  Float_t slopes[32];
  Float_t offsets[32];
  int asicNr;

  printf("initialization of ShowerCalAsic\n");

  HAsciiKey* pHAscii = pHadAsciiFileIo->GetKeyAscii();
  HAsciiKey &mydata = *pHAscii;

  if (!mydata.SetActiveSection("Shower ASIC's Parameters"))
     return kFALSE;

  while(mydata.IsNextKeyword()) {
     asicNr = mydata.ReadInt("*");
     mydata.ReadFloat(slopes, "*");
     mydata.ReadFloat(offsets, "*");

     HShowerAsic *pAsic = new HShowerAsic(asicNr, slopes, offsets);    
     if (!addAsic(pAsic))
       return kFALSE;
  } 
  
  return kTRUE;
}

Bool_t HShowerCalAsic::writeAscii(HParHadAsciiFileIo* pHadAsciiFileIo) {
  TIterator* iter =  m_pList->MakeIterator();
  HShowerAsic *pAsic;

  Char_t buffer[32]; 
  Float_t slopes[32];
  Float_t offsets[32];
  int asicNr;

  HAsciiKey* pHAscii = pHadAsciiFileIo->GetKeyAscii();
  HAsciiKey &mydata = *pHAscii;

  mydata.WriteSection("Shower ASIC's Parameters");
  mydata.SetNumInLine(8);
  while((pAsic = (HShowerAsic*)iter->Next())) {
     asicNr = pAsic->getAsicNr();
     for (int i = 0; i < 32; i++) {
        slopes[i] = pAsic->getSlope(i);
        offsets[i] = pAsic->getOffset(i);
     }

     sprintf(buffer, "ASIC %d", asicNr);
     mydata.WriteInt(buffer, asicNr);
     sprintf(buffer, "ASIC %d slopes*", asicNr);
     mydata.WriteFloat(buffer, 32, slopes);
     sprintf(buffer, "ASIC %d offsets*", asicNr);
     mydata.WriteFloat(buffer, 32, offsets);
     mydata.WriteRemark("---------------------------------", '#');
  }
  delete iter;
  return kTRUE;
}
