#ifndef HRPCDIGITIZER_H
#define HRPCDIGITIZER_H
using namespace std;
#include "hreconstructor.h"
#include <iostream> 
#include <iomanip>
#include "hlocation.h"

class HIterator;
class HCategory;
class HRpcDigiPar;

class HRpcDigitizer : public HReconstructor {

private:
  HLocation fLoc;                       //! Location for new object
  HCategory* fGeantCat;                 //! Pointer to Geant data category
  HCategory* fCalCat;                   //! Pointer to cal data category
  HRpcDigiPar *fDigiPar;                //! Digitization parameters
  HIterator* iterGeant;                 //! Iterator over GeantCell category
  HIterator* iterRpcCal;                // Iterator over HRpcCalSim category
public:
  HRpcDigitizer(void);
  HRpcDigitizer(Text_t* name,Text_t* title);
  ~HRpcDigitizer(void);
  void initParContainer();
  Bool_t init(void);
  Bool_t finalize(void) {return kTRUE;}
  Int_t execute(void);

public:
  ClassDef(HRpcDigitizer,0) //Digitizer of RPC data.
};

#endif









