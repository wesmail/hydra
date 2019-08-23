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
  //Constructor
tasklist = new HTaskSet(name,title);  
fCurrentTask = 0;
}

HTaskSeqList::HTaskSeqList(void) {
  //Default constructor
}

HTaskSeqList::HTaskSeqList(HTaskSeqList &ts) {
  //Copy constructor.
   tasklist=ts.tasklist;
   fCurrentTask=ts.fCurrentTask;
}


HTaskSeqList::~HTaskSeqList(void) {
  //Destructor.
  if (tasklist) delete tasklist;
  fCurrentTask=0;
}

void HTaskSeqList::Clear(void) {
  tasklist->Clear();
  fCurrentTask=NULL;

}

Bool_t HTaskSeqList::init(void) {
  //Calls the init function for each of the tasks in the task set.
  return tasklist->init();
}

Bool_t HTaskSeqList::reinit(void) {
  //Calls the init function for each of the tasks in the task set.
  return tasklist->reinit();
}

Bool_t HTaskSeqList::finalize(void) {
  //Calls the finalize function for each of the tasks in the set
  return tasklist->finalize();
}

void HTaskSeqList::print(void) {
  //Dump dependencies for each of the tasks in the set
   tasklist->print();
}



HTask *HTaskSeqList::next(Int_t &errCode) {
  //Iterates throught the taskset tasklist. 
  // When the iteration is finished it
  //returns a pointer to the next task as set by connectTask()
 return tasklist->next( errCode ); 

}



Bool_t HTaskSeqList::add( HTask* aTask ){
 //For the moment only elementary task are added
 //should be also a algo for other list
 //if task is empty than connect first 
 //task on the top
 

 if ((tasklist->getSetOfTask())->IsEmpty()){
     tasklist->connect( aTask );
     //printf("connecting in top task name1 %s:",aTask->GetName()); 
     // printf("task owner1 %5s;",aTask->getOwner()->GetName());
     fCurrentTask=aTask; 
     return kTRUE;  
  }else{
     aTask->setOwner( tasklist );
     //  printf(" current task %s \n", fCurrentTask->GetName());
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

ClassImp(HTaskSeqList)
