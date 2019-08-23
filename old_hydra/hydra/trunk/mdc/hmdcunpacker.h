// File: $RCSfile: hmdcunpacker.h,v $
//
// Author (previous Co-Author) Peter Zumbruch <P.Zumbruch@gsi.de>
// old Author: Walter Karig <W.Karig@gsi.de>
//
// Version: $Revision: 1.48 $
// Modified by $Author: halo $ on $Date: 2009-07-23 14:54:10 $
////////////////////////////////////////////////////////////

#ifndef HMDCUNPACKER_H
#define HMDCUNPACKER_H

#include "hldunpack.h"
#include "hmdcdataword.h"

class HCategory;
class HMdcRaw;
class HMdcRawStruct;
class HMdcRawEventHeader;
class TString;
class HMdcEvReadout;

class HMdcUnpacker: public HldUnpack {
 protected:
  static HMdcDataWord* dataword; //!
  static Bool_t    noComment; //! switch off all prints

  HCategory        *pMdcRawEventHeaderCat;
  HCategory        *pMdcDataWordCat;
    
  HMdcEvReadout    *pReadout;    //!Lookup  table
  HMdcRawStruct    *rawc;

  Int_t            subEvtId;//!
  Int_t            tdcMode; // tdcMode defines the mode the tdc has taken data:
  //                           The TDC can be operated in 2 different modes:
  //                               mode 0 (default) : trigger on leading and trailing edge
  //                               mode 1           : trigger on 2 leading edges:

  UInt_t           subHeaderDecodeVersion;      //! version number of subHeader decoding
  UInt_t           decodeVersion;               //! version number of data word decoding
  UInt_t           maxDecodeVersion;            //! maximum number of decode versions

  Bool_t           debug;                              //! flag for debug mode w/ extended output
  Bool_t           consistencyCheck;                   //! flag for consistency check on the data words
  Bool_t           disableSamNrConsistencyCheck;       //! flag to disable check on SAM Nr to sector check
  Bool_t           mdcRawEventHeaderCatPersistency;    //! flag for persistency of HMdcRawEventHeader category
  Bool_t           mdcDataWordCatPersistency;          //! flag for persistency of HMdcDataWord category
  Bool_t           fillDataWord;                       //! flag to enable filling of the dataword
  Bool_t           continueDecodingAfterInconsistency; //! flag to enable stubborn behaviour of unpacker
  Int_t            determineDecodeVersionCtr;          //! internal counter

  Bool_t           standalone; //! If MDCs are not run via the common daq system, but in a standalone mode, ...
  //!                              the event trig type is not provided.
  //!                              In case of calibration triggers which are indicated in addition
  //!                              by the roc header information, the trigger type is set
  //!                              to calibration event.

  Bool_t           versionSetManual;          //! switch for version setting of unpacker
  Int_t            setup[6][4];               //! setup of Mdc (sec,mod)
  Bool_t           unpackerDebug;             //! switch to display debug infos of the unpacker software
  UInt_t           rocEvtHeaderSize;          //! size of the roc header;

  // exclude black list feature
  Bool_t           excludeBlackList;          //! switch to exclude explicitly data words
  Int_t*           excludedBlackListCounters; //! counters of excluded data words

  //   preliminary parameter, to be replaced by a lookup table
  UInt_t*          excludeBlackListMask;     //! list of hex masks on dataword for matching test with adress, preliminary parameter, to be replaced by a lookup table
  UInt_t*          excludeBlackListAdress;   //! list of adresses for comparison, preliminary parameter, to be replaced by a lookup table
  Int_t            excludeBlackListSize;     //! number of adresses/mask of black list
  TString          excludeBlackListContext;  //! context string for different exclude list, default:"UNDEFINED";

  // recover black listed
  Bool_t           recoverBlackList;          //! switch to recover explicitly data words
  Int_t*           recoveredBlackListCounters; //! counters of recovered data words

  //   preliminary parameter, to be replaced by a lookup table
  UInt_t*          recoverBlackListMask;     //! list of hex masks on dataword for matching test with adress, preliminary parameter, to be replaced by a lookup table
  UInt_t*          recoverBlackListAdress;   //! list of adresses for comparison, preliminary parameter, to be replaced by a lookup table
  Int_t            recoverBlackListSize;     //! number of adresses/mask of black list
  TString          recoverBlackListContext;  //! context string for different recover list, default:"UNDEFINED";
  UInt_t*          recoveryStorage;          //! pointer to array to store data values for recovery
  UInt_t           recoveryStorageCounter;   //! size of recovery array
  UInt_t           createBlackListRecoveryTools; //! 0 if not active else in hex date of time

  /*  consts and enums */
  enum { versionSwitchId = 1033466400 } ; //! date --date='2002-10-01 12:00:00' +%s
  enum { COMPRESSED = 2, UNCOMPRESSED = 1 };
  enum { UNCOMPRESSED_ROC_HEADERSIZE = 3, COMPRESSED_ROC_HEADERSIZE=0} ; 

  static const TString excludeBlackListContextDefaultString;
  static const TString recoverBlackListContextDefaultString;

public:
  // Range for MDC subevents is 200 - 299

