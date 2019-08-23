// File: hmdcunpacker.cc
//
// Author: Walter Karig <W.Karig@gsi.de>
// Modified by D.B <D.Bertini@gsi.de>
// Modified: 15/05/01 by Peter Zumbruch

#include <stdio.h>
#include "hades.h"
#include "hcategory.h"
#include "hiterator.h"
#include "hldsubevt.h"
#include "hevent.h"
#include "hmdcraw.h"
#include "hmdcrawstruct.h"
#include "hspectrometer.h"
#include "hmdcdetector.h"
#include "hruntimedb.h"
#include "heventheader.h"
#include <iostream.h>
#include <fstream>
#include <TROOT.h>

#include "hmdcunpacker.h"

HMdcDataWord *HMdcUnpacker::w=0;
TTree *HMdcUnpacker::packTree=0;
 
void HMdcDataWord::clear() {
sam=samP=roc=rocP=mbo=tdc=chan=hit=time=-10;
}

Bool_t HMdcDataWord::subHeader( const UInt_t* data ){
   //************************************************************
   // decoding of additionnal Mdc sub-header() 
   // 1): ev-nb  
   // 2): subevt size 
   // 3): flag-roc#-trig
   // bits       sz-mask             comments
   // ---------------------------------------       
   // 0 - 8       m4                trigger type               
   // 8 - 12      m4                Roc#    
   // 12          m4                flag size: 0:normal 1:abnormal 
   //                               bit 0: event truncated
   //                               bit 2: token not returned on port 0
   //                               bit 3: token not returned on port 1
   //************************************************************ 

        evNb       = *data;  
        subevtSize = *(data+1);
        trigType   = *(data+2)        & m4;
        rocNb      = *(data+2) >> 8   & m4;
	sizeStatus = *(data+2) >> 12  & m4; 
 return kTRUE;
}


Bool_t HMdcDataWord::decode( const UInt_t data ){

    //***************************************************** 
    // decoding of 32 bit data word from right->left 
    //
    //  bits        sz  mask           comments 
    // ----------------------------------------------------
    //  0 - 10      11  0x7ff          time value
    //      11       1  0x1            hit #0..1
    // 12 - 14       3  0x7            Channel of TDC Chip#0..7
    // 15 - 18       4  0xf            TDC Chip#1..12
    // 19 - 20       2  0x3            MBO#1..3 
    //      21       1  0x1            LVL1 Bus Pos (0,1) 
    // 22 - 24       3  0x7            RoC#1..7 
    //      25       1  0x1            SAM# Pos  
    // 26 - 29       4  0xf            SAM#1..12
    // 30 - 31       2   -             not used
    //*******************************************************  

    sam = (data >> 26 & m4)-1; 
    samP = data >> 25 & m1;
    roc = (data >> 22 & m3);  
    rocP = data >> 21 & m1;
    mbo = (data >> 19 & m2); 
    tdc = (data >> 15 & m4)-1;
    chan = data >> 12 & m3;
    hit =  data >> 11 & m1;
    time = data & m11;   
 return  kTRUE;
}


void HMdcDataWord::dump(){ 
 cout <<"*****************************************"<< endl; 
 cout << "sam: "     << getSam()  << endl; 
 cout << "Roc#: "    << getRoc()  << endl;  
 cout << "LvL bus: " << getRocP() << endl;
 cout << "Mbo#: "    << getBus()  << endl;
 cout << "TDC#"      << getTdc()  << endl;  
 cout << "time"      << getTime() << endl;
}


void HMdcUnpacker::SetErrorOutputFileName(Char_t *f)
{
  pFileName = f; 
}

void HMdcUnpacker::SetNtupleOutputFileName(Char_t *f)
{
  ntupleOutputFileName = f;
}

void HMdcUnpacker::SetNtupleOutputFile2Name(Char_t *f)
{
  ntupleOutputFile2Name = f;
}

void HMdcUnpacker::setTdcMode(Int_t i)
{
  // In measurement mode the TDC accepts two hits per channel. The TDC can hereby
  // trigger either on on a leading and the trailing edge of a single pulse  
  // or on two leading edges.
  // This influences the polarity of the hitCounter in the HMdcRaw Class: 
  // mode 0 (default) : trigger on leading and trailing edge
  //                    (hit multiplicity nHits: +1 or +2) 
  // mode 1           : trigger on 2 leading edges:
  //                    (hit multiplicity nHits: -1 or -2)

  tdcMode=(i == 0 || i == 1)? i : 0;
  return;
}


