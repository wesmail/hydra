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
#include "hlinearcategory.h"

/////////////////////////////////
//HTofUnpacker
//
//  This is the HUnpacker used to read HTof data from LMD files
//
//////////////////////////////////

HTofUnpacker::HTofUnpacker(void) {
  pRawCat=NULL;
  pTofData = NULL;
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
  return kTRUE;
}

int HTofUnpacker::execute() {
  int i = fill();
  return i;
}

int HTofUnpacker::fill() {

  if(pTofData) {
    delete [] pTofData;
    pTofData = NULL;
  }
  if(pSubEvt) {
//
UInt_t cursor,channels,geo;
UInt_t tdcCh,time;
//
//in the beamtest of December 1998 start detector is unpacked inside TOF stuff
//This is a temporal solution!!!
//
UInt_t tdcCounter;
UInt_t startCh,startTdcCh,startTime;
//
    HLocation loc, one, zero;
    loc.set(3,0,0,0);
    one.set(1,0);
    zero.set(0);
    int length = pSubEvt->getEnd() - pSubEvt->getData();
    //  printf(" lenght:%i \n",length);  
    pTofData = new UInt_t[length];
    UInt_t *pointer = pSubEvt->getData();
    for(int j=0;j<length;j++) pTofData[j] = pointer[j];
    cursor = 0;
//
// start of loop over TOF TDC's inside TOF subevent
//
    tdcCounter = 0;
    while(cursor<length) {
      geo = (UInt_t) byte(1,pTofData[cursor]) >> 3;
      channels = (UInt_t) byte(3,pTofData[cursor++]);

      //  cout<<"geo = "<<geo<<"  channels = "<<channels<<"  chanEnd ="<<
      //	(UInt_t)(byte(3,pTofData[cursor+channels]))<<endl;


      if(channels!=((UInt_t)(byte(3,pTofData[cursor+channels])))) {
	cout << "Error in TDC" << geo-4 << " has happened" << endl;
	return 0;
      }
      tdcCh = 0;
      time = 0;
//
//start of loop over data words inside TOF TDC
//
      for(j=cursor;j<(cursor+channels);j++) {
	tdcCh = (tdcCh | (pTofData[j] & 0x001f0000)) >> 16;
        time = (pTofData[j] & 0xfff ) % 0xfff;
//cout<<"time = "<<time<<endl;
        loc[0] = 0;
        if(geo==5||geo==7) loc[1] = tdcCh/8;
	if(geo==6||geo==8) loc[1] = tdcCh/8 + 4;
	loc[2] = tdcCh%8;
	//        one[0] = loc[1]*8 + loc[2];     // needed for linear category
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
           } else return kFALSE;
        }
        if(pCell != NULL) {
	  if(geo==5||geo==6) {
	    pCell->setLeftTime((Float_t) time);
            pCell->setModule((Char_t)loc[1]);
            pCell->setCell((Char_t)loc[2]);
//cout<<"Module = "<<(UInt_t)pCell->getModule()<<"   Detector = "<<
//(UInt_t)pCell->getCell()<<endl;
	  }
	  if(geo==7||geo==8){
	    pCell->setRightTime((Float_t) time);
            pCell->setModule((Char_t)loc[1]);
            pCell->setCell((Char_t)loc[2]);
//cout<<"Module = "<<(UInt_t)pCell->getModule()<<"   Detector = "<<
//(UInt_t)pCell->getCell()<<endl;
	  }
	}
//
//end of loop over data words inside TOF TDC
//
      }	
      cursor += channels + 1;
      tdcCounter++;
//
//end of loop over TOF TDC's inside TOF subevent
//
    }
//
//unpack start detector
//
    /*
    if(cursor<length) {
      startCh = 0;
      startCh = (startCh | (pTofData[cursor++] & 0x00007000)) >> 12;
      startCh++;
      startTdcCh = 0;
      startTime = 0;
      for(j=cursor;j<(cursor+startCh);j++) {
        startTdcCh = (startTdcCh | (pTofData[j] & 0x00007000)) >> 12;
	startTime = (pTofData[j] & 0xfff) % 0xfff;
	loc[1] = 8;
	loc[2] = startTdcCh%8;
	if(HTofRaw *pCell= (HTofRaw*) pRawCat->getSlot(loc)) {
	  if(startTdcCh/8==0) pCell->setLeftTime((Float_t) startTime);
	  if(startTdcCh/8==1) pCell->setRightTime((Float_t) startTime);
	}
      }
      cursor += startCh;
    }
    */
  }  
return kTRUE;
}           

int HTofUnpacker::getModule(int chan) {
  static const int n[4]={21, 20, 17, 16};
  if(chan < 0 || chan > 3) return -1;
  else return n[chan];
}

ClassImp(HTofUnpacker) 







