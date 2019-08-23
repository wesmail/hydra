// $Id: hrichutilfunc.h,v 1.14 2003-01-24 16:16:18 eberl Exp $
// Last update by Thomas Eberl: 03/01/16 20:58:24
//
#ifndef HRICHUTILFUNC_H
#define HRICHUTILFUNC_H
#include <TObject.h>
#include <TFile.h>
#include <TObjArray.h>
#include <TCanvas.h>
#include "haddef.h"
class HGeantKine;
class HKickTrack;
class HLinearCategory;
class HMatrixCategory;
class HIterator;
class HGeomVector;
class HHitMatchSim;
class HHitMatch;
class HRichHitSim;
class HRichUtilFunc : public TObject {
public:

	HRichUtilFunc();
	~HRichUtilFunc();

    static  void calcParticleAnglesT(HGeantKine*,Float_t&,Float_t&);
    static  void calcParticleAnglesKine(HGeantKine*,Float_t&,Float_t&);
    static  Float_t calcOpeningAngleT(Float_t,Float_t,Float_t,Float_t);
    static  Float_t calcOpeningAngleKine(HGeantKine*,HGeantKine*);
    static  Float_t openAngleDeg(Float_t, Float_t, Float_t, Float_t); 
    static  Float_t calcInvMassKine(HGeantKine*,HGeantKine*);
    static  void saveHistos(TFile*,TObjArray*);
    static  void saveCuts(TFile*,TObjArray*);
    static  void dumpKine(HGeantKine*);
    static  void dumpKickTrack(HKickTrack*);
    static  void dumpCategory(Cat_t);
    static  Int_t isPi0DalitzLep(Int_t,HLinearCategory*,Char_t*);
    static  Int_t isPi0DalitzLep(HGeantKine*,HLinearCategory*,Char_t*);
    static  Int_t isPi0ConvLep(Int_t,HLinearCategory*);
    static  Int_t isPi0ConvLep(HGeantKine*,HLinearCategory*);
    static  Int_t isLepOnMirror(HGeantKine*,HMatrixCategory*);
    static  Int_t isLepOnMDC(HGeantKine*,HMatrixCategory*);
    static  HGeantKine* findParent(HGeantKine*,HLinearCategory*);
    static  HGeantKine* getKineObj(Int_t,HLinearCategory*);
    static  HGeantKine* getSecondPionDecayGamma(HGeantKine*,HLinearCategory*);
    static  HGeantKine* getSecondPionDalitzLepton(HGeantKine*,HLinearCategory*);
    static  Bool_t isGammaFromPi0(HGeantKine*,HLinearCategory*);
    static  Bool_t isGamma(HGeantKine*);
    static  Float_t calcOpeningAngleVectors(HGeomVector&,HGeomVector&);
    static  HGeantKine* getPionDalitzGamma(HGeantKine*,HLinearCategory*);
    static  Int_t getParID(Int_t,HLinearCategory*);
    static  Int_t getParentParID(HGeantKine*,HLinearCategory*);
    static  Int_t getParentParID(Int_t,HLinearCategory*);
    static  Bool_t isGoodRing(HRichHitSim*){return kFALSE;};
    static  Bool_t isGoodTrack(HHitMatchSim*){return kFALSE;};
    static  TCanvas* makeMicroCanvas(TObjArray*);
    static void rotateTrackToLab(Int_t,Float_t,Float_t,Float_t &,Float_t &);
    static Float_t getDeflectionAngle(HHitMatch*);
private:

    ClassDef(HRichUtilFunc,1)
};

#endif //HRICHUTILFUNC_H