Int_t HMdcUnpacker::getTdcMode(void)
{
  // returns mode the tdc is triggering on signals
  // mode 0 (default) : trigger on leading and trailing edge
  // mode 1           : trigger on 2 leading edges:

  return tdcMode;
}


Bool_t HMdcUnpacker::init() {
  // Initialisation of 
  //                  - ntuple if debug option
  //                  - lookup table
  //                  - category for raw-level  storage

  HParSet *rawc=0;
  if (debug) 
    { 

      if (pFileName=="")             SetErrorOutputFileName("unpack_error.root");
      if (ntupleOutputFileName=="")  SetNtupleOutputFileName("debug-hits.root");
      if (ntupleOutputFile2Name=="") SetNtupleOutputFile2Name("debug-token.root");

      pFile = (TFile*)gROOT->GetListOfFiles()->FindObject(pFileName.Data());

      if (!pFile) 
	{
	  pFile = new TFile(pFileName.Data() ,"RECREATE","
                          Mdc-Unpacker Debug-info file ");
	}
      
      packTree = (TTree*) pFile->FindObject("mdcT");
      if (!packTree)
	{
	  packTree = new TTree("mdcT",
			       "Mdc-Unpacker Debug-info Tree");
	  packTree->Branch("Mdc_Data_Word","HMdcDataWord",&w,8000);
	}

      ntupleOutputFile = (TFile*)gROOT->GetListOfFiles()->FindObject(ntupleOutputFileName.Data());
      if (!ntupleOutputFile) 
	{
	  ntupleOutputFile = new TFile(ntupleOutputFileName.Data(),"RECREATE");
	}

      ntupleOutputFile->cd();

      ntuple = (TNtuple*) ntupleOutputFile->FindObject("debug-token");
      if (!ntuple)
	{
	  ntuple = new TNtuple("debug_hits",
			       "debug_hits",
			       "EvtSeqNr:hits:sector:module:mbo:tdc:Sam:SamP:Roc:RocP:Bus");
	}


      ntupleOutputFile2 = (TFile*)gROOT->GetListOfFiles()->FindObject(ntupleOutputFile2Name.Data());
      if (!ntupleOutputFile2) 
	{
	  ntupleOutputFile2 = new TFile(ntupleOutputFile2Name.Data(),"RECREATE");
	}


      ntupleOutputFile2->cd();

      ntuple2 = (TNtuple*) ntupleOutputFile2->FindObject("debug-token");
      if (!ntuple2)
	{
	  ntuple2 = new TNtuple("debug_token",
				"debug_token",
				"EvtSeqNr:SubEvtId:Roc:Errorbyte:EventTruncated:TokenNotReturned:TokenNotReturnedPort0:TokenNotReturnedPort1");
	}
    }
   
   rawc=gHades->getRuntimeDb()->getContainer("MdcRawStruct");
   if (!rawc) {
     rawc=new HMdcRawStruct;
     gHades->getRuntimeDb()->addContainer(rawc);
     rawc->init(); //Needs explicit initialization
   }
   pReadout = (HMdcEvReadout*) 
                gHades->getRuntimeDb()->getContainer("MdcEvReadout");
   if(!pReadout){
             pReadout = new HMdcEvReadout();
             gHades->getRuntimeDb()->addContainer(pReadout); 
    }  
    
   pRawCat = gHades->getCurrentEvent()->getCategory(catMdcRaw);
   if (pRawCat){
	return kTRUE;
    } else {
 	pRawCat = gHades->getSetup()->getDetector("Mdc")
 	    ->buildCategory(catMdcRaw);
 	if (pRawCat) {
 	    return gHades->getCurrentEvent()
		->addCategory(catMdcRaw, pRawCat, "Mdc");
	} else{
	    return kFALSE;
	}
    }
}


Int_t HMdcUnpacker::execute() {
   return fill();
}

Bool_t HMdcUnpacker::finalize()
{
  if(w) delete w; w=0;
  if(packTree) delete packTree; packTree=0;  
  if (debug) 
    {
      if ((TFile*)gROOT->GetListOfFiles()->FindObject(pFileName.Data())) 
	{
	  pFile->Write(); 
	  pFile->Close();
	}

      if ((TFile*)gROOT->GetListOfFiles()->FindObject(ntupleOutputFileName.Data()))
	{
	  ntupleOutputFile->cd();
	  ntuple->Write();
	  ntupleOutputFile->Save();
	  ntupleOutputFile->Close();
	}

      if ((TFile*)gROOT->GetListOfFiles()->FindObject(ntupleOutputFile2Name.Data()))
	{
	  ntupleOutputFile2->cd();
	  ntuple2->Write();
	  ntupleOutputFile2->Save();
	  ntupleOutputFile2->Close();
	}
    }
  return kTRUE;
}


Int_t HMdcUnpacker::fill() 
{
  if (pSubEvt && pReadout) 
    {
      HLocation loc;
      loc.set(4, 0, 0, 0, 0);
      
      UInt_t* data = pSubEvt->getData();
      UInt_t* end  = pSubEvt->getEnd();

      while( data < end ) 
	{
	  
	  //decoding sub-header
	  if(w->subHeader(data))
	    {
	      // Test for not reasonable  subevtsize
	      
	      //if(w->getSizeStatus() == 1){  /* return 1;*/ } 
	      if(w->getSizeStatus() >= 1) 
		{	
		  if (debug && extraDebug)  cout << endl << "Next Event of Subevent Id " << subEvtId << endl;
		  if( extraDebug )
		    {
		      switch(w->getSizeStatus())
			{
			case 1:
			  cout << "Roc Nr " << w->getRocNb() << " Subevent Header: Event Truncated!" << endl;
			  break;
			case 4:
			  cout << "Roc Nr " << w->getRocNb() << " Subevent Header: Token not returned on port 0" << endl;
			  break;
			case 8:
			  cout << "Roc Nr " << w->getRocNb() << " Subevent Header: Token not returned on port     1" << endl;
			  break;
			case 12:
			  cout << "Roc Nr " << w->getRocNb() << " Subevent Header: Token not returned on port 0 & 1" << endl;
			  break;
			default:
			  cout << "Roc Nr " << w->getRocNb() << " Subevent Header: other value >= 1" << w->getSizeStatus() << endl;
		    }
		  }
		}
	      
	      if (debug)
		{
		  if(w->getSizeStatus() >= 1) 
		    {
		      ntuple2->Fill(gHades->getCurrentEvent()->getHeader()->getEventSeqNumber(),
				    subEvtId,w->getRocNb(),
				    w->getSizeStatus(),
				    w->getSizeStatus() & 0x1,
				    (w->getSizeStatus() >> 2) & 0x3,
				    (w->getSizeStatus() >> 2) & 0x1,
				    (w->getSizeStatus() >> 3) & 0x1);

		  if( extraDebug )
		    {
		      switch(w->getSizeStatus())
			{
			case 1:
			  cout << "Roc Nr " << w->getRocNb() << " Subevent Header: Event Truncated!" << endl;
			  break;
			case 4:
			  cout << "Roc Nr " << w->getRocNb() << " Subevent Header: Token not returned on port 0" << endl;
			  break;
			case 8:
			  cout << "Roc Nr " << w->getRocNb() << " Subevent Header: Token not returned on port     1" << endl;
			  break;
			case 12:
			  cout << "Roc Nr " << w->getRocNb() << " Subevent Header: Token not returned on port 0 & 1" << endl;
			  break;
			default:
			  cout << "Roc Nr " << w->getRocNb() << " Subevent Header: other value >= 1" << w->getSizeStatus() << endl;
			}
		    }      
		  
		    } // introduced by PZ beamtime nov00
		}
	    }
	  else
	    { 
	      Error("HMdcUnpacker::fill"," can't decode sub header \n"); 
	      return -1;
	    }
	  
          Int_t offset = w->getSubEvtSize()+3; 
	  
          for(UInt_t* deb=data+3; deb<data+offset; deb++)
	    {
	      w->clear(); 
	      if(w->decode(*deb)){
		
		if( debug ) packTree->Fill(); 
		
		if( (w->getBus()) < 0) {
		  UInt_t dato=(*deb);
		  HEvent* rec = gHades->getCurrentEvent();
		  printf(" Event nb:%i\n",
			 rec->getHeader()->getEventSeqNumber());
		  printf("Error in raw values !!! \n"); 
		  printf("id:%i sam:%i samP:%i Roc#:%i RocP:%i Bus:%i \n", subEvtId, 
			 w->getSam(),w->getSamP(),w->getRoc(), 
			 w->getRocP(),w->getBus());  
		  printf("Data word %x \n",dato);
		  goto loop;
		}
	
		Int_t nsam = w->getSam()*2+w->getSamP();

		HMdcEvReadoutBus& bus =(*pReadout)[nsam]
		                                  [w->getRoc()]
		                                  [w->getRocP()]
		                                  [w->getBus()];
		Int_t sector= bus.getSec(); 
		Int_t module= bus.getMod();
		Int_t mbo   = bus.getMbo();
		Int_t tdc = getTdc(w->getTdc(),w->getChan());

		if ( (mbo <0)  || (tdc<0) ) {
		  HEvent* recEvt = gHades->getCurrentEvent();
		  recEvt->getHeader()->getEventSeqNumber();
		  
#ifdef TRACEON
		  log << "*****************************************"<< endl; 
		  log << "Decoding Error in Evt number: "<<
		    recEvt->getHeader()->getEventSeqNumber()<< endl;
		  log << "*****************************************"<< endl;  
		  log << "mbo:" << mbo << "tdc:" << tdc << endl;
		  log << "sam:" << nsam <<"Roc#:" <<w->getRoc() 
		      << "RocP:" << w->getRocP() << "Bus:" << w->getBus() << endl;
#endif
		  goto loop;
		}

		//set location indexes 
		
		loc[0] = sector;
		loc[1] = module;
		loc[2] = mbo;
		loc[3] = tdc;

		// If MDCs are not run via the common daq system, but in a 
		// standalone mode,
		// the event trig type is not provided. 
		// In case of calibration triggers which are indicated in addition
		// by the roc header information, the trigger type is set 
		// to calibration event.
 		
		if (standalone) 
		  {
		    if (w->getTrigType()==kCalibrationEvent)
		      {
			gHades->getCurrentEvent()->getHeader()->setId(kCalibrationEvent);
		      }
		  }
   		
		HMdcRaw *pCell= NULL;
		pCell = (HMdcRaw*) pRawCat->getObject(loc);
		if (!pCell) {
		  pCell = (HMdcRaw*) pRawCat->getSlot(loc);
		  if (pCell) {
		    pCell = new (pCell) HMdcRaw();
		    pCell->setSector(sector);
		    pCell->setModule(module);  
		    pCell->setMbo(mbo);
		    pCell->setTdc(tdc);
		  }
		  else {
		    Error("HMdcUnpacker::fill()", "can't get slot\n");
		    return -2;
		  }
		}

		pCell->setTime(w->getTime(),tdcMode);
	
		if (debug)
		  {	
		    if (pCell->getNHits()>2) 
		      {
			ntuple->Fill(gHades->getCurrentEvent()->getHeader()->getEventSeqNumber(),
				     pCell->getNHits(),
				     sector, module, mbo, tdc,
				     w->getSam(), w->getSamP(),w->getRoc(),
				     w->getRocP(),w->getBus());
		      }
		  }
	      }
	      else {Error("HMdcUnpacker::fill()"," error while decoding the data-word \n");
		  return -1;}
	    }
	loop: 
	  data += offset;
	}
    }
  return 1;
}    

void HMdcUnpacker::print() {
  /*
            HEvent* recEvt = gHades->getCurrentEvent();
            recEvt->getHeader()->getEventSeqNumber();

#ifdef TRACEON    
            log <<"*****************************************"<< endl; 
            log <<" Evt number: "<<
	       recEvt->getHeader()->getEventSeqNumber()<< endl;
            log <<"*****************************************"<< endl;  
            log <<  "sam: " << nsam <<"Roc# controler : " <<w->getRoc() 
              << "Lvl bus: " << w->getRocP() << "Bus: " << w->getBus() << endl;

	    log << "sector:" << sector << "module:" << module << endl;
            log << "mbo:" << mbo << "tdc:" << tdc << endl;
#endif

    if (pSubEvt) {
        w->clear();
	for (UInt_t* data = pSubEvt->getData(), *end = pSubEvt->getEnd();
 	     data < end; data++){
         printf("values of data ===>%i\n",*data );
         
	  //   if(w->decode(*data)){
	  //   }else{
	  // printf("problem with data decoding !\n");
	  //	}
    }
}
*/

}
ClassImp(HMdcDataWord)
ClassImp(HMdcUnpacker)







