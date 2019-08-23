// File: hrichcorrcountersim.h
//
// Author: Thomas Eberl <Thomas.Eberl@physik.tu-muenchen.de>
// Last update by Thomas Eberl: 01/11/26 14:43:01
//
#ifndef HRICHCORRCOUNTERSIM_H
#define HRICHCORRCOUNTERSIM_H

#include "hrichcorrcounter.h"
#include "hhitmatchheadersim.h"
class HRichCorrCounterSim : public HRichCorrCounter {
public:

	HRichCorrCounterSim();
        HRichCorrCounterSim(Text_t *name,Text_t *title);
	~HRichCorrCounterSim();

        Bool_t init();
        Bool_t finalize();
        Int_t execute();
        void dumpCorrelationStatus();
        void iniCounters();
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
};

#endif // HRICHCORRCOUNTERSIM_H

