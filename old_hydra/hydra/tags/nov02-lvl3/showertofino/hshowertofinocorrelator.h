#ifndef HShowerTofinoCorrelator_h
#define HShowerTofinoCorrelator_h

#include "hreconstructor.h"
#include "hlocation.h"
#include "hparset.h"

class HCategory;
class HIterator;
class HShowerHit;
class HShowerHitTof;

class HShowerTofinoCorrelator : public HReconstructor {
public:
	HShowerTofinoCorrelator();
        HShowerTofinoCorrelator(Text_t *name,Text_t *title);
	~HShowerTofinoCorrelator();

        Bool_t init(void);
        Bool_t finalize(void);
        Int_t execute(void);

        HShowerHitTof* addHitTof(HShowerHit* pHit);

        HCategory* getTofinoCat() {return m_pTofinoCat;}
        HCategory* getHitCat() {return m_pHitCat;}
        HCategory* getHitTofCat() {return m_pHitTofCat;}

        void setTofinoCat(HCategory* pTofinoCat) {m_pTofinoCat = pTofinoCat;}
        void setHitCat(HCategory* pHitCat) {m_pHitCat = pHitCat;}
        void setHitTofCat(HCategory* pHitTofCat) {m_pHitTofCat = pHitTofCat;}

        ClassDef(HShowerTofinoCorrelator,1) // Produce Tofino/Shower HIT data 

private:
        HLocation m_zeroLoc;

        HCategory *m_pTofinoCat;   //!Pointer to the Tofino CAL data category
        HCategory *m_pHitCat;      //!Pointer to the Shower HIT data category
        HCategory *m_pHitTofCat;   //!Pointer to the Tofino HIT data category

        HIterator *fHitIter;       //!Iterator for Shower HIT
        HIterator *fTofinoIter;    //!Iterator for Tofino CAL
        HIterator *fHitTofinoIter; //!Iterator for combined data
 
        HParSet* m_pTofinoCalPar;  //!Tofino's calibration parameters
        HParSet* m_pTofShowerMap;  //!Shower-Tofino Map
};

#endif
