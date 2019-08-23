#include <TObject.h>
#include "hades.h"
#include "hreconstructor.h"
#include <TClass.h>
//*-- Author : Manuel Sanchez
//*-- Modified : 9/10/1998 by Manuel Sanchez
//*-- Modified : 27/04/98 by Manuel Sanchez
//*-- Copyright : GENP (Univ. Santiago de Compostela)

////////////////////////////////////
//HReconstructor
//
//  Base class for any reconstruction method or algoritm. See HTask for
//more information.
//
////////////////////////////////////

ClassImp(HReconstructor)

HReconstructor::HReconstructor(void) : HTask("Reconstructor","Generic reconstructor") {
  //Default reconstructor
    fActive=kTRUE;
    fHistograms=NULL;
}

HReconstructor::HReconstructor(Text_t *name,Text_t *title) : HTask(name,title){
  //Allocates a new reconstructor with name=name and Title=title
   fActive=kTRUE;
   fHistograms=NULL;
}

HReconstructor::~HReconstructor(void) {
  //Destructor
}

HTask *HReconstructor::next(Int_t &errCode) {
  //Returns next task to be performed according to the return value of the
  //execute() function
  Int_t e=(fActive)?execute():0;
  if (e<0) {
    errCode=e;
    printf("task :%s ",this->GetName());
    printf("exited with errCode:%i \n",errCode);
    return NULL;
  }
  errCode=0;
  return (HTask *)fOutputs[e];
}

Bool_t HReconstructor::connectTask(HTask *task,Int_t n) {
  //Connects the return value "n" to the task "task"
  if (!fOutputs[n]) 
    fOutputs.AddAt(task,n);
  else { 
    printf(" overwritten fOutputs !!!\n"); 
    return kFALSE;
  }
  return kTRUE;
}

void HReconstructor::getConnections(void){

Int_t entries = fOutputs.GetEntries();
 printf("--------------------------------------------------------------\n");
 printf(" Connected Tasks from   %10s                 \n",this->GetName());
 printf("--------------------------------------------------------------\n");  
 if( entries == 0 ) printf("no connected tasks \n");
 for(Int_t i=0; i<=fOutputs.GetLast();i++){ 
 if (fOutputs[i])
 printf("Task: %3s is connected with ctrl:%3i \n", fOutputs[i]->GetName(), i); 
  }
}


