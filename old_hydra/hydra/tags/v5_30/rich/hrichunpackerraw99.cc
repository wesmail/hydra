// File: hrichunpackerraw99.cc
//
// Author: Walter Schoen <W.Schoen@physik.tu-muenchen.de>
// Last update by Walter Schoen: 99/12/17 17:29:47
// Last update by Thomas Eberl: 00/11/22 23:11:12
//
#include "hrichunpackerraw99.h"
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
#include <stdio.h>

HRichUnpackerRaw99::HRichUnpackerRaw99(int rId,int strtEvt=0,Bool_t verbose=kFALSE) {
    richId = rId;
    startEvt=strtEvt;
    kverbose=verbose;
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
    if (!pMapPar){
	pMapPar=new HRichMappingPar;
	pMapPar->SetName("RichMappingParameters");

	if (pMapPar) rtdb->addContainer(pMapPar);
    }

}
int HRichUnpackerRaw99::getSubEvtId(void) const {
    return richId; 
}

void HRichUnpackerRaw99::printDataWord(DataWord &addr,int ch){
    cout<<"*****************************************************************************"<<endl;
    cout<<"Event Nr: "<<fEventNr<<endl;
    cout<<"SEC: "<<addr.sector<<" RC: "<<addr.controler<<" PORT: "<<addr.port<<" MOD: "
	<<addr.modul<<" CH: "<<addr.channel<<" CHRG: "<<ch<<endl;
}

void HRichUnpackerRaw99::printMapping(DataWord &addr){
    cout<<"Event Nr: "<<fEventNr<<endl;
    cout<<"Col: "<<getMappingPar()->getCol(getUpi(addr))<<endl;
    cout<<"Row: "<<getMappingPar()->getRow(getUpi(addr))<<endl;
    cout<<"IsConn: "<<getMappingPar()->isConn(getUpi(addr))<<endl;
    cout<<"*****************************************************************************"<<endl;
}
Bool_t  HRichUnpackerRaw99::testSubEvt(DataWord &addr,int ch){
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
				if (getMappingPar()->isConn(upi)){
				    if (getMappingPar()->getCol(upi)>0 
					&& getMappingPar()->getCol(upi)<92){//redundant 
					if (getMappingPar()->getRow(upi)>0 
					    && getMappingPar()->getRow(upi)<90){//redundant 
					    if (kverbose) printMapping(addr);
					}else{
					    cout<<"ERROR (evt nr: "<<fEventNr<<") in" 
                                                <<" HRichUnpackerRaw99::testSubEvt ***"
						<<" unpacked row "
						<<getMappingPar()->getRow(upi)
						<<" is out of bounds [0;89]"<<endl;
					    decision=kFALSE; 
					}
				    }else{
				    cout<<"ERROR (evt nr: "<<fEventNr<<") in" 
					<<" HRichUnpackerRaw99::testSubEvt ***"
					<<"  unpacked col "
					<<getMappingPar()->getCol(upi)
					<<" is out of bounds [0;91]"<<endl;
				    decision=kFALSE;
				    }
				}else{
				    cout<<"ERROR (evt nr: "<<fEventNr<<") in" 
					<<" HRichUnpackerRaw99::testSubEvt ***"
					<<" unpacked channel "<<upi
					<<" is not connected or does not exist"<<endl;
				    decision=kFALSE;
				}
			    }else{
				cout<<"ERROR (evt nr: "<<fEventNr<<") in" 
				    <<" HRichUnpackerRaw99::testSubEvt ***"
				    <<" unpacked channel "<<upi
				    <<" is out of bounds [0;17463]"<<endl;
				decision=kFALSE;
			    }
			}else{
			    cout<<"ERROR (evt nr: "<<fEventNr<<") in" 
				<<" HRichUnpackerRaw99::testSubEvt ***"
				<<" unpacked sector "<<addr.sector
				<<" is out of bounds [0;5]"<<endl;
			    decision=kFALSE;
			}
		    }else{
			cout<<"ERROR (evt nr: "<<fEventNr<<") in" 
			    <<" HRichUnpackerRaw99::testSubEvt ***"
			    <<" unpacked controler "<<addr.controler
			    <<" is out of bounds [0;1]"<<endl;
			decision=kFALSE;
		    }
		}else{
		    cout<<"ERROR (evt nr: "<<fEventNr<<") in" 
			<<" HRichUnpackerRaw99::testSubEvt ***"
			<<" unpacked port "<<addr.port
			<<" is out of bounds [0;4]"<<endl;
		    decision=kFALSE;
		}
	    }else{
		cout<<"ERROR (evt nr: "<<fEventNr<<") in" 
		    <<" HRichUnpackerRaw99::testSubEvt ***"
		    <<" unpacked module "<<addr.port
		    <<" is out of bounds [0;7]"<<endl;
		decision=kFALSE;
	    }
	}else{
	    cout<<"ERROR (evt nr: "<<fEventNr<<") in" 
		<<" HRichUnpackerRaw99::testSubEvt ***"
		<<" unpacked channel "<<addr.channel
		<<" is out of bounds [0;63]"<<endl;
	    decision=kFALSE;
	}
    }else{
	cout<<"ERROR (evt nr: "<<fEventNr<<") in" 
	    <<" HRichUnpackerRaw99::testSubEvt ***"
	    <<" unpacked charge "<<ch
	    <<" is out of bounds [0;1023]"<<endl;
	decision=kFALSE;
    }

return decision;
}

int HRichUnpackerRaw99::getUpi(DataWord &addr){
    return addr.controler*10000 + addr.port*1000 + addr.modul*100 + addr.channel;
}
int HRichUnpackerRaw99::execute() {
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
			  HRichRaw* pCell =
			      (HRichRaw*) pRawCat->getSlot(loc); 
			  if (pCell!=NULL) {
			      pCell=new(pCell) HRichRaw(charge);
			      pCell->setSector(loc[0]);
			      pCell->setRow(loc[1]);
			      pCell->setCol(loc[2]);
			      pCell->setEventNr(fEventNr);
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
Bool_t HRichUnpackerRaw99::finalize(void) {
    return kTRUE;
}

ClassImp(HRichUnpackerRaw99) 







