#ifndef HMUEMULATION_H
#define HMUEMULATION_H

#include <TObject.h>
#include <TArrayI.h> 
#include "hreconstructor.h" 
#include "hmatrixcatiter.h"
#include "hmuEmuleptons.h"
#include "hmumommap.h"

class HCategory;
class HIterator;


// ***************************************************************************

class HMUEmulation : public HReconstructor {

// ***************************************************************************


protected:
HCategory *ptlHMatchURich; //! Pointer to the  data category 
HCategory *ptlHMatchUTof; //! 				   
HCategory *ptlHMatchUShower; //!
HCategory *ptlHMUEMULeptons; //!

HMUMomMap *fMUMomMap; //! MU mom table container (no proper param handling!)

 public:
  HMUEmulation();
  HMUEmulation(Text_t *name, Text_t *title);
  ~HMUEmulation();

  HIterator * iterHMatchURich ; //!
  HIterator * iterHMatchUTof; //!
  HIterator * iterHMatchUShower; //!

  Bool_t init();
  Bool_t finalize();
  Int_t execute();
  
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
             



