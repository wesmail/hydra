#include "hordcategory.h"
#include "hlocateddataobject.h"
#include "heventheader.h"
#include "hordcatiter.h"
#include "hades.h"
#include "htree.h"
#include "hfilter.h"
#include "hdebug.h"
#include <TClass.h>
#include <TTree.h>

//*-- Author : Denis Bertini <D.Bertini@gsi.de>
//*-- Modified : 20/09/00 by D. Bertini
//*-- Modified : 20/07/01 by Dan Magestro (HDataObject --> TObject)


//-------------------------------------------------------------------------//
//HOrdCategory. 
//
// Category  creating sets of TObjects in a linear
// TClonesArray.
// To create such a category the caller needs to define
// the sets.  
// For example, let's suppose that we want to create sets of TObjects
// identified using two indexes ( sector, module)
//    
//    
//   >   Int_t nlevels = 2;         // doublets (sector,modules)
//   >   Int_t sizes[2]={6, 4};     // maximum sizes 
//   >   Int_t nobjects = 6*4*6*20  //estimation of multiplicity/event 
//   >   
//   >    HOrdCategory *ord = 
//   >         new HOrdCategory ("Hit", nobjects, nlevel, sizes);
// 
//
// A) Filling the HOrdCategory 
//
// The filling procedure follows the general HCategory interface i.e:
//  
// 1) Create a Location for every nodes           
//
//   > HLocation loc;
//   > loc.set(2,0,0);
//
// 2) In the event loop:
//     
//   >   for (Int_t i = 0; i < nevents; i++)
//        .... 
//   >    loc[0] = sector; 
//   >    loc[1] = module;  
//   > //-- fill the structures
//         Hit *aHit= NULL;
//   > //-- get a slot and if the slot is not allocated 
//   > //-- allocate the slot 
//   >    aHit = (Hit*) ord->getSlot(loc);
//   >     if (aHit) {
//   >     aHit = new (aHit) Hit();   
//   >  //-- fill the hit
//   >     aHit->setSector(loc[0]);
//   >     aHit->setModule(loc[1]); 
//   >     aHit->setX(xx,1.0F); 
//   >     aHit->setY(yy,1.0F); 
//   >     aHit->setOff(0.,1.0F);
//   >      }else{
//   >     Error("order1","can not get Slot\n"); 
//   > }
//
// At the end of the filling procedure, the linear structure will
// show sets of "Hit" objects identified by the 2 indices (sector,module)
// The nodes, defined as the begin point of the sets in the linear array
// are stored consecutively in lexicographic order of the indices.
//  
// Array slots:       A[0],........, A[j],.........  ,A[k],.... 
//
// Data Objects:    Hit[s1,m11],...,Hit[s1,m12],..., Hit[s2,m21], ...
// 
// where always     s1<s2 and   m11<m12
//
// Inside a set of TObjects, no lexicographic order is assumed !
//
//
// B) Accessing TObjects in HOrdCategory
//  
// It's now possible reading back the HOrdCategory 
// from a root file to have direct access to a specific set. 
// In our example, we can access a special set giving the
// (sector, module) as argument of the class method 
//                    HOrdCategory::MakeIterator(HLocation *loc) 
// 
//
//
//  HLocation loc0; 
//  loc0.set(2,0,0); 
//  loc0[0]=5; 
//  loc0[1]=0;    
//
//  HLocation loc1;
//  loc1.set(2,0,0); 
//  loc1[0]=5; 
//  loc1[1]=2;
//  
//  //Create a specific Iterator for this Category
//  HOrdCatIter* iter0= NULL; 
//  iter0 =(HOrdCatIter*) ordcat->MakeIterator(&loc0);  
//  iter2 =(HOrdCatIter*) ordcat->MakeIterator(&loc2); 
// 
//  for (int i = 0; i < nevents; i++)
//    {
//      tree->GetEntry (i);
//       iter2->Reset();
//        while ( (aHit = (Hit*) iter2->Next()) != 0 ){
//           // select (sec=5,mod=2) TObjects
//            ... so something 
//           iter0->Reset();  
//           while (  (aHit = (Hit*) iter0->Next()) != 0 ){
//           // select (sec=5,mod=0) TObjects
//            ... do something  
//         } 
//   }
// }
//-------------------------------------------------------------------------//


