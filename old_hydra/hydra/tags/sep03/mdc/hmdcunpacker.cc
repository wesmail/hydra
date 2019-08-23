// File: hmdcunpacker.cc
//
// Author: Walter Karig <W.Karig@gsi.de>
// (Co-Author meanwhile Author) Modified: May 26, 03 by Peter Zumbruch <P.Zumbruch@gsi.de>

////////////////////////////////////////////////////////////
//
// HMdcUnpacker
// 
// Unpacker for mdc data
// decodes the raw list mode data and fills the HMdcRaw category
// using information from the parameter containers HMdcEvReadout 
// and HMdcRawStruct.
// For decoding the functions of HMdcDataWord are used.
// Additionally the categories HMdcRawEventHeader and on purpose 
// the HMdcDataWord (not yet implemented) 
// 
// The constructor gives several (additional) options 
// when being called: 
//
// HMdcUnpacker( subEvtId, debug, consistency )
// HMdcUnpacker( subEvtId, version, debug, consistency)
//
//      subEvtId   : subevent Id for which this 
//                   Instance is called by the Hades::eventLoop() calls to 
//                   the HldSource and derived classes
//
//      version    : decoding version for the unpacker
//
//                   version = 1, used for data since  2002-10-01 12:00:00
//                                includes new naming scheme for the SAMs
//                   version = 0, used for data before 2002-10-01 12:00:00
//
//                   if not set in the constructor or via setDecodeVersion()
//                   the decoding is derived from the referenceRunId (if set) 
//                   or the runId
//                                                  
//                         (reference)RunId < versionSwitchId : version 0
//                         (reference)RunId > versionSwitchId : version 1
//
//      debug      : additional debug features of the Unpacker for debugging 
//                   the hardware
//
//                   debug = TRUE:
//
//                       1) Errors/Warnings are extended by 
//                       detailed informations of the dataword
//                       (if not switched of by setQuietMode())
//                       2) HMdcRawEventHeader is written out for every call 
//                       of decodeSubHeader
//                       of the Header (not only for not zero error 
//                       words in the sub header (ROC)
//                       3) HMdcDataWord is written to tree (not yet implemented)
//                       persistency changeable with setPersistencyDataWord()
//
//                   debug = FALSE: (default)
//
//                       1) Errors/Warnings are displayed in a reduced set     
//                          (if not completely switched of by setQuietMode())
//                       2) HMdcRawEventHeader is only written out for a non zero error word 
//                          in the sub header (ROC)
//                      
//      consistency: enables a rough border checking on the hardware level
//                   on the data words while decoding.
//                   this is actually a feature of HMdcDataWord::decode() HMdcDataWord::subheader()
//                   which is called during execute().
// 
// 
////////////////////////////////////////////////////////////

using namespace std;

//#include <cstdio>
#include <iostream> 
#include <iomanip>
#include <fstream>

#include <cstdlib>

#include "hades.h"
#include "hcategory.h"
#include "hdatasource.h"
#include "hiterator.h"
#include "hldsubevt.h"
#include "hevent.h"
#include "hmdcraw.h"
#include "hmdcrawstruct.h"
#include "hspectrometer.h"
#include "hmdcdetector.h"
#include "hruntimedb.h"
#include "heventheader.h"
#include "TROOT.h"

#include "hmdcdataword.h"
#include "hmdcunpacker.h"

ClassImp(HMdcUnpacker)

HMdcDataWord *HMdcUnpacker::dataword=0;
Bool_t        HMdcUnpacker::noComment=kFALSE;

HMdcUnpacker::HMdcUnpacker(Int_t id , Bool_t dbg, Bool_t consisCheck) : HldUnpack()
{ 
  // id = sub event id for which the unpacker is called by 
  subEvtId = id; 
  dataword = new HMdcDataWord(); 

  debug=dbg;
  consistencyCheck=consisCheck;
  setDefaults();
}

// ----------------------------------------------------------------------------------------------------

HMdcUnpacker::HMdcUnpacker(Int_t id, Int_t myversion, Bool_t dbg, Bool_t consisCheck)
{
  HMdcUnpacker::HMdcUnpacker(id, dbg, consisCheck); 
  setDecodeVersion(myversion);
}

// ----------------------------------------------------------------------------------------------------

HMdcUnpacker::HMdcUnpacker(const HMdcUnpacker& mdc) 
{ 
  subEvtId = mdc.subEvtId; 
  dataword=mdc.dataword;
}

// ----------------------------------------------------------------------------------------------------

HMdcUnpacker::~HMdcUnpacker()
{  
  if(dataword) delete dataword; 
  dataword=0;
} 

// ----------------------------------------------------------------------------------------------------

Bool_t HMdcUnpacker::getQuietMode()
{
  // get status of quietMode
  // quiet = kTRUE : messaging switch off
  //       = kFALSE: messages are displayed
  
  return noComment;
}

// ----------------------------------------------------------------------------------------------------

void HMdcUnpacker::setQuietMode(Bool_t quiet)
{
  // switch to disable all error messages and warnings from hmdcunpacker AND hmdcdataword
  // quiet = kTRUE : messaging switch of
  //       = kFALSE: messages are displayed

  noComment = quiet;
  if (quiet) Warning("setQuietMode",
		     "subEvtId %i - from now on all error and warning messages of this unpacker are skipped, be aware of the consequences", 
		     subEvtId);
}

// ----------------------------------------------------------------------------------------------------

void HMdcUnpacker::setContinueDecodingAfterInconsistency(Bool_t cont)
{
  // this function is only for debugging use!
  // the decoding of the data words continue even if there is 
  // unconsistency in the datawords
  // cont = kTRUE : continue
  // cont = kFALSE: skip event

  continueDecodingAfterInconsistency=cont; 
}

// ----------------------------------------------------------------------------------------------------

