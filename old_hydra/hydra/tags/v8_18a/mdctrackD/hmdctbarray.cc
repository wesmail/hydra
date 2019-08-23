//*-- Author : V. Pechenov
#include "hmdctbarray.h"

//_HADES_CLASS_DESCRIPTION 
//////////////////////////////////////////////////////////////////////
// HMdcTBArray
//
//  LookupTable and some useful functions for working
//  with an array of two bits. 
//  For MDC it can be used for keeping information about
//  number of cell times:
// only time1 - 1
// only time2 - 2
// time1 & time2 - 3
//
//   void clear(UChar_t *pF, UChar_t *pL);
//   void set(UChar_t *pF, Int_t pos, UChar_t cont);
//   Int_t get(UChar_t *pF, Int_t pos);
//   void unset(UChar_t *pF, Int_t pos, UChar_t cont);
//   Int_t getNSet(UChar_t *pF, UChar_t *pL);
//   Int_t position(UChar_t *pF, UChar_t *pL, Int_t idx);
//        = content of two bits or =-1
//   Int_t first(UChar_t *pF, UChar_t *pL);
//        = content of two bits or =-1
//   Int_t last(UChar_t *pF, UChar_t *pL);
//        = content of two bits or =-1
//   Int_t next(UChar_t *pF, UChar_t *pL, Int_t prPos);
//        = content of two bits or =-1
//   Int_t previous(UChar_t *pF, UChar_t *pL, Int_t prPos);
//        = content of two bits or =-1
//   void shiftRight(UChar_t *pF, UChar_t *pL, UChar_t *pS);
//   void shiftLeft(UChar_t *pF, UChar_t *pL, UChar_t *pS);
//   Int_t compare(UChar_t *pF1, UChar_t *pF2, Int_t nBytes);
// 
// pF - pointer to the first byte of bytes array
// pL - pointer to the last byte of bytes array
// cont - content of two bits, can be = 0,1,2,3
// pos - position of element in two bit array (for examp. - num. cell MDC)
// idx - index of element
// prPos - position of the element, used for geting position of the
//         next or previous position aftre or before pos. prPos
// pS - pointer to the byte which mast be moved to *pL (shift to right)
//                                        or to *pT (shift to left)
// 
//////////////////////////////////////////////////////////////////////  


UChar_t HMdcTBArray::nextTBit[4]={0xFC,0xF0,0xC0,0x00};
UChar_t HMdcTBArray::prevTBit[4]={0x00,0x03,0x0F,0x3F};
UChar_t HMdcTBArray::nTBitLookUp[256]={
    0,1,1,1,1,2,2,2,1,2,2,2,1,2,2,2,1,2,2,2,2,3,3,3,2,3,3,3,2,3,3,3,
    1,2,2,2,2,3,3,3,2,3,3,3,2,3,3,3,1,2,2,2,2,3,3,3,2,3,3,3,2,3,3,3,
    1,2,2,2,2,3,3,3,2,3,3,3,2,3,3,3,2,3,3,3,3,4,4,4,3,4,4,4,3,4,4,4,
    2,3,3,3,3,4,4,4,3,4,4,4,3,4,4,4,2,3,3,3,3,4,4,4,3,4,4,4,3,4,4,4,
    1,2,2,2,2,3,3,3,2,3,3,3,2,3,3,3,2,3,3,3,3,4,4,4,3,4,4,4,3,4,4,4,
    2,3,3,3,3,4,4,4,3,4,4,4,3,4,4,4,2,3,3,3,3,4,4,4,3,4,4,4,3,4,4,4,
    1,2,2,2,2,3,3,3,2,3,3,3,2,3,3,3,2,3,3,3,3,4,4,4,3,4,4,4,3,4,4,4,
    2,3,3,3,3,4,4,4,3,4,4,4,3,4,4,4,2,3,3,3,3,4,4,4,3,4,4,4,3,4,4,4};
