// File: hrichunpackerraw.cc
//
// Author: Thomas Eberl <Thomas.Eberl@physik.tu-muenchen.de>
// Last update by Thomas Eberl: 00/05/11 22:15:16
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
#include "richdef.h"

HRichUnpackerRaw::HRichUnpackerRaw(int rId) {
    richId   = rId;
    pRawCat  = NULL;
    fEventNr = -1; 
}

HRichUnpackerRaw::~HRichUnpackerRaw(void) {
}

Bool_t HRichUnpackerRaw::init(void) {

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
  initMapping();
  return kTRUE;
}
Bool_t HRichUnpackerRaw::finalize(void) {
    return kTRUE;
}
int HRichUnpackerRaw::getSubEvtId(void) const {
    return richId; 
}

int HRichUnpackerRaw::execute() {
    HLocation loc;
    loc.set(3,0,0,0);
    fEventNr++;
    //  cout<<fEventNr<<endl;
    //  if (fEventNr >=0){
    if (pSubEvt) {   
	//    cout<<"Length of Subevt :"<<pSubEvt->getDataLen()<<endl;
	for (UInt_t i = 0; i < pSubEvt->getDataLen(); i++) {
	    charge =   pSubEvt->getData()[i]        & 0x3FF;  
	    col    =  (pSubEvt->getData()[i] >> 10) & 0xFF ;
	    row    =  (pSubEvt->getData()[i] >> 18) & 0xFF ;
	    sec    =  (pSubEvt->getData()[i] >> 26) & 0x3F  ;
	    //cout<<"sec: "<<sec<<" row: "<<row<<" col: "<<col<<" charge: "<<charge<<endl;
	    loc.setOffset(col);
	    loc.setIndex(1,row);
	    loc.setIndex(0,sec);
	    if ( padReadOut(col,row) ){
	      if (diagnose(charge,col,row,sec,fEventNr)){
		// if (row != 128 || col != 128){
		if (loc.getIndex(1) >= 0 && loc.getOffset() >= 0) {
		    HRichRaw* pCell =
			(HRichRaw*) pRawCat->getSlot(loc); 
		    if (pCell!=NULL) {
			pCell=new(pCell) HRichRaw(charge);
			
			pCell->setSector(loc[0]);
			pCell->setRow(loc[1]);
			pCell->setCol(loc[2]);
//			pCell->setEventNr(fEventNr);
		    }
		}
	      }
		//	    } else {
		//return kFALSE; // skipping event with data errors
	    }	  
		  
		
		
		
	// }
	}
    }
    return kTRUE;
}
Bool_t HRichUnpackerRaw::padReadOut(int h_col,int h_row){
    if (h_row != 128 && h_col != 128){
	return kTRUE;
    }else{
	return kFALSE;
    }
}

Bool_t HRichUnpackerRaw::diagnose(int h_charge,int h_col,int h_row,int h_sec, int h_fEventNr) {
    Bool_t bCharge,bCol,bRow,bSec,bReadOut;
    bCharge = bCol = bRow = bSec = bReadOut = kTRUE;
    
    if (h_row != 128 && h_col != 128 && h_col<=92 && h_row<=90){
	if ( !(*xyuiplog)(h_col*100+h_row) ){
	    cout<<"ALARM: not a valid read-out pad  ---> pad (col,row,charge,sec,evt = "<<h_col<<","<<h_row<<","<<h_charge<<","<<h_sec<<","<<h_fEventNr<<" )"<<" skipped"<<endl;
	    bReadOut = kFALSE;
	}
    }
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
    
    if (bCharge && bCol && bRow && bSec && bReadOut){ 
	return kTRUE;
    }else{
	return kFALSE;
    }
}
void HRichUnpackerRaw::initMapping(){
TFile *pFile = new TFile("uipfile_new.root","read","Testfile",1);
    if(pFile){
	xyuiplog = (TVector*) pFile->Get("xy->uip:log");
    }
    else cout << "problems opening upifile" << endl;
}             
ClassImp(HRichUnpackerRaw) 







