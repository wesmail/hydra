#ifndef HRICHHITFILTER_H
#define HRICHHITFILTER_H
#include "hfilter.h"
#include "hrichhit.h"
#include "hparset.h"
class HRichHitFilter : public HFilter{
public:
    HRichHitFilter(void){;}
    ~HRichHitFilter(void){;}
    Bool_t check(TObject* pHit);
    void initCuts(HParSet*);
ClassDef(HRichHitFilter,1)

private:

    Int_t nPatMatCut;  
    Int_t nHouTraCut;  
    Int_t nPadNrCut;   
    Int_t nRingAmplCut;
    Int_t nLocMax4Cut;

};


#endif /* !HRICHHITFILTER_H */




