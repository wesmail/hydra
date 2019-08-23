#ifndef HORASIMEVENTFILECONFIG_H
#define HORASIMEVENTFILECONFIG_H

#include "TObject.h"
#include "TString.h"

class HOraSimEventFileConfig : public TObject {
private:
  TString packageName;
  TString packageVersion;
  TString compiler;
  TString eventSystem;
  Float_t energy;
  TString configInfo;
  TString fileFormat;
  TString author;
  TString description;
public:
  HOraSimEventFileConfig(); 
  ~HOraSimEventFileConfig() {}
  void setPackageName(const Char_t* s) { packageName=s; }
  void setPackageVersion(const Char_t* s) { packageVersion=s; }
  void setCompiler(const Char_t* s) { compiler=s; }
  void decodeSystem(const Char_t*);
  void setEventSystem(const Char_t* s) { eventSystem=s; }
  void setEnergy(Float_t e) { energy=e; }
  void setConfigInfo(const Char_t* s) { configInfo=s; }
  void setFileFormat(const Char_t* s) { fileFormat=s; }
  void setAuthor(const Char_t* s) { author=s; }
  void setDescription(const Char_t* s) { description=s; }
  const Char_t* getPackageName() { return packageName.Data(); }
  const Char_t* getPackageVersion() { return packageVersion.Data(); }
  const Char_t* getCompiler() { return compiler.Data(); }
  const Char_t* getEventSystem() { return eventSystem.Data(); }
  Float_t getEnergy() { return energy; }
  const Char_t* getConfigInfo() { return configInfo.Data(); }
  const Char_t* getFileFormat() { return fileFormat.Data(); }
  const Char_t* getAuthor() { return author.Data(); }
  const Char_t* getDescription() { return description.Data(); }
  Bool_t readConfigInfo(const Char_t*);
  void print();
private:
  ClassDef(HOraSimEventFileConfig,0) // Class for the configuration of event files for HGEANT
};

#endif  /* !HORASIMEVENTFILECONFIG_H */