UChar_t HMdcTBArray::pTBitLookUp[4][256]={
   {0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,2,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,
    2,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,2,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,
    3,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,2,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,
    2,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,2,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,
    3,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,2,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,
    2,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,2,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,
    3,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,2,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,
    2,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,2,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0},
   {0,0,0,0,0,1,1,1,0,1,1,1,0,1,1,1,0,2,2,2,2,1,1,1,2,1,1,1,2,1,1,1,
    0,2,2,2,2,1,1,1,2,1,1,1,2,1,1,1,0,2,2,2,2,1,1,1,2,1,1,1,2,1,1,1,
    0,3,3,3,3,1,1,1,3,1,1,1,3,1,1,1,3,2,2,2,2,1,1,1,2,1,1,1,2,1,1,1,
    3,2,2,2,2,1,1,1,2,1,1,1,2,1,1,1,3,2,2,2,2,1,1,1,2,1,1,1,2,1,1,1,
    0,3,3,3,3,1,1,1,3,1,1,1,3,1,1,1,3,2,2,2,2,1,1,1,2,1,1,1,2,1,1,1,
    3,2,2,2,2,1,1,1,2,1,1,1,2,1,1,1,3,2,2,2,2,1,1,1,2,1,1,1,2,1,1,1,
    0,3,3,3,3,1,1,1,3,1,1,1,3,1,1,1,3,2,2,2,2,1,1,1,2,1,1,1,2,1,1,1,
    3,2,2,2,2,1,1,1,2,1,1,1,2,1,1,1,3,2,2,2,2,1,1,1,2,1,1,1,2,1,1,1},
   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,0,2,2,2,0,2,2,2,
    0,0,0,0,0,2,2,2,0,2,2,2,0,2,2,2,0,0,0,0,0,2,2,2,0,2,2,2,0,2,2,2,
    0,0,0,0,0,3,3,3,0,3,3,3,0,3,3,3,0,3,3,3,3,2,2,2,3,2,2,2,3,2,2,2,
    0,3,3,3,3,2,2,2,3,2,2,2,3,2,2,2,0,3,3,3,3,2,2,2,3,2,2,2,3,2,2,2,
    0,0,0,0,0,3,3,3,0,3,3,3,0,3,3,3,0,3,3,3,3,2,2,2,3,2,2,2,3,2,2,2,
    0,3,3,3,3,2,2,2,3,2,2,2,3,2,2,2,0,3,3,3,3,2,2,2,3,2,2,2,3,2,2,2,
    0,0,0,0,0,3,3,3,0,3,3,3,0,3,3,3,0,3,3,3,3,2,2,2,3,2,2,2,3,2,2,2,
    0,3,3,3,3,2,2,2,3,2,2,2,3,2,2,2,0,3,3,3,3,2,2,2,3,2,2,2,3,2,2,2},
   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,3,3,0,3,3,3,0,3,3,3,
    0,0,0,0,0,3,3,3,0,3,3,3,0,3,3,3,0,0,0,0,0,3,3,3,0,3,3,3,0,3,3,3,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,3,3,0,3,3,3,0,3,3,3,
    0,0,0,0,0,3,3,3,0,3,3,3,0,3,3,3,0,0,0,0,0,3,3,3,0,3,3,3,0,3,3,3,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,3,3,0,3,3,3,0,3,3,3,
    0,0,0,0,0,3,3,3,0,3,3,3,0,3,3,3,0,0,0,0,0,3,3,3,0,3,3,3,0,3,3,3}};
    
UChar_t HMdcBArray::setBitLUp[8]={1,2,4,8,16,32,64,128};
UChar_t HMdcBArray::unsetBitLUp[8]={254,253,251,247,239,223,191,127};
UChar_t HMdcBArray::nextBit[8]={254,252,248,240,224,192,128,0};
UChar_t HMdcBArray::prevBit[8]={0,1,3,7,15,31,63,127};
UChar_t HMdcBArray::nBitLookUp[256]={
    0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4,1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,
    1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
    1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
    2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
    1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
    2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
    2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
    3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,4,5,5,6,5,6,6,7,5,6,6,7,6,7,7,8};
    
