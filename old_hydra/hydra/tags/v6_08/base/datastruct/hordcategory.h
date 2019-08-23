#ifndef HORDCATEGORY_H
#define HORDCATEGORY_H

#include <TArrayI.h>
#include <TObjString.h>
#include "hlocation.h"
#include "hordclonesarray.h"
#include "hcompositecategory.h"
class HOrdCatIter;

class PatternSetup : public TObject{
 public:
 Int_t nLevels; 
 TArrayI maxSizes; 
 TArrayI offsets; 
 TArrayI lenghts; 

 PatternSetup(){;}
 ~PatternSetup(){;}
  
 TArrayI* getSize() {return &maxSizes;}
 TArrayI* getOffset() {return &offsets;} 
 TArrayI* getLenghts() {return &lenghts;} 
 int* getArrayOffs(){ return offsets.fArray; }
 int* getArrayLen() { return lenghts.fArray; }
 void reset() {
   for (Int_t i=0;i<offsets.fN;i++){ 
                             offsets.fArray[i] = -1;
                             lenghts.fArray[i] = -1;  
               }   
 }

 void print() {
   for (Int_t i=0;i<offsets.fN;i++){ 
                             printf(" %i %i \n",offsets.fArray[i],
                                                lenghts.fArray[i]);  
               }   
 }

ClassDef( PatternSetup, 1)
};



class HOrdCategory : public HCompositeCategory {
friend class HOrdCatIter;
protected:


 HOrdClonesArray *fData; // Data array of the category
 Int_t fNDataObjs;       // Multiplicity /evts
 PatternSetup* setup;    // pattern configuration 



 HLocation location;     //! location object 
 Int_t currentOffset;    //! current offset of subpattern in structure
 Int_t currentLenght;    //! current lenght of subpattern in structure
 Bool_t isComposite;     //! current status of HOrdCategory
 Int_t currentlinearAddr;//! current linearAddress identifying the subPattern 
 static Int_t init_count;//! ref counter
 Int_t  maxSize;         //! max sizes of indexes
 Bool_t isNotset;        //! setup flag 

public:
  HOrdCategory(void);
  HOrdCategory(Text_t *className,Int_t nentries, Int_t nDim, Int_t *sizes);
  ~HOrdCategory(void);
  
  const Text_t *getClassName(void);
  void setCapacity(Text_t *className, Int_t size,
			  Int_t nDim,
			  Int_t *sizes, Int_t flag=1);

  TClass *getClass(void) {return fData->GetClass();}
  void makeBranch(TBranch *parent);
  void activateBranch(TTree *tree,Int_t splitLevel);
  Int_t getEntries() {return fData->GetEntriesFast();}
  Int_t getEntries(HLocation &loc);
  TObject *&getNewSlot(HLocation &aLoc,Int_t* pIndex=0);
  TObject *&getSlot(HLocation &aLoc,Int_t* pIndex=0);
  TObject *&getSlot(Int_t index);
  TObject *getObject(HLocation &aLoc);
  TObject *getObject(HLocation &aLoc,Int_t index);
  TObject *getObject(Int_t index);
  Int_t getNDataObjects(HLocation &aLoc);
  Bool_t filter(HFilter &aFilter);
  Bool_t filter(HLocation &aLoc,HFilter &aFilter);
  void Clear(void);
  void Compress() {fData->Compress();}   
  Bool_t isSelfSplitable(void);
  TIterator *MakeIterator(Option_t* opt="catIter",Bool_t dir=kIterForward);
  Int_t getIndex(HLocation &aLoc) {return aLoc[0];}
  const TCollection* getCollection(){return fData;} 
  Int_t getCurrentOffset(){ return currentOffset;}
  Int_t getCurrentLenght() {return currentLenght;}

  //////////////////////
 
