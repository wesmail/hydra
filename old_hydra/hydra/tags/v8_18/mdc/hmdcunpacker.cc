// File: $RCSfile: hmdcunpacker.cc,v $
//
// Author (previous Co-Author) Peter Zumbruch <P.Zumbruch@gsi.de>
// old Author: Walter Karig <W.Karig@gsi.de>
//
// Version: $Revision: 1.81 $
// Modified by $Author: halo $ on $Date: 2008-05-09 16:11:13 $
//_HADES_CLASS_DESCRIPTION 
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
const TString HMdcUnpacker::excludeBlackListContextDefaultString="undefined exclude black list context";
const TString HMdcUnpacker::recoverBlackListContextDefaultString="undefined recover black list context";

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

HMdcUnpacker::HMdcUnpacker(HMdcUnpacker& mdc)
{
  subEvtId         = mdc.subEvtId;
  dataword         = mdc.dataword;
  debug            = mdc.debug;
  consistencyCheck = mdc.consistencyCheck;
  setDefaults();
  setDecodeVersion(mdc.getDecodeVersion());
}

// ----------------------------------------------------------------------------------------------------

HMdcUnpacker::~HMdcUnpacker()
{
  if(dataword) delete dataword;
  dataword=0;
  if (excludeBlackListMask)       delete[] excludeBlackListMask ;
  if (excludeBlackListAdress)     delete[] excludeBlackListAdress;
  if (excludedBlackListCounters)  delete[] excludedBlackListCounters;
  if (recoverBlackListMask)       delete[] recoverBlackListMask ;
  if (recoverBlackListAdress)     delete[] recoverBlackListAdress;
  if (recoveredBlackListCounters) delete[] recoveredBlackListCounters;
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

  excludeBlackList=kFALSE;
  excludedBlackListCounters=NULL;
  excludeBlackListMask=NULL;
  excludeBlackListAdress=NULL;
  excludeBlackListContext=excludeBlackListContextDefaultString;

  recoverBlackList=kFALSE;
  recoveredBlackListCounters=NULL;
  recoverBlackListMask=NULL;
  recoverBlackListAdress=NULL;
  recoverBlackListContext=recoverBlackListContextDefaultString;
  recoveryStorage = NULL;
  recoveryStorageCounter = 0;

  createBlackListRecoveryTools=0;
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
  dataword->setQuietMode(quiet);
  if (quiet) warningSetQuietMode();

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
  //
  // if version exceeds the value of maxDecodeVersion exit is called

  if (version>maxDecodeVersion || version < 0)
    {
      errorAndExitSetDecodeVersion(version);
    }
  else
    {
      decodeVersion=version;
      versionSetManual=manual;
    }
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

void HMdcUnpacker::setTdcMode(Int_t i)
{
  // In measurement mode the TDC accepts two hits per channel. The TDC can hereby
  // trigger either on on a leading and the trailing edge of a single pulse
  // or on two leading edges.
  // This influences the polarity of the hitCounter in the HMdcRaw Class:
  // mode 0  : trigger on leading and trailing edge
  //           (hit multiplicity nHits: +1 or +2)
  // mode 1  : trigger on 2 leading edges:
  //           (hit multiplicity nHits: -1 or -2)
  // (0 default if i is not equal to 0 or 1)

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

TString HMdcUnpacker::myexec(Char_t * in)
{
  // tool to execute an arbitray unix command 
  // the output is returned as an Char_t pointer (maximum size 1000)
  // TODO: check whether PTools::myexec or other method can replace it

  Char_t textfile[1000];
  FILE *pipe = gSystem->OpenPipe(in,"r");
  fscanf(pipe,"%s",textfile);
  gSystem->ClosePipe(pipe);
  return TString(textfile);
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
// ----------------------------------------------------------------------------------------------------
// init

Bool_t HMdcUnpacker::init()
{
  // Initialisation of
  //                  - lookup table
  //                  - category for raw-level  storage
  //                  - additional category if debug option
  //                  - additional Tree if debug option
  //                  - local mdc setup table for lookup

  // get Setup informations of Mdcs
  getMdcSetup();

  pRawCat = NULL;

  // get pointer to runtime data base
  HRuntimeDb *rtdb = gHades->getRuntimeDb();

  // get parameter containers:
  // -- MdcRawStruct
  rawc = (HMdcRawStruct*) rtdb->getContainer("MdcRawStruct");
  if (!rawc)
    {
      errorInitCannotGetParameterContainer("MdcRawStruct");
      return kFALSE;
    }
  // ---- Needs explicit initialization, if not yet done, i.e not yet set to static
  if (!rawc->isStatic()) ((HParSet*)rawc)->init();

  // -- MdcEvReadout
  pReadout = (HMdcEvReadout*) rtdb->getContainer("MdcEvReadout");
  if (!pReadout)
    {
      errorInitCannotGetParameterContainer("MdcEvReadout");
      return kFALSE;
    }

  // exclude / recover black lists
  if (excludeBlackList) // feature up to now only used for beamtime sep05/apr06
    { if (initExcludeBlackList() == EXIT_FAILURE) {exit(EXIT_FAILURE);} }
  if (recoverBlackList) // feature up to now only used for beamtime sep05/apr06
    { if (initRecoverBlackList() == EXIT_FAILURE) {exit(EXIT_FAILURE);} }

  // initialize categories:
  // -- catMdcRaw
  if(!(pRawCat               = initCategory(catMdcRaw,           "catMdcRaw"))           ){ return kFALSE; }
  // -- catMdcRaw catMdcRawEventHeader
  if(!(pMdcRawEventHeaderCat = initCategory(catMdcRawEventHeader,"catMdcRawEventHeader"))){ return kFALSE; }
  // -- MdcDataWord
  if (debug && fillDataWord)
    { if (!(pMdcDataWordCat  = initCategory(catMdcDataWord,      "catMdcDataWord"))      ){ return kFALSE; }}

  // set persistencies:
  // -- RawEventHeader
  pMdcRawEventHeaderCat->setPersistency(mdcRawEventHeaderCatPersistency);
  // -- MdcDataWord
  if (debug && fillDataWord)
    {   pMdcDataWordCat->setPersistency(mdcDataWordCatPersistency); }

  // end of init
  return kTRUE;
}

// ----------------------------------------------------------------------------------------------------
// init - helpers

void HMdcUnpacker::getMdcSetup()
{
  // gets Mdc detector setup
  // and initializes setup array

  HMdcDetector* mdcDet=(HMdcDetector*)(((HSpectrometer*)(gHades->getSetup()))->getDetector("Mdc"));
  if (!mdcDet)
    {
      errorAndExitGetMdcSetup();
    }
  for(Int_t sector=0; sector<6; sector++)
    {
      for(Int_t module=0; module<4; module++)
	{
	  setup[sector][module]=(mdcDet->getModule(sector, module)) ? 1:0;
	}
    }
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
          errorInitCategoryCannotGetCategory(catname);
	  return NULL;
	}

      if (!((HEvent*)(gHades->getCurrentEvent()))->addCategory(cat,category,detector.Data()))
	{
          errorInitCategoryCannotAddCategory(catname,detector);
	  return NULL;
	}
    }
  return category;
}

// ----------------------------------------------------------------------------------------------------

Int_t HMdcUnpacker::initExcludeBlackList(void)
{
  // initializes list of black listed mask and adress matches to be exclude from data stream

  // beamtime sep2005
  // number of sets: 1
  //   #0: missing mboNr in sector IV, module 4

  // beamtime apr2006
  // number of sets: 1
  //   #0: missing mboNr in sector IV, module 4

  if (
      !excludeBlackListContext.CompareTo("sep05") ||
      !excludeBlackListContext.CompareTo("apr06") ||
      !excludeBlackListContext.CompareTo("may06")
     )
    {
	excludeBlackListSize=1;
	if (excludeBlackListSize)
	{
	    excludeBlackListMask      = new UInt_t[excludeBlackListSize];
	    excludeBlackListAdress    = new UInt_t[excludeBlackListSize];
	    excludedBlackListCounters = new Int_t[excludeBlackListSize];
            // list of masks
	    excludeBlackListMask  [0]=0xffe00000;
            // list of adresses
	    excludeBlackListAdress[0]=0x2cc00000;
	    for (Int_t arrayIndex=0; arrayIndex<excludeBlackListSize; arrayIndex++)
	    {
		excludedBlackListCounters[arrayIndex]=0;
	    }
	}
    }
    else if (!excludeBlackListContext.CompareTo(excludeBlackListContextDefaultString))
    { errorAndExitInitExcludeBlackListNoValidContext(); }
    else
    {
      warningInitExcludeBlackListNoValidContext(excludeBlackListContext);
      disableExcludeBlackList();
    }
    return EXIT_SUCCESS;
}

// ----------------------------------------------------------------------------------------------------

Int_t HMdcUnpacker::initRecoverBlackList(void)
{
  // initializes list of black listed mask and adress matches which can be recovered

  // beamtime sep2005
  // number of sets: 1
  //   #0: missing mboNr in sector IV, module 4

  // beamtime apr2006
  // number of sets: 1
  //   #0: missing mboNr in sector IV, module 4

    if (
	!recoverBlackListContext.CompareTo("sep05") ||
	!recoverBlackListContext.CompareTo("apr06") ||
	!recoverBlackListContext.CompareTo("may06")
       )
    {
	recoverBlackListSize=1;
	if (recoverBlackListSize)
	{
	    recoverBlackListMask      = new UInt_t[recoverBlackListSize];
	    recoverBlackListAdress    = new UInt_t[recoverBlackListSize];
	    recoveredBlackListCounters = new Int_t[recoverBlackListSize];
	    if (! excludeBlackListMask )
	      {
		// list of masks
		recoverBlackListMask  [0]=0xffe00000;
		// list of adresses
		recoverBlackListAdress[0]=0x2cc00000;
	      }
	    else
	      {
		// list of masks
		recoverBlackListMask  [0]=excludeBlackListMask  [0];
		// list of adresses
		recoverBlackListAdress[0]=excludeBlackListAdress[0];
	      }
	    for (Int_t arrayIndex=0; arrayIndex<recoverBlackListSize; arrayIndex++)
	    {
		recoveredBlackListCounters[arrayIndex]=0;
	    }
	}
    }
    else if (!recoverBlackListContext.CompareTo(recoverBlackListContextDefaultString))
      { errorAndExitInitRecoverBlackListNoValidContext(); }
    else
      {
	warningInitRecoverBlackListNoValidContext(recoverBlackListContext);
	disableRecoverBlackList();
      }
    return EXIT_SUCCESS;
}

// ----------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
// reinit

Bool_t HMdcUnpacker::reinit()
{
  // reinit function
  // if versionSetManual not set it determines the decodeVersion via determineDecodeVersion()

  if (!versionSetManual)
    {
      determineDecodeVersionCtr=0;
    }

  if (recoverBlackList) { reinitBlackListRecovery(); }

  if (unpackerDebug) print("reinit()");
  return kTRUE;
}

// ----------------------------------------------------------------------------------------------------
// reinit helpers

void HMdcUnpacker::reinitBlackListRecovery()
{
    createBlackListRecoveryTools=0;
    if (!recoverBlackListContext.CompareTo("sep05"))
    {
	if (subEvtId==222) // chamber IV-4
	{
	    createBlackListRecoveryTools=0x092005;
	}
    }
    if (!recoverBlackListContext.CompareTo("apr06"))
    {
	if (subEvtId==222) // chamber IV-4
	{
	    createBlackListRecoveryTools=0x042006;
	}
    }
    if (!recoverBlackListContext.CompareTo("may06"))
    {
	if (subEvtId==222) // chamber IV-4
	{
	    createBlackListRecoveryTools=0x042006;
	}
    }
}

// ----------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
// execute

Int_t HMdcUnpacker::execute()
{
  // execute funtion of the task
  // calls the fill() function (DETAILED) information here
  // before it checks whether the called subEvtId corresponds
  // to an active module in the mdc detector setup
  //
  // return values: always 1 (philosophy: a wrong sub event should not affect the other sub events)

  // checks
  // -- valid pointer to SubEvt
  if (!pSubEvt) return 1;

  // -- valid pointer to lookup-table
  if (!pReadout)
    {
      if(!noComment) errorExecuteNoValidPointerTo("Lookup-Table");
      return 1;
    }

  // -- valid SAM number
  Int_t samNr  = getSubEvtIdToSamNr(subEvtId,kTRUE);
  if (samNr < 0)
    {
      if(!noComment) warningExecuteSamNrNegative(samNr);
      return 1;
    }

  // -- valid sector entry in lookuptable for samNr
  Int_t sector = pReadout->getSamNumberToSector(samNr);
  if (sector < 0)
    {
      if(!noComment) warningExecuteNoValidSectorEntryInLookupTableForSamNr(samNr);
      return 1;
    }

  // -- valid sector entry in lookuptable for samNr
  Int_t module = pReadout->getSamNumberToModule(samNr);
  if (module < 0)
    {
      if(!noComment) warningExecuteNoValidModuleEntryInLookupTableForSamNr(samNr);
      return 1;
    }

  // if sector and module are in testMdcSetup
  // --> fill data
  if (testMdcSetup(sector, module))
    {
      if (0 > fill())
	{
	  if(!noComment) errorExecuteErrorInFill();
	  return 1;
	}
    }
  return 1;
}

// ----------------------------------------------------------------------------------------------------

Bool_t HMdcUnpacker::testMdcSetup(Int_t sector, Int_t module)
{
  // Tests the Mdc setup if the module are present
  // in the running analysis at position sector, module

  return (setup[sector][module]==1);
}

// ----------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
// execute helpers

Int_t HMdcUnpacker::getSubEvtIdToSamNr(Int_t localSubEvtId, Bool_t expectUndefined)
{
  // determines the samNr from the sub event id localSubEvtId
  // in addition it crosschecks the samNr with the DAQ specifications
  // and does boundary checks by default.
  // You can switch it off by setDisableSamNrConsistencyCheck()

  Int_t samNr;
  // determine samNr

  switch(decodeVersion)
    {
    case 0:
      samNr = 2*(localSubEvtId - 201); // version before Nov02
      break;
    case 1:
      samNr = localSubEvtId - 201; // version since Nov02
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
      if (!(200 < localSubEvtId && localSubEvtId < 300))
	{
	  if(!noComment) errorGetSubEvtIdToSamNrSubEvtIdOutOfMdcRange(localSubEvtId, "]200,300[");
	  return -1;
	}
      break;
    case 1:
      if (!(200 <= localSubEvtId && localSubEvtId < 300))
	{
	  if(!noComment) errorGetSubEvtIdToSamNrSubEvtIdOutOfMdcRange(localSubEvtId, "[200,300[");
	  return -1;
	}
      break;
    default:
      samNr = -1;
    }

  Int_t rocSector = pReadout->getSamNumberToSector(samNr);

  if (rocSector==-1 && !expectUndefined)
    {
      if(!noComment) errorGetSubEvtIdToSamNrSamNrNotInLookupTable(localSubEvtId,samNr);
      return -1;
    }
  return samNr;
}

// ----------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
// filling

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
  //     1.a. if enabled check for recoverable blacklisted adresses and store them
  //     1.b. if enabled check for further blacklisted adresses and ignore them  
  //     2.   call of decode for each dataword
  //     3.   the hardware address is matched to the sector/module/mbo/tdc coordinates of HMdcRaw
  //          via the lookup table HMdcEvReadout
  //     4.   a new/existing slot at the corresponding address is requested
  //     5.   the time information is filled to this slot
  //
  // --  if enabled recover recoverable datawords after the data loop
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
  //
  // --------------------------------------------------------------------------
  // DETAILS:
  // The single steps of this function are:
  //
  // 1. check for existing pointers to subEvt and pReadout 
  //    - if not 
  //      -> "goto" end of function returning 1
  //
  // 2. if the decode version is not set manually (via setDecodeVersion())
  //    -> call determineDecodeVersion()
  //
  // 3. get subEvt's pointers to data and its end
  //
  // 4. check for positive difference (data-end > 0)
  //    - if not 
  //      -> leave function with return code -1
  //
  // 5. if decodingMode() returns kFALSE (DETAILS see decodingMode())
  //      -> leave function with return code -1
  //
  // 6. if "recoverBlackList" is enabled
  //      -> call prepareBlackListRecovery()
  // 
  // 7. sub sub event while loop
  //
  // 7.1. determine end of subsub event loop from subsub event header
  // 
  // 7.2. subsub event dataword loop
  // 
  // 7.2.1. check whether subsub event endpointer exceeds sub event pointer
  //        - yes: 
  //          -> skipping rest of sub event returning -1 
  //            (incl. a possible correction of recoverable datawords)
  //          -> exception: "ontinueAfterInconsistency" is enabled
  //             -> continue with next dataword at 7.2   
  //
  // 7.2.2. if "recoverBlackList" is enabled
  //        -> check if dataword matches the list criteria
  //           - yes:
  //             -> call storeForRecoveryFromBlackList()
  //                and continue with next dataword at 7.2
  //
  // 7.2.3. if "recoverBlackList" is enabled
  //        -> check if dataword matches the list criteria
  //           - yes:
  //             -> continue with next dataword at 7.2
  //           
  // 7.2.4. dataword decoding
  // 
  //        * behaviour during the dataword decoding:
  //   	      - if the consistency check is switched 
  //            - on 
  //              -> decode returns 
  //   	             - kFALSE if any of the border checks failed
  //   	             - kTRUE else
  //   	        - off
  //              -> decode returns kTRUE
  //
  // 	      - in case of an inconsistency: (kFALSE returned from decode with checkInconsistency switched on)
  //    	-> the decoding of all following datawords of this sub event is skipped
  //      	   (including a possible correction of recoverable datawords) 
  //               the function returns -1
  //    	-> exception: "continueDecodingAfterInconsistency" is enabled 
  //         	   - then, ignoring this maybe hazardous error (!!!),
  //                 decoding is continued with the next dataword at 7.2 
  //    
  // 	      - in case of errors and "noComment" disabled
  //    	-> error messages are issued
  //       	and if also unpackerDebug is enabled
  //       	-> the dataword is dumped to the screen
  //    
  //  7.2.5. fill data to Raw categories calling fillData()
  //         in case of failures of fillData():
  //         - adress, setup or matching table errors
  //           -> continue with next dataword at 7.2
  //         - "cannot get Slot" (severe problems)
  //           -> leave function returning -1 and skipping rest of sub event
  //              (incl. possible recovery of datawords)
  //
  //  7.2.6. END OF subsub event dataword loop
  //
  //  7.3. increase data pointer by subsub event size
  //  
  //  7.4. END OF sub event while loop  
  //
  //  8. if recoverBlackList is enabled
  //     -> call of executeBlackListRecovery()  
  //        - same return code behaviour as in 7.2.5 (fillData())
  //    
  //  9. leave function always returning 1 
  //    

  // if pointer to sub event and lookup table exist ...
  if (pSubEvt && pReadout)
    {
      // if the decode version is not set by user, determine decode version
      if (!versionSetManual) determineDecodeVersion();

      UInt_t* data = pSubEvt->getData();
      UInt_t* end  = pSubEvt->getEnd();

      UInt_t subEvtSize = 0;
      UInt_t offset     = 0;
      UInt_t *enddata   = 0;
      Int_t diff = (end-data);
      if (diff<0)
	{
          // TODO errorFillNegativeSizeOfSubEvent()
	  return -1;
	}
      UInt_t eventSize  = ((diff))+1;

      // ---------- determine decoding mode --- affects rocEvtHeaderSize

      if (!decodingMode()) return -1;  // TODO: missing comment
      rocEvtHeaderSize = getRocEvtHeaderSize();

      // recovery preparations for black list recovery

      if (recoverBlackList) { prepareBlackListRecovery(eventSize); }

      // data loop
      while( data < end )
	{
	  dataword->clearAll();

	  //decoding sub-header (filling sub header information to dataword)

	  if (rocEvtHeaderSize > 0) // data stream contains ROC headers
	    {
	      if (!decodeSubHeader(data,subEvtSize))
		{
		  if(!noComment) errorFillDecodeSubHeaderFailed();
		  return -1;
		}
	      offset  = subEvtSize + rocEvtHeaderSize;
	      enddata = data+offset;
	    }
	  else // data stream does not contain ROC headers
	    {
	      enddata = end;
	    }

	  //decoding data words

	  for(UInt_t* dataPointer=data+rocEvtHeaderSize; dataPointer<enddata; dataPointer++)
	    {
	      dataword->clear();
	      dataword->clearHeader();

	      if (dataPointer > end)
		{
		  if(!noComment) errorFillDataPointerExceedingRange(dataPointer,end,enddata);
		  if (continueDecodingAfterInconsistency) continue;
		  if(!noComment) errorFillSkippingRestOfSubEvent();
		  return -1;
		}

	      // Is dataword included in an recovery black list?
	      // TODO: recovery black list from oracle (->init section)
	      // first masking then comparing to adress
	      // if it matches this dataword is stored
              // until the end of the while loop and then processes

	      if (recoverBlackList)
		{
		  if (storeForRecoveryFromBlackList(dataPointer)) continue;
		}

	      // Is dataword included in an excluding black list
              // ... and not yet skipped by recovery?
	      // TODO: black list from oracle
	      // first masking then comparing to adress
	      // if it matches this dataword is ignored

	      if (excludeBlackList)
		{
		  if (excludeBlackListData(dataPointer)) continue;
		}

	      // >>>> here the decoding of the data word is done
              //
	      // behaviour:
	      //    - if the consistency check is switched on then decode returns 
	      //       - kFALSE if any of the border checks failed
	      //       - kTRUE else
	      //    - kTRUE
	      //
	      // in case of an inconsistency: (kFALSE returned from decode with checkInconsistency switched on)
	      //    - the decoding of all following datawords of this sub event is skipped
	      //      (including a possible correction of recoverable datawords) the function returns -1
	      //    - except "continueDecodingAfterInconsistency" is enabled, 
	      //         then ignoring this maybe hazardous error (!!!) decoding is continued with the next dataword
	      //    
	      // in case of errors and "noComment" disabled
	      //    -> error messages are issued
	      //       and if also unpackerDebug is enabled
	      //       -> the dataword is dumped to the screen
	      
	      if(!dataword->decode(*dataPointer, decodeVersion,consistencyCheck))
		{
		  if(!noComment && unpackerDebug) dataword->dump(decodeVersion);
		  if (continueDecodingAfterInconsistency) continue;
		  if(!noComment) errorFillSkippingRestOfSubEvent();
		  return -1;
		}

	      // fill data to HMdcRaw categories including all checks
	      switch(fillData())
		{
		case  0:     break;
		case  1:  continue; // case of failing checks on Setup and Boundaries
		case -2: return -2; // cannot get slot for HMdcRaw
		default: errorAndExitFillUnknownReturnValueOfFillData();
		}
	    }
	  data += offset;

	}
      // recovery of corrupted data
      if(recoverBlackList)
      {
	  switch(executeBlackListRecovery())
	  {
	  case  0:     break;
	  case -2: return -2; // cannot get slot for HMdcRaw
	  default: errorAndExitFillUnknownReturnValueOfFillData();
	  }
	}
    }
  return 1;
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
// filling helpers

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
	      errorAndExitDetermineDecodeVersion();
	      break;
	    }
	  if (unpackerDebug) {infoDetermineDecodeVersion();}
	}
    }

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
      Int_t ctime = atoi((myexec(command)).Data());

      UInt_t date = header->getDate();
      UInt_t time = header->getTime();

      UInt_t year = (date & 0xFF0000) >> 16;
      year = (year < 94)? year + 2000 : year + 1900;
      UInt_t month = ((date & 0xFF00) >> 8) + 1;
      UInt_t day = date & 0xFF;

      UInt_t hour = (time & 0xFF0000) >> 16;
      UInt_t min = (time & 0xFF00) >> 8;
      UInt_t sec = (time & 0xFF);

      if (unpackerDebug) Info("compareEventDateTimeTo()"," SubEvtId %i - time-stamp: %04i-%02i-%02i %02i:%02i:%02i",
			      subEvtId,year,month,day,hour,min,sec);

      sprintf(command,"date --date=\"%04i-%02i-%02i %02i:%02i:%02i\" +%%s",year,month,day,hour,min,sec);
      Int_t evttime = atoi((myexec(command)).Data());

      if (evttime == ctime) return  0;
      if (evttime >= ctime) return  1;
      if (evttime <= ctime) return -1;
    }
  else
    {
      errorAndExitCompareEventDateTimeTo();
    }
  return -99;
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
	      fillMdcRawEventHeader(errorBits, rocModule, rocNumber, rocSector, 0-size, id);
	    }
	}
      return kTRUE;
      break;

    default:
      if(!noComment) errorDecodingModeInvalidCompressMode(compressMode,decoding);
      break;
    }
  return kFALSE;
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
      if(!noComment) errorDecodeSubHeaderCannotDecode();
      return kFALSE;
    }

  rocNumber    = dataword->getRocNb();
  errorFlag    = dataword->getErrorFlag();
  trigType     = dataword->getTrigType();
  subEvtSize   = dataword->getSubEvtSize();

  // fill raw event header container
  if (errorFlag!=0 || debug)
    {
      fillMdcRawEventHeader(errorFlag, rocModule, rocNumber, rocSector,  subEvtSize, trigType);
    }

  // if data was taken in "standalone" mode,
  // i.e. Trigger Tag is not contained in EventHeader
  // Then, in case of a calibration trigger,
  // the event Header Id is replaced by the trigger Type of the roc Header.

  if (standalone)
    {
      static const Int_t kMdc_Standalone_CalibrationEvent=9;
      if (trigType == kMdc_Standalone_CalibrationEvent)
	{
	  ((HEventHeader*)((HEvent*)(gHades->getCurrentEvent()))->getHeader())->setId(kMdc_Standalone_CalibrationEvent);
	}
    }
  return kTRUE;
}

