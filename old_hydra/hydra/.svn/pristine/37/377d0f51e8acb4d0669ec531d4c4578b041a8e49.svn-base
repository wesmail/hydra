#ifndef HRICHPARSET_H
#define HRICHPARSET_H

#include "hparset.h"
#include "hparhadasciifileio.h"

class HParIo;

class HRichParSet : public HParSet {

public:

  HRichParSet();
  virtual ~HRichParSet() {}

  virtual Bool_t init(HParIo* inp, Int_t* set);  
  virtual Int_t write(HParIo*);

  ClassDef(HRichParSet,1) // Base class for all parameter containers

protected:

  HAscii *m_pReadParam;
};

#endif  // HRICHPARSET_H

