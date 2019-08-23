#ifndef HKICKTRACKFPAR_H
#define HKICKTRACKFPAR_H

#include "hparset.h"
#include <iostream.h>
#include <fstream.h>

class HKickTrackFPar : public HParSet {
private:
  Float_t fPMin;          //Minimum accepted momentum
  Float_t fPMin2;         //Maximum value of miminum momentum
  Float_t fMaxXPull1;     //Maximum delta x / sigma: restrictive
  Float_t fMaxXPull2;     //Maximum delta x / sigma: permisive
  Float_t fXPullStep;     //Increment on xPull
  Float_t fTofXResolution; //Resolution of TOF detector in X (mm)
  Float_t fMinTof;        //Minimum accepted TOF
  Float_t fMaxTof;        //Maximum accepted TOF
public:
  HKickTrackFPar(const char* name="KickTrackFPar",
       const char* title="Parameter container for the track finder of the kick plane",
       const char* context="");
  virtual ~HKickTrackFPar(void);
  void clear(void);
  Bool_t init(HParIo *inp, Int_t *set);
  Int_t write(HParIo *io);
  Int_t writeFile(fstream *pFile);
  Bool_t read(fstream *pFile,Int_t* set,Int_t inputNumber);
  Float_t getPMin(void)          { return fPMin; }
  Float_t getPMin2(void)         { return fPMin2; }
  Float_t getMaxXPull1(void)     { return fMaxXPull1; }
  Float_t getMaxXPull2(void)     { return fMaxXPull2; }
  Float_t getXPullStep(void)     { return fXPullStep; }
  Float_t getTofXResolution(void) { return fTofXResolution; }
  Float_t getMinTof(void)        { return fMinTof; }
  Float_t getMaxTof(void)        { return fMaxTof; }
  void printTo(ostream &out);
  void readFrom(istream &in);
  void readFrom(const HKickTrackFPar &par);
  ClassDef(HKickTrackFPar,1)
};

#endif
