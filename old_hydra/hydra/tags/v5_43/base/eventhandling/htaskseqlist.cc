#include "htaskseqlist.h"
#include "hdebug.h"
#include <TROOT.h>
#include <TClass.h>

//*-- Author : D.Bertini <D.Bertini@gsi.de>

////////////////////////////////////////
// HTaskSeqList
// 
// This HTask is in fact a set of tasks sequentially connected among them.
// The tasks are connected among the so when a task is finished a pointer to
// the next task is retrieved. 
/////////////////////////////////////////

HTaskSeqList::HTaskSeqList(Text_t name[],Text_t title[]) : HTask(name,title) {
  // Constructor
  tasklist = new HTaskSet(name,title);  
  fCurrentTask = 0;
  fIsTimed = kFALSE;
}

HTaskSeqList::HTaskSeqList(void) {
  // Default constructor
  fIsTimed = kFALSE;
}

HTaskSeqList::HTaskSeqList(HTaskSeqList &ts) {
  // Copy constructor.
   tasklist=ts.tasklist;
   fCurrentTask=ts.fCurrentTask;
   fIsTimed=ts.fIsTimed;
}


HTaskSeqList::~HTaskSeqList(void) {
  // Destructor.
  if (tasklist) delete tasklist;
  fCurrentTask=0;
}

void HTaskSeqList::Clear(void) {
  tasklist->Clear();
  fCurrentTask=NULL;

}

Bool_t HTaskSeqList::init(void) {
  // Calls the init function for each of the tasks in the task set.
  if(fIsTimed) resetTimer();
  return tasklist->init();
}

Bool_t HTaskSeqList::reinit(void) {
  // Calls the init function for each of the tasks in the task set.
  return tasklist->reinit();
}

Bool_t HTaskSeqList::finalize(void) {
  // Calls the finalize function for each of the tasks in the set
  return tasklist->finalize();
}

void HTaskSeqList::print(void) {
  // Dumps dependencies for each of the tasks in the set
  printf("* * Seq Task List %15s * * * * * * * * * * * * * * * * * *\n",
                                                                    GetName());
  tasklist->print();
  printf("* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\n");
}



HTask *HTaskSeqList::next(Int_t &errCode) {
  // Iterates throught the taskset tasklist. 
  // When the iteration is finished it
  // returns a pointer to the next task as set by connectTask()
 return tasklist->next( errCode ); 

}



Bool_t HTaskSeqList::add( HTask* aTask ){
 // For the moment only elementary task are added
 // should be also a algo for other list
 // if task is empty than connect first task on the top
 

 if ((tasklist->getSetOfTask())->IsEmpty()){
   aTask->setOwner( tasklist );
   tasklist->connect( aTask );
   fCurrentTask=aTask; 
   return kTRUE;  
 } else {
   aTask->setOwner( tasklist );
   tasklist->connect(aTask,fCurrentTask,kGOFORWARD);
   fCurrentTask= aTask;
   return kTRUE;  
 }
}  


TOrdCollection* HTaskSeqList::getSetOfTask(){
  return tasklist->getSetOfTask();
}


Bool_t HTaskSeqList::connectTask(HTask *t,Int_t n){
  return tasklist->connectTask(t,n);
}


void HTaskSeqList::isTimed(Bool_t flag) {
  // Set timing flag for all tasks in list
  tasklist->isTimed(flag);
  fIsTimed = flag;
} 


void HTaskSeqList::resetTimer(void) {
  // Clear timers of all tasks in list
  if(fIsTimed) tasklist->resetTimer();
} 


void HTaskSeqList::printTimer(void) {
  // Print timers of all tasks in list
  tasklist->printTimer();
} 

ClassImp(HTaskSeqList)









