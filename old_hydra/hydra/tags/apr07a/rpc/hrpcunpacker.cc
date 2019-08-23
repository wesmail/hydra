//*-- AUTHORS :  Pablo Cabanelas / Hector Alvarez Pol
//*-- Created : 14/10/2005

//_HADES_CLASS_DESCRIPTION 
////////////////////////////////////////////////////////////////////////
//
//  HRpcUnpacker
//
//  This is the unpacker used to read for the first time the RPC data
//  (see RpcDataStructure in WIKI for subevent information)
//
//  This unpacker is quite robust and returns quite a lot of info for debugging
//
//  The unpacker accepts the following arguments:
//
//  HRpcUnpacker(UInt_t id, Bool_t unpaired, Int_t dbg)
//
//  where:
//
//  id                is the subEvent Id
//  unpaired=kTRUE    selects the unpaired mode of the TDCs in the TRB
//                    (the default value is unpaired=kFALSE)
//  debug             controls the debugging level and the number of messages 
//                    debug=0  only relevant errors (show-stoppers)
//                    debug=1  warning and all errors are on
//                    debug=2  additional information on read words
//                    debug=3  full decoding
//
////////////////////////////////////////////////////////////////////////

using namespace std;
#include "hrpcunpacker.h"
#include "rpcdef.h"
#include "hrpclookup.h"
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

ClassImp(HRpcUnpacker)
  
HRpcUnpacker::HRpcUnpacker(UInt_t id, Bool_t unpaired, Int_t dbg) {
  // constructor
  subEvtId=id;
  isUnpaired = unpaired;
  debug=dbg;
  pRawCat=NULL;
  lookup=0;
}

Bool_t HRpcUnpacker::init(void) {
  // creates the raw category and gets the pointer to the lookup table
  pRawCat=gHades->getCurrentEvent()->getCategory(catRpcRaw);
  if (!pRawCat) {
    pRawCat=gHades->getSetup()->getDetector("Rpc")->buildCategory(catRpcRaw);
    if (!pRawCat) return kFALSE;
    gHades->getCurrentEvent()->addCategory(catRpcRaw,pRawCat,"Rpc");
  }
  loc.set(1,0);
  lookup=(HRpcLookup*)(gHades->getRuntimeDb()->getContainer("RpcLookup"));
  return kTRUE;
}

