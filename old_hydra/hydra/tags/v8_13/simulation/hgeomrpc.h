#ifndef HGEOMRPC_H
#define HGEOMRPC_H

#include "hgeomset.h"

class  HGeomRpc : public HGeomSet {
protected:
  char modName[6];  // name of module
  char eleName[2];  // substring for elements in module
public:
  HGeomRpc();
  ~HGeomRpc() {}
  const char* getModuleName(Int_t) {return modName;}
  const char* getEleName(Int_t) {return eleName;}
  Int_t getSecNumInMod(const TString&);
  ClassDef(HGeomRpc,0) // Class for RPC
};

inline Int_t HGeomRpc::getSecNumInMod(const TString& name) {
  // returns the sector index retrieved from ESKOx
  return (Int_t)(name[4]-'0')-1;
}
#endif  /* !HGEOMRPC_H */