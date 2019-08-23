#if !defined HTOFGEOM_H
#define HTOFGEOM_H

#include "TObjArray.h"
#include "TAttFill.h"

class HTofGeom : public TObject, public TAttFill {

 protected:

  TObjArray  *pArray;
  Bool_t      kSetMax;
  Int_t       entriesMax;

 public:

  HTofGeom();
  virtual ~HTofGeom() {}
  TObjArray *getArray() const {return pArray;}
  void addSector(Int_t sec);
  void Paint(Option_t *t);
  void calcEntriesMax();
  void setEntriesMax(Int_t amax);
  Int_t getEntriesMax() { return entriesMax; }
  void setMaxFlag(Bool_t kflag) { kSetMax = kflag; }
  virtual Int_t DistancetoPrimitive(Int_t px, Int_t py);
  virtual void ExecuteEvent(Int_t event, Int_t px, Int_t py);
  void PaintPalette();
  void setLoopColors();

 public:
  
  ClassDef(HTofGeom,0)

};

#endif