// ----------------------------------------------------------------------------------------------------

Int_t HMdcUnpacker::fillMdcRawEventHeader(Int_t errorFlag, Int_t rocModule, Int_t rocNumber,
					  Int_t rocSector, Int_t subEvtSize,
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
      if(!noComment) errorFillMdcRawEventHeaderCannotGetSlot();
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

void HMdcUnpacker::prepareBlackListRecovery(UInt_t eventSize)
{
    if (0!=createBlackListRecoveryTools)
    {
	// clear previous pointers and content and counters
	recoveryStorageCounter=0;
	if (recoveryStorage) delete[] recoveryStorage;
	recoveryStorage = NULL;
	// create temporary array for \"difficult\" data
	recoveryStorage = new UInt_t[eventSize];
	// clear array
	recoveryStorageCounter=0;
	memset(recoveryStorage,0,eventSize*sizeof(UInt_t));
    }
}

// --------------------------------------------------------------------------------

Int_t HMdcUnpacker::executeBlackListRecovery()
{
    Int_t returnValue=0;
    if (0!=createBlackListRecoveryTools)
    {
	switch(createBlackListRecoveryTools)
	{
	case 0x092005:
	    returnValue = recoverBlackListSep2005();
	    break;
	case 0x042006:
	    returnValue = recoverBlackListApr2006();
	    break;
	default:
	    returnValue=0;
	    errorFillUnknownCodeForBlacklistRecovery();
	    break;
	}
	// finally clear storage
	if (recoveryStorage) delete[] recoveryStorage;
	recoveryStorage=NULL;
    }
    return returnValue;
}

// --------------------------------------------------------------------------------

Bool_t HMdcUnpacker::storeForRecoveryFromBlackList(UInt_t *data)
{
  // Is dataword include in an recovery black list?
  // TODO: recovery black list from oracle
  // first masking then comparing to adress
  // if it matches this dataword is stored
  // until the end of the while loop and then processes
  //
  // returns kTRUE if data word matches
  //         kFALSE else

  static Bool_t skipDataWord;
  skipDataWord=kFALSE;
  for (Int_t arrayIndex=0; arrayIndex< recoverBlackListSize; arrayIndex++)
    {
      // check if dataword deb masked by mask matches the remaining adress
      if ((*data & recoverBlackListMask[arrayIndex])==recoverBlackListAdress[arrayIndex])
	{
	  // for later recovery store all datawords
	  recoveredBlackListCounters[arrayIndex]++;
	  recoveryStorage[recoveryStorageCounter]=*data;
	  recoveryStorageCounter++;
	  skipDataWord=kTRUE;
	  break;
	}
    }
  if (skipDataWord) return kTRUE;
  else return kFALSE;
}

// --------------------------------------------------------------------------------

Bool_t HMdcUnpacker::excludeBlackListData(UInt_t* data)
{
  // Is dataword included in an excluding black list?
  // TODO: black list from oracle
  // first masking then comparing to adress
  // if it matches this dataword is ignored
  //
  // returns kTRUE if data word matches
  //         kFALSE else

  static Bool_t skipDataWord;
  skipDataWord=kFALSE;
  for (Int_t arrayIndex=0; arrayIndex< excludeBlackListSize; arrayIndex++)
    {
      // check if dataword deb masked by mask matches the remaining adress
      if ((*data & excludeBlackListMask[arrayIndex])==excludeBlackListAdress[arrayIndex])
	{
	  excludedBlackListCounters[arrayIndex]++;
	  if (unpackerDebug && !noComment)
	    {
	      warningExcludeDataWords(*data,
				      excludeBlackListMask[arrayIndex],
				      excludeBlackListAdress[arrayIndex]);
	    }
	  skipDataWord=kTRUE;
	  break;
	}
    }
  if (skipDataWord) { return kTRUE;}
  else return kFALSE;
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
// fillData

Int_t HMdcUnpacker::fillData(void)
{
  // Converts the content of the already decoded dataword via the lookup table HMdcEvReadout
  // to sector/module/mbo/tdc coordinates.
  // Then, if available the information is stored in a new or added to an existing
  // data slot of HMdcRaw
  // If activated, the raw data is written by fillMdcDataWord()
  // (formerly additional debug had to be switched on, not necessary any more)

  // variables used inside loop
  static Int_t sector;
  static Int_t module;
  static Int_t mbo   ;
  static Int_t tdc   ;

  // First the hardware address contained dataword
  // is matched to the sector/module/mbo/tdc coordinates of HMdcRaw
  // via the lookup table HMdcEvReadout
  // reset to default values
  sector = module = mbo = tdc = -1;

  if (!matchHardwareAdressToSecModMboTdc(sector, module, mbo, tdc))
    {
      return 1; // in case of failing checks on Setup and Boundaries
    }

  dataword->setAddress(sector,module,mbo,tdc,0);

  // first get a free or existing slot

  static HMdcRaw *pMdcRaw;
  pMdcRaw = NULL;
  pMdcRaw = getFreeOrExistingSlot(sector, module, mbo, tdc);
  if (!pMdcRaw) return -2; // cannot get slot

  // fill the time to this slot

  if (!pMdcRaw->setTime(dataword->getTime(),tdcMode, noComment))
    {
      if(!noComment) errorFillErrorFillingDataWord();
      if (unpackerDebug) dataword->dump(decodeVersion);
    }

  // fill dataWord to category HMdcDataWord

  if(fillDataWord) fillMdcDataWord(sector,module,mbo,tdc);

  return 0;
}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
// fillData helpers

Bool_t HMdcUnpacker::matchHardwareAdressToSecModMboTdc(Int_t &sector, Int_t &module,
						       Int_t &mbo, Int_t &tdc)
{
    // the hardware address is matched to the sector/module/mbo/tdc coordinates of HMdcRaw
    // via the lookup table HMdcEvReadout
    // the result is checked for consistency
    // deb is the pointer to the current rwa dataword
    //
    // kFALSE is returned in case of failures
    // otherwise kTRUE

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
	if (!noComment) errorFillNoValidAddressInLookUpTable();
	if (unpackerDebug) dataword->dump(decodeVersion);
	return kFALSE;
    }

    HMdcEvReadoutBus& bus =(*pReadout)[nsam][nroc][nrocP][nbus];

    sector= bus.getSec();
    module= bus.getMod();
    mbo   = bus.getMbo();
    tdc   = getTdc(dataword->getTdc(),dataword->getChan());

    if(!checkMdcSetup(sector,module)) { return kFALSE; }
    if(!checkMboRawStructBounds(sector,module,mbo,tdc)) { return kFALSE; }

    return kTRUE;
}

// ----------------------------------------------------------------------------------------------------

Bool_t HMdcUnpacker::checkMdcSetup(Int_t sector, Int_t module)
{
  // checks whether module and sector are set in the MdcSetup
  // returns kTRUE if set,
  // kFALSE else

  if(!testMdcSetup(sector,module))
    {
      if(!noComment)
	{
          errorCheckMdcSetup(sector,module);
	  if (unpackerDebug) dataword->dump(decodeVersion);
	}
      return kFALSE;
    }
  return kTRUE;
}

// ----------------------------------------------------------------------------------------------------

Bool_t HMdcUnpacker::checkMboRawStructBounds(Int_t sector,Int_t module, Int_t mbo, Int_t tdc)
{
  // checks consistency whether sector, module, mbo, tdc values are inside
  // the bounds of the parameter container MboRawStruct
  // return value
  //              kTRUE the checks were passed
  //              kFALSE if otherwise

  Int_t nMaxSector = rawc->getSize() ;
  if (sector >= nMaxSector || sector < 0 )
    {
      if (!noComment)
	{
	  errorCheckMboRawStructBoundsOutOfBounds(tdc, nMaxSector);
	  if (unpackerDebug) dataword->dump(decodeVersion);
	}
      return kFALSE;
    }
  Int_t nMaxModule = (*rawc)[sector].getSize() ;
  if (module >= nMaxModule || module < 0 )
    {
      if (!noComment)
	{
          errorCheckMboRawStructBoundsOutOfBounds(tdc, nMaxModule, sector);
	  if (unpackerDebug) dataword->dump(decodeVersion);
	}
      return kFALSE;
    }
  Int_t nMaxMbo = (*rawc)[sector][module].getSize() ;
  if (mbo >= nMaxMbo || mbo < 0 )
    {
      if (!noComment)
	{
          errorCheckMboRawStructBoundsOutOfBounds(tdc, nMaxMbo, sector, module);
	  if (unpackerDebug) dataword->dump(decodeVersion);
	}
      return kFALSE;
    }
  Int_t nMaxTdc = (*rawc)[sector][module][mbo].getNTdcs() ;
  if (tdc >= nMaxTdc || tdc < 0 )
    {
      if (!noComment)
	{
          errorCheckMboRawStructBoundsOutOfBounds(tdc, nMaxTdc, sector, module, mbo);
	  if (unpackerDebug) dataword->dump(decodeVersion);
	}
      return kFALSE;
    }

  return kTRUE;
}

// --------------------------------------------------------------------------------

HMdcRaw* HMdcUnpacker::getFreeOrExistingSlot(Int_t &sector, Int_t &module, Int_t &mbo, Int_t &tdc)
{
    // get a free or existing slot from the HMdcRaw category
    // at the location of HLocation loc
    // returns the pointer to the (new) HMdcRaw object
    // NULL if it couldn't be done

    HLocation loc;
    loc.set(4, 0, 0, 0, 0);

    // filling the data to the location
    // set location indexes

    loc[0] = sector;
    loc[1] = module;
    loc[2] = mbo;
    loc[3] = tdc;

    HMdcRaw *pMdcRaw = (HMdcRaw*) pRawCat->getObject(loc);
    if (!pMdcRaw)
    {
	pMdcRaw = (HMdcRaw*) pRawCat->getSlot(loc);
	if (pMdcRaw)
	{
	    pMdcRaw = new (pMdcRaw) HMdcRaw();
	    //setAddress to sector,module,mbo and tdc
	    pMdcRaw->setAddress(sector,module,mbo,tdc);
	}
	else
	{
	    if(!noComment) errorFillCannotGetSlot(sector,module,mbo,tdc);
	    if (unpackerDebug) pSubEvt->dumpIt();
	}
    }
    return pMdcRaw;
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
	  if(!noComment) errorFillMdcDataWordTooManyEntriesToSameLocation(maxentry);
	}
      else
	{
	  if(!noComment) errorFillMdcDataWordCannotGetSlot();
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
	  if(!noComment) errorFillMdcDataWordCannotGetSlot();
	  return -2;
	}
    }
  return 1;
}

