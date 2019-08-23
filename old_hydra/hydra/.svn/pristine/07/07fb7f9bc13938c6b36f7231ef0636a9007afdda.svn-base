// File: hrichunpackerraw.h
//
// Author: Thomas Eberl <Thomas.Eberl@physik.tu-muenchen.de>
// Last update by Thomas Eberl: 00/05/11 22:05:17
//

#ifndef HRichUnpackerRaw_H
#define HRichUnpackerRaw_H
#include "hldunpack.h"
#include "hcategory.h"
#include "hades.h"
#include "hevent.h"
#include "haddef.h"
#include "TVector.h"
class HRichUnpackerRaw: public HldUnpack {
public:
    HRichUnpackerRaw(int);
    ~HRichUnpackerRaw(void);
    Int_t getSubEvtId(void) const;
    Bool_t init(void);
    Bool_t finalize(void);
    Int_t execute(void); 
    
private:
    Bool_t diagnose(Int_t h_charge,Int_t h_col,Int_t h_row,Int_t h_sec,Int_t h_fEventNr);
    Bool_t padReadOut(Int_t h_col,Int_t h_row);
    void initMapping();
    TVector* xyuiplog;
    Int_t richId;
    Int_t col,row,sec,charge;
    Int_t fEventNr;
public:
    ClassDef(HRichUnpackerRaw,1)        // unpack RICH data
	};
#endif /* !HRichUnpackerRaw_H */






