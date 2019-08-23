// File: showerunpackold.cc
//
// Author: Walter Karig <W.Karig@gsi.de>
// Last update: 98/02/06 20:30:24
//

#include "hshowerunpackerold.h"
#include "hshowerraw.h"
#include "hades.h"
#include "hevent.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hcategory.h"
#include "hldsubevt.h"
#include "hdebug.h"
#include "showermap.h"

#include <iomanip.h>

HShowerUnpackerOld::HShowerUnpackerOld(void) {
  pRawCat = NULL;
}

HShowerUnpackerOld::~HShowerUnpackerOld(void) {
}

Bool_t HShowerUnpackerOld::init(void) {
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

int HShowerUnpackerOld::execute() {
  HShowerRaw *pCell = NULL;
  HLocation loc;
  loc.set(4, 0, 0, 0, 0);

  int ii = 0;

  if (pSubEvt) {
    for (unsigned int i = 0; i < pSubEvt->getDataLen(); i++) {
      Adc* adc = (Adc*)(pSubEvt->getData() + i);
      int nr = adc->nr;
      int charge = adc->charge;
      int sector = getSector(nr);
      int module = getModule(nr);
      if (module != empty && adc->ctrl == 0 && charge != 0) {
	int row = getRow(nr);
	int column = getColumn(nr);

//        printf("= %d %d %d %d %d=\n",  sector, module, row, column, charge);
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
          Error("HShowerUnpackerOld::execute","Not enough memory");
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

inline Map* getMap(int adcNr) {
  return map + adcNr%24;
}

int HShowerUnpackerOld::getModule(int adcNr) {
  return getMap(adcNr)->module;
}

int HShowerUnpackerOld::getColumn(int adcNr) {
  if (getModule(adcNr) != post)
    return adcNr/24;
  else
    return 31 - adcNr/24;
}

int HShowerUnpackerOld::getRow(int adcNr) {
  return getMap(adcNr)->row + rowOffset;
}

int HShowerUnpackerOld::dump() {
  if (pSubEvt) {
    unsigned int i = 0;
    cout << endl;
    while (i < pSubEvt->getDataLen()) {
      Adc* adc = (Adc*)(pSubEvt->getData() + i);
      cout.fill('0'); cout.width(4);
      cout << hex << adc->nr     << dec << '|';
      cout.width(2);
      cout << hex << adc->charge << dec << '|';
      cout.width(1);
      cout << hex << adc->ctrl   << dec;
      if (i++%8 == 7)
	cout << endl;
      else
	cout << ' ';
    }
    if (i%8)
      cout << endl;
  }

  return 1;
}

ClassImp(HShowerUnpackerOld)
