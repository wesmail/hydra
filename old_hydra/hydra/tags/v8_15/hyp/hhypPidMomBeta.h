//*-- AUTHOR : M. Wisniowski 
//*-- Modified : 16/11/2007

#ifndef HHYP_HHYPPIDMOMBETA
#define HHYP_HHYPPIDMOMBETA

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
#include <TCutG.h>
#include <henergylosscorrpar.h>



class HHypPidMomBeta:public HHypBaseAlgorithm
{

private:
  Int_t           simuflag;
  HCategory      *simCat;
  HCategory      *CatPartSim;
  TCutG          *p_CutG, *pip_CutG;
  TString         paramFile;
  Bool_t          gcutFlag;
  HEnergyLossCorrPar  enLossCorr;
  TNtuple        *qa;

protected:

public:

  HHypPidMomBeta(char *name_i = NULL, Option_t par[] = NULL);
  
  virtual ~ HHypPidMomBeta(void);

  Bool_t          execute();
  Bool_t          init();
  Bool_t          reinit();
  Bool_t          finalize();
  Bool_t          checkPID(float momentum, float beta, int ID);
  Bool_t          SetParamFile(TString pFile);


  ClassDef(HHypPidMomBeta, 0)  // creates ntuples for PP reactions
};

//============================================================================

#endif