Int_t  HOrdCategory::init_count=0;

HOrdCategory::HOrdCategory(void) {
  ++init_count;   
  currentOffset=-1;
  currentLenght=-1; 
  currentlinearAddr=-1;
  setup = NULL;
  maxSize=0; 
  fData=NULL;
  isNotset=kTRUE; 
 }

HOrdCategory::HOrdCategory(Text_t *className,Int_t nentries,
				 Int_t nDim,
				 Int_t *sizes) {
  ++init_count; 
  fData=NULL;
  fNDataObjs=-1;
  currentOffset=-1;
  currentLenght=-1; 
  currentlinearAddr=-1;
  isNotset=kTRUE; 
  setCapacity(className,nentries,nDim,sizes,1); 
}

void HOrdCategory::setCapacity(Text_t *className, 
                          Int_t size,
			  Int_t nDim,
			  Int_t *sizes,Int_t flag){
 if(!setup) setup = new PatternSetup();
 setup->nLevels = nDim;
 (setup->maxSizes).Set(setup->nLevels); 
 for(Int_t i=0;i<setup->nLevels;i++) 
    {
     (setup->maxSizes).fArray[i] = sizes[i];
 }

 fData=new HOrdClonesArray(setup->nLevels,sizes,className,size,0,flag);
 maxSize=fData->getMaxSize();  
 (setup->offsets).Set(maxSize);
 (setup->lenghts).Set(maxSize);
 fData->setCategory(this);
}

HOrdCategory::~HOrdCategory(void) {
 --init_count;
  if (init_count < 0 ) {
  if(setup)  delete setup;  
  if (fData) fData->Delete(); delete fData; fData=0;
  }
}

const Text_t *HOrdCategory::getClassName(void) {
  return fData->GetClass()->GetName();
}

void HOrdCategory::makeBranch(TBranch *parent) {
}

void HOrdCategory::activateBranch(TTree *tree,Int_t splitLevel) {
}

TObject *&HOrdCategory::getNewSlot(HLocation& aLoc,Int_t* pIndex) {
// return next sequential slot
  setCurrLinearAddr(aLoc.getLinearIndex(&(setup->maxSizes)));
  fData->setLevel(currentlinearAddr); 
  TObject *&r=fData->operator[](fNDataObjs++);
  return r;
}

TObject *&HOrdCategory::getSlot(HLocation &aLoc,Int_t* pIndex) {
  setCurrLinearAddr(aLoc.getLinearIndex(&(setup->maxSizes)));
  fData->setLevel(currentlinearAddr); 
  TObject *&r=fData->operator[](fNDataObjs++);
  return r;
}

TObject *&HOrdCategory::getSlot(Int_t index) {  
// needs Compress()
// bound check + expand if necessary
 fData->setLevel(index);
 TObject *&r= fData->operator[](index);       
 return r;
}

TObject *HOrdCategory::getObject(HLocation &aLoc) {
// no bound check and use only aLoc[0]
return (TObject *)fData->UncheckedAt(aLoc[0]);
}

TObject *HOrdCategory::getObject(Int_t index) {
// no bound check
return (TObject *)fData->UncheckedAt(index);
} 


TObject *HOrdCategory::getObject(HLocation &aLoc, Int_t index) {
// no bound check and use only aLoc[0]

if(setup){
 setCurrLinearAddr(aLoc.getLinearIndex(&(setup->maxSizes)));
 TObjArray* obj = fData->getSubArray(currentlinearAddr); 
   if(obj){
       return (TObject *) obj->UncheckedAt(index);
   }

  if ( index >  getCurrentLinearLenght() ) return NULL; 
return (TObject *)fData->UncheckedAt(getCurrentLinearOffset()+index);
}
return NULL; 
}

