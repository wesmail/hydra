#include "hseqarr.h"

//*-- AUTHOR : Pechenov Vladimir
//*-- Modified : 23/06/2004 by V.Pechenov


//_HADES_CLASS_DESCRIPTION 
////////////////////////////////////////////////////////////////
// 
// HSeqArrBlock 
// Classes for creating of huge arraies of sequential type.
// New element is added to the end of array always.
// Access to the existing elements can be rundom but
// optimization done for sequential access.
// 
// HSeqArrBlock  - one block of array, keep 0x40000 bytes.
//
// HSeqArr can be created for keeping words of the next types:
//   Char_t     --> HSeqArr(HSeqArr::kChar) 
//   UChar_t    --> HSeqArr(HSeqArr::kUChar)
//   Short_t    --> HSeqArr(HSeqArr::kShort)
//   UShort_t   --> HSeqArr(HSeqArr::kUShort)
//   Int_t      --> HSeqArr(HSeqArr::kInt)
//   UInt_t     --> HSeqArr(HSeqArr::kUInt)
//   Long64_t   --> HSeqArr(HSeqArr::kLong64)
//   ULong64_t  --> HSeqArr(HSeqArr::kULong64)
//   Float_t    --> HSeqArr(HSeqArr::kFloat)
//   Double_t   --> HSeqArr(HSeqArr::kDouble)
//   Bool_t     --> HSeqArr(HSeqArr::kBool)
// or HSeqArr arr; arr.setArrType(HSeqArr::kChar); ...
// type of array can't be changed
//
// More useful functions:
// Bool_t addElement(void) - adding new element to the end of array
//                           and setting cursor to this element
// Bool_t setCursor(UInt_t ind) - setting cursor to the element number "ind"
//
// void setElement(Char_t e) - set element in cursor position to "e"
// ...
// void getElement(Char_t& e) - get element in cursor position
// 
// UInt_t getArraySize() - number of words in array
// UInt_t getMemSize(void) - size of allocated memory in bytes
//
////////////////////////////////////////////////////////////////

ClassImp(HSeqArrBlock)
ClassImp(HSeqArr)
ClassImp(HSeqArrIter)

HSeqArrBlock::HSeqArrBlock(void) {
  blockNumber=0;
  nextBlock=NULL;
}

HSeqArrBlock::HSeqArrBlock(HSeqArrBlock* p) {
  blockNumber=p->blockNumber+1; 
  nextBlock=NULL;
}
    
HSeqArrBlock::~HSeqArrBlock(void) {
  if(nextBlock) {
    delete nextBlock;
    nextBlock=NULL;
  }
}

Bool_t HSeqArrBlock::expand(void) {
  // Creating of new array block:
  if(nextBlock==NULL) {
    nextBlock=new HSeqArrBlock(this);
    if(nextBlock) return kTRUE;
    Error("expand","Can't allocate memory for the next block of array!");
  }
  return kFALSE;
}

void HSeqArr::init(ESeqArrType tp) {
  size=0;
  firstBlock=new HSeqArrBlock;
  lastBlock=firstBlock;
  if(firstBlock==0) {
    Error("init","Can't allocate memory for the first block of array!");
    return;
  } 
  type=kNoType;
  blockLength=sizeof(HSeqArrBlock);
  nBlocks=1;
  rewind();
  setArrType(tp);
}

HSeqArr::~HSeqArr(void){
  if(firstBlock) {
    delete firstBlock;
    firstBlock=0;
  }
}

Bool_t HSeqArr::setArrType(ESeqArrType tp) {
  if(type!=kNoType) {
    Error("setArrType","Array type already seted!");
    return kFALSE;
  }
  type=tp;
  if(type==kNoType)       wordLength=sizeof(Char_t);
  else if(type==kChar)    wordLength=sizeof(Char_t);
  else if(type==kUChar)   wordLength=sizeof(UChar_t);
  else if(type==kShort)   wordLength=sizeof(Short_t);
  else if(type==kUShort)  wordLength=sizeof(UShort_t);
  else if(type==kInt)     wordLength=sizeof(Int_t);
  else if(type==kUInt)    wordLength=sizeof(UInt_t);
  else if(type==kLong64)  wordLength=sizeof(Long64_t);
  else if(type==kULong64) wordLength=sizeof(ULong64_t);
  else if(type==kFloat)   wordLength=sizeof(Float_t);
  else if(type==kDouble)  wordLength=sizeof(Double_t);
  else if(type==kBool)    wordLength=sizeof(Bool_t);
  return  kTRUE;
}

UInt_t HSeqArr::getMemSize(void) const {
  // return size of allocated memory
  return blockLength*nBlocks+sizeof(HSeqArr);
}

void HSeqArr::rewind(void) {
  block=firstBlock;
  index=0;
  isFrst=kTRUE;
  cElem=block->arr;
}

