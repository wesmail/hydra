//*-- AUTHOR : I. Froehlich
//*-- Modified : 25.7.2005 by I. Froehlich

#ifndef HHYP_HARDCUTS_H
#define HHYP_HARDCUTS_H

#include <TObject.h>
#include <TArrayI.h>
#include "TCutG.h"
#include "hhypbasealgorithm.h"
#include "hhyplistfiller.h"
#include "hpidalghardcuts.h"
#include "hpidalghardcutspar.h"


// ***************************************************************************

class HHypHardCutsAlg:public HHypBaseAlgorithm
{

  // ***************************************************************************
private:
	TNtuple       * qa;// QA ntuple (beta,mom)

    // hard cut parameters
    HPidAlgStandCutsPar *pidParams;  //hard cut par container for leptons
    HPidAlgHardCutsPar  *pidParams2; //hard cut par container for hadrons
    
    TString sInFileName;            // input file for the graphical cut
    
	 Bool_t  bSim;      	            // check experiment or simulation
    
    TCutG *GCut_8;                  // Graphical cut for pi+
    TCutG *GCut_9;                  // Graphical cut for pi-
    TCutG *GCut_14;                 // Graphical cut for p
    
    Bool_t bCut8;
    Bool_t bCut9;
    Bool_t bCut14;

protected:

public:

  HHypHardCutsAlg(Char_t *name_i = NULL, Option_t par[] = NULL);

  virtual ~ HHypHardCutsAlg(void);

  Bool_t          execute();
  Bool_t          init();
  Bool_t          reinit();
  Bool_t          finalize();

  Bool_t          isLepInRich(Int_t iPid,HPidHitData *pHitData);

  ClassDef(HHypHardCutsAlg, 0)  //Hard (banana) cuts, lepton hard cut
};

//============================================================================

#endif
