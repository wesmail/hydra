#ifndef HRichPadDigitizer_H
#define HRichPadDigitizer_H

#include "hreconstructor.h"
#include "hlocation.h"
#include "hparset.h"
#include "hrichpad.h"
#include "hrichdigitizer.h"
#include "hrichgeantwire.h"

class HCategory;

class HRichPadDigitizer : public HRichDigitizer {
public:
	HRichPadDigitizer();
        HRichPadDigitizer(Text_t *name,Text_t *title);
	~HRichPadDigitizer();

        Bool_t digitize(HDataObject *pHit);
        HRichPadDigitizer &operator=(HRichPadDigitizer &c);

        virtual Bool_t init(void);
        void initPar(void);

        ClassDef(HRichPadDigitizer,1) //ROOT extension

private:
   Float_t calcCharge(Float_t charge, Float_t dist, 
          Float_t Xd, Float_t Yd, Float_t Xu, Float_t Yu);
   void digiPads(HRichGeantWire* pWireHit);
   Int_t calcLimit(Float_t fCor, Int_t nMatrixRange, Float_t fBoxSize);
   void moveCoord(HRichPad *pPad, Float_t fDx, Float_t fDy,
               Float_t *corXd, Float_t *corYd, Float_t *corXu, Float_t *corYu);
   void analyticCalc(HRichPad *pPad, HRichGeantWire* pWireHit);
   void numericalCalc(HRichPad *pPad, HRichGeantWire* pWireHit);

   void updateCharge(HRichPad *pPad, Float_t fIndQ, Int_t nSect);

   Float_t fFactorInd;
};

#endif
