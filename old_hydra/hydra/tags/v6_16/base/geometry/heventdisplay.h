#ifndef HEVENTDISPLAY_H
#define HEVENTDISPLAY_H

#include "hreconstructor.h"
#include "TGeometry.h"
#include "TNode.h"
#include "TOrdCollection.h"
#include "TObjArray.h"

class HDetector;
class HSpecGeomPar;
class HDetEvtDisplay;

class HEventDisplay: public HReconstructor {
private: 
  TGeometry* hgeom;               // Spectrometer geometry descriptor
  TOrdCollection *detDisplays;    // list of detector displays 
  TObjArray* masterNodes;         // array of cave node and sector nodes
  TObjArray* targetNodes;         // array of target nodes
  HSpecGeomPar* specGeomPar;      // geometry parameters for cave, sectors targets
  TIterator* iter;
public:
  HEventDisplay(Int_t nDisp=0); 
  ~HEventDisplay(void);
  Bool_t init(void);
  Bool_t reinit(void);
  Int_t execute(void); 
  void clear(void);
  Bool_t finalize(void) {return kTRUE;}
  void draw(Int_t nSec=-1); 
  Bool_t showEvent(void) {return kTRUE;}  
  inline TOrdCollection* getDetDisplays(void);
  void addDetDisplay(HDetEvtDisplay*);
  inline HDetEvtDisplay* getDisplay(const Text_t*);
  inline TNode* getCaveNode(void);
  inline TObjArray* getMasterNodes(void);
  inline TObjArray* getTargetNodes(void);
  inline TNode* getSectorNode(const Int_t);
  inline TNode* getTargetNode(const Int_t);
  inline TGeometry*  getGeometry(void);
  Bool_t createGeom(); 
private: 
  ClassDef(HEventDisplay,0)
};

//----------------------  inline functions -----------------------
 
inline  TNode* HEventDisplay::getCaveNode() {
  return (TNode*)masterNodes->At(0);
}

inline TOrdCollection* HEventDisplay::getDetDisplays() { 
  return detDisplays;
}

inline HDetEvtDisplay* HEventDisplay::getDisplay(const Text_t* name) {
  return (HDetEvtDisplay*)(detDisplays->FindObject(name));
}

inline  TGeometry* HEventDisplay::getGeometry(){
  return hgeom;
}

inline TObjArray* HEventDisplay::getMasterNodes() {
  return masterNodes;
}

inline TObjArray* HEventDisplay::getTargetNodes() { 
  return targetNodes;
}
 
inline TNode* HEventDisplay::getSectorNode(const Int_t i) { 
  return (TNode*)masterNodes->At(i+1);
}

inline TNode* HEventDisplay::getTargetNode(const Int_t i) {
  return (TNode*)targetNodes->At(i);
}

#endif  /* !HEVENTDISPLAY_H */