void HMdcUnpacker::setDefaults(void)
{
  // set the default values for all intrinsic variables
  // called by the constructor

  pReadout = NULL;
  standalone=kFALSE;
  tdcMode=0;
  pMdcRawEventHeaderCat=0;
  disableSamNrConsistencyCheck=kFALSE;
  mdcRawEventHeaderCatPersistency=kFALSE;
  mdcDataWordCatPersistency=kFALSE;
  fillDataWord=kFALSE;
  subHeaderDecodeVersion=0;
  maxDecodeVersion=1;
  setDecodeVersion(1,kFALSE);
  unpackerDebug=kFALSE;
  rocEvtHeaderSize = UNCOMPRESSED_ROC_HEADERSIZE; 
  noComment=kFALSE;

  for(Int_t s=0; s<6; s++) {
    for(Int_t m=0; m<4; m++) {
      setup[s][m]=0;
    }
  }
  rawc=NULL;
  pRawCat = NULL;
  pMdcRawEventHeaderCat = NULL;
  pMdcDataWordCat = NULL;
  continueDecodingAfterInconsistency=kFALSE; 
}

// ----------------------------------------------------------------------------------------------------

UInt_t HMdcUnpacker::getDecodeVersion(void)
{
  // returns decoding version of the unpacker
  // 
  //      version    : decoding version for the unpacker
  //
  //                   version = 1, used for data since  2002-10-01 12:00:00
  //                                includes new naming scheme for the SAMs
  //                   version = 0, used for data before 2002-10-01 12:00:00
  //

return decodeVersion;

}        
// ----------------------------------------------------------------------------------------------------

void HMdcUnpacker::setDecodeVersion(UInt_t version,Bool_t manual)
{
  // set decoding version of the unpacker
  // 
  //      version    : decoding version for the unpacker
  //
  //                   version = 1, used for data since  2002-10-01 12:00:00
  //                                includes new naming scheme for the SAMs
  //                   version = 0, used for data before 2002-10-01 12:00:00
  //
  //      manual     : used for internal calls 

  if (version>maxDecodeVersion || version < 0)
    {
      Error("setDecodeVersion()",
	    " SubEvtId %i - selected version %i out of range [0,%i] ... exiting",
	    subEvtId, version, maxDecodeVersion);
      exit(EXIT_FAILURE);
    }
  else
    {
      decodeVersion=version; 
      versionSetManual=manual;
    }
};        

// ----------------------------------------------------------------------------------------------------

void HMdcUnpacker::determineDecodeVersion(void)
{
  // Begin_Html <b> obsolete !!! </b> End_Html
  // Begin_Html <b> cannot be used any more since nov02, because the runId's are now </b> End_Html
  // Begin_Html <b> random numbers not containing any useful time information </b> End_Html
  // Begin_Html <b> you have to set the decode version manually with setDecodeVersion() </b> End_Html
  //
  // method to determine from the referenceId/runId the time and so the valid version for decoding
  // see the discription in setDecodeVersion()
  //

  if (!versionSetManual)
    {
      determineDecodeVersionCtr++;
      if ( determineDecodeVersionCtr == 1)
	{
	  switch(compareEventDateTimeTo(2002,10,1,12,0,0)) // 2002-10-01/12:00:00
	    {
	    case 0:
	      // "now"
	      setDecodeVersion(1,kFALSE);
	      break;
	    case  1:
	      // event before date
	      setDecodeVersion(1,kFALSE);
	      break;
	    case -1:
	      // event before date
	      setDecodeVersion(0,kFALSE);
	      break;
	    default:
	      Error("determineDecodeVersion()",
		    " SubEvtId %i - cannot determine decode version ... exiting",subEvtId);
	      exit(EXIT_FAILURE);
	      break;
	    }
	  if (unpackerDebug) Info("determineDecodeVersion()"," SubEvtId %i - decode version: %i ",subEvtId,getDecodeVersion());
	}
    }

}  

// ----------------------------------------------------------------------------------------------------

char * HMdcUnpacker::myexec(char * in)
{
  // tool to execute an arbitray unix command 
  // the output is returned as an char pointer (maximum size 1000)

  Char_t *tmp = new Char_t[1000];
  Char_t textfile[1000];
  FILE *pipe = gSystem->OpenPipe(in,"r");
  fscanf(pipe,"%s",textfile);
  gSystem->ClosePipe(pipe);
  sprintf(tmp,"%s",textfile);
  return tmp;
}

// ----------------------------------------------------------------------------------------------------

Int_t HMdcUnpacker::compareEventDateTimeTo(UInt_t cyear,UInt_t cmonth, UInt_t cday,UInt_t chour,UInt_t cmin,UInt_t csec)
{
  // function to compare were the date and time of the event is relative to the given Time and Date
  // returns:
  // 
  //  0 : time and date are the same 
  //  1 : time and date of event are after the given time
  // -1 : time and date of event are before the given time

  HEventHeader * header = 0;
  header = ((HEvent*)(gHades->getCurrentEvent()))->getHeader();
  
  if (header != 0)
    {
      Char_t command[1000];

      sprintf(command,"date --date=\"%04i-%02i-%02i %02i:%02i:%02i\" +%%s",cyear,cmonth,cday,chour,cmin,csec);
      Int_t ctime = atoi(myexec(command));

      UInt_t date = header->getDate();
      UInt_t time = header->getTime();
      
      UInt_t year = (date & 0xFF0000) >> 16;
      year = (year < 94)? year + 2000 : year + 1900;
      UInt_t month = (date & 0xFF00) >> 8;
      UInt_t day = date & 0xFF;
      
      UInt_t hour = (time & 0xFF0000) >> 16;
      UInt_t min = (time & 0xFF00) >> 8;
      UInt_t sec = (time & 0xFF);


      if (unpackerDebug) Info("compareEventDateTimeTo()"," SubEvtId %i - time-stamp: %04i-%02i-%02i %02i:%02i:%02i",
			      subEvtId,year,month,day,hour,min,sec);
  
      sprintf(command,"date --date=\"%04i-%02i-%02i %02i:%02i:%02i\" +%%s",year,month,day,hour,min,sec);
      Int_t evttime = atoi(myexec(command));

      if (evttime == ctime) return  0; 
      if (evttime >= ctime) return  1;
      if (evttime <= ctime) return -1; 
    }
  else 
    {
      Error("compareEventDateTimeTo()",
	    " SubEvtId %i - no data header defined in current event of gHades ... exiting"
	    ,subEvtId);
      exit(EXIT_FAILURE);
    }
  return -99;
}

