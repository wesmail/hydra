#include "hrichgeantcherhit.h"
#include <iostream.h>

//////////////////////////////////////////////////////
// HRichGeantCherHit
//
//   This class contains one hit in one cell of the rich.
//
//////////////////////////////////////////////////////

ClassImp(HRichGeantCherHit)

HRichGeantCherHit::HRichGeantCherHit() {
}

HRichGeantCherHit::~HRichGeantCherHit() {
}

void HRichGeantCherHit::Clear() {
   fXCer = 0.0;
   fYCer = 0.0;
   fECer = 0.0;
   fTrkLength = 0.0;
   nMediumNum = 0;
   nStatFlag = 0;
   nPartrkCer = 0;
   nParentCer = 0;
   nProcesCer = 0;

}


void HRichGeantCherHit::getXY(Float_t* pfX, Float_t* pfY) {
   *pfX = fXCer;
   *pfY = fYCer;
}

Float_t HRichGeantCherHit::getEner() {
   return fECer;
}

Float_t HRichGeantCherHit::getTrkLen() {
   return fTrkLength;
}

Int_t HRichGeantCherHit::getStatFlag() {
   return nStatFlag;
}

void HRichGeantCherHit::setXY(Float_t fX, Float_t fY) {
   fXCer = fX;
   fYCer = fY;
}

void HRichGeantCherHit::setEner(Float_t fEner) {
   fECer = fEner;
}

void HRichGeantCherHit::setTrkLen(Float_t fTrkLen) {
   fTrkLength = fTrkLen;
}

void HRichGeantCherHit::setStatFlag(Int_t nFlag) {
  nStatFlag = nFlag;
}

ostream& operator<< (ostream& output, HRichGeantCherHit& raw) {
//        output<<"pad charge:"<<raw.charge;
        return output;
}    
