#ifndef HDIHITMATCHFILTER_H
#define HDIHITMATCHFILTER_H
#include "hfilter.h"
#include "hdihitmatch.h"
#include "hparset.h"
class HDiHitMatchFilter : public HFilter{
public:
    HDiHitMatchFilter(void){;}
    ~HDiHitMatchFilter(void){;}
    Bool_t check(TObject* pHit);
    void initCuts(HParSet*);
ClassDef(HDiHitMatchFilter,1)

private:

};


#endif /* !HDIHITMATCHFILTER_H */




