// File: showerunpackexp.cc
//
// Author: Walter Karig <W.Karig@gsi.de>
// Last update: 98/02/06 20:30:24
//

#include "hshowerunpackerexp.h"
#include "hshowerraw.h"
#include "hades.h"
#include "hevent.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hcategory.h"
#include "hldsubevt.h"
#include "hdebug.h"
#include "showermapexp.h"

#include <iomanip.h>

HShowerUnpackerExp::HShowerUnpackerExp(void) {
  pRawCat = NULL;
}

HShowerUnpackerExp::~HShowerUnpackerExp(void) {
}

Bool_t HShowerUnpackerExp::init(void) {
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

int HShowerUnpackerExp::execute() {
  HShowerRaw *pCell = NULL;
  HLocation loc;
  loc.set(4, 0, 0, 0, 0);

  int ii = 0;

  if (pSubEvt == NULL) 
    return kFALSE;

  if (getSubEvtId() != 300)
        return kFALSE;

  unsigned int len = pSubEvt->getDataLen() * 2; //2 bytes are  packed into 16-bit word
                                          //it's should be always 768*2;
  unsigned char* pData = (unsigned char*) pSubEvt->getData();

  for (unsigned int i = 0; i < len; i++) {
      int charge = pData[i];
      if (charge != 0) {
          int nr = i;
          int sector = getSector(nr);
          int module = getModule(nr);
          if (module != empty) {
	     int row = getRow(nr);
	     int column = getColumn(nr);

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
                  Error("HShowerUnpackerExp::execute","Not enough memory");
                  //return 0;
              }
          }
      }
      ii++;
    }

  return 1;
}

inline Map* getMap(int adcNr) {
  return mapExp + adcNr%24;
}

int HShowerUnpackerExp::getModule(int adcNr) {
  if (adcNr > 767) adcNr -= 768;

  return getMap(adcNr)->module;
}

int HShowerUnpackerExp::getColumn(int adcNr) {
  if (getModule(adcNr) != post)
    return adcNr/24;
  else
    return 31 - adcNr/24;
}

int HShowerUnpackerExp::getRow(int adcNr) {
  if (adcNr > 767) {
     adcNr -= 768;
     return getMap(adcNr)->row + rowOffset;
  }
  else
     return getMap(adcNr)->row;
}

int HShowerUnpackerExp::dump() {
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

ClassImp(HShowerUnpackerExp)
