#ifndef HRichCerDigitizer_H
#define HRichCerDigitizer_H

#include "hreconstructor.h"
#include "hlocation.h"
#include "hparset.h"
#include "hrichdigitizer.h"
#include "hrichgeantcherhit.h"

class HCategory;

class HRichCerDigitizer : public HRichDigitizer {
public:
	HRichCerDigitizer();
        HRichCerDigitizer(Text_t *name,Text_t *title);
	~HRichCerDigitizer();

        Bool_t digitize(HDataObject *pHit);
        HRichCerDigitizer &operator=(HRichCerDigitizer &c);

        virtual Bool_t init(void);

        ClassDef(HRichCerDigitizer,1) //ROOT extension

private:
        Bool_t digiCherenkov(HRichGeantCherHit *simhit);
	int photonQE(const float len);
};

#endif
