#include "hmatrixcategory.h"
#include "hmatrixcatiter.h"
#include "hades.h"
#include <TBrowser.h>
#include <TClonesArray.h>
#include <TClass.h>
#include "hlocation.h"
#include "hmatrixcatiter.h"
#include "htree.h"
#include "hindextable.h"
#include "hfilter.h"
#include "hdebug.h"
#include "hdataobject.h"
//*-- Author : Manuel Sanchez
//*-- Modified : 18/05/98 by Manuel Sanchez
//*-- Modified:  05/12/98  by D.Bertini (problem with clear and multiple hits)
//*-- Copyright : GENP (Univ. Santiago de Compostela)


////////////////////////////////////
//HMatrixCategory
//
//  In this particular HCategory all the objects belonging to the category
// are stored in a matrix-like fashion; so before start using the category it 
// is needed to indicate the number of indexes in this matrix and the range of
// each index
//
//  When the matrix category is stored in a tree it creates one superbranch 
// holding one subbranch per data member in the objects held by the category. 
// All the objects held by the category are stored through that superbranch. 
// 
////////////////////////////////////

ClassImp(HMatrixCategory)

HMatrixCategory::HMatrixCategory(void)  {
  //Default constructor
 fData=NULL;
 fIndexTable=new HIndexTable;
}

HMatrixCategory::HMatrixCategory(Text_t *className,
				 Int_t nSizes,
				 Int_t *sizes,
				 Float_t fillRate=0.1) 
{
  // Creates a HMatrixCategory holding objects of class "className" clasified
  //by nSizes indexes (giving their location) whose maximun values are the
  //indicated in the vector "sizes".
  //
  // However it only allocates memory for fillRate*(maximun number of objects)
  //data objects.
  fData=NULL;
  fIndexTable=new HIndexTable;
  setup(className,nSizes,sizes,fillRate);
}

HMatrixCategory::~HMatrixCategory(void) {
  //Destructor
  if (fData) {fData->Clear(); delete fData;}
  if (fIndexTable) delete fIndexTable;
}

void HMatrixCategory::setup(Text_t *className,Int_t nSizes,Int_t *sizes,Float_t fillRate=0.1) {
  //This method sets up a category.
  //
  //Input:
  //  className ---> a string with the name of the object's class stored in 
  //                 the category
  //  nSizes ---> Number of indexes needed to access an object (fBranchingLevel)
  //  sizes ---> a vector of ints containing the maximum value for each of the
  //             indexes before.
  //  fillRate---> Expected fill factor in the category 
  //               (number of slots used)/(maximun number of slots)
#if DEBUG_LEVEL>2
  gDebuger->enterFunc("MatrixCategory::setup");
#endif
 fBranchingLevel=nSizes;
 fIndexTable->setDimensions(nSizes,sizes);
 fNDataObjs=0;
#if DEBUG_LEVEL>2
 gDebuger->message("Allocation data clones array");
#endif
 if (fData) delete fData;
 fData=new TClonesArray(className,(Int_t)(fIndexTable->getEntries()*fillRate));
#if DEBUG_LEVEL>2
 gDebuger->leaveFunc("HMatrixCategory::setup");
#endif
}

const Text_t *HMatrixCategory::getClassName(void) {
  //Returns the class' name of the objects held by the category
 if (fData)
  return fData->GetClass()->GetName();
 return NULL;
}

TClass *HMatrixCategory::getClass(void) {
  if (fData)
    return fData->GetClass();
  return NULL;
}

TArrayI *HMatrixCategory::getSizes(void) {
  //returns an array of ints with the maximun value for the indexes used to 
  //get an object
 return fIndexTable->getDimensions();
}

Int_t HMatrixCategory::getSize(Int_t aIdx) {
  //Returns the maximun value of the index aIdx
 return (fIndexTable->getDimensions()->At(aIdx));
}

void HMatrixCategory::activateBranch(TTree *tree,Int_t splitLevel) {
  //Activates the branches in tree matching the appropiate names.
  Text_t name[255];
#if DEBUG_LEVEL>2
  gDebuger->enterFunc("HMatrixCategory::activateBranch");
#endif
  if (splitLevel) {
    TIterator *subBranches=NULL;
    TBranch *b=NULL; 
    sprintf(name,"%s.fData",getClassName());
    
    tree->SetBranchAddress(name,&fData);
    tree->SetBranchStatus(name,1);
    subBranches=tree->GetBranch(name)->GetListOfBranches()->MakeIterator();
    while ( (b=(TBranch *)subBranches->Next())!=NULL) {
      tree->SetBranchStatus(b->GetName());
    }

    sprintf(name,"%s_",name);
    //tree->SetBranchStatus(name,1);
    if (fHeader && gHades->getTree()) {
      sprintf(name,"%s.Header",getClassName());
      tree->SetBranchStatus(name,1);
      tree->SetBranchAddress(name,&fHeader);
    }
  }
#if DEBUG_LEVEL > 2
  gDebuger->leaveFunc("HMatrixCategory::activateBranch");
#endif
}

