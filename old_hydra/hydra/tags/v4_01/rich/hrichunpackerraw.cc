// File: hrichunpacker100.cc
//
// Author: Walter Schoen <W.Schoen@physik.tu-muenchen.de>
// Last update by Walter Schoen: 99/12/17 17:29:47
// 
#include "hrichunpackerraw.h"
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

HRichUnpackerRaw::HRichUnpackerRaw(int rId) {
    richId = rId;
  pRawCat=NULL;
}
HRichUnpackerRaw::~HRichUnpackerRaw(void) {
}
Bool_t HRichUnpackerRaw::init(void) {
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

int HRichUnpackerRaw::getSubEvtId(void) const {
    return richId; 
}

int HRichUnpackerRaw::execute() {
    HLocation loc,loc2;
  loc.set(3,0,0,0);
  int counter =0;
  if (pSubEvt) {   
      int uipId;
      for (UInt_t i = 0; i < pSubEvt->getDataLen(); i++) {
	  charge = pSubEvt->getData()[i]  & 0x3FF;   
	  upi.channel =  (pSubEvt->getData()[i] >> 10) & 0x3F;
	  upi.modul =  (pSubEvt->getData()[i] >> 16) & 0x7;
	  upi.port =  (pSubEvt->getData()[i] >> 19) & 0x7;
	  upi.controler = (pSubEvt->getData()[i] >>22) & 0x1 ; 
	  upi.sector =  (pSubEvt->getData()[i] >>23) & 0x7;
	  //diagnose(upi.modul,upi.port,upi.controler,upi.sector);
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
		      counter ++;
		  }
	      }
	  }
      }
  }
  return kTRUE;
}
void HRichUnpackerRaw::diagnose(int m,int p,int c,int s) {
    cout<<"MODUL,PORT,CONTROLER,SECTOR "
	<<m<<","<<p<<", "<<c<<", "<<s<<endl;
}              
ClassImp(HRichUnpackerRaw) 







