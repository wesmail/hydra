#ifndef HGEANTSOURCE_H
#define HGEANTSOURCE_H

#include "hdatasource.h"
#include <TList.h>
#include <THashTable.h>

class HGeantReader;

class HGeantSource : public HDataSource {
private:
  TList fReaderList; //List of active geant readers.
  THashTable fFileTable; //Hash table with input files
public:
  HGeantSource(void);
  HGeantSource(HGeantSource &s);
  ~HGeantSource(void);
  Bool_t addGeantReader(HGeantReader *r,Text_t *inputFile);
  EDsState getNextEvent(void);
  Bool_t init(void);
  ClassDef(HGeantSource,1) //Data source to read GEANT ouput
};
#endif /* !HGEANTSOURCE_H */
