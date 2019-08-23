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
#include "hmdccal2par.h"
#include "hmdcgetcontainers.h"

//*-- Author : G.Agakichiev
//*-- Modified: 28.07.2002 by V.Pechenov 
//*-- Modified: 07.05.2002 by V.Pechenov 

ClassImp(HMdcTrackFitter)

Bool_t HMdcTrackFitter::init() {
  fitPar=(HMdcTrackFitPar*)gHades->getRuntimeDb()->
      getContainer("MdcTrackFitPar");
  if(version != 0) {
    fCal2ParSim=(HMdcCal2ParSim*)gHades->getRuntimeDb()->
        getContainer("MdcCal2ParSim");
    fCal2Par=(HMdcCal2Par*)gHades->getRuntimeDb()->
        getContainer("MdcCal2Par");
  }
  else {
    fCal2ParSim=0;
    fCal2Par=0;
  }
  return kTRUE;
}

Bool_t HMdcTrackFitter::reinit(void) {
  if(version!=0 && (!HMdcGetContainers::isInited(fCal2ParSim)
                 || !HMdcGetContainers::isInited(fCal2Par))) return kFALSE;
  if( !HMdcGetContainers::isInited(fitPar) ) return kFALSE;
  return kTRUE;
}

void HMdcTrackFitter::calcTdcErrors(Double_t* par) {
  if(fCal2Par == 0) {
    for(int time = firstTime; time < nClTimes; time++) {
      errTdcTime[time]=1.;
    }
  } else {
    Double_t distance,alpha;
    Double_t xV = par[0];
    Double_t yV = par[1];
    Double_t zV = parD1-parA1*xV-parB1*yV; //zVertex;
    Double_t xPlane = par[2];
    Double_t yPlane = par[3];
    Double_t zPlane = parD2-parA2*xPlane-parB2*yPlane; //!!! zInitPlane;  
    for(int time = firstTime; time < nClTimes; time++) {
      distance=fSizeCellsLayer[time]->getImpact(xV, yV, zV,
        xPlane, yPlane, zPlane, lCell[time], alpha);
      distance=fCal2Par->calcDistance(sector,lModule[time],
          alpha,tdcTime[time]-par[4]);
      if(distance<0) distance=0.;
      errTdcTime[time]=
          fCal2ParSim->calcTimeErr(sector,lModule[time],alpha,distance);
    }
  }
}
