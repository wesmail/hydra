//*-- Author : Walter Karig 
//*-- Modified : 05/06/98 by Manuel Sanchez
//*-- Modified : 15/11/2000 by M. Golubeva
//
//  use for files after 15/11/2000 2:37, i.e. from xx00320022629.hld on.
//

#include "hstartunpacker.h"
#include "hstartlookup.h"
#include "hstartraw.h"
#include "htboxchan.h"
#include "htofinoraw.h"
#include "hldsubevt.h"
#include "hades.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hruntimedb.h"
#include "hrecevent.h"
#include "hpartialevent.h"
#include "heventheader.h"
#include "hdebug.h"
#include "hlinearcategory.h"
#include "hiterator.h"
#include "hcategory.h"
#include <iostream.h>

//////////////////////////////////
//HStartUnpacker
//
//  This is the HUnpacker used to read Tofino+Start+Scallers data 
//  from .hld files
//
//////////////////////////////////

FILE* HStartUnpacker::fp=NULL;
Int_t HStartUnpacker::lookup[][22]={0};

HStartUnpacker::HStartUnpacker(Int_t id) {
  subEvtId = id;
  pRawCat=NULL;
  pStartData = NULL;
  tboxCat=NULL;
  tofinoRawCat=NULL;
  for(Int_t i=0; i<256; i++) {
    for(Int_t j=0; j<22; j++) lookup[i][j] = -1;
  }
}

HStartUnpacker::~HStartUnpacker(void) {
}

Bool_t HStartUnpacker::init(void) {

  HRecEvent *event=0;
  HPartialEvent *par=0;

  loc.set(3,0,0,0);
  locStart.set(2,0,0);
  event=(HRecEvent *)gHades->getCurrentEvent();
  if (!event) return kFALSE;


  HDetector* det=gHades->getSetup()->getDetector("Start");
  if (det) {
    par=event->getPartialEvent(catStart);
    if (!par) { 
      event->addPartialEvent(catStart,"Start","The Start subevent");
      par=event->getPartialEvent(catStart);
      if (!par) return kFALSE;
    }
    pRawCat=(HCategory*)par->getCategory(catStartRaw); 
    if (!pRawCat) {
      pRawCat=det->buildCategory(catStartRaw);
      if (pRawCat) par->addCategory(catStartRaw,pRawCat);
      else return kFALSE;
    } 
/*
    lookup=(HStartLookup*)(gHades->getRuntimeDb()
                                 ->getContainer("StartLookup"));
    if(!lookup) {
      lookup=new HStartLookup(1,16);  // must be changed when 2 TDCs are used
      gHades->getRuntimeDb()->addContainer(lookup); 
    }
*/
  } // if(det)

  det=gHades->getSetup()->getDetector("TBox");
  if(det) {
    par=event->getPartialEvent(catStart);
    if (!par) { 
      event->addPartialEvent(catStart,"Start","The Start subevent");
      par=event->getPartialEvent(catStart);
      if (!par) return kFALSE;
    }
    tboxCat=(HCategory*)par->getCategory(catTBoxChan); 
    if (!tboxCat) {
      tboxCat=det->buildCategory(catTBoxChan);
      if (tboxCat) par->addCategory(catTBoxChan,tboxCat);
      else return kFALSE;
    }
  }

  det=gHades->getSetup()->getDetector("Tofino"); 
  if (det) {
    par=event->getPartialEvent(catTofino);
    if (!par) { 
      event->addPartialEvent(catTofino,"Tofino","The Tofino subevent");
      par=event->getPartialEvent(catTofino);
      if (!par) return kFALSE;
    }
    tofinoRawCat=(HCategory*)par->getCategory(catTofinoRaw); 
    if (!tofinoRawCat) {
      tofinoRawCat=det->buildCategory(catTofinoRaw);
      if (tofinoRawCat) par->addCategory(catTofinoRaw,tofinoRawCat);
      else return kFALSE;
    }  
  }

  if (fp==NULL) {
    fp = fopen("startunp.txt","r");
    if (!fp) return kFALSE;
    Int_t ncol;
    Int_t idx, crate, slot;
    Char_t comm[100];
    while(1) {
      ncol = fscanf(fp,"%i %i %i %s",&idx,&crate,&slot,comm);
      if (ncol<0) break;
      if (crate>=0 && crate<256 && slot>0 && slot<22) {
        cout<<"idx : "<<idx<<"  crate : "<<crate<<"  slot :"<<slot
                         <<"   "<<comm <<endl;
        lookup[crate][slot] = idx;
      }
    }
  }
  return kTRUE;
}

