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
    int getSubEvtId(void) const;
    Bool_t init(void);
    int execute(void); 
    Bool_t diagnose(int h_charge,int h_col,int h_row,int h_sec,int h_fEventNr);
    void initCalPar(void);
    Bool_t finalize();
    HParSet* getCalPar(){return fCalPar;}
private:
    int richId;
    int col,row,sec,charge;
    int fEventNr;
    HCategory *pCalCat; 
    HParSet *fCalPar;   
    HParSet *fGeometry; 

public:
    ClassDef(HRichUnpackerCal,1)        // unpack RICH data
};
#endif /* !HRichUnpackerCal_H */
















