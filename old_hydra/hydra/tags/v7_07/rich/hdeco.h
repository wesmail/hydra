#ifndef HDECO_H
#define HDECO_H
#include "hreconstructor.h"
#include <TString.h>
class HCategory;
class HDeco : public HReconstructor {
public:
  HDeco(Text_t *name, Text_t *title,Option_t * sel);
  HDeco();
  virtual ~HDeco();
  Bool_t init();
 
  Bool_t finalize() {return kTRUE;}
  Int_t execute();
  TString sele;
ClassDef(HDeco,1)
};
#endif /* !HDECO_H */
