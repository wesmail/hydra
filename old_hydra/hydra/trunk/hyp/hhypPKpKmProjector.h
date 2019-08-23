//*-- AUTHOR : P. Marek
//*-- Modified : 21/8/2008

#ifndef HHYP_PKPKMI0_PROJECTOR_H
#define HHYP_PKPKMI0_PROJECTOR_H

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

/* Double_t Tofino_dEdx_Corr(Float_t AnglePhi, Float_t Sector, Float_t TofELoss, TH1F* Tofino_dEdx_offsets) */
/* { */
/*     // variables */
/*     Int_t Phibin; */
/*     Double_t Tofino_dEdx_pion, Tofino_dEdx_proton; // measured dEdx values at choosen bin */
/*     Double_t Eloss_val_pion_calc   = 0.892;  // nominal dEdx values at choosen bin */
/*     Double_t Eloss_val_proton_calc = 2.2764; // nominal dEdx values at choosen bin */
/*     Double_t Tofino_dEx_pion_ratio, Tofino_dEx_proton_ratio; */

/*     if(Sector != 5) */
/*     { */
/*         Phibin = (Int_t)((AnglePhi-(Sector+1.0)*60.0)/15.0); */
/*     } */
/*     else */
/*     { */
/*         Phibin = (Int_t)(AnglePhi/15.0); */
/*     } */

/*     Tofino_dEdx_pion        = Tofino_dEdx_offsets->GetBinContent(Sector*4+Phibin+1); */
/*     //Tofino_dEdx_proton      = Tofino_dEdx_offsets->GetBinContent(Sector*4+Phibin+1+24); */
/*     //cout << "Tofino_dEdx_pion = " << Tofino_dEdx_pion << ", Tofino_dEdx_proton = " << Tofino_dEdx_proton << endl; */
/*     Tofino_dEx_pion_ratio   = Eloss_val_pion_calc/Tofino_dEdx_pion; */
/*     //Tofino_dEx_proton_ratio = Eloss_val_proton_calc/Tofino_dEdx_proton; */
/*     Float_t TofELoss_corr = TofELoss*Tofino_dEx_pion_ratio; */

/*     return TofELoss_corr; */
/* } */

/*     TFile *file_dEdx_Tofino   = TFile::Open("/d/hades12/data/schmah_may07/Tofino_calib/Tofino_dEdx_shifts.root"); */
/*     TH1F* Tofino_dEdx_offsets = (TH1F*)file_dEdx_Tofino->Get("Tofino_dEdx_offsets"); */


class HHypPKpKmProjector:public HHypBaseAlgorithm
{

private:
  HEnergyLossCorrPar  enLossCorr;
  TNtuple        *pkpkm;
  Int_t           simuflag;
  HCategory      *simCat;
  HCategory      *CatTrackCandSim;
  TCutG          *p_CutG, *pip_CutG, *p_pip_CutG;
  TString        paramFile;
  Bool_t         gcutFlag;

protected:

public:

  HHypPKpKmProjector(Char_t *name_i = NULL, Option_t par[] = NULL);
  
  virtual ~ HHypPKpKmProjector(void);

  Bool_t          execute();
  Bool_t          init();
  Bool_t          reinit();
  Bool_t          finalize();
  Bool_t          IsOpposit(Short_t, Short_t);
  Bool_t          SetParamFile(TString pFile);
  Float_t         calcVertex(HPidTrackCand *p1, HPidTrackCand *p2,
        TVector3 *vertex, TVector3 *distance);


  ClassDef(HHypPKpKmProjector, 0)  // creates ntuples for PP reactions
};

//============================================================================

#endif