// ----------------------------------------------------------------------------------------------------

Int_t HMdcUnpacker::recoverBlackListApr2006()
{
    // Hardware bugfix for beamtime apr2006:
    // In Module IV (Orsay) of Sector IV (SubEvtId 222)
    // The mob id of the two motherboards in the LVL1 chain
    // of mbo 113 and mbo 115 is corrupted.
    //
    // Up to now the Blacklist option ignored all datawords of this if activated
    // Now it the channels should if possible be reconstructed:
    //
    // Because mbo 113 is a 64-channel board and mbo 115 is a 96 Long_t     // the topmost 32 channels can automatically be assigned to mbo 115
    //
    // Since this is the same error as in Sep2005
    // recoverBlackListSep2005 is called

    return recoverBlackListSep2005();
}

// ----------------------------------------------------------------------------------------------------

Int_t HMdcUnpacker::recoverBlackListSep2005()
{
  // Hardware bugfix for beamtime sep05:
  // In Module IV (Orsay) of Sector IV (SubEvtId 222)
  // The mob id of the two motherboards in the LVL1 chain
  // of mbo 113 and mbo 115 is corrupted.
  //
  // Up to now the Blacklist option ignored all datawords of this if activated
  // Now it the channels should if possible be reconstructed:
  //
  // Because mbo 113 is a 64-channel board and mbo 115 is a 96 Long_t   // the topmost 32 channels can automatically be assigned to mbo 115

  // for later recovery store all datawords

  UInt_t datum;
  Int_t arrayIndex;
  Int_t hitCounter[64];
  Int_t hitCounterRecovered[64];
  memset(hitCounter         ,0,64*sizeof(Int_t));
  memset(hitCounterRecovered,0,64*sizeof(Int_t));
  Int_t tdc;

  for (UInt_t index=0; index< recoveryStorageCounter; index++)
    {
      datum = recoveryStorage[index];
      // (a) matches mask #0
      //     check if dataword masked by mask #0 matches the remaining adress #0
      arrayIndex=0;
      if ((datum & recoverBlackListMask[arrayIndex])==recoverBlackListAdress[arrayIndex])
	{
	  // decode w/o consistency check
	  if(!dataword->decode( datum, decodeVersion))
	    {
	      if(!noComment){ warningRecoverBlackListSep2005SkippingDataWord(datum); }
	      continue;
	    }
	  // get tdc
	  // reset to default values
	  tdc = -1;
	  tdc = getTdc(dataword->getTdc(),dataword->getChan());
          if (tdc < 0) continue;
	  // if tdc > 63 assign values to Long_t motherboard (LVL1 Position = 2) , i.e. modify dataword
	  if (tdc > 63)
	    {
	      // set LVL1 Position to 2;
	      dataword->setMboNb(2);
	      // redecode
	      if (!dataword->decode(dataword->getCodedDataWord(), decodeVersion, consistencyCheck)) {continue;}
	      // fill data to HMdcRaw categories including all checks
	      switch(fillData())
		{
		case  0:     break;
		case  1:  continue; // case of failing checks on Setup and Boundaries
		case -2: return -2; // cannot get slot for HMdcRaw
                default: errorAndExitRecoverBlackListSep2005UnknownReturnValueOfFillData();
		}
	      recoveredBlackListCounters[arrayIndex]++;
	    }
	  else
	    {
	      // if tdc < 63 count hits
	      hitCounter[tdc]++;
	    }
	}
    }
  for (UInt_t index=0; index< recoveryStorageCounter; index++)
    {
      datum = recoveryStorage[index];
      // (b) matches mask #0
      //     check if dataword masked by mask #0 matches the remaining adress #0
      arrayIndex=0;
      if ((datum & recoverBlackListMask[arrayIndex])==recoverBlackListAdress[arrayIndex])
	{
	  // decode w/o consistency check
	  if(!dataword->decode( datum, decodeVersion)) { continue; }

	  // get tdc
	  // reset to default values
	  tdc = -1;
	  tdc = getTdc(dataword->getTdc(),dataword->getChan());
	  if (tdc < 0) continue;
	  if (tdc < 64)
	    {
	      // 2,6 -> Fill to both
	      if (hitCounter[tdc] == 2 || hitCounter[tdc] == 6)
		{
		  // set LVL1 Position to 1;
		  dataword->setMboNb(1);
		  // redecode
		  if (!dataword->decode(dataword->getCodedDataWord(), decodeVersion, consistencyCheck)) {continue;}
		  // fill data to HMdcRaw categories including all checks
		  switch(fillData())
		    {
		    case  0:     break;
		    case  1:  continue; // case of failing checks on Setup and Boundaries
		    case -2: return -2; // cannot get slot for HMdcRaw
		    default: errorAndExitRecoverBlackListSep2005UnknownReturnValueOfFillData();
		    }

		  // set LVL1 Position to 2;
		  dataword->setMboNb(2);
		  // redecode
		  if (!dataword->decode(dataword->getCodedDataWord(), decodeVersion, consistencyCheck)) {continue;}
		  // fill data to HMdcRaw categories including all checks
		  switch(fillData())
		    {
		    case  0:     break;
		    case  1:  continue; // case of failing checks on Setup and Boundaries
		    case -2: return -2; // cannot get slot for HMdcRaw
		    default: errorAndExitRecoverBlackListSep2005UnknownReturnValueOfFillData();
		    }
		  hitCounterRecovered[tdc]++;
		  recoveredBlackListCounters[arrayIndex]++;
		}

	      // 4 Hits fill first 2 hits to mbo 1 and last 2 hits to mbo 2
	      else if (hitCounter[tdc] == 4)
		{
		  if (hitCounterRecovered[tdc] < 2)
		    { dataword->setMboNb(1); }
		  else
		    { dataword->setMboNb(2); }
		  // redecode
		  if (!dataword->decode(dataword->getCodedDataWord(), decodeVersion, consistencyCheck)) {continue;}
		  // fill data to HMdcRaw categories including all checks
		  switch(fillData())
		    {
		    case  0:     break;
		    case  1:  continue; // case of failing checks on Setup and Boundaries
		    case -2: return -2; // cannot get slot for HMdcRaw
		    default: errorAndExitRecoverBlackListSep2005UnknownReturnValueOfFillData();
		    }
		  hitCounterRecovered[tdc]++;
		  recoveredBlackListCounters[arrayIndex]++;
		}
	      // 12 Hits fill first 6 hits to mbo 1 and last 6 hits to mbo 2
	      else if (hitCounter[tdc] == 12)
		{
		  if (hitCounterRecovered[tdc] < 6)
		    { dataword->setMboNb(1); }
		  else
		    { dataword->setMboNb(2); }
		  // redecode
		  if (!dataword->decode(dataword->getCodedDataWord(), decodeVersion, consistencyCheck)) {continue;}
		  // fill data to HMdcRaw categories including all checks
		  switch(fillData())
		    {
		    case  0:     break;
		    case  1:  continue; // case of failing checks on Setup and Boundaries
		    case -2: return -2; // cannot get slot for HMdcRaw
		    default: errorAndExitRecoverBlackListSep2005UnknownReturnValueOfFillData();
		    }
		  hitCounterRecovered[tdc]++;
		  recoveredBlackListCounters[arrayIndex]++;
		}
	      else
		{
		  if(!noComment){ warningRecoverBlackListSep2005IgnoringDatum(hitCounter[tdc],datum); }
		}
	    }
	}
    }

  return 0;
}
// -------------------------------------------------------------------------------

