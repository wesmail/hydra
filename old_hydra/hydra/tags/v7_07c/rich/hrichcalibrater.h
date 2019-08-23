#ifndef HRICHCALIBRATER_H
#define HRICHCALIBRATER_H

#include "hreconstructor.h"
#include "hlocation.h"
#include "hparset.h"

class HCategory;
class HIterator;
class HRichMappingPar;
class HRichCalibrater : public HReconstructor {
public:

	HRichCalibrater();
        HRichCalibrater(Text_t *name,Text_t *title, Bool_t swap_enabled=kFALSE);
	~HRichCalibrater();

        Bool_t init();
        Bool_t finalize();
        Int_t execute();
        Bool_t calibrate(HLocation &fLoc);
        HRichCalibrater &operator=(HRichCalibrater &c);

        HCategory* getRawCat(){return fRawCat;}
        HCategory* getCalCat(){return fCalCat;}
        HParSet* getCalPar(){return fCalPar;}
        HParSet* getGeometry(){return fGeometry;}

        void setRawCat(HCategory* pRawCat){fRawCat = pRawCat;}
        void setCalCat(HCategory* pCalCat){fCalCat = pCalCat;}
        void setCalPar(HParSet* pCalPar){ if (fCalPar) delete fCalPar;
                                          fCalPar= pCalPar;}
        void setGeometry(HParSet* pGeometry){fGeometry= pGeometry;}

        void initCalPar();


        ClassDef(HRichCalibrater,1) //ROOT extension

private:

        HCategory *fRawCat; //Pointer to the raw data category
        HCategory *fCalCat; //Pointer to the cal data category

        HParSet *fCalPar; //Pointer to the cal data category
        HRichMappingPar *fMapPar; //Pointer to the mappar category
        HParSet *fGeometry; //!
	
        HIterator* fIter;    //!

	//Introduced April 2003 by Tassilo, enable this flag if you whish to perform 
	//software cable swapping in sector 3 to compensate for erroneus connections
	//Required only for Nov01 Data
	Bool_t perform_cable_swapping;
};

#endif // HRICHCALIBRATER_H
