// File: hclusterselectorsim.h
//
// Author: Laura Fabbietti <L.Fabbietti@physik.tu-muenchen.de>
// Last update by Laura Fabbietti: 02/08/08 20:52:31
//
#ifndef HCLUSTERSELECTORSIM_H
#define HCLUSTERSELECTORSIM_H

#include "hreconstructor.h"
#include "hlocation.h"
#include "hparset.h"
#include <TFile.h>
#include <TH2.h>
#include <TNtuple.h>
#include "hclusterselector.h"
class HCategory;
class HIterator;
class HRichCal;
class HPhotonCluster;
class HRichGeometryPar;

class HClusterSelectorSim : public HClusterSelector {
public:
    HClusterSelectorSim(Text_t *name, Text_t *title,  Int_t hitMin, 
		     Int_t secShadow,
		     Float_t minTheta1, Float_t maxTheta1, Float_t minTheta2,
		     Float_t maxTheta2, 
		     Int_t secEdge1,Int_t secEdge2,Int_t minHitsecEdge1,
		     Int_t minHitsecEdge2): HClusterSelector(name, title, hitMin, 
		     secShadow,minTheta1, maxTheta1, minTheta2,
		      maxTheta2, secEdge1, secEdge2, minHitsecEdge1,
							     minHitsecEdge2){};
    ~HClusterSelectorSim(){};
    
    Bool_t init();
    ClassDef(HClusterSelectorSim,1)
};
#endif /* !HCLUSTERSELECTORSIM_H */

