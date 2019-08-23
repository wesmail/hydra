// File: hrichcorrelatorsim.h
//
// Author: Thomas Eberl <Thomas.Eberl@physik.tu-muenchen.de>
// Last update by Thomas Eberl: 01/10/09 08:38:57
//
#ifndef HRICHCORRELATORSIM_H
#define HRICHCORRELATORSIM_H
#include "hrichcorrelator.h"
#include "htrackinfo.h"

class HGeantKine;
class HCategory;
class HIterator;
class HRichHitSim;
class HHitMatchSim;
class HRichCorrelatorSim : public HRichCorrelator {
public:

	HRichCorrelatorSim();
        HRichCorrelatorSim(Text_t *name,Text_t *title);
        HRichCorrelatorSim(Text_t *name,Text_t *title,char* filename);
	~HRichCorrelatorSim();

        Bool_t init();
        Bool_t reinit();
        Bool_t finalize();
        Int_t execute();
        void diag(HHitMatchSim*);
        HCategory* getGeantKineCat(){return fGeantKineCat;};
        void assignRichTracks(Int_t);
        void assignMdcTracks(Int_t);
        void assignShowerTrack(Int_t);
        void assignTofTracks(Int_t);
        void cleanTracks();
        void matchTracks(HHitMatchSim*);
        void corrTracksRT_S(HHitMatchSim*);
        void corrTracksMT_S(HHitMatchSim*);
        void corrTracksRM(HHitMatchSim*);
        void setGeantTrackInfoRM(Int_t,Int_t,HTrackInfo*);
        void setGeantTrackInfoMT_S(Int_t,HTrackInfo*);
        void setGeantTrackInfoRT_S(Int_t,HTrackInfo*);
        Float_t calcRichWeight(Int_t);
        Float_t calcMdcWeight(Int_t);
        void evalBiCorrs(HHitMatchSim*);
protected:
    HCategory *fGeantKineCat; //!
    HIterator *iter_kine; //!
    HCategory *fGeantRichCat; //!
    HIterator *iter_geantrichraw; //!
    HCategory *fShowerTrack; //!
    HIterator *iter_showertrack;//!
    Int_t richTracks[3]; //!
    Int_t richWeights[3]; //!
    Int_t mdcTracks[5]; //!
    Int_t mdcWeights[5]; //!
    Int_t tofTracks[2]; //!
    Int_t showerTracks[5]; //!
    Int_t   sumWeightsRich,   sumWeightsMdc;
    ClassDef(HRichCorrelatorSim,1)



};

#endif //HRICHCORRELATORSIM_H



