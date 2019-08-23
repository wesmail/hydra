// File: HRichUnpackerCal99.cc
//
// Author: Walter Schoen <W.Schoen@physik.tu-muenchen.de>
// Last update by Walter Schoen: 99/12/17 17:16:06
//
#include "hrichunpackercal99.h"
#include "hrichcalibrater.h"
#include "hcategory.h"
#include "hlocation.h"
#include "hevent.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hcategory.h"
#include "hldsubevt.h"
#include "hdebug.h"
#include "richdef.h"
#include "hruntimedb.h"
#include "hrichdetector.h"
#include "hiterator.h"
#include "hmatrixcatiter.h"
#include "hlocation.h"
#include "hrichcalpar.h"
#include "hrichcal.h"
#include <stdio.h>

HRichUnpackerCal99::HRichUnpackerCal99(int rId) {
    richId = rId;
    pCalCat=NULL;
    fCalPar = NULL;
    fEventNr = 0;
}

HRichUnpackerCal99::~HRichUnpackerCal99(void) {
}
Bool_t HRichUnpackerCal99::init(void) {
    TFile *pFile = new TFile("uipfile_new.root","read","Testfile",1);
    if(pFile){
	uipTable       = (TVector*) pFile->Get("uip");
	uip2xyPosition = (TVector*) pFile->Get("pads_xy");
	uip2xPosition  = (TVector*) pFile->Get("pads_x");
	uip2yPosition  = (TVector*) pFile->Get("pads_y"); 
	xyPosition2uip = (TVector*) pFile->Get("xy->uip");
    }
    else cout << "problems opening upifile" << endl;
    HRichDetector *pRichDet = (HRichDetector*)gHades->getSetup()
	->getDetector("Rich");
    pCalCat=gHades->getCurrentEvent()->getCategory(catRichCal);
    if (!pCalCat) {
	pCalCat=pRichDet->buildCategory(catRichCal);
	if (!pCalCat) return kFALSE;
	else gHades->getCurrentEvent()
		 ->addCategory(catRichCal, pCalCat, "Rich");
    }
    initCalPar();
    return kTRUE;
}

int HRichUnpackerCal99::getSubEvtId(void) const {
    return richId; 
}

int HRichUnpackerCal99::execute() {
    HLocation loc,loc2;
    loc.set(3,0,0,0);
    fEventNr++;
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
		    HRichCalParCell *calparcell = NULL;
		    HRichCal *pCalCell = NULL;
		    calparcell = (HRichCalParCell*) ((HRichCalPar*)getCalPar())
			->getObject(loc);
		    Float_t fCharge;
		    fCharge = charge - calparcell->getOffset(); 
		    if (fCharge > 0.0) { 
			pCalCell = (HRichCal *)pCalCat->getSlot(loc);
			if (pCalCell != NULL) {
			    pCalCell = new(pCalCell) HRichCal;      
			    pCalCell->setCharge(fCharge);
			    pCalCell->setSector(loc[0]);
			    pCalCell->setRow(loc[1]);
			    pCalCell->setCol(loc[2]);
			    pCalCell->setEventNr(fEventNr);
			}
		    }
		}
	    }
	}
    }
    return kTRUE;
}
void HRichUnpackerCal99::initCalPar() {
    HRichDetector *pRichDet = (HRichDetector*)gHades->getSetup()
	->getDetector("Rich");
    HRuntimeDb* rtdb=gHades->getRuntimeDb();
    fCalPar = rtdb->getContainer("RichCalPar");
    if (!fCalPar) {
	fCalPar= new HRichCalPar;
	fCalPar->SetName("RichCalPar");
	if (fCalPar) {
	    ((HRichCalPar*)fCalPar)->
		setSetup(pRichDet->getSectors(),
			 pRichDet->getRows(),
			 pRichDet->getColumns());
	    rtdb->addContainer(fCalPar);
	}
    }
}
void  HRichUnpackerCal99::diagnose(int m,int p,int c,int s) {
    cout<<"MODUL,PORT,CONTROLER,SECTOR "
	<<m<<","<<p<<", "<<c<<", "<<s<<endl;
}
Bool_t HRichUnpackerCal99::finalize(void) {
    ((HRichCalPar*)fCalPar)->PrintCalPar();
    return kTRUE;
}

ClassImp(HRichUnpackerCal99) 








