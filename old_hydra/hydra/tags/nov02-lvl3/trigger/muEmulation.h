#ifndef HMUEMULATION_H
#define HMUEMULATION_H

#include <TObject.h>
#include <TArrayI.h> 
#include "hreconstructor.h" 
#include "hmatrixcatiter.h"
#include "hmuEmuleptons.h"

class HCategory;
class HIterator;
class HTriggerParMuLep;
class HTriggerParMomMap;
class HSpectrometer;
class HRuntimeDb;
// ***************************************************************************

class HMUEmulation : public HReconstructor {

// ***************************************************************************


protected:
HCategory *ptlHMatchURich; 
HCategory *ptlHMatchUTof; 
HCategory *ptlHMatchUShower;
HCategory *ptlHMUEMULeptons;

HTriggerParMuLep *fTrigPar; 
HTriggerParMomMap *fTrigParMom;

 public:
  HMUEmulation();
  HMUEmulation(Text_t *name, Text_t *title);
  ~HMUEmulation();

  HIterator * iterHMatchURich ; //!
  HIterator * iterHMatchUTof; //!
  HIterator * iterHMatchUShower; //!

  Bool_t init();
  void initParContainer(HSpectrometer *,HRuntimeDb *);
  Bool_t finalize();
  Int_t execute();
  Int_t roundAngle(Float_t, Int_t, Float_t); 
 
  HMUEMULeptons  *pEmuLeptons;
  Int_t nEmuLeptons;

  HCategory* getMatchURichCat() { return ptlHMatchURich; }
  HCategory* getMatchUTofCat() { return ptlHMatchUTof; }
  HCategory* getMatchUShowerCat() { return ptlHMatchUShower; }

  HLocation loc;
  void updateEmuLeptons();

  ClassDef(HMUEmulation,1) 
};

//============================================================================
 
#endif  // HMUEMULATION_H
             




