// File: showerunpack.cc
//
// Author: Walter Karig <W.Karig@gsi.de>
// Last update: 98/02/06 20:30:24
//

#include "hades.h"
#include "hevent.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hcategory.h"
#include "hldsubevt.h"
#include "hdebug.h"
#include "hshowerunpacker.h"
#include "hshowerraw.h"

#include <iomanip.h>

HShowerUnpacker::HShowerUnpacker(void) {
  pRawCat = NULL;
  nEvNr = 0;
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
  return kTRUE;                                                
}

Int_t HShowerUnpacker::fillData(HLocation& loc, Int_t nCharge) {
  HShowerRaw *pCell = NULL;
  
  pCell = (HShowerRaw*) pRawCat->getSlot(loc);
  if (pCell != NULL) {
      pCell = new(pCell) HShowerRaw;
      pCell->setCharge(nCharge);
      pCell->setSector(loc[0]);
      pCell->setModule(loc[1]);
      pCell->setRow(loc[2]);
      pCell->setCol(loc[3]);
      pCell->setEventNr(nEvNr);
      pCell->setEventType(eventType());
  }
  else
  {
      Error("HShowerUnpacker::execute","Not enough memory");
      return 0;
  }
  return 1;
}

Int_t HShowerUnpacker::execute() {
  HLocation loc;
  loc.set(4, 0, 0, 0, 0);

  Int_t ii = 0;
  Int_t sector, module, rowH, rowL, column, chargeH, chargeL;
  UInt_t nControlData;
  nEvNr++;

  if (pSubEvt) {
    for (unsigned int i = 0; i < pSubEvt->getDataLen(); i++) {
//      if (i == 800) break;
//      continue;
      UInt_t* dataWord = pSubEvt->getData() + i;
      nControlData = (*dataWord >> 16);
      if (frameIdentifier(nControlData)!=0) continue;  //no data

//      if ((i>382) && (i<387)) printf("Error at %d : data word %x\n", i, (int)*dataWord);
//      if ((i>770) && (i<777)) printf("Error at %d : data word %x\n", i, (int)*dataWord);

      sector = getSector(nControlData);
      module = getModule(nControlData);
      column = getColumn(nControlData);

      chargeH = getChargeHigh(*dataWord);
      if (chargeH != 0) {
        rowH = getRowHigh(nControlData);

        loc[0]=sector;	
        loc[1]=module;	
        loc[2]=rowH;	
        loc[3]=column;	
        if (fillData(loc, chargeH)) ii++;
      }

      chargeL = getChargeLow(*dataWord);
      if (chargeL != 0) {
        rowL = getRowLow(nControlData);

        loc[0]=sector;
        loc[1]=module;
        loc[2]=rowL;
        loc[3]=column;
        if (fillData(loc, chargeL)) ii++;
      }

/*
     loc[0] = 0; 
     loc[1] = 0; 
     loc[2] = 0; 
     loc[3] = 0; 
     chargeL = 0;
     fillData(loc, chargeL);
     fillData(loc, chargeL);
*/
    }
  }
// printf("ii unpack = %d\n", ii);
//  dump();

  return 1;
}


Int_t HShowerUnpacker::getChargeLow(UInt_t dataWord) {
   return dataWord & 0xFF; 
}

Int_t HShowerUnpacker::getChargeHigh(UInt_t dataWord) {
   UInt_t d = dataWord;
   d >>= 8;
   return d & 0xFF; 
}

Int_t HShowerUnpacker::getRowHigh(UInt_t dataWord) {
   return getRowLow(dataWord) + 4;
}

Int_t HShowerUnpacker::getRowLow(UInt_t dataWord) {
   UInt_t d = (dataWord & 0x1FF);
   return (d & 0x03) + getRowOffset(dataWord);
}

Int_t HShowerUnpacker::getRowOffset(UInt_t dataWord) {
   UInt_t d = dataWord;
   d >>= 7;
   return (d & 0x18);
}

Int_t HShowerUnpacker::getColumn(UInt_t dataWord) {
   UInt_t d = (dataWord & 0x1FF);
   d >>= 2;

   return d / 3;
}

Int_t HShowerUnpacker::getModule(UInt_t dataWord) {
   UInt_t d = (dataWord &  0x1FF);
   d >>= 2;

   return d % 3;
}

Int_t HShowerUnpacker::getSector(UInt_t dataWord) {
   return 2; //not defined yet;
}

Int_t HShowerUnpacker::frameIdentifier(UInt_t dataWord) {
   UInt_t d = dataWord;
   d >>= 14;
   return d & 0x03;
}

Int_t HShowerUnpacker::dataStatus(UInt_t dataWord) {
   UInt_t d = dataWord;
   d >>= 12;
   return d & 0x01;
}


Int_t HShowerUnpacker::dump() {
  if (pSubEvt) {
    UInt_t i = 0;
    cout << endl;
    while (i < pSubEvt->getDataLen()) {
      UInt_t* dataWord = pSubEvt->getData() + i;
      Int_t nDataStatus;
      Int_t nChargeLow = getChargeHigh(*dataWord);
      Int_t nChargeHigh  = getChargeLow(*dataWord);
 
      printf("Word: %d \tData %x %d %d\n", i, *dataWord, nChargeLow, nChargeHigh); 
      i++;
    }
    cout << endl;
  }

  return 1;
}

ClassImp(HShowerUnpacker)
