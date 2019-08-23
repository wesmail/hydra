#include "hmdcclfnstack.h"

//*-- AUTHOR : Pechenov Vladimir
//*-- Modified : 06/11/2002 by V.Pechenov

////////////////////////////////////////////////////////////////
// HMdcClFnStack
//
// Stack for claster finder
//
////////////////////////////////////////////////////////////////

ClassImp(HMdcClFnStack)

void HMdcClFnStack::create(void) {
  stack=new Int_t [stackSize+100];
  stackPos=stack;
  stackEnd=stack+stackSize;
}

HMdcClFnStack::~HMdcClFnStack() {
  if(stack) delete [] stack;
  stack=0;
}

void HMdcClFnStack::increaseStack(void) {
  Warning("increaseStack","Stack increased on 50%c",'%');
  Int_t* oldStack=stack;
  Int_t* oldStackPos=stackPos;
  stackSize += stackSize/2;
  create();
  for(Int_t* n=oldStack; n<oldStackPos; n++) push(*n);
  delete [] oldStack;
}