Int_t HOrdCategory::getNDataObjects(HLocation &aLoc){

if(setup){
 setCurrLinearAddr(aLoc.getLinearIndex(&(setup->maxSizes)));
 TObjArray* obj = fData->getSubArray(currentlinearAddr); 
   if(obj){
       return (Int_t) obj->GetEntriesFast();
   }
 
return  getCurrentLinearLenght(); 
}
return -1;
}  


Bool_t HOrdCategory::filter(HFilter &aFilter) {
  Int_t i;
  Bool_t r=kTRUE;
  for (i=0;i<=fData->GetLast();i++) {
    if (!aFilter.check((TObject *)fData->UncheckedAt(i)))
      fData->RemoveAt(i);
  }
  fData->Compress();
  return r;
}

Bool_t HOrdCategory::filter(HLocation &aLoc,HFilter &aFilter) {
  Int_t i;
  Bool_t r=kTRUE;
  if (aLoc.getNIndex()==0) {
    for (i=0;i<=fData->GetLast();i++) {
      if (!aFilter.check((TObject *)fData->UncheckedAt(i)))
	fData->RemoveAt(i);
    }
    fData->Compress();
  } else r=kFALSE;    
  return r;
}

void HOrdCategory::Clear(void) {
   currentOffset=currentLenght=currentlinearAddr=-1; 
   setup->reset();  
   fNDataObjs=-1;
   fData->Clear();
}


inline Bool_t HOrdCategory::isSelfSplitable(void) {
 return kFALSE;
}

TIterator *HOrdCategory::MakeIterator(Option_t *opt,Bool_t dir) {
// iterator aver all linear structure 
  TString option = opt;
  option.ToLower(); 
  if (option.Contains("native")) return fData->MakeIterator(dir);
 return  fData->MakeIterator(dir);
}


TIterator *HOrdCategory::MakeIterator(HLocation *loc,
                                      Option_t *opt,Bool_t dir) {
  
// iterator over a subset defined by location parameter
 if(!loc) return MakeIterator(opt,dir);
  
 if(setup){
 TObjArray* obj = fData->getSubArray(loc->getLinearIndex(&(setup->maxSizes))); 
   if(obj){

     #if DEBUG_LEVEL>2
      Warning(" i create a TObjArray iter:  %i  \n",
                obj->GetEntries());
     #endif
 
     return   obj->MakeIterator(dir);
   }
 }

   HOrdCatIter *iterator=NULL;
    #if DEBUG_LEVEL>2
     Warning("MakeIterator",
             "linear address :%i \n", currentlinearAddr );
    #endif

   iterator=new HOrdCatIter(loc,this,dir);
   return iterator;
}

void HOrdCategory::Streamer(TBuffer &R__b)
{
  
  // Stream an object of class HOrdCategory.
   Char_t clase[200];
    if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(); if (R__v) { }
      HCategory::Streamer(R__b);
      if(setup) setup->reset();
      R__b >> setup;
      R__b >> fNDataObjs;
      R__b.ReadString(clase,200);
          
      if ( fData && strcmp(clase,fData->GetClass()->GetName())==0)
         fData->Clear();
      else {
         setCapacity(clase,fNDataObjs,setup->nLevels,
                      (setup->maxSizes).fArray,0); 
      }
     
      fData->Streamer(R__b);

   } else {
      R__b.WriteVersion(HOrdCategory::IsA());
      HCategory::Streamer(R__b);
      fData->createPattern(); 
      R__b << setup;   
      R__b << fNDataObjs;
      strcpy(clase,fData->GetClass()->GetName());
      R__b.WriteString(clase);
      fData->Streamer(R__b);
   }
}


ClassImp(PatternSetup)
ClassImp(HOrdCategory)
