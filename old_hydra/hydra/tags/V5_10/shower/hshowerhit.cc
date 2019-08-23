#pragma implementation
#include "hshowerhit.h"
ClassImp(HShowerHit)

//////////////////////////////////////////////////////////////////////
//HShowerHit
//
// Structure for Hit Level
// One object describes one hit in detector
//
////////// **********ShowerHit defintion ****************/////////////77

inline Int_t HShowerHit::clear()
{
   m_fCharge=0.0F;
   m_fX = 0.0;
   m_fY = 0.0;

   m_fSum0 = 0.0;  //sum of charge
   m_fSum1 = 0.0;  //sum of charge
   m_fSum2 = 0.0;  //sum of charge
   m_fSum2_25 = 0.0;  //sum of charge
   m_fSum2_N = 0.0;  //sum of charge
   m_fVar0 = 0.0;  //var. of sum of charge
   m_fVar1 = 0.0;  //var. of sum of charge
   m_fVar2 = 0.0;  //var. of sum of charge
   m_fLocalMax = 0.0; //charge in local max

   m_fSumDiff01 = 0.0;
   m_fSumDiff02 = 0.0;
   m_fSumDiff12 = 0.0;
   m_fShower = 0.0; //shower - diff m_fSum2 - m_fSum1 if > 0

   m_nSector = -1;
   m_lEventNr = -1;
   m_nModule = -1;
   m_nRow = -1;
   m_nCol = -1;
   m_nAddress = -1;
 
   return 1;
}

Int_t HShowerHit::calcAddress() {
  Char_t s = (m_nSector) ? m_nSector : 6;
  m_nAddress = 100000 * s;
  m_nAddress += 10000 * m_nModule;
  m_nAddress += 100 * m_nRow;
  m_nAddress += m_nCol;

  return m_nAddress;
}


inline void HShowerHit::setSum(Int_t i, Float_t fSum) {
  switch(i) {
     case 0: m_fSum0 = fSum;
             break;
 
     case 1: m_fSum1 = fSum;
             break;
 
     case 2: m_fSum2 = fSum;
             break;
 
     default: break;
  }
}

inline void HShowerHit::setVar(Int_t i, Float_t fVar) {
  switch(i) {
     case 0: m_fVar0 = fVar;
             break;

     case 1: m_fVar1 = fVar;
             break;

     case 2: m_fVar2 = fVar;
             break;

     default: break;
  }
}

inline void HShowerHit::setClusterSize(Int_t i, Int_t cs) {
  switch(i) {
     case 0: m_nClusterSize0 = cs;
             break;

     case 1: m_nClusterSize1 = cs;
             break;

     case 2: m_nClusterSize2 = cs;
             break;

     default: break;
  }

}

inline void HShowerHit::setSum25(Float_t fSum) {
    m_fSum2_25 = fSum;
    m_fSum2_N = fSum * 0.36;
}

inline Float_t HShowerHit::getSum(Int_t i) {
  Float_t fSum = -1.0;
  switch(i) {
     case 0: fSum = m_fSum0;
             break;

     case 1: fSum = m_fSum1;
             break;

     case 2: fSum = m_fSum2;
             break;

     default: break;
  }

  return fSum;
}

inline Float_t HShowerHit::getVar(Int_t i) {
  Float_t fVar = -1.0;
  switch(i) {
     case 0: fVar = m_fVar0;
             break;

     case 1: fVar = m_fVar1;
             break;

     case 2: fVar = m_fVar2;
             break;

     default: break;
  }

  return fVar;
}

inline Int_t   HShowerHit::getClusterSize(Int_t i) {
  Int_t cs = -1;
  switch(i) {
     case 0: cs = m_nClusterSize0;
             break;

     case 1: cs = m_nClusterSize1;
             break;

     case 2: cs = m_nClusterSize2;
             break;

     default: break;
  }

  return cs;
}

void HShowerHit::updateCalc() {
   m_fSumDiff01 = m_fSum1 - m_fSum0;
   m_fSumDiff02 = m_fSum2 - m_fSum0;
   m_fSumDiff12 = m_fSum2 - m_fSum1;
}

