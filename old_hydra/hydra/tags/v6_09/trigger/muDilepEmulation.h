#ifndef HMUDILEPEMULATION_H
#define HMUDILEPEMULATION_H

#include <TObject.h>
#include <TArrayI.h> 
#include "hreconstructor.h" 
#include "hmatrixcatiter.h"
#include "hmuEmuleptons.h"
#include "hmumommap.h"

class HCategory;
class HIterator;


// ***************************************************************************

class HMUDilepEmulation : public HReconstructor {

// ***************************************************************************


protected:
HCategory *ptlHMULeptons; //! Pointer to the  data category 
HCategory *ptlHMUEMUDiLeptons; //!


 public:
  HMUDilepEmulation();
  HMUDilepEmulation(Text_t *name, Text_t *title);
  ~HMUDilepEmulation();

  HIterator * iterHMULeptons ; //!
  HIterator * iterHMULeptons2 ; //!


  Bool_t init();
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
             



