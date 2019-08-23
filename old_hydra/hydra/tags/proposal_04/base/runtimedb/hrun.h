#ifndef HRUN_H
#define HRUN_H

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

class HRun : public TNamed {
protected:
  TList* parVersions;   // List of container names with the versions
  TString refRun;       //! name of the reference run for initialization
public:
  HRun() {}
  HRun(Text_t* name,Text_t* refName="");
  HRun(Int_t r,Int_t rr=-1);
  HRun(HRun &run);
  ~HRun();
  inline Int_t getRunId(void);
  void addParVersion(HParVersion *pv);
  HParVersion* getParVersion(Text_t* name);
  TList* getParVersions() {return parVersions;}
  const Text_t* getRefRun() {return refRun.Data();}
  void setRefRun(Text_t* s) {refRun=s;}
  inline void setRefRun(Int_t r);
  void resetInputVersions();
  void resetOutputVersions();
  void print();

  ClassDef(HRun,1) // Class for parameter version management of a run
};

// -------------------- inlines ---------------------------

inline Int_t HRun::getRunId(void) {
  Int_t r;
  sscanf(GetName(),"%i",&r);
  return r;
}

inline void HRun::setRefRun(Int_t r) {
  if (r==-1) refRun="";
  else {  
    char name[255];
    sprintf(name,"%i",r);
    refRun=name;
  }
}

#endif  /* !HRUN_H */
