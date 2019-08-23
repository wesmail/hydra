#ifndef HORASIMRUN_H
#define HORASIMRUN_H

#include "TObject.h"
#include "TString.h"

class HOraSimRunConfig;

class HOraSimRun : public TObject {
private:
  Int_t runId;
  Int_t fileNum;
  TString fileName;
  TString fileDirectory;
  Int_t fileSize;
  TString created;
  Int_t nEvents;
  HOraSimRunConfig* config;
  TString eventFile;  
public:
  HOraSimRun();
  ~HOraSimRun();
  void setRunId(Int_t id) { runId=id; }
  void setFileNum(Int_t n) { fileNum=n; }
  void setFileName(const char* s) { fileName=s; }
  void setFileDirectory(const char* s) {fileDirectory =s; }
  void setCreationDate(const char* s) { created=s; }
  void setNEvents(Int_t n) { nEvents=n; }
  void setNEvents(const char* s);
  void setFileSize(Int_t n) { fileSize=n; }
  void setFileSize(const char* s);
  void setEventFile(const char* s) { eventFile=s; }
  Int_t getRunId() { return runId; }
  Int_t getFileNum() { return fileNum; }
  const char* getFileName() { return fileName.Data(); }
  const char* getFileDirectory() { return fileDirectory.Data(); }
  const char* getCreationDate() { return created.Data(); }
  Int_t getNEvents() { return nEvents; }
  Int_t getFileSize() { return fileSize; }
  HOraSimRunConfig* getConfig() { return config; }
  const char* getEventFile() { return eventFile.Data(); }
  void print();
private:
  Int_t convertToInt(TString& s);
  ClassDef(HOraSimRun,0) // Class for HGEANT simulation runs
};

#endif  /* !HORASIMRUN_H */

