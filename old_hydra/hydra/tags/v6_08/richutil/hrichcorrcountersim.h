// File: hrichcorrcountersim.h
//
// Author: Thomas Eberl <Thomas.Eberl@physik.tu-muenchen.de>
// Last update by Thomas Eberl: 02/03/08 22:42:38
//
#ifndef HRICHCORRCOUNTERSIM_H
#define HRICHCORRCOUNTERSIM_H

#include "hrichcorrcounter.h"
#include "hhitmatchheadersim.h"
#include "hhitmatchsim.h"
class HRichCorrCounterSim : public HRichCorrCounter {
public:

	HRichCorrCounterSim();
        HRichCorrCounterSim(Text_t *name,Text_t *title);
        HRichCorrCounterSim(Text_t *name,Text_t *title, char* filename);
	~HRichCorrCounterSim();

        Bool_t init();
        Bool_t finalize();
        Int_t execute();
        void dumpCorrelationStatus();
        void iniCounters();
        void iniHistos();
        Int_t fillHistos();
        Int_t fillGeantMdcTofdTheta(HHitMatchSim*,Int_t,Int_t*,Int_t*);
        Int_t fillGeantMdcShowerdTheta(HHitMatchSim*,Int_t,Int_t*,Int_t*);
        Int_t fillGeantMdcTofdThetanoGCond(HHitMatchSim*,Int_t,Int_t*,Int_t*);
    Int_t selectDalitzTrack(TObjArray*,HHitMatchSim*);
    Int_t selectConvTrack(TObjArray*,HHitMatchSim*);
    Int_t selectAll(TObjArray*,HHitMatchSim*);
    ClassDef(HRichCorrCounterSim,1)

	    protected:
	Int_t ng_Part;
    Int_t ngNb_ConfTracklet;
    Int_t ngNb_Fakes;
    Float_t fAvrgRichWeight;
    Float_t fAvrgMdcWeight;
    Int_t ngGNbRMonly;
    Int_t ngGNbRTSonly;
    Int_t ngGNbMTSonly;
    Int_t ngGNbRMTSonly;
    Int_t ng_ParId[MAXPARIDS];
    Int_t ngNb_RW;
    Int_t ngNb_MW;
    //HISTOGRAMS
    TObjArray* pHistArray2;//!
    // Tof
    TH2F* pH_ParIdMdcTofdTheta;//!
    TH2F* pH_MechMdcTofdTheta;//!
    TH2F* pH_MedMdcTofdTheta;//!
    TH2F* pH_MomMdcTofdTheta;//!
    TH2F* pH_CreaIdMdcTofdTheta;//!

    TH2F* pH_ele_MechMdcTofdTheta;//!
    TH2F* pH_ele_MedMdcTofdTheta;//!
    TH2F* pH_ele_MomMdcTofdTheta;//!
    TH2F* pH_ele_CreaIdMdcTofdTheta;//!

    TH2F* pH_pos_MechMdcTofdTheta;//!
    TH2F* pH_pos_MedMdcTofdTheta;//!
    TH2F* pH_pos_MomMdcTofdTheta;//!
    TH2F* pH_pos_CreaIdMdcTofdTheta;//!

    // Shower
    TH2F* pH_ParIdMdcShowerdTheta;//!
    TH2F* pH_MechMdcShowerdTheta;//!
    TH2F* pH_MedMdcShowerdTheta;//!
    TH2F* pH_MomMdcShowerdTheta;//!
    TH2F* pH_CreaIdMdcShowerdTheta;//!

    TH2F* pH_ele_MechMdcShowerdTheta;//!
    TH2F* pH_ele_MedMdcShowerdTheta;//!
    TH2F* pH_ele_MomMdcShowerdTheta;//!
    TH2F* pH_ele_CreaIdMdcShowerdTheta;//!

    TH2F* pH_pos_MechMdcShowerdTheta;//!
    TH2F* pH_pos_MedMdcShowerdTheta;//!
    TH2F* pH_pos_MomMdcShowerdTheta;//!
    TH2F* pH_pos_CreaIdMdcShowerdTheta;//!
    // under certain cond !!
    TH2F* pH_ParIdMech;//!
    TH2F* pH_ParIdMom;//!
    TH2F* pH_ParIdMed;//!
    TH2F* pH_ParIdCreaId;//!

    // opening angles
    TH1F* pH_opangleMDCa;//!
    TH1F* pH_opangleMDCDalitz;//!
    TH1F* pH_opangleMDCConv;//!
    TH1F* pH_opangleMETADalitz;//!
    TH1F* pH_opangleMETAConv;//!

    TH2F* pH_opangleMETAdThetaDalitz;//!
    TH2F* pH_opangleMETAdPhiDalitz;//!
    TH2F* pH_opangleMETAdThetaConv;//!
    TH2F* pH_opangleMETAdPhiConv;//!
};

#endif // HRICHCORRCOUNTERSIM_H



