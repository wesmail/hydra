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
#include "richdef.h"
#include <TVector.h>
class HRichUnpackerRaw: public HldUnpack {
public:
    HRichUnpackerRaw(int);
    ~HRichUnpackerRaw(void);
    int getSubEvtId(void) const;
    Bool_t init(void);
    Bool_t finalize(void);
    int execute(void); 
    
private:
    Bool_t diagnose(int h_charge,int h_col,int h_row,int h_sec,int h_fEventNr);
    Bool_t padReadOut(int h_col,int h_row);
    void initMapping();
    TVector* xyuiplog;
    int richId;
    int col,row,sec,charge;
    int fEventNr;
public:
    ClassDef(HRichUnpackerRaw,1)        // unpack RICH data
	};
#endif /* !HRichUnpackerRaw_H */






