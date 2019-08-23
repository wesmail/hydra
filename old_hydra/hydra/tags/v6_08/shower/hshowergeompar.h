#ifndef HSHOWERGEOMPAR_H
#define HSHOWERGEOMPAR_H

#include "hdetgeompar.h"

class HShowerGeomPar: public HDetGeomPar{
public:
	HShowerGeomPar(const char* name="ShowerGeomPar",
					const char* title="Shower geometry parameters",
					const char* context="GeomProduction");
	~HShowerGeomPar(){}					
	Bool_t init(HParIo*,Int_t*);
	Int_t write(HParIo* output);
	Int_t getSecNum(const TString&);
	Int_t getModNumInMod(const TString&);
	Int_t getModNumInComp(const TString&);
ClassDef(HShowerGeomPar,1)//Class for PreShower geometry container	
};
#endif /* !HSHOWERGEOMPAR_H */
