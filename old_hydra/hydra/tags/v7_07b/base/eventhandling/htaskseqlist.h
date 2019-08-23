#ifndef HTASKSEQLIST_H
#define HTASKSEQLIST_H

#include "htaskset.h"

class HTaskSeqList : public HTask {
protected:
  HTaskSet* tasklist;
  HTask *fCurrentTask;
public:
  HTaskSeqList(void);
  HTaskSeqList(Text_t name[],Text_t title[]);
  HTaskSeqList(HTaskSeqList&);
  ~HTaskSeqList(void);
  
  // initialisation
  Bool_t init(void);
  Bool_t finalize(void);
  Bool_t reinit(void);
  
  //Automatic sequential connection
  Bool_t connectTask(HTask *t,Int_t n); 
  Bool_t add( HTask* aTask);  

  // clearing of table
  void Clear(void);
  
  //Control execution flow
  HTask *next(Int_t &errCode);
  HTask *operator()(Int_t &errCode) {return next(errCode);}
  //characteristics
  HTask* getComposite() {return this;}
  TOrdCollection* getSetOfTask();
  //Dump of connections
  void print();
  void isTimed(Bool_t flag=kTRUE);
  void resetTimer(void);
  void printTimer(void); 

  ClassDef(HTaskSeqList,1) // Set of tasks in sequential order
};

#endif /* !HTASKSEQLIST_H */











