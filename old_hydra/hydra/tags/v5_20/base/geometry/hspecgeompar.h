#ifndef HSPECGEOMPAR_H
#define HSPECGEOMPAR_H

#include "hparset.h"
#include "TNamed.h"
#include "TObjArray.h"

class HGeomVolume;
class HGeomShapes;

class HSpecGeomPar :  public HParSet {
protected:
  HGeomVolume* cave;   // volume describing the cave
  TObjArray* sectors;  // array of sector volumes (type HGeomVolume)
  TObjArray* targets;  // array of target volumes (type HGeomVolume)
  HGeomShapes* shapes; //! pointer to the shape classes
public:
  HSpecGeomPar(void);
  HSpecGeomPar(HGeomShapes*,const Int_t maxSec=0);
  ~HSpecGeomPar(void);
  Int_t getNumSectors(void);
  Int_t getNumTargets(void);
  HGeomVolume* getCave(void) {return cave;}
  HGeomVolume* getSector(const Int_t n);
  HGeomVolume* getTarget(const Int_t n);
  void addCave(HGeomVolume* v);
  void createTargets(const Int_t);
  void addTarget(HGeomVolume*);
  void addSector(HGeomVolume*);
  Bool_t init(HParIo*);
  Bool_t init(HParIo*,Int_t*);
  Int_t write(HParIo*);
  void clear(void);
  void printParam(void); 
  Int_t getSectorIndex(const Text_t*);
  Int_t getTargetIndex(const Text_t*);
  HGeomShapes* getShapes(void) {return shapes;}
  ClassDef(HSpecGeomPar,1) // geometry container for cave, sectors, targets
};

#endif /* !HSPECGEOMPAR_H */
