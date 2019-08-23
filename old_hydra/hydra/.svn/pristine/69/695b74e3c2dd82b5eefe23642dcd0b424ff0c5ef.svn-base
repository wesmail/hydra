#ifndef HRPCDITIZER_H
#define HRPCINODITIZER_H
using namespace std;
#include "hreconstructor.h"
#include <iostream>
#include <iomanip>


class HRpcDigitizer : public HReconstructor {

private:

public:
  HRpcDigitizer(void);
  HRpcDigitizer(Text_t* name,Text_t* title);
  ~HRpcDigitizer(void);
  Bool_t init(void);
  Bool_t finalize(void); 
  Int_t execute(void);

public:
  ClassDef(HRpcDigitizer,0) // Digitizer for RPC HGeant hits
};

#endif
