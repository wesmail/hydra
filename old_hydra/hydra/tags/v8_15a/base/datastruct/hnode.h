#ifndef HNODE_H
#define HNODE_H

#include <TNamed.h>
#include <TObjArray.h>
#include <TBranch.h>
#include <TObjString.h>
#include <TTree.h>
#include <TBrowser.h>

class HNode : public TNamed {
protected:
  TObjArray *fSubNodes; //|| Subnodes of this node
  Int_t fNSubNodes; //|| Number of subnodes for this node
public:
  HNode(void);
  HNode(Text_t *name,Text_t *title);
  ~HNode(void);
  virtual void makeBranch(TBranch *parent);
  virtual void activateBranch(TTree *tree);
  //virtual Bool_t activateBranch(void);
  HNode *getSubNode(Int_t i);
  HNode *getFirstSubNode(void);
  HNode *getLastSubNode(void);
  //HRecNode &get(Text_t *name);
  //void setNSons(Int_t n);
  void addSubNode(HNode *aSon);
  Bool_t addSubNodeAt(Int_t idx,HNode *aSon);
  //void removeSon(Int_t idx);
  //Int_t getNSons(void);
  virtual void Clear(void); // Cleans
  virtual void Delete(void); // Remove all sons and cleans
  inline Bool_t IsFolder(void) const;
  inline virtual void Browse(TBrowser *b);
  ClassDef(HNode,1) //Base class for nodes in tree structures
};

//---------------------inlines------------------------------
inline Bool_t HNode::IsFolder(void) const {
   return kTRUE;
}

inline void HNode::Browse(TBrowser *b) {
   if (!fSubNodes) return;
   TIter next(fSubNodes);
   HNode *node;
   while ( (node=(HNode *)next())!=NULL) b->Add(node,node->GetName());
}

#endif /* !HNODE */