  Int_t init();  
  Int_t init(HLocation *loc);
  Int_t* getOffsets(){ return (setup->offsets).fArray;}   
  Int_t* getLenghts() { return (setup->lenghts).fArray;}
  Int_t getNLevels(){  return setup->nLevels;}
  TArrayI getMaxSizes(){ return setup->maxSizes;}
  Int_t  getMaxSize(){ return maxSize;}  
  void setLenghts(int loc, int val){ setup->lenghts[loc] = val; }
  void setOffsets(int loc, int val){ setup->offsets[loc] = val; }
  Bool_t isaComposite(){ return isComposite;} 
  void reSet(Int_t* arr);  
  void  setPattern(Int_t* off,Int_t* len );
  void setCurrLinearAddr(int j){ currentlinearAddr=j;} 
  Int_t getCurrentLinearAddr() { return  currentlinearAddr;}
  Int_t getCurrentLinearOffset() { return  (setup->offsets[currentlinearAddr]);}
  Int_t getCurrentLinearLenght() { return  (setup->lenghts[currentlinearAddr]);}
  Bool_t noSetup(){ return isNotset; }
  void copy(HLocation &loc1,HLocation &loc2);
  HLocation* getLocation(){ return &location;} 
  TIterator *MakeIterator(HLocation *loc ,
                          Option_t *opt="catIter",
                          Bool_t dir=kIterForward);

 ClassDef(HOrdCategory,1) //Generic data category
};



//
//**   Inlines functions  
//

inline Int_t HOrdCategory::getEntries(HLocation &loc){
    return (setup->lenghts[loc.getLinearIndex(&(setup->maxSizes))]);
}

inline Int_t HOrdCategory::init(){
  // Initialize the linear addresses & offsets & lenghts
  // for a sub part of this category. 
 
 if (maxSize == 0) 
     {
      maxSize=fData->getMaxSize(setup->nLevels,
                                (setup->maxSizes).fArray);
#if DEBUG_LEVEL>0
            printf("init called  maxSize:%i , 
                    nLevels:%i \n",maxSize,setup->nLevels);
#endif    
   }
  setCurrLinearAddr(location.getLinearIndex( &(setup->maxSizes) ) );
 if ((currentlinearAddr>maxSize) || (location.getNIndex()>setup->nLevels))  
   return -1; 
  currentOffset=setup->offsets[currentlinearAddr];
  currentLenght=setup->lenghts[currentlinearAddr];

#if DEBUG_LEVEL>0
 Warning("init",
         " linear addresse: %i maxSize:%i \n 
           Off: %i Len:%i \n",
              currentlinearAddr,maxSize,currentOffset,currentLenght);
#endif
 isNotset=kFALSE; 
 return 0;
}

inline Int_t HOrdCategory::init(HLocation *location){
  // Initialize the linear addresses & offsets & lenghts
  // for a sub part of this category. 

  if (maxSize == 0) {
      maxSize=fData->getMaxSize(setup->nLevels,
                               (setup->maxSizes).fArray);
#if DEBUG_LEVEL>0
     printf("init called  maxSize:%i ,
             nLevels:%i \n",maxSize,setup->nLevels);
#endif
 


  }
 setCurrLinearAddr(location->getLinearIndex( &(setup->maxSizes)));
 if ((currentlinearAddr>maxSize)|| (location->getNIndex()>setup->nLevels)) 
    return -1; 
    currentOffset=setup->offsets[currentlinearAddr];
    currentLenght=setup->lenghts[currentlinearAddr];

#if DEBUG_LEVEL>0
Warning("init with location ",
         " linear addresse: %i maxSize:%i \n 
         Off: %i Len:%i \n",
         currentlinearAddr,maxSize,currentOffset,currentLenght);
#endif

isNotset=kFALSE; 
return 0;
}



inline void HOrdCategory::copy(HLocation &loc1,HLocation &loc2){
  //copy of too location since 
  // operator = had some problems 
   TArrayI* un=loc1.getIndexes(); 
   TArrayI* deux=loc2.getIndexes();
   un->fN=0;
   un->Set(loc2.getNIndex(),deux->fArray); 
}

inline void HOrdCategory::reSet(Int_t* arr){
 for(Int_t i=0;i<maxSize;i++) arr[i]=-1; 
}

inline void HOrdCategory::setPattern(int* off, int* len){
  for (int i=0;i<maxSize;i++) {
         setup->offsets[i] = off[i]; 
         setup->lenghts[i] = len[i]; 
  }

#if DEBUG_LEVEL>0
for (int i=0;i<maxSize;i++) 
                                    Warning("setPattern"," 
                                    index : %i Off: %i Len:%i \n",i,
                                    setup->offsets[i], 
                                    setup->lenghts[i]);   
#endif

}
 
#endif /* !HORDCATEGORY_H */
