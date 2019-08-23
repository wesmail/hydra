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
class HMdcCutStat;
class HMdcRaw;
class HMdcCal1;
class HMdcCalParTdc;

class HMdcCalibrater1 : public HReconstructor {
protected:
  HCategory* rawCat;       //! pointer to the raw data
  HCategory* calCat;       //! pointer to the cal data
  HCategory* startHitCat;  //! pointer to the start cal data
  HMdcRaw* raw;            //! pointer to raw data word
  HMdcCal1* cal;           //! pointer to cal1 data word

  Bool_t StartandCal;      // switch between Cal&&Start,noCal&&Start,noCal&&noStart
  Bool_t NoStartandNoCal;  // switch between Cal&&Start,noCal&&Start,noCal&&noStart
  Bool_t NoStartandCal;    // switch between Cal&&Start,noCal&&Start,noCal&&noStart
  Bool_t setTimeCut;       // switch on/off cuts on time1, time2 and time2-time1
  Bool_t hasPrinted;       // flag is set if printStatus is called
  Int_t  embedding;        // flag is set if real data should be embedded into simulation data
  HLocation loc;           //! location for new object.
  HIterator* iter;         //! iterator on raw data.
  HIterator* iterstart;    //! iterator on start data.

  HMdcCalParRaw* calparraw;//! calibration parameters
  HMdcLookupGeom* lookup;  //! lookup table for mapping
  HMdcTimeCut* timecut;    //! container for cuts on time1,time2,time2-time1
  HMdcCutStat* cutStat;    //! container for statistics on cuts

  static Float_t globalOffset[4]; //  global offset per plane
  static Float_t globalSlope;     //  global tdc slope
  static Int_t countNrWiresPerMod[6][4]; // counter array for wires /module/event
  Int_t cuts[4];          // counter array passed/not passed time and multiplicity cuts

public:
  HMdcCalibrater1(void);
  HMdcCalibrater1(Text_t* name,Text_t* title,Int_t vers=1,Int_t cut=1,Int_t domerge=0);
  ~HMdcCalibrater1(void);
  Bool_t init(void);
  void initParameters(void);
  void initCounters()
  {
      for(Int_t i=0;i<6;i++)
      {
	  for(Int_t j=0;j<4;j++)
	  {
           countNrWiresPerMod[i][j]=0;
	  }
      }

  };
  void switchArguments(Int_t,Int_t,Int_t);
  void setParContainers(void);
  void setGlobalOffset(Float_t o0,Float_t o1,Float_t o2,Float_t o3)
  {
      globalOffset[0]=o0;
      globalOffset[1]=o1;
      globalOffset[2]=o2;
      globalOffset[3]=o3;
  }
  void setGlobalSlope(Float_t s){globalSlope=s;}
  Bool_t finalize(void) {return kTRUE;}
  Float_t getstarttime();
  Bool_t testTimeCuts(Float_t,Float_t);
  Bool_t translateAddress(Int_t*,Int_t*);
  void calcTimes(Float_t*,Float_t*,Float_t,Int_t,HMdcCalParTdc*);
  void fillCal1(Float_t ,Float_t ,Int_t );
  void countWiresPerMod();
  void printStatus();
  Int_t execute(void);
  ClassDef(HMdcCalibrater1,0) // Calibrater raw->cal1 for Mdc data
};

#endif /* !HMDCCALIBRATER1_H */

