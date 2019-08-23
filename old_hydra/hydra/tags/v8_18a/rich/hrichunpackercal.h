// File: hrichunpackercal.h
//
// Author: Thomas Eberl <Thomas.Eberl@physik.tu-muenchen.de>
// Last update by Thomas Eberl: 00/04/27 11:24:46
//

#ifndef HRichUnpackerCal_H
#define HRichUnpackerCal_H
#include "hldunpack.h"
#include "hcategory.h"
#include "hades.h"
#include "hevent.h"
#include "haddef.h"
#include "hparset.h"
class HCategory;
class HIterator;
class HRichUnpackerCal: public HldUnpack {
public:
    HRichUnpackerCal(int);
    ~HRichUnpackerCal(void);
    Int_t getSubEvtId(void) const;
    Bool_t init(void);
    Int_t execute(void); 
    Bool_t diagnose(Int_t h_charge,Int_t h_col,Int_t h_row,Int_t h_sec,Int_t h_fEventNr);
    void initCalPar(void);
    Bool_t finalize();
    HParSet* getCalPar(){return fCalPar;}
private:
    Int_t richId;
    Int_t col,row,sec,charge;
    Int_t fEventNr;
    HCategory *pCalCat; 
    HParSet *fCalPar;   
    HParSet *fGeometry; 

public:
    ClassDef(HRichUnpackerCal,1)        // unpack RICH data
};
#endif /* !HRichUnpackerCal_H */
















