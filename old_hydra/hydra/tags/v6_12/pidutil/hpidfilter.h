#ifndef HPIDFILTER_H
#define HPIDFILTER_H
#include "hfilter.h"
#include "hparset.h"
class HPidCutO;
class HPidFilter : public HFilter{
public:
    HPidFilter(void){;}
    ~HPidFilter(void){;}
    Bool_t check(TObject* pHit);
    void initCuts(HParSet*);
    void initCuts(HPidCutO*);
ClassDef(HPidFilter,1)

private:

HPidCutO* pcut; //!
};


#endif /* !HPIDFILTER_H */