// ----------------------------------------------------------------------------------------------------

void HMdcUnpacker::print(TString parent)
{
  // prints out the variables of the unpacker object
  
  Info(parent.Data(),"############################################################");
  Info(parent.Data(),"HMdcUnpacker: ClassName %s :",this->ClassName());
  Info(parent.Data(),"%s: %p ClassName %s","HMdcEvReadout",pReadout,(pReadout)?pReadout->ClassName():"---"); //!Lookup up table 
  Info(parent.Data(),"%s: %3i ","sub event Id",subEvtId);
  Info(parent.Data(),"%s: %i","TdcMode",tdcMode);
  Info(parent.Data(),"%s: %s","debug",(debug)?"kTRUE":"kFALSE");             
  Info(parent.Data(),"%s: %s","consistencyCheck",(consistencyCheck)?"kTRUE":"kFALSE");                  
  Info(parent.Data(),"%s: %s","disableSamNrConsistencyCheck",(disableSamNrConsistencyCheck)?"kTRUE":"kFALSE");     
  Info(parent.Data(),"%s: %s","mdcRawEventHeaderCatPersistency",(mdcRawEventHeaderCatPersistency)?"kTRUE":"kFALSE");   
  Info(parent.Data(),"%s: %s","mdcDataWordCatPersistency",(mdcDataWordCatPersistency)?"kTRUE":"kFALSE");         
  Info(parent.Data(),"%s: %s","fillDataWord",(fillDataWord)?"kTRUE":"kFALSE");                                  
  Info(parent.Data(),"%s: %s","standalone",standalone?"kTRUE":"kFALSE"); 
  Info(parent.Data(),"%s: %3i ","decodeVersion",decodeVersion);
  Info(parent.Data(),"%s: %2i ","subHeaderDecodeVersion",subHeaderDecodeVersion);
  Info(parent.Data(),"%s: %10i ","versionSwitchId (RunId) from which on to use version 1",versionSwitchId);
  Info(parent.Data(),"%s: %3i ","ROC headerSize",rocEvtHeaderSize); 
};

// ----------------------------------------------------------------------------------------------------

Int_t HMdcUnpacker::fillMdcRawEventHeader(Int_t errorFlag, Int_t rocModule, Int_t rocNumber, 
					  Int_t rocSector, Int_t subEvtId,  Int_t subEvtSize, 
					  Int_t trigType)
{
  // function for filling the category HMdcRawEventHeader

  HLocation dummy;
  HMdcRawEventHeader *pMdcRawEventHeader= NULL;
  pMdcRawEventHeader = (HMdcRawEventHeader*) pMdcRawEventHeaderCat->getNewSlot(dummy);
  
  if (pMdcRawEventHeader) 
    {
      pMdcRawEventHeader = new (pMdcRawEventHeader) HMdcRawEventHeader();
    }
  else 
    {
      if(!noComment)
	{ 
	  Error("fillMdcRawEventHeader()", "SubEvtId %i - can't get slot for MdcEventHeader",subEvtId);
	  Error("fillMdcRawEventHeader()", "              ..... but continuing unpacking for the rest of this sub event!!!");
	}
      return -2;
    }
 
  Int_t samNr; // samNr = sam * 2 + samPos

  samNr = -1;
  samNr = getSubEvtIdToSamNr(subEvtId);

  if (samNr != -1) 
    {
      rocSector = pReadout->getSamNumberToSector(samNr);
      rocModule = pReadout->getSamNumberToModule(samNr);
    }
  else
    {
      rocSector = 0 - subEvtId; // until lookup-table sam->sector/module exists
      rocModule = 0 - subEvtId; // until lookup-table sam->sector/module exists
    }

  pMdcRawEventHeader->set(errorFlag, rocModule, rocNumber, 
			  rocSector, subEvtId,  subEvtSize, trigType);

  return 0;
}

// ----------------------------------------------------------------------------------------------------

