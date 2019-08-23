#include "hbitfieldcategory.h"
#include "heventheader.h"
#include "hdataobject.h"
#include "hades.h"
#include "htree.h"
#include "hfilter.h"
#include "hdebug.h"
#include <TClass.h>
#include <TBranch.h>
#include <TTree.h>
#include <TStorage.h>

//*************************************************************************//
//*-- Author : Denis Bertini <D.Bertini@gsi.de>
//*-- Modified : 20/09/00 by D. Bertini






//*************************************************************************//
//
//
// HBitFieldCategory. 
// 
// This category stores HLocatedDataObjects in a linear structure 
// (using TClonesArray) and provides:
//
//                             - full random access ( bit pattern based )
//                             - consecutive storage of HLocatedDataObjects in 
//                               lexicographic order of the indexes   
//
// The TClonesArray is expanded to the maximum size defined 
// by the particular setup that the caller provides.
// This allows the HLocatedDataObjects to appear in memory in  
// "lexicographic order" of its indexes ("row major order").
// When filling the TClonesArray, the position of the HLocatedDataObjects 
// is computed so that:
//
//                   If 
//                   - A is a HLocatedDataObject 
//                   - A is identified by 4 indexes (I,J,K,L)
//                     
//             =>      Location(A[I,J,K,L]) = a0 + a1*I + a2*J + a3*k + a4*L
//
//              where a0, a1, a2, a3, a4 are constants. 
//
//
// Before filling the HTree structure, a dedicated Compress() method is
// called in order to keep track of the position for the"relevant" sub-nodes 
// in the linear array (i.e HClonesArray::Compress()).This "zero suppression"
// procedure preserve the lexicographic order of the HLocatedDataObjects
// indexes.
// For Example, in the preceding case where we 
// defined a HLocatedDataObject A[I,J,K,L] idetified by 4 indexes, 
// the "sub-nodes"  will be the set:  
//                     
//                   A[I,J,K,Lmin]  where  VL  Lmin <= L  
//    
// Within one sub-node defined by A[Io,Jo,Ko,Lmin] and assuming that 
// the HLocatedDataObjects are stored consecutively in lexicographic
// order of the indices, a Bit-Array lookup (HBArray) can then be used to 
// locate the remaining  HLocatedDataObjects.   
//
// The interface to access/write objects to the Category follows
// general interface (see comment on HCategory Class); 
// Only Standard ROOT iterator is provided (for the moment)
// by the function:
//
//  TIterator* HBitFieldCategory::MakeIterator();
//      
//-------------------------------------------------------------------------//




HBitFieldCategory::HBitFieldCategory(void) {
 fData=NULL;
 fNDataObjs=0;
 testbit=kFALSE;  
}

HBitFieldCategory::HBitFieldCategory(Text_t *className,
				     Int_t nDim,
				     Int_t *sizes,
                                     Float_t fillrate=0.1) {

  fData=NULL;
  fNDataObjs=0;
  fBitIndex = new HBArray();
  testbit=kTRUE;
  setCapacity(className,nDim,sizes); 
}

void HBitFieldCategory::setCapacity(Text_t *className, 
			  Int_t nDim,
			  Int_t *sizes,Float_t fillrate=0.1){

 fBitIndex->setDimensions(nDim,sizes); 
 if (fData) delete fData;
 fData=new HClonesArray(className,
                        (Int_t)(fBitIndex->getEntries())); 
 fData->Clear(); 
 fData->setBitArray(fBitIndex); 
 }

HBitFieldCategory::~HBitFieldCategory(void) {
  if (fData) {fData->Clear(); delete fData;}
  if (fBitIndex) delete fBitIndex;
}

const Text_t *HBitFieldCategory::getClassName(void) {
  return fData->GetClass()->GetName();
}


void HBitFieldCategory::makeBranch(HTree *tree) {
 //If this category is selfsplitable 
 // this function will be called (needs HSelfSplitable class)

 ((TTree*)tree)->Branch ("ClonesBranch", &fData, 64000,1);
 ((TTree*)tree)->Branch ("BArrayBranch","HBArray", &fBitIndex, 32000,0);   
}


