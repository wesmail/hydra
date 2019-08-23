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
  Bool_t finalizeUnpacker(void);
  void addUnpacker(HldUnpack *unpacker);
  Bool_t finalize(void);
  EDsState showIt(HldEvt *evt);
  EDsState dumpEvt(void); 
  EDsState scanIt(HldEvt *evt);
  EDsState scanEvt(void); 
  Bool_t getDecodingStyle() {return oldDecodingStyle;}
  void setOldDecodingStyle(Bool_t decodingStyle=kTRUE)
    {oldDecodingStyle= decodingStyle;}
  void setScanned(Bool_t scanned=kTRUE) {isScanned=scanned;}  
protected:
  Bool_t isDumped,isScanned;
  Bool_t oldDecodingStyle; 
  void decodeHeader(void);
  TList* fUnpackerList; //! List of the unpackers used to extract data
  HldEvt* fReadEvent;   //! Buffer where the data is first read.
 
public:  
  inline void setDump(){isDumped=kTRUE;}
public:
  ClassDef(HldSource,0) //Data source to read LMD data
};
#endif /* !HLDSOURCE_H */