Bool_t HSeqArr::next(ESeqArrType tp) {
  // Setting cursor to the next word
  if(!tpOk(tp)) return kFALSE;
  if(isFrst) isFrst=kFALSE;
  else {
    if(index+wordLength >= size) return kFALSE;
    index+=wordLength;
    if(HSeqArrBlock::blockIndex(index) > block->blockNumber) {
      block=block->nextBlock;
      cElem=block->arr;
    } else cElem += wordLength;
  }
  return kTRUE;
}

Bool_t HSeqArr::nextAExp(ESeqArrType tp) {
  if(!tpOk(tp)) return kFALSE;
  if(isFrst) isFrst=kFALSE;
  else {
    if(HSeqArrBlock::blockIndex(index+wordLength) > block->blockNumber) {
      if(block->nextBlock==0 && !addBlock()) return kFALSE;
      block=block->nextBlock;
      cElem=block->arr;
    } else cElem += wordLength;
    index+=wordLength;
  }
  if(index>=size) size+=wordLength;
  return kTRUE;
}

Bool_t HSeqArr::addBlock(void) {
  if(!(lastBlock->expand())) return kFALSE;
  lastBlock=lastBlock->nextBlock;
  nBlocks++;
  return kTRUE;
}

Bool_t HSeqArr::setCurrBlock(UInt_t n) {
  // finding array block number n:
  if(n==block->blockNumber) return kTRUE;
  if(n==nBlocks) {
    // adding next block of array:
    if(!addBlock()) return kFALSE;
  } else if(n<block->blockNumber) rewind();
  else if(n>nBlocks) return kFALSE;
  while(n!=block->blockNumber) block=block->nextBlock;
  return kTRUE;
}

Bool_t HSeqArr::addElement(void) {
  // adding new element in array (to the end of array)
  if(!setCurrBlock(HSeqArrBlock::blockIndex(size))) return kFALSE;
  cElem=(block->arr)+HSeqArrBlock::indexInBlock(size);
  index=size;
  size+=wordLength;
  return kTRUE;
}

Bool_t HSeqArr::setCursor(Int_t indArr) {
  // setting of block,index for element number ind
  if(indArr<0) {
    isFrst=kTRUE;
    indArr=0;
  }
  UInt_t ind=indArr*wordLength;
  if(ind==index) return kTRUE;
  if(ind>=size) return kFALSE;
  setCurrBlock(HSeqArrBlock::blockIndex(ind));
  cElem=(block->arr)+HSeqArrBlock::indexInBlock(ind);
  index=ind;
  return kTRUE;
}

Bool_t HSeqArr::err(ESeqArrType tp) {
  if(tp==kNoType) Error("err","Array type is not seted!");
  else Error("err","Mismatching of types!");
  return kFALSE;
}

HSeqArrIter* HSeqArr::makeIterator(void) {
  return new HSeqArrIter(this);
}

HSeqArrIter::HSeqArrIter(HSeqArr* arr) {
  array=arr;
  reset();
}

void HSeqArrIter::reset(void) {
  block=array->firstBlock;
  index=0;
  isFrst=kTRUE;
  cElem=block->arr;
}

Bool_t HSeqArrIter::next(ESeqArrType tp) {
  // Setting cursor to the next word
  if(array->tpOk(tp)) return next(); 
  return kFALSE;
}

Bool_t HSeqArrIter::next(void) {
  // Setting cursor position to the next word
  if(isFrst) isFrst=kFALSE;
  else {
    if(!array->isBoundOk(index+array->wordLength)) return kFALSE;
    index+=array->wordLength;
    if(HSeqArrBlock::blockIndex(index) > block->blockNumber) {
      block=block->nextBlock;
      cElem=block->arr;
    } else cElem += array->wordLength;
  }
  return kTRUE;
}

Bool_t HSeqArrIter::nextAndExpand(void) {
  // Setting cursor position to the next word and expand array if necessary
  if(isFrst) isFrst=kFALSE;
  else {
    if(HSeqArrBlock::blockIndex(index+array->wordLength) > block->blockNumber) {
      if(block->nextBlock==0 && !array->addBlock()) return kFALSE;
      block=block->nextBlock;
      cElem=block->arr;
    } else cElem += array->wordLength;
    index+=array->wordLength;
  }
  if(index>=array->size) array->size += array->wordLength;
  return kTRUE;
}

Bool_t HSeqArrIter::nextAExp(ESeqArrType tp) {
  if(array->tpOk(tp)) return nextAndExpand();
  return kFALSE;
}

Bool_t HSeqArrIter::setCursor(Int_t indArr) {
  // setting of block,index for element number ind
  if(indArr<0) {
    isFrst=kTRUE;
    indArr=0;
  }
  UInt_t ind=indArr*array->wordLength;
  if(ind==index) return kTRUE;
  if(ind>=array->size) return kFALSE;
  UInt_t n=HSeqArrBlock::blockIndex(ind);
  if(n!=block->blockNumber) {
    if(n < block->blockNumber) block=array->firstBlock;
    while(n != block->blockNumber) block=block->nextBlock;
  }
  index=ind;
  cElem=(block->arr)+HSeqArrBlock::indexInBlock(index);
  return kTRUE;
}