UChar_t HMdcBArray::pBitLookUp[8][256]={
   {0,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
    5,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
    6,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
    5,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
    7,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
    5,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
    6,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,
    5,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0,4,0,1,0,2,0,1,0,3,0,1,0,2,0,1,0},
   {0,0,0,1,0,2,2,1,0,3,3,1,3,2,2,1,0,4,4,1,4,2,2,1,4,3,3,1,3,2,2,1,
    0,5,5,1,5,2,2,1,5,3,3,1,3,2,2,1,5,4,4,1,4,2,2,1,4,3,3,1,3,2,2,1,
    0,6,6,1,6,2,2,1,6,3,3,1,3,2,2,1,6,4,4,1,4,2,2,1,4,3,3,1,3,2,2,1,
    6,5,5,1,5,2,2,1,5,3,3,1,3,2,2,1,5,4,4,1,4,2,2,1,4,3,3,1,3,2,2,1,
    0,7,7,1,7,2,2,1,7,3,3,1,3,2,2,1,7,4,4,1,4,2,2,1,4,3,3,1,3,2,2,1,
    7,5,5,1,5,2,2,1,5,3,3,1,3,2,2,1,5,4,4,1,4,2,2,1,4,3,3,1,3,2,2,1,
    7,6,6,1,6,2,2,1,6,3,3,1,3,2,2,1,6,4,4,1,4,2,2,1,4,3,3,1,3,2,2,1,
    6,5,5,1,5,2,2,1,5,3,3,1,3,2,2,1,5,4,4,1,4,2,2,1,4,3,3,1,3,2,2,1},
   {0,0,0,0,0,0,0,2,0,0,0,3,0,3,3,2,0,0,0,4,0,4,4,2,0,4,4,3,4,3,3,2,
    0,0,0,5,0,5,5,2,0,5,5,3,5,3,3,2,0,5,5,4,5,4,4,2,5,4,4,3,4,3,3,2,
    0,0,0,6,0,6,6,2,0,6,6,3,6,3,3,2,0,6,6,4,6,4,4,2,6,4,4,3,4,3,3,2,
    0,6,6,5,6,5,5,2,6,5,5,3,5,3,3,2,6,5,5,4,5,4,4,2,5,4,4,3,4,3,3,2,
    0,0,0,7,0,7,7,2,0,7,7,3,7,3,3,2,0,7,7,4,7,4,4,2,7,4,4,3,4,3,3,2,
    0,7,7,5,7,5,5,2,7,5,5,3,5,3,3,2,7,5,5,4,5,4,4,2,5,4,4,3,4,3,3,2,
    0,7,7,6,7,6,6,2,7,6,6,3,6,3,3,2,7,6,6,4,6,4,4,2,6,4,4,3,4,3,3,2,
    7,6,6,5,6,5,5,2,6,5,5,3,5,3,3,2,6,5,5,4,5,4,4,2,5,4,4,3,4,3,3,2},
   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,4,0,0,0,4,0,4,4,3,
    0,0,0,0,0,0,0,5,0,0,0,5,0,5,5,3,0,0,0,5,0,5,5,4,0,5,5,4,5,4,4,3,
    0,0,0,0,0,0,0,6,0,0,0,6,0,6,6,3,0,0,0,6,0,6,6,4,0,6,6,4,6,4,4,3,
    0,0,0,6,0,6,6,5,0,6,6,5,6,5,5,3,0,6,6,5,6,5,5,4,6,5,5,4,5,4,4,3,
    0,0,0,0,0,0,0,7,0,0,0,7,0,7,7,3,0,0,0,7,0,7,7,4,0,7,7,4,7,4,4,3,
    0,0,0,7,0,7,7,5,0,7,7,5,7,5,5,3,0,7,7,5,7,5,5,4,7,5,5,4,5,4,4,3,
    0,0,0,7,0,7,7,6,0,7,7,6,7,6,6,3,0,7,7,6,7,6,6,4,7,6,6,4,6,4,4,3,
    0,7,7,6,7,6,6,5,7,6,6,5,6,5,5,3,7,6,6,5,6,5,5,4,6,5,5,4,5,4,4,3},
   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,0,0,0,0,0,0,0,5,0,0,0,5,0,5,5,4,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,0,0,0,0,0,0,0,6,0,0,0,6,0,6,6,4,
    0,0,0,0,0,0,0,6,0,0,0,6,0,6,6,5,0,0,0,6,0,6,6,5,0,6,6,5,6,5,5,4,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,0,7,0,0,0,7,0,7,7,4,
    0,0,0,0,0,0,0,7,0,0,0,7,0,7,7,5,0,0,0,7,0,7,7,5,0,7,7,5,7,5,5,4,
    0,0,0,0,0,0,0,7,0,0,0,7,0,7,7,6,0,0,0,7,0,7,7,6,0,7,7,6,7,6,6,4,
    0,0,0,7,0,7,7,6,0,7,7,6,7,6,6,5,0,7,7,6,7,6,6,5,7,6,6,5,6,5,5,4},
   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,0,0,0,0,0,0,0,6,0,0,0,6,0,6,6,5,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,0,7,0,0,0,7,0,7,7,5,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,0,7,0,0,0,7,0,7,7,6,
    0,0,0,0,0,0,0,7,0,0,0,7,0,7,7,6,0,0,0,7,0,7,7,6,0,7,7,6,7,6,6,5},
   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,0,7,0,0,0,7,0,7,7,6},
   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7}};