Int_t HMdcUnpacker::fillMdcDataWord(Int_t sector,Int_t module,Int_t mbo,Int_t tdc)
{
  // function for filling the category HMdcDataword
  // 

  Int_t entry=0;
  UChar_t maxentry=10;

  maxentry=8;
  
  HLocation dataLoc;
  dataLoc.set(5,0,0,0,0,0);
  //set location indexes 
  
  dataLoc[0] = sector;
  dataLoc[1] = module;
  dataLoc[2] = mbo;
  dataLoc[3] = tdc;
  dataLoc[4] = entry;
  
  HMdcDataWord *pMdcDataWord=NULL;
  
  for (entry = 0; entry < maxentry; entry++)
    {
      pMdcDataWord = NULL;
      
      dataLoc[4] = entry;
      pMdcDataWord = (HMdcDataWord*) pMdcDataWordCat->getObject(dataLoc);
      if (!pMdcDataWord) break;
      //       Info("HMdcUnpacker","%2i %10p ",entry,pMdcDataWord);
      //       Info("HMdcUnpacker","s:%i m:%i mb:%2i td:%2i e:%2i ",dataLoc[0],dataLoc[1],dataLoc[2],dataLoc[3],dataLoc[4]);
      //       dataLoc.Dump();
      //       pMdcDataWord->print();
    }

  //   cout << ".............................." << endl;
  //   Info("HMdcUnpacker","%2i %10p ",entry,pMdcDataWord);
  //   Info("HMdcUnpacker","s:%i m:%i mb:%2i td:%2i e:%2i ",dataLoc[0],dataLoc[1],dataLoc[2],dataLoc[3],dataLoc[4]);
  //   dataLoc.Dump();
  
  if (pMdcDataWord)
    {
      if (entry==maxentry-1)
	{
	  if(!noComment) Error("fillMdcDataWord()", " SubEvtId %i - to many entries to the same location entries>%i",subEvtId,maxentry);
	}
      else
	{
	  if(!noComment) 
	    {
	      Error("fillMdcDataWord()", " SubEvtId %i - can't get empty slot for MdcDataWord",subEvtId);
	      Error("fillMdcDataWord()", "              ..... but continuing unpacking for the rest of this sub event!!!");
	    }
	}
      return -2;
    }
  else
    {
      pMdcDataWord = (HMdcDataWord*) pMdcDataWordCat->getSlot(dataLoc);
      if (pMdcDataWord) 
	{
	  pMdcDataWord = new (pMdcDataWord) HMdcDataWord(*dataword);
	  pMdcDataWord->setAdress(sector,module,mbo,tdc,entry);
	}
      else
	{
	  //	  dataLoc.Dump();
	  if(!noComment) 
	    {
	      Error("fillMdcDataWord()", " SubEvtId %i - can't get slot for MdcDataWord",subEvtId);
	      Error("fillMdcDataWord()", "              ..... but continuing unpacking for the rest of this sub event!!!");
	    }
	  return -2;
	}
    }
  return 1;
}

// ----------------------------------------------------------------------------------------------------

Bool_t HMdcUnpacker::testMdcSetup(Int_t s, Int_t m)
{
  // Tests the Mdc setup if the modules are present
  // in the running analysis

  Bool_t result=kFALSE;
  if(setup[s][m]==0) result=kFALSE;
  if(setup[s][m]==1) result=kTRUE;
  return result;
}

// ----------------------------------------------------------------------------------------------------

void HMdcUnpacker::getMdcSetup()
{
  // gets Mdc detector setup

  HMdcDetector* mdcDet=(HMdcDetector*)(((HSpectrometer*)(gHades->getSetup()))->getDetector("Mdc"));
  if (!mdcDet)
    {
      Error("getMdcSetup()"," SubEvtId %i - Mdc-Detector setup (gHades->getSetup()->getDetector(\"Mdc\")) missing.",subEvtId);
    }
  for(Int_t s=0; s<6; s++) {
    for(Int_t m=0; m<4; m++) {
      if (!mdcDet->getModule(s, m)) setup[s][m]=0;
      if ( mdcDet->getModule(s, m)) setup[s][m]=1;
    }
  }
}

// ----------------------------------------------------------------------------------------------------

Int_t HMdcUnpacker::getSubEvtIdToSamNr(Int_t subEvtId, Bool_t expectUndefined)
{
  // determines the samNr from the subEvtId 
  // in addition it crosschecks the samNr with the DAQ specifications 
  // and does boundary checks by default.
  // You can switch it off by setDisableSamNrConsistencyCheck()

  Int_t samNr;
  // determine samNr

  switch(decodeVersion)
    {
    case 0:
      samNr = 2*(subEvtId - 201); // version before Nov02
      break;
    case 1:
      samNr = subEvtId - 201; // version since Nov02
      break;
    default:
      samNr = -1;
    }

  // additional checks 

  if (disableSamNrConsistencyCheck)
    {
      if (samNr < 0) return -1;
      return samNr;
    }

  // additional checks by default on !!

  switch(decodeVersion)
    {
    case 0:
      if (!(200 < subEvtId && subEvtId < 300))
	{
	  if(!noComment) Error("getSubEvtIdToSamNr()"," SubEvtId %i - sub event id: %i, out of mdc range ]200,300[",subEvtId,subEvtId);
	  return -1;
	}
      break;
    case 1:
      if (!(200 <= subEvtId && subEvtId < 300))
	{
	  if(!noComment) Error("getSubEvtIdToSamNr()"," SubEvtId %i - sub event id: %i, out of mdc range [200,300[",subEvtId,subEvtId);
	  return -1;
	}
      break;
    default:
      samNr = -1;
    }

  
  Int_t rocSector = pReadout->getSamNumberToSector(samNr);

  if (rocSector==-1 && !expectUndefined)
    {
      if(!noComment) Error("getSubEvtIdToSamNr()",
			   "SubEvtId %i - calculated samNr: %i is not in the lookup table (HMdcEvReadout)",
			   subEvtId,samNr);
      return -1;
    }
  return samNr;
}

// ----------------------------------------------------------------------------------------------------

