// File: richunpack.cc
//
//*-- Author : Walter Karig 
//*-- Modified : 25/03/98 by Manuel Sanchez 
//

//_HADES_CLASS_DESCRIPTION 
/////////////////////////////////////////////////
//HRichUnpacker
//
//  This class gets the Rich data stored in a LMD event and puts it into
//  a HEvent
//
//////////////////////////////////////////////////
#include "hrichunpacker.h"
#include "hcategory.h"
#include "hlocation.h"
#include "hrichraw.h"
#include "hevent.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hcategory.h"
#include "hldsubevt.h"
#include "hdebug.h"
#include "richfumble.h"

const Int_t richId=8;

HRichUnpacker::HRichUnpacker(void) {
  pRawCat=NULL;
  fEventNr = 0;
}

HRichUnpacker::HRichUnpacker(HRichUnpacker &unpacker) {
  pSubEvt=unpacker.pSubEvt;
  pRawCat=unpacker.pRawCat;
  fEventNr = 0;
}

HRichUnpacker::~HRichUnpacker(void) {
}

Bool_t HRichUnpacker::init(void) {
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


Int_t HRichUnpacker::getSubEvtId(void) const {
  return richId; 
}

Int_t HRichUnpacker::execute() {
  HLocation loc,loc2;
  loc.set(3,0,0,0);
  //cout << "\tDentro del unpacker" << endl;
  fEventNr++;
  if (pSubEvt) {
    //cout << "\t\tHay datos:" << endl;
    for (UInt_t i = 0; i < pSubEvt->getDataLen(); i++) {
      Int_t camId;
      Int_t camData;
      camId = pSubEvt->getData()[i] & 0xffff;
      camData = (pSubEvt->getData()[i] >> 16) & 0xffff;
      //cout << "camId=" << camId << endl;
      if (camId >= 1 && camId < (1 + 4096)) {
        Int_t dramsAddr = camId - 1;
	loc.setIndex(1,getRow(dramsAddr));
	loc.setOffset(getColumn(dramsAddr));
//      cout <<"Row="<<getRow(dramsAddr)<<" Col="<<getColumn(dramsAddr)<<endl;
        if (loc.getIndex(1) >= 0 && loc.getOffset() >= 0) {
	  //loc.Dump();
          HRichRaw* pCell = (HRichRaw*) pRawCat->getSlot(loc);
//        Item(0)->getItem(row)->getItem(column);
	  if (pCell!=NULL) {
            pCell=new(pCell) HRichRaw(camData);
            pCell->setSector(loc[0]);
            pCell->setRow(loc[1]);
            pCell->setCol(loc[2]);
//            pCell->setEventNr(fEventNr);
          }
        }
      }
    }
  }
  return kTRUE;
}

Int_t HRichUnpacker::getRow(Int_t pad) {
  if (pad >= 0 && pad < 64*64)
    return padXY[pad][0] - 8;   // mapping table is for 80x80 rich
  else
    return -1;
}

Int_t HRichUnpacker::getColumn(Int_t pad) {
  if (pad >= 0 && pad < 64*64)
    return padXY[pad][1] - 8;   // mapping table is for 80x80 rich
  else
    return -1;
} 
              
ClassImp(HRichUnpacker) 
