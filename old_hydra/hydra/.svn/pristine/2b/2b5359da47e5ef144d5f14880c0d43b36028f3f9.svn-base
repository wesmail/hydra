#ifndef HORASIMRUNCONFIG_H
#define HORASIMRUNCONFIG_H

#include "TObject.h"
#include "TString.h"

class HOraSimRunConfig : public TObject {
private:
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
  void setGeantVersion(const Char_t* v) { geantVersion=v; }
  void setHydraVersion(const Char_t* v) { hydraVersion=v; }
  void setRootVersion(const Char_t* v) { rootVersion=v; }
  void setCernLib(const Char_t* v) { cernLib=v; }
  void setCompiler(const Char_t* s) { compiler=s; }
  void setGeantFlags(const Char_t* s) { geantFlags=s; }
  void setParameters(const Char_t* s) { parameters=s; }
  void setAuthor(const Char_t* s) { author=s; }
  void setDescription(const Char_t* s) { description=s; }
  const Char_t* getGeantVersion() { return geantVersion.Data(); }
  const Char_t* getHydraVersion() { return hydraVersion.Data(); }
  const Char_t* getRootVersion() { return rootVersion.Data(); }
  const Char_t* getCernLib() { return cernLib.Data(); }
  const Char_t* getCompiler() { return compiler.Data(); }
  const Char_t* getGeantFlags() { return geantFlags.Data(); }
  const Char_t* getParameters() { return parameters.Data(); }
  const Char_t* getAuthor() { return author.Data(); }
  const Char_t* getDescription() { return description.Data(); }
  void print();
  Bool_t compare(HOraSimRunConfig&);
private:
  ClassDef(HOraSimRunConfig,0) // Class for the configuration of HGEANT simulations runs
};

#endif  /* !HORASIMRUNCONFIG_H */
