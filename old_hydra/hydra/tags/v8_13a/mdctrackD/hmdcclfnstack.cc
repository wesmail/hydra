#include "hmdcclfnstack.h"

//*-- AUTHOR : Pechenov Vladimir
//*-- Modified : 06/11/2002 by V.Pechenov

//_HADES_CLASS_DESCRIPTION 
////////////////////////////////////////////////////////////////
// HMdcClFnStack
//
// Stack for claster finder
//
////////////////////////////////////////////////////////////////

ClassImp(HMdcClFnStack)
ClassImp(HMdcClFnStacksArr)

HMdcClFnStack::HMdcClFnStack(void) {
 stack     = 0;  
 stackEnd  = 0;
 stackPos  = 0;
 stackSize = 0;
}

HMdcClFnStack::HMdcClFnStack(Int_t size) {
  stackSize=(size>0) ? size:1000;
  create();
}

void HMdcClFnStack::setSize(Int_t size) {
  Int_t stSize=(size>0) ? size:1000;
  if(stSize < stackSize) return;
  stackSize = stSize;
  create();
}

void HMdcClFnStack::create(void) {
  if(stack) delete [] stack;
  stack=new Int_t [stackSize+10];
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
  stack = 0;
  create();
  for(Int_t* n=oldStack; n<oldStackPos; n++) push(*n);
  delete [] oldStack;
}

//===================================================================

HMdcClFnStacksArr::HMdcClFnStacksArr(Int_t size) {
  for(Int_t am=0;am<24;am++) {
    Int_t sz=size/(am*4+1);
    if(sz<200) sz=200;
    stacksArr[am].setSize(sz);
  }
}

void HMdcClFnStacksArr::init(Int_t maxAmp) {
  amplitude    = 0;
  maxAmplitude = maxAmp-1;
  for(Int_t am=0;am<maxAmp;am++) stacksArr[am].init();
}

void HMdcClFnStacksArr::init(Int_t maxAmp,Int_t n,Int_t amp) {
  init(maxAmp);
  push(n,amp);
}

Int_t HMdcClFnStacksArr::pop(Int_t& binNum) {
  // return amplitude !!!
  binNum = stacksArr[amplitude].pop();
  if(binNum>=0) return amplitude+1;
  if(amplitude == 0) return -1;
  amplitude--;
  return pop(binNum);
}