void HMdcUnpacker::enableExcludeBlackList(TString context)
{
  setExcludeBlackListInternal(kTRUE);
  setExcludeBlackListContext(context);
}

// -------------------------------------------------------------------------------

void HMdcUnpacker::disableExcludeBlackList()
{
  setExcludeBlackListInternal(kFALSE);
  setExcludeBlackListContext(HMdcUnpacker::excludeBlackListContextDefaultString);
}

// -------------------------------------------------------------------------------

void HMdcUnpacker::setExcludeBlackList(Bool_t b)
{
  if(b) { errorAndExitSetExcludeBlackListObsoleteCall(); }
  else  { disableExcludeBlackList(); }
}

// -------------------------------------------------------------------------------

void HMdcUnpacker::setExcludeBlackListContext(TString context)
{
  context.ToLower();
  excludeBlackListContext=context;
}

// -------------------------------------------------------------------------------

void HMdcUnpacker::setExcludeBlackList(TString context)
{
  // function set/unset exclude black list feature via HMdcUnpacker::setExcludeBlackList
  // and its context via HMdcUnpacker::setExcludeBlackListContext
  // in one line.
  //
  // if context is empty or equal to HMdcUnpacker::excludeBlackListContextDefaultString
  // the feature is deactivated.
  // Any other value activates this, provided a corresponding table exists
  // up to now "sep05" is the only valid context
  // (see HMdcUnpacker::init() function)

  setExcludeBlackListContext(context);
  if (!context.IsNull() && context.CompareTo(excludeBlackListContextDefaultString))
    { setExcludeBlackList(kTRUE); }
  else
    { setExcludeBlackList(kFALSE);}
}

