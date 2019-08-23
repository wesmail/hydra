// File: hrichunpackerraw99.h
//
// Author: Walter Schoen <W.Schoen@physik.tu-muenchen.de>
// Last update by Walter Schoen: 99/12/17 17:17:17
// Last update by Thomas Eberl: 01/04/21 01:22:33
//
#ifndef HRichUnpackerCal99_H
#define HRichUnpackerCal99_H

#include "hldunpack.h"
#include "hlocation.h"

class HParSet;
class HCategory;
class HRichMappingPar;


class HRichUnpackerCal99: public HldUnpack {

// public data members and functions
public:
  HRichUnpackerCal99(Int_t  rId,
		     Bool_t wrtRaw       = kTRUE,
		     Int_t  strtEvt      = 0,
		     Bool_t isPed        = kFALSE,
		     Bool_t verbose      = kFALSE,
		     Int_t  iSloppyL     = 0,
		     Bool_t swap_enabled = kFALSE);

  ~HRichUnpackerCal99(void);
  Int_t  getSubEvtId(void) const;
  Bool_t init(void);
  Int_t  execute(void);
  Bool_t finalize();

// private data members and functions
 private:

// flags to control the operation of the unpacker
  Bool_t perform_cable_swapping; //! relevant only for NOV01 analysis
  Bool_t kverbose;               //! verbose operation
  Bool_t isPedestal;             //! true only for pedestal mode (obsolete)
  Bool_t writeRaw;               //! fill catRichRaw (needed for IPU emulation)
  Int_t  iSloppyLevel;           //! error tolerance
  Int_t  startEvt;               //! start unpacking with event #


  Int_t  fEventNr;               //! current event #
  Int_t  nSubEvtCntr;            //! 
  Int_t  nSecMisMatchCntr;       //! 
  Int_t  richId;                 //! subevent ID identifying RC
  Int_t  SubEvtIdSec;            //! sector number
  Int_t  charge;                 //! measured charge

  struct DataWord {
    Int_t sector;
    Int_t controler;
    Int_t port;
    Int_t modul;
    Int_t channel;
  } dataword;                    //! structure for channel address

  HParSet   *pMapPar;            //!
  HCategory *pCalCat;            //!
  HCategory *pRawCat;            //!
  HParSet   *fCalPar;            //!


  void printDataWord(DataWord &,int);
  void printMapping(DataWord &);
  void printCalParCell(HLocation& loc);
  void initMappingPar();
  void initCalPar(void);
  HRichMappingPar* getMappingPar(){return (HRichMappingPar*) pMapPar;}
  HParSet* getCalPar(){return fCalPar;}
  Int_t getUpi(DataWord&);
  Bool_t testSubEvt(DataWord&,int);


 public:
  ClassDef(HRichUnpackerCal99,0)        // unpack RICH data
};

#endif /* !HRichUnpackerCal99_H */