  HMdcUnpacker(Int_t id, Bool_t dbg=kFALSE, Bool_t consisCheck=kFALSE);
  HMdcUnpacker(Int_t id, Int_t version, Bool_t dbg=kFALSE, Bool_t consisCheck=kFALSE); 
  HMdcUnpacker(HMdcUnpacker& mdc);
  ~HMdcUnpacker();

  Bool_t           init();
  Bool_t           reinit();
  Int_t            execute();
  Bool_t           finalize();
                   
  Bool_t           checkMdcSetup(Int_t sector, Int_t module);
  Bool_t           checkMboRawStructBounds(Int_t sector, Int_t module, Int_t mbo, Int_t tdc);
  Bool_t           decodeSubHeader(UInt_t *data, UInt_t &subEvtSize);
  Bool_t           decodingMode(void); 
  void             determineDecodeVersion(void);

  Bool_t           getConsistencyCheck(void){return consistencyCheck;};
  Bool_t           getContinueDecodingAfterInconsistency(void){return continueDecodingAfterInconsistency;};
  Bool_t           getDebug(void){return debug;};
  UInt_t           getDecodeVersion(void);
  UInt_t           getMaxDecodeVersion(void){return maxDecodeVersion;};
  Bool_t           getExcludeBlackList(void) {return excludeBlackList;};
  Bool_t           getRecoverBlackList(void) {return recoverBlackList;};
  Bool_t           getFillDataWord(void){return fillDataWord;};
  void             getMdcSetup(void);
  Bool_t           getPersistencyRawEventHeader(void){return mdcRawEventHeaderCatPersistency;};
  Bool_t           getPersistencyDataWord(void){return mdcDataWordCatPersistency;};
  Bool_t           getStandalone(void) {return standalone;};
  Int_t            getSubEvtId(void) const { return subEvtId; }
  Int_t            getSubEvtIdToSamNr(Int_t subEvtId,Bool_t expectUndefined=kFALSE);
  UInt_t           getSubHeaderDecodeVersion(void){return subHeaderDecodeVersion;};

  UInt_t           getRocEvtHeaderSize(void){return rocEvtHeaderSize;};

  Int_t            getTime(void) const { return dataword->getTime();};
  Int_t            getTdc(const Int_t chip, const Int_t chan) const {return 8*chip + chan;};
  Int_t            getTdcMode(void);
  Bool_t           getUnpackerDebug(void){return unpackerDebug;};
    
  HCategory*       initCategory(Cat_t cat, TString catname=" ", TString detector="Mdc");

  void             print(TString t="");
  void             setConsistencyCheck(Bool_t cons=kTRUE){consistencyCheck=cons;};
  void             setContinueDecodingAfterInconsistency(Bool_t cont=kTRUE);
  void             setDebug(Bool_t deb=kTRUE){debug=deb;};
  void             setDecodeVersion(UInt_t version, Bool_t manual=kTRUE);
  void             setDefaults(void);
  void             setDisableSamNrConsistencyCheck(Bool_t cons=kTRUE){disableSamNrConsistencyCheck=cons;};
  void             setErrorOutputFileName(const Char_t* f){Warning("setErrorOutputFileName()","obsolete function, no effect");};
  void             SetErrorOutputFileName(const Char_t* c){setErrorOutputFileName(c);};

  void             setExcludeBlackList(Bool_t b=kTRUE);
  void             setExcludeBlackListContext(TString s);
  void             setExcludeBlackList(TString s);
  void             enableExcludeBlackList(TString context);
  void             disableExcludeBlackList(void);

  void             setRecoverBlackList(Bool_t b=kTRUE);
  void             setRecoverBlackListContext(TString s);
  void             setRecoverBlackList(TString s);
  void             enableRecoverBlackList(TString context);
  void             disableRecoverBlackList(void);

  void             setFillDataWord(Bool_t b=kTRUE)
  {
      Warning("setFillDataWord()","Functionality switch disabled until bug is found - default: kFALSE");
      //fillDataWord=b;
  };
  void             setPersistencyRawEventHeader(Bool_t b=kTRUE){mdcRawEventHeaderCatPersistency=b;};
  void             setRocEvtHeaderSize(UInt_t ui=3);
  void             setPersistencyDataWord(Bool_t b=kTRUE){mdcDataWordCatPersistency=b;};
  void             setStandalone(Bool_t b=kTRUE){standalone = b;};
  void             setSubHeaderDecodeVersion(UInt_t ui, Bool_t manual=kTRUE)
  {subHeaderDecodeVersion=ui; versionSetManual=manual;};
  void             setTdcMode(int);
  void             setUnpackerDebug(Bool_t b=kTRUE){unpackerDebug=b;};
  Bool_t           testMdcSetup(Int_t sec, Int_t mod);

  void             setQuietMode(Bool_t quiet=kTRUE);
  static Bool_t    getQuietMode(void);
  Int_t              compareEventDateTimeTo(UInt_t cyear,  UInt_t cmonth=1,
					  UInt_t cday=1, UInt_t chour=0,
					  UInt_t cmin=0, UInt_t csec=0);
  static TString   myexec(Char_t *);

protected:

