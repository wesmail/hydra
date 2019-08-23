#include "hmdctrackfitter.h"
#include "hdebug.h"
#include "hades.h"
//#include <iostream.h>
#include "TStopwatch.h"
#include "TMath.h"
#include "hmdctrackfitpar.h"
#include "hruntimedb.h"
#include "hmdcsizescells.h"
#include "hgeomvector.h"
#include "hmdccal2parsim.h"
#include "hmdcgetcontainers.h"

//*-- Author : G.Agakichiev
//*-- Modified: 07.05.2002 by V.Pechenov 

ClassImp(HMdcTrackFitter)

Bool_t HMdcTrackFitter::init() {
  fitPar=(HMdcTrackFitPar*)gHades->getRuntimeDb()->
      getContainer("MdcTrackFitPar");
  if(version != 0)
    fCal2ParSim=(HMdcCal2ParSim*)gHades->getRuntimeDb()->
        getContainer("MdcCal2ParSim");
  else fCal2ParSim=0;
  return kTRUE;
}

Bool_t HMdcTrackFitter::reinit(void) {
  if(version!=0 && !HMdcGetContainers::isInited(fCal2ParSim)) return kFALSE;
  if( !HMdcGetContainers::isInited(fitPar) ) return kFALSE;
  return kTRUE;
}
