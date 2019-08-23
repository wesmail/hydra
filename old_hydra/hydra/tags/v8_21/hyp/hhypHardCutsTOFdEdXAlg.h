//*-- AUTHOR : B. Spruck
//*-- Modified : Juli 2008 by B. Spruck

#ifndef HHYP_HARDCUTSTOFdEdX_H
#define HHYP_HARDCUTSTOFdEdX_H

// #include "TObject.h"
// #include "TArrayI.h"
#include "hhypbasealgorithm.h"
#include "TH2I.h"
#include "TCutG.h"
// #include "hpidalghardcuts.h"
// #include "hpidalghardcutspar.h"


// ***************************************************************************

class HHypHardCutsTOFdEdXAlg:public HHypBaseAlgorithm
{

  // ***************************************************************************
private:
//	TNtuple       * qa;// QA ntuple (beta,mom)
 	TH2I	*MomdEdx, *MomdEdxInside;// QA hist

    // hard cut parameters
//    HPidAlgStandCutsPar *pidParams;  //hard cut par container for leptons
//    HPidAlgHardCutsTOFdEdXPar  *pidParams2; //hard cut par container for hadrons

//	 Bool_t  bSim;      	            // check experiment or simulation

   TCutG *GCut_8;                  // Graphical cut for pi+
   TCutG *GCut_9;                  // Graphical cut for pi-
   TCutG *GCut_14;                 // Graphical cut for p

   Bool_t bCut8;
   Bool_t bCut9;
   Bool_t bCut14;

protected:

public:

  HHypHardCutsTOFdEdXAlg(Char_t *name_i = NULL, Option_t par[] = NULL);

  virtual ~ HHypHardCutsTOFdEdXAlg(void);

  Bool_t          execute();
  Bool_t          init();
  Bool_t          reinit();
  Bool_t          finalize();

  Int_t          CheckdEdX(Int_t iPid,Double_t mom,HPidTrackCand *pData);

  ClassDef(HHypHardCutsTOFdEdXAlg, 0)  //Hard (banana) cuts, lepton hard cut
};

//============================================================================

#endif