Int_t HRpcUnpacker::execute() {
  //
  // This function fills the RPC raw data structure.
  //
  //---------------------------------------------------------------------
  /*
    The subevent Format for the RPC detector (Sep05) will look like this:
    
    # RPC Header
    
    1        size                    Length of whole subevent 
    2        0x3001                  SubEvt Decoding as 32bit Data
    3        subdetecor-ID           Non-ambiguous ID. Set to 0x221
    4        trigger tag             Trigger tag of subevent
    # subEvent
    
    5        0xbeefTTNW              Internal header, where TT is Trigger Tag and NW is the 
                   number of words of the subEvent (including header and trailer (0xdeadface)
    6        0x330d701b              first word of event 
    0x........              next word 
    0x........              next word 
    N-1      0x........             (N-5)th word of event  
    N        0xdeadface              last word 
    
    First word of the subEvent is started always as 0xbeefTTNW, and last word is 0xdeadface. 
    0xbeefTTNW is created by FPGA. 0xbeef and 0xdeadface are only markers, which is usefull 
    to checking by Etrax that data transfer is ok. FPGA is counting how many words belong to 
    each event and put this number as 0xNW. 0xTT is counter of events. Trigger tag of each 
    next event is higher by 1.
    
    daq_sniff output is something like this:
    
    size: 0x00000134  decoding: 0x00030001  id:    0x00000221  seqNr:  0x000a2e6e  
    date: 2005-09-22  time:     20:27:01    runNr: 0x42a1f5ee  expId:              
    size: 0x00000114  decoding: 0x00020001  id:    0x00000221  trigNr: 0x0000d7d7
    
    00000000:  0xbeefd741  0x230d7051  0x430058c4  0x43605589 
    00000010:  0x43805cc3  0x43f85d6b  0x430858c7  0x43605738 
    00000020:  0x43885cc4  0x43c05ec2  0x431058c6  0x43405ac5 
    00000030:  0x43905cc5  0x43c85ec2  0x432059c5  0x43485ac5 
    00000040:  0x43a05dc3  0x43d05ec2  0x432859c4  0x43505ac6 
    00000050:  0x43a85dc4  0x433059c4  0x43605bc1  0x43b05dc2 
    00000060:  0x43685bd5  0x43705bd6  0x43605db9  0x330d701b
    00000070:  0x230d7051  0x430058f0  0x43605589  0x4390552e 
    00000080:  0x43f85d6a  0x430858e6  0x436855c8  0x4380557c 
    00000090:  0x43c05ec6  0x431058f0  0x4370557e  0x4388558d 
    000000a0:  0x43c85ec7  0x432059e7  0x43405ae9  0x4390556e 
    000000b0:  0x43d05ec6  0x432859e7  0x43485ae8  0x438855eb 
    000000c0:  0x433059de  0x43505adf  0x43805cc6  0x43605bc4 
    000000d0:  0x43905cc6  0x43685bc6  0x43a05dcd  0x43705bc5 
    000000e0:  0x43a85dd0  0x43705d3b  0x43b05dcd  0x43885cc6 
    000000f0:  0x330d7021  0x240d7051  0x44f85d6e  0x340d7003 
    00000100:  0xdeadface
    
    //------------------------------------------------------------------------
    */

  HRpcRaw* pRaw=0;
  Int_t nChannel=0, nCell=-1;
  Int_t fModeFlag      =-1;    //Indicates de mode of the word (1=leading, 2=leading+width, 3=trailing)
  Int_t nId            = 0;    //trigger type
  Int_t nEvt           = 0;    //Evt Id
  Int_t nTdcEvtId      = 0;    //TdcEvt Id
  Int_t nTdc           = 0;    //Tdc Id
  Int_t nBunchId       = 0;    //Bunch Id
  UInt_t uWordCount    = 0;    //auxiliary
  UInt_t uTdcWordCount = 0;    //auxiliary
  UInt_t uBlockSize    = 0;    //Number of data words in one block
  //UInt_t *uTmp         = NULL; //auxiliary

  Char_t side='U';   
  
  //TDC Id is always 0 (known bug in TRB). We have to count TDCs 
  Int_t nCountTDCHeader = 0;
  Int_t nCountTDCTrailer = 0;
  Int_t nCountTDC = 0;
  
  nId = gHades->getCurrentEvent()->getHeader()->getId();
  
  if (nId){
    if(debug>2) Info("execute()","\n\t nId: %i \n",nId);  
  }
  else  
    if(debug>0)
      Warning("execute()","\n \t There is no Current Event !!! \n\n");
  
  
  if( gHades->isCalibration()){  //FIXME
    //calibration event
    //Nothing to do for rpc detector
    // cout << "Is Calibration" << endl;
    return 1;
  }
  
  if(pSubEvt) {  
    
    UInt_t* data = pSubEvt->getData();
    UInt_t* end  = pSubEvt->getEnd();
    nEvt = gHades->getCurrentEvent()->getHeader()->getEventSeqNumber();
    
    if(debug>2) Info("execute()","\n\t nEvt: %i \n",nEvt);
    
    //Loop over all data in this subevt
    do {
      uWordCount++;
      
      //Scan for Internal Header: 0xbeefTTNW
      // TT = Triger Tag;  NW = Number of Words	
      if( ((*data >>16) & 0xffff)==48879 ) { 
	if(debug>1) Info("execute()","Data word: %X  (First subevent word)\n",*data);
	uBlockSize = (*data) & 0x000000ff;  	
      }
      
      //Scan for TDC header
      else if ( ((*data>>28) & 0xf)==2 ){
	if(debug>1) Info("execute()","Data word: %X  (TDC header)\n",*data);
	
	nTdc       = ( (*data) >> 24  ) & 0xf;    //Programmed ID of TDC
	nTdcEvtId  = ( (*data) >> 12  ) & 0xfff;  //Event ID from event counter
	nBunchId   = (*data) & 0xfff;             //Bunch ID of trigger (trigger time tag)
	
	nCountTDCHeader++; 
	
	//Scan all data words
	if(debug>2) Info("execute()"," -- TDC header 0x%08x. Tdc Id: %i. Event Id: %i. Bunch Id: %i.\n",
			 *data,nTdc,nTdcEvtId,nBunchId);
	
	data++;
	uTdcWordCount=0;
 
	//after a successful TDC header, tdc info comes...
	//Only leading measurement (pairing or unpairing modes) and trailing measurement are admitted
	
	while( ( (((*data)>>28 & 0xf)==4) || (((*data)>>28 & 0xf)==5) ) && *data!=0x0 ){
	  if(debug>1) Info("execute()","Data word: %X  (TDC info)\n",*data);
	  
	  //counter to set the TDC Id (BUG reading it from word)
	  if(nCountTDCHeader == (nCountTDCTrailer+1)) nCountTDC = nCountTDCHeader-1;  
	  
	  uTdcWordCount++;
	  //nChannel = (32*nTDC) + ( (*(data) >> 19 ) & 0x001f ); //nTDC IS A BUG, currently is not correct
	  nChannel = (32*nCountTDC) +  ( (*(data) >> 19 ) & 0x001f );
	  
	  if(debug>2) Info("execute()","Channel #: %i  \t",nChannel);
	  
	  //Obtain the cell and side for each TDC channel from the lookup table 
	  HRpcLookupChan* dcs=lookup->getChannel(nChannel+1);
	  if (dcs){
	    dcs->getAddress(nCell,side); 
	    // FIXME: solo para el test de Nov05 "&& nCell<24)" (24 celdas)
	    if (nCell>=0) { 
	      loc[0]=nCell;   
	      pRaw=(HRpcRaw *)pRawCat->getObject(loc);
	      if(!pRaw) {                                //creating an slot for the object
		pRaw=(HRpcRaw *)pRawCat->getSlot(loc);
		if (pRaw) {
		  pRaw=new (pRaw) HRpcRaw;
		  pRaw->setCell(nCell);
		  pRaw->setChannel(nChannel);
		}
		else {
		  Error("execute()",
			"Can't get Cell or Channel\n");
		  return -1;
		}
	      }
	      
	      
	      if(((*data)>>28 & 0xf)==4) { //Leading measurement
		
		if(isUnpaired) {  //if unpaired mode... leading measurement
      
                fModeFlag=1;
                pRaw->setModeFlag(fModeFlag);		

		  Float_t fVal = (*data & 0x7ffff);  //bit 0 to 18

		  if(side=='R')  {
		    if(pRaw->getRightTime()<1) pRaw->setRightTime(fVal); //Only the first leading is taken
		    pRaw->incRightNHits();
		    if(debug>2) Info("execute()","TOF: %f  TOT: %f\n",pRaw->getRightTime(), 
				     pRaw->getRightTot());
		    if(pRaw->getRightNHits()>1 && debug>1) 
		      Info("execute()","nHits>1: new value %i \n",pRaw->getRightNHits());
		  }
		  else  {
		    if(pRaw->getLeftTime()<1) pRaw->setLeftTime(fVal); //Only the first leading is taken
		    pRaw->incLeftNHits();
		    if(debug>2) Info("execute()","TOF: %f  TOT: %f\n",pRaw->getLeftTime(), 
				     pRaw->getLeftTot());
		    if(pRaw->getLeftNHits()>1 && debug>1) 
		      Info("execute()","nHits>1: new value %i \n",pRaw->getLeftNHits());
		  } 
		}
		else{                                       //if paired mode... leading time and width
  
                fModeFlag=2;
                pRaw->setModeFlag(fModeFlag);		

		  Float_t fTot = (((*data) >> 12) & 0x7f);  // Time-over-threshold
		  Float_t fVal = (*data & 0xfff);           // Leading time value
		  
		  if(side=='R')  {
		    if(pRaw->getRightTime()<1){  //Only the first leading is taken
		      pRaw->setRightTime(fVal);
		      pRaw->setRightTot(fTot);
		    }
		    pRaw->incRightNHits();
		    if(debug>2) Info("execute()","TOF: %f  TOT: %f\n",pRaw->getRightTime(),
				     pRaw->getRightTot());
		    if(pRaw->getRightNHits()>1 && debug>1) 
		      Info("execute()","nHits>1: new value %i \n",pRaw->getRightNHits());
		  }
		  else  {
		    if(pRaw->getLeftTime()<1){  //Only the first leading is taken
		      pRaw->setLeftTime(fVal);
		      pRaw->setLeftTot(fTot);
		    }
		    pRaw->incLeftNHits();
		    if(debug>2) Info("execute()","TOF: %f  TOT: %f\n",pRaw->getLeftTime(), 
				     pRaw->getLeftTot());
		    if(pRaw->getLeftNHits()>1 && debug>1) 
		      Info("execute()","nHits>1: new value %i \n",pRaw->getLeftNHits());
		  }	
		}
	      } //end of leading measurement
	      
	      
	      else if(((*data)>>28 & 0xf)==5) {  //Trailing measurement in single edge mode
                fModeFlag=3;
                pRaw->setModeFlag(fModeFlag);		
 
		if(!isUnpaired) {                     //should never be the case!
		  if(debug>0) Warning("execute()",
				      "Wrong code (5) in pairing mode \n");
		  data++; 
		  continue;
		}
		
		Float_t fTrail  = (*data & 0x7ffff);     //trailing edge
		
		if(side=='R')  {
		  if(pRaw->getRightTime()==-999.){   //if there is no previous leading measurement
		    if(debug>0){
		      Warning("execute()",
			      "Evt: %i, Trailing measurement without previous leading! Skipping",nTdcEvtId);
		    }
		    data++;
		    continue;
		  }
		  Float_t preTime =  pRaw->getRightTime(); 
		  Float_t fTot = fTrail-preTime;
		  pRaw->setRightTot(fTot);
		  if(fTot>0) if(debug>2) Info("execute()","TOF: %f  TOT: %f\n",
					      pRaw->getRightTime(), pRaw->getRightTot());
		  else                 //takes also negative values of fTot, and warns
		    if(debug>0)
		      Warning("execute()",
			      "Evt: %i, Trailing measurement coming before than leading measurement!",
			      nTdcEvtId); 
		}
		else  {	  
		  if(pRaw->getLeftTime()==-999.){    //if there is no previous leading measurement
		    if(debug>0){
		      Warning("execute()",
			      "Evt: %i, Trailing measurement without previous leading! Skipping",nTdcEvtId);
		    }
		    data++;
		    continue;
		  }
		  Float_t preTime =  pRaw->getLeftTime();
		  Float_t fTot = fTrail-preTime;  
		  pRaw->setLeftTot(fTot);
		  if(fTot>0) if(debug>2) Info("execute()","TOF: %f  TOT: %f\n",
					      pRaw->getLeftTime(), pRaw->getLeftTot());
		  else 
		    if(debug>0)
		      Warning("execute()",
			      "Evt: %i, Trailing measurement coming before than leading measurement! Skipping",
			      nTdcEvtId);
		}
	      } //end of trailing measurement in single edge mode
	    }  // end nCells
	    else { 
	      if(debug>0){
		Warning("execute()",
			"Wrong address! Cell %i\n",
			nCell);
	      }
	    }
	  }//end dcs
	  else {  //if no access to the lookup table!
	    Warning("execute()",
		    "TDC channel: %i not found\n",nChannel);
	  }
	  data++;  

	}
	// End of Scan-all-data-words bucle
       	data--;
        uWordCount=uWordCount+uTdcWordCount;

      } // end of Scan-Tdc-header

      else if ( ((*data >>16) & 0xffff)==57005 ) {    //last word of the subevent
        if (uBlockSize!=uWordCount) {
                   Warning("execute()","Corrupted Block: uBlockSize=%i, uWordCount=%i \n",
                                uBlockSize,uWordCount);
                   continue;
        }
	if(debug>1) Info("execute()","Data word: %X  (Last word of the subevent)\n",*data);
	
      }

      else if ( ((*data >>28) & 0xf)==3 ){   // Scan for TDC Trailer
	if(debug>1) Info("execute()","Data word: %X  (TDC trailer)\n",*data);
	
	nCountTDCTrailer++;
	
	UInt_t uSubBlockSize = (*data & 0xfff);
	
	if(debug>2)  Info("execute()","\t uSubBlockSize =  %i \n",uSubBlockSize);
	if ( uSubBlockSize != (uTdcWordCount+2) )  {
	  if(debug>0) Warning("execute()",
			      "Found %i but expecting %i words in this TDC sub-block!!!\n",
			      uTdcWordCount,uSubBlockSize);
	  continue;
	}
	uTdcWordCount=0;
      }

      else if ( ((*data>>24) & 0x7)>5 && ((*data>>24) & 0x7)<14 ) {   
	// Invalid Data Word: Skipping it
	
	if(debug>0){
	  Warning("execute()",
		  "Evt: %i, Invalid TDC in Data Word! Type: %d,Skipping 0x%08x",
		  nTdcEvtId,((*data>>24) & 0x7),*data);
	  if(debug>1) pSubEvt->dumpIt();
	}	
      }

      else {           	//any other case
	if(debug>0){
	  Warning("execute()",
		  "Evt: %i, Unknow raw data word 0x%08x",
		  nTdcEvtId,*data);
	}
      } 
    } while ( ++data<end && *data!=0x0 ); //end of data
  }
  
  if(debug>1) Info("execute()","Final Generated Objects: %i\n",pRawCat->getEntries());
  if(debug>2) pRawCat->Dump();
  
  return 1;
}







