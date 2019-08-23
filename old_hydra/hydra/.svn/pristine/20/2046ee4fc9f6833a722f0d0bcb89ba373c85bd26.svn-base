// File: richunpackold.cc
//
//*-- Author : Walter Karig 
//*-- Modified : 25/03/98 by Manuel Sanchez 
//

/////////////////////////////////////////////////
//HRichUnpackerOld
//
//  This class gets the Rich data stored in a LMD event and puts it into
//  a HEvent
//
//////////////////////////////////////////////////
#include "hrichunpackerold.h"
#include "hevent.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hcategory.h"
#include "hldsubevt.h"
#include "hdebug.h"
#include "hlocation.h"
#include "hrichraw.h"
#include "richfumble.h"

const int richId=8;

HRichUnpackerOld::HRichUnpackerOld(void) {
  pRawCat=NULL;
}

HRichUnpackerOld::HRichUnpackerOld(HRichUnpackerOld &unpacker) {
  pSubEvt=unpacker.pSubEvt;
  pRawCat=unpacker.pRawCat;
}

HRichUnpackerOld::~HRichUnpackerOld(void) {
}

Bool_t HRichUnpackerOld::init(void) {
  pRawCat=gHades->getCurrentEvent()->getCategory(catRichRaw);
  if (!pRawCat) {
    pRawCat=gHades->getSetup()->getDetector("Rich")
                             ->buildCategory(catRichRaw);
    if (!pRawCat) {
      return kFALSE;
    } else {
      gHades->getCurrentEvent()->addCategory(catRichRaw,pRawCat,"Rich");
    }
  }
  return kTRUE;
}


int HRichUnpackerOld::getSubEvtId(void) const {
  return richId;
}

int HRichUnpackerOld::execute() {
  HLocation loc,loc2;
  loc.set(3,0,0,0);
  //cout << "\tDentro del unpacker" << endl;
  if (pSubEvt) {
    //cout << "\t\tHay datos:" << endl;
    for (UInt_t i = 0; i < pSubEvt->getDataLen(); i++) {
      int camId;
      int camData;
      camId = pSubEvt->getData()[i] & 0xffff;
      camData = (pSubEvt->getData()[i] >> 16) & 0xffff;
      //cout << "camId=" << camId << endl;
      if (camId >= 1 && camId < (1 + 4096)) {
        int dramsAddr = camId - 1;
	loc.setIndex(1,getRow(dramsAddr));
	loc.setOffset(getColumn(dramsAddr));
	//cout << "Row=" << getRow(dramsAddr) << " Col=" << getColumn(dramsAddr) << endl;
        if (loc.getIndex(1) >= 0 && loc.getOffset() >= 0) {
	  //loc.Dump();
          HRichRaw* pCell =
            (HRichRaw*) pRawCat->getSlot(loc); //Item(0)->getItem(row)->getItem(column);
	  if (pCell!=NULL) {
            pCell=new(pCell) HRichRaw(camData);
            pCell->setSector(loc[0]);
            pCell->setRow(loc[1]);
            pCell->setCol(loc[2]);
          }
        }
      }
    }
  }
  return kTRUE;
}

int HRichUnpackerOld::getRow(int pad) {
  if (pad >= 0 && pad < 64*64)
    return padXY[pad][0] - 8;   // mapping table is for 80x80 rich
  else
    return -1;
}

int HRichUnpackerOld::getColumn(int pad) {
  if (pad >= 0 && pad < 64*64)
    return padXY[pad][1] - 8;   // mapping table is for 80x80 rich
  else
    return -1;
} 
              
ClassImp(HRichUnpackerOld) 
