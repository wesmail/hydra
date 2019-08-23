using namespace std;
#include <iostream>
#include <iomanip>
#include "hmdclistcells.h"

//*-- Author : V. Pechenov
//*-- Modified : 25/06/2004 by V.Pechenov

//_HADES_CLASS_DESCRIPTION 
///////////////////////////////////////////////////////////////////////
//  HMdcLayListCells - list of fired wires in one layer
//
//  HMdcLayListCellsAndTimes - HMdcLayListCells + drift times
//
//  HMdcModListCells - list of fired wires in one MDC 
//                  (6 HMdcLayListCells objects)
//
//  HMdcModListCellsAndTimes - HMdcModListCells + drift times
//
//  HMdcSecListCells - list of fired wires in one sector 
//                     (4 HMdcModListCells objects)
//
//  HMdcSecListCellsAndTimes - HMdcSecListCells + drift times
//
//  HMdcEvntListCells - list of fired wires in one sector 
//                       (6 HMdcSecListCells objects)
//
//  HMdcEvntListCellsAndTimes - HMdcEvntListCells + drift times
//
///////////////////////////////////////////////////////////////////////

void HMdcLayListCells::print(void) const {
  printf("%i cells:",getNCells());
  Int_t cell=-1;
  while((cell=next(cell)) >= 0) {
    if(drTimes==0) printf(" %i(%i)",cell+1,getTime(cell));
    else printf(" %i/%.1f",cell+1,drTimes[cell]);
  }
  printf("\n");
}

void HMdcModListCellsAndTimes::setDrTimesArr(void) {
  drTimes6l=times[0];
  for(Int_t l=0;l<6;l++) list[l].setDrTimesArr(times[l]);
}

void HMdcSecListCellsAndTimes::setDrTimesArr(void) {
  drTimes4m=times[0][0];
  for(Int_t m=0;m<4;m++) for(Int_t l=0;l<6;l++) 
    list[m][l].setDrTimesArr(times[m][l]);
}

void HMdcEvntListCellsAndTimes::setDrTimesArr(void) {
  drTimes6s=times[0][0][0];
  for(Int_t s=0;s<6;s++) for(Int_t m=0;m<4;m++) for(Int_t l=0;l<6;l++) 
    list[s][m][l].setDrTimesArr(times[s][m][l]);
}

void HMdcModListCells::print(void) const {
  for(Int_t l=0;l<6;l++) {
    if(list[l].getFirstCell()<0) continue;
    printf(" %i layer:",l+1);
    list[l].print();
  }
}

void HMdcSecListCells::print(void) const {
  for(Int_t m=0;m<4;m++) {
    printf(" ---- %imod.:  %i cells in %i layers\n",
        m+1,list[m].getNCells(),list[m].getNLayers());
    list[m].print();
  }
}

void HMdcEvntListCells::print(void) const {
  printf("==== Event: %i cells ===============================\n",getNCells());
  for(Int_t s=0;s<6;s++) {
    printf("%i sector:  %i cells in %i layers \n",
        s+1,list[s].getNCells(),list[s].getNLayers());
    if(list[s].getNCells()) list[s].print();
  }
}

ClassImp(HMdcLayListCells)
ClassImp(HMdcLayListCellsAndTimes)
ClassImp(HMdcModListCells)
ClassImp(HMdcModListCellsAndTimes)
ClassImp(HMdcSecListCells)
ClassImp(HMdcSecListCellsAndTimes)
ClassImp(HMdcEvntListCells)
ClassImp(HMdcEvntListCellsAndTimes)
