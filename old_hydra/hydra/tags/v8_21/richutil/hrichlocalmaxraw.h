// File: hrichlocalmaxraw.h
//
// Author: Soenke Schroeder <soenke.schroeder@physik.tu-muenchen.de>
// Last update by Soenke Schroeder: 01/07/02 10:50:22
// based on hydra532 and root 2.25-03
//
#ifndef HRICHLOCALMAXRAW_H
#define HRICHLOCALMAXRAW_H

#include "hrichlocalmaxcal.h"

class HCategory;
class HIterator;
class HRichRaw;
class HRichLocal;
class HRichGlobal;

class HRichLocalMaxRaw : public HRichLocalMaxCal {
public:
    HRichLocalMaxRaw(const Text_t *name,const Text_t *title, Int_t pads, Float_t charMin,
		     Float_t charMax, Int_t hitMin, Int_t hitMax, Int_t sector,
		     Float_t minTheta1, Float_t maxTheta1,Float_t minTheta2,
		     Float_t maxTheta2, Char_t energyFileName[128]):
	HRichLocalMaxCal(name,title,pads,charMin,charMax,hitMin,hitMax,sector,
			 minTheta1,maxTheta1,minTheta2,maxTheta2,
			 energyFileName){};
    ~HRichLocalMaxRaw(){};

    Bool_t init();
    Bool_t energyfile();
    
    HCategory* getRawCat() {return fRawCat;}
    
    void setRawCat(HCategory* pRawCat) {fRawCat = pRawCat;}
    
    ClassDef(HRichLocalMaxRaw,1) //ROOT extension
	
protected:
    HCategory *fRawCat; //! Pointer to the raw data category
};

#endif // HRICHLOCALMAXRAW_H
