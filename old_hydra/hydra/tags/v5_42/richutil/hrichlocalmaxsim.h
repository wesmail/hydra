// File: hrichlocalmaxsim.h
//
// Author: Soenke Schroeder <soenke.schroeder@physik.tu-muenchen.de>
// Last update by Soenke Schroeder: 01/07/04 12:56:31
// based on hydra532 and root 2.25-03
//
#ifndef HRICHLOCALMAXSIM_H
#define HRICHLOCALMAXSIM_H

#include "hrichlocalmaxcal.h"

class HCategory;
class HIterator;
class HRichCalSim;
class HRichLocal;
class HRichGlobal;

class HRichLocalMaxSim : public HRichLocalMaxCal {
public:
    HRichLocalMaxSim(Text_t *name, Text_t *title, Int_t pads, Float_t charMin,
		     Float_t charMax, Int_t hitMin, Int_t hitMax, Int_t sector,
		     Float_t minTheta1, Float_t maxTheta1, Float_t minTheta2,
		     Float_t maxTheta2, Char_t energyFileName[128]):
	HRichLocalMaxCal(name,title,pads,charMin,charMax,hitMin,hitMax,sector,
			 minTheta1,maxTheta1,minTheta2,maxTheta2,
			 energyFileName){};
    ~HRichLocalMaxSim(){};
    
    Bool_t init();
    Bool_t energyout();
    Bool_t energyfile();
    Bool_t energyreset();
    Bool_t multihit(Int_t nRow, Int_t nCol, Int_t nSector, HLocation& fLoc);
    Float_t energyset(Int_t nRow, Int_t nCol, Int_t nSector, HLocation& fLoc);
     
    ClassDef(HRichLocalMaxSim,1) //ROOT extension

private:	
    Float_t mEnergy[6][90][92];
    Int_t mMultiHit[6][90][92];
};

#endif // HRICHLOCALMAXSIM_H
