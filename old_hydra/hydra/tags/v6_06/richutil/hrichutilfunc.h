// File: hrichutilfunc.h
//
// Author: Thomas Eberl <Thomas.Eberl@physik.tu-muenchen.de>
// Last update by Thomas Eberl: 01/10/04 19:00:12
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
    static  void saveHistos(TFile*,TObjArray*);
    static  void dumpKine(HGeantKine*);
    static  HGeantKine* findParent(HGeantKine*,HLinearCategory*);
    static  HGeantKine* getKineObj(Int_t,HLinearCategory*);
    static  Int_t getParID(Int_t,HLinearCategory*);
    static  TCanvas* makeMicroCanvas(TObjArray*);
private:

    ClassDef(HRichUtilFunc,1)
};

#endif //HRICHUTILFUNC_H



