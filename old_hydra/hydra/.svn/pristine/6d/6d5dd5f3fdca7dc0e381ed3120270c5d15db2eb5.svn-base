// File: hrichunpackerraw99.h
//
// Author: Walter Schoen <W.Schoen@physik.tu-muenchen.de>
// Last update by Walter Schoen: 99/12/17 17:17:17
// Last update by Thomas Eberl: 01/04/21 01:22:33
//
#ifndef HRichUnpackerCal99_H
#define HRichUnpackerCal99_H
#include <TROOT.h>
#include "hldunpack.h"
#include "hcategory.h"
#include "richdef.h"
#include "hades.h"
#include "hevent.h"
#include "haddef.h"
#include "hrichmappingpar.h"
#include "hparset.h"

class HRichUnpackerCal99: public HldUnpack {
public:
    HRichUnpackerCal99(Int_t rId,Int_t strtEvt=0,Bool_t isPed=kFALSE,Bool_t verbose=kFALSE,Int_t iSloppyL=0, Bool_t swap_enabled=kFALSE);
    ~HRichUnpackerCal99(void);
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
    HCategory *pCalCat; 
    HParSet *fCalPar;  
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
    ClassDef(HRichUnpackerCal99,1)        // unpack RICH data
};
#endif /* !HRichUnpackerCal99_H */








