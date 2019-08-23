#ifndef HShowerPadDigitizer_H
#define HShowerPadDigitizer_H

#include "hreconstructor.h"
#include "hlocation.h"
#include "hiterator.h"
#include "hparset.h"
#include "hshowerpad.h"
#include "hshowerdigitizer.h"
#include "hshowergeantwire.h"

class HCategory;
class HShowerRaw;

class HShowerPadDigitizer : public HShowerDigitizer {
public:
	HShowerPadDigitizer();
        HShowerPadDigitizer(Text_t *name,Text_t *title);
	~HShowerPadDigitizer();

        Bool_t digitize(HDataObject *pHit);
        HShowerPadDigitizer &operator=(HShowerPadDigitizer &c);

        HCategory* getTrackCat(){return m_pTrackCat;}
        void setTrackCat(HCategory* pTrackCat){m_pTrackCat = pTrackCat;}

        Bool_t init(void);
        Bool_t finalize(void) {return kTRUE;}

        ClassDef(HShowerPadDigitizer,1) //ROOT extension

private:
        void initPar(void);

   Float_t calcCharge(Float_t charge, Float_t dist, 
          Float_t Xd, Float_t Yd, Float_t Xu, Float_t Yu);
   void digiPads(HShowerGeantWire* pWireHit);
   Int_t calcLimit(Float_t fCor, Int_t nMatrixRange, Float_t fBoxSize);
   void moveCoord(HShowerPad *pPad, Float_t fDx, Float_t fDy,
            Float_t *corXld, Float_t *corYld, Float_t *corXlu, Float_t *corYlu,
            Float_t *corXrd, Float_t *corYrd, Float_t *corXru, Float_t *corYru);
   void analyticCalc(HShowerPad *pPad, HShowerGeantWire* pWireHit);
   void numericalCalc(HShowerPad *pPad, HShowerGeantWire* pWireHit);

   void updatePad(HShowerPad *pPad, Float_t fIndQ, Int_t nSect, Int_t nMod, Int_t nTrack);
   void updateTrack(HShowerRaw* pShowerRaw,  HLocation& loc,  Int_t nTrack);
 
   Float_t fFactorInd;
   Float_t fChannelCoeff;

   HCategory *m_pTrackCat; //!Pointer to the raw data category
   HIterator* fTrackIter;     //!
};

#endif
