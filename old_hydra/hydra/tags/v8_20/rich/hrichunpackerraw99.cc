// File: hrichunpackerraw99.cc
//
// Author: Walter Schoen <W.Schoen@physik.tu-muenchen.de>
// Last update by Walter Schoen: 99/12/17 17:29:47
// Last update by Thomas Eberl: 02/10/25 14:59:28
//*-- Modified : Tue Feb 15 18:50:45 CET 2005 martin.jurkovic@ph.tum.de
// Event number is obtained from the event header				  
//
#include "hrichunpackerraw99.h"
#include "hcategory.h"
#include "hlocation.h"
#include "hrichraw.h"
#include "hevent.h"
#include "heventheader.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hcategory.h"
#include "hldsubevt.h"
#include "hdebug.h"
#include "hruntimedb.h"
#include "TRandom.h"
#include <stdio.h>
#include <stdlib.h>

HRichUnpackerRaw99::HRichUnpackerRaw99(Int_t rId,Int_t strtEvt,Bool_t isPed,Bool_t verbose,Int_t iSloppyL, Bool_t swap_enabled) {
    richId = rId;
    startEvt=strtEvt;
    isPedestal=isPed;
    kverbose=verbose;
    iSloppyLevel=iSloppyL;
    
    //Introduced April 2003 to compensate for the cable swapping in sector 3 during nov01 beamtime.
    //Use only to analyze nov01 data!!
    perform_cable_swapping=swap_enabled;
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
HRichUnpackerRaw99::~HRichUnpackerRaw99(void) {
}
Bool_t HRichUnpackerRaw99::init(void) {
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
  initMappingPar();
  return kTRUE;
}

void HRichUnpackerRaw99::initMappingPar(void){
    HRuntimeDb* rtdb=gHades->getRuntimeDb();
    pMapPar = rtdb->getContainer("RichMappingParameters");
}

Int_t HRichUnpackerRaw99::getSubEvtId(void) const {
    return richId; 
}

void HRichUnpackerRaw99::printDataWord(DataWord &addr,Int_t ch){
    cout<<"*****************************************************************************"<<endl;
    cout<<"Event Nr: "<<fEventNr<<endl;
    cout<<"SEC: "<<addr.sector<<" RC: "<<addr.controler<<" PORT: "<<addr.port<<" MOD: "
	<<addr.modul<<" CH: "<<addr.channel<<" CHRG: "<<ch<<endl;
}

void HRichUnpackerRaw99::printMapping(DataWord &addr){
    cout<<"Event Nr: "<<fEventNr<<endl;
    cout<<"Col: "<<getMappingPar()->getCol(getUpi(addr))<<endl;
    cout<<"Row: "<<getMappingPar()->getRow(getUpi(addr))<<endl;
    cout<<"isValidUpi: "<<getMappingPar()->isValidUpi(getUpi(addr))<<endl;
    cout<<"*****************************************************************************"<<endl;
}

Bool_t  HRichUnpackerRaw99::testSubEvt(DataWord &addr,Int_t ch){
    if (kverbose) printDataWord(addr,ch);
    Bool_t debugOutput = kFALSE;
    Bool_t decision = kTRUE;
    if (iSloppyLevel < 3){
 Int_t upi = getUpi(addr);
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
						if (iSloppyLevel < 2 && debugOutput){
						    cout<<"SubEvtId "<<getSubEvtId()<<":ERROR (evt nr: "<<fEventNr<<") in" 
							<<" HRichUnpackerRaw99::testSubEvt ***"
							<<" unpacked row "
							<<getMappingPar()->getRow(upi)
							<<" is out of bounds [0;89]"<<endl;
						}
						decision=kFALSE; 
					    }
					}else{
					    if (iSloppyLevel < 2 && debugOutput){
						//cout<<"SloppyLevel: "<<iSloppyLevel<<endl;
						cout<<"SubEvtId "<<getSubEvtId()<<":ERROR (evt nr: "<<fEventNr<<") in" 
						    <<" HRichUnpackerRaw99::testSubEvt ***"
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
						if (iSloppyLevel < 2 && debugOutput){
						    cout<<"SubEvtId "<<getSubEvtId()<<":ERROR (evt nr: "<<fEventNr<<") in"
							<<" HRichUnpackerRaw99::testSubEvt ***"
							<<" unpacked channel "<<upi
							<<" exists, but is not connected to any pad."
							<<" It is allowed only in a pedestal run !"<<endl;
						}
						decision=kFALSE;
					    }
					}else{ //FALSE (FALSE FALSE | TRUE FALSE -> FALSE
					    if (iSloppyLevel < 2 && debugOutput){
						    cout<<"SubEvtId "<<getSubEvtId()<<":ERROR (evt nr: "<<fEventNr<<") in" 
							<<" HRichUnpackerRaw99::testSubEvt ***"
							<<" unpacked channel "<<upi
							<<" does not exist"<<endl;
					    }
						decision=kFALSE;
					}
				    }
				}else{
				    if (iSloppyLevel < 2 && debugOutput){
					cout<<"SubEvtId "<<getSubEvtId()<<":ERROR (evt nr: "<<fEventNr<<") in" 
					    <<" HRichUnpackerRaw99::testSubEvt ***"
					    <<" unpacked channel "<<upi
					    <<" is out of bounds [0;17463]"<<endl;
				    }
				    decision=kFALSE;
				}
			    }else{
				if (iSloppyLevel < 2 && debugOutput){
				    cout<<"SubEvtId "<<getSubEvtId()<<":ERROR (evt nr: "<<fEventNr<<") in" 
					<<" HRichUnpackerRaw99::testSubEvt ***"
					<<" unpacked sector "<<addr.sector
					<<" is out of bounds [0;5]"<<endl;
				}
				decision=kFALSE;
			    }
			}else{
			    if (iSloppyLevel < 2 && debugOutput){
				cout<<"SubEvtId "<<getSubEvtId()<<":ERROR (evt nr: "<<fEventNr<<") in" 
				    <<" HRichUnpackerRaw99::testSubEvt ***"
				    <<" unpacked controler "<<addr.controler
				    <<" is out of bounds [0;1]"<<endl;
			    }
			    decision=kFALSE;
			}
		    }else{
			if (iSloppyLevel < 2 && debugOutput){
			    cout<<"SubEvtId "<<getSubEvtId()<<":ERROR (evt nr: "<<fEventNr<<") in" 
				<<" HRichUnpackerRaw99::testSubEvt ***"
				<<" unpacked port "<<addr.port
				<<" is out of bounds [0;4]"<<endl;
			}
			decision=kFALSE;
		    }
		}else{
		    if (iSloppyLevel < 2 && debugOutput){
			cout<<"SubEvtId "<<getSubEvtId()<<":ERROR (evt nr: "<<fEventNr<<") in" 
			    <<" HRichUnpackerRaw99::testSubEvt ***"
			    <<" unpacked module "<<addr.modul
			    <<" is out of bounds [0;4]"<<endl;
			printMapping(addr);
			printDataWord(addr,ch);
		    }
		    decision=kFALSE;
		}
	    }else{
		if (iSloppyLevel < 2  && debugOutput){
		    cout<<"SubEvtId "<<getSubEvtId()<<":ERROR (evt nr: "<<fEventNr<<") in" 
			<<" HRichUnpackerRaw99::testSubEvt ***"
			<<" unpacked channel "<<addr.channel
			<<" is out of bounds [0;63]"<<endl;
		    printMapping(addr);
		    printDataWord(addr,ch);
		}
		decision=kFALSE;
	    }
	}else{
	    if (iSloppyLevel < 2  && debugOutput){
		cout<<"SubEvtId "<<getSubEvtId()<<":ERROR (evt nr: "<<fEventNr<<") in" 
		    <<" HRichUnpackerRaw99::testSubEvt ***"
		    <<" unpacked charge "<<ch
		    <<" is out of bounds [0;1023]"<<endl;
	    }
	    decision=kFALSE;
	}
    }
    if (decision == kFALSE) nSubEvtCntr++;
    return decision;
}

