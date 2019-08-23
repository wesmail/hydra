#ifndef HLDREMOTESOURCE_H
#define HLDREMOTESOURCE_H


#include "hldsource.h"

class HldRemoteSource : public HldSource {

protected:
  Text_t  *currNodeName;
  TIterator * iter;

public:
  HldRemoteSource(void);
  HldRemoteSource(Text_t *nodeName);
  ~HldRemoteSource(void);
   Bool_t init();  
  EDsState getNextEvent(void);
  Text_t* getNodeName(){return currNodeName;}
  
  ClassDef(HldRemoteSource,0) //Data source to read rpc buffers;
};
#endif /* !HLDREMOTESOURCE_H */
