#pragma interface
#ifndef HBRANCHCLONES_H
#define HBRANCHCLONES_H

#include <TBranchClones.h>

class HBranchClones : public TBranchClones {
public:
  HBranchClones(void);
  HBranchClones(const Text_t *classname, 
                   void* clonesaddress,Int_t basketsize,Int_t compress=-1);
  ~HBranchClones(void);
  virtual Int_t Fill();
  ClassDef(HBranchClones,1) //Modified TBranchClones
};

#endif /* !HBRANCHCLONES */