// -------------------------------------------------------------------------------

void HMdcUnpacker::enableRecoverBlackList(TString context)
{
  setRecoverBlackListInternal(kTRUE);
  setRecoverBlackListContext(context);
}

// -------------------------------------------------------------------------------

void HMdcUnpacker::disableRecoverBlackList()
{
  setRecoverBlackListInternal(kFALSE);
  setRecoverBlackListContext(HMdcUnpacker::recoverBlackListContextDefaultString);
}

// -------------------------------------------------------------------------------

void HMdcUnpacker::setRecoverBlackList(Bool_t b)
{
  if(b) { errorAndExitSetRecoverBlackListObsoleteCall(); }
  else  { disableRecoverBlackList(); }
}

// -------------------------------------------------------------------------------

void HMdcUnpacker::setRecoverBlackListContext(TString context)
{
  context.ToLower();
  recoverBlackListContext=context;
}

// -------------------------------------------------------------------------------

void HMdcUnpacker::setRecoverBlackList(TString context)
{
  // function set/unset recover black list feature via HMdcUnpacker::setRecoverBlackList
  // and its context via HMdcUnpacker::setRecoverBlackListContext
  // in one line.
  //
  // if context is empty or equal to HMdcUnpacker::recoverBlackListContextDefaultString
  // the feature is deactivated.
  // Any other value activates this, provided a corresponding table exists
  // up to now "sep05" is the only valid context
  // (see HMdcUnpacker::init() function)

  setRecoverBlackListContext(context);
  if (!context.IsNull() && context.CompareTo(recoverBlackListContextDefaultString))
    { setRecoverBlackList(kTRUE); }
  else
    { setRecoverBlackList(kFALSE);}
}