Bool_t HMdcUnpacker::checkMboRawStructBounds(Int_t sector,Int_t module, Int_t mbo, Int_t tdc , UInt_t ldataword)
{
  // checks consistency whether ldataword is inside 
  // the bounds of the parameter container MboRawStruct
  // return value
  //              kTRUE the checks were passed
  //              kFALSE if otherwise

  Int_t nMaxSec = rawc->getSize() ;
  if (sector >= nMaxSec || sector < 0 )
    {
      if (!noComment)
	{
	  Error("checkMboRawStructBounds",
		" Event: %i - SubEvtId %3i - sector %i, number out of bounds [0,%i[ for dataword 0x%x",
		((HEventHeader*)((HEvent*)(gHades->getCurrentEvent())->getHeader()))->getEventSeqNumber(),subEvtId,
		sector,
		nMaxSec,
		ldataword); 
	  if (unpackerDebug) dataword->dump();
	}
      return kFALSE;
    }
  Int_t nMaxModule = (*rawc)[sector].getSize() ;
  if (module >= nMaxModule || module < 0 )
    {
      if (!noComment)
	{
	  Error("checkMboRawStructBounds",
		" Event: %i - SubEvtId %3i - module %i, number out of bounds [0,%i[ for sector: %i for dataword 0x%x ",
		((HEventHeader*)((HEvent*)(gHades->getCurrentEvent())->getHeader()))->getEventSeqNumber(),subEvtId,
		module,
		nMaxModule, 
		sector,
		ldataword);
	  if (unpackerDebug) dataword->dump();
	}
      return kFALSE;
    }
  Int_t nMaxMbo = (*rawc)[sector][module].getSize() ;
  if (mbo >= nMaxMbo || mbo < 0 )
    {
      if (!noComment)
	{
	  Error("checkMboRawStructBounds",
		" Event: %i - SubEvtId %3i - mbo %i, number out of bounds [0,%i[ for sector: %i, module: %i for dataword 0x%x",
		((HEventHeader*)((HEvent*)(gHades->getCurrentEvent())->getHeader()))->getEventSeqNumber(),subEvtId,
		mbo, 
		nMaxMbo,
		sector, 
		module,
		ldataword);
	  if (unpackerDebug) dataword->dump();
	}
      return kFALSE;
    }
  Int_t nMaxTdc = (*rawc)[sector][module][mbo].getNTdcs() ;
  if (tdc >= nMaxTdc || tdc < 0 )
    {
      if (!noComment)
	{
	  Error("checkMboRawStructBounds",
		" Event: %i - SubEvtId %3i - tdc %i, number out of bounds [0,%i[ for sector: %i, module: %i, mbo: %i for dataword 0x%x",
		((HEventHeader*)((HEvent*)(gHades->getCurrentEvent())->getHeader()))->getEventSeqNumber(),subEvtId,
		tdc,
		nMaxTdc, 
		sector, 
		module,
		mbo,
		ldataword);
	  if (unpackerDebug) dataword->dump();
	}
      return kFALSE;
    }

  return kTRUE;
}

// ----------------------------------------------------------------------------------------------------

Bool_t HMdcUnpacker::checkMdcSetup(Int_t sector, Int_t module, UInt_t ldataword)
{
  // checks whether module and sector are set in the MdcSetup
  // (dataword is only needed for debug information )
  // returns kTRUE if set, kFALSE else

  if(!testMdcSetup(sector,module)) 
    {
      if(!noComment) 
	{
	  Error("checkMdcSetup()"," Event: %i - SubEvtId %3i - though sector %i, module %i is excluded in the setup,  this dataword: 0x%08x points to this detector.",
		((HEventHeader*)((HEvent*)(gHades->getCurrentEvent())->getHeader()))->getEventSeqNumber(),
		subEvtId,sector,module,ldataword);
	  if (unpackerDebug) dataword->dump();
	}
      return kFALSE; 
    }
  return kTRUE;
}

// ----------------------------------------------------------------------------------------------------

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

// ----------------------------------------------------------------------------------------------------

void HMdcUnpacker::setRocEvtHeaderSize(UInt_t ui)
{
  // function to set the size in words of the ROC Header

  rocEvtHeaderSize=ui;
}

// ----------------------------------------------------------------------------------------------------

Int_t HMdcUnpacker::getTdcMode(void)
{
  // returns mode the tdc is triggering on signals
  // mode 0 (default) : trigger on leading and trailing edge
  // mode 1           : trigger on 2 leading edges:

  return tdcMode;
}

// ----------------------------------------------------------------------------------------------------

HCategory* HMdcUnpacker::initCategory(Cat_t cat, TString catname, TString detector)
{
  // inits the container cat 
  // looks first whether it already exists
  // otherwise it is created
  // default for detector is "Mdc"
  // catname is used for Error handling

  HCategory * category = 0;

  category = (HCategory*)(((HEvent*)(gHades->getCurrentEvent()))->getCategory(cat));

  if (!category)
    {
      category=(HCategory*)((HMdcDetector*)(((HSpectrometer*)(gHades->getSetup()))->getDetector(detector.Data()))->buildCategory(cat));
 
      if (!category)
	{
	  Error("initCategory()"," SubEvtId %i - can't create or get category \"%s\"",subEvtId,catname.Data());
	  return NULL;
	}

      if (!((HEvent*)(gHades->getCurrentEvent()))->addCategory(cat,category,detector.Data()))
	{
	  Error("initCategory()"," SubEvtId %i - can't add category \"%s\" to detector \"%s\"",subEvtId,catname.Data(),detector.Data());
	  return NULL;
	}
    }
  return category; 
}

// ----------------------------------------------------------------------------------------------------

Bool_t HMdcUnpacker::init() 
{
  // Initialisation of 
  //                  - lookup table
  //                  - category for raw-level  storage
  //                  - additional category if debug option
  //                  - additional Tree if debug option
  //                  - local mdc setup table for lookup

  getMdcSetup();

  pRawCat = NULL;

  HRuntimeDb *rtdb = gHades->getRuntimeDb();
  rawc = (HMdcRawStruct*) rtdb->getContainer("MdcRawStruct");
  if (!rawc)
    {
      Error("init()"," SubEvtId %i - can't get container \"%s\" ",subEvtId,"MdcRawStruct");
      return kFALSE;
    }

  if (!rawc->isStatic()) ((HParSet*)rawc)->init(); //Needs explicit initialization

  pReadout = (HMdcEvReadout*) rtdb->getContainer("MdcEvReadout");
  if (!pReadout)
    {
      Error("init()"," SubEvtId %i - can't get MdcEvReadout", subEvtId);
      return kFALSE;
    }
  
  pRawCat = initCategory(catMdcRaw,"catMdcRaw");
  if (!pRawCat) return kFALSE;

  pMdcRawEventHeaderCat = initCategory(catMdcRawEventHeader,"catMdcRawEventHeader");
  if (!pMdcRawEventHeaderCat) return kFALSE;
  pMdcRawEventHeaderCat->setPersistency(mdcRawEventHeaderCatPersistency);

  if (debug && fillDataWord) 
    { 
      if (!(pMdcDataWordCat = initCategory(catMdcDataWord,"catMdcDataWord"))) return kFALSE;
      pMdcDataWordCat->setPersistency(mdcDataWordCatPersistency);
    }

  return kTRUE;
}

