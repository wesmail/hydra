// File: hrichunpacker.cc
//
// Author: Walter Schoen <W.Schoen@physik.tu-muenchen.de>
// Last update by Walter Schoen: 99/09/08 11:17:45
//
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
#include <stdio.h>
const int richId=213;
//const int richId=100;
HRichUnpacker::HRichUnpacker(void) {
  pRawCat=NULL;
}

HRichUnpacker::~HRichUnpacker(void) {
}

Bool_t HRichUnpacker::init(void) {
    cout<< " initialising unpacker" << endl;
    TFile *pFile = new TFile("uipfile_new.root","read","Testfile",1);
    if(pFile){
	uipTable       = (TVector*) pFile->Get("uip");
	uip2xyPosition = (TVector*) pFile->Get("pads_xy");
	uip2xPosition  = (TVector*) pFile->Get("pads_x");
	uip2yPosition  = (TVector*) pFile->Get("pads_y"); 
	xyPosition2uip = (TVector*) pFile->Get("xy->uip");
    }
    else cout << "problems opening upifile" << endl;
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


int HRichUnpacker::getSubEvtId(void) const {
  return richId; 
}

int HRichUnpacker::execute() {
  HLocation loc,loc2;
  loc.set(3,0,0,0);
  if (pSubEvt) {
           cout <<"nevent= " << pSubEvt->getDataLen() << endl;
           cout <<"           " << endl;	
	   cout <<"           " << endl;		   
      int uipId;
    for (UInt_t i = 0; i < pSubEvt->getDataLen(); i++) {
      charge = pSubEvt->getData()[i]  & 0x3FF;
      upi.channel =  (pSubEvt->getData()[i] >> 10) & 0x3F;
      upi.modul =  (pSubEvt->getData()[i] >> 16) & 0x7;
      upi.port =  (pSubEvt->getData()[i] >> 19) & 0x7;
      upi.controler = (pSubEvt->getData()[i] >>22) & 0x1 ; 
      upi.sector =  (pSubEvt->getData()[i] >>23) & 0x7;
      uipId = getUipIdent(upi.controler,upi.port,upi.modul,upi.channel);
      if ((*uipTable)(uipId)) {
	loc.setOffset((*uip2xPosition)(uipId));
	loc.setIndex(1,(*uip2yPosition)(uipId));
        if (loc.getIndex(1) >= 0 && loc.getOffset() >= 0) {
          HRichRaw* pCell =
            (HRichRaw*) pRawCat->getSlot(loc); 
	  if (pCell!=NULL) {
            pCell=new(pCell) HRichRaw(charge);
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
              
ClassImp(HRichUnpacker) 





