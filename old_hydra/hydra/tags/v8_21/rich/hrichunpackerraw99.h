// File: hrichunpackerraw99.h
//
// Author: Walter Schoen <W.Schoen@physik.tu-muenchen.de>
// Last update by Walter Schoen: 99/12/17 17:17:17
// Last update by Thomas Eberl: 01/04/21 01:22:33
//
#ifndef HRichUnpackerRaw99_H
#define HRichUnpackerRaw99_H
#include "TROOT.h"
#include "hldunpack.h"
#include "hcategory.h"
#include "richdef.h"
#include "hades.h"
#include "hevent.h"
#include "haddef.h"
#include "hrichmappingpar.h"
#include "hparset.h"

class HRichUnpackerRaw99: public HldUnpack {
public:
    HRichUnpackerRaw99(Int_t rId,Int_t strtEvt=0,Bool_t isPed=kFALSE,Bool_t verbose=kFALSE,Int_t iSloppyL=0, Bool_t swap_enabled=kFALSE);
    ~HRichUnpackerRaw99(void);
    Int_t getSubEvtId(void) const;
    Bool_t init(void);
    Int_t execute(void);
    Bool_t finalize();
private:
    Bool_t perform_cable_swapping;
    Bool_t kverbose, isPedestal;
    Int_t iSloppyLevel;
    Int_t startEvt;
    Int_t fEventNr;
    Int_t nSubEvtCntr;
    Int_t nSecMisMatchCntr;
    struct DataWord{
 Int_t sector;
 Int_t controler;
 Int_t port;
 Int_t modul;
 Int_t channel;
    } dataword;//!
    Int_t richId;
    Int_t SubEvtIdSec;
    Int_t charge;
    HParSet *pMapPar;
    HCategory *pRawCat;
    void printDataWord(DataWord &,int);
    void printMapping(DataWord &);
    void initMappingPar();
    HRichMappingPar* getMappingPar(){return (HRichMappingPar*) pMapPar;}
    Int_t getUpi(DataWord&);
    Bool_t testSubEvt(DataWord&,int);
public:
    ClassDef(HRichUnpackerRaw99,1)        // unpack RICH data
};
#endif /* !HRichUnpackerRaw99_H */






