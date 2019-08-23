#ifndef HTASKSET_H
#define HTASKSET_H

#include <THashList.h>
#include <THashTable.h>
#include <TOrdCollection.h>
#include "htask.h"

class HTaskSet : public HTask {
protected:
  TOrdCollection fTasks; //Tasks in this set.
  HTask *fNextTask;
  HTask *fFirstTask;
 
public:
  HTaskSet(void);
  HTaskSet(HTaskSet &ts);
  HTaskSet(Text_t name[],Text_t title[]);
  ~HTaskSet(void);
  
  // initialisation
  Bool_t init(void);
  Bool_t finalize(void);
  Bool_t reinit(void);
  
  // connection via macros
  Bool_t connect(HTask *);
  Bool_t connect(HTask *,HTask *,Int_t n = kGOFORWARD);
  Bool_t connect(HTask *,Text_t *where,Int_t n = kGOFORWARD);
  Bool_t connect(Text_t task[],Text_t where[],Int_t n=0);
  Bool_t connectTask(HTask *,Int_t n);  
 // clearing of table
  void Clear(void);
  
  //Control execution flow
  HTask *next(Int_t &errCode);
  HTask *operator()(Int_t &errCode) {return next(errCode);}
  //characteristics
  HTask* getComposite() {return this;}
  TOrdCollection* getSetOfTask(){return &fTasks;}
  //Dump of connections
  void print();

  //Browsing
  Bool_t IsFolder(void) const;
  void Browse(TBrowser *b);
  void isTimed(Bool_t flag=kTRUE);
  void resetTimer(void);
  void printTimer(void);

  ClassDef(HTaskSet,1) // Set of tasks
};

#endif /* !HTASKSET_H */

