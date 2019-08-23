// File: hrichphotonsim.h
//
// Author: Soenke Schroeder <soenke.schroeder@physik.tu-muenchen.de>
// Last update by Soenke Schroeder: 01/07/02 17:40:12
// based on hydra532 and root 2.25-03
//
#ifndef HRICHPHOTONSIM_H
#define HRICHPHOTONSIM_H

#include "hrichphotoncal.h"

class HCategory;
class HIterator;
class HRichCalSim;
class HRichLocal;

class HRichPhotonSim : public HRichPhotonCal {
public:
    HRichPhotonSim(Text_t *name, Text_t *title, Float_t charMin, 
		   Float_t charMax, Int_t hitMin, Int_t hitMax,
		   Float_t thetaMin, Float_t thetaMax):
	HRichPhotonCal(name,title,charMin,charMax,hitMin,hitMax,thetaMin,
		       thetaMax){};
    ~HRichPhotonSim(){};
    
    Bool_t init();
    
    ClassDef(HRichPhotonSim,1) //ROOT extension
       
};

#endif // HRICHPHOTONSIM_H
