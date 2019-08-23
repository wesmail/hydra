#ifndef HTOFINODITIZER_H
#define HTOFINODITIZER_H
using namespace std;
#include "hreconstructor.h"
#include <iostream>
#include <iomanip>
#include "hlocation.h"
#include "htofinodigitpar.h"
#include "TObjArray.h"

#define MAXHIT 10

class HIterator;
class HCategory;
class HLinearCategory;
class HTofinoDigitPar;
class HGeantTof;

class HTofinoDigitizer : public HReconstructor {

private:
  HLocation fLoc;               //! Location for new object
  HLocation ffLoc;              //! Location for new object
  HCategory* fGeantCat;         //! Pointer to HGeant data category
  HCategory* fCalCat;           //! Pointer to Cal data category
  HLinearCategory* fGeantKineCat;//! Pointer to GeantKine data category
  HIterator* iterGeant;         //! Iterator over HGeantTof category
  HIterator* iterCal;           //! Iterator over HTofinoCalSim category
  HTofinoDigitPar *pTofinoDigitPar; //! Digit parameters
  TObjArray* array;             //! temporary array for HTofinoCalSim objects
  Int_t      numTracks[6][4][MAXHIT]; //! temporary array
  Int_t      ctTracks [6][4];         //! temporary array
  Float_t geaTofTracks[6][4][MAXHIT]; //! temporary array
  Float_t tdcTofTracks[6][4][MAXHIT]; //! temporary array

  //--------------------------------------------------------------------------
  Int_t storeFirstTrack;         //! flag:
                                 //        0  = realistic (secondaries included)
                                 //        1 primary particle is stored
                                 //        2 (default) the track number entering the tof in SAME SECTOR is stored , not the
                                 //          secondaries created in TOF itself (delta electrons etc)
                                 //        3 as 2 but condition on SAME CELL
  Bool_t debug;                  //! change stored tracknumbers (when storeFirstTrack!=0)
  static HTofinoDigitizer* pTofinoDigi;//! pointer to this

  Int_t findFirstHitInTofino(HGeantTof* pOld,HGeantTof** pNew,Int_t* count);

public:
  HTofinoDigitizer(void);
  HTofinoDigitizer(const Text_t* name,const Text_t* title);
  ~HTofinoDigitizer(void);
  Bool_t initParContainer();
  Bool_t init(void);
  Bool_t finalize(void);
  void   fillArray(void);
  void   doFinalCheckOnArray(void);
  void   fillOutput(void);
  static HTofinoDigitizer* getTofinoDigtizer(){return pTofinoDigi;}
  void   setStoreFirstTrack(Int_t flag) {storeFirstTrack=flag;}
  void   setDebug(Bool_t flag)          {debug=flag;}
 Int_t   execute(void);

public:
  ClassDef(HTofinoDigitizer,0) // Digitizer for TOFINO HGeant hits
};

#endif
