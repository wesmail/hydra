#ifndef HRICHPADFILTER_H
#define HRICHPADFILTER_H
# include "hfilter.h"
# include "hrichcalsim.h"

class HRichPadFilter : public HFilter{
public:
    HRichPadFilter(void){;}
    ~HRichPadFilter(void){;}
    Bool_t check(TObject* padCal);

ClassDef(HRichPadFilter,1)
};


#endif /* !HRICHPADFILTER_H */




