#ifndef HORASLOWPARTITION_H
#define HORASLOWPARTITION_H

using namespace std;
#include "TNamed.h"
#include "TObjArray.h"
#include <iostream>
#include <iomanip>
#include <fstream>

class HOraSlowPeriod;
class TGraph;

class HOraSlowPartition : public TNamed {
private:
  TString    partition;   // Name of the data partition
  TString    startTime;   // Start time of the data
  TString    endTime;     // Start time of the data
  TObjArray* pRunPeriods; // Array of periods
  TIterator* periodIter;  // iterator on pRunPeriods
  TObjArray* pRates;      // Array of archiver rates
  TIterator* ratesIter;   // iterator on pRates
  TGraph*    pGraph;      // Pointer to the graph showing the archiver rates
public:
  HOraSlowPartition(const char* name="");
  ~HOraSlowPartition();
  void setStartTime(const char* t) {startTime=t;}
  void setEndTime(const char* t) {endTime=t;}
  const char* getStartTime() {return startTime.Data();}
  const char* getEndTime() {return endTime.Data();}
  TObjArray* setNumPeriods(Int_t);
  TObjArray* getRunPeriods() {return pRunPeriods;}
  void setRates(TObjArray* p);
  HOraSlowPeriod* getPeriod(Int_t);
  HOraSlowPeriod* getRun(Int_t);
  HOraSlowPeriod* getRun(const char*);
  void print(Int_t opt=0);  
  void write(fstream& fout,Int_t opt=0);
  void printRates();  
  void writeRates(fstream& fout);
  void setHldFileFilter(TList* l); 
  TGraph* getRatesGraph(Int_t mStyle=7,Int_t mColor=4);
private:
  void clearRunPeriods();
  void clearRates();
  void deleteGraph();
  ClassDef(HOraSlowPartition,0) // Class for a slowcontrol data partition
};

#endif  /* !HORASLOWPARTITION */
