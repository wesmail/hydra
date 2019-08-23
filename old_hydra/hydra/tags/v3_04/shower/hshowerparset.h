#ifndef HSHOWERPARSET_H
#define HSHOWERPARSET_H

#include "hparset.h"
#include "hparhadasciifileio.h"

class HParIo;

class HShowerParSet : public HParSet {
protected:
public:
  HShowerParSet();
  virtual ~HShowerParSet() {}

  virtual Bool_t init(HParIo* inp,Int_t* set);  
  virtual Int_t write(HParIo*);

  ClassDef(HShowerParSet,1) // Base class for all parameter containers
protected:
  HAscii *m_pReadParam;
};

#endif  /* !HSHOWERPARSET_H */

