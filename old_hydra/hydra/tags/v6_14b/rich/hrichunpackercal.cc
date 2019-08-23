// File: hrichunpackercal.cc
//
// Author: Thomas Eberl <Thomas.Eberl@physik.tu-muenchen.de>
// Last update by Thomas Eberl: 00/04/27 11:24:44
//

#include "hrichunpackercal.h"
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


HRichUnpackerCal::HRichUnpackerCal(int rId) {
    richId = rId;
    pCalCat=NULL;
    fCalPar = NULL;
    fEventNr = 0;
}

HRichUnpackerCal::~HRichUnpackerCal(void) {
}
Bool_t HRichUnpackerCal::init(void) {

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

int HRichUnpackerCal::getSubEvtId(void) const {
    return richId; 
}

int HRichUnpackerCal::execute() {
    HLocation loc;
    loc.set(3,0,0,0);
    fEventNr++;
    if (pSubEvt) {
	for (UInt_t i = 0; i < pSubEvt->getDataLen(); i++) {
	    charge =   pSubEvt->getData()[i]        & 0x3FF;  
	    col    =  (pSubEvt->getData()[i] >> 10) & 0xFF ;
	    row    =  (pSubEvt->getData()[i] >> 18) & 0xFF ;
	    sec    =  (pSubEvt->getData()[i] >> 26) & 0x3F  ;
	    
	    loc.setOffset(col);
	    loc.setIndex(1,row);
	    loc.setIndex(0,sec);
	    if (row != 255 && col != 255){
		if (diagnose(charge,col,row,sec,fEventNr)){
		    
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
//				pCalCell->setEventNr(fEventNr);
			    }
			}
		    }
		}
	    }
	}
    }
    return kTRUE;
}
void HRichUnpackerCal::initCalPar() {
    HRichDetector *pRichDet = (HRichDetector*)gHades->getSetup()
	->getDetector("Rich");
    HRuntimeDb* rtdb=gHades->getRuntimeDb();
    fCalPar = rtdb->getContainer("RichCalPar");
    if (fCalPar) {
        ((HRichCalPar*)fCalPar)->
	    setSetup(pRichDet->getSectors(),
	    	     pRichDet->getRows(),
		     pRichDet->getColumns());
    }
}
Bool_t HRichUnpackerCal::diagnose(int h_charge,int h_col,int h_row,int h_sec, int h_fEventNr) {
    Bool_t bCharge,bCol,bRow,bSec;
    bCharge = bCol = bRow = bSec = kTRUE;
    if (h_charge<0 || h_charge>=1023){
	cout<<"ALARM: charge over 1022 :"<<h_charge<<" ---> pad (col,row,charge,sec,evt = "<<h_col<<","<<h_row<<","<<h_charge<<","<<h_sec<<","<<h_fEventNr<<" )"<<" skipped"<<endl;
	bCharge = kFALSE;
    }
    if (h_col<0 || h_col>=92){
	cout<<"ALARM: col over 92 :"<<h_col<<" ---> pad (col,row,charge,sec,evt = "<<h_col<<","<<h_row<<","<<h_charge<<","<<h_sec<<","<<h_fEventNr<<" )"<<" skipped"<<endl;
	bCol = kFALSE;
    }
    if (h_row<0 || h_row>=90){
	cout<<"ALARM: row over 90 :"<<h_row<<" ---> pad (col,row,charge,sec,evt = "<<h_col<<","<<h_row<<","<<h_charge<<","<<h_sec<<","<<h_fEventNr<<" )"<<" skipped"<<endl;
	bRow = kFALSE;
    }
    if (h_sec<0 || h_sec>=6){
	cout<<"ALARM: sec wrong :"<<h_sec<<" ---> pad (col,row,charge,sec,evt = "<<h_col<<","<<h_row<<","<<h_charge<<","<<h_sec<<","<<h_fEventNr<<" )"<<" skipped"<<endl;
	bSec = kFALSE;
    }
    if (bCharge && bCol && bRow && bSec){ 
	return kTRUE;
    }else{
	return kFALSE;
    }
}
Bool_t HRichUnpackerCal::finalize(void) {
    ((HRichCalPar*)fCalPar)->PrintCalPar();
    return kTRUE;
}

ClassImp(HRichUnpackerCal) 








