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
    return NULL;
  }

  return (HTask *)fOutputs[e];
}

Bool_t HReconstructor::connectTask(HTask *task,Int_t n) {
  //Connects the return value "n" to the task "task"
  if (!fOutputs[n]) 
    fOutputs.AddAt(task,n);
  else 
    return kFALSE;
  return kTRUE;
}