void HMatrixCategory::makeBranch(TBranch *parent) {
  //This function should not be called, since HMatrixCategory is not self
  //splittable
  Text_t name[255];
  if (fHeader && gHades->getTree()) {
    TBranch *b=NULL;
    sprintf(name,"%s.Header",getClassName());
    b=gHades->getTree()->makeBranch(name,
				  fHeader->IsA()->GetName(),&fHeader,4000,1);
    if (b) parent->GetListOfBranches()->Add(b);
  }
}

Bool_t HMatrixCategory::isSelfSplitable(void) {
  //Determines if the category can be automatically split or it has
  //its own splitting algorithm (in this last case the function makeBranch is 
  //called for splitting).
  //
  //returns kFALSE
  return kFALSE;
}

TObject *&HMatrixCategory::getNewSlot(HLocation &aLoc) {
  //Should not be used for HMatrixCategory
  Int_t la=0,lae=0,pos=0;
  TClonesArray &data=*fData;
  la=aLoc.getLinearIndex(fIndexTable->getDimensions());
  lae=la+fIndexTable->getDimensions()->At(fBranchingLevel-1);
  while (la<lae) {
    if (fIndexTable->getIndex(la)==-1) {
      pos=fNDataObjs;
      fNDataObjs++;
      fIndexTable->setIndex(la,pos);
      return data[pos];
    }
    la++;
  }
  return gNullObject;
}

TObject *&HMatrixCategory::getSlot(HLocation &aLoc) {
  //Returns a memory address corresponding to the HLocation aLoc, where a new
  //object can be instantiated
 Int_t la=0,pos=0,idx;
 TClonesArray &data=*fData;

 if (aLoc.getNIndex()<fBranchingLevel) return gNullObject;
#if DEBUG_LEVEL>0
 if (!fIndexTable->checkLocation(aLoc)) return gNullObject;
#endif

 la=aLoc.getLinearIndex(fIndexTable->getDimensions());
 idx=fIndexTable->getIndex(la);
 if (idx==-1) {
   pos=fNDataObjs;
   fNDataObjs++;
   fIndexTable->setIndex(la,pos);
 } else {
   pos=idx;
 }
 return data[pos];
}

HDataObject *HMatrixCategory::getObject(HLocation &aLoc) {
  //Returns the object at the HLocation aLoc (see HCategory)
 Int_t pos=0;
 pos=fIndexTable->getIndex(aLoc);

 if (pos==-1) return NULL;
 return ((HDataObject *)fData->At(pos));
}

Bool_t HMatrixCategory::query(TCollection *aCol,HFilter &aFilter) {
  //Puts all objects in the category which pass the HFilter aFilter in the
  //collection pointed by aCol.
  TIter next(fData);
  HDataObject *data;

  if (!fData) return kFALSE;
  
  while ((data=(HDataObject *)next())!=NULL) {
    if (aFilter.check(data)) aCol->Add(data);
  }
  return kTRUE;
}

Bool_t HMatrixCategory::query(TCollection *aCol,HLocation &aLoc) {
  //Puts all objects in the category corresponding to the HLocation aLoc in 
  //the collection pointed by aCol

  Int_t i=0,n=0,pos=0;
#if DEBIG_LEVEL>2
  gDebuger->enterFunc("HMatrixCategory::query");
  gDebuger->message("entries=%i",fData->GetEntries());
#endif

  if (!fData) return kFALSE;

  n=fIndexTable->gotoLocation(aLoc);
 
  for (i=0;i<n;i++) {
    if ((pos=fIndexTable->next())!=-1) {
      aCol->Add(fData->At(pos));
    }
  }
  return kTRUE;
#if DEBIG_LEVEL > 2
  gDebuger->leaveFunc("HMatrixCategory::query");
#endif
}

Bool_t HMatrixCategory::query(TCollection *aCol,
			      HLocation &aLoc,
			      HFilter &aFilter) {
  //Puts all the objects in the category passing the HFilter aFilter and 
  //corresponding to the HLocation aLoc into the collection pointed by aCol
  Int_t i=0,n=0,pos=0;
  HDataObject *data=NULL;
 
  if (!fData) return kFALSE;

  n=fIndexTable->gotoLocation(aLoc);

  for (i=0;i<n;i++) {
   if ((pos=fIndexTable->next())!=-1) { 
      data=(HDataObject *)fData->At(pos);
      if (aFilter.check(data)) aCol->Add(data);
   }
  }
  return kTRUE;
}

