#ifndef HORASLOWCHANRAW_H
#define HORASLOWCHANRAW_H

using namespace std;
#include "TObject.h"
#include "TString.h"
#include <iostream>
#include <iomanip>
#include <fstream>

class HOraSlowChannel;

class HOraSlowChanRaw : public TObject {
private:
  TString  timestamp;   // Timestamp (data + time)
  Int_t    nanoSeconds; // Nanoseconds
  Double_t value;       // Data value
public:
  HOraSlowChanRaw();
  ~HOraSlowChanRaw() {}
  void setTimestamp(const char* s) {timestamp=s;}
  void setNanoSeconds(Int_t v) {nanoSeconds=v;}
  void setValue(Double_t v) {value=v;}
  void setValue(Int_t v) {value=(Double_t)v;}
  void fill(const char*,Int_t,Double_t);
  void fill(const char*,Int_t,Int_t);
  const char* getTimestamp() {return timestamp.Data();}
  Int_t getNanoSeconds() {return nanoSeconds;}
  Double_t getValue() {return value;}
  Int_t getTimeDiff(const char*);
  void print(Int_t valPrec=3);
  void write(fstream& fout,Int_t valPrec=3);
private:
  ClassDef(HOraSlowChanRaw,0) // Class for raw data for a slowcontrol channel
};

#endif  /* !HORASLOWCHANRAW */
