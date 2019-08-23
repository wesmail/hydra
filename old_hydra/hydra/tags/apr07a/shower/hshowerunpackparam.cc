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
#include "hshowerunpackparam.h"
#include "hshowersubidpar.h"
ClassImp(HShowerUnpackParam)

HShowerUnpackParam::HShowerUnpackParam(const char* name,const char* title,
                                       const char* context)
                   : HShowerParSet(name,title,context) {
  m_nIsInit = kFALSE;
  m_nUseThresholds = kFALSE;
  for (int i = 0; i < 18; m_nThresholds[i++]=0);
  for (int i = 0; i < 384; i++) {
    m_nColumnsNumbers[i] = 0;
    m_nRowsNumbers[i] = 0;
    m_nModulesNumbers[i] = 0;
  }

  m_pSubIdArr = new TObjArray(12);
}

HShowerUnpackParam::~HShowerUnpackParam()
{
   if (m_pSubIdArr) {
     m_pSubIdArr->Delete();    
     delete m_pSubIdArr;
   }
}

Bool_t HShowerUnpackParam::defaultInit() {
  for (int i = 0; i < 384; i++) {
    m_nColumnsNumbers[i] = calcColumn(i);
    m_nRowsNumbers[i] = calcRow(i);
    m_nModulesNumbers[i] = calcModule(i);
  }
 
  m_nIsInit = kTRUE;
  return kTRUE;
}
 

Bool_t HShowerUnpackParam::initAscii(HParHadAsciiFileIo* pHadAsciiFileIo) {
  Bool_t status = kTRUE;
  HShowerSubIdPar *pSubIdPar;
  Int_t nNSubId;
  Int_t subid[12];

  printf("initialization of unpacker's parameters\n");
  try {

     HAsciiKey* pHAscii = pHadAsciiFileIo->GetKeyAscii();
     HAsciiKey &mydata = *pHAscii;

     
     mydata.SetActiveSection("Shower's  subevents");
     nNSubId = mydata.ReadInt("Subevents number");
     mydata.ReadInt(subid, "Subevents", nNSubId); 

     for(Int_t i = 0; i < nNSubId; i++) {
        pSubIdPar = new HShowerSubIdPar(subid[i]);
        m_pSubIdArr->AddAt(pSubIdPar, subid[i] - 300);     
        pSubIdPar->initAscii(pHadAsciiFileIo);
     }
     printf("init: entries %d\n", m_pSubIdArr->GetEntries()); 

     mydata.SetActiveSection("Shower Unpacker's Parameters");
 
     m_nUseThresholds = mydata.ReadInt("Use thresholds");
     mydata.ReadInt(m_nThresholds, "Unpacker thresholds");  
     mydata.ReadInt(m_nColumnsNumbers, "Columns numbers");  
     mydata.ReadInt(m_nRowsNumbers, "Rows numbers");  
     mydata.ReadInt(m_nModulesNumbers, "Modules numbers");  
     m_nIsInit = kTRUE;
  }
  catch (Bool_t ret) {
    status = ret;
  }

  return status;
}

Bool_t HShowerUnpackParam::writeAscii(HParHadAsciiFileIo* pHadAsciiFileIo) {
  Bool_t status = kTRUE;
  HShowerSubIdPar *pSubIdPar[12];
  Int_t nNSubId;
  Int_t subid[12];

  TIterator* iter;
  iter = m_pSubIdArr->MakeIterator();
  printf("write: entries %d\n", m_pSubIdArr->GetEntries()); 
  try {

     HAsciiKey* pHAscii = pHadAsciiFileIo->GetKeyAscii();
     HAsciiKey &mydata = *pHAscii;

     nNSubId = 0;
     while ((pSubIdPar[nNSubId] = (HShowerSubIdPar*)iter->Next())){
        subid[nNSubId] = pSubIdPar[nNSubId]->getSubID();
        nNSubId++;
     }

     mydata.WriteSection("Shower's  subevents");
     mydata.WriteInt("Subevents number", nNSubId);
     mydata.WriteInt("Subevents", nNSubId, subid); 

     for(Int_t i = 0; i < nNSubId; i++)
       pSubIdPar[i]->writeAscii(pHadAsciiFileIo);

     mydata.WriteSection("Shower Unpacker's Parameters");
     mydata.SetNumInLine(9);

     mydata.WriteInt("Use thresholds", m_nUseThresholds);
     mydata.WriteInt("Unpacker thresholds*", 18, m_nThresholds);
   
     mydata.SetNumInLine(12);
     mydata.WriteInt("Columns numbers*", 384, m_nColumnsNumbers);
     mydata.WriteInt("Rows numbers*", 384, m_nRowsNumbers);
     mydata.WriteInt("Modules numbers*", 384, m_nModulesNumbers);
  }
  catch (Bool_t ret) {
    status = ret;
  }

  delete iter;

  return status;
}

HShowerSubIdPar* HShowerUnpackParam::getSubIdPar(Int_t nSubId) {
  return (HShowerSubIdPar*)m_pSubIdArr->At(nSubId - 300);
}
 