// ----------------------------------------------------------------------------------------------------

Bool_t HMdcUnpacker::reinit()
{
  // reinit function
  // if versionSetManual not set it determines the decodeVersion via determineDecodeVersion()

  if (!versionSetManual) 
    {
      determineDecodeVersionCtr=0;
    }
  if (unpackerDebug) print("reinit()");
  return kTRUE;
}

// ----------------------------------------------------------------------------------------------------

Int_t HMdcUnpacker::execute() 
{
  // execute funtion of the task
  // calls the fill() function
  // before it checks whether the called subEvtId corresponds 
  // to an active module in the mdc detector setup 

  // checks
  if (!pSubEvt) return 1; 

  if (!pReadout) 
    {
      if(!noComment)
	{
	  Error("execute"," Event: %i - SubEvtId %i - pointer to Lookup-Table zero",
		((HEventHeader*)((HEvent*)(gHades->getCurrentEvent())->getHeader()))->getEventSeqNumber(),
		subEvtId);
	}
      return 1;
    }

  if (!versionSetManual) determineDecodeVersion();

  Int_t samNr  = getSubEvtIdToSamNr(subEvtId,kTRUE);
  if (samNr < 0) 
    {
      if(!noComment)
	{
	  Warning("execute",
		  " Event: %i - SubEvtId %i - samNr: %i (=lookup table index+1) negative ... sub event id %i ignored",
		  ((HEventHeader*)((HEvent*)(gHades->getCurrentEvent())->getHeader()))->getEventSeqNumber(),
		  subEvtId,samNr,subEvtId);
	}
      return kTRUE;
    }

  Int_t sector = pReadout->getSamNumberToSector(samNr);
  if (sector < 0) 
    {
      if(!noComment)
	{
	  if(!noComment) Warning("execute",
				 " Event: %i - SubEvtId %i - no valid sector entry in lookuptable for virtual samNr: %i (=lookup table index+1) ... sub event id %i ignored",
				 ((HEventHeader*)((HEvent*)(gHades->getCurrentEvent())->getHeader()))->getEventSeqNumber(),
				 subEvtId,samNr,subEvtId);
	}
      return kTRUE;
    }
  Int_t module = pReadout->getSamNumberToModule(samNr);
  if (module < 0) 
    {
      if(!noComment)
	{
	  if(!noComment) Warning("execute",
				 " Event: %i - SubEvtId %i - no valid module entry in lookuptable for virtual samNr: %i (=lookup table index+1) ... sub event id %i ignored",
				 ((HEventHeader*)((HEvent*)(gHades->getCurrentEvent())->getHeader()))->getEventSeqNumber(),
				 subEvtId,samNr,subEvtId);
	}
      return kTRUE;
    }
  
  // all checks are passed
  if (testMdcSetup(sector, module)) 
    {
      // module in given setup ?
      Int_t err = fill(); // fill the data words
      if (err<0) 
	{
	  if(!noComment)
	    {
	      if(!noComment) Error("execute"," Event: %i - SubEvtId %i - error in unpacking",
				   ((HEventHeader*)((HEvent*)(gHades->getCurrentEvent())->getHeader()))->getEventSeqNumber(),
				   subEvtId);
	    }
	  return kTRUE; 
	}
    }
  return kTRUE;
}

// ----------------------------------------------------------------------------------------------------

Bool_t HMdcUnpacker::finalize()
{
  // dummy finalize function
  // returning kTRUE;
  return kTRUE;
}

// ----------------------------------------------------------------------------------------------------

Bool_t HMdcUnpacker::decodeSubHeader(UInt_t *data, UInt_t &subEvtSize)
{
  // decodes the sub header (ROC-Header) 
  // and returns the size of this header
  //
  // If the errorFlag of the ROC-Header is set or 
  // the debug flag of the unpacker is set
  // the header is written out to the category HMdcRawEventHeader 
  // (set its persistency via, setPersistencyRawEventHeader())
  //
  // If the standalone mode is chosen the trigger type (id) of the whole event 
  // is set to the trigger type of the roc header if it is a calibration trigger
  // (If MDCs are not run via the common daq system, but in a standalone mode,
  // the event trig type is not provided in the event header (it is always 1).) 

  Int_t errorFlag    = 0;
  Int_t trigType     = 0;
  Int_t rocNumber    = 0;
  Int_t rocSector    = 0;
  Int_t rocModule    = 0;
  
  dataword->clearHeader();
  if(!dataword->subHeader(data,subHeaderDecodeVersion,consistencyCheck))
    { 
      if(!noComment) Error("decodeSubHeader()","SubEvtId %i - can't decode sub header ",subEvtId); 
      return kFALSE;
    }
  
  rocNumber    = dataword->getRocNb();
  errorFlag    = dataword->getErrorFlag();
  trigType     = dataword->getTrigType();
  subEvtSize   = dataword->getSubEvtSize();
  
  
  if (errorFlag!=0 || debug) 
    {
      fillMdcRawEventHeader(errorFlag, rocModule, rocNumber, rocSector, subEvtId,  subEvtSize, trigType);
    }

  if (standalone) 
    {
      Int_t kMdc_Standalone_CalibrationEvent=9;
      if (trigType == kMdc_Standalone_CalibrationEvent)
	{
	  ((HEventHeader*)((HEvent*)(gHades->getCurrentEvent()))->getHeader())->setId(kMdc_Standalone_CalibrationEvent);
	}
    }

  return kTRUE;
}

// ----------------------------------------------------------------------------------------------------

