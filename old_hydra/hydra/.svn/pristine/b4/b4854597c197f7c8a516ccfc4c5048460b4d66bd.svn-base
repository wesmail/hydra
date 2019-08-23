//*-- Author : Walter Karig 
//*-- Modified : 05/06/98 by Manuel Sanchez
//*-- Modified : 97/11/13 15:24:59 by Walter Karig
//
#include "htofunpacker.h"
#include "htofraw.h"
#include "hdebug.h"
#include "hades.h"
#include "hevent.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hcategory.h"
#include "hldsubevt.h"
#include "iostream.h"

/////////////////////////////////
//HTofUnpacker
//
//  This is the HUnpacker used to read HTof data from LMD files
//
//////////////////////////////////

FILE* HTofUnpacker::fp=NULL;
Int_t HTofUnpacker::lookup[][22]={0};

HTofUnpacker::HTofUnpacker(void) {
  subEvtId = 0;
  pRawCat=NULL;
  pTofData = NULL;
  for(Int_t i=0; i<256; i++) {
    for(Int_t j=0; j<22; j++) lookup[i][j] = -1;
  }
}


HTofUnpacker::HTofUnpacker(UInt_t id) {
  subEvtId = id;
  pRawCat=NULL;
  pTofData = NULL;
  for(Int_t i=0; i<256; i++) {
    for(Int_t j=0; j<22; j++) lookup[i][j] = -1;
  }
}


HTofUnpacker::~HTofUnpacker(void) {
}


Bool_t HTofUnpacker::init(void) {
  pRawCat=gHades->getCurrentEvent()->getCategory(catTofRaw);
  if (!pRawCat) {
    pRawCat=gHades->getSetup()->getDetector("Tof")->buildCategory(catTofRaw);
    if (!pRawCat) {
      return kFALSE;
    } else {
      gHades->getCurrentEvent()->addCategory(catTofRaw,pRawCat,"Tof");
    }
  }

  if (fp==NULL) {
    fp = fopen("tofunp.txt","r");
    if (!fp) return kFALSE;
    Int_t ncol;
    Int_t idx, crate, slot;
    Char_t comm[100];
    while(1) {
      ncol = fscanf(fp,"%i %i %i %s",&idx,&crate,&slot,comm);
      if (ncol<0) break;
      if (crate>=0 && crate<256 && slot>0 && slot<22) {

      //      cout<<"idx : "<<idx<<"  crate : "<<crate<<"  slot :"<<slot<<
      //	"   "<<comm<<endl;

        lookup[crate][slot] = idx;
      }
    }
  }
  return kTRUE;
}

int HTofUnpacker::execute() {

  int i = fill();
  return i;
}




