#ifndef HLDFILESOURCE_H
#define HLDFILESOURCE_H

#include "hldsource.h"
#include "TIterator.h"

class HldFileSource : public HldSource {
protected:
  TIterator* iter;
public:
  HldFileSource(void);
  ~HldFileSource(void);
  Bool_t init();
  EDsState getNextEvent(void);
  Bool_t getNextFile();
  ClassDef(HldFileSource,0) //! Data source to read LMD files;
};
#endif /* !HLDFILESOURCE_H */
