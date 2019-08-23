// File: hrichunpackercal99.cc
//
// Author: Walter Schoen <W.Schoen@physik.tu-muenchen.de>
// Last update by Walter Schoen: 99/12/17 17:29:47
// Last update by Thomas Eberl: 02/10/25 14:57:34
//
#include "hrichunpackercal99.h"
#include "hrichutilfunc.h"
#include "hcategory.h"
#include "hlocation.h"
#include "hrichraw.h"
#include "hevent.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hcategory.h"
#include "hldsubevt.h"
#include "hdebug.h"
#include "hruntimedb.h"
#include "richdef.h"
#include "hrichdetector.h"
#include "hrichcalpar.h"
#include "hrichcal.h"
#include <stdio.h>
#include <stdlib.h>

HRichUnpackerCal99::HRichUnpackerCal99(int rId,int strtEvt=0,Bool_t isPed=kFALSE,Bool_t verbose=kFALSE,Int_t iSloppyL=0) {
    richId = rId;
    startEvt=strtEvt;
    isPedestal=isPed;
    kverbose=verbose;
    iSloppyLevel=iSloppyL;
    nSubEvtCntr = 0;
    nSecMisMatchCntr = 0;
    pRawCat=NULL;
    fEventNr = -1;
    if (startEvt !=0) cout<<"For SubEvtId "<<richId<<
			  " starting to unpack at evt # "<<startEvt<<endl;
    switch (richId){
    case 100:
	SubEvtIdSec=0;
	break;
    case 101:
	SubEvtIdSec=0;
	break;
    case 102:
	SubEvtIdSec=1;
	break;
    case 103:
	SubEvtIdSec=1;
	break;
    case 104:
	SubEvtIdSec=2;
	break;
    case 105:
	SubEvtIdSec=2;
	break;
    case 106:
	SubEvtIdSec=3;
	break;
    case 107:
	SubEvtIdSec=3;
	break;
    case 108:
	SubEvtIdSec=4;
	break;
    case 109:
	SubEvtIdSec=4;
	break;
    case 110:
	SubEvtIdSec=5;
	break;
    case 111:
	SubEvtIdSec=5;
	break;
    }
   
}
HRichUnpackerCal99::~HRichUnpackerCal99(void) {
}
Bool_t HRichUnpackerCal99::init(void) {
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
    initMappingPar();
    return kTRUE;
}

void HRichUnpackerCal99::initMappingPar(void){
    HRuntimeDb* rtdb=gHades->getRuntimeDb();
    pMapPar = rtdb->getContainer("RichMappingParameters");
}

int HRichUnpackerCal99::getSubEvtId(void) const {
    return richId; 
}

void HRichUnpackerCal99::printDataWord(DataWord &addr,int ch){
    cout<<"*****************************************************************************"<<endl;
    cout<<"Event Nr: "<<fEventNr<<endl;
    cout<<"SEC: "<<addr.sector<<" RC: "<<addr.controler<<" PORT: "<<addr.port<<" MOD: "
	<<addr.modul<<" CH: "<<addr.channel<<" CHRG: "<<ch<<endl;
}

void HRichUnpackerCal99::printMapping(DataWord &addr){
    cout<<"Event Nr: "<<fEventNr<<endl;
    cout<<"Col: "<<getMappingPar()->getCol(getUpi(addr))<<endl;
    cout<<"Row: "<<getMappingPar()->getRow(getUpi(addr))<<endl;
    cout<<"isValidUpi: "<<getMappingPar()->isValidUpi(getUpi(addr))<<endl;
    HLocation loc;
    loc.set(3,0,0,0);
    loc.setOffset(getMappingPar()->getCol(getUpi(addr)));
    loc.setIndex(1,getMappingPar()->getRow(getUpi(addr)));
    loc.setIndex(0,addr.sector);
    printCalParCell(loc);
    cout<<"*****************************************************************************"<<endl;
}

