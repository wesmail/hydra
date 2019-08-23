#include "hdebug.h"
#include "hnode.h"
#include "hevent.h"
#include "hades.h"
#include "htree.h"
#include <TBranchObject.h>
#include <TString.h>
#include <TBrowser.h>
//*-- Author : Manuel Sanchez
//*-- Modified : 14/04/98 by Manuel Sanchez
//*-- Copyright : GENP (Univ. Santiago de Comp.)

////////////////////////////////////
//HNode
// 
// This is an utility class to build generic trees (not Root trees) like
//an HSplitTree or a reconstruction tree.
//
// A tree is made of arcs (lines joining two nodes) and nodes (the points at
//the end or begin of a tree), so a HNode represents one node. This can be 
//clarified by the following picture:
//Begin_Html <img src="treeView.gif"> 
//End_Html
//
// So, a HNode supports its subnodes (its sons) and provides functions to 
//access those subnodes and to add new subnodes.
////////////////////////////////

ClassImp(HNode)

HNode::HNode(void) : TNamed ("node","generic node") {
  //Allocates a generic node
   fNSubNodes=0;
   fSubNodes=NULL;
}

HNode::HNode(Text_t *name,Text_t *title) : TNamed(name,title) {
  //Allocates a node with name "name" and title "title"
   fNSubNodes=0;
   fSubNodes=NULL;
}

HNode::~HNode(void) {
  // Deletes the subnodes hanging from this one
   if (fSubNodes) {fSubNodes->Delete(); delete fSubNodes;}
}

void HNode::makeBranch(TBranch *parent) {
  // Utility function to build Root trees from trees made from HNode's
  Int_t i=0;  
  if (fSubNodes!=NULL) {
    TObjArray &vector=*fSubNodes;
    HNode **node=NULL;
    TBranch *b=NULL;
    
    for (i=0;i<=vector.GetLast();i++) {
      node=(HNode **)&(vector[i]);
      if ( (*node)!=NULL) {
	b=gHades->getTree()->Branch((*node)->GetTitle(),
					(*node)->ClassName(),node,100,1);
	//parent->GetListOfBranches()->Add(b);
	(*node)->makeBranch(b);
      }
    }
  }
}

void HNode::activateBranch(TTree *tree) {
  // Activates the branches in the Root TTree "tree" that match those defined
  //in this node
  Int_t i=0;  
  if (fSubNodes!=NULL) {
    TObjArray &vector=*fSubNodes;
    HNode **node=NULL;
    
    for (i=0;i<=vector.GetLast();i++) {
      node=(HNode **)&(vector[i]);
      if ( (*node)!=NULL) {
	tree->SetBranchAddress((*node)->GetTitle(),node);
	tree->SetBranchStatus((*node)->GetTitle(),1);
	(*node)->activateBranch(tree);
      }
    }
  } 	
}

inline HNode *HNode::getSubNode(Int_t i) {
  //Returns the subnode number i
   if (fSubNodes && i<fNSubNodes)
      return( (HNode *)fSubNodes->UncheckedAt(i) );
   return NULL;
}

HNode *HNode::getFirstSubNode(void) {
  //Returns the first subnode
  if (fSubNodes)
    return ( (HNode *)fSubNodes->First());
  return NULL;
}

HNode *HNode::getLastSubNode(void) {
  //Returns the last subnode
  if (fSubNodes)
    return ( (HNode *)fSubNodes->Last());
  return NULL;
}

void HNode::addSubNode(HNode *aSon) {
  //Adds a new subnode (aSon) and modifies its title to include the title of
  //the parent node
   Text_t title[255];
   sprintf(title,"%s.%s",GetTitle(),aSon->GetTitle());
   aSon->SetTitle(title);
   if (fSubNodes) {fSubNodes->Add(aSon); fNSubNodes++;}
   else {
      fSubNodes=new TObjArray(10);      
      if (fSubNodes) {fSubNodes->Add(aSon); fNSubNodes++;}
   }
}

Bool_t HNode::addSubNodeAt(Int_t idx,HNode *aSon) {
  Text_t title[255];
  sprintf(title,"%s.%s",GetTitle(),aSon->GetTitle());
  aSon->SetTitle(title);
  if (fSubNodes) {fSubNodes->AddAtAndExpand(aSon,idx); fNSubNodes++;}
  else {
    fSubNodes=new TObjArray(10);      
    if (fSubNodes) {fSubNodes->AddAtAndExpand(aSon,idx); fNSubNodes++;}
    else return kFALSE;
  }
  return kTRUE;
}  

void HNode::Clear(void) {
  // Pass the clear signal to each subnode 
   if (fSubNodes) {
      TIter next(fSubNodes);
      HNode *node;
      while ( (node=(HNode *)next())!=NULL) {
         node->Clear();
      }
   }
}

void HNode::Delete(void){ 
  // Deletes subnodes
   if (fSubNodes) fSubNodes->Delete();
}

inline Bool_t HNode::IsFolder(void) const {
   return kTRUE;
}

inline void HNode::Browse(TBrowser *b) {
   if (!fSubNodes) return;
   TIter next(fSubNodes);
   HNode *node;
   while ( (node=(HNode *)next())!=NULL) b->Add(node,node->GetName());
}

//void HNode::Streamer(TBuffer &R__b)
//{
//   // Will be replaced by Root generated one in next version.

//   if (R__b.IsReading()) {
//      Version_t R__v = R__b.ReadVersion(); if (R__v) { }
//      TNamed::Streamer(R__b);
//      R__b >> fSubNodes;
//      R__b >> fNSubNodes;
//   } else {
//      R__b.WriteVersion(HNode::IsA());
//      TNamed::Streamer(R__b);
//      R__b << fSubNodes;
//      R__b << fNSubNodes;
//   }
//}
