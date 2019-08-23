#ifndef HMDCTBARRAY_H
#define HMDCTBARRAY_H

#include "Rtypes.h"

class HMdcTBArray {
  protected:
    static UChar_t nTBitLookUp[256];         
    static UChar_t pTBitLookUp[4][256];
    static UChar_t nextTBit[4];
    static UChar_t prevTBit[4];
  public: 
    inline static void clear(UChar_t *pF, UChar_t *pL);
    inline static void set(UChar_t *, Int_t pos, UChar_t cont);
    inline static UChar_t get(UChar_t *, Int_t pos); 
    inline static void unset(UChar_t *, Int_t pos, UChar_t cont);
    inline static Int_t getNSet(UChar_t *pF, UChar_t *pL); 
    inline static Int_t position(UChar_t *pF, UChar_t *pL, Int_t idx);
    inline static Int_t first(UChar_t *pF, UChar_t *pL);
    inline static Int_t last(UChar_t *pF, UChar_t *pL);
    inline static Int_t next(UChar_t *pF, UChar_t *pL, Int_t prPos);
    inline static Int_t previous(UChar_t *pF, UChar_t *pL, Int_t prPos);
    inline static void shiftRight(UChar_t *pF, UChar_t *pL, UChar_t *pS);
    inline static void shiftLeft(UChar_t *pF, UChar_t *pL, UChar_t *pS);
    inline static Int_t compare(UChar_t *pF1, UChar_t *pF2, Int_t nBytes);

  ClassDef(HMdcTBArray,0) //A group of cells in layers
};

class HMdcBArray {
  protected:
    static UChar_t nBitLookUp[256];
    static UChar_t setBitLUp[8];
    static UChar_t unsetBitLUp[8];
    static UChar_t pBitLookUp[8][256];
    static UChar_t nextBit[8];
    static UChar_t prevBit[8];
  public: 
    inline static void clear(UChar_t *pF, UChar_t *pL);
    inline static void set(UChar_t *, Int_t pos);
    inline static UChar_t get(UChar_t *, Int_t pos); 
    inline static void unset(UChar_t *, Int_t pos);
    inline static Bool_t testAndUnset(UChar_t *, Int_t pos); 
    inline static Int_t getNSet(UChar_t *pF, UChar_t *pL);
    inline static Int_t getNSet(UChar_t *pF);   //One byte
    inline static Int_t getNSet(UChar_t byte);   //One byte
    inline static Int_t getNSet2B(UChar_t *pF);   //Two bytes
    inline static Int_t position(UChar_t *pF, UChar_t *pL, Int_t idx);
    inline static Int_t first(UChar_t *pF, UChar_t *pL);
    inline static Int_t last(UChar_t *pF, UChar_t *pL);
    inline static Int_t next(UChar_t *pF, UChar_t *pL, Int_t prPos);
    inline static Int_t nextAndUnset(UChar_t *pF, UChar_t *pL, Int_t prPos);
    inline static Int_t previous(UChar_t *pF, UChar_t *pL, Int_t prPos);
    inline static void shiftRight(UChar_t *pF, UChar_t *pL, UChar_t *pS);
    inline static void shiftLeft(UChar_t *pF, UChar_t *pL, UChar_t *pS);
    inline static Int_t compare(UChar_t *pF1, UChar_t *pF2, Int_t nBytes);

    ClassDef(HMdcBArray,0) //A group of cells in layers
};

//----------------------- Inlines HMdcTBarray------------------------------
void HMdcTBArray::clear(UChar_t *pF, UChar_t *pL) {
  while( pF<=pL ) *(pF++)=0;
}

Int_t HMdcTBArray::getNSet(UChar_t *pF, UChar_t *pL) {
  Int_t nTBits=0;
  while(pF <= pL) nTBits+=nTBitLookUp[*(pF++)];
  return nTBits;
}

void HMdcTBArray::set(UChar_t *pF, Int_t pos, UChar_t cont) {
  pF[pos>>2] |= (cont & 0x3) << (pos&3)*2;
}

void HMdcTBArray::unset(UChar_t *pF, Int_t pos, UChar_t cont) {
  pF[pos>>2] &= ~(( cont & 0x3 ) << (pos&3)*2);
}

