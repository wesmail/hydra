//*-- Author : A. Nekhaev
//*-- Modified: 05/08/99 by R. Holzmann
//*-- Modified: 07/06/99 by Ilse Koenig


////////////////////////////////////////
//HMdcGeantCell
//
// adapted from /u/halo/packages/abase/new 
////////////////////////////////////////

#include "hmdcgeantcell.h"


void HMdcGeantCell::clear() {
  sector = module = layer = cell = fNumHits = 0;
  for(int i=0;i<15;i++) {
    fMinDist[i]=0.F;
    fTimeFlight[i]=0.F;
    nTrack[i]=0;
    fImpactAngle[i]=0.F;
    fOldMinDist[i]=0.F;
    flagCutEdge[i]=kFALSE;
    fOldImpactAngle[i]=0.F;

  }
}

ClassImp(HMdcGeantCell)
