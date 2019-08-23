#ifndef HLDSOCKREMOTESOURCE_H
#define HLDSOCKREMOTESOURCE_H


#include "hldsource.h"

class HldSockRemoteSource : public HldSource {

protected:
  Int_t refId; // Reference run id for initialization
  Text_t  *currNodeName;
  TIterator * iter;
  Int_t evtMax;
public:
  HldSockRemoteSource(void);
  HldSockRemoteSource(Text_t *nodeName,Int_t evtMax=10000000);
  ~HldSockRemoteSource(void);
   Bool_t init();  
  EDsState getNextEvent(void);
  Text_t* getNodeName(){return currNodeName;}
  Int_t getCurrentRunId(void);
  Int_t getCurrentRefId(void){return refId;}
  void setRefId(Int_t r) {refId=r;} 
  Text_t const *getCurrentFileName(void){return NULL;}


  ClassDef(HldSockRemoteSource,0) //Data source to read rpc buffers;
};
#endif /* !HLDSOCKREMOTESOURCE_H */
