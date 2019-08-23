// File: showerunpacker.cc
//
// Author: Leszek Kidon
// Last update: 25/5/2000
//
using namespace std;
#include "hades.h"
#include "hevent.h"
#include "hspectrometer.h"
#include "hruntimedb.h"
#include "hdetector.h"
#include "hcategory.h"
#include "hldsubevt.h"
#include "hdebug.h"
#include "heventheader.h"
#include "hshowerunpackparam.h"
#include "hshowersubidpar.h"
#include "hshowerunpacker.h"
#include "hshowerrawhist.h"
#include "hshowerraw.h"
#include "heventheader.h"

#include <iostream>
#include <iomanip> 

HShowerUnpacker::HShowerUnpacker(Int_t nSubId) {
  m_nSubId = nSubId;
  pRawCat = NULL;
  nEvNr = 0;
  m_loc.set(4,0,0,0,0);
  m_zeroLoc.set(0);
}

HShowerUnpacker::~HShowerUnpacker(void) {
}

Bool_t HShowerUnpacker::init(void) {
  pRawCat=gHades->getCurrentEvent()->getCategory(catShowerRaw);
  if (!pRawCat) {         
    pRawCat=gHades->getSetup()->getDetector("Shower")
                             ->buildCategory(catShowerRaw);
    if (!pRawCat) {   
      return kFALSE;  
    } else {
      gHades->getCurrentEvent()->addCategory(catShowerRaw,pRawCat,"Shower");
    }         
  }

  HRuntimeDb* rtdb=gHades->getRuntimeDb();

  HShowerUnpackParam *pParam = (HShowerUnpackParam*)rtdb->
                                            getContainer("ShowerUnpackParam");
  pParam->defaultInit();
  setParams(pParam);
  //configure(pParam);

  return kTRUE;
}

Bool_t HShowerUnpacker::reinit(void) {
  configure(m_pParams);
  return kTRUE;
}

void HShowerUnpacker::configure(HShowerUnpackParam* pParam) {
   HShowerSubIdPar *pSubIdPar;

   pSubIdPar = pParam->getSubIdPar(m_nSubId);
   if (pSubIdPar) {
   m_nSector = pSubIdPar->getSector();
   m_nRowOffset = 16 * pSubIdPar->getPartOfSector();
   m_nSwap = (pSubIdPar->getSwap()) ? -1 : 1;
   if (m_nSwap == -1)
     m_nRowOffset += 8;
   }
   else
     printf("not corrected initialization of shower unpacker id = %d\n", m_nSubId);
   
}

Int_t HShowerUnpacker::fillData(class HLocation &loc, Int_t nCharge, Int_t nEvtId,Byte_t bErrorCode) {
  HShowerRaw *pCell = NULL;
  // check the location of the data, should not be out of the detector !!!
  Int_t nEvt=gHades->getCurrentEvent()->getHeader()->getEventSeqNumber();
  if(loc[0]<0 || loc[0]>6 || loc[1]<0 || loc[1]>3 || loc[2]<0 || loc[2]>32 || loc[3]<0 || loc[3]>32){
    cout<<"--ERROR --EvNb:"<<nEvt<<" ShowerUnpack: data out of range: sec:"
    <<loc[0]<<" mod:"<<loc[1]<<" row:"<<loc[2]<<" col:"<<loc[3]<<endl;
    return 0;
  }


  pCell = (HShowerRaw*) pRawCat->getNewSlot(m_zeroLoc);
  if (pCell != NULL) {
      pCell = new(pCell) HShowerRaw;



      pCell->setCharge(nCharge);
      pCell->setSector(loc[0]);
      pCell->setModule(loc[1]);
      pCell->setRow(loc[2]);
      pCell->setCol(loc[3]);
      pCell->setEventType(nEvtId&0xFF);
      pCell->setErrorCode(bErrorCode);
  }
  else
  {
      Error("HShowerUnpacker::execute","Not enough memory");
      return 0;
  }
  return 1;
}

Int_t HShowerUnpacker::execute() {
  Int_t ii = 0;
  Int_t sector, module, row, column, chargeH, chargeL;
  UInt_t nControlData;
  UInt_t nByteNumber;
  Int_t threshold;
  nEvNr++;

  Int_t nEvtId;
  nEvtId = gHades->getCurrentEvent()->getHeader()->getId();

  if (pSubEvt) {
	Byte_t bErrorCode=0;
	bErrorCode = ( *(pSubEvt->getHeader()) >> 9 ) & 0x1;
    for (UInt_t i = 0; i < pSubEvt->getDataLen(); i++) {
      UInt_t* dataWord = pSubEvt->getData() + i;
      nControlData = (*dataWord >> 16);
      nByteNumber = nControlData & 0x1FF;

      if (frameIdentifier(nControlData)!=0) continue;  //no data

      sector = getSector(nControlData);
      module = m_pParams->getModuleNumber(nByteNumber);
      threshold = 0;
      if (m_pParams->useThreshold()) 
             threshold = m_pParams->getThreshold(sector, module); 
      column = m_pParams->getColumnNumber(nByteNumber);
//      column = 31 - column;  //flip shower det. 
      row = m_pParams->getRowNumber(nByteNumber) + getRowOffset(nControlData);

      chargeH = getChargeHigh(*dataWord);
      if (chargeH >= threshold) {
        m_loc[0]=sector;	
        m_loc[1]=module;	
        m_loc[2]=row + 4;	
        m_loc[3]=column;	
         if (fillData(m_loc, chargeH, nEvtId,bErrorCode)) ii++;
     }

      chargeL = getChargeLow(*dataWord);
      if (chargeL >= threshold) {
        m_loc[0]=sector;
        m_loc[1]=module;
        m_loc[2]=row;
        m_loc[3]=column;
        if (fillData(m_loc, chargeL, nEvtId,bErrorCode)) ii++;
      }

    }
  }
// printf("ii unpack = %d\n", ii);
//  dump();

  return 1;
}


Bool_t HShowerUnpacker::finalize() {
  return kTRUE;
}

Int_t HShowerUnpacker::dump() {
  if (pSubEvt) {
    UInt_t i = 0;
    cout << endl;
    while (i < pSubEvt->getDataLen()) {
      UInt_t* dataWord = pSubEvt->getData() + i;
      Int_t nChargeLow = getChargeHigh(*dataWord);
      Int_t nChargeHigh  = getChargeLow(*dataWord);
 
      printf("Word: %d \tData %x %d %d\n", i, *dataWord, nChargeLow, 
				nChargeHigh); 
      i++;
    }
    cout << endl;
  }

  return 1;
}

ClassImp(HShowerUnpacker)