void HBitFieldCategory::makeBranch(TBranch *parent) {
  if(gHades){
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
}


void HBitFieldCategory::activateBranch(TTree *tree,Int_t splitLevel) {
  Text_t name[255];
  if (splitLevel) {
    TIterator *subBranches = NULL;
    TBranch* b = 0;
    sprintf(name,"%s.fData",getClassName());
    
    tree->SetBranchAddress(name,&fData);
    subBranches = tree->GetBranch(name)->GetListOfBranches()->MakeIterator();
    while ( (b = (TBranch*)subBranches->Next()) != NULL) {
      tree->SetBranchStatus(b->GetName(),1);
    }
  }
  if (fHeader) {
    sprintf(name,"%s.Header*",getClassName());
    tree->SetBranchStatus(name,1);
    tree->SetBranchAddress(name,&fHeader);
  }
}

TObject *&HBitFieldCategory::getNewSlot(HLocation& aLoc,Int_t* pIndex) {
// return next sequential slot
  TObject *&r=fData->operator[](fNDataObjs++);
  return r;
}

TObject *&HBitFieldCategory::getSlot(HLocation &aLoc,Int_t* pIndex) {

 Int_t linear=0,pos=0;
 TObject *obj; 
 HClonesArray &data=*fData;

  if( testbit ) {
    #if DEBUG_LEVEL>2  
     Warning("getSlot","not streamed i get linear pos \n");
    #endif  
   linear =  aLoc.getLinearIndex(fBitIndex->getDimensions(),
                                 fBitIndex->getNDim());
   obj = fData->UncheckedAt(linear);
   
    #if DEBUG_LEVEL>2
    Warning("getSlot","not streamed i get linear pos: %i  Heap? :%i \n", linear,
                         TStorage::IsOnHeap(obj) );
    #endif  
       
       if(!(TStorage::IsOnHeap(obj))){ 
	 
	    fNDataObjs++;
	     fBitIndex->set(aLoc);
       }  
      return data[linear]; 
       
    }else{
      #if DEBUG_LEVEL>2  
          Warning("getSlot","streamed i use bit-position \n");
      #endif
  
	pos=fBitIndex->getPosition(aLoc);  
        if(pos==-1) {  
       #if DEBUG_LEVEL>2   
	  Warning("getSlot","no object found for location: \n");
	  	       aLoc.Dump();
       #endif    
                       return gNullObject;
	}
	return data[pos]; 
   }  
   return gNullObject; 
}


TObject *&HBitFieldCategory::getSlot(Int_t index) {  
// needs Compress()
 TObject *&r= fData->operator[](index);       
 return r;
}

HDataObject *HBitFieldCategory::getObject(HLocation &aLoc) {
Int_t pos=0,linear=0;

 if( testbit ){ 
   
   linear =  aLoc.getLinearIndex(fBitIndex->getDimensions(),
                                 fBitIndex->getNDim() );
     #if DEBUG_LEVEL>2
      Warning("getObject","not streamed : linear addressing used=>%i \n",linear);
      aLoc.Dump(); 
      obj = fData->UncheckedAt(linear) ;
      Warning("getObject","test object :%i for location \n",fData->UncheckedAt(linear)); 
      aLoc.Dump();
    #endif     

   return ( (HDataObject *)  fData->UncheckedAt(linear) );  
  }else{
   
    pos=fBitIndex->getPosition(aLoc);
    #if DEBUG_LEVEL>2  
     Warning("getObject","streamed : bit-position used \n",pos);
    #endif 
    if (pos==-1) {
    #if DEBUG_LEVEL>2
     Warning("getObject","no object found for location: \n");
     aLoc.Dump();
    #endif
                   return NULL;
    }
    return ((HDataObject *)fData->UncheckedAt(pos));
  }

 return NULL;
}

HDataObject *HBitFieldCategory::getObject(Int_t index) {
// no bound check
return (HDataObject *)fData->UncheckedAt(index); 
} 

Bool_t HBitFieldCategory::filter(HFilter &aFilter) {
  Int_t i;
  Bool_t r=kTRUE;
  for (i=0;i<fData->GetLast();i++) {
    if (!aFilter.check((HDataObject *)fData->UncheckedAt(i))) 
      fData->RemoveAt(i);
  }
  fData->Compress();
  return r;
}

Bool_t HBitFieldCategory::filter(HLocation &aLoc,HFilter &aFilter) {
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

void HBitFieldCategory::Clear(void) {
  #if DEBUG_LEVEL>2
  Warning("Clear","called"); 
  #endif
  
  fNDataObjs=-1;
  fBitIndex->clear();
  fData->Clear();
}


inline Bool_t HBitFieldCategory::isSelfSplitable(void) {
 return kFALSE;
}

TIterator *HBitFieldCategory::MakeIterator(Option_t *opt,Bool_t dir) {
// Create global iterator on this linear structure 
  TString option = opt;
  option.ToLower(); 
  if (option.Contains("native")) return fData->MakeIterator(dir);
 return  fData->MakeIterator(dir);
}


void HBitFieldCategory::Streamer(TBuffer &R__b)
{
  
  // Stream an object of class HBitCategory.
    Char_t clase[200];
     if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(); if (R__v) { }
      HCategory::Streamer(R__b); 
      fBitIndex->Streamer(R__b);
      R__b.ReadString(clase,200);

      if ( fData && strcmp(clase,fData->GetClass()->GetName())==0)
         fData->Clear();
      else {
         setCapacity(clase,fBitIndex->getNDim(),
                      fBitIndex->getDimensions()); 
      }
      R__b >> fNDataObjs;     

   } else {
      R__b.WriteVersion(HBitFieldCategory::IsA()); 
      HCategory::Streamer(R__b);
      R__b << fBitIndex;
      fData->Streamer(R__b);
      strcpy(clase,fData->GetClass()->GetName()); 
      R__b.WriteString(clase);
      R__b << fNDataObjs;


   }
}

ClassImp(HBitFieldCategory)
