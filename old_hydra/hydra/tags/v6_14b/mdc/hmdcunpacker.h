#ifndef HMDCUNPACKER_H
#define HMDCUNPACKER_H

#include "hldunpack.h"
#include "TArrayI.h"
#include "hmdcevreadout.h"
#include "TTree.h"
#include "TNtuple.h"
#include "TString.h"
#include "hmdcdef.h"
#include "hmdcdataword.h"
#include "hmdcraweventheader.h"
#include "hmdcrawstruct.h"
#include "TUnixSystem.h"

class HMdcUnpacker: public HldUnpack {
 protected:
  static HMdcDataWord* dataword; //!
  static Bool_t noComment; //! switch off all prints

  HCategory *pMdcRawEventHeaderCat;
  HCategory *pMdcDataWordCat;
    
  HMdcEvReadout *pReadout;       //!Lookup up table 
  HMdcRawStruct *rawc;

  Int_t subEvtId;//!
  Int_t tdcMode; // tdcMode defines the mode the tdc has taken data:
  // The TDC can be operated in 2 different modes:
  // mode 0 (default) : trigger on leading and trailing edge
  // mode 1           : trigger on 2 leading edges:

  UInt_t subHeaderDecodeVersion;      //! version number of subHeader decoding
  UInt_t decodeVersion;               //! version number of data word decoding
  UInt_t maxDecodeVersion;

  Bool_t debug;                              //!
  Bool_t consistencyCheck;                   //!
  Bool_t disableSamNrConsistencyCheck;       //!
  Bool_t mdcRawEventHeaderCatPersistency;    //!
  Bool_t mdcDataWordCatPersistency;          //!
  Bool_t fillDataWord;                       //!
  Bool_t continueDecodingAfterInconsistency; //!
  Int_t determineDecodeVersionCtr; //! internal counter 

  Bool_t standalone; //! If MDCs are not run via the common daq system, but in a
  // standalone mode,							
  // the event trig type is not provided. 					
  // In case of calibration triggers which are indicated in addition	
  // by the roc header information, the trigger type is set 		
  // to calibration event.
  Bool_t versionSetManual;// ! switch for version setting of unpacker
  Int_t setup[6][4]; //! setup of Mdc (sec,mod)
  Bool_t unpackerDebug; //! switch to display debug infos of the unpacker software 
  UInt_t rocEvtHeaderSize;//! size of the roc header; 

  enum { versionSwitchId = 1033466400 } ;//! date --date='2002-10-01 12:00:00' +%s  
  enum { COMPRESSED = 2, UNCOMPRESSED = 1 };
  enum { UNCOMPRESSED_ROC_HEADERSIZE = 3, COMPRESSED_ROC_HEADERSIZE=0} ; 

 public:
  // Range for MDC subevents is 200 - 299

  HMdcUnpacker(Int_t id, Bool_t dbg=kFALSE, Bool_t consisCheck=kFALSE);
  HMdcUnpacker(Int_t id, Int_t version, Bool_t dbg=kFALSE, Bool_t consisCheck=kFALSE); 
  HMdcUnpacker(const HMdcUnpacker& mdc);
  ~HMdcUnpacker();

  Bool_t init();
  Bool_t reinit();
  Int_t execute();
  Bool_t finalize();

  inline Bool_t checkMdcSetup(Int_t sector, Int_t module, UInt_t dataword);
  inline Bool_t checkMboRawStructBounds(Int_t sector,Int_t module, Int_t mbo, Int_t tdc , UInt_t dataword);
  inline Bool_t decodeSubHeader(UInt_t *data, UInt_t &subEvtSize);
  Bool_t decodingMode(void); 
  void determineDecodeVersion(void);

