using namespace std;
#include <iostream>
#include <iomanip>
#include "hmdclistgroupcells.h"

//*-- Author : V. Pechenov
//*-- Modified : 25/06/2004 by V.Pechenov

///////////////////////////////////////////////////////////////////////
//
//  HMdcList12GroupCells - list groups of cells in 12 layers (2 mdc)
//                         It's base class for HMdcClus
//
//  HMdcList24GroupCells - list groups of cells in 24 layers (4 mdc)
//
///////////////////////////////////////////////////////////////////////

Int_t HMdcList12GroupCells::compare(HMdcList12GroupCells* lst,
    Int_t l1, Int_t l2) const {
  // return the num. of layers which have one or more identical cells
  if(l1>=12) return 0;
  setInBounds(l1,l2);
  Int_t nLay=0;
  for(Int_t lay=l1; lay<=l2; lay++) {
    if(arr[lay*laySz]==0 || lst->arr[lay*laySz]==0) continue;  // no cells in lay.
    Int_t c1,c2;
    if(firstCell[lay] < lst->firstCell[lay]) {
      c1=lst->firstCell[lay];
      c2=firstCell[lay]+48;
    } else {
      c1=firstCell[lay];
      c2=lst->firstCell[lay]+48;
    }
    if(c1>c2) continue;  // no overlaping
    if(HMdcTBArray::compare(arr+lay*laySz+((c1-firstCell[lay])>>2),
       (lst->arr)+lay*laySz+((c1-lst->firstCell[lay])>>2),(c2-c1)>>2)) nLay++;
  }
  return nLay;
}

Int_t HMdcList12GroupCells::nIdentDrTimes(HMdcList12GroupCells* lst,
    Int_t l1, Int_t l2) const {
  // return the num. of identical drift times (cells now)
  if(l1>=12) return 0;
  setInBounds(l1,l2);
  Int_t nICells=0;
  for(Int_t lay=l1; lay<=l2; lay++) {
    if(arr[lay*laySz]==0 || lst->arr[lay*laySz]==0) continue;  // no cells in lay.
    Int_t c1,c2;
    if(firstCell[lay] < lst->firstCell[lay]) {
      c1=lst->firstCell[lay];
      c2=firstCell[lay]+48;
    } else {
      c1=firstCell[lay];
      c2=lst->firstCell[lay]+48;
    }
    if(c1>c2) continue;  // no overlaping
    nICells+=HMdcTBArray::compare(arr+lay*laySz+((c1-firstCell[lay])>>2),
       (lst->arr)+lay*laySz+((c1-lst->firstCell[lay])>>2),(c2-c1)>>2);
  }
  return nICells;
}

Int_t HMdcList12GroupCells::getMaxNCellsPerLay(void) const {
  Int_t maxNum=0;
  for(Int_t lay=0;lay<12;lay++) {
    Int_t nl=getNCells(lay);
    if(nl>maxNum) maxNum=nl;
  }
  return maxNum;
}

Int_t HMdcList12GroupCells::add(HMdcList12GroupCells* lst) {
  // return: 0 - ok.!;  n>0 - num.cells excl.from cl.
  Int_t nLay=0;
  for(Int_t lay=0; lay<12; lay++) {
    UChar_t *lst2=(lst->arr)+lay*laySz;
    if(HMdcTBArray::getNSet(lst2,lst2+11) == 0) continue;
    UChar_t *lst1=arr+lay*laySz;
    if(HMdcTBArray::getNSet(lst1,lst1+11) == 0) {
      for(Int_t n=0; n<laySz; n++) lst1[n] = lst2[n];
      firstCell[lay]=lst->firstCell[lay];
      continue;
    }
    Int_t sing=firstCell[lay] - lst->firstCell[lay];
    if(sing != 0) {
      Int_t shift=((sing<0) ? -sing:sing)>>2;  // == /4
      if(shift>12) shift=12;
      if( sing < 0 ) {
        nLay=HMdcTBArray::getNSet(lst2+laySz-shift,lst2+11); //??? only for message!?
        for(Int_t n=shift; n<laySz; n++) lst1[n] |= lst2[n-shift];
      }
      else if( sing > 0 ){
        nLay=HMdcTBArray::getNSet(lst1+laySz-shift,lst1+11); //??? only for message!?
        for(Int_t n=shift; n<laySz; n++) lst2[n] |= lst1[n-shift];
        for(Int_t n=0; n<laySz; n++) lst1[n] = lst2[n];
        firstCell[lay]=lst->firstCell[lay];
      }
    }
    else for(Int_t n=0; n<laySz; n++) lst1[n] |= lst2[n];
  }
  return nLay;
}