Bool_t  HRichUnpackerCal99::testSubEvt(DataWord &addr,int ch){
    if (kverbose) printDataWord(addr,ch);
    Bool_t decision = kTRUE;
    if (iSloppyLevel < 3){
	int upi = getUpi(addr);
	//	cout<<"testSubEvt: upi: "<<upi<<endl;
	// *** check if address is within reasonable boundaries
	if (ch >=0 && ch<1024){
	    if (addr.channel>=0 && addr.channel<64){ 
		if (addr.modul>=0 && addr.modul<5){ 
		    if (addr.port>=0 && addr.port<8){
			if (addr.controler >=0 && addr.controler<2){
			    if (addr.sector>=0 && addr.sector<6){
				if (upi>=0 && upi<17464){//redundant 
				    if (getMappingPar()->isValidUpi(upi)){//
					if (getMappingPar()->getCol(upi)>0 
					    && getMappingPar()->getCol(upi)<92){//redundant
					    //cout<<"col: "<<getMappingPar()->getCol(upi)<<endl;
					    if (getMappingPar()->getRow(upi)>0 
						&& getMappingPar()->getRow(upi)<90){//redundant
						//cout<<"row: "<<getMappingPar()->getRow(upi)<<endl;
						if (kverbose) printMapping(addr);
					    }else{
						if (iSloppyLevel < 2){
						    cout<<"SubEvtId "<<getSubEvtId()<<":ERROR (evt nr: "<<fEventNr<<") in" 
							<<" HRichUnpackerCal99::testSubEvt ***"
							<<" unpacked row "
							<<getMappingPar()->getRow(upi)
							<<" is out of bounds [0;89]"<<endl;
						}
						decision=kFALSE; 
					    }
					}else{
					    if (iSloppyLevel < 2){
						//cout<<"SloppyLevel: "<<iSloppyLevel<<endl;
						cout<<"SubEvtId "<<getSubEvtId()<<":ERROR (evt nr: "<<fEventNr<<") in" 
						    <<" HRichUnpackerCal99::testSubEvt ***"
						    <<"  unpacked col "
						    <<getMappingPar()->getCol(upi)
						    <<" is out of bounds [0;91]"<<endl;
					    }
					    decision=kFALSE;
					}
				    }else{
					//cout<<"not a valid upi"<<endl;
					if((getMappingPar()->isUnConnCh(upi))){
					  //TRUE
					  //cout<<"not a valid upi"<<endl;
					    if(isPedestal){
					      //TRUE TRUE -> FALSE
						nSubEvtCntr--;
						decision=kFALSE;
					    }else{ //FALSE TRUE -> FALSE
						if (iSloppyLevel < 2){
						    cout<<"SubEvtId "<<getSubEvtId()<<":ERROR (evt nr: "<<fEventNr<<") in"
							<<" HRichUnpackerCal99::testSubEvt ***"
							<<" unpacked channel "<<upi
							<<" exists, but is not connected to any pad."
							<<" It is allowed only in a pedestal run !"<<endl;
						}
						decision=kFALSE;
					    }
					}else{ //FALSE (FALSE FALSE | TRUE FALSE -> FALSE
					    if (iSloppyLevel < 2){
						    cout<<"SubEvtId "<<getSubEvtId()<<":ERROR (evt nr: "<<fEventNr<<") in" 
							<<" HRichUnpackerCal99::testSubEvt ***"
							<<" unpacked channel "<<upi
							<<" does not exist"<<endl;
					    }
						decision=kFALSE;
					}
				    }
				}else{
				    if (iSloppyLevel < 2){
					cout<<"SubEvtId "<<getSubEvtId()<<":ERROR (evt nr: "<<fEventNr<<") in" 
					    <<" HRichUnpackerCal99::testSubEvt ***"
					    <<" unpacked channel "<<upi
					    <<" is out of bounds [0;17463]"<<endl;
				    }
				    decision=kFALSE;
				}
			    }else{
				if (iSloppyLevel < 2){
				    cout<<"SubEvtId "<<getSubEvtId()<<":ERROR (evt nr: "<<fEventNr<<") in" 
					<<" HRichUnpackerCal99::testSubEvt ***"
					<<" unpacked sector "<<addr.sector
					<<" is out of bounds [0;5]"<<endl;
				}
				decision=kFALSE;
			    }
			}else{
			    if (iSloppyLevel < 2){
				cout<<"SubEvtId "<<getSubEvtId()<<":ERROR (evt nr: "<<fEventNr<<") in" 
				    <<" HRichUnpackerCal99::testSubEvt ***"
				    <<" unpacked controler "<<addr.controler
				    <<" is out of bounds [0;1]"<<endl;
			    }
			    decision=kFALSE;
			}
		    }else{
			if (iSloppyLevel < 2){
			    cout<<"SubEvtId "<<getSubEvtId()<<":ERROR (evt nr: "<<fEventNr<<") in" 
				<<" HRichUnpackerCal99::testSubEvt ***"
				<<" unpacked port "<<addr.port
				<<" is out of bounds [0;4]"<<endl;
			}
			decision=kFALSE;
		    }
		}else{
		    if (iSloppyLevel < 2){
			cout<<"SubEvtId "<<getSubEvtId()<<":ERROR (evt nr: "<<fEventNr<<") in" 
			    <<" HRichUnpackerCal99::testSubEvt ***"
			    <<" unpacked module "<<addr.modul
			    <<" is out of bounds [0;4]"<<endl;
			printMapping(addr);
			printDataWord(addr,ch);
		    }
		    decision=kFALSE;
		}
	    }else{
		if (iSloppyLevel < 2){
		    cout<<"SubEvtId "<<getSubEvtId()<<":ERROR (evt nr: "<<fEventNr<<") in" 
			<<" HRichUnpackerCal99::testSubEvt ***"
			<<" unpacked channel "<<addr.channel
			<<" is out of bounds [0;63]"<<endl;
		    printMapping(addr);
		    printDataWord(addr,ch);
		}
		decision=kFALSE;
	    }
	}else{
	    if (iSloppyLevel < 2){
		cout<<"SubEvtId "<<getSubEvtId()<<":ERROR (evt nr: "<<fEventNr<<") in" 
		    <<" HRichUnpackerCal99::testSubEvt ***"
		    <<" unpacked charge "<<ch
		    <<" is out of bounds [0;1023]"<<endl;
	    }
	    decision=kFALSE;
	}
    }
    if (decision == kFALSE) nSubEvtCntr++;
    return decision;
}