Int_t HRichUnpackerRaw99::getUpi(DataWord &addr){
  if(perform_cable_swapping){
    //swapping of rich electronics coordinates!!!
    if(addr.controler==0 && addr.sector==3 && addr.port==6)
      {
	return addr.controler*10000 + 4*1000 + addr.modul*100 + addr.channel;
      }
    if(addr.controler==0 && addr.sector==3 && addr.port==4)
      {
	return addr.controler*10000 + 6*1000 + addr.modul*100 + addr.channel;
      }
  }
  return addr.controler*10000 + addr.port*1000 + addr.modul*100 + addr.channel;
}

Int_t HRichUnpackerRaw99::execute() {
  HLocation loc;
  Bool_t debugOutput = kFALSE;
  loc.set(3,0,0,0);
//   fEventNr++;
  fEventNr = gHades->getCurrentEvent()->getHeader()->getEventSeqNumber();

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
			      HRichRaw* pCell =
				  (HRichRaw*) pRawCat->getSlot(loc); 
			      if (pCell!=NULL) {
				  // add a random nb [0;1] to account 
				  // for the cut-off in the ADC
				pCell=new(pCell) 
			            HRichRaw(charge+gRandom->Rndm());
			        pCell->setSector(loc[0]);
				pCell->setRow(loc[1]);
				pCell->setCol(loc[2]);
				pCell->setEventNr(fEventNr);
			      }
			  }
		      }else{
			  cout<<"SubEvtId "<<getSubEvtId()<<" (HRichUnpackerRaw99::execute) Error in event :"
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
		      if (iSloppyLevel<2 && debugOutput) {
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

Bool_t HRichUnpackerRaw99::finalize(void) {
  //Int_t id = getSubEvtId(); // unused
  //cout<<"SubEvtId:"<<id<<" RICH: Number of evts processed: "<<fEventNr+1<<endl;
  //cout<<"SubEvtId:"<<id<<" RICH: Sloppy level:             "<<iSloppyLevel<<endl;
  //cout<<"SubEvtId:"<<id<<" RICH: Corrupt datawords:        "<<nSubEvtCntr<<endl;
  //cout<<"SubEvtId:"<<id<<" RICH: Sector mismatch errors:   "<<nSecMisMatchCntr<<endl;
  return kTRUE;
}

ClassImp(HRichUnpackerRaw99) 







