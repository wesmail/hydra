#ifndef HRPCDIGITIZER_H
#define HRPCDIGITIZER_H
using namespace std;
#include "hreconstructor.h"
#include <iostream> 
#include <iomanip>
#include "hlocation.h"

class HIterator;
class HCategory;
class HRpcGeomCellPar;
class HRpcDigiPar;

class HRpcDigitizer : public HReconstructor {

private:
  HLocation fLoc;                       // Location for new object
  HCategory* fGeantRpcCat;              // Pointer to Rpc Geant data category
  HCategory* fCalCat;                   // Pointer to Cal  data category
  HCategory* fKineCat;                  // Pointer to Kine data category
  HRpcGeomCellPar *fGeomCellPar;        // Geometrical parameters
  HRpcDigiPar *fDigiPar;                // Digitization parameters
  HIterator* iterGeantRpc;              // Iterator over catRpcGeantRaw category
  HIterator* iterRpcCal;                // Iterator over HRpcCalSim category
public:
  HRpcDigitizer(void);
  HRpcDigitizer(const Text_t* name,const Text_t* title);
  ~HRpcDigitizer(void);
  void initParContainer();
  Bool_t init(void);
  Bool_t finalize(void) {return kTRUE;}
  Int_t execute(void);

public:
  ClassDef(HRpcDigitizer,0) //Digitizer of RPC data.
};

#endif