// -------------------------------------------------------------------------------

Bool_t HMdcUnpacker::finalize()
{
  // finalize function
  // returning always kTRUE

  if (!noComment && excludeBlackList) printExcludedBlackListCounter();
  if (!noComment && recoverBlackList) printRecoveredBlackListCounter();
  return kTRUE;
}

// ----------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
// print functions

void HMdcUnpacker::printExcludedBlackListCounter()
{
  // prints out messages for each mask - adress pair how many excludes were encountered

  Bool_t print=kFALSE;
  for (Int_t arrayIndex=0; arrayIndex< excludeBlackListSize; arrayIndex++)
    {
      // check if any excludes were encountered
      if (excludedBlackListCounters[arrayIndex]>0)
	{
	  print=kTRUE;
	  break;
	}
    }
  if (print)
    {
      for (Int_t arrayIndex=0; arrayIndex< excludeBlackListSize; arrayIndex++)
	{
	  Warning("printExcludedBlackListCounter",
		  " subEvtId: %i - %i Data words were excluded by mask 0x%x and adress 0x%x",
		  subEvtId,
		  excludedBlackListCounters[arrayIndex],
		  excludeBlackListMask     [arrayIndex],
		  excludeBlackListAdress   [arrayIndex]
		  );
	}
    }
}
// ----------------------------------------------------------------------------------------------------

void HMdcUnpacker::printRecoveredBlackListCounter()
{
  // prints out messages for each mask - adress pair how many recovers were encountered

  Bool_t print=kFALSE;
  for (Int_t arrayIndex=0; arrayIndex< recoverBlackListSize; arrayIndex++)
    {
      // check if any recovers were encountered
      if (recoveredBlackListCounters[arrayIndex]>0)
	{
	  print=kTRUE;
	  break;
	}
    }
  if (print)
    {
      for (Int_t arrayIndex=0; arrayIndex< recoverBlackListSize; arrayIndex++)
	{
	  Info("printRecoverdBlackListCounter",
	       " subEvtId: %i - %i Data words were recovered by mask 0x%x and adress 0x%x",
	       subEvtId,
	       recoveredBlackListCounters[arrayIndex],
	       recoverBlackListMask     [arrayIndex],
	       recoverBlackListAdress   [arrayIndex]
	      );
	}
    }
}

