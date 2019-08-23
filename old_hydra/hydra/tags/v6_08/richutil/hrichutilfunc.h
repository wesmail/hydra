// File: hrichutilfunc.h
//
// Author: Thomas Eberl <Thomas.Eberl@physik.tu-muenchen.de>
// Last update by Thomas Eberl: 02/03/06 22:36:00
//
#ifndef HRICHUTILFUNC_H
#define HRICHUTILFUNC_H
#include <TObject.h>
#include <TFile.h>
#include <TObjArray.h>
#include <TCanvas.h>
class HGeantKine;
class HLinearCategory;
class HIterator;
class HRichUtilFunc : public TObject {
public:

	HRichUtilFunc();
	~HRichUtilFunc();

    static  void calcParticleAnglesT(HGeantKine*,Float_t&,Float_t&);
    static  void calcParticleAnglesKine(HGeantKine*,Float_t&,Float_t&);
    static  Float_t calcOpeningAngleT(Float_t,Float_t,Float_t,Float_t);
    static  Float_t calcOpeningAngleKine(HGeantKine*,HGeantKine*);
    static  Float_t openAngleDeg(Float_t, Float_t, Float_t, Float_t); 
    static  void saveHistos(TFile*,TObjArray*);
    static  void dumpKine(HGeantKine*);
    static  HGeantKine* findParent(HGeantKine*,HLinearCategory*);
    static  HGeantKine* getKineObj(Int_t,HLinearCategory*);
    static  HGeantKine* getSecondPionDecayGamma(HGeantKine*,HLinearCategory*);
    static  HGeantKine* getSecondPionDalitzLepton(HGeantKine*,HLinearCategory*);
    static  Int_t getParID(Int_t,HLinearCategory*);
    static  Int_t getParentParID(HGeantKine*,HLinearCategory*);
    static  Int_t getParentParID(Int_t,HLinearCategory*);
    static  TCanvas* makeMicroCanvas(TObjArray*);
private:

    ClassDef(HRichUtilFunc,1)
};

#endif //HRICHUTILFUNC_H