Bool_t HMdcList12GroupCells::isIncluded(HMdcList12GroupCells& lst,
    Int_t l1, Int_t l2) const {
  // return kTRUE if all cells in "this" is included in "lst"
  for(Int_t lay=l1; lay<=l2; lay++) {
   Int_t lsh=lay*laySz;
    const UChar_t* arr1=arr+lsh;
    if((*arr1)==0) continue;
    const UChar_t* arr2=lst.arr+lsh;
    if((*arr2)==0) return kFALSE;
    Int_t shift=(firstCell[lay]-lst.firstCell[lay])>>2;
    if(shift<0 || shift>=laySz) return kFALSE;
    const UChar_t* arr1e2=arr1+laySz;
    const UChar_t* arr1e=arr1e2-shift;
    arr2 += shift;
    for(;arr1<arr1e;arr1++) {
      if(*arr1 && *arr1 != (*arr1 & *arr2)) return kFALSE;
      arr2++;
    }
    if(shift) for(;arr1<arr1e2;arr1++) if(*arr1) return kFALSE;
  }
  return kTRUE;
}

void HMdcList12GroupCells::print(void) const {
  for(Int_t lay=0; lay<12; lay++) print(lay);
}

void HMdcList12GroupCells::print(Int_t lay) const {
  printf("L.%i  %i cells:",lay+1,getNCells(lay));
  Int_t cell=-1;
  while((cell=next(lay,cell)) >= 0) printf(" %i(%i)",cell+1,getTime(lay,cell));
  printf("\n");
}

Int_t HMdcList24GroupCells::compare(HMdcList24GroupCells* lst,
    Int_t l1, Int_t l2) const {
  // return the num. of layers which have one or more identical cells
  if(l1>=24) return 0;
  setInBounds(l1,l2);
  Int_t nLay=0;
  for(Int_t lay=l1; lay<=l2; lay++) {
    if(arr[lay*laySz]==0 || lst->arr[lay*laySz]==0) continue;  // no cells in lay.
    Int_t c1,c2;
    if(firstCell[lay] < lst->firstCell[lay]) {
      c1=lst->firstCell[lay];
      c2=firstCell[lay]+48;
    } else {
      c1=firstCell[lay];
      c2=lst->firstCell[lay]+48;
    }
    if(c1>c2) continue;  // no overlaping
    if(HMdcTBArray::compare(arr+lay*laySz+((c1-firstCell[lay])>>2),
       (lst->arr)+lay*laySz+((c1-lst->firstCell[lay])>>2),(c2-c1)>>2)) nLay++;
  }
  return nLay;
}

Int_t HMdcList24GroupCells::getMaxNCellsPerLay(void) const {
  Int_t maxNum=0;
  for(Int_t lay=0;lay<24;lay++) {
    Int_t nl=getNCells(lay);
    if(nl>maxNum) maxNum=nl;
  }
  return maxNum;
}

