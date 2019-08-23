#ifndef HMUDILEPEMULATION_H
#define HMUDILEPEMULATION_H

#include <TObject.h>
#include <TArrayI.h> 
#include "hreconstructor.h" 
#include "hmatrixcatiter.h"
#include "hmuEmuleptons.h"

class HCategory;
class HIterator;
class HTriggerParMuDilep;
class HSpectrometer;
class HRuntimeDb;
// ***************************************************************************

class HMUDilepEmulation : public HReconstructor {

// ***************************************************************************
 public:
  HCategory *ptlHMULeptons;  
  HCategory *ptlHMUEMUDiLeptons; 
  
  HTriggerParMuDilep *fTrigParDil; 
 
  HMUDilepEmulation();
  HMUDilepEmulation(Text_t *name, Text_t *title);
  ~HMUDilepEmulation();

  HIterator * iterHMULeptons ; //!
  HIterator * iterHMULeptons2 ; //!

  Float_t thetaRich[600];
  Float_t phiRich[600];
  Float_t mom[600];
  Int_t flag[600];
  Int_t nLep;

  void prepareInput();
  Int_t searchDilep();

  Bool_t init();
  void initParContainer(HSpectrometer *,HRuntimeDb *);
  Bool_t finalize();
  Int_t execute();
  
  HMUEMUDiLeptons  *pEmuDiLeptons;
  Int_t nEmuDiLeptons;

  HCategory* getMULeptons() { return ptlHMULeptons; }

  HLocation loc;
  void updateEmuDiLeptons();

  ClassDef(HMUDilepEmulation,1) 
};

//============================================================================
 
#endif  // HMUDILEPEMULATION_H
             








