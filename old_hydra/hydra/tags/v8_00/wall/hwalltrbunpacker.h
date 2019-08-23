#ifndef HWALLTRBUNPACKER_H
#define HWALLTRBUNPACKER_H

#include "htrbbaseunpacker.h"
#include "hlocation.h"

class HTrbLookup;

class HWallTrbUnpacker: public HTrbBaseUnpacker {

protected:
  HLocation loc;
  Int_t debugWallFlag; 

public:
  HWallTrbUnpacker(UInt_t id=416);
  ~HWallTrbUnpacker(void) { }

  Int_t execute(void); 
  Bool_t init(void);
  void setDebugWallFlag(Int_t db){ debugWallFlag = db;}

public:
  ClassDef(HWallTrbUnpacker,0) // Unpacker for Foward Wall data
};          

#endif /* !HWALLTRBUNPACKER_H */
