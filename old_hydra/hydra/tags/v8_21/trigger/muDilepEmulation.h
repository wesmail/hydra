//*-- Author: Alberica Toia
//*-- Modified: Joern Wuestenfeld 05/10/2005
//*-- Version: $Id: muDilepEmulation.h,v 1.7 2009-07-15 11:34:06 halo Exp $

#ifndef HMUDILEPEMULATION_H
#define HMUDILEPEMULATION_H


#include "hreconstructor.h" 
#include "hmatrixcatiter.h"

class HCategory;
class HIterator;
class HTriggerParMuDilep;
class HSpectrometer;
class HRuntimeDb;
class TObjArray;
// ***************************************************************************

class HMUDilepEmulation : public HReconstructor
{
// ***************************************************************************
 protected:
  HCategory *ptlHMULeptons;  
  HCategory *ptlHMUEMUDiLeptons; 
  
  HTriggerParMuDilep *fTrigParDil; 

  HIterator * iterHMULeptons ; //!
  HIterator * iterHMULeptons2 ; //!

  Float_t thetaRich[600];
  Float_t phiRich[600];
  Float_t mom[600];
  Int_t flag[600];
  Int_t nLep;

  TObjArray *pEmuDiLeptons;
  Int_t nEmuDiLeptons;

  HLocation loc;

  void updateEmuDiLeptons(void);
  void prepareInput(void);
  Int_t searchDilep(void);
  Bool_t initParContainer(HSpectrometer *spec,HRuntimeDb *rtdb);
	void reset(void);

 public:
  HMUDilepEmulation(void);
  HMUDilepEmulation(const Text_t *name,const Text_t *title);
  virtual ~HMUDilepEmulation(void);

  virtual Bool_t init(void);
  virtual Bool_t finalize(void);
  virtual Int_t execute(void);
  
  HCategory* getMULeptons(void) { return ptlHMULeptons; }

  ClassDef(HMUDilepEmulation,1) 
};

//============================================================================
 
#endif  // HMUDILEPEMULATION_H