//---------------------------------------------------------------------
//**  int HTofUnpacker::fill()
//
// This function fills the TOF raw data structure.
//
//---------------------------------------------------------------------
/*November 2000 Subevent Format for TOF, TOFINO and START


The new subevent format for TOF and START/TOFINO will look like this.

The first four words are the usual subevent header:

 subEvtSize             size of subevent including header (bytes)
 subEvtDecoding         0x00020001
 subEvtId               TOF: 411   START: 511
 subEvtTrigNr           trigger number

The next 32 bit word contains the data word, which was sent by the trigger
distribution system to the readout board. It contains information about
the trigger type, which produced the event (normal, calibration...).

 dtuTrigWord            0x a1 02 00 01 (example)
                                    \/
                                     \current trigger tag (should always
                                      be equal to the 8 LSB of the
                                      subEvtTrigNr!)
                                 \/
                                  \always 0x00 (for future use)
                              \/
                               \trigger tag, which was distributed with
                                this trigger (will be the current trigger
                                tag of the following event, when triggered
                                positive)
                           \/
                            \trigger type, 4 MSB are always 0xa
                             4 LSB are: 0x1=normal trigger
                                        0x4=calibration
                             currently only these two trigger types are
                             in use, might be extended during this beamtime

Now come the "real" data, which is equivalent to the corresponding part of 
the old subevent format.

Example for TOF:

 1st TDC:
 0x32002000             V878 header word (see v878 doku)
 0x30000fd7             V878 data word (channel 0)
 0x30010fb7             V878 data word (channel 1)
 0x30020f97             V878 data word (channel 2)
 ...
 0x301e0fb0             V878 data word (channel 30)
 0x301f0fb4             V878 data word (channel 31)
 0x34000002             V878 end-of-block word
 
 2nd TDC:
 0x3a002000             see above and see V878 doku for details
 0x38000fcb
 ...
 0x381f0f93
 0x3c000002

The number of TDC "blocks" depends on the number of TDC/ADCs mounted to
a certain crate. The number of V878 data words per TDC depends on the
number of channels fired (zero and overflow are suppressed). Anyhow, every
TDC/ADC will send at least the V878 header word and the end-of-block word
independently of the number of fired channels.

That's it, there is no end-of-event word or something like this. After the
last V878 end-of-block word the next subevent starts.

Here is a memory dump of my readout board. It contains one subevent marked
with [ ]. It's a subevent containing three TDCs.

TOF-RTU memory dump
------------------------------------------------------------
start address : 0x1000000
length [byte] : 0x100000
---------------------------------------------
 1000000:[000001ac 00020001 00000191 00000001
 1000010: a1020001 32002000 30000fd7 30010fb7
 1000020: 30020f97 30030f96 30040f89 30050f9f
 1000030: 30060f90 30070f7c 30080f89 30090f8e
 1000040: 300a0f8f 300b0f8a 300c0f97 300d0f91
 1000050: 300e0f96 300f0f87 30100f93 30110f8e
 1000060: 30120f94 30130f8e 30140f9c 30150f95
 1000070: 30160fa8 30170f97 30180fa8 30190fa9
 1000080: 301a0fad 301b0fa6 301c0fb5 301d0fae
 1000090: 301e0fb0 301f0fb4 34000002 3a002000
 10000a0: 38000fcb 38010fad 38020f86 38030f6b
 10000b0: 38040f7f 38050f80 38060f90 38070f6e
 10000c0: 38080f81 38090f80 380a0f8f 380b0f9a
 10000d0: 380c0f7a 380d0f7e 380e0f8a 380f0f74
 10000e0: 38100f84 38110f83 38120f82 38130f76
 10000f0: 38140f8a 38150f81 38160f89 38170f84
 1000100: 38180f90 38190f90 381a0f90 381b0f87
 1000110: 381c0f93 381d0f8e 381e0f8b 381f0f93
 1000120: 3c000002 42002000 40000fcf 40010fb1
 1000130: 40020f87 40030f7f 40040f88 40050f8d
 1000140: 40060f8c 40070f88 40080f86 40090f87
 1000150: 400a0f8c 400b0f97 400c0f84 400d0f87
 1000160: 400e0f8b 400f0f86 40100f8d 40110f92
 1000170: 40120f97 40130f8e 40140f94 40150f90
 1000180: 40160f9d 40170f9a 40180f97 40190fa6
 1000190: 401a0fa2 401b0f93 401c0f9b 401d0f94
 10001a0: 401e0fa6 401f0f93 44000002]000001ac
 10001b0: 00020001 00000191 00000002 a1030002
 10001c0: 32002000 30000fec 30010fd1 30020fae
 10001d0: 30030f94 30040f88 30050f9e 30060f97
 10001e0: 30070f7f 30080f89 30090f89 300a0f8d
 10001f0: 300b0f8c 300c0f97 300d0f90 300e0f96



The structure of the START/TOFINO subevent will be similar, except for
the scaler module, which will be included there. Since it will be the first
time, that this scaler will be in the subevent, I will have to investigate
it.

I will leave for lunch now and we can talk about it after, ok ?

Ciao,
ER!K*/

 
int HTofUnpacker::fill() {

  //  cout << "In HTofUnpacker::fill()" << endl;

  if(pTofData) {
    delete [] pTofData;
    pTofData = NULL;
  }

  //  if(pSubEvt) cout << "pSubEvt exists" << endl;
  //  else cout << "pSubEvt doesn't exist" << endl;

  Int_t i,j;
  //  UInt_t bit5s; // first 5 bits if counting from the left
  //  Int_t moduleType = 0; // 0 - nothing, 1 = TDC or 2 = ADC
  UInt_t slot = 22;
  UInt_t crate = 256;
  UInt_t moduleCh = 32; 
  UInt_t data;
  Int_t idx = -1;
  UInt_t channels = 0;
  Int_t cursor;
  Int_t temp;
  Int_t flag; // 1 - TDC, 2 - ADC

  if(pSubEvt) {

    HLocation loc;

    loc.set(3,0,0,0);
//
// get the length of the subevent (in 4 byte words)
//
    Int_t length = pSubEvt->getEnd() - pSubEvt->getData();

    //    printf(" length = %i \n",length);  
//
// copy the subevent to the pTofData
//
    pTofData = new UInt_t[length];
    UInt_t *pointer = pSubEvt->getData();
    for(i=0;i<length;i++) pTofData[i] = pointer[i];
//
// start the cursor
//
    cursor = 0;
//
// read the first word (it does not contain the information needed for
// the unpacker) and increment the cursor
//
    cursor++;
//
// start the loop over the words in the subevent
//
    while(cursor < length) {
//      
// read the slot number
//
      slot = (UInt_t) byte(1, pTofData[cursor]) >> 3;

      //      printf("slot = %d\n",slot);

//
// read the crate number
//
      crate = (UInt_t) byte(2,pTofData[cursor]); 

      //      printf("\n");
      //      printf("crate = %d\tslot = %d\n",crate,slot);

//      
// read the number of fired channels and increment the cursor
//
      channels = (UInt_t) byte(3,pTofData[cursor++]);

      //      printf("crate = %d\tslot = %d\tchannels = %d\n",crate,slot,channels);

//
// extract the TDC/ADC index from the lookup table
// it serves to 1) identify TDC or ADC
//              2) identify sector number
//              3) identify module number
//
      if(crate>=0 && crate<256 && slot>0 && slot<22) idx = lookup[crate][slot];
      else idx = -1;
//
// start the loop over data words inside the TDC/ADC
//
      for(j = cursor; j<(Int_t)(cursor+channels); j++) {
//
// read the channel number
//
	moduleCh = (moduleCh | (pTofData[j] & 0x001f0000)) >> 16;

	//	printf("moduleCh = %d\n",moduleCh);

//
// read the data in the channel
//
        data = (pTofData[j] & 0xfff ) % 0xfff;

	//	printf("moduleCh = %d\tdata = %d\n",moduleCh,data);

//
// now identify the location (sector/module/strip)
//
	if(idx>=0 && idx<48 && moduleCh >= 0 && moduleCh < 32) {

	  if(idx < 24) { // TDC

	    loc[0] = idx/4; // sector
	    temp = idx%4;
	    flag = 1;
	  }

	  if(idx >= 24) { // ADC

	    loc[0] = (idx-24)/4; // sector
	    temp = (idx-24)%4;
	    flag = 2;
	  }

	  if(temp==0 || temp==2) loc[1] = moduleCh/8;     // modules 1-4
	  if(temp==1 || temp==3) loc[1] = moduleCh/8 + 4; // modules 5-8

	  loc[2] = moduleCh % 8; // strip
	
//
// extract the cell (object of the class HTofRaw) from the TOF raw
// data category using loc as the address
//
	  HTofRaw *pCell = (HTofRaw*) pRawCat->getObject(loc);

//
// if that cell didn't exist create it
//
	  if (pCell == NULL) { 
	    pCell = (HTofRaw*) pRawCat->getSlot(loc);
	    if (pCell != NULL) {
	      pCell = new(pCell) HTofRaw;   // needed to get a streamer
	                                    // for HTree::Fill() 
	      pCell->setRightTime(0.0);
	      pCell->setLeftTime(0.0);
	      pCell->setRightCharge(0.0);
	      pCell->setLeftCharge(0.0);
	      pCell->setSector((Char_t) loc[0]);
	      pCell->setModule((Char_t)loc[1]);
	      pCell->setCell((Char_t)loc[2]);
	    } else return kFALSE;
	  }

//
// start filling the cell
//
	  if(temp==0 || temp==1) { // right sides
	    if(flag==1) { // TDC
	      pCell->setRightTime((Float_t) data);
	      pCell->setSector((Char_t) loc[0]);
	      pCell->setModule((Char_t) loc[1]);
	      pCell->setCell((Char_t) loc[2]);

	    }	      
	    if(flag == 2) { // ADC
	      pCell->setRightCharge((Float_t) data);
	      pCell->setSector((Char_t) loc[0]);
	      pCell->setModule((Char_t) loc[1]);
	      pCell->setCell((Char_t) loc[2]);
	    }
	  }

	  if(temp==2 || temp==3) { // left sides
	    if(flag == 1) { // TDC
	      pCell->setLeftTime((Float_t) data);
	      pCell->setSector((Char_t) loc[0]);
	      pCell->setModule((Char_t) loc[1]);
	      pCell->setCell((Char_t) loc[2]);
	    }
	    if(flag == 2) { // ADC
	      pCell->setLeftCharge((Float_t) data);
	      pCell->setSector((Char_t) loc[0]);
	      pCell->setModule((Char_t) loc[1]);
	      pCell->setCell((Char_t) loc[2]);
	    }
	  }
	  
	} // if(idx>=0 && idx<48 && moduleCh >= 0 && moduleCh < 32)
//
// end of loop over data words inside TDC/ADC
//
      } // for(j = cursor; j<(Int_t)(cursor+channels); j++)

      cursor += channels + 1;
// 	  
// end of loop over TDC/ADC's inside TOF subevent
//
    } // while(cursor < length)	  

  }

  return kTRUE;
	
}

