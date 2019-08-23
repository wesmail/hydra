#ifndef HLDREMOTESOURCE_H
#define HLDREMOTESOURCE_H


#include "hldsource.h"

class HldRemoteSource : public HldSource {

protected:
  Int_t refId; // Reference run id for initialization
  Text_t  *currNodeName;
  TIterator * iter;
public:
  HldRemoteSource(void);
  HldRemoteSource(Text_t *nodeName);
  ~HldRemoteSource(void);
   Bool_t init();  
   Int_t getCurrentRunId(void);
   Int_t getCurrentRefId(void) {return refId;}
   void setRefId(Int_t r) {refId=r;}
   EDsState getNextEvent(Bool_t doUnpack=kTRUE);
   Text_t const *getCurrentFileName(void) {return ((const Char_t*)currNodeName);}
   Text_t* getNodeName(){return currNodeName;}

  ClassDef(HldRemoteSource,0) //Data source to read rpc buffers;
};
#endif /* !HLDREMOTESOURCE_H */
