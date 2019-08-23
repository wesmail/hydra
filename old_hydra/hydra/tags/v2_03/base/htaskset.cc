#include "htaskset.h"
#include "hdebug.h"

//*-- Author : Manuel Sanchez
//*-- Modified : 9/11/1998

////////////////////////////////////////
//HTaskSet
// 
// This HTask is in fact a set of tasks arbitrarily connected among them.
//
// The tasks are connected among the so when a task is finished a pointer to
//the next task is retrieved; note that in general the tasks are not connected
//in a linear fashion.
/////////////////////////////////////////

HTaskSet::HTaskSet(Text_t name[],Text_t title[]) : HTask(name,title) {
  //Constructor
}

HTaskSet::HTaskSet(void) {
  //Default constructor
}

HTaskSet::HTaskSet(HTaskSet &ts) {
  //Copy constructor.
  fFirstTask=ts.fFirstTask;
  fNextTask=ts.fNextTask;
  fTasks.AddAll(&ts.fTasks);
}

HTaskSet::~HTaskSet(void) {
  //Destructor.
  fTasks.Delete();
}

Bool_t HTaskSet::connect(HTask *task) {
  //Connects "task" as the first task to be performed in the set.
  fTasks.Add(task);
  fFirstTask=task;
  return kTRUE;
}

Bool_t HTaskSet::connect(HTask *task,HTask *where,Int_t n) {
  //connects task "task" to the task "where" with parameter "n"
  if (where) {
    if (fTasks.FindObject(where)) {
      where->connectTask(task,n);
      if (task) fTasks.Add(task);
    } else return kFALSE;
  } else {
    fTasks.Add(task);
    fFirstTask=task;
  }
  return kTRUE;
}

Bool_t HTaskSet::connect(HTask *task,Text_t *where,Int_t n=0) {
  //Connects task "task" to the task with name "where" using "n" as parameter.
  HTask *wh=NULL;
  wh=(HTask *)fTasks.FindObject(where);
  if (wh) {
    wh->connectTask(task,n);
    if (task) fTasks.Add(task);
  } else return kFALSE;
  return kTRUE;
}

Bool_t HTaskSet::connect(Text_t task[],Text_t where[],Int_t n=0) {
  //Connects the task named "task" to the one named "where" using "n" as 
  //paramter.
  HTask *ta=NULL,*wh=NULL;
  wh=(HTask *)fTasks.FindObject(where);
  ta=(HTask *)fTasks.FindObject(task);
  if (ta && wh) {
    wh->connectTask(ta,n);
  } else return kFALSE;
  return kTRUE;
}

void HTaskSet::Clear(void) {
#warning "HTaskSet::Clear should clear not delete..."
  fTasks.Delete();
  fFirstTask=NULL;
}

Bool_t HTaskSet::init(void) {
  //Calls the init function for each of the tasks in the task set.
  TIter next(&fTasks);
  HTask *task=NULL;
  while ( (task=(HTask *)next())!=NULL) {
    if (!task->init()) return kFALSE;
  }
  return kTRUE;
}

Bool_t HTaskSet::finalize(void) {
  //Calls the finalize function for each of the tasks in the set
  TIter next(&fTasks);
  HTask *task=NULL;
  while ( (task=(HTask *)next())!=NULL) {
    if (!task->finalize()) return kFALSE;
  }
  return kTRUE;
}

HTask *HTaskSet::next(Int_t &) {
  //Iterates throught the task set. When the iteration is finished it
  //returns a pointer to the next task as set by connectTask()
  HTask *task;
  Int_t err;
  task=fFirstTask;
  while (task) {
    task=task->next(err);
  }
  return fNextTask;
}

Bool_t HTaskSet::connectTask(HTask *task,Int_t) {
  //Connects "task" as the next task to be performed; n is ignored.
  fNextTask=task;
  return kTRUE;
}

ClassImp(HTaskSet)
