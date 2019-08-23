#ifndef HRFIOROOTSOURCE_H
#define HRFIOROOTSOURCE_H

#include "hrootsource.h"
#include "TFile.h"
#include <iostream>

class HRFIORootSource : public HRootSource {
protected:
  TString fArchive; //!
  Bool_t access(const TString &name, Int_t mode); 
  TFile* getFile(TString name){return (new TFile(name.Data()));};
  TString getFileName(Text_t file[]);
public:
  HRFIORootSource(Bool_t fPersistent=kTRUE,Bool_t fMerge=kFALSE);
  virtual ~HRFIORootSource(void);
  void setArchive(Text_t archiveName[]);
  const char* getArchive(void){return fArchive.Data();}
  Bool_t setInput(Text_t *fileName,Text_t *archiveName,Text_t *treeName);
  ClassDef(HRFIORootSource,1) //Data source to read Root files from the robot.
};

#endif /* !HRFIOROOTSOURCE_H */


 

