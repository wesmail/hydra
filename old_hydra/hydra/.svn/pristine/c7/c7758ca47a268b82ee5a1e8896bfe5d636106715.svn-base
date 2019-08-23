#ifndef HRICHIPUPARLOCMAX_H
#define HRICHIPUPARLOCMAX_H

#include <TObject.h>
#include <TObjArray.h>
#include "hlocation.h"
#include "hparcond.h"

class HParamList; 
class HRichIPUParLocMax : public HParCond {
private: 
  Int_t locmax;  
  Int_t locmax_flag;  

public:
  HRichIPUParLocMax(const char* name="RichIPUParLocMax",
	      const char* title="Parameters for Rich IPU LocMax ",
	      const char* context="RICHIPUEmulation");
  ~HRichIPUParLocMax(void);
  
  Int_t getLocMax(){return locmax;}
  void setLocMax(Int_t lm){locmax=lm;}

  Int_t getLocMaxFlag(){return locmax_flag;}
  void setLocMaxFlag(Int_t fl){locmax_flag=fl;}

  Bool_t init(HParIo*,Int_t*);
  Int_t write(HParIo*);
  void putParams(HParamList*);
  Bool_t getParams(HParamList*);
  void clear();
  void printParam();
  void readline(const char*, Int_t*);  
  void printPause(void);

  void putAsciiHeader(TString&);
/*   Bool_t writeline(char*, Int_t, Int_t, Int_t);  */
  void setDefaults(void);

  ClassDef(HRichIPUParLocMax,1)  // Container for the RICH IPU LocMax parameters
};

#endif /* !HRICHIPUPARLOCMAX_H */











