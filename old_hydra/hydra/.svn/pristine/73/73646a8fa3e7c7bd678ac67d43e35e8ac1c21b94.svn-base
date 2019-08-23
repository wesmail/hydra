// File: hldsubevt.cc
//
//*-- Author : Walter Karig <W.Karig@gsi.de>
//*-- Modified : 97/12/10 15:24:45 by Walter Karig
//*-- Modified : 25/03/98 by Manuel Sanchez Garcia
//

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

