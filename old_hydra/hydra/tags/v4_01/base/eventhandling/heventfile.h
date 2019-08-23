#ifndef HEVENTFILE_H
#define HEVENTFILE_H

#include "TNamed.h"
#include "TList.h"
#include "TString.h"

class HParVersion : public TNamed {
protected:
  Int_t inputVersions[3]; //! version of the container in the 2 possible inputs
  Int_t rootVersion;      // version of the container in ROOT output file
public:
  HParVersion() {}
  HParVersion(Text_t* name);
  ~HParVersion() {}
  void setInputVersion(Int_t v=-1,Int_t i=0) {
    if (i>=0 && i<3)  inputVersions[i]=v;
  }
  Int_t getInputVersion(Int_t i) {
    if (i>=0 && i<3) return inputVersions[i];
    else return -1;
  }
  void resetInputVersions() {
    for(Int_t i=0;i<3;i++) {inputVersions[i]=-1;}
  }
  void setRootVersion(Int_t v) {rootVersion=v;}
  Int_t getRootVersion() {return rootVersion;}
  ClassDef(HParVersion,1) // Class for parameter versions
};

class HEventFile : public TNamed {
protected:
  Long_t fileId;        // Id number of the event file 
  TList* parVersions;   // List of container names with the versions
  TString refFileName;  //! name of the reference event file for initialization
public:
  HEventFile() {}
  HEventFile(Text_t* name,Text_t* refName="");
  HEventFile(HEventFile &ef);
  ~HEventFile();

  void addParVersion(HParVersion *pv);
  HParVersion* getParVersion(Text_t* name);
  TList* getParVersions() {return parVersions;}
  Long_t getFileId() {return fileId;}
  void setFileId(Long_t n) {fileId=n;}
  const Text_t* getRefFileName() {return refFileName.Data();}
  void setRefFileName(Text_t* s) {refFileName=s;}
  void resetInputVersions();
  void resetOutputVersions();
  void print();

  ClassDef(HEventFile,1) // Class for management of the input event files
};

#endif  /* !HEVENTFILE_H */

