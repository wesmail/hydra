#ifndef HLDFILESOURCE_H
#define HLDFILESOURCE_H

#include "hldsource.h"
#include "TIterator.h"
#include <TString.h>

class HldFileDesc : public TObject {
  TString fName; // File name
  Int_t fRunId;  // RunId
  Int_t fRefId;  // Reference run Id for initialization
  HldFileDesc(void) : fRunId(-1) {}
 public:
  HldFileDesc(const Text_t *name,const Int_t runId,const Int_t refId=-1) : fName(name),fRunId(runId) {
    fRefId=refId;
  }
  ~HldFileDesc(void) {}
  Text_t const *GetName(void) const { return fName.Data(); }
  Int_t getRunId(void) { return fRunId; }
  Int_t getRefId(void) { return fRefId; }
  void setRefId(Int_t r) { fRefId=r; }
  ClassDef(HldFileDesc,0) // File descriptor for Hld source
};

class HldFileSource : public HldSource {
protected:
  TList fFileList;  // List with all files to be analyzed
  HldFileDesc *fCurrentFile; // Pointer to current file.
  TString fCurrentDir; //Current directory for addFile
  TIterator* iter;  
  Int_t fEventNr, fEventLimit; //Event counter and maximun event nr per file
public:
  HldFileSource(void);
  ~HldFileSource(void);
  HldFileSource(HldFileSource &so);
  Bool_t init();
  void setMaxEventPerFile(Int_t n) {fEventLimit = n; } 
  EDsState getNextEvent(void);
  Bool_t getNextFile();
  Int_t getCurrentRunId(void);
  Int_t getCurrentRefId(void);
  Int_t getRunId(const Text_t *fileName);
  Text_t const *getCurrentFileName(void) {return fCurrentFile->GetName();}
  void addFile(Text_t *fileName, Text_t *refFile);
  void addFile(Text_t *fileName, Int_t refId=-1);
  void setDirectory(Text_t *direc) {fCurrentDir = direc;}
  ClassDef(HldFileSource,0) //! Data source to read LMD files;
};
#endif /* !HLDFILESOURCE_H */
