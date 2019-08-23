#ifndef HHITMATCHFILTER_H
#define HHITMATCHFILTER_H
#include "hfilter.h"
#include "hhitmatch.h"
#include "hparset.h"
class HHitMatchFilter : public HFilter{
public:
    HHitMatchFilter(void){;}
    ~HHitMatchFilter(void){;}
    Bool_t check(TObject* pHit);
    void initCuts(HParSet*);
ClassDef(HHitMatchFilter,1)

private:

//     Int_t nPatMatCut;  
//     Int_t nHouTraCut;  
//     Int_t nPadNrCut;   
//     Int_t nRingAmplCut;
//     Int_t nLocMax4Cut;

};


#endif /* !HHITMATCHFILTER_H */




