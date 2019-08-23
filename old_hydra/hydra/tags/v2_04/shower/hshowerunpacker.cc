// File: showerunpack.cc
//
// Author: Walter Karig <W.Karig@gsi.de>
// Last update: 98/02/06 20:30:24
//

#include "hshowerunpacker.h"
#include "hshowerraw.h"
#include "hades.h"
#include "hevent.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hcategory.h"
#include "hldsubevt.h"
#include "hdebug.h"

#include <iomanip.h>

HShowerUnpacker::HShowerUnpacker(void) {
  pRawCat = NULL;
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

Int_t HShowerUnpacker::execute() {
  HShowerRaw *pCell = NULL;
  HLocation loc;
  loc.set(4, 0, 0, 0, 0);

  Int_t ii = 0;

  if (pSubEvt) {
    for (unsigned int i = 1; i < pSubEvt->getDataLen(); i++) {
      UInt_t* dataWord = pSubEvt->getData() + i;

      Int_t charge = getCharge(*dataWord);
      if (charge != 0) {
        Int_t sector = getSector(*dataWord);
        Int_t module = getModule(*dataWord);
	Int_t row = getRow(*dataWord);
	Int_t column = getColumn(*dataWord);

        loc[0]=sector;	
        loc[1]=module;	
        loc[2]=row;	
        loc[3]=column;	
        pCell = (HShowerRaw*) pRawCat->getSlot(loc);
        if (pCell != NULL) {
          pCell = new(pCell) HShowerRaw;
          pCell->setCharge(charge);
          pCell->setSector(sector);
          pCell->setModule(module);
          pCell->setRow(row);
          pCell->setCol(column);
        }
        else
        {
          Error("HShowerUnpacker::execute","Not enough memory");
          //return 0;
        }
        ii++;
      }
    }
  }


// printf("ii unpack = %d\n", ii);
//  dump();

  return 1;
}


Int_t HShowerUnpacker::getCharge(UInt_t dataWord) {
   return dataWord & 0xFF; 
}

Int_t HShowerUnpacker::getRow(UInt_t dataWord) {
   dataWord >>= 8;
   return dataWord & 0x1F;
}

Int_t HShowerUnpacker::getColumn(UInt_t dataWord) {
   dataWord >>= 13;
   return dataWord & 0x1F; 
}

Int_t HShowerUnpacker::getModule(UInt_t dataWord) {
   dataWord >>= 18;
   return (dataWord & 0x03) - 1; 
}

Int_t HShowerUnpacker::getSector(UInt_t dataWord) {
   dataWord >>= 20;
   return (dataWord & 0x07) - 1; 
}

Int_t HShowerUnpacker::dump() {
  if (pSubEvt) {
    UInt_t i = 0;
    cout << endl;
    while (i < pSubEvt->getDataLen()) {
    }
    cout << endl;
  }

  return 1;
}

ClassImp(HShowerUnpacker)
