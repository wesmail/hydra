#ifndef HDEBUG_H
#define HDEBUG_H

#include <TObject.h>

class HDebug : public TObject {
private:
  Int_t fIdentLevel; //identation level;
public:
  HDebug(void);
  ~HDebug(void);
  void setIdentLevel(Int_t level) {fIdentLevel=level;}
  Int_t getIdentLevel(void) {return fIdentLevel;}
  void incIdentLevel(void) {fIdentLevel++;}
  void decIdentLevel(void) {fIdentLevel--;}
  void message(Text_t *format,...);
  void enterFunc(Text_t *func);
  void leaveFunc(Text_t *func);
  //  void warning(Text_t *where,Text_t *format,...);
  //void error(Text_t *where,Text_t *format,...);
  ClassDef(HDebug,0) //Class for debugging;
};

EXTERN HDebug *gDebuger;

#endif /* !HDEBUG_H */
