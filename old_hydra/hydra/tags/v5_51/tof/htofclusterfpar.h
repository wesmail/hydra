#ifndef HTOFCLUSTERFPAR_H
#define HTOFCLUSTERFPAR_H

#include "hparset.h"
#include <iostream.h>
#include <fstream.h>

class HTofClusterFPar : public HParSet {
private:
  Float_t diffTmax;       //Maximal time distance of two hits.
  Float_t diffXmax;       //Maximal x position distance of two hits.

public:
  HTofClusterFPar(const char* name="TofClusterFPar",
                  const char* title="Parameter container for the cluster finder",
                  const char* context="");
  virtual ~HTofClusterFPar(void);
  void clear(void);
  Bool_t init(HParIo *inp, Int_t *set);
  Int_t write(HParIo *io);
  Int_t writeFile(fstream *pFile);
  Bool_t read(fstream *pFile,Int_t* set,Int_t inputNumber);
  Float_t getMaxTDiff(void)      { return diffTmax; }
  Float_t getMaxXDiff(void)      { return diffXmax; }
  void printTo(ostream &out);
  void readFrom(istream &in);
  void readFrom(const HTofClusterFPar &par);
  ClassDef(HTofClusterFPar,1)
};

#endif
