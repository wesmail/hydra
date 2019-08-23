#ifndef HORASLOWARCHRATEOBJ_H
#define HORASLOWARCHRATEOBJ_H

using namespace std;
#include "TObject.h"
#include "TString.h"
#include <iostream>
#include <iomanip>
#include <fstream>

class HOraSlowArchRateObj : public TObject {
private:
  TString startTime;  // Timestamp of the rate
  Int_t   rate;       // Number of entries per minute in the channel archiver
public:
  HOraSlowArchRateObj();
  HOraSlowArchRateObj(const char*,Int_t);
  ~HOraSlowArchRateObj() {}
  void setStartTime(const char* s) {startTime=s;}
  void setRate(Int_t n) {rate=n;}
  const char* getStartTime() {return startTime.Data();}
  Int_t getRate() {return rate;}
  Int_t getTimeDiff(const char*);
  void print();
  void write(fstream&);
private:
  ClassDef(HOraSlowArchRateObj,0) // Class for a slowcontrol archiver rate objects
};

#endif  /* !HORASLOWARCHRATEOBJ */