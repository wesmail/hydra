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
 public:
  HCategory *pRich; 
  HCategory *pTof; 
  HCategory *pShower;
  HCategory *pLeptons;
  
  HTriggerParMuLep *fTrigPar; 
  HTriggerParMomMap *fTrigParMom;
  
 HMUEmulation();
 HMUEmulation(Text_t *name, Text_t *title);
 ~HMUEmulation();
 
 Float_t thetaRich[6][600];
 Float_t phiRich[6][600];

 Float_t thetaShower[6][600];
 Float_t phiShower[6][600];

 Float_t thetaTof[6][600];
 Float_t phiTof[6][600];
 Int_t flagTof[6][600];
 
 Int_t nRichHits[6];
 Int_t nTofHits[6];
 Int_t nShowerHits[6];
 
 HIterator * iterRich ; //!
 HIterator * iterTof; //!
 HIterator * iterShower; //!
 
 Bool_t init();
 void initParContainer(HSpectrometer *,HRuntimeDb *);
 Bool_t finalize();
 Int_t execute();
 Int_t roundAngle(Float_t, Int_t, Float_t); 
 
 HMUEMULeptons  *pEmuLeptons;
 Int_t nEmuLeptons;
 
 /*   HCategory* getMatchURichCat() { return pRich; } */
 /*   HCategory* getMatchUTofCat() { return pTof; } */
 /*   HCategory* getMatchUShowerCat() { return pShower; } */
 
 HLocation loc;
 void updateEmuLeptons();
 void prepareInput();
 Int_t matching();
 
 ClassDef(HMUEmulation,1) 
   };
   
//============================================================================
 
#endif  // HMUEMULATION_H
             




