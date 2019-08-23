// $Id: hrichcorrelatorsim.h,v 1.10 2008-05-09 16:16:32 halo Exp $
// Last update by Thomas Eberl: 02/10/11 19:06:03
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
        HRichCorrelatorSim(Text_t *name,Text_t *title,Char_t* filename,Bool_t style=kFALSE);
	~HRichCorrelatorSim();

        Bool_t init();
        Bool_t reinit();
        Bool_t finalize();
        Int_t execute();
    //        void fillHitMatchHeaderSim();
        void diag(HHitMatchSim*);
        HCategory* getGeantKineCat(){return fGeantKineCat;};
        HCategory* getGeantRichMirrorCat(){return fGeantRichMirrorCat;};
        void setGeometryPar(HParSet* pPar){pGeoPar = pPar;};
        HParSet* getGeometryPar(){return pGeoPar;};
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
        Int_t isPi0Dalitz(Int_t);
        Int_t isConvLep(Int_t);
        Int_t isLepOnMirror(HHitMatchSim*);
        Int_t isLepOnMDC(HHitMatchSim*,Int_t);
        Int_t isGRing(HHitMatchSim*);
        Int_t isGCorrRing(HTrackInfo*);
        Int_t matchRingMirrtracks(HRichHitSim *pH,Int_t trackMirr,Int_t idMirr);
        virtual void iniCounters(void);
        void diagOut(HHitMatchSim*,HTrackInfo*);
        
protected:
    HParSet  * pGeoPar;//!
    HCategory *fGeantKineCat; //!
    HIterator *iter_kine; //!
    HCategory *fGeantRichMirrorCat; //!
    HIterator *iter_mirror; //!
    HCategory *fGeantMdcCat; //!
    HIterator *iter_mdcgeant; //!
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