Bool_t HMdcUnpacker::decodingMode(void) 
{
  // evaluates the decoding info in the sub event header
  //
  // dependent on the decoding information in the sub event header
  // the following modes are selected:
  //
  // the least significant byte of the decoding word determines the compression modes:
  // so decoding & f =
  //  
  //    0 : not allowed by definition of the sub event header
  //
  //    1 : uncompressed structure 
  //              - the event contains a roc sub structure 
  //              - error bits and roc event sizes are determined from the roc headers 
  //                and filled independently to HMdcRawEventHeader
  //
  //    2 : compressed structure
  //              - the only contains pure data words
  //              - error informations are determined from 
  //                the second least significant byte 
  //                and assigned to all roc header synchronously 
  //                to HMdcRawEventHeader but shifted by 4 bits to the left (*16)
  //                also the size of the complete sub event is assigned to each roc
  //                to distinguish from the case they have a negativ sign.
  //
  //    any other value: not defined

  Int_t rocSector    = 0;
  Int_t rocModule    = 0;

  UInt_t decoding = pSubEvt->getDecoding();
  
  UInt_t compressMode = decoding & 0xf;
  UInt_t errorBits    = decoding & 0xf0; // bits intentionally not shifted !!!

  switch(compressMode)
    {
    case UNCOMPRESSED:
      setRocEvtHeaderSize(UNCOMPRESSED_ROC_HEADERSIZE); 
      return kTRUE;
      break;

    case COMPRESSED:
      setRocEvtHeaderSize(COMPRESSED_ROC_HEADERSIZE);   
      if (errorBits!=0 || debug) 
	{
	  UInt_t size = pSubEvt->getSize();
	  UInt_t id   = pSubEvt->getId() & 0x7fffffff;
	  Int_t samNr = getSubEvtIdToSamNr(subEvtId);
	  
	  if (samNr != -1) 
	    {
	      rocSector = pReadout->getSamNumberToSector(samNr);
	      rocModule = pReadout->getSamNumberToModule(samNr);
	    }
	  else
	    {
	      rocSector = 0 - subEvtId; 
	      rocModule = 0 - subEvtId; 
	    }
	  
	  for (Int_t rocNumber=1; rocNumber<=5; rocNumber++)
	    {
	      fillMdcRawEventHeader(errorBits, rocModule, rocNumber, rocSector, subEvtId, 0-size, id);
	    }
	}
      return kTRUE;
      break;

    case 0:
      if(!noComment) Error("decodingMode()"," Event: %i -  SubEvtId %i - compressMode %i from decoding word 0x%x invalid ... sub event id %i ignored",
			   ((HEventHeader*)((HEvent*)(gHades->getCurrentEvent())->getHeader()))->getEventSeqNumber(),
			   subEvtId,compressMode,decoding,subEvtId);
      return kFALSE;
      break;

    default:
      if(!noComment) Error("decodingMode()"," Event: %i -  SubEvtId %i - compressMode %i from decoding word 0x%x invalid ... sub event id %i ignored",
			   ((HEventHeader*)((HEvent*)(gHades->getCurrentEvent())->getHeader()))->getEventSeqNumber(),
			   subEvtId,compressMode,decoding,subEvtId);
      return kFALSE;
      break;
    }
  return kFALSE;
}

// ----------------------------------------------------------------------------------------------------

