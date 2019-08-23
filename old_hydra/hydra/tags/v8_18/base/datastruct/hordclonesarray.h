#ifndef __HORDCLONESARRAY__
#define __HORDCLONESARRAY__

#include "hcompositecategory.h"
#include <TClonesArray.h>
#include <TObjArray.h>
#include <TMath.h>

class HOrdCatIter;

class HOrdClonesArray : public TClonesArray {
friend class HOrdCatIter;
protected:
  HCompositeCategory *cat;       //! associated category 
  TObjArray **bKeep;             //! additional sub-structure ptr-array like. 
  Int_t *split_counter;            //! internal split counter  
  Int_t maxSize;                   //! max size of subset Ptr array
  Int_t currentLevel;            //! current subPattern

public:
   HOrdClonesArray();
   HOrdClonesArray(Int_t nLevel,Int_t* size,
                const Char_t * classname, Int_t s=1000, Bool_t b=kFALSE, 
                Int_t flag=1); 
   ~HOrdClonesArray();
   void createPtrArray(void);
   void setCategory(HCompositeCategory* aCat){ cat=aCat;}
   HCompositeCategory* getCategory(){ return cat;}
   void setLevel(Int_t j){currentLevel=j;}   
   Int_t getMaxSize() { return maxSize;}   
   inline Int_t getMaxSize(Int_t nLevels,Int_t* size);
   inline TObjArray* getSubArray(Int_t index);
   Int_t getSplittedSize() { return ((int) GetSize()/maxSize +1 );}
   inline void  update( Int_t i );
   inline void createPattern();
   TObject *& operator[](Int_t idx); 
   void Clear();
ClassDef( HOrdClonesArray, 1 )
};




//
//-------------- inline functions 
// 

inline TObjArray* HOrdClonesArray::getSubArray(Int_t index) {
  if (bKeep){
  return *(bKeep+index); 
  }else{
  return NULL; 
  }
}


inline Int_t HOrdClonesArray::getMaxSize(Int_t nLevel, Int_t* size){
 maxSize=1; 
 for (Int_t i=0;i<nLevel;i++) maxSize*=size[i];
 return maxSize; 
}

inline void HOrdClonesArray::createPattern(){
 Int_t incr=0;
 Int_t * offsets = cat->getOffsets(); 
 Int_t * lenghts = cat->getLenghts();  

 TObject ** keep = &( (*fKeep)[0]); 
 for (Int_t i = 0; i < maxSize; i++) {
  TObjArray *b = *(bKeep +i );    
  offsets[i] = incr;   
  lenghts[i] =  b->GetEntriesFast();

   for(Int_t j = 0; j < lenghts[i] ; j++){
        fCont[incr] = ((TObject*) b->UncheckedAt(j));
        *(keep+incr)= ((TObject*) b->UncheckedAt(j));

#if DEBUG_LEVEL>0   
     Warning(" createPattern","objnr:%i ptr:%i  \
              fCont:%i  keep:%i \n",
              j,b->UncheckedAt(j),fCont[incr],keep[incr]);
#endif

             incr++;
    } 
 }

#if DEBUG_LEVEL>0   
    for (Int_t i=0;i<maxSize;i++) 
                                Warning("createPattern","  \
                                    index : %i Off: %i Len:%i \n",i,
                                    offsets[i], 
                                    lenghts[i]); 

#endif
}

inline void HOrdClonesArray::update( Int_t i ) {
  if (bKeep == 0) {
         Error("update()","can not use [] operator \
                           while reading \n use instead \
                           category->getObject(location)"); 
  }

 TObjArray* b = *(bKeep+i); 
 b->TObjArray::Add(fCont[GetLast()]);

#if DEBUG_LEVEL>0
      Warning("update"," level:%i  count:%i  \
                         fCont:%i Lastptr:%i \n", 
                         i,b->GetEntriesFast(),fCont[GetLast()+1], 
                         b->Last());
#endif
}


#endif 
 




