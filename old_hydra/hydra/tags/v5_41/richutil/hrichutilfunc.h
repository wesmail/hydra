// File: hrichutilfunc.h
//
// Author: Thomas Eberl <Thomas.Eberl@physik.tu-muenchen.de>
// Last update by Thomas Eberl: 01/09/18 13:49:56
//
#ifndef HRICHUTILFUNC_H
#define HRICHUTILFUNC_H
#include <TObject.h>
#include <TFile.h>
#include <TObjArray.h>
#include <TCanvas.h>
class HGeantKine;

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
    static  TCanvas* makeMicroCanvas(TObjArray*);
private:

    ClassDef(HRichUtilFunc,1)
};

#endif //HRICHUTILFUNC_H