Int_t HMdcUnpacker::fill() 
{
  // fill function:
  // performs the loop over the subevent data
  // 
  // -- if the ROC header size > 0 (change it via setRocEvtHeaderSize())
  //    then the roc header is decoded via decodeSubHeader()
  //    otherwise the whole subevent is decoded as once
  // 
  // -- The data loop performs the following steps
  //     1. call of decode for each dataword
  //     2. the hardware address is matched to the sector/module/mbo/tdc coordinates of HMdcRaw 
  //        via the lookup table HMdcEvReadout
  //     3. a new/existing slot at the corresponding address is requested
  //     4. the time information is filled to this slot
  //
  // --  if the debug and fillDataWord flag is set (set via setFillDataWord() and setDebug())
  //     the extended dataword information is written to the category HMdcDataword()
  //     (the persistency of this container can be set via setPersistencyDataWord())  
  // 
  // -- the return values of the function are:
  //     1 : success  
  //    -1 : Error while decoding the dataword
  //    -1 : Error while decoding the roc header
  //    -1 : Error while decoding the in data loop (data pointer exceeds data range)
  //    -2 : Error while filling the to category HMdcRaw
  //
  // -- in the case that there is no entry in the lookup table 
  //    for a correctly decoded dataword there the next dataword is taken

  if (pSubEvt && pReadout) 
    {
      HLocation loc;
      loc.set(4, 0, 0, 0, 0);

      UInt_t* data = pSubEvt->getData();
      UInt_t* end  = pSubEvt->getEnd();

      UInt_t subEvtSize = 0;
      UInt_t offset = 0;
      UInt_t *enddata = 0;

      // ---------- determine decoding mode --- affects rocEvtHeaderSize
      
      if (!decodingMode()) return -1;
      rocEvtHeaderSize = getRocEvtHeaderSize();

      while( data < end ) 
	{
	  dataword->clearAll();

	  //decoding sub-header

	  if (rocEvtHeaderSize > 0)
	    {
	      if (!decodeSubHeader(data,subEvtSize)) 
		{
		  if(!noComment) ErrorFillDecodeSubHeaderFailed(subEvtId);
		  return -1;
		}
	      offset = subEvtSize + rocEvtHeaderSize; 
	      enddata = data+offset;
	    }
	  else 
	    {
	      enddata = end;
	    }

	  //decoding data word
	  
	  for(UInt_t* deb=data+rocEvtHeaderSize; deb<enddata; deb++)
	    {
	      dataword->clear();
	      dataword->clearHeader();

	      if (deb > end)
		{ 
		  if(!noComment) ErrorFillDataPointerExceedingRange(subEvtId,deb,end,enddata);
		  if (continueDecodingAfterInconsistency) continue;
		  if(!noComment) ErrorFillSkippingRestOfSubEvent();
		  return -1;
		}
		    
	      // >>>> here the decoding of the data word is done 

	      if(!dataword->decode(*deb, decodeVersion,consistencyCheck))
		{
		  if(!noComment && unpackerDebug) dataword->dump();
		  if (continueDecodingAfterInconsistency) continue;
		  if(!noComment) ErrorFillSkippingRestOfSubEvent();
		  return -1;
		}

	      // now the hardware address is matched to the sector/module/mbo/tdc coordinates of HMdcRaw 
	      // via the lookup table HMdcEvReadout

	      Int_t nsam  = -1;
	      switch (decodeVersion)
		{
		case 0:
		  nsam = dataword->getSam()*2+dataword->getSamP();
		  break;
		case 1:
		  nsam = dataword->getVirtualSamNr();
		  break;
		}

	      Int_t nroc  = dataword->getRoc();
	      Int_t nrocP = dataword->getRocP();
	      Int_t nbus  = dataword->getBus();

	      if (nsam <0 || nroc <0 || nrocP <0 || nbus <0)
		{
		  if(!noComment) ErrorFillNoValidAddressInLookUpTable(subEvtId,*deb);
		  if (unpackerDebug) dataword->dump();
		  continue;
		}

	      HMdcEvReadoutBus& bus =(*pReadout)[nsam][nroc][nrocP][nbus];

	      Int_t sector= bus.getSec(); 
	      Int_t module= bus.getMod();
	      Int_t mbo   = bus.getMbo();
	      Int_t tdc   = getTdc(dataword->getTdc(),dataword->getChan());

	      if(!checkMdcSetup(sector,module,*deb)) continue;
	      if(!checkMboRawStructBounds(sector,module,mbo,tdc,*deb)) continue;

	      dataword->setAddress(sector,module,mbo,tdc,0);

	      //set location indexes 
	      
	      loc[0] = sector;
	      loc[1] = module;
	      loc[2] = mbo;
	      loc[3] = tdc;

	      // filling the data to the location
	      // first get a free or existing slot

	      HMdcRaw *pMdcRaw = NULL;
	      pMdcRaw = (HMdcRaw*) pRawCat->getObject(loc);
	      if (!pMdcRaw) 
		{
		  pMdcRaw = (HMdcRaw*) pRawCat->getSlot(loc);
		  if (pMdcRaw) 
		    {
		      pMdcRaw = new (pMdcRaw) HMdcRaw();
		      pMdcRaw->setAddress(sector,module,mbo,tdc);
		    }
		  else 
		    {
		      if(!noComment) ErrorFillCannotGetSlot(subEvtId,sector,module,mbo,tdc);
		      if (unpackerDebug) pSubEvt->dumpIt();
		      return -2;
		    }
		}
	      
	      // fill the times to this slot
	      
	      if (!pMdcRaw->setTime(dataword->getTime(),tdcMode))
		{
		  if(!noComment) ErrorFillErrorFillingDataWord(subEvtId,*deb);
		  if (unpackerDebug) dataword->dump();
		}
	      
	      // fill dataWord to category HMdcDataWord 

	      if(debug && fillDataWord) fillMdcDataWord(sector,module,mbo,tdc);
	    }
	  data += offset;
	}
    }
  return 1;
}    

// --------------------------------------------------------------------------------

void HMdcUnpacker::ErrorFillDecodeSubHeaderFailed(Int_t subEvtId)
{
  Error("fill()"," Event: %i -  SubEvtId %i - decoding of sub header (ROC) failed",
	((HEventHeader*)((HEvent*)(gHades->getCurrentEvent())->getHeader()))->getEventSeqNumber(),
	subEvtId);
  Error("fill()", "                           ..... skipping unpacking for the rest of this sub event!!!");
}

// -------------------------------------------------------------------------------

void HMdcUnpacker::ErrorFillDataPointerExceedingRange(Int_t subEvtId,UInt_t* deb, UInt_t* end, UInt_t* enddata)
{
  Error("fill()",
	" Event: %i -  SubEvtId %i - data pointer %p exceeding range of sub event end %p\n with calculated end point at %p",
	((HEventHeader*)((HEvent*)(gHades->getCurrentEvent())->getHeader()))->getEventSeqNumber(),
	subEvtId,deb,end,enddata);
}

// -------------------------------------------------------------------------------

void HMdcUnpacker::ErrorFillSkippingRestOfSubEvent(void)
{
  Error("fill()", "                           ..... skipping unpacking for the rest of this sub event!!!");
}


// -------------------------------------------------------------------------------

void HMdcUnpacker::ErrorFillNoValidAddressInLookUpTable(Int_t subEvtId, UInt_t deb)
{
  Error("fill()",
	" Event: %i - SubEvtId %3i - No valid address in lookup table [HMdcEvReadout] for this dataword: 0x%08x",
	((HEventHeader*)((HEvent*)(gHades->getCurrentEvent())->getHeader()))->getEventSeqNumber(),
	subEvtId, deb);
}

// -------------------------------------------------------------------------------

void HMdcUnpacker::ErrorFillCannotGetSlot(Int_t subEvtId, Int_t sector, Int_t module, Int_t mbo, Int_t tdc)
{
  Error("fill()", " Event: %i - SubEvtId %i - can't get slot for s:%i m:%i mb:%i tdc:%i ...",
	((HEventHeader*)((HEvent*)(gHades->getCurrentEvent())->getHeader()))->getEventSeqNumber(),
	subEvtId,sector,module,mbo,tdc);
  Error("fill()", "                           ..... skipping unpacking for the rest of this sub event!!!");
}


// -------------------------------------------------------------------------------

void HMdcUnpacker::ErrorFillErrorFillingDataWord(Int_t subEvtId, UInt_t deb)
{
  Error("fill()"," Event: %i - SubEvtId %i - error filling slot for dataword 0xx08%",
	((HEventHeader*)((HEvent*)(gHades->getCurrentEvent())->getHeader()))->getEventSeqNumber(),
	subEvtId, deb);
} 
