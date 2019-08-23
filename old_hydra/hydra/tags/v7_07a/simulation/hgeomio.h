#ifndef HGEOMIO_H
#define HGEOMIO_H

#include "TObject.h"

class HGeomMedia;
class HGeomSet;
class HGeomHit;
class HGeomInterface;

class HGeomIo : public TObject {
public:
  virtual Bool_t open(const char*,Text_t* status="in")=0;
  virtual void close()=0;
  virtual void print()=0;
  virtual Bool_t isOpen()=0;
  virtual Bool_t isWritable()=0;
  virtual Bool_t read(HGeomMedia*)=0;
  virtual Bool_t read(HGeomSet*,HGeomMedia*)=0;
  virtual Bool_t read(HGeomHit*)=0;
  virtual Bool_t write(HGeomMedia*)=0;
  virtual Bool_t write(HGeomSet*)=0;
  virtual Bool_t readGeomConfig(HGeomInterface*)=0; 
  virtual Bool_t setSimulRefRun(const char*)=0;
  virtual Bool_t setHistoryDate(const char*)=0;
private:
  ClassDef(HGeomIo,0) // Abstract base class for geometry I/O
};

#endif  /* !HGEOMIO_H */
