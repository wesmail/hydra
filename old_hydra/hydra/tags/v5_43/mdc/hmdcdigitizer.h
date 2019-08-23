#ifndef HMDCDITIZER_H
#define HMDCDITIZER_H

#include "hreconstructor.h"
#include "hcategory.h"
#include "TRandom.h"
#include "TNtuple.h"
#include "iostream.h"
#include "hlocation.h"

class HIterator;
class HMdcLayerGeomPar;
class HMdcDigitPar;
class HMdcCal2ParSim;
class HMdcCal1Sim;
class HMdcCellEff;
class HMdcGeantCell;
class TFile;

class HMdcDigitizer : public HReconstructor {
private:
  HCategory* fGeantMdcCat;              //! MDC HGeant input data 
  HLocation loc;                        // Location for new object
  HCategory* fGeantCellCat;             // Pointer to sim data category
  HCategory* fCalCat;                   // Pointer to cal data category
  HMdcGeantCell* hit;                   // Pointer to HMdcGeantCell hit
  HMdcGeantCell* hitNeigh;              // Pointer to neighbour
  HMdcLayerGeomPar* fDigitGeomPar;      // Digitisation "geom" parameters
  HMdcDigitPar* fDigitPar;              // Digitisation "phys" parameters
  HMdcCal2ParSim* fCal2ParSim;          // pointer to cal2 parameter container
  HMdcCellEff* fCellEff;                // pointer to cell efficiency parameter container

  HIterator* iterin;                    // Iterator over input category
  HIterator* itercell;                  // Iterator over cell category
  Int_t fEventId;                       // Number of current event
  Float_t yDist;                        // Distance to the sence wire
  Float_t pi;                           // Il y quatre pis a une vache
  Int_t setup[6][4];                    // setup of Mdc (sec,mod)
  Float_t offsets[4];                   // offsets are used to substract min tof
  Float_t Level[4];                     // level of requiered maximum charge to create a signal (for example 20 (=20%))

  Float_t time1;
  Float_t time1Error;
  Float_t time2;
  Float_t time2Error;
  Int_t counthit;
  Float_t myalpha;
  TFile *myoutput;
  TNtuple *distance_time;
  Bool_t fntuple;
  Bool_t useOffsets;
  Bool_t useCellEff;
  Int_t firstHit;     // number of first valid hit
  Int_t secondHit;    // number of second valid hit
  Float_t firstTime2; // time2 of first valid hit
  Int_t endList1;
  Bool_t noTof;
  Int_t modetdc;      // 2 leading edges or leading and trailing edge of the signal
public:
  HMdcDigitizer(void);
  HMdcDigitizer(Text_t* name,Text_t* title,Int_t,Bool_t);
  ~HMdcDigitizer(void);
  Bool_t init(void);
  Bool_t finalize();
  Int_t execute(void);
  void setParContainers();
  void setOffsets(Float_t off0,Float_t off1,Float_t off2,Float_t off3)
  {
      useOffsets=kTRUE;
      offsets[0] =off0;
      offsets[1] =off1;
      offsets[2] =off2;
      offsets[3] =off3;
  }
  void setEffLevel(Float_t eff0,Float_t eff1,Float_t eff2,Float_t eff3)
  {
      useCellEff=kTRUE;
      Level[0] =eff0;
      Level[1] =eff1;
      Level[2] =eff2;
      Level[3] =eff3;
  }

  void setNoTof() {noTof=kTRUE;}
  Bool_t transform(Float_t,Float_t,Float_t,Float_t,Float_t,Int_t);
  void storeCell(Float_t,Float_t,Float_t,Int_t,Bool_t);
  void select(Float_t*,Float_t*,Float_t*,
	      Float_t*,Float_t*,Float_t*,
	      Int_t*,Int_t*,Int_t);
  void fillTrackList(HMdcCal1Sim*, Int_t*, Int_t*);
  void findFirstValidHit(Int_t*, Float_t*, Float_t*, Int_t*, Float_t*, Int_t*);
  void findSecondValidHit(Int_t, Int_t*, Float_t*, Float_t*, Int_t*);
  void getMdcSetup();
  Bool_t testMdcSetup(Int_t s, Int_t m);
  void setNTuples(void);
  ClassDef(HMdcDigitizer,0) // Digitizer of MDC data
};

#endif









