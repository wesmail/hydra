// File: hrichunpackercal99.h
//
// Author: Walter Schoen <W.Schoen@physik.tu-muenchen.de>
// Last update by Walter Schoen: 99/12/17 17:12:02
// Last update by Thomas Eberl: 00/11/22 23:13:46
//
#ifndef HRichUnpackerCal99_H
#define HRichUnpackerCal99_H
#include <TROOT.h>
#include "hldunpack.h"
#include "hcategory.h"
#include "haddef.h"
#include "richdef.h"
#include "hades.h"
#include "hevent.h"
#include "hrichmappingpar.h"
#include "hparset.h"

class HRichUnpackerCal99: public HldUnpack {
public:
    HRichUnpackerCal99(int rId,int strtEvt=0,Bool_t verbose=kFALSE);
    ~HRichUnpackerCal99(void);
    int getSubEvtId(void) const;
    Bool_t init(void);
    int execute(void); 
    Bool_t finalize();
private:
    Bool_t kverbose;
    int startEvt;
    int fEventNr;
    struct DataWord{
	int sector;
	int controler;
	int port;
	int modul;
	int channel;
    } dataword;//!
    int richId;
    int SubEvtIdSec;
    int charge;
    HParSet *pMapPar;
    HCategory *pCalCat; 
    HParSet *fCalPar;   
    void printDataWord(DataWord &,int);
    void printMapping(DataWord &);
    void printCalParCell(HLocation& loc);
    void initMappingPar();
    void initCalPar(void);
    HParSet* getCalPar(){return fCalPar;}
    HRichMappingPar* getMappingPar(){return (HRichMappingPar*) pMapPar;}
    int getUpi(DataWord&);
    Bool_t testSubEvt(DataWord&,int);
public:
    ClassDef(HRichUnpackerCal99,1)        // unpack RICH data
};
#endif /* !HRichUnpackerCal99_H */
















