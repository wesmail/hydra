//*-- AUTHOR : B. Spruck
//*-- Modified : Juli 2008 by B. Spruck

#ifndef HHYP_HARDCUTSMDCdEdX_H
#define HHYP_HARDCUTSMDCdEdX_H

// #include "TObject.h"
// #include "TArrayI.h"
#include "hhypbasealgorithm.h"
#include "TH2I.h"
#include "TCutG.h"
// #include "hpidalghardcuts.h"
// #include "hpidalghardcutspar.h"


// ***************************************************************************

class HHypHardCutsMDCdEdXAlg:public HHypBaseAlgorithm
{

  // ***************************************************************************
private:
//	TNtuple       * qa;// QA ntuple (beta,mom)
  	TH2I	*MomdEdx, *MomdEdxInside;// QA histos


    // hard cut parameters
//    HPidAlgStandCutsPar *pidParams;  //hard cut par container for leptons
//    HPidAlgHardCutsMDCdEdXPar  *pidParams2; //hard cut par container for hadrons

//	 Bool_t  bSim;      	            // check experiment or simulation

    TCutG *GCut_8;                  // Graphical cut for pi+
    TCutG *GCut_9;                  // Graphical cut for pi-
    TCutG *GCut_14;                 // Graphical cut for p

    Bool_t bCut8;
    Bool_t bCut9;
    Bool_t bCut14;

protected:

public:

  HHypHardCutsMDCdEdXAlg(Char_t *name_i = NULL, Option_t par[] = NULL);

  virtual ~ HHypHardCutsMDCdEdXAlg(void);

  Bool_t          execute();
  Bool_t          init();
  Bool_t          reinit();
  Bool_t          finalize();

  Int_t          CheckdEdX(Int_t iPid,Double_t mom,HPidTrackCand *pData);

  ClassDef(HHypHardCutsMDCdEdXAlg, 0)  //Hard (banana) cuts, lepton hard cut
};

//============================================================================

#endif