  Bool_t getConsistencyCheck(void){return consistencyCheck;};
  Bool_t getContinueDecodingAfterInconsistency(void){return continueDecodingAfterInconsistency;};
  Bool_t getDebug(void){return debug;};
  inline UInt_t getDecodeVersion(void);            
  Bool_t getFillDataWord(void){return fillDataWord;};
  void getMdcSetup(void);
  Bool_t getPersistencyRawEventHeader(void){return mdcRawEventHeaderCatPersistency;};
  Bool_t getPersistencyDataWord(void){return mdcDataWordCatPersistency;};
  Bool_t getStandalone(void) {return standalone;};
  Int_t getSubEvtId() const { return subEvtId; }
  inline Int_t getSubEvtIdToSamNr(Int_t subEvtId,Bool_t expectUndefined=kFALSE);
  UInt_t getSubHeaderDecodeVersion(void){return subHeaderDecodeVersion;};     

  UInt_t getRocEvtHeaderSize(void){return rocEvtHeaderSize;}; 

  Int_t getTime() const { return dataword->getTime();};
  Int_t getTdc(const Int_t chip, const Int_t chan) const {return 8*chip + chan;};
  Int_t getTdcMode(void);
  Bool_t getUnpackerDebug(void){return unpackerDebug;};
    
  HCategory* initCategory(Cat_t cat, TString catname=" ", TString detector="Mdc");   

  void print(TString t="");
  void setConsistencyCheck(Bool_t cons=kTRUE){consistencyCheck=cons;};
  void setContinueDecodingAfterInconsistency(Bool_t cont=kTRUE);
  void setDebug(Bool_t deb=kTRUE){debug=deb;};
  inline void setDecodeVersion(UInt_t version, Bool_t manual=kTRUE);
  void setDefaults(void);
  void setDisableSamNrConsistencyCheck(Bool_t cons=kTRUE){disableSamNrConsistencyCheck=cons;};
  void setErrorOutputFileName(Char_t* f){Warning("setErrorOutputFileName()","obsolete function, no effect");};
  void SetErrorOutputFileName(Char_t* c){setErrorOutputFileName(c);};
  void setFillDataWord(Bool_t b=kTRUE) 
    {
      Warning("setFillDataWord()","Functionality switch disabled until bug is found - default: kFALSE");
      //fillDataWord=b;
    };
  void setPersistencyRawEventHeader(Bool_t b=kTRUE){mdcRawEventHeaderCatPersistency=b;};
  void setRocEvtHeaderSize(UInt_t ui=3);
  void setPersistencyDataWord(Bool_t b=kTRUE){mdcDataWordCatPersistency=b;};
  void setStandalone(Bool_t b=kTRUE){standalone = b;};
  void setSubHeaderDecodeVersion(UInt_t ui, Bool_t manual=kTRUE){subHeaderDecodeVersion=ui; versionSetManual=manual;};     
  void setTdcMode(int);
  void setUnpackerDebug(Bool_t b=kTRUE){unpackerDebug=b;};
  inline Bool_t testMdcSetup(Int_t sec, Int_t mod); 

  void setQuietMode(Bool_t quiet=kTRUE);
  static Bool_t getQuietMode();
  int compareEventDateTimeTo(UInt_t cyear,UInt_t cmonth=1, UInt_t cday=1,UInt_t chour=0,UInt_t cmin=0,UInt_t csec=0);
  static char* myexec(char *);
 protected:
  Int_t fill(void);
  inline Int_t fillMdcDataWord(Int_t sector,Int_t module,Int_t mbo,Int_t tdc);
  inline Int_t fillMdcRawEventHeader(Int_t errorFlag, Int_t rocModule, Int_t rocNumber, 
				     Int_t rocSector, Int_t subEvtId,  Int_t subEventSize, 
				     Int_t trigType);

  void ErrorFillDecodeSubHeaderFailed(Int_t subEvtId);
  void ErrorFillDataPointerExceedingRange(Int_t subEvtId,UInt_t* deb, UInt_t* end, UInt_t* enddata);
  void ErrorFillSkippingRestOfSubEvent(void);
  void ErrorFillNoValidAddressInLookUpTable(Int_t subEvtId, UInt_t deb);
  void ErrorFillCannotGetSlot(Int_t subEvtId, Int_t sector, Int_t module, Int_t mbo, Int_t tdc);
  void ErrorFillErrorFillingDataWord(Int_t subEvtId, UInt_t deb);

  ClassDef(HMdcUnpacker, 3)	// unpack MDC data 
};
#endif /* ! HMDCUNPACKER_H */

