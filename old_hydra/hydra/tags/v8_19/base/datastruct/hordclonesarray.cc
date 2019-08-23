#include "hordclonesarray.h"
#include "TROOT.h"
#include "TClass.h"
#include "assert.h"

//-- Author : Denis Bertini <dbertini@gsi.de>
//-- Modified : 19/09/00 by  D. Bertini
//
//*************************************************************************//
// This Class allows the TClonesArray to create sub-sets of HDataObjects
// when filling its structure. This is done creating an additionnal ptr-array
// like structure for a special bookeeping of TObject* pointers (bkeep* array)
// according the sub-sets defined in the HOrdCategory.
// Overloaded Methods :
//
//       TClonesArray::operator[](int);
//       TClonesArra::Clear();
//
//*************************************************************************//

ClassImp(HOrdClonesArray)

HOrdClonesArray::HOrdClonesArray() : TClonesArray(){
currentLevel=0;
}

HOrdClonesArray::HOrdClonesArray( Int_t nLevel, Int_t* sizes,
                                   const Char_t *classname, 
                                   Int_t          s, 
				   Bool_t b, Int_t flag): 
                                  TClonesArray(classname,s,b) 
{
currentLevel=0;
maxSize=1;
#if DEBUG_LEVEL>0 
 Warning("HOrdClonesArray()"," class name%s",classname); 
#endif
for (Int_t i=0;i<nLevel;i++) { maxSize*=sizes[i];}
if (flag==1) createPtrArray();
}

HOrdClonesArray::~HOrdClonesArray() {
  if(bKeep){ 
 for( Int_t i=0;i<maxSize;i++){
   if(bKeep[i]) bKeep[i]->Delete(); 
   delete bKeep[i]; bKeep[i]=0;
  }
 delete bKeep; bKeep=0;
 }
}


void HOrdClonesArray::createPtrArray(void){
  bKeep = new TObjArray* [maxSize]; 
  for(Int_t i=0; i<maxSize; i++) {
  bKeep[i] = new TObjArray(getSplittedSize()); 
  } 
 split_counter = new Int_t[maxSize];
}


TObject *&HOrdClonesArray::operator[](Int_t idx)
{
   TObject*& obj = TClonesArray::operator[](idx+1);
#if DEBUG_LEVEL>0 
     printf("nb of object in Clones==> \
            ndataobj:%i idx:%i getlast:%i \n", 
            fNDataObjects, idx,GetLast()+1); 
#endif   
   update(currentLevel);
   return obj;

}

void HOrdClonesArray::Clear(){
 TObjArray::Clear();
 currentLevel=0;
 // remove ptr from substructure
 for(Int_t i=0; i<maxSize; i++) {
#if DEBUG_LEVEL>0 
  Warning("Clear","sub array contents ===> :%i \n",
          bKeep[i]->GetEntriesFast());
#endif
      bKeep[i]->Clear(); 
   }
}