UChar_t HMdcBArray::highBit[256]={    
    0,0,1,1,2,2,2,2,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
    5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
    6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
    6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
    7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
    7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
    7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
    7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7};
    
UChar_t HMdcBArray::nLayOrientation[64]={
    0,1,1,2,1,2,2,3,1,2,2,3,1,2,2,3,1,2,2,3,2,3,3,4,2,3,3,4,2,3,3,4,
    1,2,2,3,2,3,3,4,2,3,3,4,2,3,3,4,2,3,3,4,3,4,4,5,3,4,4,5,3,4,4,5};

UChar_t HMdcBArray::is40degCross[64]={    
    0,0,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,0,1,1,0,1,1,1,0,1,1,1,0,1,1,1,
    0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};

/*void HMdcTBArray::initLookups(void) {
  //Initializes lookup tables
  Int_t j=0,shift;
  for (Int_t i=0;i<256;i++) {
    nTBitLookUp[i]=((i & 0x3)?1:0) + (((i>>2) & 0x3)?1:0) + 
                                     (((i>>4) & 0x3)?1:0) + 
                                     (((i>>6) & 0x3)?1:0);           
    shift=0; j=0;
    while (shift<4) {
      Int_t k=i>>(2*shift);
      if (k & 0x3)
	{ pTBitLookUp[j][i]=shift+0; shift++; } 
      else if (k & 0xC)
	{ pTBitLookUp[j][i]=shift+1; shift+=2;}
      else if (k & 0x30)
	{ pTBitLookUp[j][i]=shift+2; shift+=3; }
      else if (k & 0xC0)
	{ pTBitLookUp[j][i]=shift+3; shift+=4; }
      else
	{ pTBitLookUp[j][i]=0; shift+=4; }
      j++;
    }
  }
  lookUpsInitialized=kTRUE;
}*/


Int_t HMdcTBArray::next(const UChar_t *pF, const UChar_t *pL, Int_t prPos) {
  // prPos <0 eq. first
  // return -1 if next is absent
  const UChar_t *pB=pF;
  UChar_t next=*pB;
  if(prPos >=0 ) {
    pB+=prPos>>2;
    if(pB>pL) return -1;
    next=*pB & nextTBit[prPos&3];
  }
  for(; pB<=pL; next=*(++pB)) {
     if( next ) return ((pB-pF)<<2)+pTBitLookUp[0][next];
  }
  return -1;
}

UChar_t HMdcTBArray::next2Bits(const UChar_t *pF, const UChar_t *pL, Int_t& pos) {
  // prPos <0 eq. first
  // return 1 for time1, 2 for time2 or 3 for time1 and time2
  // return 0 if next is absent
  const UChar_t *pB=pF;
  UChar_t next=*pB;
  if(pos >=0 ) {
    pB+=pos>>2;
    if(pB>pL) return 0;
    next=*pB & nextTBit[pos&3];
  }
  for(; pB<=pL; next=*(++pB)) if( next ) {
    pos = ((pB-pF)<<2)+pTBitLookUp[0][next];
    return (next >> (pos&3)*2) &  0x3;;
  }
  return 0;
}

Int_t HMdcTBArray::previous(const UChar_t *pF,const  UChar_t *pL,Int_t prPos) {
  // prPos >size eq. last
  // return -1 if previous is absent
  if(prPos<0) return -1;
  const UChar_t *pB=pL;
  UChar_t next=*pB;
  if((prPos>>2)+pF <= pL) {
    pB=pF+(prPos>>2);
    next=*pB & prevTBit[prPos&3];
  }
  for(; pB>=pF; next=*(--pB)) {
    if(next) return ((pB-pF)<<2)+pTBitLookUp[nTBitLookUp[next]-1][next];
  }
  return -1;
}

    
ClassImp(HMdcTBArray)
ClassImp(HMdcBArray)  