#if !defined HCONFIGURE_H
#define HCONFIGURE_H

#include <TObject.h>

class HConfigure : public TObject {

 public:

  Int_t fConfig[27];
  Text_t input[200];
  Text_t output[200];

 public:

  HConfigure() : TObject() {}
  virtual ~HConfigure() {}
  Bool_t configure();

 public:

  ClassDef(HConfigure,0)

};

#endif

