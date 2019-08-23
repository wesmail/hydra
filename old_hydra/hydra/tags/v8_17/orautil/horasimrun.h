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
  void setFileName(const Char_t* s) { fileName=s; }
  void setFileDirectory(const Char_t* s) {fileDirectory =s; }
  void setCreationDate(const Char_t* s) { created=s; }
  void setNEvents(Int_t n) { nEvents=n; }
  void setNEvents(const Char_t* s);
  void setFileSize(Int_t n) { fileSize=n; }
  void setFileSize(const Char_t* s);
  void setEventFile(const Char_t* s) { eventFile=s; }
  Int_t getRunId() { return runId; }
  Int_t getFileNum() { return fileNum; }
  const Char_t* getFileName() { return fileName.Data(); }
  const Char_t* getFileDirectory() { return fileDirectory.Data(); }
  const Char_t* getCreationDate() { return created.Data(); }
  Int_t getNEvents() { return nEvents; }
  Int_t getFileSize() { return fileSize; }
  HOraSimRunConfig* getConfig() { return config; }
  const Char_t* getEventFile() { return eventFile.Data(); }
  void print();
private:
  Int_t convertToInt(TString& s);
  ClassDef(HOraSimRun,0) // Class for HGEANT simulation runs
};

#endif  /* !HORASIMRUN_H */

