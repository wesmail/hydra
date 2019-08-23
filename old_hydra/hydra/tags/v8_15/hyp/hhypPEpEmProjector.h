//*-- AUTHOR : M. Wisniowski 
//*-- Modified : 5/12/2007

#ifndef HHYP_PEPEM_PROJECTOR_H
#define HHYP_PEPEM_PROJECTOR_H

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
#include "hpidparticle.h"
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
#include <hpidparticlesim.h>
#include <hpidgeanttrackset.h>
#include <hgeantkine.h>
#include <henergylosscorrpar.h>
#include <TCutG.h>



class HHypPEpEmProjector:public HHypBaseAlgorithm
{

private:

  TNtuple        *pp;
  HEnergyLossCorrPar  enLossCorr;
  Int_t           simuflag;
  HCategory      *simCat;
  HCategory      *CatPartSim;
  TString        paramFile;
  Bool_t         gcutFlag;

protected:

public:

  HHypPEpEmProjector(char *name_i = NULL, Option_t par[] = NULL);
  
  virtual ~ HHypPEpEmProjector(void);

  Bool_t          execute();
  Bool_t          init();
  Bool_t          reinit();
  Bool_t          finalize();
  Bool_t          IsOpposit(Short_t, Short_t);
  Bool_t          SetParamFile(TString pFile);
  Float_t HHypPEpEmProjector::calcVertex(HPidParticle *p1, HPidParticle *p2,
        TVector3 *vertex, TVector3 *distance);

  ClassDef(HHypPEpEmProjector, 0)  // creates ntuples for PP reactions
};

//============================================================================

#endif
