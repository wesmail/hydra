//*-- AUTHOR : Vladimir Pechenov

////////////////////////////////////////////////////////////////
// HMdcClusFit
//
//  Container class keep fit information for debuging.
//
////////////////////////////////////////////////////////////////

#include "hmdcclusfit.h"
#include <iostream.h>

ClassImp(HMdcClusFit)

void HMdcClusFit::print() {
  printf("-HMdcClusFit- Sec.%i",sec+1);
  if(mod<0) printf(" Seg.%i:",seg+1);
  else printf(" Mod.%i:",mod+1);
  printf("    Auth.%i; Dist-time vr.%i; Fit type %i; Num.of param.=%i\n",
      fitAuthor,distTimeVer,fitType,nParam);
  printf(" Fun.=%g Nwires=%i, Nwires(WT>WTmin)=%i, Nlayers.(WT>WTmin)=%i",
      functional,indl-indf+1,numOfWires,numOfLayers);
  printf(" Niter.=%i TOF=%g\n Track: (%f,%f,%f)-(%f,%f,%f)\n",
      numIter,timeOff,x1,y1,z1,x2,y2,z2);
}

