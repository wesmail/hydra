#ifndef HPARAMFILEGENERATOR_H
#define HPARAMFILEGENERATOR_H

using namespace std;
#include "TObject.h"
#include "TList.h"
#include <iostream>
#include <iomanip>

class HParRootFileIo;

class HParamFileGenerator : public TObject {
private:
  TString paramFilename;
  TString logFilename;
  TString experiment;  
  TString rangeStart;  
  TString rangeEnd;  
  TList* runs;
  fstream* logFile;     
public:
  HParamFileGenerator(const char* beamtime,
                      const char* startAt="",const char* endAt="" );
  ~HParamFileGenerator();
  HParRootFileIo* openParameterFile(const char* filename);
  const char* getParamFilename() {return paramFilename.Data();} 
  const char* getLogFilename() {return logFilename.Data();} 
  const char* getExperiment() {return experiment.Data();} 
  const char* getRangeStart() {return rangeStart.Data();} 
  const char* getRangeEnd() {return rangeEnd.Data();} 
  void setListOfRuns(TList* r);
  void writeRuns();
  void closeLogFile();
  fstream* getLogFile() {return logFile;}
private:
  ClassDef(HParamFileGenerator,0) // Class to create a parameter file
};

#endif  /* !HPARAMFILEGENERATOR_H */
