#ifndef HShowerCalibrater_H
#define HShowerCalibrater_H

#include "hreconstructor.h"
#include "hlocation.h"
#include "hparset.h"
#include "hshowerraw.h"

class HCategory;
class HIterator;

class HShowerCalibrater : public HReconstructor {
public:
	HShowerCalibrater();
        HShowerCalibrater(Text_t *name,Text_t *title);
	~HShowerCalibrater();

        Bool_t init(void);
        Bool_t reinit(void);
        Bool_t finalize(void);
        Int_t execute(void);
        Bool_t calibrate(HShowerRaw *raw);
        HShowerCalibrater &operator=(HShowerCalibrater &c);

        HCategory* getRawCat(){return m_pRawCat;}
        HCategory* getCalCat(){return m_pCalCat;}
        HParSet* getCalPar(){return m_pCalPar;}
        HParSet* getGeometry(){return m_pGeometry;}

        void setRawCat(HCategory* pRawCat){m_pRawCat = pRawCat;}
        void setCalCat(HCategory* pCalCat){m_pCalCat = pCalCat;}
        void setCalPar(HParSet*);
        void setGeometry(HParSet* pGeometry){m_pGeometry= pGeometry;}

        void initCalPar();
        

        ClassDef(HShowerCalibrater,1) //ROOT extension

private:
        int nn;
        HLocation m_zeroLoc; 
	HLocation m_loc; //Location of object being calibrated
        HCategory *m_pRawCat; //Pointer to the raw data category
        HCategory *m_pCalCat; //Pointer to the cal data category

        HParSet *m_pGeometry; //!
        HParSet *m_pCalPar; //!Pointer to the cal data category

        HIterator *fIter;  //!
};

#endif