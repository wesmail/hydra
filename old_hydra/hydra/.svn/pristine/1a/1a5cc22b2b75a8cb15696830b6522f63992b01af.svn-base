#ifndef HMDCCALIBRATER1_H
#define HMDCCALIBRATER1_H

#include "hreconstructor.h"
#include "hlocation.h"
#include "TRandom.h"
#include "hstartdef.h"

class HCategory;
class HIterator;
class HMdcCalParRaw;
class HMdcLookupGeom;
class HMdcTimeCut;

class HMdcCalibrater1 : public HReconstructor {
protected:
  HCategory* rawCat;       //! pointer to the raw data
  HCategory* calCat;       //! pointer to the cal data
  HCategory* startHitCat;  //! pointer to the start cal data
  Bool_t StartandCal;      // switch between Cal&&Start,noCal&&Start,noCal&&noStart
  Bool_t NoStartandNoCal;  // switch between Cal&&Start,noCal&&Start,noCal&&noStart
  Bool_t NoStartandCal;    // switch between Cal&&Start,noCal&&Start,noCal&&noStart
  Bool_t setTimeCut;       // switch on/off cuts on time1, time2 and time2-time1

  HLocation loc;           //! location for new object.
  HIterator* iter;         //! iterator on raw data.
  HIterator* iterstart;    //! iterator on start data.

  HMdcCalParRaw* calparraw;//! calibration parameters
  HMdcLookupGeom* lookup;  //! lookup table for mapping
  HMdcTimeCut* timecut;    //! container for cuts on time1,time2,time2-time1

public:
  HMdcCalibrater1(void);
  HMdcCalibrater1(Text_t* name,Text_t* title,Int_t vers=1,Int_t cut=1);

  ~HMdcCalibrater1(void);
  Bool_t init(void);
  void setParContainers(void);
  Bool_t finalize(void) {return kTRUE;}
  Float_t getstarttime();
  Int_t execute(void);
  ClassDef(HMdcCalibrater1,0) // Calibrater raw->cal1 for Mdc data
};

#endif /* !HMDCCALIBRATER1_H */

