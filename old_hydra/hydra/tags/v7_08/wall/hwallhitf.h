#ifndef HWALLHITF_H
#define HWALLHITF_H
using namespace std;
#include "hreconstructor.h"
#include <iostream> 
#include <iomanip>
#include "hlocation.h"
#include "hwallcalpar.h"  

class HIterator;
class HCategory;
class HWallCalPar;
class HRuntimeDb;
class HWallHit;
class HWallRaw;


class HWallHitF : public HReconstructor {
protected:
  //private://FK//
  HLocation fLoc;             //! Location for new object
  //HCategory* fCalCat;       //! Pointer to cal data category
  HCategory* fRawCat;         //! Pointer to raw data category
  HCategory* fHitCat;         //FK//???????????????????????????
  HIterator* iter;            //! Iterator over Raw category
  HWallCalPar *pWallCalPar; //! Calibration parameters 
  HCategory *fStartHitCat; //! Category with start hits
  Bool_t fWallSimulation;      //FK//

  virtual void fillHit(HWallHit *, HWallRaw *);

public:
  HWallHitF(void);
  HWallHitF(Text_t* name,Text_t* title);
  ~HWallHitF(void);
  void initParContainer();
  Bool_t init(void);
  Bool_t finalize(void) {return kTRUE;}
  Int_t execute(void);
 
  ClassDef(HWallHitF,0) // Calibrater for Wall hit data
};

#endif
