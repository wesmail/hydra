#ifndef HRUNINFO_H
#define HRUNINFO_H

using namespace std;
#include "TNamed.h"
#include <iostream>
#include <iomanip>
#include <fstream>

class HRunInfo : public TNamed {
private:
  Int_t   runId;      // run identifier
  TString startTime;  // start time
  TString endTime;    // stop time
  Int_t   numEvents;  // num of events
public:
  HRunInfo(const char* filename="");
  ~HRunInfo() {}
  void setRunId(Int_t n) {runId=n;}
  void setStartTime(const char* s) {startTime=s;}
  void setEndTime(const char* s) {endTime=s;}
  void setNumEvents(Int_t n) {numEvents=n;}
  Int_t getRunId() {return runId;}
  const char* getStartTime() {return startTime.Data();}
  const char* getEndTime() {return endTime.Data();}
  Int_t getNumEvents() {return numEvents;}
  void print();
  void write(fstream&);
private:
  ClassDef(HRunInfo,0) // Class for run information
};

#endif  /* !HRUNINFO */
