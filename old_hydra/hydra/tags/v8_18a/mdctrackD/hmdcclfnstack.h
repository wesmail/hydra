#ifndef HMDCCLFNSTACK_H
#define HMDCCLFNSTACK_H

#include "TObject.h"

class HMdcClFnStack : public TObject {
  protected:
    Int_t* stack;         // stack array
    Int_t* stackEnd;      // address of end stack
    Int_t* stackPos;      // address of stack position
    Int_t  stackSize;     // stackSize;
  public:
    HMdcClFnStack(void);
    HMdcClFnStack(Int_t size);
    ~HMdcClFnStack();
    void  setSize(Int_t size);
    void  init(void)      {stackPos=stack;}
    void  init(Int_t n)   {stackPos=stack; *stackPos=n; stackPos++;}
    void  push(Int_t n)   {*stackPos=n; stackPos++; checkSize();}
    Int_t pop(void)       {return (stackPos>stack) ? *(--stackPos) : -1;}
    void  checkSize(void) {if(stackPos>=stackEnd) increaseStack();}
  protected:
    void  create(void);
    void  increaseStack(void);
    
  ClassDef(HMdcClFnStack,0)
};

class HMdcClFnStacksArr : public TObject {
  protected:
    HMdcClFnStack stacksArr[24]; //
    Int_t         maxAmplitude;
    Int_t         amplitude;
  public:
    HMdcClFnStacksArr(Int_t size);
    ~HMdcClFnStacksArr() {}
    void  init(Int_t maxAmp=24);
    void  init(Int_t maxAmp,Int_t n,Int_t amp);
    void  push(Int_t n,Int_t amp)     {amp--; stacksArr[amp].push(n);
                                       if(amp>amplitude) amplitude=amp;}
    Int_t pop(Int_t& binNum);
    HMdcClFnStack* getOneStack(void) {return stacksArr;}
  protected:
    
  ClassDef(HMdcClFnStacksArr,0)
};  

#endif  /*!HMDCCLFNSTACK_H*/
