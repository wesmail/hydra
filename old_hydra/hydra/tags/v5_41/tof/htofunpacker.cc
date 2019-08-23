//*-- Author : Walter Karig 
//*-- Modified : 14/12/2000 by Ilse Koenig
//*-- Modified : 05/06/98 by Manuel Sanchez
//*-- Modified : 97/11/13 15:24:59 by Walter Karig
//
#include "htofunpacker.h"
#include "tofdef.h"
#include "htoflookup.h"
#include "htofraw.h"
#include "hdebug.h"
#include "hades.h"
#include "hevent.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hruntimedb.h"
#include "hcategory.h"
#include "hldsubevt.h"
#include "iostream.h"

/////////////////////////////////
//HTofUnpacker
//
//  This is the unpacker used to read HTof data from LMD files
//
//////////////////////////////////

HTofUnpacker::HTofUnpacker(UInt_t id) {
  subEvtId = id;
  pRawCat=NULL;
  lookup=0;
}


Bool_t HTofUnpacker::init(void) {
  pRawCat=gHades->getCurrentEvent()->getCategory(catTofRaw);
  if (!pRawCat) {
    pRawCat=gHades->getSetup()->getDetector("Tof")->buildCategory(catTofRaw);
    if (!pRawCat) return kFALSE;
    gHades->getCurrentEvent()->addCategory(catTofRaw,pRawCat,"Tof");
  }
  loc.set(3,0,0,0);
  lookup=(HTofLookup*)(gHades->getRuntimeDb()
                             ->getContainer("TofLookup"));
  if(!lookup) {
    lookup=new HTofLookup();
    gHades->getRuntimeDb()->addContainer(lookup);
  }
  return kTRUE;
}


Int_t HTofUnpacker::execute() {
// This function fills the TOF raw data structure.
//
//---------------------------------------------------------------------
/*
November 2000 Subevent Format for TOF, TOFINO and START


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

*/
//---------------------------------------------------------------------

  HTofRaw* pCell=0;
  Int_t crate=-1, slot=-1, nChan=0, chan=-1, sec=-1, mod=-1, cell=-1;
  Char_t dcType='U', side='U';
  if(pSubEvt) {
    UInt_t* data=pSubEvt->getData();
    UInt_t* end=pSubEvt->getEnd();
    // cout<<"----------------  Tof subevent  ----------------"<<endl;
    while (++data<end && *data!=0x0) {
      slot=(*data >> 27 & 0x1f);
      crate=(*data >> 16 & 0xff);
      nChan=(*data >> 8 & 0xff);
      // cout <<"found: crate,slot,nChan: "
      //      <<crate<<" "<<slot<<" "<<nChan<<endl;
      HTofLookupSlot* dc=lookup->getSlot(crate-1,slot-1);
      if (dc && (dcType=dc->getType())!='U') {
        for(Int_t i=0;i<nChan;i++) {
          data++;
          chan = (*data & 0x001f0000) >> 16;
          HTofLookupChan* dcs=dc->getChannel(chan);
          if (!dcs) continue;
          dcs->getAddress(sec,mod,cell,side);
          if (sec>=0) {
            loc[0]=sec;
            loc[1]=mod;
            loc[2]=cell;
            pCell=(HTofRaw *)pRawCat->getObject(loc);
            if (!pCell) {
              pCell=(HTofRaw *)pRawCat->getSlot(loc);
              if (pCell) {
                pCell=new (pCell) HTofRaw;
	        pCell->setSector((Char_t)sec);
	        pCell->setModule((Char_t)mod);
	        pCell->setCell((Char_t)cell);
              } else {
                Error("HTofUnpacker::execute()", "can't get slot\n");
                return -2;
              }
	    }
            Float_t val=(*data & 0xfff) % 0xfff;
            if (dcType=='T') {
              if (side=='R') pCell->setRightTime(val);
              else pCell->setLeftTime(val);
            } else {
              if (side=='R') pCell->setRightCharge(val);
              else pCell->setLeftCharge(val);
            }
            // cout <<"filled: crate,slot,type,chan,sec,mod,cell,side,data = "
            //      <<crate<<" "<<slot<<" "<<dcType<<" "
            //      <<chan<<" "<<sec<<" "<<mod<<" "<<cell<<" "<<side<<" "
            //      <<val<<endl;
          }
        }
      } else data+=nChan;
      data++;
    }
  }
  return 1;
}

ClassImp(HTofUnpacker) 
