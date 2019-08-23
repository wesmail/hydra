#include "hlinearcategory.h"
#include "hlinearcatiter.h"
#include "hades.h"
#include "heventheader.h"
#include "htree.h"
#include "hfilter.h"
#include "hdataobject.h"
#include "hdebug.h"
#include <TClass.h>
#include <TTree.h>

//*-- Author : Manuel Sanchez
//*-- Modified : 16/12/99 by R. Holzmann
//*-- Modified : 24/8/99 by Manuel Sanchez
//*-- Modified : 26/02/99 by Ilse Koenig
//*-- Modified : 29/02/00 by Denis Bertini

/////////////////////////////
//HLinearCategory. See HCategory
//
//  The HLinearCategory is a kind of HCategory. This category is in fact a
// wrapper to a TClonesArray. Therefore it only supports locations with just
// one index. In order to use random access with getSlot(aLoc&), a Compress()
// of the category is needed before calling fillTree()  (otherwise crash!)
//
//  As for the output, one single branch is created for all the data. As usual
// this branch will have one subbranch for each data members in the class of
// the data objects contained in the category.
/////////////////////////////////

HLinearCategory::HLinearCategory(void) {
  fData=NULL;
  fNDataObjs=0;
}

HLinearCategory::HLinearCategory(Text_t *className,Int_t size) {
  fData=new TClonesArray(className,size);
  fNDataObjs=0;
}

HLinearCategory::~HLinearCategory(void) {
  if (fData) delete fData;
}

const Text_t *HLinearCategory::getClassName(void) {
  return fData->GetClass()->GetName();
}

void HLinearCategory::makeBranch(TBranch *parent) {
  Text_t name[255];
  if (parent==NULL) Error("makeBranch","parent is NULL");
  else {
    if (fHeader && gHades->getTree()) {
      TBranch *b=NULL;
      sprintf(name,"%s.Header",getClassName());
      b=gHades->getTree()->makeBranch(name,
				      fHeader->IsA()->GetName(),
				      &fHeader,2000,1);
      if (b) parent->GetListOfBranches()->Add(b);
    }
  }
}

void HLinearCategory::activateBranch(TTree *tree,Int_t splitLevel) {
  Text_t name[255];
  if (splitLevel) {
    TIterator *subBranches = NULL;
    TBranch* b = 0;
    sprintf(name,"%s.fData",getClassName());
    
    tree->SetBranchAddress(name,&fData);
    //tree->SetBranchStatus(name,1);
    subBranches = tree->GetBranch(name)->GetListOfBranches()->MakeIterator();
    while ( (b = (TBranch*)subBranches->Next()) != NULL) {
      tree->SetBranchStatus(b->GetName(),1);
    }
  }
//  if (fHeader && gHades->getTree()) {
  if (fHeader) {
    sprintf(name,"%s.Header*",getClassName());
    tree->SetBranchStatus(name,1);
    tree->SetBranchAddress(name,&fHeader);
  }
}

TObject *&HLinearCategory::getNewSlot(HLocation&,Int_t* pIndex) {
  if(pIndex) *pIndex = fNDataObjs;
  return fData->operator[](fNDataObjs++);  // return next sequential slot
}

TObject *&HLinearCategory::getSlot(HLocation &aLoc,Int_t* pIndex) {  
                                              // needs Compress()
  TObject *&r=fData->operator[](aLoc[0]);
  if (!r) fNDataObjs++;
  if(pIndex) *pIndex = aLoc[0];
  return r;
}

TObject *&HLinearCategory::getSlot(Int_t index) {  // needs Compress()
  TObject *&r= (*fData)[index];       // bound check + expand if necessary
  if (!r) fNDataObjs++;
  return r;
}

HDataObject *HLinearCategory::getObject(HLocation &aLoc) {
  return (HDataObject *)fData->At(aLoc[0]);
}

HDataObject *HLinearCategory::getObject(Int_t index) {
  return (HDataObject *)fData->At(index);     // no bound check
}


Bool_t HLinearCategory::filter(HFilter &aFilter) {
  Int_t i;
  Bool_t r=kTRUE;
  for (i=0;i<fData->GetLast();i++) {
    if (!aFilter.check((HDataObject *)fData->UncheckedAt(i))) 
      fData->RemoveAt(i);
  }
  fData->Compress();
  return r;
}

Bool_t HLinearCategory::filter(HLocation &aLoc,HFilter &aFilter) {
  Int_t i;
  Bool_t r=kTRUE;
  if (aLoc.getNIndex()==0) {
    for (i=0;i<fData->GetLast();i++) {
      if (!aFilter.check((HDataObject *)fData->UncheckedAt(i))) 
	fData->RemoveAt(i);
    }
    fData->Compress();
  } else r=kFALSE;    
  return r;
}

void HLinearCategory::Clear(void) {
  fData->Clear(); 
  fNDataObjs=0;
}

inline Bool_t HLinearCategory::isSelfSplitable(void) {
 return kFALSE;
}

TIterator *HLinearCategory::MakeIterator(Option_t *opt,Bool_t dir) {
  TString option = opt;
  option.ToLower(); 
  if (option.Contains("native")) return fData->MakeIterator(dir);

  HLinearCatIter *iterator=NULL;
  iterator=new HLinearCatIter(this,dir);
  return iterator;
}

void HLinearCategory::Streamer(TBuffer &R__b)
{
   // Stream an object of class HLinearCategory.
  Char_t clase[200];
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(); if (R__v) { }
      HCategory::Streamer(R__b);
      R__b.ReadString(clase,200);
      if ( fData && strcmp(clase,fData->GetClass()->GetName())==0)
       fData->Clear();
      else {
       delete fData;
       fData=new TClonesArray(clase);
      }
      fData->Streamer(R__b);
   } else {
      R__b.WriteVersion(HLinearCategory::IsA());
      HCategory::Streamer(R__b);
      strcpy(clase,fData->GetClass()->GetName());
      R__b.WriteString(clase);
      fData->Streamer(R__b);
   }
}

ClassImp(HLinearCategory)
