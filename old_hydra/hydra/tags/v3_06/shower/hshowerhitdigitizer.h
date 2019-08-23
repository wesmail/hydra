#ifndef HShowerHitDigitizer_H
#define HShowerHitDigitizer_H

#include "hreconstructor.h"
#include "hlocation.h"
#include "hparset.h"
#include "hshowerdigitizer.h"
#include "hgeantshower.h"

class HCategory;

class HShowerHitDigitizer : public HShowerDigitizer {
public:
	HShowerHitDigitizer();
        HShowerHitDigitizer(Text_t *name,Text_t *title);
	~HShowerHitDigitizer();

        Bool_t digitize(HDataObject *pHit);
        HShowerHitDigitizer &operator=(HShowerHitDigitizer &c);

        Bool_t init(void);
        Bool_t finalize(void) {return kTRUE;}

        ClassDef(HShowerHitDigitizer,1) //SHOWER hit digitizer
private:
        virtual Bool_t digiHits(HGeantShower *simhit);
        virtual Float_t calcHit(Float_t fBeta, Float_t fVoltage);
        virtual Float_t calcQMean(Float_t fBeta, Float_t fVoltage);

        double piconst;

};

#endif
