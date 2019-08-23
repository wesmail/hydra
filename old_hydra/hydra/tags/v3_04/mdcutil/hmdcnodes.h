#ifndef HMDCNODES_H
#define HMDCNODES_H

#include "TNode.h"
#include "TObjArray.h"
#include "TArrayI.h"

class HMdcLayerGeomPar;

class HMdcCompNodes : public TObject {
private:
  TObjArray* compNodes;  // geometry nodes of layers and wires
  Int_t firstWire;       // index of first wire node in the array
public:
  HMdcCompNodes(Int_t n=0,Int_t maxLay=6);
  ~HMdcCompNodes(void);
  void set(TNode* node ,Int_t i) {compNodes->AddAt(node,i);}
  TNode* get(Int_t i) {return (TNode*)(compNodes->At(i));}
  void clear(void);
  ClassDef(HMdcCompNodes,0)    // Mdc geometry nodes of layers and wires
};


class HMdcNodes : public TObject {
private:
  Int_t maxSec;            // maximum number of sectors
  Int_t maxMod;            // maximum number of modules
  Int_t maxLay;            // maximum number of layers
  Int_t maxWire;           // maximum number of wires
  TObjArray* moduleNodes;  // array of modules
public:
  HMdcNodes(Int_t s=6,Int_t m=4);
  ~HMdcNodes(void);
  void createCompNodes(TArrayI&,Int_t l=0,Int_t w=0);
  void removeCompNodes(void);
  void addNode(TNode*,Int_t,Int_t,Int_t l=-1,Int_t w=-1);
  TNode* getNode(Int_t,Int_t,Int_t l=-1,Int_t w=-1);
  void clear();
  Int_t getMaxSec(void) const { return maxSec; }
  Int_t getMaxMod(void) const{ return maxMod; }
  Int_t getMaxLay(void) const{ return maxLay; }
  Int_t getMaxWire(void) const { return maxWire; }
  ClassDef(HMdcNodes,0)    // Mdc geometry nodes
};

#endif  /* !HMDCNODES_H */