/*


    i = 0;
    while(i<length) {

      if(i==0) { 
	if(((UInt_t)(byte(1,pTofData[i]))) != 193) break; // not 0xC1(not TOF)
	i++;
	continue;
      }

      if(pTofData[i] == 0xc1fffff0) break; // end of TOF subevent

      bit5s = (UInt_t) byte(1,pTofData[i]) >> 3;

      if(bit5s > 21) {

	if(((UInt_t)(byte(1,pTofData[i]))) == 209) {
	  moduleType = 1; // 0xD1 - TDC
	  i++;
	  continue;
	}
	if(((UInt_t)(byte(1,pTofData[i]))) == 210) {
	  moduleType = 2; // 0xD2 - ADC
	  i++;
	  continue;
	}
      }

      slot = bit5s;
      crate = (UInt_t) byte(2,pTofData[i]); 

      //      cout << "crate : " << crate << "   slot : " << slot << endl;

      if(crate>=0 && crate<256 && slot>0 && slot<22) idx = lookup[crate][slot];
      

      channels = (UInt_t) byte(3,pTofData[i++]);

      //      cout << "idx : " << idx << "   channels : " << channels << endl;

      for(j=i; j < (Int_t)(i+channels); j++) {
	moduleCh = (moduleCh | (pTofData[j] & 0x001f0000)) >> 16;
        data = (pTofData[j] & 0xfff ) % 0xfff;

	if(idx>=0 && idx<48 && moduleCh >= 0 && moduleCh < 32) {

	  //	  cout << "crate : " << crate << "   slot : " << slot << 
	  //	    "  moduleCh :" << moduleCh << "   data : " << data << endl;

	  if(idx < 24) { // TDC

	    loc[0] = idx/4;
	    temp = idx%4;
	    flag = 1;
	  }

	  if(idx >= 24) { // ADC

	    loc[0] = (idx-24)/4;
	    temp = (idx-24)%4;
	    flag = 2;
	  }

	  if(temp==0 || temp==2) loc[1] = moduleCh/8;     // modules 1-4
	  if(temp==1 || temp==3) loc[1] = moduleCh/8 + 4; // modules 5-8

	  loc[2] = moduleCh % 8;
	  

	  //--------------------------------------------------------
	  //that cabling was changed

      	  if(temp==0) { // modules 1-2
	    if(moduleCh/8 == 0 || moduleCh/8 == 2) {
	      loc[1] = 0;
	      loc[2] = moduleCh%8;
	    }
	    if(moduleCh/8 == 1 || moduleCh/8 == 3) {
	      loc[1] = 1;
	      loc[2] = moduleCh%8;
	    }
	  }
	  if(temp==1) { // modules 3-4
	    if(moduleCh/8 == 0 || moduleCh/8 == 2) {
	      loc[1] = 2;
	      loc[2] = moduleCh%8;
	    }
	    if(moduleCh/8 == 1 || moduleCh/8 == 3) {
	      loc[1] = 3;
	      loc[2] = moduleCh%8;
	    }
	  }
	  if(temp==2) { // modules 5-6
	    if(moduleCh/8 == 0 || moduleCh/8 == 2) {
	      loc[1] = 4;
	      loc[2] = moduleCh%8;
	    }
	    if(moduleCh/8 == 1 || moduleCh/8 == 3) {
	      loc[1] = 5;
	      loc[2] = moduleCh%8;
	    }
	  }
	  if(temp==3) { // modules 7-8
	    if(moduleCh/8 == 0 || moduleCh/8 == 2) {
	      loc[1] = 6;
	      loc[2] = moduleCh%8;
	    }
	    if(moduleCh/8 == 1 || moduleCh/8 == 3) {
	      loc[1] = 7;
	      loc[2] = moduleCh%8;
	    }
	  }
	  //--------------------------------------------------

	  HTofRaw *pCell = (HTofRaw*) pRawCat->getObject(loc);

	  if (pCell == NULL) { 
	    pCell = (HTofRaw*) pRawCat->getSlot(loc);
	    if (pCell != NULL) {
	      pCell = new(pCell) HTofRaw;   // needed to get a streamer
	                                    // for HTree::Fill() 
	      pCell->setRightTime(0.0);
	      pCell->setLeftTime(0.0);
	      pCell->setRightCharge(0.0);
	      pCell->setLeftCharge(0.0);
	      pCell->setSector((Char_t) loc[0]);
	      pCell->setModule((Char_t)loc[1]);
	      pCell->setCell((Char_t)loc[2]);
	    } else return kFALSE;
	  }

	  if(temp==0 || temp==1) { // right sides
	    if(flag==1) { // TDC
	      pCell->setRightTime((Float_t) data);
	      pCell->setSector((Char_t) loc[0]);
	      pCell->setModule((Char_t) loc[1]);
	      pCell->setCell((Char_t) loc[2]);

	    }	      
	    if(flag == 2) { // ADC
	      pCell->setRightCharge((Float_t) data);
	      pCell->setSector((Char_t) loc[0]);
	      pCell->setModule((Char_t) loc[1]);
	      pCell->setCell((Char_t) loc[2]);
	    }
	  }

	  if(temp==2 || temp==3) { // left sides
	    if(flag == 1) { // TDC
	      pCell->setLeftTime((Float_t) data);
	      pCell->setSector((Char_t) loc[0]);
	      pCell->setModule((Char_t) loc[1]);
	      pCell->setCell((Char_t) loc[2]);
	    }
	    if(flag == 2) { // ADC
	      pCell->setLeftCharge((Float_t) data);
	      pCell->setSector((Char_t) loc[0]);
	      pCell->setModule((Char_t) loc[1]);
	      pCell->setCell((Char_t) loc[2]);
	    }
	  }


	  //--------------------------------------------------------
	  //that cabling was changed

	  if(moduleCh/16 == 0) { // left sides

	    if(flag == 1) { // TDC
	      pCell->setLeftTime((Float_t) data);
	      pCell->setSector((Char_t) loc[0]);
	      pCell->setModule((Char_t) loc[1]);
	      pCell->setCell((Char_t) loc[2]);
	    }
	    if(flag == 2) { // ADC
	      pCell->setLeftCharge((Float_t) data);
	      pCell->setSector((Char_t) loc[0]);
	      pCell->setModule((Char_t) loc[1]);
	      pCell->setCell((Char_t) loc[2]);
	    }
	  }

	  if(moduleCh/16 == 1) { // right sides

	    if(flag == 1) { // TDC
	      pCell->setRightTime((Float_t) data);
	      pCell->setSector((Char_t) loc[0]);
	      pCell->setModule((Char_t) loc[1]);
	      pCell->setCell((Char_t) loc[2]);
	    }
	    if(flag == 2) { // ADC
	      pCell->setRightCharge((Float_t) data);
	      pCell->setSector((Char_t) loc[0]);
	      pCell->setModule((Char_t) loc[1]);
	      pCell->setCell((Char_t) loc[2]);
	    }
	  }
	  //--------------------------------------------------


	} // idx>=0 && idx<48 && moduleCh >= 0 && moduleCh < 32

      } // j=i; j < (Int_t)(i+channels); j++

      i += channels + 1;

      if(pTofData[i] == 0xd1fffff0 || pTofData[i]  == 0xd2fffff0) i++;

    }
      
  }

  return kTRUE;

}
*/

ClassImp(HTofUnpacker) 








