// File: hrichcorrelatorsim.h
//
// Author: Thomas Eberl <Thomas.Eberl@physik.tu-muenchen.de>
// Last update by Thomas Eberl: 01/08/21 19:33:38
//
#ifndef HRICHCORRELATORSIM_H
#define HRICHCORRELATORSIM_H
#include "hrichcorrelator.h"
class HGeantKine;
//  class HCategory;
//  class HIterator;
  class HRichHitSim;
//  class HMdcSegSim;
//  class HShowerHitTof;
//  class HTofHit;
//  class HHitMatch;
class HRichCorrelatorSim : public HRichCorrelator {
public:

	HRichCorrelatorSim();
        HRichCorrelatorSim(Text_t *name,Text_t *title);
        HRichCorrelatorSim(Text_t *name,Text_t *title,char* filename,Int_t sec=-1);
	~HRichCorrelatorSim();

        Bool_t init();
        Bool_t finalize();
        Int_t execute();
        Int_t iterateKine(HRichHitSim*);
        void iterateGeantRichRaw();
        void dumpKine(HGeantKine*);
        Bool_t isLepton(Int_t);
        Bool_t isVertexInsideRich(Float_t,Float_t,Float_t);
        Bool_t isInsideRing(Float_t,Float_t,Float_t,Float_t);
        void calcParticleAngles(HGeantKine*,Float_t&,Float_t&);
private:
    HCategory *fGeantKineCat; //!
    HIterator *iter_kine; //!
    HCategory *fGeantRichCat; //!
    HIterator *iter_geantrichraw; //!
    ClassDef(HRichCorrelatorSim,1)



};

#endif //HRICHCORRELATORSIM_H