  Bool_t           excludeBlackListData(UInt_t* data);

  void             errorAndExitCompareEventDateTimeTo(void);
  void             errorAndExitDetermineDecodeVersion(void);
  void             errorAndExitGetMdcSetup(void);
  void             errorAndExitFillUnknownReturnValueOfFillData(void);
  void             errorAndExitInitExcludeBlackListNoValidContext(void);
  void             errorAndExitInitRecoverBlackListNoValidContext(void);
  void             errorAndExitRecoverBlackListSep2005UnknownReturnValueOfFillData(void);
  void             errorAndExitSetDecodeVersion(UInt_t version);
  void             errorAndExitSetExcludeBlackListObsoleteCall(void);
  void             errorAndExitSetRecoverBlackListObsoleteCall(void);

  void             errorCheckMdcSetup(Int_t sector, Int_t module);
  void             errorCheckMboRawStructBoundsOutOfBounds(Int_t value, Int_t maxValue,
							   Int_t sector=-100, Int_t module=-100, Int_t mbo=-100);
  void             errorDecodeSubHeaderCannotDecode(void);
  void             errorDecodingModeInvalidCompressMode(UInt_t compressMode,UInt_t decoding);
  void             errorExecuteNoValidPointerTo(TString pointerName);
  void             errorExecuteErrorInFill(void);
  void             errorGetSubEvtIdToSamNrSamNrNotInLookupTable(UInt_t localSubEvtId, Int_t samNr);
  void             errorGetSubEvtIdToSamNrSubEvtIdOutOfMdcRange(UInt_t localSubEvtId, TString range);
  void             errorInitCannotGetParameterContainer(TString container);
  void             errorInitCategoryCannotAddCategory(TString catname, TString detector);
  void             errorInitCategoryCannotGetCategory(TString catname);
  void             errorFillCannotGetSlot(Int_t sector, Int_t module, Int_t mbo, Int_t tdc);
  void             errorFillDecodeSubHeaderFailed(void);
  void             errorFillDataPointerExceedingRange(UInt_t* deb,
						      UInt_t* end, UInt_t* enddata);
  void             errorFillErrorFillingDataWord(void);
  void             errorFillNoValidAddressInLookUpTable();
  void             errorFillSkippingRestOfSubEvent(void);
  void             errorFillMdcDataWordCannotGetSlot(void);
  void             errorFillMdcDataWordTooManyEntriesToSameLocation(Int_t maxentry);
  void             errorFillMdcRawEventHeaderCannotGetSlot(void);
  void             errorFillUnknownCodeForBlacklistRecovery(void);
  Int_t            executeBlackListRecovery(void);

  Int_t            fill(void);
  Int_t            fillData(void);
  Int_t            fillMdcDataWord(Int_t sector,Int_t module,Int_t mbo,Int_t tdc);
  Int_t            fillMdcRawEventHeader(Int_t errorFlag, Int_t rocModule, Int_t rocNumber,
					 Int_t rocSector, Int_t subEventSize,
					 Int_t trigType);

  HMdcRaw*         getFreeOrExistingSlot(Int_t &sector,Int_t &module,Int_t &mbo,Int_t &tdc);

  void             infoDetermineDecodeVersion(void);
  Int_t            initExcludeBlackList(void);
  Int_t            initRecoverBlackList(void);

  Bool_t           matchHardwareAdressToSecModMboTdc(Int_t &sector, Int_t &module, Int_t &mbo, Int_t &tdc);

  void           prepareBlackListRecovery(UInt_t eventSize);
  void             printExcludedBlackListCounter(void);
  void             printRecoveredBlackListCounter(void);

  Int_t            recoverBlackListSep2005(void);
  Int_t            recoverBlackListApr2006(void);
	Int_t            recoverBlackListSep2008(void);
	void             reinitBlackListRecovery(void);


  void             setExcludeBlackListInternal(Bool_t b){excludeBlackList=b;};
  void             setRecoverBlackListInternal(Bool_t b){recoverBlackList=b;};
  Bool_t           storeForRecoveryFromBlackList(UInt_t *data);

  void             warningExcludeDataWords(UInt_t data, UInt_t mask, UInt_t adress);
  void             warningExecuteSamNrNegative(Int_t samNr);
  void             warningExecuteNoValidSectorEntryInLookupTableForSamNr(Int_t samNr);
  void             warningExecuteNoValidModuleEntryInLookupTableForSamNr(Int_t samNr);
  void             warningInitExcludeBlackListNoValidContext(TString context);
  void             warningInitRecoverBlackListNoValidContext(TString context);
  void             warningRecoverBlackListSep2005IgnoringDatum(Int_t ctr, UInt_t datum);
  void             warningRecoverBlackListSep2005SkippingDataWord(UInt_t datum);
  void             warningSetQuietMode(void);


  ClassDef(HMdcUnpacker, 3)	// unpack MDC data
};
#endif /* ! HMDCUNPACKER_H */

