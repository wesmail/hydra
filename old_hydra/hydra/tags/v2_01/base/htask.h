#ifndef HTASK_H
#define HTASK_H

#include <TNamed.h>

class HTask : public TNamed {
public:
  virtual Bool_t connectTask(HTask *task,Int_t n)=0;
public:
  HTask(void) {}
  HTask(Text_t name[],Text_t title[]) : TNamed(name,title) {}
  virtual ~HTask(void) {}
  virtual HTask *next(Int_t &errCode)=0;
  virtual Bool_t init(void)=0;
  virtual Bool_t finalize(void)=0;
  //void error(void);
  ClassDef(HTask,1) //ABC for a task
};


#endif /* !HTASK_H */
