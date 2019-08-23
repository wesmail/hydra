#pragma interface
#ifndef HBRANCHORDCLONES_H
#define HBRANCHORDCLONES_H

#include <TBranchClones.h>

class HBranchOrdClones : public TBranchClones {
public:
  HBranchOrdClones(void);
  HBranchOrdClones(const Text_t *classname, 
                   void* clonesaddress,Int_t basketsize,Int_t compress=-1);
  ~HBranchOrdClones(void);
  virtual Int_t Fill();
  ClassDef(HBranchOrdClones,1) //Modified TBranchClones
};

#endif /* !HORDBRANCHCLONES */
