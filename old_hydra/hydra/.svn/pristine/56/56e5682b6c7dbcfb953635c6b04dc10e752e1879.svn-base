// File: hrichcorrelatorsim.h
//
// Author: Thomas Eberl <Thomas.Eberl@physik.tu-muenchen.de>
// Last update by Thomas Eberl: 01/10/29 23:43:42
//
#ifndef HRICHCORRELATORSIM_H
#define HRICHCORRELATORSIM_H

#define MAXPARIDS 100
#include "hrichcorrelator.h"
#include "htrackinfo.h"

class HGeantKine;
class HCategory;
class HIterator;
class HRichHitSim;
class HHitMatchSim;
class HHitMatchHeaderSim;

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
        void fillHitMatchHeaderSim();
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
        virtual void iniCounters(void);
        void diagOut(HHitMatchSim*,HTrackInfo*);
protected:
    HCategory *fGeantKineCat; //!
    HIterator *iter_kine; //!
    //    HCategory *fGeantRichCat; //!
    //HIterator *iter_geantrichraw; //!
    HCategory *fShowerTrack; //!
    HIterator *iter_showertrack;//!
    Int_t richTracks[3]; //!
    Int_t richWeights[3]; //!
    Int_t mdcTracks[5]; //!
    Int_t mdcWeights[5]; //!
    Int_t tofTracks[2]; //!
    Int_t showerTracks[5]; //!
    Int_t   sumWeightsRich,   sumWeightsMdc;
    //COUNTERS
    Int_t ng_Part, ngGNb_RM, ngGNb_RT_S, ngGNb_MT_S, ngGNb_RMT_S;
    Int_t ngGNbRMonly,ngGNbRTSonly,ngGNbMTSonly,ngGNbRMTSonly;
    Int_t ngNb_RW,ngNb_MW;
    Float_t fg_RW,fg_MW;
    Int_t ngNb_Fakes;
    Int_t ngNb_ConfTracklet;
    Int_t ng_ParId[MAXPARIDS];
    //CONTROL HISTOS
    //TObjArray *histArray;
    ClassDef(HRichCorrelatorSim,1)



};

#endif //HRICHCORRELATORSIM_H