Bool_t HMatrixCategory::filter(HFilter &aFilter) {
  //see HCategory
  Int_t *changes=NULL,i=0,j=0,nEntries=0;
  Int_t dest=-1;
  HDataObject *obj=NULL;
  nEntries=fData->GetEntriesFast();
  changes = new Int_t[nEntries];
  if (changes==NULL) return kFALSE;
  //Pass filter
  for (i=0;i<nEntries;i++) {
    obj=(HDataObject *)fData->At(i);
    if (obj!=NULL)
      if (!aFilter.check(obj)) fData->RemoveAt(i);
  }
  //Elliminate gaps in fData
  for (i=0,dest=0;i<nEntries;i++) {
    if (fData->At(i)!=NULL) {
      changes[i]=dest;
      dest++;
    } else changes[i]=-1;
  }
  //Compress array
  fData->Compress();
  //Update index table
  nEntries=fIndexTable->gotoBegin();
  for (i=0;i<nEntries;i++) {
   if ((j=fIndexTable->next())!=-1) fIndexTable->setIndex(i,changes[j]);
  }

  return kTRUE;
  //Version 2
//   HIterator *p=NULL;
//   Int_t i=0,n=0;
//   HDataObject *obj=NULL;
//   p=(HIterator *)MakeIterator();
//   while ( (obj=(HDataObject *)p->Next())!=NULL) {
//     if (aFilter.check(obj)) {
//       if (!obj->HasIdentifier()) obj->setLocation(p->getLocation());
//     } else {
//       fData->RemoveAt(fIndexTable->getIndex(p->getLocation()));
//     }
//   }
//   fData->Compress();
//   fIndexTable->Clear();
//   n=fData->GetEntriesFast();
//   for (i=0;i<n;i++) {
//     fIndexTable->setIndex((*obj->getLocation()),i);
//   }
//   return kTRUE;
}

Bool_t HMatrixCategory::filter(HLocation &aLoc,HFilter &aFilter) {
  //see HCategory
  Int_t *changes=NULL,i=0,j=0,nEntries=0;
  Int_t nLocs=0;
  Int_t dest=-1;
  HDataObject *obj=NULL;
  nEntries=fData->GetEntriesFast();
  changes = new Int_t[nEntries];
  if (changes==NULL) return kFALSE;
  //Pass filter
  nLocs=fIndexTable->gotoLocation(aLoc);
  for (i=0;i<nLocs;i++) {
    j=fIndexTable->next();
    if (j!=-1) {
      obj=(HDataObject *)fData->At(j);
      if (obj!=NULL)
	if (!aFilter.check(obj)) fData->RemoveAt(j);
    }
  }
  //Elliminate gaps in fData
  for (i=0,dest=0;i<nEntries;i++) {
    if (fData->At(i)!=NULL) {
      changes[i]=dest;
      dest++;
    } else changes[i]=-1;
  }
  //Compress array
  fData->Compress();
  //Update index table
  nEntries=fIndexTable->gotoBegin();
  for (i=0;i<nEntries;i++) {
   if ((j=fIndexTable->next())!=-1) fIndexTable->setIndex(i,changes[j]);
  }

  return kTRUE;
}

TIterator *HMatrixCategory::MakeIterator(Bool_t dir) {
  //Returns an iterator, which iterates in the direction dir and (as default)
  //throuhg the whole category.
  HMatrixCatIter *iterator=NULL;
  iterator=new HMatrixCatIter(this,dir);
  return iterator;
}

void HMatrixCategory::Clear(void) {
  // see HCategory
 
 fNDataObjs=0;
 fData->Clear();
 fIndexTable->Clear();
}

void HMatrixCategory::Browse(TBrowser *b) {
  //Browse objects in this category
  if (fData) b->Add(fData,fData->GetClass()->GetName());
}

void HMatrixCategory::Streamer(TBuffer &R__b)
{
   // Stream an object of class HMatrixCategory.
   Char_t clase[200];
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(); if (R__v) { }
      HCategory::Streamer(R__b);
      R__b >> fIndexTable;
      R__b >> fNDataObjs;
      R__b.ReadString(clase,200);
      if ( fData && strcmp(clase,fData->GetClass()->GetName())==0)
       fData->Clear();
      else {
       delete fData;
       fData=new TClonesArray(clase);
      }
      fData->Streamer(R__b);
   } else {
       R__b.WriteVersion(HMatrixCategory::IsA());
       HCategory::Streamer(R__b);
       R__b << fIndexTable;
       R__b << fNDataObjs;
       strcpy(clase,fData->GetClass()->GetName());
       R__b.WriteString(clase);
       fData->Streamer(R__b);
   }
}

