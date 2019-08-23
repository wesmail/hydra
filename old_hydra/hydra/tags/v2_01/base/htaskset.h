#ifndef HTASKSET_H
#define HTASKSET_H

#include <THashTable.h>
#include "htask.h"

class HTaskSet : public HTask {
protected:
  THashTable fTasks; //Tasks in this set.
  HTask *fNextTask;
  HTask *fFirstTask;
  Bool_t connectTask(HTask *,Int_t n);

public:
  HTaskSet(void);
  HTaskSet(HTaskSet &ts);
  HTaskSet(Text_t name[],Text_t title[]);
  ~HTaskSet(void);
  Bool_t connect(HTask *);
  Bool_t connect(HTask *,HTask *,Int_t n=0);
  Bool_t connect(HTask *,Text_t *where,Int_t n=0);
  Bool_t connect(Text_t task[],Text_t where[],Int_t n=0);
  Bool_t init(void);
  Bool_t finalize(void);
  void Clear(void);
  HTask *next(Int_t &errCode);
  HTask *operator()(Int_t &errCode) {return next(errCode);}
  ClassDef(HTaskSet,1) //Set of tasks
};

#endif /* !HTASKSET_H */