Int_t HMdcList24GroupCells::nIdentDrTimes(HMdcList24GroupCells* lst,
    Int_t l1, Int_t l2) const {
  // return the num. of identical drift times (cells now)
  if(l1>=24) return 0;
  setInBounds(l1,l2);
  Int_t nICells=0;
  for(Int_t lay=l1; lay<=l2; lay++) {
    if(arr[lay*laySz]==0 || lst->arr[lay*laySz]==0) continue;  // no cells in lay.
    Int_t c1,c2;
    if(firstCell[lay] < lst->firstCell[lay]) {
      c1=lst->firstCell[lay];
      c2=firstCell[lay]+48;
    } else {
      c1=firstCell[lay];
      c2=lst->firstCell[lay]+48;
    }
    if(c1>c2) continue;  // no overlaping
    nICells+=HMdcTBArray::compare(arr+lay*laySz+((c1-firstCell[lay])>>2),
       (lst->arr)+lay*laySz+((c1-lst->firstCell[lay])>>2),(c2-c1)>>2);
  }
  return nICells;
}

Int_t HMdcList24GroupCells::add(HMdcList24GroupCells* lst) {
  // return: 0 - ok.!;  n>0 - num.cells excl.from cl.
  Int_t nLay=0;
  for(Int_t lay=0; lay<24; lay++) {
    UChar_t *lst2=(lst->arr)+lay*laySz;
    if(HMdcTBArray::getNSet(lst2,lst2+11) == 0) continue;
    UChar_t *lst1=arr+lay*laySz;
    if(HMdcTBArray::getNSet(lst1,lst1+11) == 0) {
      for(Int_t n=0; n<laySz; n++) lst1[n] = lst2[n];
      firstCell[lay]=lst->firstCell[lay];
      continue;
    }
    Int_t sing=firstCell[lay] - lst->firstCell[lay];
    if(sing != 0) {
      Int_t shift=((sing<0) ? -sing:sing)>>2;  // == /4
      if(shift>12) shift=12;
      if( sing < 0 ) {
        nLay=HMdcTBArray::getNSet(lst2+laySz-shift,lst2+11); //??? only for message!?
        for(Int_t n=shift; n<laySz; n++) lst1[n] |= lst2[n-shift];
      } else if( sing > 0 ){
        nLay=HMdcTBArray::getNSet(lst1+laySz-shift,lst1+11); //??? only for message!?
        for(Int_t n=shift; n<laySz; n++) lst2[n] |= lst1[n-shift];
        for(Int_t n=0; n<laySz; n++) lst1[n] = lst2[n];
        firstCell[lay]=lst->firstCell[lay];
      }
    }
    else for(Int_t n=0; n<laySz; n++) lst1[n] |= lst2[n];
  }
  return nLay;
}

Bool_t HMdcList24GroupCells::isIncluded(HMdcList24GroupCells& lst,
    Int_t l1, Int_t l2) const {
  // return kTRUE if all cells in "this" are included in "lst"
  for(Int_t lay=l1; lay<=l2; lay++) {
   Int_t lsh=lay*laySz;
    const UChar_t* arr1=arr+lsh;
    if((*arr1)==0) continue;
    const UChar_t* arr2=lst.arr+lsh;
    if((*arr2)==0) return kFALSE;
    Int_t shift=(firstCell[lay]-lst.firstCell[lay])>>2;
    if(shift<0 || shift>=laySz) return kFALSE;
    const UChar_t* arr1e2=arr1+laySz;
    const UChar_t* arr1e=arr1e2-shift;
    arr2 += shift;
    for(;arr1<arr1e;arr1++) {
      if(*arr1 && *arr1 != (*arr1 & *arr2)) return kFALSE;
      arr2++;
    }
    if(shift) for(;arr1<arr1e2;arr1++) if(*arr1) return kFALSE;
  }
  return kTRUE;
}

void HMdcList24GroupCells::print(void) const {
  for(Int_t lay=0; lay<24; lay++) print(lay);
}

void HMdcList24GroupCells::print(Int_t lay) const {
  printf("L.%i  %i cells:",lay+1,getNCells(lay));
  Int_t cell=-1;
  while((cell=next(lay,cell)) >= 0) printf(" %i(%i)",cell+1,getTime(lay,cell));
  printf("\n");
}
   
ClassImp(HMdcList12GroupCells)
ClassImp(HMdcList24GroupCells)
