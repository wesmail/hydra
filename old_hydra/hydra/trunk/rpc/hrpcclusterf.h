#ifndef HRPCCLUSTERF_H
#define HRPCCLUSTERF_H

#include "hreconstructor.h"
#include "hlocation.h"

class HCategory;

class HRpcClusterF : public HReconstructor {
protected:
  HCategory *pHitCat;              // pointer to the hit data
  HCategory *pClusterCat;          // pointer to the cluster data
  HCategory *pGeantRpcCat;         // pointer to the GeantRpc data
  HLocation loc;                   // location for hit
  HLocation loc_cluster;           // location for cluster

  Int_t  maxcells;                 // Maximum number of cells

  Bool_t simulation;               // Flag to decide whether simulation(1) or analisis(0)

public:
  HRpcClusterF();
  HRpcClusterF(const Text_t* name,const Text_t* title);
  ~HRpcClusterF();
  void initParContainer();
  Bool_t init();
  Bool_t finalize() { return kTRUE; }
  Int_t execute();
public:
  ClassDef(HRpcClusterF,0)      // Cluster Finder hit->cluster for RPC data
};

#endif /* !HRPCCLUSTERF_H */