// ----------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
// info functions


void HMdcUnpacker::infoDetermineDecodeVersion(void)
{
  Info("determineDecodeVersion()"," SubEvtId %i - decode version: %i ",
       subEvtId, getDecodeVersion());
}


// ----------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
// warnings functions

void HMdcUnpacker::warningExcludeDataWords(UInt_t data, UInt_t mask, UInt_t adress)
{
  Warning("fill()"," Event: %i -  SubEvtId %i - excluded data word %x due to matching mask 0x%x and adress 0x%x",
	  ((HEventHeader*)((HEvent*)(gHades->getCurrentEvent())->getHeader()))->getEventSeqNumber(),
	  subEvtId,
	  data,
	  mask,
	  adress);
}

// ----------------------------------------------------------------------------------------------------

void HMdcUnpacker::warningExecuteNoValidModuleEntryInLookupTableForSamNr(Int_t samNr)
{
  Warning("execute",
	  " Event: %i - SubEvtId %i - no valid module entry in lookuptable for virtual samNr: %i (=lookup table index+1) ... sub event id %i ignored",
	  ((HEventHeader*)((HEvent*)(gHades->getCurrentEvent())->getHeader()))->getEventSeqNumber(),
	  subEvtId,samNr,subEvtId);
}

// ----------------------------------------------------------------------------------------------------

void HMdcUnpacker::warningExecuteNoValidSectorEntryInLookupTableForSamNr(Int_t samNr)
{
  Warning("execute",
	  " Event: %i - SubEvtId %i - no valid sector entry in lookuptable for virtual samNr: %i (=lookup table index+1) ... sub event id %i ignored",
	  ((HEventHeader*)((HEvent*)(gHades->getCurrentEvent())->getHeader()))->getEventSeqNumber(),
	  subEvtId,samNr,subEvtId);
}

// ----------------------------------------------------------------------------------------------------

void HMdcUnpacker::warningExecuteSamNrNegative(Int_t samNr)
{
  Warning("execute",
	  " Event: %i - SubEvtId %i - samNr: %i (=lookup table index+1) negative ... sub event id %i ignored",
	  ((HEventHeader*)((HEvent*)(gHades->getCurrentEvent())->getHeader()))->getEventSeqNumber(),
	  subEvtId,samNr,subEvtId);
}

// ----------------------------------------------------------------------------------------------------

void HMdcUnpacker::warningInitExcludeBlackListNoValidContext(TString context)
{
  Warning("init()",
	  "No valid exclude list defined for context \"%s\" ... 'BlackList' feature is deaktivated",
	  context.Data());
}

// ----------------------------------------------------------------------------------------------------
void HMdcUnpacker::warningInitRecoverBlackListNoValidContext(TString context)
{
  Warning("init()",
	  "No valid exclude list defined for context \"%s\" ... 'BlackList' feature is deaktivated",
	  context.Data());
}

// --------------------------------------------------------------------------------

void HMdcUnpacker::warningRecoverBlackListSep2005IgnoringDatum(Int_t ctr, UInt_t datum)
{
  Warning("recoverBlackList",
	  "sep05: neither 2, 4, 6 or 12 Hits but %i ... ignoring datum: 0x%x",
	  ctr, datum);
}

// ----------------------------------------------------------------------------------------------------

void HMdcUnpacker::warningRecoverBlackListSep2005SkippingDataWord(UInt_t datum)
{
  Warning("recoverBlackListSep2005()","skipping dataword %x",datum);
}

// ----------------------------------------------------------------------------------------------------

void HMdcUnpacker::warningSetQuietMode(void)
{
  Warning("setQuietMode",
	  "subEvtId %i - from now on all error and warning messages of this unpacker are skipped, be aware of the consequences",
	  subEvtId);
}

// ----------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
// error functions

void HMdcUnpacker::errorCheckMboRawStructBoundsOutOfBounds(Int_t value, Int_t maxValue,
							   Int_t sector, Int_t module, Int_t mbo)
{
  Int_t eventSeqNr = -1;
  if (gHades) eventSeqNr = (((HEventHeader*)((HEvent*)(gHades->getCurrentEvent())->getHeader()))->getEventSeqNumber());

  if (sector == -100 && module==-100 && mbo ==-100)
    { // sector value error
    Error("checkMboRawStructBounds",
	  " Event: %i - SubEvtId %3i - sector %i, number out of bounds [0,%i[ for dataword 0x%x",
	    eventSeqNr, subEvtId, value, maxValue,
	  dataword->getCodedDataWord());
    }
  else if (module==-100 && mbo ==-100)
    { // module value error
      Error("checkMboRawStructBounds",
	    " Event: %i - SubEvtId %3i - module %i, number out of bounds [0,%i[ for sector: %i for dataword 0x%x ",
	    eventSeqNr, subEvtId, value, maxValue, sector,
	    dataword->getCodedDataWord());
    }
  else if (mbo ==-100)
    { // mbo value error
      Error("checkMboRawStructBounds",
	    " Event: %i - SubEvtId %3i - mbo %i, number out of bounds [0,%i[ for sector: %i, module: %i for dataword 0x%x",
	    eventSeqNr, subEvtId, value, maxValue, sector, module,
	    dataword->getCodedDataWord());
    }
  else
    { // tdc value error
      Error("checkMboRawStructBounds",
	    " Event: %i - SubEvtId %3i - tdc %i, number out of bounds [0,%i[ for sector: %i, module: %i, mbo: %i for dataword 0x%x",
	    eventSeqNr, subEvtId, value, maxValue, sector, module, mbo,
	    dataword->getCodedDataWord());
    }
}

// ----------------------------------------------------------------------------------------------------

void HMdcUnpacker::errorCheckMdcSetup(Int_t sector, Int_t module)
{
  Int_t eventSeqNr = -1;
  if (gHades) eventSeqNr = (((HEventHeader*)((HEvent*)(gHades->getCurrentEvent())->getHeader()))->getEventSeqNumber());

  Error("checkMdcSetup()",
	" Event: %i - SubEvtId %3i - though sector %i, module %i are excluded in the setup, dataword: 0x%08x points to this detector.",
	eventSeqNr,subEvtId,sector,module,
	dataword->getCodedDataWord());
}

// ----------------------------------------------------------------------------------------------------

void HMdcUnpacker::errorDecodeSubHeaderCannotDecode()
{
  Error("decodeSubHeader()","SubEvtId %i - can't decode sub header ",subEvtId);
}

// ----------------------------------------------------------------------------------------------------

void HMdcUnpacker::errorDecodingModeInvalidCompressMode(UInt_t compressMode,UInt_t decoding)
{
  Error("decodingMode()"," Event: %i -  SubEvtId %i - compressMode %i from decoding word 0x%x invalid ... sub event id %i ignored",
	((HEventHeader*)((HEvent*)(gHades->getCurrentEvent())->getHeader()))->getEventSeqNumber(),
	subEvtId,compressMode,decoding,subEvtId);
}

// ----------------------------------------------------------------------------------------------------

void HMdcUnpacker::errorExecuteErrorInFill()
{
  if(!noComment) Error("execute"," Event: %i - SubEvtId %i - error in unpacking",
		       ((HEventHeader*)((HEvent*)(gHades->getCurrentEvent())->getHeader()))->getEventSeqNumber(),
		       subEvtId);

}
// ----------------------------------------------------------------------------------------------------

void HMdcUnpacker::errorExecuteNoValidPointerTo(TString pointerName)
{
  Error("execute"," Event: %i - SubEvtId %i - pointer to %s zero",
	((HEventHeader*)((HEvent*)(gHades->getCurrentEvent())->getHeader()))->getEventSeqNumber(),
	subEvtId, pointerName.Data());
}

// -------------------------------------------------------------------------------

void HMdcUnpacker::errorFillCannotGetSlot(Int_t sector, Int_t module, Int_t mbo, Int_t tdc)
{
  Error("fill()", " Event: %i - SubEvtId %i - can't get slot for s:%i m:%i mb:%i tdc:%i ...",
	((HEventHeader*)((HEvent*)(gHades->getCurrentEvent())->getHeader()))->getEventSeqNumber(),
	subEvtId,sector,module,mbo,tdc);
  Error("fill()", "                           ..... skipping unpacking for the rest of this sub event!!!");
}

