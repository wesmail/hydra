// File: hrichphotonraw.h
//
// Author: Soenke Schroeder <soenke.schroeder@physik.tu-muenchen.de>
// Last update by Soenke Schroeder: 01/07/02 17:50:33
// based on hydra532 and root 2.25-03
//
#ifndef HRICHPHOTONRAW_H
#define HRICHPHOTONRAW_H

#include "hrichphotoncal.h"

class HCategory;
class HIterator;
class HRichRaw;
class HRichLocal;

class HRichPhotonRaw : public HRichPhotonCal {
public:
    HRichPhotonRaw(const Text_t *name,const Text_t *title, Float_t charMin,
		   Float_t charMax, Int_t hitMin, Int_t hitMax,
		   Float_t thetaMin, Float_t thetaMax):
	HRichPhotonCal(name,title,charMin,charMax,hitMin,hitMax,thetaMin,
		       thetaMax){};
    ~HRichPhotonRaw(){};
    
    Bool_t init();
    
    HCategory* getRawCat() {return fRawCat;}
    
    void setRawCat(HCategory* pRawCat) {fRawCat = pRawCat;}
    
    ClassDef(HRichPhotonRaw,1) //ROOT extension
	
protected:
    HCategory *fRawCat; //!
};

#endif // HRICHPHOTONRAW_H