Int_t HMdcTBArray::position(UChar_t *pF, UChar_t *pL, Int_t idx) {
  // return: >=0 - ok.!;
  if(idx<0) return -1;
  for(UChar_t *pB=pF; pB<=pL; pB++) {
    Int_t idn=idx-nTBitLookUp[*pB];
    if( idn < 0 ) return ((pB-pF)<<2)+pTBitLookUp[idx][*pB];
    idx=idn;
  }
  return -1;
}

Int_t HMdcTBArray::first(UChar_t *pF, UChar_t *pL) {
  // return: -1 - num.cells=0
  for(UChar_t *pB=pF; pB<=pL; pB++) {
     if( *pB > 0 ) return ((pB-pF)<<2)+pTBitLookUp[0][*pB];
  }
  return -1;
}

Int_t HMdcTBArray::last(UChar_t *pF, UChar_t *pL) {
    // return: >=0 - ok.!; -2 - num. of cells =0
  for(UChar_t *pB=pL; pB>=pF; pB--) {
    if(*pB > 0) return ((pB-pF)<<2)+pTBitLookUp[nTBitLookUp[*pB]-1][*pB];
  }
  return -1;
}

Int_t HMdcTBArray::next(UChar_t *pF, UChar_t *pL, Int_t prPos) {
  // prPos <0 eq. first
  // return: -1 next is absent
  UChar_t *pB=pF;
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

Int_t HMdcTBArray::previous(UChar_t *pF, UChar_t *pL, Int_t prPos) {
  // prPos >size eq. last
  // return: -1 previous is absent
  if(prPos<0) return -1;
  UChar_t *pB=pL;
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

void HMdcTBArray::shiftRight(UChar_t *pF, UChar_t *pL, UChar_t *pS) {
  // pF - pointer to first el., pL - pointer to last el.
  // pS - pointer to el. which must be moved to *pL ((pL-pS) - shift)
  while( pS>=pF ) *(pL--)=*(pS--);
  clear(pF, pL);
}

void HMdcTBArray::shiftLeft(UChar_t *pT, UChar_t *pL, UChar_t *pS) {
  // pT - pointer to first el., pL - pointer to last el.
  // pS - pointer to el. which must be moved to *pT
  while( pS<=pL ) *(pT++)=*(pS++);
  clear(pT, pL);
}

UChar_t HMdcTBArray::get(UChar_t *pF, Int_t pos) {
  //Returns 1 for time1, 2 for time2 and 3 for tim1+tim2 in cell
  return (pF[pos>>2] >> (pos&3)*2) &  0x3;
}

inline Int_t HMdcTBArray::compare(UChar_t *pF1,UChar_t *pF2,Int_t nBytes) {
  Int_t nTBits=0;
  while((nBytes--)>0) nTBits+=nTBitLookUp[*(pF1++) & *(pF2++)];
  return nTBits;
}
//----------------------- Inlines HMdcBArray------------------------------
void HMdcBArray::clear(UChar_t *pF, UChar_t *pL) {
  while( pF<=pL ) *(pF++)=0;
}

Int_t HMdcBArray::getNSet(UChar_t *pF, UChar_t *pL) {
  Int_t nBits=0;
  while(pF <= pL) nBits+=nBitLookUp[*(pF++)];
  return nBits;
}

Int_t HMdcBArray::getNSet(UChar_t *pF) {
  return nBitLookUp[*pF];
}

Int_t HMdcBArray::getNSet(UChar_t byte) {
  return nBitLookUp[byte];
}

Int_t HMdcBArray::getNSet2B(UChar_t *pF) {
  return nBitLookUp[*pF]+nBitLookUp[*(pF+1)];
}

void HMdcBArray::set(UChar_t *pF, Int_t pos) {
  pF[pos>>3] |= setBitLUp[pos&7];
}

void HMdcBArray::unset(UChar_t *pF, Int_t pos) {
  pF[pos>>3] &= unsetBitLUp[pos&7];
}

Int_t HMdcBArray::position(UChar_t *pF, UChar_t *pL, Int_t idx) {
  // return: >=0 - ok.!;
  if(idx<0) return -1;
  for(UChar_t *pB=pF; pB<=pL; pB++) {
    Int_t idn=idx-nBitLookUp[*pB];
    if( idn < 0 ) return ((pB-pF)<<3)+pBitLookUp[idx][*pB];
    idx=idn;
  }
  return -1;
}

Int_t HMdcBArray::first(UChar_t *pF, UChar_t *pL) {
  // return: -1 - num.cells=0
  for(UChar_t *pB=pF; pB<=pL; pB++) {
     if( *pB > 0 ) return ((pB-pF)<<3)+pBitLookUp[0][*pB];
  }
  return -1;
}

Int_t HMdcBArray::last(UChar_t *pF, UChar_t *pL) {
    // return: >=0 - ok.!; -2 - num. of cells =0
  for(UChar_t *pB=pL; pB>=pF; pB--) {
    if(*pB > 0) return ((pB-pF)<<3)+pBitLookUp[nBitLookUp[*pB]-1][*pB];
  }
  return -1;
}

Int_t HMdcBArray::next(UChar_t *pF, UChar_t *pL, Int_t prPos) {
  // prPos <0 eq. first
  // return: -1 next is absent
  UChar_t *pB=pF;
  UChar_t next=*pB;
  if(prPos >=0 ) {
    pB+=prPos>>3;
    if(pB>pL) return -1;
    next=*pB & nextBit[prPos&7];
  }
  for(; pB<=pL; next=*(++pB)) {
     if( next ) return ((pB-pF)<<3)+pBitLookUp[0][next];
  }
  return -1;
}

Int_t HMdcBArray::nextAndUnset(UChar_t *pF, UChar_t *pL, Int_t prPos) {
  // The bit prPos must be unseted (=0) before it !!!
  // prPos <0 eq. first
  // return: -1 next is absent
  // the bit will unseted
  for(UChar_t *pB=pF+(prPos>>3); pB<=pL; pB++) {
    if(*pB) {
      UChar_t bitN=pBitLookUp[0][*pB];
      *pB &= unsetBitLUp[bitN];   // Bin unsetting
      return ((pB-pF)<<3)+bitN;
    }
  }
  return -1;
}

Int_t HMdcBArray::previous(UChar_t *pF, UChar_t *pL, Int_t prPos) {
  // prPos >size eq. last
  // return: -1 previous is absent
  if(prPos<0) return -1;
  UChar_t *pB=pL;
  UChar_t next=*pB;
  if((prPos>>3)+pF <= pL) {
    pB=pF+(prPos>>3);
    next=*pB & prevBit[prPos&7];
  }
  for(; pB>=pF; next=*(--pB)) {
    if(next) return ((pB-pF)<<3)+pBitLookUp[nBitLookUp[next]-1][next];
  }
  return -1;
}

void HMdcBArray::shiftRight(UChar_t *pF, UChar_t *pL, UChar_t *pS) {
  // pF - pointer to first el., pL - pointer to last el.
  // pS - pointer to el. which must be moved to *pL ((pL-pS) - shift)
  while( pS>=pF ) *(pL--)=*(pS--);
  clear(pF, pL);
}

void HMdcBArray::shiftLeft(UChar_t *pT, UChar_t *pL, UChar_t *pS) {
  // pT - pointer to first el., pL - pointer to last el.
  // pS - pointer to el. which must be moved to *pT
  while( pS<=pL ) *(pT++)=*(pS++);
  clear(pT, pL);
}

UChar_t HMdcBArray::get(UChar_t *pF, Int_t pos) {
  //Returns 1 if bit is seted, 0 if not
  return (pF[pos>>3] >> (pos&7)) & 1;
}

Bool_t HMdcBArray::testAndUnset(UChar_t *pF, Int_t pos) {
  // Bit pos will unseted. 
  // Returns kTRUE if bit was seted, kFALSE if not
  UChar_t mask=1<<(pos&7);
  pF+=(pos>>3);             // eg. pos/8
  if(*pF & mask) {
    *pF &= ~mask;
    return kTRUE;
  }
  return kFALSE;
}

inline Int_t HMdcBArray::compare(UChar_t *pF1,UChar_t *pF2,Int_t nBytes) {
  Int_t nBits=0;
  while((nBytes--)>0) nBits+=nBitLookUp[*(pF1++) & *(pF2++)];
  return nBits;
}


#endif
