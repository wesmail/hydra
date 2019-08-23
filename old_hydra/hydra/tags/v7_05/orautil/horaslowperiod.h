#ifndef HORASLOWPERIOD_H
#define HORASLOWPERIOD_H

using namespace std;
#include "TObject.h"
#include "TString.h"
#include <iostream>
#include <iomanip>
#include <fstream>

class HOraSlowPeriod : public TObject {
private:
  Int_t      periodId;   // Id of period
  TString    startTime;  // Start time of period 
  TString    endTime;    // End time of period 
  Int_t      runId;      // Id of corresponding run
  TString    filename;   // Hld-filename of corresponding run
  Int_t      filterFlag; // Flag for Hld-file filter
public:
  HOraSlowPeriod();
  ~HOraSlowPeriod() {}
  void setPeriodId(Int_t n) {periodId=n;}
  void setStartTime(const char* s) {startTime=s;}
  void setEndTime(const char* s) {endTime=s;}
  void setRunId(Int_t n) {runId=n;}
  void setFilename(const char* s) {filename=s;}
  void setFilterFlag(Int_t f) {filterFlag=f;}
  Int_t getPeriodId() {return periodId;}
  const char* getStartTime() {return startTime.Data();}
  const char* getEndTime() {return endTime.Data();}
  Int_t getRunId() {return runId;}
  const char* getFilename() {return filename.Data();}
  Int_t getFilterFlag() {return filterFlag;}
  void print();
  void write(fstream&);
private:
  ClassDef(HOraSlowPeriod,0) // Class for a slowcontrol run period
};

#endif  /* !HORASLOWPERIOD */
