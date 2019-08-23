#ifndef HRichHitDigitizer_H
#define HRichHitDigitizer_H

#include "hreconstructor.h"
#include "hlocation.h"
#include "hparset.h"
#include "hrichdigitizer.h"
#include "hrichgeantdirhit.h"

class HCategory;

class HRichHitDigitizer : public HRichDigitizer {
public:
	HRichHitDigitizer();
        HRichHitDigitizer(Text_t *name,Text_t *title);
	~HRichHitDigitizer();

        Bool_t digitize(HDataObject *pHit);
        HRichHitDigitizer &operator=(HRichHitDigitizer &c);

        virtual Bool_t init(void);

        ClassDef(HRichHitDigitizer,1) //ROOT extension
private:
        Bool_t digiHits(HRichGeantDirHit *simhit);

};

#endif
