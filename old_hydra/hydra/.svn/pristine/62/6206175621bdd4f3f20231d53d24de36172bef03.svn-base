#ifndef HORASIMEVENTFILE_H
#define HORASIMEVENTFILE_H

using namespace std;
#include "TObject.h"
#include "TString.h"
#include <fstream>

class HOraSimEventFile : public TObject {
private:
  TString fileName;
  TString fileDirectory;
  Int_t fileSize;
  TString created;
  Int_t nEvents;
public:
  HOraSimEventFile();
  ~HOraSimEventFile() {}
  void setFileName(const Char_t* s) { fileName=s; }
  void setFileDirectory(const Char_t* s) {fileDirectory =s; }
  void setCreationDate(const Char_t* s) { created=s; }
  void setNEvents(Int_t n) { nEvents=n; }
  void setNEvents(const Char_t* s);
  void setFileSize(Int_t n) { fileSize=n; }  
  void setFileSize(const Char_t* s);  
  const Char_t* getFileName() { return fileName.Data(); }
  const Char_t* getFileDirectory() { return fileDirectory.Data(); }
  const Char_t* getCreationDate() { return created.Data(); }
  Int_t getNEvents() { return nEvents; }
  Int_t getFileSize() { return fileSize; }
  void print();
  void read(fstream& fin);
private:
  Int_t convertToInt(TString& s);
  ClassDef(HOraSimEventFile,0) // Class for event files for HGEANT
};

#endif  /* !HORASIMEVENTFILE_H */
