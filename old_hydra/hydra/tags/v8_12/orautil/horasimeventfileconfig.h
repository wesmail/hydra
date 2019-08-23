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
  void setPackageName(const char* s) { packageName=s; }
  void setPackageVersion(const char* s) { packageVersion=s; }
  void setCompiler(const char* s) { compiler=s; }
  void decodeSystem(const char*);
  void setEventSystem(const char* s) { eventSystem=s; }
  void setEnergy(Float_t e) { energy=e; }
  void setConfigInfo(const char* s) { configInfo=s; }
  void setFileFormat(const char* s) { fileFormat=s; }
  void setAuthor(const char* s) { author=s; }
  void setDescription(const char* s) { description=s; }
  const char* getPackageName() { return packageName.Data(); }
  const char* getPackageVersion() { return packageVersion.Data(); }
  const char* getCompiler() { return compiler.Data(); }
  const char* getEventSystem() { return eventSystem.Data(); }
  Float_t getEnergy() { return energy; }
  const char* getConfigInfo() { return configInfo.Data(); }
  const char* getFileFormat() { return fileFormat.Data(); }
  const char* getAuthor() { return author.Data(); }
  const char* getDescription() { return description.Data(); }
  Bool_t readConfigInfo(const char*);
  void print();
private:
  ClassDef(HOraSimEventFileConfig,0) // Class for the configuration of event files for HGEANT
};

#endif  /* !HORASIMEVENTFILECONFIG_H */
