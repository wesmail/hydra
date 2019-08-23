#include "hordcatiter.h"
#include "hordcategory.h"

#include <TClonesArray.h>
#include <TIterator.h>




//-------------------------------------------------------------------------//
//HOrdCatIter
//
// This is an iterator for a HOrdCategory.
//-------------------------------------------------------------------------//


HOrdCatIter::HOrdCatIter(void) {
  //Private constructor
}

HOrdCatIter::HOrdCatIter(HOrdCategory *cat,Bool_t dir) {
  //Constructor. "cat" is the category being iterated and "dir" is the
  //iteration direction. 
  ordCat=cat; 
  fArray=cat->fData;
  fDir=dir;
  fCurrentLoc = cat->getLocation(); 
  fCurrentOff = cat->getCurrentOffset(); 
  fCurrentLen = cat->getCurrentLenght();
 
  if( fCurrentOff == -1 && fCurrentLen == -1){
              fCurrentOff=0; 
              fCurrentLen=fArray->Capacity(); 
  }
  if(fDir == kIterForward) { 
   cursor = fCurrentOff;  
   }else{ 
   cursor = fCurrentOff+fCurrentLen-1; 
  }
 
}

HOrdCatIter::HOrdCatIter(HLocation *loc,HOrdCategory *cat,Bool_t dir) {
  //Constructor. "cat" is the category being iterated and "dir" is the
  //iteration direction. 
  ordCat=cat; 
  fArray=cat->fData;
  fDir=dir;
  fCurrentLoc = loc; 
  fCurrentOff = cat->getCurrentOffset(); 
  fCurrentLen = cat->getCurrentLenght();
 
  if( fCurrentOff == -1 && fCurrentLen == -1){
              fCurrentOff=0; 
              fCurrentLen=fArray->Capacity(); 
  }
  if(fDir == kIterForward) { 
   cursor = fCurrentOff;  
   }else{ 
   cursor = fCurrentOff+fCurrentLen-1; 
  }
 
}

HOrdCatIter::~HOrdCatIter(void) {
  //Destructor

}

TCollection *HOrdCatIter::GetCollection(void) const {
  return fArray;
}

TObject *HOrdCatIter::Next(void) {
  //Advances to next object in the category and returns the current one.
  // test if setup is done otherwizs do it!

 #if DEBUG_LEVEL>2  
   printf("in HOrdCatIter::Next() cursor: %i current off:%i lenght:%i \n",
             cursor,fCurrentOff,fCurrentLen); 
   printf("n HOrdCatIter::Next() fData:: current pos:%i current len:%i \n", 
              (ordCat)->currentOffset, (ordCat)->currentLenght);
    (*ordCat)->Dump();
 #endif  
   
   if (fDir == kIterForward) {
    
      for ( ; ( cursor < (fCurrentOff+fCurrentLen)) 
               && fArray->fCont[cursor] == 0;
                  (cursor)++) { }

      if (cursor < (fCurrentOff+fCurrentLen)){
         fCurrent= fArray->fCont[(cursor)++];
         return fCurrent; 
      }
   } else {
 
      for ( ; cursor >= fCurrentOff && fArray->fCont[cursor] == 0;
              (cursor)--) { }

      if ( cursor >= fCurrentOff){
         fCurrent = fArray->fCont[(cursor)--];  
         return fCurrent;
      }
   } 
 
 // end 
   return 0;

}

void HOrdCatIter::Reset(void) {
 //Resets the iterator

   ordCat->init(fCurrentLoc);
   fCurrentOff = ordCat->currentOffset; 
   fCurrentLen = ordCat->currentLenght;
   #if DEBUG_LEVEL>2  
   fCurrentLoc->Dump();
   printf(" sec:%i mod:%i \n",fCurrentLoc->operator[](0),
                              fCurrentLoc->operator[](1));
   printf(" Iter::Reset => linear:%i Off:%i Lenght:%i \n", 
           ordCat->currentlinearAddr, 
           ordCat->currentOffset, 
           ordCat->currentLenght);
   #endif

  if (fDir == kIterForward) {
     cursor = fCurrentOff; 
  }else{ 
     cursor = fCurrentOff+fCurrentLen-1;
  }

}

Bool_t HOrdCatIter::gotoLocation(HLocation &loc) {
  // if object exist at a loc[0] position 
  // return kTRUE.
 
  if (loc.getNIndex()==0) {
    Reset();
  } else {
    fCurrent=fArray->At(loc[0]);
    if (!fCurrent) return kFALSE;
    printf("!gotoLocation\n");
  }
 
  return kTRUE;
}

HLocation &HOrdCatIter::getLocation(void) {
  fCurrentLoc->operator[](0)=fArray->IndexOf(fCurrent); 
  return *fCurrentLoc;
}


ClassImp(HOrdCatIter)


  
