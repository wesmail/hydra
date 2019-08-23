#ifndef HORASIMRUNCONFIG_H
#define HORASIMRUNCONFIG_H

#include "TObject.h"
#include "TString.h"

class HOraSimRunConfig : public TObject {
private:
  Int_t   generation;
  TString geantVersion;
  TString hydraVersion;
  TString rootVersion;
  TString cernLib;
  TString compiler;
  TString geantFlags;
  TString parameters;
  TString author;
  TString description;
public:
  HOraSimRunConfig();
  ~HOraSimRunConfig() {}
  void setGeneration(Int_t g) { generation=g; }
  void setGeantVersion(const char* v) { geantVersion=v; }
  void setHydraVersion(const char* v) { hydraVersion=v; }
  void setRootVersion(const char* v) { rootVersion=v; }
  void setCernLib(const char* v) { cernLib=v; }
  void setCompiler(const char* s) { compiler=s; }
  void setGeantFlags(const char* s) { geantFlags=s; }
  void setParameters(const char* s) { parameters=s; }
  void setAuthor(const char* s) { author=s; }
  void setDescription(const char* s) { description=s; }
  Int_t getGeneration() { return generation; }
  const char* getGeantVersion() { return geantVersion.Data(); }
  const char* getHydraVersion() { return hydraVersion.Data(); }
  const char* getRootVersion() { return rootVersion.Data(); }
  const char* getCernLib() { return cernLib.Data(); }
  const char* getCompiler() { return compiler.Data(); }
  const char* getGeantFlags() { return geantFlags.Data(); }
  const char* getParameters() { return parameters.Data(); }
  const char* getAuthor() { return author.Data(); }
  const char* getDescription() { return description.Data(); }
  void print();
  Bool_t compare(HOraSimRunConfig&);
private:
  ClassDef(HOraSimRunConfig,0) // Class for the configuration of HGEANT simulations runs
};

#endif  /* !HORASIMRUNCONFIG_H */
