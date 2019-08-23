#ifndef HTREE_H
#define HTREE_H

#include <TTree.h>
#include <TBranch.h>

class HTree : public TTree {
public:
  HTree(void);
  HTree(Text_t* name, Text_t* title, Int_t maxvirtualsize = 0);
  ~HTree(void);
  TBranch* makeBranch(Text_t* name, void* clonesaddress, Int_t bufsize = 32000, Int_t splitlevel = 1,int version=0);
  TBranch* makeBranch(const Text_t* name, const Text_t* classname, void* addobj, Int_t bufsize = 32000, Int_t splitlevel = 1);
  TBranch* makeBranch(Text_t* name, void* address, Text_t* leaflist, Int_t bufsize = 32000);
  TBranch* addBranch(Text_t* name, void* clonesaddress, Int_t bufsize = 32000, Int_t splitlevel = 1);
  TBranch* addBranch(const Text_t* name, const Text_t* classname, void* addobj, Int_t bufsize = 32000, Int_t splitlevel = 1);
  TBranch* addBranch(Text_t* name, void* address, Text_t* leaflist, Int_t bufsize = 32000);
  void  printContainerSizes(void);
  virtual Int_t MakeCode(char* filename = 0);
  ClassDef(HTree,1) //TTree implementing a modified split algorithm
};

R__EXTERN TTree *gTree;
#endif /* !HTREE_H */




