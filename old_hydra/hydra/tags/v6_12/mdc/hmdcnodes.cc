//*-- Author : Ilse Koenig
//*-- Modified : 11/07/99 by Ilse Koenig

///////////////////////////////////////////////////////////////////////////////
// HMdcNodes
// HMdcCompNodes
// 
// classes for MDC geometry nodes
///////////////////////////////////////////////////////////////////////////////

#include "hmdcnodes.h"
#include "hmdclayergeompar.h"

ClassImp(HMdcCompNodes)
ClassImp(HMdcNodes)


HMdcCompNodes::HMdcCompNodes(Int_t n,Int_t maxLay) {
  // constructor creates a linear array to n nodes
  // takes a additional parameter the maximum number of layers 
  firstWire=maxLay+1;
  compNodes=new TObjArray(n);
}


HMdcCompNodes::~HMdcCompNodes(void) {
  // destructor deletes the array but not the nodes
  if (compNodes) delete compNodes;
} 


void HMdcCompNodes::clear() {
  // makes all wires invisible
  TNode* node=0;
  for(Int_t i=firstWire;i<compNodes->GetSize();i++) {
    node=(TNode*)((*compNodes)[i]);
    if (node) node->SetVisibility(0);
  }
}


HMdcNodes::HMdcNodes(Int_t s,Int_t m) {
  maxSec=s;
  maxMod=m;
  maxLay=0;
  maxWire=0;
  moduleNodes=new TObjArray(maxSec*maxMod);
}


HMdcNodes::~HMdcNodes(void) {
  removeCompNodes();
  delete moduleNodes;
  moduleNodes=0;
}


void HMdcNodes::createCompNodes(TArrayI& mods,Int_t l,Int_t w ) {
  maxLay=l;
  maxWire=w;
  for(Int_t i=0;i<maxSec;i++) {
    for(Int_t j=0;j<maxMod;j++) {
      Int_t n=i*maxMod+j;
      if (mods.At(n)>0)
        (*moduleNodes)[n]=
          new HMdcCompNodes((maxLay*maxWire+maxLay+1));
    }
  }
}


void HMdcNodes::removeCompNodes() {
  if (moduleNodes) {
    HMdcCompNodes* comp=0;
    for(Int_t i=0;i<moduleNodes->GetSize();i++) {
      comp=(HMdcCompNodes*)((*moduleNodes)[i]);
      if (comp) delete comp;
      comp=0;
    }
  }
}


void HMdcNodes::addNode(TNode* node,Int_t s,Int_t m,Int_t l,Int_t w) {
  // adds a node
  HMdcCompNodes* cn=0;
  if (s>=0) cn=(HMdcCompNodes*)(moduleNodes->At((s*maxMod+m)));
  else cn=(HMdcCompNodes*)(moduleNodes->At(m));
  if (cn) {
    if (w>=0) cn->set(node,1+maxLay+l*maxWire+w);
    else {
      if (l>=0) cn->set(node,l+1);
      else cn->set(node,0);
    }
  }
}


TNode* HMdcNodes::getNode(Int_t s,Int_t m,Int_t l,Int_t w) {
  // returns a node
  HMdcCompNodes* cn=(HMdcCompNodes*)(moduleNodes->At((s*maxMod+m)));
  if (cn) {
    if (w>=0) return (TNode*)(cn->get(1+maxLay+l*maxWire+w));
    else {
      if (l>=0) return (TNode*)(cn->get(l+1));
      else return (TNode*)(cn->get(0));
    }
  }
  else return 0;
}


void HMdcNodes::clear() {
  // calls the clear function of all modules
  HMdcCompNodes* nodes=0;
  for(Int_t i=0;i<moduleNodes->GetSize();i++) {
    nodes=(HMdcCompNodes*)((*moduleNodes)[i]);
    if (nodes) nodes->clear();
  } 
}
