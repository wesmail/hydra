//*-- Author : V. Pechenov
using namespace std;
#include <iostream>
#include <iomanip>
#include "hmdclistcells.h"

//////////////////////////////////////////
//
//

////////////////////////////////////////////////////////////

/*void HMdcListCells::initLookups(void) {
  //Initializes lookup tables
  Int_t j=0,shift;
  for (Int_t i=0;i<256;i++) {
    nCellsLookUp[i]=((i & 0x3)?1:0) + (((i>>2) & 0x3)?1:0) + (((i>>4) & 0x3)?1:0) + (((i>>6) & 0x3)?1:0);
    shift=0; j=0;
    while (shift<4) {
      Int_t k=i>>(2*shift);
      if (k & 0x3)
	{ cellPosLookUp[j][i]=shift+0; shift++; }
      else if (k & 0xC)
	{ cellPosLookUp[j][i]=shift+1; shift+=2;}
      else if (k & 0x30)
	{ cellPosLookUp[j][i]=shift+2; shift+=3; }
      else if (k & 0xC0)
	{ cellPosLookUp[j][i]=shift+3; shift+=4; }
      else
	{ cellPosLookUp[j][i]=0; shift+=4; }
      j++;
    }
  }
  lookUpsInitialized=kTRUE;
}*/

void HMdcListGroupCells::print(void) const {
  printf("%i cells:",getNCells());
  Int_t cell=-1;
  while((cell=next(cell)) >= 0) {
    printf(" %i(%i)",cell+1,getTime(cell));
  }
  printf("\n");
}

void HMdcListLayCells::print(void) const {
  printf("%i cells:",getNCells());
  Int_t cell=-1;
  while((cell=next(cell)) >= 0) {
    printf(" %i(%i)",cell+1,getTime(cell));
  }
  printf("\n");
}

void HMdcListCells::print(void) const {
  for(Int_t l=0;l<6;l++) {
    printf("%i layer) ",l+1);
    list[l].print();
  }
}

void HMdcList12GroupCells::print(Int_t lay) const {
  printf("L.%i  %i cells:",lay+1,getNCells(lay));
  Int_t cell=-1;
  while((cell=next(lay,cell)) >= 0) {
    printf(" %i(%i)",cell+1,getTime(lay,cell));
  }
  printf("\n");
}

ClassImp(HMdcListCells)
ClassImp(HMdcListLayCells)
ClassImp(HMdcListGroupCells)
ClassImp(HMdcList12GroupCells)