Int_t HStartUnpacker::execute() {

  Int_t i = fill();
  return i;
}

int HStartUnpacker::fill() {

//  cout << "*** enter fill() ***" << endl;
  HTofinoRaw* tofinoRaw=0;
  HTBoxChan* ptboxchan=0;
  HStartRaw* startRaw=0;

  if(pStartData) {
    delete [] pStartData;
    pStartData = NULL;
  }

//  if(pSubEvt) cout << "pSubEvt exists" << endl;
//  else cout << "pSubEvt doesn't exist" << endl;

  UInt_t slot = 22;
  UInt_t crate = 256;
  UInt_t moduleCh = 32; 
  UInt_t data;
  Int_t idx = -1;
  UInt_t channels = 0;
  Int_t cursor, scalData[32];
//  Int_t i1, i2, i3, i4;
  Int_t s, m, c; 
  Int_t ppp;

  if(pSubEvt) {

    for(Int_t is=0; is<32; is++) scalData[is] = -2;

// get the length of the subevent (in 4 byte words)
    Int_t length = pSubEvt->getEnd() - pSubEvt->getData();
//printf(" length = %i \n",length);  

// copy the subevent to the pStartData
    pStartData = new UInt_t[length];
    UInt_t *pointer = pSubEvt->getData();
    for(Int_t i=0;i<length;i++) 
      pStartData[i] = pointer[i];

// start the cursor
    cursor = 0;

// read the first word (it does not contain the information needed for
// the unpacker) and increment the cursor
    cursor++;

// start the loop over the words in the subevent

    while(cursor < length) {

// read the slot number
      slot = (UInt_t) byte(1, pStartData[cursor]) >> 3;

// read the crate number
      crate = (UInt_t) byte(2,pStartData[cursor]); 

// read the number of fired channels and increment the cursor
      channels = (UInt_t) byte(3,pStartData[cursor++]);
//cout <<"cursor,crate,slot,channels = " <<cursor <<" " <<crate <<" " <<slot <<" " <<channels <<endl;

// extract the TDC/ADC index from the lookup table
// it serves to 1) identify TDC or ADC
//              2) identify sector number
//              3) identify module number
      if(crate>=0 && crate<256 && slot>0 && slot<22) idx = lookup[crate][slot];
      else idx = -1;

// start the loop over data words inside the TDC/ADC

      for(Int_t j = cursor; j<(Int_t)(cursor+channels); j++) {

// read the channel number
	moduleCh = (moduleCh | (pStartData[j] & 0x001f0000)) >> 16;

// read the data in the channel
        data = (pStartData[j] & 0xfff ) % 0xfff;
//cout <<"idx,moduleCh,data = " <<idx <<" "<<moduleCh <<" " <<data <<endl;

// Nov. 2000 Run
// idx = 1 - TDC Tofino(0-15), TDC Veto(16-23), TDC Start(24-31)
// idx = 2 - ADC Tofino(0-23), CFD Tofino(24-25) ??? (Sec 1-4 -> ADC(0-15))

// Sec 1 -> 0-3; Sec 3 -> 4-7; Sec 4 -> 8-11; Sec 6 -> 12-15;   

        if(idx>=0 && idx<4 && moduleCh >= 0 && moduleCh < 32) {

  	  if((idx == 1 || idx == 2) ) { // 
	    //	  	  if(idx == 1) { // Tofino TDC, Veto TDC, Start TDC

	    if(moduleCh > 15) goto c77; // Start or Veto TDC if idx = 1 

	    loc[0] = moduleCh/4; loc[1] = 0; loc[2] = moduleCh%4;
//cout <<"loc = " <<loc[0] <<" " <<loc[2] <<endl;

	    if(idx == 1) {
	      if(loc[0] == 1 || loc[0] == 2) loc[0] = loc[0] + 1; // new config.
	      else	      
		if(loc[0] == 3) loc[0] = 5;
	    }

// extract the cell (object of the class HTofinoRaw) from the Tofino raw
// data category using loc as the address

	    tofinoRaw = (HTofinoRaw*)tofinoRawCat->getObject(loc);

            if (tofinoRaw==NULL) {  // fill now TOFINO raw category
              tofinoRaw=(HTofinoRaw *)tofinoRawCat->getSlot(loc);
              if (tofinoRaw) {
	        tofinoRaw=new(tofinoRaw) HTofinoRaw;
	        tofinoRaw->setTime(-2.0); 
		tofinoRaw->setCharge(-2.0); 
		tofinoRaw->setAddress(loc[0],loc[1],loc[2]);
	      } else return kFALSE;
	    }

//tofinoRaw->getAddress(s,m,c);
//cout <<"Tofino 1 - s,m,c,tdc,adc = " <<s <<" " <<m <<" " <<c <<" " <<tofinoRaw->getTime() <<" " <<tofinoRaw->getCharge() <<endl;
// start filling the cell
            if(idx == 1 ) tofinoRaw->setTime((Short_t)data); 
	    if(idx == 2 ) tofinoRaw->setCharge((Short_t)data); 
	    tofinoRaw->setAddress(loc[0],loc[1],loc[2]);

//tofinoRaw->getAddress(s,m,c);
//cout <<"Tofino 2 - s,m,c,tdc,adc = " <<s <<" " <<m <<" " <<c <<" " <<tofinoRaw->getTime() <<" " <<tofinoRaw->getCharge() <<endl;

            goto c71;
c77:		

// Start (Mod=0) & Veto (Mod=1) TDC
	    if(idx == 1) {

	      if(moduleCh <= 23) { // Veto TDC
		locStart[0] = 1; locStart[1] = moduleCh-16; 
	      }  
	      else { // Start TDC
		locStart[0] = 0; locStart[1] = moduleCh-24; 
	      }

	      startRaw = (HStartRaw *)pRawCat->getObject(locStart);
	      
	      if (startRaw==NULL) { // fill now START raw category
		startRaw=(HStartRaw *)pRawCat->getSlot(locStart);
		if (startRaw) {
		  startRaw=new(startRaw) HStartRaw;
		  startRaw->fill(-2.0,locStart[0],locStart[1]);
		} else return kFALSE;
	      }
/*
    startRaw->getAddress(m,s);
    if(moduleCh <= 23)
      cout <<"Veto 1 - m,s,tdc = " <<m <<" " <<s <<" " <<startRaw->getTime() <<endl;
    else
      cout <<"Start 1 - m,s,tdc = " <<m <<" " <<s <<" " <<startRaw->getTime() <<endl;
*/
		startRaw->fill((Short_t)data,locStart[0],locStart[1]);
	      } // if(idx == 1) (Start & Veto TDC)
	  } // if(idx == 1 || idx == 2) // Tofino TDC & ADC, Start TDC

// Scalers
	  if(idx == 0) scalData[moduleCh] = data;
c71:
ppp = 7;
	} // if(idx>=0 && idx<4 && moduleCh >= 0 && moduleCh < 32)

// end of loop over data words inside TDC/ADC
      } // for(j = cursor; j<(Int_t)(cursor+channels); j++)

//cout <<"tboxCat = " <<tboxCat <<endl;
/*
    if(tboxCat) { // fill scaler data
      for (Int_t i=0; i<8; i++){
        i1=scalData[i];
        i2=scalData[i+8];
        i3=scalData[i+16];
        i4=scalData[i+24];
        ptboxchan=(HTBoxChan*)tboxCat->getNewSlot(loc);
        ptboxchan->set(i,i1,i2,i3);
      }
//for (Int_t i=0; i<8; i++){
//ptboxchan->get(i,i1,i2,i3);
//cout <<"i1,i2,i3,i4 = " <<i <<" " <<i1 <<" " <<i2 <<" " <<i3 <<endl;
//}
    } // if(tboxCat)
*/

    cursor += channels + 1;

// end of loop over TDC/ADC's inside Tofino+Start+Scallers subevent
    } // while(cursor < length)	  
  } // if(pSubEvt) 

  return kTRUE;
	
} // fill()

ClassImp(HStartUnpacker) 










