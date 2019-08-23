#ifndef HTREE_H
#define HTREE_H

#include <TTree.h>
#include <TBranch.h>

class HTree : public TTree {
public:
  HTree(void);
  HTree(Text_t* name, Text_t* title, Int_t maxvirtualsize = 0);
  ~HTree(void);
  TBranch *GetBranch(const char * name);
  void  printContainerSizes(void);
  virtual Int_t MakeCode(const char* filename = 0);

  ClassDef(HTree,1) //TTree implementing a modified split algorithm
};

R__EXTERN TTree *gTree;
#endif /* !HTREE_H */




