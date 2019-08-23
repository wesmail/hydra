#ifndef HLDSOURCE_H
#define HLDSOURCE_H

#include "hdatasource.h"
#include <TList.h>

class HldUnpack;
class HldEvt;

class HldSource : public HDataSource {
public:
  HldSource(void);
  virtual ~HldSource(void);
  virtual EDsState getNextEvent(void) {return  kDsError;}
  Bool_t initUnpacker(void);
  void addUnpacker(HldUnpack *unpacker);
  EDsState showIt(HldEvt *evt);
  EDsState dumpEvt(void); 
protected:
  Bool_t isDumped;
  void decodeHeader(void);
  TList* fUnpackerList; //! List of the unpackers used to extract data
  HldEvt* fReadEvent;   //! Buffer where the data is first read.
 
public:  
  inline void setDump(){isDumped=kTRUE;}
public:
  ClassDef(HldSource,0) //Data source to read LMD data
};
#endif /* !HLDSOURCE_H */

