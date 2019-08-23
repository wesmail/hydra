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
  int *split_counter;            //! internal split counter  
  int maxSize;                   //! max size of subset Ptr array
  Int_t currentLevel;            //! current subPattern

public:
   HOrdClonesArray();
   HOrdClonesArray(Int_t nLevel,Int_t* size,
                const char * classname, Int_t s=1000, Bool_t b=kFALSE, 
                int flag=1); 
   ~HOrdClonesArray();
   void createPtrArray(void);
   void setCategory(HCompositeCategory* aCat){ cat=aCat;}
   HCompositeCategory* getCategory(){ return cat;}
   void setLevel(int j){currentLevel=j;}   
   int getMaxSize() { return maxSize;}   
   inline int getMaxSize(int nLevels,int* size);
   inline TObjArray* getSubArray(int index);
   int getSplittedSize() { return ((int) GetSize()/maxSize +1 );}
   inline void  update( Int_t i );
   inline void createPattern();
   TObject *& operator[](Int_t idx); 
   void Clear();
ClassDef( HOrdClonesArray, 1 )
};




//
//-------------- inline functions 
// 

inline TObjArray* HOrdClonesArray::getSubArray(int index) {
  if (bKeep){
  return *(bKeep+index); 
  }else{
  return NULL; 
  }
}


inline int HOrdClonesArray::getMaxSize(int nLevel, int* size){
 maxSize=1; 
 for (Int_t i=0;i<nLevel;i++) maxSize*=size[i];
 return maxSize; 
}

inline void HOrdClonesArray::createPattern(){
 Int_t incr=0;
 int * offsets = cat->getOffsets(); 
 int * lenghts = cat->getLenghts();  

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
    for (int i=0;i<maxSize;i++) 
                                Warning("createPattern","  \
                                    index : %i Off: %i Len:%i \n",i,
                                    offsets[i], 
                                    lenghts[i]); 

#endif
}

inline void HOrdClonesArray::update( int i ) {
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
 




