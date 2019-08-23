//*-- AUTHOR : M. Wisniowski 
//*-- Modified : 13/9/2005

#ifndef HHYP_PPPI0_PROJECTOR_H
#define HHYP_PPPI0_PROJECTOR_H

#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include "htofhit.h"

#include "hevent.h"
#include "heventheader.h"
#include "hdetector.h"
#include "hratreeext.h"
#include "hcategory.h"
#include "hmatrixcategory.h"
#include "hlinearcategory.h"
#include "hlinearcatiter.h"
#include "hlocation.h"
#include "hiterator.h"
#include "hdebug.h"
#include "hades.h"

#include "hhyplist.h"
#include "hhypchannel.h"
#include "hypinfodef.h"

#include "hsplinetrack.h"
#include "hrktrackB.h"

#include "hmessagemgr.h"
#include "hpidhitdata.h"
#include "hpidtrackcand.h"
#include "hpidcandidate.h"
#include "hpidtrackcand.h"
#include "hpidphysicsconstants.h"
#include "hpidfl.h"
#include "piddef.h"

#include "hcategory.h"
#include "kickdef.h"
#include "hmdctrackgdef.h"
#include "hmdcdef.h"

#include "hruntimedb.h"
#include <hrktrackB.h>
#include <TObject.h>
#include <TArrayI.h>
#include <TNtuple.h>
#include <TFile.h>
#include "hhypbasealgorithm.h"
#include <hpidtrackcandsim.h>
#include <hpidgeanttrackset.h>
#include <hgeantkine.h>
#include <TCutG.h>
#include <henergylosscorrpar.h>



class HHypPPPi0Projector:public HHypBaseAlgorithm
{

private:
  HEnergyLossCorrPar  enLossCorr;
  TNtuple        *pp;
  Int_t           simuflag;
  HCategory      *simCat;
  HCategory      *CatTrackCandSim;
  TCutG          *p_CutG, *pip_CutG, *p_pip_CutG;
  TString        paramFile;
  Bool_t         gcutFlag;

protected:

public:

  HHypPPPi0Projector(Char_t *name_i = NULL, Option_t par[] = NULL);
  
  virtual ~ HHypPPPi0Projector(void);

  Bool_t          execute();
  Bool_t          init();
  Bool_t          reinit();
  Bool_t          finalize();
  Bool_t          IsOpposit(Short_t, Short_t);
  Bool_t          SetParamFile(TString pFile);
  Float_t         calcVertex(HPidTrackCand *p1, HPidTrackCand *p2,
        TVector3 *vertex, TVector3 *distance);


  ClassDef(HHypPPPi0Projector, 0)  // creates ntuples for PP reactions
};

//============================================================================

#endif
