// File: hldsubevt.cc
//
//*-- Author : Walter Karig <W.Karig@gsi.de>
//*-- Modified : 97/12/10 15:24:45 by Walter Karig
//*-- Modified : 25/03/98 by Manuel Sanchez Garcia
//*-- Modified : 19/03/99 by D.Bertini

//////////////////////////////////////////////////////////
//HldSubEvent
//
//  This class contains the data of a LMD event which are not stored in
//  its header. That is the actual data for the mdc,rich,tof and shower
//
//////////////////////////////////////////////////////////

#include "hldsubevt.h"
#include <iostream.h>


void HldSubEvt::swapData() {
    if (wasSwapped()) {
	switch (getWordSize()) {
	case 4:
	    swap4((UInt_t*) getData(), getDataLen());
	    break;
	case 2:
	    swap2((UShort_t*) getData(), getDataLen());
	    break;
	case 1:
	    break;
	default:
	    cerr << "Word Size of " << getWordSize()
		 << " not supported" << endl;
	    exit(1);
	}
    }
} 


Bool_t HldSubEvt::scanIt(void) {
  //Checks the subevent for corruption. Returns kTRUE if correct.
  Bool_t r=kTRUE;
  
  if (getId()>699 || getId()<1) {
    r=kFALSE;
    printf("SubEvent corrupted\n");
  }
  return r;
}

void HldSubEvt::dumpIt() const {
  cout.fill('0');
   printf("===========================================================\n ");
   cout << "SubEvent:"   
       << "Id: " << getId()
       << "\tSize: " << getSize() 
	<< "\tDecoding: " << getDecoding() <<endl;   
    cout  << "      \tTrigNr: "<< getTrigNr() << endl;
   printf("===========================================================\n ");
  UInt_t *data;
  data= getData();
  for(UInt_t i=0;i<getDataSize()/sizeof(UInt_t);i++){
    if(i % 4 == 0 ){ // newline with offset 
    printf("\n0x%08x:", (char *) data - (char *) getData());
    }
    printf("  0x%08x", data[i]);
  }
  cout << endl; 
  cout << endl; 
}



