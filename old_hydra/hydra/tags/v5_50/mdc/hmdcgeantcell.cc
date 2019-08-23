//*-- Author : A. Nekhaev
//*-- Modified: 05/08/99 by R. Holzmann
//*-- Modified: 07/06/99 by Ilse Koenig


////////////////////////////////////////
//HMdcGeantCell
//
// container to store temporally cell data for
// MdcDigitizer
////////////////////////////////////////

#include "hmdcgeantcell.h"


void HMdcGeantCell::clear() {
  sector = module = layer = cell = fNumHits = 0;
  for(int i=0;i<15;i++) {
    fMinDist[i]=0.F;
    fTimeFlight[i]=0.F;
    nTrack[i]=0;
    fImpactAngle[i]=0.F;
    flagCutEdge[i]=kFALSE;
  }
}

ClassImp(HMdcGeantCell)
