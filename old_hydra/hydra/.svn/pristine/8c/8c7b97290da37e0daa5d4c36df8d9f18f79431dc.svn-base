//*-- Author: Alberica Toia
//*-- Modified: Joern Wuestenfeld 05/13/2005
//*-- Version: $Id: muEmulation.h,v 1.7 2005-08-31 09:51:01 wuestenf Exp $

#ifndef HMUEMULATION_H
#define HMUEMULATION_H

#include "hreconstructor.h"

class HCategory;
class HIterator;
class HTriggerParMuLep;
class HTriggerParMomMap;
class HSpectrometer;
class HRuntimeDb;
class TArrayI;
class TArrayF;
class TObjArray;
class HEventHeader;
class HTriggerSetup;
// ***************************************************************************

class HMUEmulation : public HReconstructor
{
// ***************************************************************************

protected:
  HCategory *pRich;                 //! Pointer to RICH hit category
  HCategory *pTof;                  //! Pointer to Tof hit category
  HCategory *pShower;               //! Pionter to SHOWER hit category
  HCategory *pLeptons;              //! Pointer to MU lepton category

  HTriggerParMuLep *fTrigPar;       //! Pointer to MU lepton parameter container
  HTriggerParMomMap *fTrigParMom;   //! Pointer to MU momentum map parameter container
  HTriggerSetup *fTriggerSetup;     //! Pointer to trigger setup parameters

  Int_t muDileptons;                //! Minimum number of dileptons for positiv LVL2 trigger
  Int_t muLeptons;                  //! Minimum number of leptons for positiv LVL2 trigger
  Int_t muMaxLeptons;               //! Number of leptons resulting in positiv LVL2 trigger
  Int_t muRichHits;                 //! Minimum number of RICH hits for positiv LVL2 trigger
  Int_t muRichTotalHits;            //! Number of rings in RICH giving a positiv LVL2 trigger
  Int_t muMetaHits;                 //! Minimum number of META hits for positiv LVL2 trigger

  TArrayF *thetaRich[6];            // Theta of RICH hit from the MU
  TArrayF *phiRich[6];              // Phi of RICH hit from MU

  TArrayF *thetaShower[6];          // Theta of SHOWER hit from MU
  TArrayF *phiShower[6];            // Phi of SHOWER hit from MU

  TArrayF *thetaTof[6];             // Theta of TOF hit from MU
  TArrayF *phiTof[6];               // PHI of TOF hit from MU
  TArrayI *flagTof[6];              // Pid of TOF hit from MU

  Int_t nRichHits[6];               // Number of RICH hits per sector
  Int_t nTofHits[6];                // Number of TOF hits per sector
  Int_t nShowerHits[6];             // Number of SHOWER hits per sector
  UInt_t nMetaHits;                 //! Total number of hits in META
  UInt_t nRichTotal;                //! Total n umber of hits in RICH
  UInt_t triggerDecision;			//! Trigger decision, result of matching


  HIterator *iterRich;              //! Iterator over RICH hit category
  HIterator *iterTof;               //! Iterator over TOF hit category
  HIterator *iterShower;            //! Iterator over SHOWER hit category

  TObjArray *pEmuLeptons;           //! Pointer to array for storing lepons
  Int_t nEmuLeptons;                //! Number of found leptons per event
  HLocation loc;                    //! Address used to store leptons in output category
  HEventHeader *pEventHeader;       //! Pointer to event header

  virtual Int_t matching(void);
  virtual void updateEmuLeptons(void);
  void reset(void);

private:
  Bool_t initParContainer(HSpectrometer *spec, HRuntimeDb *rtdb);
  Int_t roundAngle(Float_t fAngle, Int_t iRange, Float_t round=0.5);
  virtual void prepareInput(void);

public:
  HMUEmulation(void);
  HMUEmulation(Text_t *name, Text_t *title);
  virtual ~HMUEmulation(void);

  virtual Bool_t init(void);
  virtual Bool_t reinit(void);
  virtual Bool_t finalize(void);
  virtual Int_t execute(void);

  ClassDef(HMUEmulation,1) // Emulation of the matching unit for real data with MU data.
};

//============================================================================

#endif  // HMUEMULATION_H