int HRichUnpackerCal99::getUpi(DataWord &addr){
    return addr.controler*10000 + addr.port*1000 + addr.modul*100 + addr.channel;
}

int HRichUnpackerCal99::execute() {
  HLocation loc;
  loc.set(3,0,0,0);
  fEventNr++;
  if (fEventNr>=startEvt){
      if (pSubEvt) {   
	  for (UInt_t i = 0; i < pSubEvt->getDataLen(); i++) {
	      charge = pSubEvt->getData()[i]  & 0x3FF;   
	      dataword.channel =  (pSubEvt->getData()[i] >> 10) & 0x3F;
	      dataword.modul =  (pSubEvt->getData()[i] >> 16) & 0x7;
	      dataword.port =  (pSubEvt->getData()[i] >> 19) & 0x7;
	      dataword.controler = (pSubEvt->getData()[i] >>22) & 0x1 ; 
	      dataword.sector =  (pSubEvt->getData()[i] >>23) & 0x7;
	      if (dataword.sector == SubEvtIdSec){ //check sector consistency
		if (testSubEvt(dataword,charge)){ //check if pad is valid
		  loc.setOffset(getMappingPar()->getCol(getUpi(dataword)));
		  loc.setIndex(1,getMappingPar()->getRow(getUpi(dataword)));
		  loc.setIndex(0,dataword.sector);
		  if (getMappingPar()->isReadOut(loc[2],loc[1])){
		    if (loc.getIndex(1) >= 0 && loc.getOffset() >= 0) {
		      HRichCalParCell *calparcell = NULL;
		      HRichCal *pCalCell = NULL;
		      calparcell = (HRichCalParCell*) ((HRichCalPar*)getCalPar())
			->getObject(loc);
		      // subtract measured mean value and add a random nb
		      // [0;1] to account for the cut-off in the ADC
		      Float_t fCharge = charge    * 
			  calparcell->getSlope()  - 
			  calparcell->getOffset() + 
			  rand()/(double)RAND_MAX   ; 
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
		  }else{
		    cout<<"SubEvtId "<<getSubEvtId()<<" (HRichUnpackerCal99::execute) Error in event :"
			<<fEventNr<<endl;
		    cout<<"Pad col: "<<loc[2]<<" row: "<<loc[1]
			<<" sec: "<<loc[0]<<" chrg: "<<charge
			<<" is invalid and slipped through testSubEvt"<<endl;
		    cout<<"printDataWord says:"<<endl;
		    printDataWord(dataword,charge);
		    cout<<"printMapping says:"<<endl;
		    printMapping(dataword);
		  }
		}else{
		  if (iSloppyLevel<2) {
		    if (!isPedestal){
		      cout<<"RICH: Dataword skipped in Event: "
			  <<fEventNr<<endl;
		    }
		  }
		}
	      }else{
		nSecMisMatchCntr++;
		if (iSloppyLevel<1){
		  cout<<"Error in event :"<<fEventNr<<endl;
		  cout<<"Unpacked sector info: "<<dataword.sector<<
		    "  SubEvtId claims sector: "<<SubEvtIdSec<<endl;
		  cout<<"RICH: Dataword skipped"<<endl;
		}
	      }
	  }//end of unpacking sub event
      }// end of if data in subevent      
  }// end of start event condition   
  return kTRUE;
}

Bool_t HRichUnpackerCal99::finalize(void) {
  Int_t id = getSubEvtId();
  //cout<<"SubEvtId:"<<id<<" RICH: Number of evts processed: "<<fEventNr+1<<endl;
  //cout<<"SubEvtId:"<<id<<" RICH: Sloppy level:             "<<iSloppyLevel<<endl;
  //cout<<"SubEvtId:"<<id<<" RICH: Corrupt datawords:        "<<nSubEvtCntr<<endl;
  //cout<<"SubEvtId:"<<id<<" RICH: Sector mismatch errors:   "<<nSecMisMatchCntr<<endl;
  return kTRUE;
}

void HRichUnpackerCal99::printCalParCell(HLocation& loc) {
    HRichCalParCell *calparcell = NULL;
    calparcell = (HRichCalParCell*) ((HRichCalPar*)getCalPar())
	->getObject(loc);
    cout<<"SEC: "<<loc[0]<<" ROW: "<<loc[1]<<" COL: "<<loc[2]<<
	" OFFSET: "<<calparcell->getOffset()<<" SIGMA : "<<
	calparcell->getSigma()<<" SLOPE : "<<calparcell->getSlope()<<
	endl;
} 

void HRichUnpackerCal99::initCalPar() {
  HRuntimeDb* rtdb=gHades->getRuntimeDb();
  fCalPar = rtdb->getContainer("RichCalPar");
}

ClassImp(HRichUnpackerCal99) 