// --------------------------------------------------------------------------------

void HMdcUnpacker::errorFillDecodeSubHeaderFailed()
{
  Error("fill()"," Event: %i -  SubEvtId %i - decoding of sub header (ROC) failed",
	((HEventHeader*)((HEvent*)(gHades->getCurrentEvent())->getHeader()))->getEventSeqNumber(),
	subEvtId);
  Error("fill()", "                           ..... skipping unpacking for the rest of this sub event!!!");
}

// -------------------------------------------------------------------------------

void HMdcUnpacker::errorFillDataPointerExceedingRange(UInt_t* deb, UInt_t* end, UInt_t* enddata)
{
  Error("fill()",
	" Event: %i -  SubEvtId %i - data pointer %p exceeding range of sub event end %p\n with calculated end point at %p",
	((HEventHeader*)((HEvent*)(gHades->getCurrentEvent())->getHeader()))->getEventSeqNumber(),
	subEvtId,deb,end,enddata);
}

// -------------------------------------------------------------------------------

void HMdcUnpacker::errorFillErrorFillingDataWord()
{
  Error("fill()"," Event: %i - SubEvtId %i - error filling slot for dataword 0x%08x",
	((HEventHeader*)((HEvent*)(gHades->getCurrentEvent())->getHeader()))->getEventSeqNumber(),
	subEvtId,
	dataword->getCodedDataWord());
}

// ----------------------------------------------------------------------------------------------------

void HMdcUnpacker::errorFillMdcDataWordTooManyEntriesToSameLocation(Int_t maxentry)
{
  Error("fillMdcDataWord()", " SubEvtId %i - to many entries to the same location entries>%i",
	subEvtId,maxentry);
}

// ----------------------------------------------------------------------------------------------------

void HMdcUnpacker::errorFillMdcDataWordCannotGetSlot()
{
  Error("fillMdcRawEventHeader()", "SubEvtId %i - can't get slot for MdcDataWord",subEvtId);
  Error("fillMdcRawEventHeader()", "              ..... but continuing unpacking for the rest of this sub event!!!");
}

// ----------------------------------------------------------------------------------------------------

void HMdcUnpacker::errorFillMdcRawEventHeaderCannotGetSlot()
{
  Error("fillMdcRawEventHeader()", "SubEvtId %i - can't get slot for MdcEventHeader",subEvtId);
  Error("fillMdcRawEventHeader()", "              ..... but continuing unpacking for the rest of this sub event!!!");
}

// -------------------------------------------------------------------------------

void HMdcUnpacker::errorFillNoValidAddressInLookUpTable()
{
  Error("fill()",
	" Event: %i - SubEvtId %3i - No valid address in lookup table [HMdcEvReadout] for this dataword: 0x%08x",
	((HEventHeader*)((HEvent*)(gHades->getCurrentEvent())->getHeader()))->getEventSeqNumber(),
	subEvtId,
	dataword->getCodedDataWord());
}

// -------------------------------------------------------------------------------

void HMdcUnpacker::errorFillSkippingRestOfSubEvent(void)
{
  Error("fill()", "                           ..... skipping unpacking for the rest of this sub event!!!");
}

// --------------------------------------------------------------------------------

void HMdcUnpacker::errorFillUnknownCodeForBlacklistRecovery()

{
  Error("fill()","unknown code for blacklist recovery");
}

// ----------------------------------------------------------------------------------------------------

void HMdcUnpacker::errorGetSubEvtIdToSamNrSamNrNotInLookupTable(UInt_t localSubEvtId, Int_t samNr)
{
  Error("getSubEvtIdToSamNr()",
	"SubEvtId %i - calculated samNr: %i is not in the lookup table (HMdcEvReadout)",
	localSubEvtId,samNr);
}

// ----------------------------------------------------------------------------------------------------

void HMdcUnpacker::errorGetSubEvtIdToSamNrSubEvtIdOutOfMdcRange(UInt_t localSubEvtId, TString range)
{
  Error("getSubEvtIdToSamNr()"," SubEvtId %i - sub event id: %i, out of mdc range %s",
	localSubEvtId,localSubEvtId,
	range.Data());
}


// ----------------------------------------------------------------------------------------------------

void HMdcUnpacker::errorInitCannotGetParameterContainer(TString container)
{
  Error("init()"," SubEvtId %i - can't get parameter container \"%s\" ",subEvtId,container.Data());
}

// ----------------------------------------------------------------------------------------------------

void HMdcUnpacker::errorInitCategoryCannotAddCategory(TString catname, TString detector)
{
  Error("initCategory()"," SubEvtId %i - can't add category \"%s\" to detector \"%s\"",subEvtId,catname.Data(),detector.Data());
}

// ----------------------------------------------------------------------------------------------------

void HMdcUnpacker::errorInitCategoryCannotGetCategory(TString catname)
{
  Error("initCategory()"," SubEvtId %i - can't create or get category \"%s\"",subEvtId,catname.Data());
}

// ----------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------
// errorAndExit functions


void HMdcUnpacker::errorAndExitCompareEventDateTimeTo()
{
  Error("compareEventDateTimeTo()",
	" SubEvtId %i - no data header defined in current event of gHades ... exiting",
	subEvtId);
  exit(EXIT_FAILURE);
}

// ----------------------------------------------------------------------------------------------------

void HMdcUnpacker::errorAndExitDetermineDecodeVersion(void)
{
  Error("determineDecodeVersion()",
	" SubEvtId %i - cannot determine decode version ... exiting",subEvtId);
  exit(EXIT_FAILURE);
}

// --------------------------------------------------------------------------------

void HMdcUnpacker::errorAndExitFillUnknownReturnValueOfFillData()
{
    Error("fill()","unknown return value by fillData()... exiting()");
    exit(EXIT_FAILURE);
}

// ----------------------------------------------------------------------------------------------------

void HMdcUnpacker::errorAndExitGetMdcSetup()
{
  Error("getMdcSetup()"," SubEvtId %i - Mdc-Detector setup (gHades->getSetup()->getDetector(\"Mdc\")) missing... exiting()",
	subEvtId);
  exit(EXIT_FAILURE);
}

// ----------------------------------------------------------------------------------------------------

void HMdcUnpacker::errorAndExitInitExcludeBlackListNoValidContext()
{
  Error("initExcludeBlackList()","you have activated the excludeBlackList feature w/o specifying any context. \
	\n Add setExcludeBlackListContext(\"context\") for the HMdcUnpacker object to your code! \n \
	... exiting!");
  exit(EXIT_FAILURE);
}

// ----------------------------------------------------------------------------------------------------
void HMdcUnpacker::errorAndExitInitRecoverBlackListNoValidContext()
{
  Error("initRecoverBlackList()","you have activated the excludeBlackList feature w/o specifying any context. \
	\n Add setRecoverBlackListContext(\"context\") for the HMdcUnpacker object to your code! \n \
	... exiting!");
  exit(EXIT_FAILURE);
}

// --------------------------------------------------------------------------------

void HMdcUnpacker::errorAndExitRecoverBlackListSep2005UnknownReturnValueOfFillData()
{
  Error("fill()","unknown return value by fillData()... exiting()");
  exit(EXIT_FAILURE);
}

// -------------------------------------------------------------------------------

void HMdcUnpacker::errorAndExitSetDecodeVersion(UInt_t version)
{
  Error("setDecodeVersion()",
	" SubEvtId %i - selected version %i out of range [0,%i] ... exiting",
	subEvtId, version, maxDecodeVersion);
  exit(EXIT_FAILURE);
};

// ----------------------------------------------------------------------------------------------------

void HMdcUnpacker::errorAndExitSetExcludeBlackListObsoleteCall()
{
  Error("setExcludeBlackList(Bool_t b)",
	"obsolete function not supporting any context ... use enableExcludeBlackList(TString context) instead ... exiting\n");
  exit(EXIT_FAILURE);
}

// -------------------------------------------------------------------------------

void HMdcUnpacker::errorAndExitSetRecoverBlackListObsoleteCall()
{
  Error("setExcludeBlackList(Bool_t b)",
	"obsolete function not supporting any context ... use enableExcludeBlackList(TString context) instead ... exiting\n");
  exit(EXIT_FAILURE);
}

