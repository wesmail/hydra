//*-- AUTHOR : Jerzy Pietraszko, Ilse Koenig 
//*-- Created : 03/02/2006
//*-- Modified: 14/09/2006 by P.Cabanelas

//_HADES_CLASS_DESCRIPTION 
/////////////////////////////////////////////////////////////
//
//  HRpcTrbUnpacker
//
//  Class for unpacking TRB data and filling 
//	the Rpc Raw category
//  Base subevent decoding is performed by decode function
//  see: htrbbaseunpacker.h (/base/datasource/) 
//
/////////////////////////////////////////////////////////////

using namespace std;
#include "hrpctrbunpacker.h"
#include "rpcdef.h"
#include "htrblookup.h"
#include "hrpcraw.h"
#include "hdebug.h"
#include "hades.h"
#include "hevent.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hruntimedb.h"
#include "hcategory.h"
#include "hldsubevt.h"
#include "heventheader.h"
#include <iostream>
#include <iomanip>
#include <stdlib.h>

ClassImp(HRpcTrbUnpacker) 

HRpcTrbUnpacker::HRpcTrbUnpacker (UInt_t id):HTrbBaseUnpacker(id) {
  // constructor
  pRawCat = NULL;
  debugRpcFlag=0;
}

Bool_t HRpcTrbUnpacker::init (void) {
  // creates the raw category and gets the pointer to the lookup table
  pRawCat = gHades->getCurrentEvent()->getCategory(catRpcRaw);
  if (!pRawCat) {
    pRawCat = gHades->getSetup()->getDetector("Rpc")->buildCategory(catRpcRaw);
    if (!pRawCat)
      return kFALSE;
    gHades->getCurrentEvent()->addCategory(catRpcRaw,pRawCat,"Rpc");
  }
  loc.set(3,0,0,0);
  lookup = (HTrbLookup *)(gHades->getRuntimeDb()->getContainer("TrbLookup"));
  return kTRUE;
}


Int_t HRpcTrbUnpacker::execute(void) {

  HRpcRaw *pRaw = 0;	// pointer to Raw category 
  Int_t nEvt = 0;	// Evt Seq. Nb. 
  Char_t side = 'U';    // side declaration

  nEvt = gHades->getCurrentEvent()->getHeader()->getEventSeqNumber();

  if (gHades->isCalibration()) {
    //calibration event
    //cout << " --calibration event nothing to do for forward rpc" << endl;
    return 1;
  }
  // decode subevent data
  if (pSubEvt) {		// pSubEvt - make sure that there is something for decoding 
    decode();			// decode the subevent and fill arrays, see: htrbbaseunpacker.h 

    // get the part of the lookup table which belongs to the subevent ID
    // in order to check which TRBchannel belongs to the RpcDetector

    ////////////////////////////////////////////
    //
    //  Previous HLD data are with SubEventId = 545
    //  In Hydra Allowed SubEvents Ids for TRB starts from 800...
    //  RPC ID is 852 (in principle) 
    //  
    ///////////////////////////////////////////   
    if(debugRpcFlag==802){  // FIXME: RPC ID ??
       subEvtId = 802;      // this line changes SubEventID 
    } 


    HTrbLookupBoard *board = lookup->getBoard(subEvtId);
    if (!board) {  
				// Problem: unpacker for specific SubId was called but for this SubId 
				// no channel in lookup table axist. 
				// 	
      Warning ("execute", "SubEvent: %i unpacked but channel in Lookup table does not exist", subEvtId);
    }

    //Int_t j=0;
    Int_t rightMulti=0;
    Int_t leftMulti=0;

    //fill RAW category for the Rpc detector 
    for (Int_t i = 0; i < 128; i++) {
        //j++;
        if (trbLeadingMult[i] < 1) continue;		//Leading Time data for this channel exist 

        // fill rpc raw category
        HTrbLookupChan *chan = board->getChannel(i);
        if (chan && 'R' == (Char_t)(chan->getDetector())) {	//Channel belongs to Rpc det
		//cout << "--Evnt Nb " << nEvt << "\t --Channel= " << i << " belongs to Rpc \n";

		side = chan->getSide();
		loc[0] = (Int_t)chan->getSector();  
		loc[1] = (Int_t)chan->getModule();  
		loc[2] = (Int_t)chan->getCell();  
		pRaw = (HRpcRaw *)pRawCat->getObject(loc);
		if (!pRaw) {
	  		pRaw = (HRpcRaw *)pRawCat->getSlot(loc);
	  		if (pRaw) {
	    			pRaw = new (pRaw) HRpcRaw;
	    			pRaw->setCell((Int_t) chan->getCell());
	    			pRaw->setModule((Int_t) chan->getModule());
	    			pRaw->setSector((Int_t) chan->getSector());
	    			pRaw->setChannel(i);
				if(side=='r') {
	    				pRaw->setRightTime(trbLeadingTime[i][0]);
	    				pRaw->setRightTot(trbADC[i][0]);
					pRaw->incRightNHits();
					rightMulti++;
				} else {
	    				pRaw->setLeftTime(trbLeadingTime[i][0]);
	    				pRaw->setLeftTot(trbADC[i][0]);
					pRaw->incLeftNHits();
					leftMulti++;
				}

				if(i==127) { // FIXME: cambia la condicion de if !!
					//cout << "\t rightMulti = " << rightMulti << endl;
					pRaw->setRightMult(rightMulti);
					pRaw->setLeftMult(leftMulti);
				}


	  		} else {
	    			Error ("execute()", "Can't get slot\n");
	    			return -1;
	  		}
		} else {
			//
			// here you can fill more than one hit for one
            		// detector channel:  trbLeadingTime[i][j>0]
            		// Raw category has to be prepared for this
			//	 
			#if DEBUG_LEVEL>4
	  		Info ("execute()", "Slot already exists!\n");
			#endif
		}

      }   // channel belongs to RPC
    }    // for(Int_t i=0; i<128; i++)  
  }	//if(pSubEvt)

  return 1;

}
