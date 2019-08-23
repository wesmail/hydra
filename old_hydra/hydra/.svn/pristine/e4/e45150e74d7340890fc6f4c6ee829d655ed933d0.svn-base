#ifndef HRPCHITF_H
#define HRPCHITF_H

#include "hreconstructor.h"
#include "hlocation.h"

class HCategory;
class HIterator;
class HRpcGeomCellPar;
class HRpcDigiPar;

class HRpcHitF : public HReconstructor {
protected:
  HCategory *pCalCat;              // pointer to the cal data
  HCategory *pHitCat;              // pointer to the hit data
  HCategory *pGeantRpcCat;         // pointer to the GeantRpc data
  HRpcDigiPar *pDigiPar;           // Digitization parameters (needed for vprop)
  HIterator *iter;                 // iterator on cal data.
  HRpcGeomCellPar* pGeomCellPar;   // rpc cells geometry parameters
  HLocation loc;                   // location

  Bool_t simulation;               // Flag to decide whether simulation(1) or analisis(0)

public:
  HRpcHitF();
  HRpcHitF(const Text_t* name,const Text_t* title);
  ~HRpcHitF();
  void initParContainer();
  Bool_t init();
  Bool_t finalize() { return kTRUE; }
  Int_t execute();
public:
  ClassDef(HRpcHitF,0) // Hit Finder cal->hit for RPC data
};

#endif /* !HRPCHITF_H */

