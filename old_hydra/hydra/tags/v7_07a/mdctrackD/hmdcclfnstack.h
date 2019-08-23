#ifndef HMDCCLFNSTACK_H
#define HMDCCLFNSTACK_H

#include "TObject.h"

class HMdcClFnStack : public TObject {
  protected:
    Int_t* stack;         // stack array
    Int_t* stackEnd;      // address of end stack
    Int_t* stackPos;      // address of stack position
    Int_t stackSize;      // stackSize;
  public:
    HMdcClFnStack(Int_t size) { stackSize=(size>0) ? size:1000; create(); }
    ~HMdcClFnStack();
    void init(void) {stackPos=stack;}
    void init(Int_t n) {stackPos=stack; *stackPos=n; stackPos++;}
    void push(Int_t n) {*stackPos=n; stackPos++;}
    Int_t pop(void) {return (stackPos>stack) ? *(--stackPos) : -1;}
    void checkSize(void) {if(stackPos>=stackEnd) increaseStack();}
  protected:
    void create(void);
    void increaseStack(void);
    
  ClassDef(HMdcClFnStack,0)
};
    

#endif  /*!HMDCCLFNSTACK_H*/
