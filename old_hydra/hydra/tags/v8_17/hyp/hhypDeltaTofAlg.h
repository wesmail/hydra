//*-- AUTHOR : I. Froehlich
//*-- Modified : 07.07.2005 by T. Perez

#ifndef HHYP_DELTATOF_H
#define HHYP_DELTATOF_H

#include <TObject.h>
#include <TArrayI.h>
#include <TNtuple.h>

#include <stdlib.h>
#include <iostream>
#include <cmath>

#include "hades.h"

#include "hevent.h"
#include "hhypbasealgorithm.h"
#include "hhyplistfiller.h"
#include "hypinfodef.h"

#include "hmdcseg.h"
#include "hmdctrkcand.h"
#include "hkicktrack.h"
#include "hsplinetrack.h"
#include "hmetamatch.h"
#include "hmdcdef.h"
#include "hmdctrackgdef.h"
#include "hdebug.h"
#include "hades.h"
#include "hiterator.h"
#include "hruntimedb.h"
#include "hevent.h"
#include "hcategory.h"
#include "kickdef.h"
#include "hmessagemgr.h"

#include "hsplinetrack.h"
#include "hrktrackB.h"

#include "hlinearcategory.h"
#include "hpidhitdata.h"
#include "hpidparticle.h"
#include "hpidcandidate.h"
#include "hpidtrackcand.h"
#include "hpidphysicsconstants.h"
#include "hpidfl.h"
#include "piddef.h"


const Double_t  c = 299.792458; // units [mm/ns]

// ***************************************************************************

class HHypDeltaTofAlg:public HHypBaseAlgorithm
{

  // ***************************************************************************
private:

  TNtuple       * qa;

  Bool_t          getTrackInfo(Int_t momIndex1, Int_t momIndex2);
  // Does all that complicated procedure to follow down link until it reaches the 
  // propper TrackClass and retrieves TOF (tof_exp) and PathLength .

protected:

  // delta tof 
  Double_t        dtof;
  Double_t        sigmaDeltaTof;

  Double_t        tof1_exp, tof1_theo, beta1_th, pathLength1;
  Double_t        tof2_exp, tof2_theo, beta2_th, pathLength2;
  Double_t        deltaTofTheo[10], deltaTofExp[10], deltaTOF[10];
  Double_t        deltaTofTheoError[10], deltaTofExpError[10];

  // Objects to get tracking
  Int_t           hyppid1, hyppid2;
  HPidParticle   *particle1;
  HPidParticle   *particle2;

  TLorentzVector  partHyp1, partHyp2;
  TVector3        partHyp1mom, partHyp2mom;
  Double_t        mass1, mass2;

  Int_t           momIndex1, momIndex2;
  Int_t           trackIndex1, trackIndex2;

public:

  HHypDeltaTofAlg(Char_t *name_i = NULL, Option_t par[] = NULL);

  virtual ~ HHypDeltaTofAlg(void);

  Bool_t          execute();
  Bool_t          init();
  Bool_t          reinit();
  Bool_t          finalize();

  ClassDef(HHypDeltaTofAlg, 0) //The DeltaTof algorithm
};

//============================================================================

#endif
