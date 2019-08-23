// File: hrichunpackercal99.cc
//
// Author: Walter Schoen <W.Schoen@physik.tu-muenchen.de>
// Last update by Walter Schoen: 99/12/17 17:16:06
// Last update by Thomas Eberl: 01/09/17 17:54:07
//
#include "hrichunpackercal99.h"
#include "hlocation.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hldsubevt.h"
#include "hdebug.h"
#include "richdef.h"
#include "hruntimedb.h"
#include "hrichdetector.h"
#include "hrichcalpar.h"
#include "hrichcal.h"
#include <stdio.h>

HRichUnpackerCal99::HRichUnpackerCal99(int rId,int strtEvt,Bool_t isPed,Bool_t verbose) {
    richId = rId;
    startEvt=strtEvt;
    isPedestal=isPed;
    kverbose=verbose;
    pCalCat=NULL;
    fCalPar = NULL;
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
    int upi = getUpi(addr);

    // *** check if address is within reasonable boundaries
    if (ch >=0 && ch<1024){
	if (addr.channel>=0 && addr.channel<64){ 
	    if (addr.modul>=0 && addr.modul<5){ 
		if (addr.port>=0 && addr.port<8){
		    if (addr.controler >=0 && addr.controler<2){
			if (addr.sector>=0 && addr.sector<6){
			    if (upi>=0 && upi<17464){//redundant 
				if (getMappingPar()->isValidUpi(upi)){
				    if (getMappingPar()->getCol(upi)>0 
					&& getMappingPar()->getCol(upi)<92){//red
					if (getMappingPar()->getRow(upi)>0 
					    && getMappingPar()->getRow(upi)<90){//red
					    if (kverbose) printMapping(addr);
					}else{
					    cout<<"ERROR (evt nr: "<<fEventNr<<") in" 
                                                <<" HRichUnpackerCal99::testSubEvt ***"
						<<" unpacked row "
						<<getMappingPar()->getRow(upi)
						<<" is out of bounds [0;89]"<<endl;
					    decision=kFALSE; 
					}
				    }else{
				    cout<<"ERROR (evt nr: "<<fEventNr<<") in" 
					<<" HRichUnpackerCal99::testSubEvt ***"
					<<"  unpacked col "
					<<getMappingPar()->getCol(upi)
					<<" is out of bounds [0;91]"<<endl;
				    decision=kFALSE;
				    }
				}else{
				    if(isPedestal){
					if(!(getMappingPar()->isUnConnCh(upi))){
					    cout<<"ERROR (evt nr: "<<fEventNr<<") in"
						<<" HRichUnpackerRaw99::testSubEvt ***"
						<<" unpacked channel "<<upi
						<<" is not connected to any pad"<<endl;
					    decision=kFALSE; 
					}
				    }else{
					if(!(getMappingPar()->isUnConnCh(upi))){
					    cout<<"ERROR (evt nr: "<<fEventNr<<") in"
						<<" HRichUnpackerRaw99::testSubEvt ***"
						<<" unpacked channel "<<upi
						<<" is not connected to any pad"<<endl;
					    decision=kFALSE; 
					}else{
					    cout<<"ERROR (evt nr: "<<fEventNr<<") in" 
						<<" HRichUnpackerRaw99::testSubEvt ***"
						<<" unpacked channel "<<upi
						<<" does not exist"<<endl;
					    decision=kFALSE;
					}
				    }
				}
			    }else{
				cout<<"ERROR (evt nr: "<<fEventNr<<") in" 
				    <<" HRichUnpackerCal99::testSubEvt ***"
				    <<" unpacked channel "<<upi
				    <<" is out of bounds [0;17463]"<<endl;
				decision=kFALSE;
			    }
			}else{
			    cout<<"ERROR (evt nr: "<<fEventNr<<") in" 
				<<" HRichUnpackerCal99::testSubEvt ***"
				<<" unpacked sector "<<addr.sector
				<<" is out of bounds [0;5]"<<endl;
			    decision=kFALSE;
			}
		    }else{
			cout<<"ERROR (evt nr: "<<fEventNr<<") in" 
			    <<" HRichUnpackerCal99::testSubEvt ***"
			    <<" unpacked controler "<<addr.controler
			    <<" is out of bounds [0;1]"<<endl;
			decision=kFALSE;
		    }
		}else{
		    cout<<"ERROR (evt nr: "<<fEventNr<<") in" 
			<<" HRichUnpackerCal99::testSubEvt ***"
			<<" unpacked port "<<addr.port
			<<" is out of bounds [0;4]"<<endl;
		    decision=kFALSE;
		}
	    }else{
		cout<<"ERROR (evt nr: "<<fEventNr<<") in" 
		    <<" HRichUnpackerCal99::testSubEvt ***"
		    <<" unpacked module "<<addr.modul
		    <<" is out of bounds [0;4]"<<endl;
		decision=kFALSE;
	    }
	}else{
	    cout<<"ERROR (evt nr: "<<fEventNr<<") in" 
		<<" HRichUnpackerCal99::testSubEvt ***"
		<<" unpacked channel "<<addr.channel
		<<" is out of bounds [0;63]"<<endl;
	    decision=kFALSE;
	}
    }else{
	cout<<"ERROR (evt nr: "<<fEventNr<<") in" 
	    <<" HRichUnpackerCal99::testSubEvt ***"
	    <<" unpacked charge "<<ch
	    <<" is out of bounds [0;1023]"<<endl;
	decision=kFALSE;
    }

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
		      if (loc.getIndex(1) >= 0 && loc.getOffset() >= 0) {
			  HRichCalParCell *calparcell = NULL;
			  HRichCal *pCalCell = NULL;
			  calparcell = (HRichCalParCell*) ((HRichCalPar*)getCalPar())
			      ->getObject(loc);
			  Float_t fCharge = charge - calparcell->getOffset(); 
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
		  }else cout<<"Dataword skipped in Event: "<<fEventNr<<endl;
		  
	      }else{
		  cout<<"Error in event :"<<fEventNr<<endl;
		  cout<<"Unpacked sector info: "<<dataword.sector<<
		      "  SubEvtId claims sector: "<<SubEvtIdSec<<endl;
		  cout<<"Dataword skipped"<<endl;
	      }
	  }//end of unpacking sub event
      }// end of if data in subevent
  }// end of start event condition
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

Bool_t HRichUnpackerCal99::finalize(void) {
    //    ((HRichCalPar*)fCalPar)->PrintCalPar();
    return kTRUE;
}

ClassImp(HRichUnpackerCal99) 








