#ifndef HMDCEVTDISPLAY_H
#define HMDCEVTDISPLAY_H

#include "hdetevtdisplay.h"
#include "TGeometry.h"
#include "TNode.h"
#include "TObjArray.h"

class HCatEvtDisplay;
class HMdcNodes;
class HDetector;
class HMdcGeomPar;
class HMdcLayerGeomPar;
class HGeomTransform;
class HGeomShapes;
class HCategory;
class HMdcConnectedWires;

class HMdcEvtDisplay : public HDetEvtDisplay {
private:
  Int_t maxSec;     // maximum number of sectors
  Int_t maxMod;     // maximum number of modules
  Int_t maxLay;     // maximum number of layers
  Int_t maxRefMod;  // maximum number of reference modules
  Int_t numCatDisp; // number of event displays for the categories
  TObjArray* catDisplays;  // array of displays for the categories
  HMdcNodes* moduleNodes;  // array of module nodes
  HMdcNodes* refNodes;  // array of reference module nodes
  TObjArray* centers;   // centers of ROOT volumes   
  HMdcGeomPar *mdcGeomPar; // pointer to MDC geometry container
  HMdcLayerGeomPar *mdcLayGeomPar; // pointer to geometry container for layers
  HGeomShapes* shapes;  // pointer to the shape classes
  HMdcConnectedWires* connectedWires; // pointer to container "MdcConnectedWires"
public:
  HMdcEvtDisplay(void);
  HMdcEvtDisplay(HDetector* det);
  ~HMdcEvtDisplay(); 
  TNode* getNode(Int_t,Int_t,Int_t l=-1,Int_t w=-1);
  void draw(Int_t,Int_t,Int_t l=-1);
  void drawConnectedWires(Int_t,Int_t,Int_t mo=-1);
  void drawConnectedWires(Int_t,Int_t,Text_t*);
  Bool_t init(void);
  Bool_t needsChange(void);
  Bool_t reinit(void);
  Int_t execute(void);   
  void clear(void);
  void addCatEvtDisplay(HCatEvtDisplay*);
  HCatEvtDisplay* getCatEvtDisplay(Text_t*);
  Bool_t createGeom(TNode*,const HGeomTransform*,const HGeomTransform*,
                    Int_t secNum=-1);   
  void cleanup(void);   
  Int_t getMaxSec(void) const { return maxSec; }
  Int_t getMaxMod(void) const{ return maxMod; }
  Int_t getMaxLay(void) const{ return maxLay; }
private:
  void copyWires(Int_t,Int_t,Int_t,Int_t,Int_t);
  TNode* copyNode(const Text_t*,TNode*);
  void addCenter(HGeomTransform*,Int_t,Int_t l=-1);
  HGeomTransform* getCenter(Int_t,Int_t l=-1);
  ClassDef(HMdcEvtDisplay,0)  // Mdc event display class
};

#endif  /* !HMDCEVTDISPLAY_H */

